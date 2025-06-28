#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_CHAIRS 3
#define NUM_STUDENTS 5

sem_t students_sem;    // 학생들이 TA를 깨우는 용도 (학생이 도착하면 post)
sem_t ta_sem;          // TA가 학생을 도와줄 때 사용 (도움이 끝날 때 post)
pthread_mutex_t chair_mutex; // 의자(대기열) 접근 보호

int waiting_students = 0;
int chairs[NUM_CHAIRS]; // 학생 번호 저장용
int next_seat = 0;      // 다음에 앉을 의자 인덱스
int next_teach = 0;     // TA가 도와줄 의자 인덱스

void *student_thread(void *arg) {
    int id = *(int*)arg;

    while (1) {
        printf("%dth student: do programming assignment\n", id);
        sleep(rand() % 5 + 1); // 프로그래밍 시간 랜덤 대기

        pthread_mutex_lock(&chair_mutex);
        if (waiting_students < NUM_CHAIRS) {
            // 의자에 앉기
            chairs[next_seat] = id;
            waiting_students++;
            printf("%dth student: sat on a chair waiting for the TA to finish.\n", id);
            next_seat = (next_seat + 1) % NUM_CHAIRS;

            // TA가 자고 있으면 깨움
            sem_post(&students_sem);
            pthread_mutex_unlock(&chair_mutex);

            // TA가 도움을 줄 때까지 대기
            sem_wait(&ta_sem);

            printf("%dth student: Help student finished assignment ...\n", id);
        } else {
            // 의자가 꽉 차면 다시 프로그래밍
            printf("%dth student: no available chair, will try later.\n", id);
            pthread_mutex_unlock(&chair_mutex);
        }
        sleep(1); // 잠시 대기 후 다시 시도
    }
    return NULL;
}

void *ta_thread(void *arg) {
    while (1) {
        // 학생이 올 때까지 대기(잠)
        sem_wait(&students_sem);

        pthread_mutex_lock(&chair_mutex);
        if (waiting_students == 0) {
            // 아무도 없으면 계속 잠
            pthread_mutex_unlock(&chair_mutex);
            continue;
        }
        // 의자에서 대기 중인 학생 번호 찾기
        int id = chairs[next_teach];
        printf("TA: Get next student on the chair (student %d)\n", id);
        waiting_students--;
        next_teach = (next_teach + 1) % NUM_CHAIRS;
        pthread_mutex_unlock(&chair_mutex);

        // 학생 도와주기
        printf("TA: Helping student %d...\n", id);
        sleep(rand() % 3 + 2); // 도와주는 시간

        // 학생에게 도움 완료 알림
        sem_post(&ta_sem);
    }
    return NULL;
}

int main() {
    srand(time(NULL));
    pthread_t students[NUM_STUDENTS];
    pthread_t ta;
    int student_ids[NUM_STUDENTS];

    sem_init(&students_sem, 0, 0);
    sem_init(&ta_sem, 0, 0);
    pthread_mutex_init(&chair_mutex, NULL);

    // 학생 스레드 생성
    for (int i = 0; i < NUM_STUDENTS; ++i) {
        student_ids[i] = i;
        pthread_create(&students[i], NULL, student_thread, &student_ids[i]);
    }
    // TA 스레드 생성
    pthread_create(&ta, NULL, ta_thread, NULL);

    // 스레드 종료 대기 (여기선 무한 반복이라 종료 안 됨)
    for (int i = 0; i < NUM_STUDENTS; ++i)
        pthread_join(students[i], NULL);
    pthread_join(ta, NULL);

    // 자원 해제 (실제로 도달하지 않음)
    sem_destroy(&students_sem);
    sem_destroy(&ta_sem);
    pthread_mutex_destroy(&chair_mutex);

    return 0;
}

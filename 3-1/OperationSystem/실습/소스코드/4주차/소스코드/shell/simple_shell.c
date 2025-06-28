#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/* The maximum length command */
#define MAX_LEN 100
#define HISTORY_LEN 5

/* Doubly Circular Linked List 사용 */
typedef struct HistoryNode {
    char command[MAX_LEN];
    struct HistoryNode* prev;
    struct HistoryNode* next;
} HistoryNode;

HistoryNode* history_head = NULL;
int history_count = 0;

void add_history(const char* cmd) {
    HistoryNode* new_node = (HistoryNode*)malloc(sizeof(HistoryNode));
    strcpy(new_node->command, cmd);

    if (history_head == NULL) {
        new_node->next = new_node;
        new_node->prev = new_node;
        history_head = new_node;
        history_count = 1;
        return;
    }

    if (history_count == HISTORY_LEN) {
        printf("Deleted cmd : %s\n", history_head->command);  // 🔥 삭제 메시지 출력
        HistoryNode* to_remove = history_head;
        history_head = history_head->next;
        to_remove->prev->next = to_remove->next;
        to_remove->next->prev = to_remove->prev;
        free(to_remove);
        history_count--;
    }

    HistoryNode* tail = history_head->prev;
    tail->next = new_node;
    new_node->prev = tail;
    new_node->next = history_head;
    history_head->prev = new_node;
    history_count++;
}

void print_history() {
    if (history_head == NULL) {
        printf("히스토리가 없습니다.\n");
        return;
    }

    HistoryNode* curr = history_head;
    int index = 0;
    do {
        printf("%d\t%s\n", index++, curr->command);
        curr = curr->next;
    } while (curr != history_head);
}

void clear_history() {
    if (!history_head) return;
    HistoryNode* curr = history_head->next;
    while (curr != history_head) {
        HistoryNode* next = curr->next;
        free(curr);
        curr = next;
    }
    free(history_head);
    history_head = NULL;
    history_count = 0;
}


int main(void) {https://learning.hanyang.ac.kr/conversations
 	//command line arguments
	char *args[MAX_LEN / 2 + 1];

	int should_run = 1;	/* flag to determine when to exit program */
	int background = 0;

	while( should_run ) {
		printf("myshell> ");
		fflush(stdout);
		
		/* A부터 F까지 요구사항 만족할 수 있도록 구현하기 */
		// A - 문자열 입력받기 fgets 사용
		char *input = (char*)malloc(MAX_LEN*sizeof(char));
		fgets(input, MAX_LEN, stdin);
		
		// B - 문자열을 \n 단위로 쪼개기 strtok 사용
		char *s[MAX_LEN];
		char *p = strtok(input, "\n");
		if (p == NULL) goto end_time;
		
		 // C - 쪼개진 문자열 비교 strcmp 사용
        int i = 0;
        char *token = strtok(input, " ");
        while (token != NULL) {
            if (strcmp(token, "&") == 0) {
                background = 1;
                break;
            }
            args[i] = token;
            i++;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        if (args[0] == NULL) continue;
		
		// E - directory 이동 - chdir() 사용
		if (strcmp(args[0], "cd") == 0) {
            if (args[1] == NULL) {
                printf("cd: 인자가 필요합니다.\n");
            } else {
                if (chdir(args[1]) == 0) {
                    printf("SUCCESS\n");
                } else {
                    perror("FAIL");
                }
            }
            add_history(input);
            free(input);
            continue;
        }

		// F - history 구현 (터미널에서 실행한 명령어 기록 확인)
		if (strcmp(args[0], "history") == 0) {
			print_history();
			add_history(input);
			continue;
		}
		if (strcmp(input, "exit") == 0) {
            printf("Delete cmd history list :\n");
            print_history();
            printf("Finish!\n");
            clear_history();
            break;
        }

        add_history(input);

		// D - fork() 진행 (현재 process의 자식 프로세스 생성)
		pid_t pid = fork();
        if (pid < 0) {
            perror("fork 실패");
            free(input);
            continue;
        } else if (pid == 0) {
            execvp(args[0], args);
            perror("실행 실패");
            exit(1);
        } else {
            if (background) {
                printf("background process\n");
            } else {
                printf("waiting for child, not a background process\n");
                waitpid(pid, NULL, 0);
                printf("child process complete\n");
            }
        }

        free(input);
	}
	end_time:
		printf("HERE is the END.");
	return 0;
}



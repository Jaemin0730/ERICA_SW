#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define DATA_SIZE 16

struct range {
	char *t_name;
	int index;
	int end;
	int *data;
};

/* TODO 7: Merge sort */
void merge(int *data, int left, int mid, int right) {
	int i, j, k;
	int n1 = mid - left + 1;
	int n2 = right - mid;
	
	int *L = (int *)malloc(n1 * sizeof(int));
	int *R = (int *)malloc(n2 * sizeof(int));
	
	for (i = 0; i < n1; i++)
		L[i] = data[left + i];
	for (j = 0; j < n2; j++)
		R[j] = data[mid + 1 + j];
		
	i = 0;
	j = 0;
	k = left;
	
	while (i < n1 && j < n2) {
		if (L[i] <= R[j]) {
			data[k] = L[i];
			i++;
		} else {
			data[k] = R[j];
			j++;
		}
		k++;
	}
	
	while (i < n1) {
		data[k] = L[i];
		i++;
		k++;
	}
	
	while (j < n2) {
		data[k] = R[j];
		j++;
		k++;
	}
	
	free(L);
	free(R);
}

void merge_sort(int *data, int left, int right) {
	if (left < right) {
		int mid = left + (right - left) / 2;
		merge_sort(data, left, mid);
		merge_sort(data, mid + 1, right);
		merge(data, left, mid, right);
	}
}

void *sort_thread(void *arg) {
	struct range *r = (struct range *)arg;
	printf("%s: sorting from index %d to %d\n", r->t_name, r->index, r->end);
	merge_sort(r->data, r->index, r->end);
	return NULL;
}

void *merge_thread(void *arg) {
	struct range *r = (struct range *)arg;
	printf("%s: merging from index %d to %d\n", r->t_name, r->index, r->end);
	merge(r->data, 0, r->index, r->end);
	return NULL;
}

int main(void) {
	int thr_id;
	pthread_t tid[3];
	int data[DATA_SIZE] = {5, 16, 4, 7, 1, 3, 2, 6, 8, 13, 11, 9, 10, 12, 15, 14};

	struct range first_half, second_half, merge_range;
	int mid = DATA_SIZE / 2;

	first_half.t_name = "thread_1";
	first_half.index = 0;
	first_half.end = mid;
	first_half.data = data;

	second_half.t_name = "thread_2";
	second_half.index = mid + 1;
	second_half.end = DATA_SIZE - 1;
	second_half.data = data;

	merge_range.t_name = "thread_3";
	merge_range.index = mid;
	merge_range.end = DATA_SIZE - 1;
	merge_range.data = data;

	/* TODO 1: 1st Thread - Sort first half of data */
	thr_id = pthread_create(&tid[0], NULL, sort_thread, &first_half);
	if( thr_id < 0 ) {
		perror("Thread create error : ");
		exit(0);
	}

	/* TODO 2: 2st Thread - Sort second half of data */
	thr_id = pthread_create(&tid[1], NULL, sort_thread, &second_half);
	if( thr_id < 0 ) {
		perror("Thread create error : ");
		exit(0);
	}

	/* TODO 3: 3rd Thread - Merge the result of two halfs */
	thr_id = pthread_create(&tid[2], NULL, merge_thread, &merge_range);
	if( thr_id < 0 ) {
		perror("Thread create error : ");
		exit(0);
	}

	/* TODO 4: Waits for the first thread */
	pthread_join(tid[0], NULL);

	/* TODO 5: Waits for the second thread */
	pthread_join(tid[1], NULL);

	/* TODO 6: Waits for the third thread */
	pthread_join(tid[2], NULL);

	for( int i = 0; i < DATA_SIZE; i++ )
		printf("%d ", data[i]);
	printf("\n");

	return 0;
}

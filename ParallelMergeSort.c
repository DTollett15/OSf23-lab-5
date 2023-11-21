#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 100

int array[SIZE];

void fillArrayWithRandomNumbers(int arr[SIZE]) {
    for (int i = 0; i < SIZE; i++) 
        arr[i] = rand() % 100;
}

void printArray(int arr[SIZE]) {
    for (int i = 0; i < SIZE; i++) 
        printf("%5d", arr[i]);
    printf("\n");
}

typedef struct StartEndIndexes {
    int start;
    int end;
} StartEndIndexes;

void merge(int arr[], int start, int mid, int end) {
    // Merge two halves of the array
    int n1 = mid - start + 1;
    int n2 = end - mid;

    int left[n1], right[n2];

    for (int i = 0; i < n1; i++)
        left[i] = arr[start + i];
    for (int j = 0; j < n2; j++)
        right[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = start;

    while (i < n1 && j < n2) {
        if (left[i] <= right[j]) {
            arr[k] = left[i];
            i++;
        } else {
            arr[k] = right[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = left[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = right[j];
        j++;
        k++;
    }
}

void* mergeSort(void* args) {
    StartEndIndexes* sei = (StartEndIndexes*)args;
    int start = sei->start;
    int end = sei->end;

    if (start < end) {
        // Find the middle point of the array
        int mid = start + (end - start) / 2;

        // Create two threads to sort the two halves
        pthread_t tid[2];
        StartEndIndexes left_sei, right_sei;

        left_sei.start = start;
        left_sei.end = mid;

        right_sei.start = mid + 1;
        right_sei.end = end;

        pthread_create(&tid[0], NULL, mergeSort, &left_sei);
        pthread_create(&tid[1], NULL, mergeSort, &right_sei);

        // Wait for the threads to finish
        pthread_join(tid[0], NULL);
        pthread_join(tid[1], NULL);

        // Merge the sorted halves
        merge(array, start, mid, end);
    }

    pthread_exit(NULL);
}

int main() {
    srand(time(0));
    StartEndIndexes sei;
    sei.start = 0;
    sei.end = SIZE - 1;

    // 1. Fill array with random numbers.
    fillArrayWithRandomNumbers(array);

    // 2. Print the array.
    printf("Unsorted array: ");
    printArray(array);

    // 3. Create threads for merge sort.
    pthread_t tid;
    pthread_create(&tid, NULL, mergeSort, &sei);

    // 4. Wait for mergesort to finish.
    pthread_join(tid, NULL);

    // 5. Print the sorted array.
    printf("Sorted array:   ");
    printArray(array);

    return 0;
}

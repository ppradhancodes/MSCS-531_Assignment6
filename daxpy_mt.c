#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4  // Adjust based on the number of cores you want to simulate

typedef struct {
    int start;
    int end;
    double a;
    double *x;
    double *y;
} ThreadData;

void *daxpy_thread(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    for (int i = data->start; i < data->end; i++) {
        data->y[i] = data->a * data->x[i] + data->y[i];
    }
    return NULL;
}

void daxpy_mt(int n, double a, double *x, double *y) {
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];
    int chunk_size = n / NUM_THREADS;

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].start = i * chunk_size;
        thread_data[i].end = (i == NUM_THREADS - 1) ? n : (i + 1) * chunk_size;
        thread_data[i].a = a;
        thread_data[i].x = x;
        thread_data[i].y = y;
        pthread_create(&threads[i], NULL, daxpy_thread, &thread_data[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
}

int main() {
    int n = 1000000;  // Size of vectors
    double a = 2.0;
    double *x = (double *)malloc(n * sizeof(double));
    double *y = (double *)malloc(n * sizeof(double));

    // Initialize vectors
    for (int i = 0; i < n; i++) {
        x[i] = 1.0;
        y[i] = 2.0;
    }

    // Perform multi-threaded DAXPY
    daxpy_mt(n, a, x, y);

    // Verify result (check a few elements)
    printf("Verification (first 5 elements):\n");
    for (int i = 0; i < 5 && i < n; i++) {
        printf("y[%d] = %f\n", i, y[i]);
    }

    free(x);
    free(y);
    return 0;
}
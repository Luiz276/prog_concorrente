#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <stdlib.h>

FILE* out;
sem_t sem[2];

void *thread_a(void *args) {
    for (int i = 0; i < *(int*)args; ++i) {
        sem_wait(&sem[0]);
	//      +---> arquivo (FILE*) destino
	//      |    +---> string a ser impressa
	//      v    v
        fprintf(out, "A");
        // Importante para que vocês vejam o progresso do programa
        // mesmo que o programa trave em um sem_wait().
        fflush(stdout);
        sem_post(&sem[1]);
    }
    return NULL;
}

void *thread_b(void *args) {
    for (int i = 0; i < *(int*)args; ++i) {
        sem_wait(&sem[1]);
        fprintf(out, "B");
        fflush(stdout);
        sem_post(&sem[0]);
    }
    return NULL;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Uso: %s [ITERAÇÕES]\n", argv[0]);
        return 1;
    }
    int iters = atoi(argv[1]);
    srand(time(NULL));
    out = fopen("result.txt", "w");

    pthread_t ta, tb;

    sem_init(&sem[0], 0, 0);
    sem_init(&sem[1], 0, 0);

    // Cria threads
    pthread_create(&ta, NULL, thread_a, &iters);
    pthread_create(&tb, NULL, thread_b, &iters);

    sem_post(&sem[0]);

    // Espera pelas threads
    pthread_join(ta, NULL);
    pthread_join(tb, NULL);

    sem_destroy(&sem[0]);
    sem_destroy(&sem[1]);

    //Imprime quebra de linha e fecha arquivo
    fprintf(out, "\n");
    fclose(out);
  
    return 0;
}

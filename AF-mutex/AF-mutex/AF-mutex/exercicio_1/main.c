#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>

//                 (main)      
//                    |
//    +----------+----+------------+
//    |          |                 |   
// worker_1   worker_2   ....   worker_n


// ~~~ argumentos (argc, argv) ~~~
// ./program n_threads

// ~~~ printfs  ~~~
// main thread (após término das threads filhas): "Contador: %d\n"
// main thread (após término das threads filhas): "Esperado: %d\n"

// Obs:
// - pai deve criar n_threds (argv[1]) worker threads 
// - cada thread deve incrementar contador_global (operador ++) n_loops vezes
// - pai deve esperar pelas worker threads  antes de imprimir!


int contador_global = 0;
pthread_mutex_t mut1;

void *comportamento_thread(void *n_loops) {
    int loop_vezes = *(int*)n_loops;
    pthread_mutex_lock(&mut1);
    for (int i = 0; i < loop_vezes; i++) {
        contador_global++;
    }
    pthread_mutex_unlock(&mut1);
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("n_threads é obrigatório!\n");
        printf("Uso: %s n_threads n_loops\n", argv[0]);
        return 1;
    }

    int n_threads = atoi(argv[1]);
    int n_loops = atoi(argv[2]);
    //...
    pthread_mutex_init(&mut1, NULL);

    pthread_t thread_id[n_threads];
    for (int i = 0; i < n_threads; i++) {
        pthread_create(&thread_id[i], NULL, comportamento_thread, &n_loops);
    }

    for (int i = 0; i < n_threads; i++) {
        pthread_join(thread_id[i], NULL);
    }

    pthread_mutex_destroy(&mut1);

    printf("Contador: %d\n", contador_global);
    printf("Esperado: %d\n", n_threads*n_loops);
    return 0;
}
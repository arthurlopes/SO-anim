//  main.c
//  anim
//  Created by Arthur Costa Lopes on 3/31/16.
//  Copyright © 2016 Arthur Costa Lopes. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>

#define await(x) while(!(x))

#define N 10
#define num_repeat 1

#define BLACK 1
#define WHITE 2

volatile int b;
volatile int choosing[N] = { 0, };
volatile int num[N] = { 0, };
volatile int color = WHITE;
volatile int my_color[N] = { 0, };

int max(int color);
void print();
void printarray();

void *thr(void *v) {
    int thr_id = (int)(long)v;
    int j, repeat;

    for (repeat = 0; repeat < num_repeat; repeat++) {
        choosing[thr_id] = 1;
        my_color[thr_id] = color;
        num[thr_id] = 1 + max(my_color[thr_id]);

        sleep(1 + (rand() % 3));
        print();
        choosing[thr_id] = 0;
        for (j = 0; j < N; j++) {
            if (j == thr_id) continue;
            await (choosing[j] == false);
            if (my_color[thr_id] == my_color[j])
                await (num[j] == 0 || num[j] > num[thr_id] || (num[j] == num[thr_id] && j > thr_id) || my_color[j] !=   my_color[thr_id]);
            else
                await (num[j] == 0 || my_color[thr_id] != color || my_color[thr_id] == my_color[j]);
    }

    // CS
    b = thr_id;
    sleep(1 + (rand() % 3));
    print();

    if (my_color[thr_id] == BLACK)
        color = WHITE;
    else
        color = BLACK;
    num[thr_id] = 0;

    pthread_yield_np();

}

return NULL;

}

void printarray(int* v) {
    int i;
    
    printf("|      ");

    for (i = 0; i < N; i++) {
        if (!v[i]) {
            printf("   ");
        } else {
            printf("T%d ", i);
        }
    }

    printf("        |\n");
}



void print() {
    int i;

    printf("|--------------------------------------------|\n");
    printf("|                Choosing                    |\n");
    printf("|--------------------------------------------|\n");
    printf("|                                            |\n");

    printarray((int*)choosing);

    printf("|                                            |\n");
    printf("|--------------------------------------------|\n");
    printf("|                Waiting                     |\n");
    printf("|--------------------------------------------|\n");
    printf("|                                            |\n");

    printarray((int*)num);

    printf("|      ");
    for (i = 0; i < N; i++) {
        if (!num[i]) {
                printf("   ");
        } else {
                printf("%d", num[i]);
            if (my_color[i] == WHITE) {
                printf("W ");
            } else {
                printf("B ");
            }

        }
    }
    printf("        |\n");

    printf("|                                            |\n");
    printf("|--------------------------------------------|\n");
    printf("|                Buying                      |\n");
    printf("|--------------------------------------------|\n");
    
    if (b >= 0) {
        printf("|                  T%d                        |\n", b);
    } else {
        printf("|                                            |\n");
    }
    
    printf("|--------------------------------------------|\n");

}



int max(int color) {

    int i, max = 0;
    for (i = 0; i < N; i++)
        if (my_color[i] == color)
            if (num[i] > max)
                max = num[i];
    return max;
}



int main() {

    pthread_t threads[N];
    int i;
        
    b = -1;
    for (i = 0; i < N; i++) {
        choosing[i] = 0;
        num[i]      = 0;
        my_color[i] = 0;
    }

    for (i = 0; i < N; i++) {
        pthread_create(&threads[i], NULL, thr, (void*)(long)i);
        sleep(1 + (rand() % 3));
    }

    for (i = 0; i < N; i++)
        pthread_join(threads[i], NULL);
    
    print();
    
    return 0;
}

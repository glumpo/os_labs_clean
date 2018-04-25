#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>

int pow_diam(const int cir_diam, const int k) {
    int res_diam = cir_diam;
    for (int i = 1; i < k; ++i) {
        res_diam += cir_diam - 1;
    }
    return res_diam;
}

void fill_pic(int pic_size, int pic[][pic_size]) {
    srand(time(0));
    for (int i = 0; i < pic_size; ++i) {
        for (int j = 0; j < pic_size; ++j) {
            pic[i][j] = rand() % 10;
        }
    }
}


void fill_circle(int cir_diam, int circle[][cir_diam]) {
    int mid = cir_diam / 2;
    int pick = 1;
    for (int i = 0; i < cir_diam; ++i) {
        for (int j = 0; j <= mid - pick; ++j)
            circle[i][j] = 0;
        for (int j = mid - pick + 1; j < mid + pick; ++j)
            circle[i][j] = 1;
        for (int j = mid + pick; j < cir_diam; ++j)
            circle[i][j] = 0;

        if (i < mid)
            ++pick;
        else
            --pick;
    }
}

void print_matrix(int pic_size, int pic[][pic_size]) {
    for (int i = 0; i < pic_size; ++i) {
        for (int j = 0; j < pic_size; ++j) {
            printf("%d ", pic[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void copy_matrix(int size, int orig[][size], int dist[][size]) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            dist[i][j] = orig[i][j];
        }
    }
}

typedef struct point {
    int x;
    int y;
} point;


int **Pic_orig;
int **Pic_res;
int **Circle;

int Pic_size = 0;
int Cir_diam = 0;

point Center;

pthread_mutex_t mutex;

void calculate_in_point(point cur) {
//    pthread_mutex_lock(&mutex);
    const int orig[Pic_size][Pic_size];
    int res[Pic_size][Pic_size];
    const int circle[Cir_diam][Cir_diam];


//    pthread_mutex_lock(&mutex);

    copy_matrix(Pic_size, Pic_orig, orig);
    copy_matrix(Pic_size, Circle,   circle);
    copy_matrix(Pic_size, Pic_res,  res);

//    pthread_mutex_unlock(&mutex);

    int max = orig[cur.x][cur.y];

    for (int i = 0; i < Cir_diam; ++i) {
        for (int j = 0; j < Cir_diam; ++j) {
            int working_x = cur.x + (Center.x - i);
            int working_y = cur.y + (Center.y - j);
//            printf("(%d %d);", working_x, working_y);
            if (0 == circle[i][j])
                continue;
            if (working_x < 0 || working_x > Pic_size)
                continue;
            if (working_y < 0 || working_y > Pic_size)
                continue;

            int cur_element = orig[working_x][working_y];
            if (cur_element > max)
                max = cur_element;
        }
    }
//    printf("\n\n");

    res[cur.x][cur.y] = max;

    pthread_mutex_lock(&mutex);

    copy_matrix(Pic_size, res, Pic_res);

    pthread_mutex_unlock(&mutex);
}

void *calculate_in_row(void *arg) {
    for (int i = 0; i < Pic_size; ++i) {
        point cur = {i, *((int *)arg)};
        calculate_in_point(cur);
    }
    return NULL;
}


int main(int argc, char *argv[]) {
    Pic_size = 0;
    Cir_diam = 0;
    int k = 0;

    printf("Input matrix: ");
    scanf("%d", &Pic_size);

    printf("Input circle diameter: ");
    scanf("%d", &Cir_diam);

    printf("Input k: ");
    scanf("%d", &k);

    Cir_diam = pow_diam(Cir_diam, k);

//    int pic_orig[pic_size][pic_size];
//    int pic_res[pic_size][pic_size];
//    int circle[cir_diam][cir_diam];

    Pic_orig = malloc(Pic_size * sizeof(int*));
    for (int i = 0; i < Pic_size; ++i) {
        Pic_orig[i] = malloc(Pic_size * sizeof(int*));
    }

    Pic_res = malloc(Pic_size * sizeof(int*));
    for (int i = 0; i < Pic_size; ++i) {
        Pic_res[i] = malloc(Pic_size * sizeof(int*));
    }

    Circle = malloc(Cir_diam * sizeof(int*));
    for (int i = 0; i < Cir_diam; ++i) {
        Circle[i] = malloc(Cir_diam * sizeof(int*));
    }


    fill_pic(Pic_size, Pic_orig);
    copy_matrix(Pic_size, Pic_orig, Pic_res);
    fill_circle(Cir_diam, Circle);

    print_matrix(Pic_size, Pic_orig);

    Center.x = Cir_diam/2;
    Center.y = Cir_diam/2;
    for (int i = 0; i < Pic_size; ++i) {
        for (int j = 0; j < Pic_size; ++j) {
            point cur = {i, j};
            calculate_in_point(cur);
        }
    }

    pthread_t tid[Pic_size];
    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < Pic_size; ++i) {
           if (pthread_create(&tid[i], NULL, calculate_in_row, &i) != 0) {
               perror("Can`t create thread\n");
               return 1;
           }
       }
       for (int i = 0; i < Pic_size; ++i) {
           if (pthread_join(tid[i], NULL) != 0) {
               perror("Can`t join threads");
               return 2;
           }
   }

    print_matrix(Pic_size, Pic_res);
    print_matrix(Cir_diam, Circle);

    return 0;
}

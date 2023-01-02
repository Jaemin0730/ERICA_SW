#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

int main() {
    FILE* file1;
    FILE* file2;
    char FN[100];
    int a;
    int read = 0;
    char name[9] = "_sol.txt";
    srand(time(NULL));

    printf("Input file name: ");
    scanf("%s", FN);

    file1 = fopen(FN, "r");


    int* kdn = (int*)malloc(sizeof(int) * 3);

    int j = 0;
    while (read = fscanf(file1, "%d", &a) != EOF) {
        kdn[j] = (int)a;

        j++;

        if (j == 3) {
            break;
        }
    }

    fseek(file1, 0, SEEK_SET);

    int** save;
    save = (int**)malloc(sizeof(int*) * ((int)kdn[2]));
    for (int x = 0; x < (int)kdn[2]; x++) {
        save[x] = (int*)malloc(sizeof(int) * (int)kdn[1]);
    }


    int* list = (int*)malloc(sizeof(int) * (((int)kdn[2] * (int)kdn[1]) + 3));

    j = 0;
    while (read = fscanf(file1, "%d", &a) != EOF) {
        list[j] = (int)a;

        j++;
    }

    strcat(FN, name);
    file2 = fopen(FN, "wt");
    if (file2 == NULL) {
        printf("FAIL");
        return 1;
    }
    int** cluster;
    cluster = (int**)malloc(sizeof(int*) * ((int)kdn[(int)0]));
    for (int x = 0; x < (int)kdn[(int)0]; x++) {
        cluster[x] = (int*)malloc(sizeof(int) * (int)kdn[(int)2]);
    }


    int start = 3;
    for (int i = 0; i < kdn[2]; i++) {
        for (int j = 0; j < kdn[1]; j++) {
            save[i][j] = list[start];
            start++;
        }

    }

    for (int i = 0; i < kdn[2]; i++) {
        printf("%d번째 점 ", i + 1);
        for (int j = 0; j < kdn[1]; j++) {
            printf("%d ", save[i][j]);
        }
        printf("\n");
    }

    //---------------------------------------------------
    int* random = (int*)malloc(sizeof(int) * kdn[0]);
    int randTemp = -1;
    int c = 0;
    printf("random 숫자");
    for (int i = 0; i < kdn[0]; i++) {
        randTemp = (rand() % (kdn[2] - kdn[0] + 1 + c - randTemp - 1)) + randTemp + 1;
        random[i] = randTemp;
        c++;
        printf("%d ", random[i]);
    }
    printf("\n");
    int** distance;
    distance = (int**)malloc(sizeof(int*) * ((int)kdn[(int)0]));
    for (int x = 0; x < (int)kdn[(int)0]; x++) {
        distance[x] = (int*)malloc(sizeof(int) * (int)kdn[(int)2]);
    }



    for (int i = 0; i < kdn[0]; i++) {
        for (int j = 0; j < kdn[2]; j++) {
            distance[i][j] = 0;

        }
    }

    printf("distance\n");
    for (int i = 0; i < kdn[0]; i++) {
        for (int k = 0; k < kdn[2]; k++) {
            for (int j = 0; j < kdn[1]; j++) {
                distance[i][k] += pow((double)((int)save[(int)random[(int)i]][(int)j] - (int)save[(int)k][(int)j]), (double)2);
            }

            printf("%d ", distance[i][k]);
        }
        printf("\n");
    }

    int* beforemin = (int*)malloc(sizeof(int) * kdn[0]);
    int min;

    for (int i = 0; i < kdn[0]; i++) {
        for (int j = 0; j < kdn[2]; j++) {
            cluster[i][j] = 0;
        }
    }


    for (int i = 0; i < kdn[2]; i++) {
        for (int j = 0; j < kdn[0]; j++) {
            beforemin[j] = distance[j][i];
        }
        min = beforemin[0];

        for (int k = 0; k < kdn[0]; k++) {
            if (min > beforemin[k]) {
                min = beforemin[k];
            }
        }



        for (int s = 0; s < kdn[0]; s++) {
            if (min == beforemin[s]) {
                for (int j = 0; j < kdn[2]; j++) {
                    if (cluster[s][j] == 0) {
                        cluster[s][j] = i + 1;
                        break;
                    }

                }
                break;
            }
        }

    }

    fprintf(file2, "1\n");
    for (int i = 0; i < kdn[0]; i++) {
        printf("%d번째클러스터 ", i + 1);
        for (int j = 0; j < kdn[2]; j++) {
            if (cluster[i][j] == 0) {
                break;
            }

            printf("%d ", cluster[i][j]);
            fprintf(file2, "%d ", cluster[i][j]);

        }
        printf("\n");
        fprintf(file2, "\n");
    }

    int count = 0;
    int** center;
    center = (int**)malloc(sizeof(int*) * ((int)kdn[0]));
    for (int x = 0; x < (int)kdn[0]; x++) {
        center[x] = (int*)malloc(sizeof(int) * (int)kdn[1]);
    }
    int dsum = 0;
    for (int i = 0; i < kdn[0]; i++) {
        for (int j = 0; j < kdn[1]; j++) {
            for (int k = 0; k < kdn[0]; k++) {
                if (cluster[i][k] == 0) {
                    break;
                }
                dsum += save[cluster[i][k] - 1][j];
                count++;
            }
            dsum = dsum / count;
            center[i][j] = dsum;
            dsum = 0;
            count = 0;

        }
    }

    printf("center(중심축)구성요소\n");
    for (int i = 0; i < kdn[0]; i++) {
        for (int j = 0; j < kdn[1]; j++) {
            printf("%d ", center[i][j]);
        }
        printf("\n");
    }

    int** joker;
    joker = (int**)malloc(sizeof(int*) * ((int)kdn[(int)0]));
    for (int x = 0; x < (int)kdn[(int)0]; x++) {
        joker[x] = (int*)malloc(sizeof(int) * (int)kdn[(int)2]);
    }

    for (int i = 0; i < kdn[0]; i++) {
        for (int j = 0; j < kdn[2]; j++) {
            joker[i][j] = cluster[i][j];
        }
    }

    int samecount = 0;
    int kcount = 2;
    while (samecount != (int)(kdn[0] * kdn[2])) {
        fprintf(file2, "%d\n", kcount);
        samecount = 0;
        count = 0;
        dsum = 0;
        for (int i = 0; i < kdn[0]; i++) {
            for (int j = 0; j < kdn[2]; j++) {
                distance[i][j] = 0;

            }
        }
        printf("distance\n");

        for (int i = 0; i < kdn[0]; i++) {
            for (int k = 0; k < kdn[2]; k++) {
                for (int j = 0; j < kdn[1]; j++) {
                    distance[i][k] += pow((double)((int)center[i][j] - (int)save[(int)k][(int)j]), (double)2);
                }

                printf("%d ", distance[i][k]);
            }
            printf("\n");
        }

        for (int i = 0; i < kdn[0]; i++) {
            for (int j = 0; j < kdn[2]; j++) {
                cluster[i][j] = 0;
            }
        }


        for (int i = 0; i < kdn[2]; i++) {
            for (int j = 0; j < kdn[0]; j++) {
                beforemin[j] = distance[j][i];
            }
            min = beforemin[0];

            for (int k = 0; k < kdn[0]; k++) {
                if (min > beforemin[k]) {
                    min = beforemin[k];
                }
            }



            for (int s = 0; s < kdn[0]; s++) {
                if (min == beforemin[s]) {
                    for (int j = 0; j < kdn[2]; j++) {
                        if (cluster[s][j] == 0) {
                            cluster[s][j] = i + 1;
                            break;
                        }

                    }
                    break;
                }
            }

        }


        for (int i = 0; i < kdn[0]; i++) {
            printf("%d번째클러스터 ", i + 1);
            for (int j = 0; j < kdn[2]; j++) {
                if (cluster[i][j] == 0) {
                    break;
                }
                printf("%d ", cluster[i][j]);
                fprintf(file2, "%d ", cluster[i][j]);
            }
            printf("\n");
            fprintf(file2, "\n");

        }

        for (int i = 0; i < kdn[0]; i++) {
            for (int j = 0; j < kdn[2]; j++) {
                if (joker[i][j] == cluster[i][j]) {
                    samecount++;
                }
            }
        }
        for (int i = 0; i < kdn[0]; i++) {
            for (int j = 0; j < kdn[2]; j++) {
                joker[i][j] = cluster[i][j];
            }
        }



        for (int i = 0; i < kdn[0]; i++) {
            for (int j = 0; j < kdn[1]; j++) {
                for (int k = 0; k < kdn[0]; k++) {
                    if (cluster[i][k] == 0) {
                        break;
                    }
                    dsum += save[cluster[i][k] - 1][j];
                    count++;
                }
                dsum = dsum / count;
                center[i][j] = dsum;
                dsum = 0;
                count = 0;

            }
        }
        printf("center(중심축)구성요소\n");
        for (int i = 0; i < kdn[0]; i++) {
            for (int j = 0; j < kdn[1]; j++) {
                printf("%d ", center[i][j]);
            }
            printf("\n");
        }

        kcount++;
    }

    fclose(file2);



    return 0;

}
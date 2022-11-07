#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main()
{
    FILE* file1;
    FILE* file2;
    int a = 0;
    int read = 0;
    int f = 0;
    char FN[100];
    int line = 0;
    char c;
    char name[9] = "_sol.txt";


    printf("We constructed the code using Visual Studio 2019ver.\n");

    printf("Input file name: ");
    scanf("%s", FN);

    file1 = fopen(FN, "r");


    while ((c = fgetc(file1)) != EOF) {
        if (c == '\n') line++;
    }
    line++;


    int* save = (int*)malloc(sizeof(int) * (line * 2));


    fseek(file1, 0, SEEK_SET);

    int j = 0;
    while (read = fscanf(file1, "%d", &a) != EOF) {
        save[j] = (int)a;

        j++;
    }

    // -------------------------------------------------------

    int start3 = 1;
    int mresult = 0;
    for (int i = 0; i < (int)(line - 1); i++) {
        mresult += save[0] * save[start3] * save[start3 + 2];
        start3 += 2;
    }
    printf("Total Number Of Multiplications:");
    printf("%d\n", mresult);
    printf("\n");

    // -------------------------------------------------------



    fclose(file1);

    int* list = (int*)malloc(sizeof(int) * (line + 1));

    int o = 1;
    for (int i = 0; i < line * 2; i++) {
        if (i == 0) {
            list[i] = save[i];
        }
        else if (i != 0) {
            if (i % 2 == 1) {
                list[o] = save[i];
                o++;
            }
        }
    }


    
    // ------------------------------------------------

    int*** M3;
    M3 = (int***)malloc(sizeof(int**) * line);
    int start = 0;
    srand(time(NULL));

    for (int i = 0; i < (int)line * 2; i += 2) {
        M3[start] = (int**)malloc(sizeof(int*) * (int)save[i]);

        for (int j = 0; j < (int)save[i]; j++) {
            M3[start][j] = (int*)malloc(sizeof(int) * (int)save[i + 1]);
            for (int k = 0; k < (int)save[i + 1]; k++) {
                M3[start][j][k] = rand() % 9 + 1;
            }
        }
        start++;

    }
    //-------------------------------------------------------------------

    int*** joker;
    joker = (int***)malloc(sizeof(int**) * (int)(line - 1));
    int start2 = 0;

    for (int i = 3; i < (int)(line * 2); i += 2) {
        joker[start2] = (int**)malloc(sizeof(int*) * (int)save[0]);
        for (int j = 0; j < (int)save[0]; j++) {
            joker[start2][j] = (int*)malloc(sizeof(int) * (int)save[i]);
        }
        start2++;

    }




    //--------------------------------------------------------------------

    for (int i = 0; i < (int)save[0]; i++) {
        for (int j = 0; j < (int)save[3]; j++) {
            joker[0][i][j] = 0;
            for (int k = 0; k < (int)save[1]; k++) {
                joker[0][i][j] += M3[0][i][k] * M3[1][k][j];
            }
        }
    }
    int se = 3;

    for (int z = 1; z < line - 1; z++) {
        for (int i = 0; i < save[0]; i++) {
            for (int j = 0; j < save[se + 2]; j++) {
                joker[z][i][j] = 0;
                for (int k = 0; k < save[se]; k++) {
                    joker[z][i][j] += joker[z - 1][i][k] * M3[z + 1][k][j];
                }
            }
        }
        se += 2;
    }

    // --------------------------------------------------------------------

    printf("Output Matrix\n");
    for (int j = 0; j < (int)save[0]; j++) {
        for (int k = 0; k < (int)save[(int)((line * 2) - 1)]; k++) {
            printf("%d ", joker[(int)((int)line - 2)][j][k]);

        }
        printf("\n");
    }



    int plus = 0;
    for (int i = 0; i < (int)line; i++) {
        printf("\nInput Matrix %d\n", i + 1);
        for (int j = 0; j < (int)save[(int)plus]; j++) {
            for (int k = 0; k < (int)save[(int)(plus + 1)]; k++) {
                printf("%d ", M3[i][j][k]);
            }
            printf("\n");
        }
        plus += 2;
    }
    strcat(FN, name);
    file2 = fopen(FN, "wt");
    if (file2 == NULL) {
        printf("FAIL");
        return 1;
    }

    fprintf(file2, "Total Number Of Multiplications : %d\n", mresult);
    fprintf(file2, "\nOutput Matrix\n");
    for (int j = 0; j < (int)save[0]; j++) {
        for (int k = 0; k < (int)save[(int)((line * 2) - 1)]; k++) {
            fprintf(file2, "%d \t", joker[(int)((int)line - 2)][j][k]);
        }
        fprintf(file2, "\n");
    }
    plus = 0;
    for (int i = 0; i < (int)line; i++) {

        fprintf(file2, "\nInput Matrix %d\n", i + 1);
        for (int j = 0; j < (int)save[(int)plus]; j++) {
            for (int k = 0; k < (int)save[(int)(plus + 1)]; k++) {
                fprintf(file2, "%d ", M3[i][j][k]);
            }
            fprintf(file2, "\n");
        }
        plus += 2;

    }

    printf("\nThe Output File Is In The Input File Storage Location.\n");

    fclose(file2);

    return 0;

}
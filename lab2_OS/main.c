#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <ctype.h>


int a[20][20];
int b[20][20];
int c[20][20];
int no_rows_a = 0, no_cols_a = 0;
int no_rows_b = 0, no_cols_b = 0;
int rows_c = 0, cols_c = 0;
char *file_a , *file_b , *file_c_m1 , *file_c_m2 , *file_c_m3 ;
struct position {
    int row_index;
    int col_index;
};
void setvalue_zero(int c[20][20]){
    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 20; ++j) {
            c[i][j]=0;
        }
    }
}
void PrintoutputMatrix(FILE *fp){
    for (int i = 0; i < rows_c; ++i) {
        for (int j = 0; j < rows_c; ++j) {
            fprintf(fp, "%d ", c[i][j]);
        }
        fprintf(fp, "\n");
    }
}
int findint(char *str){
    char  *p = str;
    while (*p) { // While there are more characters to process...
        if ( isdigit(*p) || ( (*p=='-'||*p=='+') && isdigit(*(p+1)) )) {
            // Found a number
            long val = strtol(p, &p, 10); // Read number
            return val;// and print it.
        } else {
            // Otherwise, move on to the next character.
            p++;
        }
    }
}
char *substring(char *string, int position, int length) {
    char *pointer;int c;
    pointer = malloc(length + 1);
    if(pointer == NULL)
        exit(EXIT_FAILURE);
    for(c = 0; c < length; c++)
        *(pointer + c) = *((string + position - 1) + c);
    *(pointer + c ) = '\0';
    return pointer;
}
char *insert_substring(char *a, char *b, int position) {
    char *f, *e;
    int length  = strlen(a);
    int new_str_length = strlen(a) + strlen(b) + 1;
    char *new_str = (char *) malloc(new_str_length * sizeof(char ));


    f = substring(a, 1, position - 1);
    e = substring(a, position + 1, length - position);

    strcpy(new_str, f);
    free(f);
    strcat(new_str, b);
    strcat(new_str, e);
    free(e);
    return new_str;
}
char *add_pathANDextension(char *path, char *fileName, char *extension) {
    char *new_str = "";
    new_str = insert_substring(new_str, path, 1);
    new_str = insert_substring(new_str, fileName, strlen(new_str) + 1);
    new_str = insert_substring(new_str, extension, strlen(new_str) + 1);
    return new_str;
}
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void *calculate_matrix_c_using_single_thread() {

    for (int i = 0; i < rows_c; ++i) {
        for (int j = 0; j < cols_c; ++j) {
            for (int k = 0; k < no_cols_a; ++k) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    pthread_exit(NULL);
}
void *calculate_matrix_c_using_thread_per_row(void *thread_arg) {
    long row_index = (long) thread_arg;
    for (int j = 0; j < cols_c; ++j) {
        for (int k = 0; k < no_cols_a; ++k) {
            c[row_index][j] += a[row_index][k] * b[k][j];
        }
    }
    pthread_exit(NULL);
}
void *calculate_matrix_c_using_thread_per_element(void *thread_arg) {
    struct position *current_pos = (struct position *) thread_arg;
    int row_index = current_pos->row_index;
    int col_index = current_pos->col_index;
    for (int k = 0; k < no_cols_a; ++k) {
        c[row_index][col_index] += a[row_index][k] * b[k][col_index];
    }
    pthread_exit(NULL);
}
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
char **parse_input(char *input) {
    char **parsed_command = (char **) malloc(10 * sizeof(char *));
    char *token = strtok(input, " \t");
    int index = 0;
    while (token != NULL) {
        parsed_command[index] = token;
        token = strtok(NULL, " ");
        index++;
    }
    parsed_command[index] = NULL;
    return parsed_command;

}
void ReadMatrix(char *file_name) {
    FILE *fp = fopen(file_name, "r");
    if (fp == NULL)
        exit(1);
    char str[200];
    char **parsed_input;
    if(fgets(str, sizeof(str), fp)) {
        parsed_input = parse_input(str);
        int row0os = findint(parsed_input[0]);
        int co0ols = findint(parsed_input[1]);
        if(no_rows_a == 0) {
            no_rows_a = row0os;
            no_cols_a = co0ols;
            int row_index=0;
            while (fgets(str, sizeof(str), fp)) {
                parsed_input = parse_input(str);
                for (int i = 0; i < no_cols_a; ++i)a[row_index][i] = strtol(parsed_input[i], NULL, 10);
                row_index++;
                if (row_index == no_rows_a)break;
            }
        }
        else {
            no_rows_b = row0os;
            no_cols_b = co0ols;
            int row_index=0;
            while (fgets(str, sizeof(str), fp)) {
                parsed_input = parse_input(str);
                for (int i = 0; i < no_cols_b; ++i)b[row_index][i] = strtol(parsed_input[i],NULL,10);
                row_index++;
                if (row_index == no_rows_b)break;
            }
        }
    }
    fclose(fp);
}
void write_in_file(char *file_name, int x) {
    FILE *fp = fopen(file_name, "w");
    if (fp == NULL)
        exit(1);

    if(x ==1)
        fputs("Method: A thread per matrix\n", fp);
    else if(x ==2)
        fputs("Method: A thread per row\n", fp);
    else if(x ==3)
        fputs("Method: A thread per element\n", fp);

    fprintf(fp, "row=%d col=%d\n", rows_c, cols_c);
    PrintoutputMatrix(fp);
    fclose(fp);
}
//______________________________________________________________________________________________________
/*                                               MAIN                                             */
//______________________________________________________________________________________________________
int main(int argc, char *argv[]) {
    if (strcmp(*argv, "./matMultp") ) {
        printf("Invalid Command");
        exit(1);
    }
    /*_________________________ Check the arguments __________________________*/
    {
        if (argv[1] == NULL) { // Case no arguments
        file_a = "/home/seif/Desktop/lab2_OS/a.txt";
        file_b = "/home/seif/Desktop/lab2_OS/b.txt";
        file_c_m1 = "/home/seif/Desktop/lab2_OS/c_per_matrix.txt";
        file_c_m2 = "/home/seif/Desktop/lab2_OS/c_per_row.txt";
        file_c_m3 = "/home/seif/Desktop/lab2_OS/c_per_element.txt";
        }
        else if (argv[1] != NULL && argv[2] != NULL && argv[3] != NULL) { // case arguments ( Matrices ) passed
            char *path = "/home/seif/Desktop/lab2_OS/";
            file_a = add_pathANDextension(path, argv[1], ".txt");
            file_b = add_pathANDextension(path, argv[2], ".txt");
            file_c_m1 = add_pathANDextension(path, argv[3], "_per_matrix.txt");
            file_c_m2 = add_pathANDextension(path, argv[3], "_per_row.txt");
            file_c_m3 = add_pathANDextension(path, argv[3], "_per_element.txt");
        }
    }
    /*____________________Read Given matrices in the .TXT files__________________*/
    ReadMatrix(file_a);
    ReadMatrix(file_b);
    /*__________________________Check matrix dimensions________________________*/
    if (no_cols_a != no_rows_b) {
        printf("\nInvalid operation!\n");
        exit(1);
    }
    rows_c = no_rows_a;
    cols_c = no_cols_b;
    struct timeval stop, start;
//______________________________________________________________________________________________________
/*                                               MethoD 1                                             */
//______________________________________________________________________________________________________
    {
        printf("calculate output using single thread Method ==>\n");
        setvalue_zero( &*c);
        pthread_t T1;
        gettimeofday(&start, NULL);
        pthread_create(&T1, NULL, calculate_matrix_c_using_single_thread, NULL);
        pthread_join(T1, NULL);
        gettimeofday(&stop, NULL);
        printf("Time taken: %lu us\n", stop.tv_usec - start.tv_usec);
        write_in_file(file_c_m1, 1);
    }
//______________________________________________________________________________________________________
/*                                               MethoD 2                                             */
//______________________________________________________________________________________________________
    {
        pthread_t threads[rows_c];
        printf("\ncalculate output using thread/row Method ==>\n");
        setvalue_zero( &*c);
        int status;
        gettimeofday(&start, NULL);
        for (long i = 0; i < rows_c; ++i) {
            pthread_create(&threads[i], NULL, calculate_matrix_c_using_thread_per_row, (void *) i);
        }

        for (long i = 0; i < rows_c; ++i) {
            pthread_join(threads[i], NULL);
        }
        gettimeofday(&stop, NULL);
        printf("Time taken: %lu us\n", stop.tv_usec - start.tv_usec);
        write_in_file(file_c_m2, 2);
    }
//______________________________________________________________________________________________________
/*                                               MethoD 3                                             */
//______________________________________________________________________________________________________
    {
        printf("\ncalculate output using thread/element Method ==>\n");
        setvalue_zero( &*c);
        int size = rows_c * cols_c;
        pthread_t threads[rows_c*cols_c];
        struct position *current_position_arr = (struct position *) malloc(size * sizeof(struct position));

        gettimeofday(&start, NULL);
        for (int i = 0; i < size; ++i) {
            current_position_arr[i].row_index = i / cols_c;
            current_position_arr[i].col_index = i % cols_c;
            pthread_create(&threads[i], NULL, calculate_matrix_c_using_thread_per_element,(void *) &current_position_arr[i]);
        }
        for (int i = 0; i < size; ++i) {
            pthread_join(threads[i], NULL);
        }
        gettimeofday(&stop, NULL);
        printf("Time taken: %lu us\n", stop.tv_usec - start.tv_usec);
        write_in_file(file_c_m3, 3);
        free(current_position_arr);
    }
    pthread_exit(NULL);
}
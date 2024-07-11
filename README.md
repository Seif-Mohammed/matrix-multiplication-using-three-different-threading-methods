# matrix-multiplication-using-three-different-threading-methods
The program reads two matrices from text files, performs matrix multiplication using three different threading methods, and writes the resulting matrices to separate files. The execution time for each method is measured and printed.

**Key Components**

Global Variables:

a[20][20], b[20][20], c[20][20]: Matrices for multiplication.

no_rows_a, no_cols_a, no_rows_b, no_cols_b, rows_c, cols_c: Dimensions of the matrices.

file_a, file_b, file_c_m1, file_c_m2, file_c_m3: File paths for the input and output matrices.

**Helper Functions:**

setvalue_zero(int c[20][20]): Initializes matrix c to zero.

PrintoutputMatrix(FILE *fp): Prints matrix c to the given file.

findint(char *str): Extracts the first integer from a string.

substring(char *string, int position, int length): Extracts a substring.

insert_substring(char *a, char *b, int position): Inserts a substring b into a at the given position.

add_pathANDextension(char *path, char *fileName, char *extension): Constructs a file path with the given base path, filename, and extension.

parse_input(char *input): Parses a string into tokens based on spaces.

ReadMatrix(char *file_name): Reads a matrix from a file.

write_in_file(char *file_name, int x): Writes the resulting matrix c to a file with a header indicating the method used.

**Thread Functions**

calculate_matrix_c_using_single_thread(): Multiplies matrices a and b using a single thread.

calculate_matrix_c_using_thread_per_row(void *thread_arg): Multiplies matrices a and b using a separate thread for each row of the result matrix c.

calculate_matrix_c_using_thread_per_element(void *thread_arg): Multiplies matrices a and b using a separate thread for each element of the result matrix c.

Main Function
The main function handles the execution flow:

Argument Parsing:
Checks the command line arguments and sets default file paths if none are provided.

Reading Matrices:
Reads matrices a and b from the specified files.

Dimension Check:
Ensures the matrices can be multiplied (i.e., the number of columns in a matches the number of rows in b).

**Matrix Multiplication Methods:**

Method 1: Single Thread:
Uses a single thread to multiply matrices a and b and writes the result to file_c_m1.

Method 2: Thread per Row:
Uses a thread for each row of the result matrix c and writes the result to file_c_m2.

Method 3: Thread per Element:
Uses a thread for each element of the result matrix c and writes the result to file_c_m3.

Execution Time Measurement:

Measures and prints the execution time for each method using gettimeofday.

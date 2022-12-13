#ifndef SRC_PARSER_H_
#define SRC_PARSER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "s21_matrix.h"

#define MAX_STR 256

typedef struct f_node {
  int *array_numbers;
  int count_numbers;
} f_node;

int get_size(FILE **obj_file, int *v_count, int *f_count, char file_name[]);
matrix_t *fill_matrix_array(FILE *obj_file, f_node **array_f, int v_count,
                            int *f_count, char file_name[]);
void create_f_array(f_node *result, const unsigned int *array);
void fill_matrix(matrix_t *A, const double v_vector[]);
int get_result(char *file_name, matrix_t **v, f_node **f, int *v_count,
               int *f_count);
int get_matrix_multi(matrix_t **v, int v_count, matrix_t mult_matrix);
void free_all(matrix_t **v, f_node **f, int v_count, int f_count);

#if DEBUG == 1
void print_matr(matrix_t *A);
void print_array(f_node *f);
void print_v(matrix_t *v, f_node *f, int f_count);
#endif

#endif  // SRC_PARSER_H_

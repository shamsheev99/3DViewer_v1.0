#include "parser.h"

int get_size(FILE **obj_file, int *v_count, int *f_count, char file_name[]) {
  int error = FAILURE;
  if (((*obj_file) = fopen(file_name, "r")) == NULL) {
    fprintf(stderr, "FILE EXIST -> %s", file_name);
  } else {
    char obj_str[MAX_STR];
    while (fgets(obj_str, MAX_STR, (*obj_file))) {
      if (obj_str[1] == ' ') {
        if (obj_str[0] == 'v') {
          (*v_count)++;
        } else if (obj_str[0] == 'f') {
          (*f_count)++;
        }
      }
    }
    error = SUCCESS;
    fclose((*obj_file));
  }
  return error;
}

matrix_t *fill_matrix_array(FILE *obj_file, f_node **array_f, int v_count,
                            int *f_count, char file_name[]) {
  matrix_t *array_matrix = (matrix_t *)malloc((v_count + 1) * sizeof(matrix_t));
  *array_f = (f_node *)malloc((*f_count) * sizeof(f_node));
  fill_matrix(&array_matrix[0], (double[]){0, 0, 0, 0});

  if ((obj_file = fopen(file_name, "r")) == NULL) {
    fprintf(stderr, "FILE EXIST -> %s", file_name);
  } else {
    char obj_str[MAX_STR];
    int i = 1, k = 0;

    while (fgets(obj_str, MAX_STR, obj_file)) {
      if (obj_str[1] == ' ') {
        if (obj_str[0] == 'v') {
          double vector[4] = {0};
          sscanf(obj_str, "v %lf%lf%lf", &vector[0], &vector[1], &vector[2]);
          vector[3] = 1.0;
          fill_matrix(&array_matrix[i], vector);
          i++;
        } else if (obj_str[0] == 'f') {
          unsigned int array_tmp[100] = {0};
          char *ptr = NULL;
          char *f_token = strtok_r(obj_str + 2, " ", &ptr);
          int j = 0;
          while (f_token != NULL) {
            if (sscanf(f_token, "%u", &array_tmp[j]) == 1) {
              j++;
            } else {
              break;
            }
            f_token = strtok_r(NULL, " ", &ptr);
          }
          (*array_f)[k].count_numbers = j;
          if (j > 2) {
            create_f_array(&((*array_f)[k]), array_tmp);
            k++;
          }
        }
      }
    }
    *f_count = k;
    fclose(obj_file);
  }
  return array_matrix;
}

void create_f_array(f_node *result, const unsigned int *array) {
  result->array_numbers = (int *)malloc(result->count_numbers * sizeof(int));
  for (int i = 0; i < result->count_numbers; i++) {
    result->array_numbers[i] = array[i];
  }
}

void fill_matrix(matrix_t *A, const double v_vector[]) {
  *A = s21_create_matrix(4, 1);
  if (A->rows > 0 && A->columns > 0) {
    for (int i = 0; i < A->rows; i++) {
      A->matrix[i][0] = v_vector[i];
    }
    A->matrix_type = CORRECT_MATRIX;
  }
}

int get_result(char *file_name, matrix_t **v, f_node **f, int *v_count,
               int *f_count) {
  int error = FAILURE;
  FILE *obj_file = NULL;
  error = get_size(&obj_file, v_count, f_count, file_name);
  if (error == SUCCESS) {
    (*v) = fill_matrix_array(obj_file, f, *v_count, f_count, file_name);
  }
  return error;
}

int get_matrix_multi(matrix_t **v, int v_count, matrix_t mult_matrix) {
  int error = FAILURE;
  if (mult_matrix.rows > 0 && mult_matrix.columns > 0) {
    matrix_t *tmp = &mult_matrix;
    for (int i = 1; i <= v_count; i++) {
      (*v)[i] = s21_mult_matrix(&mult_matrix, &((*v)[i]));
    }
    s21_remove_matrix(tmp);
    error = SUCCESS;
  }
  return error;
}

void free_all(matrix_t **v, f_node **f, int v_count, int f_count) {
  for (int i = 0; i <= v_count; i++) {
    s21_remove_matrix((*v) + i);
  }
  for (int i = 0; i < f_count; i++) {
    free(((*f) + i)->array_numbers);
  }
  free(*v);
  free(*f);
}

#if DEBUG == 1

void print_matr(matrix_t *A) {
  if (A->columns == 1 && A->rows == 4) {
    printf("V %.2lf %.2lf %.2lf %.2lf", A->matrix[0][0], A->matrix[1][0],
           A->matrix[2][0], A->matrix[3][0]);
    printf("\n");
  } else {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        if (j != 0) printf("\t");
        printf("%.2lf", A->matrix[i][j]);
      }
      printf("\n");
    }
  }
}

void print_array(f_node *f) {
  printf("F ");
  for (int i = 0; i < f->count_numbers; i++) {
    printf("%d ", f->array_numbers[i]);
  }
  printf("\n");
}

void print_v(matrix_t *v, f_node *f, int f_count) {
  for (int i = 0; i < f_count; i++) {
    for (int j = 0; j < f[i].count_numbers; j++) {
      int k = 0;
      printf("%d ", f[i].array_numbers[j]);
      k = f[i].array_numbers[j];
      printf("(v %lf %lf %lf) ", v[k].matrix[0][0], v[k].matrix[1][0],
             v[k].matrix[2][0]);
    }
    printf("\n");
  }
}

int main() {
  //  char file_name[] = "Logo_S21.obj";
  // char file_name[] = "./parsertest.obj";
  //  char file_name[] = "/Users/locie/projects/3D/lowpoly.obj";
  char file_name[] = "/Users/locie/projects/3D/parsertest.obj";
  f_node *f = NULL;
  matrix_t *v = NULL;
  int v_count = 0;
  int f_count = 0;
  double x = 2, y = 3, z = 4;

  get_result(file_name, &v, &f, &v_count, &f_count);

  get_matrix_multi(&v, v_count, xyz_axis_rotation(x, y, z));
  get_matrix_multi(&v, v_count, matrix_shifting(x, y, z));
  get_matrix_multi(&v, v_count, matrix_scaling(x, y, z));

  printf("v_count = %d   f_count = %d\n", v_count, f_count);
  for (int i = 1; i <= v_count; i++) {
    printf("%d.\t", i);
    print_matr(&v[i]);
  }

  printf("v_count = %d   f_count = %d\n", v_count, f_count);
  for (int i = 1; i <= v_count; i++) {
    printf("%d.\t", i);
    print_matr(&v[i]);
  }

  for (int i = 0; i < f_count; i++) {
    printf("%d.\t", i);
    print_array(&f[i]);
  }

  print_v(v, f, f_count);

  free_all(&v, &f, v_count, f_count);
  return 0;
}
#endif

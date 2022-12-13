#include "s21_matrix.h"

int is_correct_one(matrix_t* A);
int get_type(matrix_t* A);

matrix_t s21_create_matrix(int rows, int columns) {
  matrix_t result;
  result.columns = columns;
  result.rows = rows;
  if (rows > 0 && columns > 0) {
    result.matrix = (double**)calloc(rows, sizeof(double*));
    for (int i = 0; i < result.rows; i++) {
      result.matrix[i] = (double*)calloc(result.columns, sizeof(double));
    }
    result.matrix_type = ZERO_MATRIX;
  } else {
    result.matrix_type = INCORRECT_MATRIX;
    result.columns = 0;
    result.rows = 0;
    result.matrix = NULL;
  }
  return result;
}

void s21_remove_matrix(matrix_t* A) {
  if (A != NULL) {
    if (A->rows > 0 && A->columns > 0) {
      for (int i = 0; i < A->rows; i++) {
        free(A->matrix[i]);
      }
      free(A->matrix);
      A->matrix = NULL;
      A->columns = 0;
      A->rows = 0;
      A->matrix_type = INCORRECT_MATRIX;
    }
  }
}

matrix_t s21_mult_matrix(matrix_t* A, matrix_t* B) {
  matrix_t result;
  if (is_correct_one(A) == SUCCESS && is_correct_one(B) == SUCCESS &&
      A->columns == B->rows) {
    result = s21_create_matrix(A->rows, B->columns);
    for (int i = 0; i < result.rows; i++) {
      for (int j = 0; j < result.columns; j++) {
        result.matrix[i][j] = 0;
        for (int k = 0; k < A->columns; k++) {
          result.matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
        }
      }
    }
    s21_remove_matrix(B);
    result.matrix_type = get_type(&result);
  } else {
    result = s21_create_matrix(0, 0);
  }
  return result;
}

int is_correct_one(matrix_t* A) {
  int result = FAILURE;
  if (A != NULL) {
    if (A->rows > 0 && A->columns > 0 && A->matrix_type != INCORRECT_MATRIX) {
      result = SUCCESS;
    }
  }
  return result;
}

int get_type(matrix_t* A) {
  int result = INCORRECT_MATRIX;
  if (is_correct_one(A)) {
    int is_zero_diagonal = 1;
    int is_zero = 1;
    int is_idenity = 1;
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        if (i == j) {
          if (fabs(A->matrix[i][j] - 1.0) > EPS) is_idenity = 0;
          if (fabs(A->matrix[i][j]) > EPS) is_zero_diagonal = 0;
        } else {
          if (fabs(A->matrix[i][j]) > EPS) is_zero = 0;
        }
      }
    }
    if (is_zero && is_zero_diagonal)
      result = ZERO_MATRIX;
    else if (is_zero && is_idenity)
      result = IDENTITY_MATRIX;
    else
      result = CORRECT_MATRIX;
  }
  return result;
}

matrix_t x_axis_rotation(double angle) {
  matrix_t result = s21_create_matrix(4, 4);
  result.matrix[0][0] = result.matrix[3][3] = 1.0;
  result.matrix[1][1] = result.matrix[2][2] = cos(angle);
  result.matrix[2][1] = -sin(angle);
  result.matrix[1][2] = sin(angle);
  result.matrix_type = CORRECT_MATRIX;
  return result;
}

matrix_t y_axis_rotation(double angle) {
  matrix_t result = s21_create_matrix(4, 4);
  result.matrix[1][1] = result.matrix[3][3] = 1.0;
  result.matrix[2][2] = result.matrix[0][0] = cos(angle);
  result.matrix[0][2] = -sin(angle);
  result.matrix[2][0] = sin(angle);
  result.matrix_type = CORRECT_MATRIX;
  return result;
}

matrix_t z_axis_rotation(double angle) {
  matrix_t result = s21_create_matrix(4, 4);
  result.matrix[2][2] = result.matrix[3][3] = 1.0;
  result.matrix[1][1] = result.matrix[0][0] = cos(angle);
  result.matrix[1][0] = -sin(angle);
  result.matrix[0][1] = sin(angle);
  result.matrix_type = CORRECT_MATRIX;
  return result;
}

matrix_t xyz_axis_rotation(double ax, double ay, double az) {
  matrix_t x = x_axis_rotation(ax);
  matrix_t y = y_axis_rotation(ay);
  matrix_t z = z_axis_rotation(az);
  matrix_t xy = s21_mult_matrix(&x, &y);
  matrix_t result = s21_mult_matrix(&xy, &z);

  s21_remove_matrix(&x);
  s21_remove_matrix(&y);
  s21_remove_matrix(&z);
  s21_remove_matrix(&xy);
  return result;
}

matrix_t matrix_shifting(double a, double b, double c) {
  matrix_t result = s21_create_matrix(4, 4);
  for (int i = 0; i < result.rows; i++) {
    result.matrix[i][i] = 1.0;
  }
  result.matrix[0][3] = a;
  result.matrix[1][3] = b;
  result.matrix[2][3] = c;
  result.matrix_type = CORRECT_MATRIX;
  return result;
}

matrix_t matrix_scaling(double a, double b, double c) {
  matrix_t result = s21_create_matrix(4, 4);
  result.matrix[0][0] = a;
  result.matrix[1][1] = b;
  result.matrix[2][2] = c;
  result.matrix[3][3] = 1.0;
  result.matrix_type = CORRECT_MATRIX;
  return result;
}



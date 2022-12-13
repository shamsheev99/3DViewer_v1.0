#include <check.h>
#include <math.h>
#include <stdio.h>

#include "parser.h"
#include "s21_matrix.h"

int is_correct_one(matrix_t *A);
void matrix_print(matrix_t *A);
void print_test(char *name);
void fill_matrix_fotest(matrix_t *A);
void matrix_fill_array(matrix_t *A, const double *arr);
int s21_eq_matrix(matrix_t *A, matrix_t *B);
int is_correct(matrix_t *A, matrix_t *B);
double test_determ(double *f, int n);
void test_inverse_matr(double *arr, double *res, int n);
void test_rotation(matrix_t (*f)(double));
void test_rotation_2(matrix_t (*f)(double), double arg, double *arr_res);
void test_shift_scale(matrix_t (*f)(double, double, double), double arg1,
                      double arg2, double arg3, double *arr_res);

START_TEST(test_create_matrix) {
  print_test("#TEST1: s21_create_matrix, s21_eq_matrix, s21_remove_matrix");
  matrix_t A = s21_create_matrix(5, 7);
  matrix_t B = s21_create_matrix(7, 5);
  matrix_t C = s21_create_matrix(5, 7);
  matrix_t D = s21_create_matrix(-5, 7);
  matrix_t E = s21_create_matrix(1, 1);

  ck_assert_int_eq(A.matrix_type, ZERO_MATRIX);
  ck_assert_int_eq(B.matrix_type, ZERO_MATRIX);
  ck_assert_int_eq(C.matrix_type, ZERO_MATRIX);
  ck_assert_int_eq(D.matrix_type, INCORRECT_MATRIX);
  fill_matrix_fotest(&A);
  fill_matrix_fotest(&B);
  ck_assert_int_eq(s21_eq_matrix(&A, &B), FAILURE);
  ck_assert_int_eq(s21_eq_matrix(&A, &C), FAILURE);
  fill_matrix_fotest(&C);
  ck_assert_int_eq(s21_eq_matrix(&A, &C), SUCCESS);
  A.matrix[2][2] = 8.000005;
  ck_assert_int_eq(s21_eq_matrix(&A, &C), FAILURE);
  ck_assert_int_eq(s21_eq_matrix(&E, &E), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&C);
  s21_remove_matrix(&D);
  s21_remove_matrix(&E);

  ck_assert_int_eq(A.matrix_type, INCORRECT_MATRIX);
  ck_assert_ptr_eq(A.matrix, NULL);
  ck_assert_int_eq(B.matrix_type, INCORRECT_MATRIX);
  ck_assert_int_eq(C.matrix_type, INCORRECT_MATRIX);
  ck_assert_int_eq(D.matrix_type, INCORRECT_MATRIX);
}
END_TEST

START_TEST(test_mult_matrix) {
  print_test("#TEST4: s21_mult_matrix");
  matrix_t A = s21_create_matrix(4, 3);
  matrix_t B = s21_create_matrix(3, 4);
  matrix_t C = s21_create_matrix(2, 4);
  fill_matrix_fotest(&A);
  fill_matrix_fotest(&B);
  double arrC[] = {1, 2, -1, -2, 3, -3, 4, -4};
  matrix_fill_array(&C, arrC);

  matrix_t AB = s21_mult_matrix(&A, &B);
  matrix_t AC = s21_mult_matrix(&A, &C);
  matrix_t CA = s21_mult_matrix(&C, &A);
  matrix_t CB = s21_mult_matrix(&C, &B);

  ck_assert_int_eq(AC.matrix_type, INCORRECT_MATRIX);
  ck_assert_int_eq(CB.matrix_type, INCORRECT_MATRIX);

  matrix_t AB_res = s21_create_matrix(4, 4);
  matrix_t CA_res = s21_create_matrix(2, 3);

  double arrAB_res[] = {20, 23,  26,  29,  56,  68,  80,  92,
                        92, 113, 134, 155, 128, 158, 188, 218};
  matrix_fill_array(&AB_res, arrAB_res);
  double arrCA_res[] = {-18, -18, -18, -21, -21, -21};
  matrix_fill_array(&CA_res, arrCA_res);

  ck_assert_int_eq(s21_eq_matrix(&AB, &AB_res), SUCCESS);
  ck_assert_int_eq(s21_eq_matrix(&CA, &CA_res), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&C);

  s21_remove_matrix(&AB_res);
  s21_remove_matrix(&CA_res);

  s21_remove_matrix(&AB);
  s21_remove_matrix(&AC);
  s21_remove_matrix(&CA);
  s21_remove_matrix(&CB);
}
END_TEST

START_TEST(test_x_axis_rotation) {
  print_test("#TEST9: matrix_get_rotation_x");
  test_rotation(x_axis_rotation);
  test_rotation_2(x_axis_rotation, M_PI, (double[]){1.0, -1.0, -1.0, 1.0});
}
END_TEST

START_TEST(test_y_axis_rotation) {
  print_test("#TEST10: matrix_get_rotation_y");
  test_rotation(y_axis_rotation);
  test_rotation_2(y_axis_rotation, M_PI, (double[]){-1.0, 1.0, -1.0, 1.0});
}
END_TEST

START_TEST(test_z_axis_rotation) {
  print_test("#TEST11: matrix_get_rotation_x");
  test_rotation(z_axis_rotation);
  test_rotation_2(z_axis_rotation, M_PI, (double[]){-1.0, -1.0, 1.0, 1.0});
}

START_TEST(test_matrix_shifting) {
  print_test("#TEST12: matrix_get_shift");
  test_shift_scale(matrix_shifting, 1.0, 0.0, 0.0,
                   (double[]){2.0, 1.0, 1.0, 1.0});
  test_shift_scale(matrix_shifting, -1.0, 1.0, -3.0,
                   (double[]){0.0, 2.0, -2.0, 1.0});
  test_shift_scale(matrix_shifting, -1.0, 1.0, -3.0,
                   (double[]){0.0, 2.0, -2.0, 1.0});
}

START_TEST(test_matrix_scaling) {
  print_test("#TEST13: matrix_get_scale");
  test_shift_scale(matrix_scaling, 1.0, 2.0, 0.0,
                   (double[]){1.0, 2.0, 0.0, 1.0});
  test_shift_scale(matrix_scaling, -1.5, -2.0, -0.5,
                   (double[]){-1.5, -2.0, -0.5, 1.0});
  test_shift_scale(matrix_scaling, 1.1, 1.1, 1.1,
                   (double[]){1.1, 1.1, 1.1, 1.0});
}

START_TEST(test_parser) {
  char file_name[] = "./test_files/Logo_S21.obj";
  int v_result = 46;
  int f_result = 84;
  f_node *f = NULL;
  matrix_t *v = NULL;
  int v_count = 0;
  int f_count = 0;
  get_result(file_name, &v, &f, &v_count, &f_count);
  free_all(&v, &f, v_count, f_count);
  ck_assert_int_eq((v_count == v_result), (f_count == f_result));
}

START_TEST(test_matrx_multi) {
  char file_name[] = "./test_files/Logo_S21.obj";
  f_node *f = NULL;
  matrix_t *v = NULL;
  int v_count = 0;
  int f_count = 0;
  get_result(file_name, &v, &f, &v_count, &f_count);
  int err = get_matrix_multi(&v, v_count, xyz_axis_rotation(1, 1, 1));
  free_all(&v, &f, v_count, f_count);
  ck_assert_int_eq(err, SUCCESS);
}

int test_rdontos() {
  Suite *s = suite_create("All Part");
  TCase *tc = tcase_create("Test1");
  SRunner *sr = srunner_create(s);
  suite_add_tcase(s, tc);
  tcase_add_test(tc, test_create_matrix);
  tcase_add_test(tc, test_mult_matrix);
  tcase_add_test(tc, test_x_axis_rotation);
  tcase_add_test(tc, test_y_axis_rotation);
  tcase_add_test(tc, test_z_axis_rotation);
  tcase_add_test(tc, test_matrix_shifting);
  tcase_add_test(tc, test_matrix_scaling);
  tcase_add_test(tc, test_parser);
  tcase_add_test(tc, test_matrx_multi);

  srunner_run_all(sr, CK_ENV);
  int nf = srunner_ntests_failed(sr);
  srunner_free(sr);
  return nf;
}

int main(void) {
  int nf = 0;
  nf += test_rdontos();
  return nf == 0 ? 0 : 1;
}

void print_test(char *name) { printf("%s:\n", name); }

void fill_matrix_fotest(matrix_t *A) {
  if (A->rows > 0 && A->columns > 0) {
    double f = 0;
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        A->matrix[i][j] = f;
        f++;
      }
    }
    A->matrix_type = CORRECT_MATRIX;
  }
}

void matrix_fill_array(matrix_t *A, const double *arr) {
  if (A->rows > 0 && A->columns > 0) {
    int it = 0;
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        A->matrix[i][j] = arr[it];
        it++;
      }
    }
  }
}

void test_rotation(matrix_t (*f)(double)) {
  matrix_t A = f(0.0);
  matrix_t B = f(2.0 * M_PI);
  ck_assert_int_eq(s21_eq_matrix(&A, &B), SUCCESS);
  matrix_t v = s21_create_matrix(4, 1);
  double arr[] = {4.0, -5.0, 13.0, 1.0};
  matrix_fill_array(&v, arr);
  matrix_t C = f((2.0 * M_PI) / 15.0);
  for (int i = 0; i < 15; i++) {
    matrix_t t = s21_mult_matrix(&C, &v);
    s21_remove_matrix(&v);
    v = t;
  }
  matrix_t v_res = s21_create_matrix(4, 1);
  matrix_fill_array(&v_res, arr);
  ck_assert_int_eq(s21_eq_matrix(&v, &v_res), SUCCESS);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&C);
  s21_remove_matrix(&v);
  s21_remove_matrix(&v_res);
}

void test_rotation_2(matrix_t (*f)(double), double arg, double *arr_res) {
  matrix_t A = f(arg);
  matrix_t v = s21_create_matrix(4, 1);
  double arr[] = {1.0, 1.0, 1.0, 1.0};
  matrix_fill_array(&v, arr);
  matrix_t t = s21_mult_matrix(&A, &v);
  matrix_t v_res = s21_create_matrix(4, 1);
  matrix_fill_array(&v_res, arr_res);
  ck_assert_int_eq(s21_eq_matrix(&t, &v_res), SUCCESS);
  s21_remove_matrix(&A);
  s21_remove_matrix(&v);
  s21_remove_matrix(&t);
  s21_remove_matrix(&v_res);
}

void test_shift_scale(matrix_t (*f)(double, double, double), double arg1,
                      double arg2, double arg3, double *arr_res) {
  matrix_t A = f(arg1, arg2, arg3);
  matrix_t v = s21_create_matrix(4, 1);
  double arr[] = {1.0, 1.0, 1.0, 1.0};
  matrix_fill_array(&v, arr);
  matrix_t t = s21_mult_matrix(&A, &v);
  matrix_t v_res = s21_create_matrix(4, 1);
  matrix_fill_array(&v_res, arr_res);
  ck_assert_int_eq(s21_eq_matrix(&t, &v_res), SUCCESS);
  s21_remove_matrix(&A);
  s21_remove_matrix(&v);
  s21_remove_matrix(&t);
  s21_remove_matrix(&v_res);
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  int result = FAILURE;
  if (is_correct(A, B) == SUCCESS) {
    result = SUCCESS;
    for (int i = 0; i < A->rows && result == SUCCESS; i++) {
      for (int j = 0; j < A->columns && result == SUCCESS; j++) {
        if (fabs(A->matrix[i][j] - B->matrix[i][j]) > EPS) {
          result = FAILURE;
        }
      }
    }
  }
  return result;
}

int is_correct(matrix_t *A, matrix_t *B) {
  int result = FAILURE;
  if (A != NULL && B != NULL) {
    if ((A->rows > 0 && A->columns > 0 && A->matrix_type != INCORRECT_MATRIX) &&
        (B->rows > 0 && B->columns > 0 && B->matrix_type != INCORRECT_MATRIX) &&
        (A->rows == B->rows && A->columns == B->columns)) {
      result = SUCCESS;
    }
  }
  return result;
}

#ifndef SRC_S21_MATRIX_H_
#define SRC_S21_MATRIX_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define EPS 1e-8
// Сравнение должно происходить вплоть до 7 знака после запятой включительно.

#define SUCCESS 1
#define FAILURE 0

typedef enum {
  CORRECT_MATRIX = 0,
  INCORRECT_MATRIX = 1,
  IDENTITY_MATRIX = 2,
  ZERO_MATRIX = 3,
  VECTOR_MATRIX = 4
} matrix_type_t;

// CORRECT_MATRIX - корректная матрица,
// INCORRECT_MATRIX - при ошибках в действиях с матрицами,
// IDENTITY_MATRIX - единичная матрица,
// ZERO_MATRIX - нулевая матрица.

typedef struct matrix_struct {
  double** matrix;
  int rows;
  int columns;
  matrix_type_t matrix_type;
} matrix_t;

// Создание матрицы размером m × n. Поле matrix_type должно инициализироваться
// значением ZERO_MATRIX.
matrix_t s21_create_matrix(int rows, int columns);
// Очистка матриц (remove_matrix)
void s21_remove_matrix(matrix_t* A);
// Произведением матрицы A = m × k на матрицу B = k × n называется матрица C = m
// × n = A × B размера m × n, элементы которой определяются равенством C(i,j) =
// A(i,1) × B(1,j) + A(i,2) × B(2,j) + … + A(i,k) × B(k,j).
matrix_t s21_mult_matrix(matrix_t* A, matrix_t* B);
// Вращение вокруг оси OX в трёхмерном пространстве, где aх - угол поворота
matrix_t x_axis_rotation(double angle);
// Вращение вокруг оси OY в трёхмерном пространстве, где ay - угол поворота
matrix_t y_axis_rotation(double angle);
// Вращение вокруг оси OZ в трёхмерном пространстве, где az - угол поворота
matrix_t z_axis_rotation(double angle);
// Вращение вокруг произвольной оси XYZ в трёхмерном пространстве
matrix_t xyz_axis_rotation(double ax, double ay, double az);
// Перемещение, где a, b и с - величины по x, y и z, на которые необходимо
// переместить исходную точку
matrix_t matrix_shifting(double a, double b, double c);
// масштабирование, где  a, b и с - коэффициенты масштабирования соответственно
// по осям OX, OY и OZ.
matrix_t matrix_scaling(double a, double b, double c);

#endif  // SRC_S21_MATRIX_H_

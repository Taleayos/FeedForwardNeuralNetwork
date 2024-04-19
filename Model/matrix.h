#ifndef MATRIX_H_
#define MATRIX_H_

#include <cmath>
#include <deque>
#include <iostream>

class Matrix {
 public:
  Matrix();                    // базовый конструктор
  Matrix(int rows, int cols);  // параметризированный конструктор
  Matrix(const Matrix& other);  // конструктор копирования
  Matrix(Matrix&& other) noexcept;  // конструктор переноса
  ~Matrix();                        // деструктор

  // методы доступа и изменения приватных полей
  int GetRows() const;
  int GetCols() const;
  void SetRows(int rows);
  void SetCols(int cols);

  // операции
  void Print();
  double GetMaxValue();
  double GetMinValue();
  void SetValue(double value);
  void SetValue(std::deque<double>& values);
  void SetRandomValue(int max_value = 0);
  bool EqMatrix(const Matrix& other) const noexcept;
  void SumMatrix(const Matrix& other);
  void SubMatrix(const Matrix& other);
  void SumNumber(const double num) noexcept;
  void MulNumber(const double num) noexcept;
  void MulMatrix(const Matrix& other);
  void Hadamard(const s21::Matrix& other);  // поэлементное произведение Адамара
  Matrix Transpose() noexcept;

  // перегружаемые операторы
  Matrix operator+(const Matrix& other) const;
  Matrix operator-(const Matrix& other) const;
  Matrix operator*(const Matrix& other) const;
  Matrix operator*(const double multiplicator) const noexcept;
  friend Matrix operator*(const double multiplicator, const Matrix& other);
  bool operator==(const Matrix& other) const noexcept;
  Matrix& operator=(const Matrix& other) noexcept;
  Matrix& operator=(Matrix&& other) noexcept;
  Matrix& operator+=(const Matrix& other);
  Matrix& operator+=(const double num) noexcept;
  Matrix& operator-=(const Matrix& other);
  Matrix& operator*=(const Matrix& other);
  Matrix& operator*=(const double multiplicator) noexcept;
  Matrix& operator%=(
      const s21::Matrix& other);  // поэлементное произведение Адамара
  double& operator()(int row, int col);
  double& operator()(const int row, const int col) const;

 private:
  // аттрибуты
  int rows_, cols_;  // количество строк и столбцов в матрице
  double** matrix_;  // указатель на область памяти, где расположена матрица
  // вспомогательные методы
  void CreateMatrix();  // выделение области памяти для матрицы
  void RemoveMatrix();  // очистка области памяти для матрицы
};
#endif  // MATRIX_H_

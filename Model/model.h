#ifndef MODEL_H
#define MODEL_H

#include <algorithm>
#include <cmath>
#include <deque>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <random>
#include <sstream>
#include <vector>

#include "matrix.h"

struct Metrics {
  // доля объектов, для которых правильно предсказан класс
  double average_accuracy;
  // доля правильно предсказанных положительных объектов
  // среди всех объектов, предсказанных положительным классом
  double precision;  // точность
  // доля правильно найденных положительных объектов
  // среди всех объектов положительного класса
  double recall;  // полнота
  // среднее гармоническое
  double f_measure;  // f-мера
};

struct Precision {
  double total_size;  // общее количество прогнозов
  double accuracy = 0;  // количества правильных прогнозов
  double tp = 0;  // кол-во правильно предсказанных положительных объектов
  double fp = 0;
  double tn = 0;
  double fn = 0;
};

class Model {
 public:
  Model() = default;
  ~Model() = default;
  // функция проверки наличия нейронной сети
  bool isEmpty();
  // функция вывода информации о нейронной сети
  std::string GetInfo();
  // функция создания и инициализации нейронной сети;
  // size - вектор со значениями глубины каждого слоя
  void InitNeuronNet(std::vector<int> size);
  void InitNeuronNet(std::vector<std::deque<double>>& dataset,
                     int in_size = 784);
  // тестирование нейронной сети
  Metrics Testing(std::string filename, double test_value);
  void SetValueInput(std::deque<double>& values);
  // обучение нейронной сети
  void Learning(std::string filename, std::vector<int> layers, bool normalize,
                int k_fold = 1);
  // прямое распространение
  // normalize - признак необходимости нормализации весов и смещений
  void Forward(bool normalize);

  // обратное распространение:
  // target_index - индекс правильного ответа в векторе
  // alfa - скорость обучения
  // граф одного слоя сети: X (W, B) ---> T = X * W + B ---> H = F(T)
  void Backward(int target_index, double alfa);

  // поиск индекса элемента с максимальным значением
  int GetMaxIndex();
  // Очистка нейронной сети (удаление всех ее слоев)
  void ClearNeuronNet();
  int ReturnWeightsLAmount() { return weights_.size(); };
  // запись информации в файл (метод пока не сохраняет значения беасов)!
  void SaveWeightsToFile(std::string path);
  // чтение весов из файла
  void LoadWeightsFromFile(std::string path);
  // вычисление коэффициентов функции y = kx + c
  // для отображения отрезка [x1, x2] в отрезок [y1, y2]
  std::pair<double, double> GetCoef(double x1, double x2, double y1, double y2);
  // расчет метрик результатов тестирования
  Metrics Metrics_Calc(Precision p);
  // нормализация входных данных
  void Normalize(Matrix& m, double d1, double d2);
  // нормализация входных данных
  void Normalize(Matrix& m);

 protected:
  // перемешивание вектора данных
  void Shuffle(std::vector<std::deque<double>>& dataset);
  // парсер
  void Parcer(std::vector<std::deque<double>>& dataset, std::string filename);

 private:
  std::vector<Matrix> neurons_;  // слои нейронной сети
  std::vector<Matrix> weights_;  // веса слоев сети
  std::vector<Matrix> biases_;   // смещения слоев сети

  void Sigmoid(Matrix& m);
  // функция активации
  double Sigmoid(double x);
  // производная функции Сигмоида
  void Sigmoid_Deriv(Matrix& m);
  // функция активации
  void Softmax(Matrix& m);
  // логарифмическая функция потерь
  double Cross_Entropy(int index);
  //  std::vector<std::deque<double>> dataset;  // данные для обучения

  // отладочная функция вывода всех слоев нейронной сети
  void PrintNeuronNet();
  //отладочная функция вывода буквы в правильной ориентации
  void PrintInputLayer(std::deque<double>& values, int width = 4);
  void PrintInputLayer(Matrix& values, int width = 10);

  // отладочная функция вывода буквы по ее индексу в файле
  void PrintSymbol(int index);
};

#endif  // MODEL_H

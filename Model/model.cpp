#include "model.h"

bool Model::isEmpty() {
  return (neurons_.size() == 0 && weights_.size() == 0 && biases_.size() == 0);
}

// функция вывода информации о нейронной сети
std::string Model::GetInfo() {
  std::stringstream ss;
  ss << (isEmpty() ? "Перцептрон не создан! " : "Перцептрон: ");
  for (auto& layer : neurons_) ss << layer.GetCols() << " ";
  return ss.str();
}
// функция создания и инициализации нейронной сети;
// size - вектор со значениями глубины каждого слоя
void Model::InitNeuronNet(std::vector<int> size) {
  // ClearNeuronNet();
  if (neurons_.empty()) {
    for (auto v : size) {
      Matrix layer;
      layer.SetRows(1);
      layer.SetCols(v);
      layer.SetRandomValue(255);
      neurons_.push_back(layer);
    }
  }
  if (weights_.empty()) {
    for (size_t i = 1; i < neurons_.size(); ++i) {
      Matrix layer;
      layer.SetRows(neurons_[i - 1].GetCols());
      layer.SetCols(neurons_[i].GetCols());
      layer.SetRandomValue();
      weights_.push_back(layer);
    }
  }
  if (biases_.empty()) {
    for (size_t i = 1; i < neurons_.size(); ++i) {
      Matrix layer;
      layer.SetRows(1);
      layer.SetCols(neurons_[i].GetCols());
      layer.SetValue(1);
      biases_.push_back(layer);
    }
  }
}

void Model::InitNeuronNet(std::vector<std::deque<double>>& dataset,
                               int in_size) {
  ClearNeuronNet();
  Matrix input_layer(1, in_size);
  neurons_.push_back(input_layer);
  for (auto v : dataset) {
    Matrix layer;
    int flag = static_cast<int>(v[0]);  // признак весов или смещений
    int rows = static_cast<int>(v[1]);
    int cols = static_cast<int>(v[2]);
    layer.SetRows(rows);
    layer.SetCols(cols);
    for (int i = 0; i < rows; ++i)
      for (int j = 0; j < cols; ++j) layer(i, j) = v[i * cols + j + 3];

    if (flag == 0)
      weights_.push_back(layer);
    else {
      biases_.push_back(layer);
      neurons_.push_back(layer);
    }
  }
}

// отладочная функция вывода всех слоев нейронной сети
void Model::PrintNeuronNet() {
  for (auto layer : neurons_) {
    std::cout << "слой:" << std::endl;
    layer.Print();
  }
  for (auto layer : weights_) {
    std::cout << "веса слоя:" << std::endl;
    layer.Print();
  }
  //    for (auto layer : biases_) {
  //      std::cout << "смещения слоя:" << std::endl;
  //      layer.Print();
  //    }
}

//отладочная функция вывода буквы в правильной ориентации
void Model::PrintInputLayer(std::deque<double>& values, int width) {
  for (int j = 0; j < 28; ++j) {
    for (int i = 0; i < 28; ++i) {
      std::cout << std::setw(width);
      if (values[i * 28 + j] > 0)
        std::cout << values[i * 28 + j];
      else
        std::cout << " ";
    }
    std::cout << std::endl;
  }
}

void Model::PrintInputLayer(Matrix& values, int width) {
  for (int j = 0; j < 28; ++j) {
    for (int i = 0; i < 28; ++i) {
      std::cout << std::setw(width);
      if (values(0, i * 28 + j) > 0)
        std::cout << values(0, i * 28 + j);
      else
        std::cout << " ";
    }
    std::cout << std::endl;
  }
}

//отладочная функция вывода буквы по ее индексу в файле
void Model::PrintSymbol(int index) {
  std::string name =
      "/Users/twilawal/CPP7_MLP-1/src/emnist-letters-mapping.txt";
  std::ifstream file(name);
  if (!file) throw std::invalid_argument("File opening error");
  while (!file.eof()) {
    std::string line;
    std::getline(file, line);
    std::istringstream iss(line);
    std::vector<int> int_values{std::istream_iterator<int>(iss),
                                std::istream_iterator<int>()};
    if (int_values[0] == index) {
      std::cout << index << " " << (char)int_values[1] << " "
                << (char)int_values[2] << std::endl;
      break;
    }
  }
  if (file) file.close();
}

// тестирование нейронной сети
Metrics Model::Testing(std::string filename, double test_value) {
  std::vector<std::deque<double>> dataset;
  Parcer(dataset, filename);
  Shuffle(dataset);

  int tests_amount = static_cast<int>(dataset.size() * test_value);
  std::cout << "test_value = " << test_value << std::endl;
  std::cout << "dataset_size = " << dataset.size() << std::endl;
  std::cout << "tests_amount = " << tests_amount << std::endl;

  Precision metrics;
  metrics.total_size = tests_amount;
  for (int i = 0; i < tests_amount; ++i) {
    if (dataset[i].size() == 785) {
      double value = dataset[i][0];
      int target_index = static_cast<int>(value) - 1;

      dataset[i].pop_front();
      neurons_[0].SetValue(dataset[i]);
      Normalize(neurons_[0]);
      dataset[i].push_front(value);

      Forward(false);

      Matrix& out_layer = neurons_[neurons_.size() - 1];

      if (GetMaxIndex() == target_index) metrics.accuracy++;

      for (int j = 0; j < out_layer.GetCols(); ++j) {
        if (GetMaxIndex() == target_index) {
          if (out_layer(0, target_index) > 0.5)
            metrics.tp++;
          else
            metrics.fp++;
        } else {
          if (out_layer(0, j) > 0.5)
            metrics.fn++;
          else
            metrics.tn++;
        }
      }
    }
  }
  return Metrics_Calc(metrics);
}

void Model::SetValueInput(std::deque<double>& values) {
  neurons_[0].SetValue(values);
}

// обучение нейронной сети
void Model::Learning(std::string filename, std::vector<int> layers,
                          bool normalize, int k_fold) {
  double alfa = 0.1;
  InitNeuronNet(layers);  // для отладки
  std::vector<std::deque<double>> dataset;
  Parcer(dataset, filename);
  Shuffle(dataset);
  std::vector<int> idx(26, 0);  // для отладки, количество отгаданных букв
  int count = 0;
  int k = 0;
  for (auto& data : dataset) {
    if (++k % k_fold == 0 && k_fold > 1) continue;
    double value = data[0];
    int target_index = static_cast<int>(value) - 1;

    data.pop_front();
    neurons_[0].SetValue(data);
    Normalize(neurons_[0]);
    data.push_front(value);

    Forward(normalize);
    if (normalize == true) normalize = false;

    idx[GetMaxIndex()]++;
    if (GetMaxIndex() == target_index)
      count++;
    else {
      Backward(target_index, alfa);
    }
  }
  //  std::cout << " правильных ответов " << count << " "
  //            << 100.0 * count / dataset.size() << "%" << std::endl;
  //  for (auto amount : idx) std::cout << amount << " ";
  //  std::cout << std::endl;
}

// прямое распространение
// normalize - признак необходимости нормализации весов и смещений
void Model::Forward(bool normalize) {
  size_t n = neurons_.size() - 1;
  for (size_t i = 1; i <= n; ++i) {
    neurons_[i] = neurons_[i - 1];
    neurons_[i] *= weights_[i - 1];
    neurons_[i] += biases_[i - 1];
    // подогнать значения нейронов к аргументам Сигмоиды на отерзке [-5, 5]
    if (normalize == true) {
      double max = neurons_[i].GetMaxValue();
      double min = neurons_[i].GetMinValue();
      double k = GetCoef(min, max, -5, 5).first;   // y = kx + c
      double c = GetCoef(min, max, -5, 5).second;  // y = kx + c
      weights_[i - 1] *= k;
      biases_[i - 1] *= k;
      biases_[i - 1] += c;
      neurons_[i] *= k;
      neurons_[i] += c;
    }
    if (i < n)
      Sigmoid(neurons_[i]);
    else
      Softmax(neurons_[i]);
  }
}

// обратное распространение:
// target_index - индекс правильного ответа в векторе
// alfa - скорость обучения
// граф одного слоя сети: X (W, B) ---> T = X * W + B ---> H = F(T)
void Model::Backward(int target_index, double alfa) {
  int n = weights_.size();  // количество матриц весов меньше на 1 кол-ва
                            // векторов нейронов
  Matrix Y(1, neurons_[n].GetCols());  // полный вектор правильного ответа
  Y.SetValue(0);
  Y(0, target_index) = 1;

  Matrix dE_dT = neurons_[n];
  dE_dT -= Y;  // dE_dT = Z - Y для выходного слоя
  // пройти и обновить все матрицы весов
  for (int i = n - 1; i >= 0; i--) {
    Matrix& X = neurons_[i];
    Matrix& W = weights_[i];
    Matrix& B = biases_[i];
    Matrix T = X;
    T *= W;
    T += B;
    if (i < n - 1) {  // для скрытых слоев dE_dT = dE_dH(X) % F_deriv(T)
      Sigmoid_Deriv(T);
      dE_dT %= T;  // поэлементное произведение Адамара
    }
    // реализовать формулы dE_dW = Xt * dE_dT; dE_dB = dE_dT
    Matrix dE_dW = X.Transpose();
    dE_dW *= dE_dT;
    Matrix dE_dB = dE_dT;
    // подготовить входные данные dE_dX(H) = dE_dT * Wt для следующего слоя
    Matrix Wt = W.Transpose();
    dE_dT *= Wt;
    // обновить веса по формулам W = W - alfa * dE_dW; B = B - alfa * dE_dB
    dE_dW *= alfa;
    W -= dE_dW;
    dE_dB *= alfa;
    B -= dE_dB;
  }
}

// поиск индекса элемента с максимальным значением
int Model::GetMaxIndex() {
  Matrix& out_layer = neurons_[neurons_.size() - 1];
  double max = out_layer(0, 0);
  int m = out_layer.GetRows();
  //    std::cout << "m = " << m << std::endl;
  int n = out_layer.GetCols();
  //    std::cout << "n = " << n << std::endl;
  int index = 0;
  for (int i = 0; i < m; ++i)
    for (int j = 0; j < n; ++j)
      if (out_layer(i, j) > max) {
        max = out_layer(i, j);
        index = i * n + j;
      }
  return index;
}

// Очистка нейронной сети (удаление всех ее слоев)
void Model::ClearNeuronNet() {
  neurons_.clear();
  weights_.clear();
  biases_.clear();
}
// запись информации в файл (метод пока не сохраняет значения беасов)!
void Model::SaveWeightsToFile(std::string path) {
  std::ofstream out;
  out.open(path);
  if (out.is_open())
    for (int num = 0; num < 2; num++) {
      std::vector<Matrix>& values = (num == 0 ? weights_ : biases_);
      for (auto m : values) {
        out << num << " ";
        out << m.GetRows() << " ";
        out << m.GetCols() << " ";
        for (int i = 0; i < m.GetRows(); ++i)
          for (int j = 0; j < m.GetCols(); ++j) out << m(i, j) << " ";
        out << std::endl;
      }
    }
  out.close();
}

// чтение весов из файла
void Model::LoadWeightsFromFile(std::string path) {
  std::vector<std::deque<double>> dataset;
  Parcer(dataset, path);
  ClearNeuronNet();
  InitNeuronNet(dataset);
}

void Model::Sigmoid(Matrix& m) {
  for (int i = 0; i < m.GetRows(); ++i)
    for (int j = 0; j < m.GetCols(); ++j) m(i, j) = Sigmoid(m(i, j));
}
// функция активации
double Model::Sigmoid(double x) { return 1.0 / (1.0 + exp(-x)); }
// производная функции Сигмоида
void Model::Sigmoid_Deriv(Matrix& m) {
  for (int i = 0; i < m.GetRows(); ++i)
    for (int j = 0; j < m.GetCols(); ++j) {
      m(i, j) = Sigmoid(m(i, j));
      m(i, j) = m(i, j) * (1 - m(i, j));
    }
}
// функция активации
void Model::Softmax(Matrix& m) {
  double sum = 0;
  for (int i = 0; i < m.GetRows(); ++i)
    for (int j = 0; j < m.GetCols(); ++j) sum += exp(m(i, j));
  for (int i = 0; i < m.GetRows(); ++i)
    for (int j = 0; j < m.GetCols(); ++j) m(i, j) = exp(m(i, j)) / sum;
}
// логарифмическая функция потерь
double Model::Cross_Entropy(int index) {
  Matrix& out_layer = neurons_[neurons_.size() - 1];
  return -log(out_layer(0, index));
}
// вычисление коэффициентов функции y = kx + c
// для отображения отрезка [x1, x2] в отрезок [y1, y2]
std::pair<double, double> Model::GetCoef(double x1, double x2, double y1,
                                              double y2) {
  double k = (y2 - y1) / (x2 - x1);
  double c = y1 - k * x1;
  return {k, c};
}

// перемешивание вектора данных
void Model::Shuffle(std::vector<std::deque<double>>& dataset) {
  auto rd = std::random_device{};
  auto rng = std::default_random_engine{rd()};
  std::shuffle(std::begin(dataset), std::end(dataset), rng);
}

// парсер
void Model::Parcer(std::vector<std::deque<double>>& dataset,
                        std::string filename) {
  //    std::cout << "Parcing started " << std::endl;
  //            int i = 0;
  std::string name = filename;
  std::ifstream file(name);
  if (!file) throw std::invalid_argument("File opening error");
  while (!file.eof()) {
    //                if (++i == 5) break;
    std::string line;
    std::getline(file, line);
    std::replace(line.begin(), line.end(), ',', ' ');
    std::istringstream iss(line);
    std::deque<double> values{std::istream_iterator<double>(iss),
                              std::istream_iterator<double>()};
    if (values.size() != 0) dataset.push_back(values);
  }
  if (file) file.close();
  //    std::cout << "Parcing ended " << std::endl;
}

// расчет метрик результатов тестирования
Metrics Model::Metrics_Calc(Precision p) {
  Metrics m;
  m.average_accuracy = p.accuracy / p.total_size;
  m.precision = p.tp / (p.tp + p.fp);
  m.recall = p.tp / (p.tp + p.fn);
  m.f_measure = 2 * ((m.precision * m.recall) / (m.precision + m.recall));
  return m;
}

// нормализация входных данных
void Model::Normalize(Matrix& m, double d1, double d2) {
  double max = m.GetMaxValue();
  double min = m.GetMinValue();
  for (int i = 0; i < m.GetRows(); ++i)
    for (int j = 0; j < m.GetCols(); ++j)
      m(i, j) = (m(i, j) - min) * (d2 - d1) / (max - min) + d1;
}
// нормализация входных данных
void Model::Normalize(Matrix& m) {
  for (int i = 0; i < m.GetRows(); ++i)
    for (int j = 0; j < m.GetCols(); ++j) m(i, j) /= 255.0;
}

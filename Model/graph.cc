#include "graph.h"

void GraphModel::InitNeuronNet(int hide_layers,
                                    std::deque<double> &input_layer,
                                    std::vector<int> l_size) {
  std::deque<double> layer;
  if (neurons_.size() == 0) {
    for (int i = 0; i <= hide_layers + 1; ++i) {
      for (int j = 0; j < l_size[i]; ++j) {
        layer.push_back(1);
      }
      neurons_.push_back(layer);
      layer.clear();
    }
  }
  LoadInputLayer(input_layer);
  if (weights_.empty()) {
    for (int i = 1; i <= hide_layers + 1; ++i) {
      s21::Matrix w(neurons_[i - 1].size(), neurons_[i].size());
      w.SetRandomValue();
      weights_.push_back(w);
    }
  }
  if (biases_.empty()) {
    for (int i = 1; i <= hide_layers + 1; i++) {
      for (size_t j = 0; j < neurons_[i].size(); ++j) layer.push_back(1);
      biases_.push_back(layer);
      layer.clear();
    }
  }
}

void GraphModel::Forward() {
  for (size_t i = 1; i < neurons_.size(); ++i) {
    NeuronsWeights(i);
    for (size_t j = 0; j < neurons_[i].size(); ++j)
      neurons_[i][j] += biases_[i - 1][j];
    if (i == neurons_.size() - 1) {
      Softmax(i);
    } else {
      Sigmoida(i);
    }
  }
}

void GraphModel::NeuronsWeights(int index) {
  for (size_t k = 0; k < neurons_[index].size(); ++k) {
    neurons_[index][k] = 0;
    for (size_t j = 0; j < neurons_[index - 1].size(); ++j) {
      neurons_[index][k] += neurons_[index - 1][j] * weights_[index - 1](j, k);
    }
  }
}

int GraphModel::GetMaxIndex() {
  int index = 0;
  double max = 0;
  for (size_t i = 0; i < neurons_[neurons_.size() - 1].size(); ++i) {
    if (neurons_[neurons_.size() - 1][i] > max) {
      max = neurons_[neurons_.size() - 1][i];
      index = i;
    }
  }
  return index;
}

void GraphModel::LoadInputLayer(std::deque<double> &input_layer) {
  if (input_layer.size() == 784) {
    neurons_[0].clear();
    for (int i = 0; i < 784; ++i) {
      neurons_[0].push_back(input_layer[i]);
    }
  }
}

void GraphModel::Sigmoida(int index) {
  for (size_t i = 0; i < neurons_[index].size(); ++i) {
    neurons_[index][i] = Sigmoid(neurons_[index][i]);
  }
}

double GraphModel::Sigmoid(double x) { return 1.0 / (1.0 + exp(-x)); }

void GraphModel::Softmax(int index) {
  double exp_sum = 0;
  for (size_t i = 0; i < neurons_[index].size(); ++i) {
    exp_sum += exp(neurons_[index][i]);
  }
  for (size_t i = 0; i < neurons_[index].size(); ++i) {
    neurons_[index][i] =
        exp(neurons_[index][i]) / exp_sum;  // Производная тогда  = x * (1 - x)
  }
};

Metrics GraphModel::Testing(std::string filename, double test_value) {
  std::vector<std::deque<double>> dataset;
  Parcer(dataset, filename);
  Shuffle(dataset);
  size_t tests_amount = static_cast<int>(dataset.size() * test_value);
  Precision metrics;
  metrics.total_size = tests_amount;
  for (size_t i = 0; i < tests_amount; ++i) {
    double value = dataset[i][0];
    int target_index = static_cast<int>(value) - 1;
    dataset[i].pop_front();
    Normalise(dataset[i]);
    LoadInputLayer(dataset[i]);
    dataset[i].push_front(value);
    Forward();
    if (GetMaxIndex() == target_index) metrics.accuracy++;
    for (size_t j = 0; j < neurons_[neurons_.size() - 1].size(); ++j) {
      if (GetMaxIndex() == target_index) {
        if (neurons_[neurons_.size() - 1][target_index] > 0.5)
          metrics.tp++;
        else
          metrics.fp++;
      } else {
        if (neurons_[neurons_.size() - 1][j] > 0.5)
          metrics.fn++;
        else
          metrics.tn++;
      }
    }
  }
  return Metrics_Calc(metrics);
}
void GraphModel::Normalise(std::deque<double> &dataset) {
  for (size_t i = 0; i < dataset.size(); ++i) {
    dataset[i] /= 255;
  }
}

bool GraphModel::IsEmptyGraph() { return neurons_.empty(); }

std::string GraphModel::GetInfoGraph() {
  std::stringstream ss;
  ss << ("Перцептрон: ");
  for (auto &layer : neurons_) ss << layer.size() << " ";
  return ss.str();
}

//// чтение весов из файла
void GraphModel::LoadWeightsFromFile(std::string path) {
  if (!IsEmptyGraph()) ClearNN();
  std::vector<std::deque<double>> dataset;
  std::deque<double> input_layer, b_layer;
  std::vector<int> input;
  Parcer(dataset, path);
  for (auto v : dataset) {
    s21::Matrix layer;
    int flag = static_cast<int>(v[0]);  // признак весов или смещений
    int rows = static_cast<int>(v[1]);
    int cols = static_cast<int>(v[2]);
    if (flag == 0) {
      input.push_back(rows);
      layer.SetRows(rows);
      layer.SetCols(cols);
      for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
          layer(i, j) = v[i * cols + j + 3];
        }
      }
      weights_.push_back(layer);

    } else {
      for (int i = 0; i < cols; ++i) b_layer.push_back(v[i + 3]);
      biases_.push_back(b_layer);
      b_layer.clear();
    }
  }
  input.push_back(26);
  InitNeuronNet(input.size() - 2, input_layer, input);
}

void GraphModel::ClearNN() {
  neurons_.clear();
  weights_.clear();
  biases_.clear();
}

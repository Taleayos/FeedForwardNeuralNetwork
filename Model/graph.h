#include <cmath>
#include <deque>
#include <iostream>
#include <vector>

#include "matrix.h"
#include "model.h"

class GraphModel : Model {
 public:
  GraphModel() = default;
  ~GraphModel() = default;

  void InitNeuronNet(int hide_layers, std::deque<double> &input_layer,
                     std::vector<int> l_size = {0});
  void Forward();
  void NeuronsWeights(int index);
  int GetMaxIndex();
  void LoadInputLayer(std::deque<double> &input_layer);
  Metrics Testing(std::string filename, double test_value);
  void Normalise(std::deque<double> &dataset);
  bool IsEmptyGraph();
  std::string GetInfoGraph();
  void SaveWeightsToFile(std::string path);
  void LoadWeightsFromFile(std::string path);
  void ClearNN();

 private:
  void Sigmoida(int index);
  double Sigmoid(double x);
  void Softmax(int index);

  std::vector<std::deque<double>> neurons_;
  std::vector<s21::Matrix> weights_;
  std::vector<std::deque<double>> biases_;
};

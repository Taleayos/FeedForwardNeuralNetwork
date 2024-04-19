#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <deque>
#include <iostream>

#include "../Model/graph.h"
#include "../Model/model.h"

class Controller {
 public:
  Controller(Model *model, GraphModel *graph_model)
      : model_(model), graph_(graph_model) {}
  ~Controller() = default;

  void ClearNeuronNet() { model_->ClearNeuronNet(); }
  void InitNeuronNet(std::vector<int> size) { model_->InitNeuronNet(size); }
  //  void PrintNeuronNet() { model_->PrintNeuronNet(); }
  void Forward() { model_->Forward(false); }
  int GetMaxIndex() { return model_->GetMaxIndex(); }
  void SaveWeightsToFile(std::string path) { model_->SaveWeightsToFile(path); }
  Metrics Testing(std::string filename, double test_value) {
    return model_->Testing(filename, test_value);
  }
  void LoadWeightsFromFile(std::string path) {
    model_->LoadWeightsFromFile(path);
  }
  int ReturnWeightsLAmount() { return model_->ReturnWeightsLAmount(); }
  void Learning(std::string filename, std::vector<int> layers, bool normalize,
                int k_fold = 1) {
    model_->Learning(filename, layers, normalize, k_fold);
  }

  void SetValueInput(std::deque<double> &values) {
    model_->SetValueInput(values);
  }

  //  void PrintInputLayer(std::deque<double> &values) {
  //    model_->PrintInputLayer(values);
  //  }
  bool NeuronNetIsEmpty() { return model_->isEmpty(); }

  std::string GetInfo() { return model_->GetInfo(); }

  void InitNeuronNetGraph(int hide_layers, std::deque<double> &input_layer,
                          std::vector<int> l_size = {0}) {
    graph_->InitNeuronNet(hide_layers, input_layer, l_size);
  }
  void ForwardGraph() { graph_->Forward(); }
  int GetMaxIndexGraph() { return graph_->GetMaxIndex(); }
  Metrics TestingGraph(std::string filename, double test_value) {
    return graph_->Testing(filename, test_value);
  }
  //  void LearningGraph(int hide_layers, double alfa, std::string filename,
  //                     std::vector<int> l_size, int k_fold = 1) {
  //    graph_->Learning(hide_layers, alfa, filename, l_size, k_fold);
  //  }
  bool IsEmptyGraph() { return graph_->IsEmptyGraph(); }
  std::string GetInfoGraph() { return graph_->GetInfoGraph(); };
  void SaveWeightsGraph(std::string path) { graph_->SaveWeightsToFile(path); };
  void LoadWeightsGraph(std::string path) {
    graph_->LoadWeightsFromFile(path);
  };

 private:
  Model *model_;
  GraphModel *graph_;
};


#endif  // CONTROLLER_H

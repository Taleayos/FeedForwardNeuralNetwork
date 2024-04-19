#ifndef VIEW_H
#define VIEW_H

#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QObject>
#include <QOpenGLWidget>
#include <QWidget>
#include <chrono>
#include <deque>

#include "../Controller/controller.h"
#include "QtGifImage/src/gifimage/qgifimage.h"
#include "widget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class View;
}
QT_END_NAMESPACE

class View : public QMainWindow {
  Q_OBJECT

 public:
  View(Controller *controller, QWidget *parent = nullptr);
  ~View() = default;
  std::deque<double> GetDataFromImg(const QImage &img);
  void Recognize(bool learning, bool matrix_style);

  //Для нового окна
  void InitLearningRate(double x);
  void InitInputDeque(double x);

 private slots:
  void on_clear_widget_clicked();

  void on_spinBox_valueChanged(int arg1);

  void on_graff_style_clicked();

  void on_matrix_style_clicked();

  void on_recognize_clicked();

  void on_Download_BMP_clicked();

  void on_start_testing_clicked();

  void on_save_weights_clicked();

  void on_load_weights_clicked();

  void on_start_learning_clicked();

  void on_checkBox_stateChanged(int arg1);
  void on_show_learning_results_clicked();

 private:
  Ui::View *ui_;
  Controller *controller_;
  Widget *widget_;
  int layers_ = 1;

  bool matrix_form_ = true;
  int in_neur_amount_ = 28 * 28;
  int out_neuron_amount_ = 26;
  // plot
  double xBegin, xEnd, h, X;  // Х временная переменная для заполнения вектора
  std::vector<double> Y = {100};
  int N;  //количество точек
  QVector<double> x, y;
  void Clear_Metrics();
  std::vector<int> InitVector();
  void PrintMetrics(Metrics);

  //  //
  //  double learning_rate_ = 0.01;
  //  std::deque<double> layers_size_;
};

#endif  // VIEW_H

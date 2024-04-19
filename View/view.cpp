#include "view.h"

#include <stdlib.h>

#include <QtWidgets>

#include "ui_view.h"
#include "widget.h"

View::View(Controller *controller, QWidget *parent)
    : QMainWindow(parent), ui_(new Ui::View), controller_(controller) {
  ui_->setupUi(this);
  widget_ = new Widget;
  ui_->input->setScene(widget_);
  widget_->setSceneRect(0, 0, ui_->input->width() - 20,
                        ui_->input->height() - 20);
  ui_->label_info->setText(QString::fromStdString(
      matrix_form_ ? controller_->GetInfo() : controller_->GetInfoGraph()));
}

void View::on_clear_widget_clicked() {
  widget_->clear();
  ui_->output->clear();
}

void View::on_spinBox_valueChanged(int arg) { layers_ = arg; }

void View::on_graff_style_clicked() { matrix_form_ = false; }

void View::on_matrix_style_clicked() { matrix_form_ = true; }

void View::on_recognize_clicked() {
  if (controller_->NeuronNetIsEmpty() && controller_->IsEmptyGraph()) {
    QMessageBox::information(this, "Распознавание символа",
                             "Нейронная сеть не создана и не обучена!");
  } else {
    int index = 0;
    QPixmap pixmap = ui_->input->grab();
    QImage img = pixmap.scaled(28, 28).toImage();
    std::deque<double> neurons_values = GetDataFromImg(img);
    if (matrix_form_) {
      controller_->SetValueInput(neurons_values);
      controller_->Forward();
      index = controller_->GetMaxIndex();
    } else {
      controller_->InitNeuronNetGraph(layers_, neurons_values);
      controller_->ForwardGraph();
      index = controller_->GetMaxIndexGraph();
    }
    index += 65;
    char let = index;
    QString letter(let);
    ui_->output->setText(letter);
  }
}

std::deque<double> View::GetDataFromImg(const QImage &img) {
  std::deque<double> values;
  double res = 0;
  for (int i = 0; i < 28; i++) {
    for (int j = 0; j < 28; ++j) {
      QColor current_color = img.pixel(i, j);
      int red, green, black;
      current_color.getRgb(&red, &green, &black);
      res = (double)(red + green + black) / 3.0;
      res == 23 ? values.push_back(0) : values.push_back(res / 255);
    }
  }
  return values;
}

void View::on_Download_BMP_clicked() {
  QString path = QFileDialog::getOpenFileName(
      this, "Choose BMP file", "/Users/", "BMP Files All Files (*.*)");
  QImage image(path);
  if (image.isNull()) {
    QMessageBox::information(this, "Image Viewer", "Error Displaying image");
    return;
  }
  QGraphicsPixmapItem *item =
      new QGraphicsPixmapItem(QPixmap::fromImage(image).scaled(280, 280));
  widget_->addItem(item);
}

void View::on_save_weights_clicked() {
  QString path = QFileDialog::getSaveFileName(0, "Select Output file",
                                              QDir::currentPath());
  if (!path.isEmpty()) {
    controller_->SaveWeightsToFile(path.toStdString());
    QMessageBox::information(this, "Сохранение весов",
                             "Сохранение весов выполнено успешно");
  }
}

void View::on_start_testing_clicked() {
  Clear_Metrics();
  if (controller_->NeuronNetIsEmpty() && controller_->IsEmptyGraph())
    QMessageBox::information(this, "Ошибка наличия перцептрона",
                             "Перцептрон не создан и не обучен!");
  else {
    auto start = std::chrono::steady_clock::now();
    QString path =
        QFileDialog::getOpenFileName(this, "Choose Test file", "/Users/",
                                     "Files (*test.csv);; All Files (*.*)");
    if (!path.isEmpty()) {
      Metrics Test_final =
          matrix_form_ ? controller_->Testing(path.toStdString(),
                                              ui_->test_value->value())
                       : controller_->TestingGraph(path.toStdString(),
                                                   ui_->test_value->value());
      auto end = std::chrono::steady_clock::now();
      auto duration =
          std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
      PrintMetrics(Test_final);
      ui_->total_time_value->setText(QString::number(duration));
    }
  }
}

void View::on_load_weights_clicked() {
  QString path = QFileDialog::getOpenFileName(this, "Choose Test file",
                                              QDir::currentPath());
  if (!path.isEmpty()) {
    matrix_form_ ? controller_->LoadWeightsFromFile(path.toStdString())
                 : controller_->LoadWeightsGraph(path.toStdString());
    QMessageBox::information(this, "Загрузка весов",
                             "Загрузка весов выполнена успешно");
    ui_->label_info->setText(QString::fromStdString(
        matrix_form_ ? controller_->GetInfo() : controller_->GetInfoGraph()));
  }
}

void View::on_start_learning_clicked() {
  QMessageBox::StandardButton reply = QMessageBox::Yes;

  if (matrix_form_ ? controller_->NeuronNetIsEmpty() == false
                   : controller_->IsEmptyGraph() == false) {
    reply = QMessageBox::question(this, "Имеется обученный перцептрон",
                                  "Переобучить его?",
                                  QMessageBox::Yes | QMessageBox::No);
  }

  if (reply == QMessageBox::Yes) {
    QString path =
        QFileDialog::getOpenFileName(this, "Choose Train file", "/Users/",
                                     "Files (*train.csv);; All Files (*.*)");
    if (!path.isEmpty()) {
      size_t num_epochs = ui_->epochs->value();
      std::vector<int> layers = InitVector();
      bool normalize = true;
      controller_->ClearNeuronNet();
      for (size_t k = 1; k <= num_epochs; ++k) {
        std::cout << "эпоха " << k << std::endl;
        Clear_Metrics();
        controller_->Learning(path.toStdString(), layers, normalize,
                              ui_->groups->value());
        QString path2 = path;
        QString::iterator it = path.end() - 9;
        path.erase(it, it + 9);
        path += "test.csv";
        QString path3 = path;
        path = path2;
        auto start = std::chrono::steady_clock::now();
        Metrics Test_final = controller_->Testing(
            path3.toStdString(),
            ui_->checkBox->isChecked()
                ? (static_cast<double>(ui_->groups->value() - 1) /
                   static_cast<double>(ui_->groups->value()))
                : ui_->test_value->value());
        auto end = std::chrono::steady_clock::now();
        auto duration =
            std::chrono::duration_cast<std::chrono::seconds>(end - start)
                .count();

        PrintMetrics(Test_final);
        ui_->total_time_value->setText(QString::number(duration));
        double lala = 100 - (Test_final.average_accuracy * 100);
        Y.push_back(lala);
      }
      y = QVector<double>(Y.begin(), Y.end());
      ui_->label_info->setText(QString::fromStdString(
          matrix_form_ ? controller_->GetInfo() : controller_->GetInfoGraph()));
    }
  }
}

void View::PrintMetrics(Metrics Test_final) {
  ui_->average_accuracy_value->setText(
      QString::number(Test_final.average_accuracy));
  ui_->f_measure_value->setText(QString::number(Test_final.f_measure));
  ui_->precision_value->setText(QString::number(Test_final.precision));
  ui_->recall_value->setText(QString::number(Test_final.recall));
}

std::vector<int> View::InitVector() {
  std::vector<int> layers;
  int res = (matrix_form_ ? 784 : 785);
  for (int i = 0; i <= layers_; ++i) {
    if (i == 0) layers.push_back(res);
    if (i == 1) layers.push_back(64);
    if (i == 2) layers.push_back(64);
    if (i == 3) layers.push_back(64);
    if (i == 4) layers.push_back(64);
    if (i == 5) layers.push_back(32);
  }
  layers.push_back(26);
  return layers;
}

void View::on_show_learning_results_clicked() {
  x.clear();
  h = 1;  //шаг
  xBegin = 0;
  xEnd = ui_->epochs->value();
  N = (xEnd - xBegin) / h + 2;
  ui_->widget->xAxis->setRange(xBegin, xEnd);
  ui_->widget->yAxis->setRange(0, 100);
  for (X = xBegin; X <= xEnd; X += h) {
    x.push_back(X);
  }
  ui_->widget->addGraph();
  ui_->widget->graph(0)->addData(x, y);
  ui_->widget->replot();
  ui_->widget->setInteraction(QCP::iRangeZoom, true);
  ui_->widget->setInteraction(QCP::iRangeDrag, true);
}

void View::on_checkBox_stateChanged(int arg1) {
  ui_->label_10->setEnabled(true);
  ui_->groups->setEnabled(true);
}

void View::Clear_Metrics() {
  ui_->average_accuracy_value->clear();
  ui_->f_measure_value->clear();
  ui_->precision_value->clear();
  ui_->recall_value->clear();
  ui_->total_time_value->clear();
  y.clear();
}

#ifndef WIDGET_H
#define WIDGET_H

#include <QColor>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGuiApplication>
#include <QMouseEvent>
#include <QObject>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QPainter>
#include <QRectF>
#include <QTimer>
#include <QtOpenGL>

class Widget : public QGraphicsScene {
  Q_OBJECT

 public:
  explicit Widget(QObject *parent = nullptr);
  ~Widget();

 private:
  QPointF previousPoint_;
  void mousePressEvent(QGraphicsSceneMouseEvent *event);
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
};

#endif  // WIDGET_H

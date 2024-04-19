#include "widget.h"

//Widget::Widget(QWidget *parent) : QOpenGLWidget{parent} {
//    setMouseTracking(true);
//}

//void Widget::paintEvent(QPaintEvent *event)
//{
//    Q_UNUSED(event);
//    if (mouse_tracking == true) {
//    QWidget::paintEvent(event);
//    QPainter painter(this);
////    const QRect & rect = event->rect();

//    QPoint cursor_pos = mapFromGlobal(QCursor::pos());
//    qreal width = 20;
//    QPen pen(Qt::red, width, Qt::SolidLine,Qt::RoundCap, Qt::RoundJoin);
//        painter.setPen(pen);
//        painter.drawLine(previousPoint_, cursor_pos);
//        previousPoint_ = cursor_pos;
//    }

//}

//void Widget::mousePressEvent(QMouseEvent *event) {
//    QWidget::mousePressEvent(event);
//    if (event) mouse_tracking = true;
//    previousPoint_ = mapFromGlobal(QCursor::pos());
//}

//void Widget::mouseMoveEvent(QMouseEvent *event)
//{
//    QWidget::mouseMoveEvent(event);
//        update();
//}

//void Widget::mouseReleaseEvent(QMouseEvent *event) {
//    QWidget::mouseReleaseEvent(event);
//    if (event) mouse_tracking = false;
//}

//void Widget::ClearWidget() {
//    update();
//}

Widget::Widget(QObject *parent) : QGraphicsScene(parent) {}

Widget::~Widget() {}

void Widget::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  addEllipse(event->scenePos().x(), event->scenePos().y(), 20, 20,
             QPen(Qt::white), QBrush(Qt::white));
  previousPoint_ = event->scenePos();
}

void Widget::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  addLine(previousPoint_.x(), previousPoint_.y(), event->scenePos().x(),
          event->scenePos().y(),
          QPen(Qt::white, 20, Qt::SolidLine, Qt::RoundCap));
  previousPoint_ = event->scenePos();
}

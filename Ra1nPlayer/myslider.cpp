#include "myslider.h"

MySlider::MySlider(QWidget* parent)
    :QSlider(parent)
{}

MySlider::~MySlider()
{}

void MySlider::mousePressEvent(QMouseEvent* event){
        QSlider::mousePressEvent(event);
    if (event->button() == Qt::LeftButton) {
        //获取鼠标的位置，这里并不能直接从ev中取值（因为如果是拖动的话，鼠标开始点击的位置没有意义了）
        position_ = event->pos().x() / (double)width();
        position_ = position_ * maximum();
        setValue(position_);
        event->accept();
        //触发滑动条改变信号
        emit SliderChange(position_);
    }
}

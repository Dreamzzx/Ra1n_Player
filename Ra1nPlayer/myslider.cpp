#include "myslider.h"

MySlider::MySlider(QWidget* parent)
    :QSlider(parent)
{}

MySlider::~MySlider()
{}

void MySlider::mousePressEvent(QMouseEvent* event){
        QSlider::mousePressEvent(event);
    if (event->button() == Qt::LeftButton) {
        //��ȡ����λ�ã����ﲢ����ֱ�Ӵ�ev��ȡֵ����Ϊ������϶��Ļ�����꿪ʼ�����λ��û�������ˣ�
        position_ = event->pos().x() / (double)width();
        position_ = position_ * maximum();
        setValue(position_);
        event->accept();
        //�����������ı��ź�
        emit SliderChange(position_);
    }
}

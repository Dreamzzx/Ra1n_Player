#pragma once

#include <QObject>
#include <QSlider>
#include <QMouseEvent>

class MySlider : public QSlider
{
	Q_OBJECT

public:
	MySlider(QWidget*parent = nullptr);
	~MySlider();

signals:
	void SliderChange(double position_);
private:
	void mousePressEvent(QMouseEvent * event) override;

	double position_ = 0.0;
};

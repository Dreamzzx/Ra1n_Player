#pragma once

#include <QWidget>
#include "ui_ctrlbar.h"

class CtrlBar : public QWidget
{
	Q_OBJECT

public:
	CtrlBar(QWidget *parent = nullptr);
	~CtrlBar();

	void SetTime(int event,double playtime);

private:

	void InitUi();

	void SliderChange(double position)
	{
		emit Play_Seek(position);
	}

	void VolumeSliderChange(float position)
	{
		emit Volume_Change(position);
	}
signals:
	void EndBtnClick();
	void FullBtnClick();
	void Play_Or_PauseBtnClick();
	void VoiceBtnClick();
	void SigPlayOrPause();
	void FastForwardBtnClick();
	void BackOffdBtnClick();
	void Play_Seek(double position);
	void Volume_Change(float positon);
	void Speed_Change(const QString str);

private:
	Ui::CtrlBarClass ui;
};

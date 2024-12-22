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
	void ChangePlay_Or_PauseBtnStyle();
	void ChangeVoiceBtnStyle();
	void isVoiceMute();

private:

	void InitUi();

	void SliderChange(double position)
	{
		emit Play_Seek(position);
	}

	void VolumeSliderChange(float position)
	{
		voice_ = position;
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


	QIcon voice_btn;
	QIcon playStyleBtn;
	QIcon pauseStyleBtn;
	QIcon endBtn;
	QIcon rewindBtn;
	QIcon fast_forwardBtn;
	QIcon voice_x;
	QIcon voice_1;
	QIcon voice_2;

	float voice_;
	bool playBtn = false;
	bool voice_mute = false;
};

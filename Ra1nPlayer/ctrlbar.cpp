#include "ctrlbar.h"
#include "ffmsg_queue.h"
#include "ffmsg.h"

CtrlBar::CtrlBar(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	InitUi();
}

CtrlBar::~CtrlBar()
{}

void CtrlBar::SetTime(int event,double playtime)
{
	int minutes = playtime / 60.0;
	int seconds = playtime - minutes*60;
	char buf1[256] = { 0 };
	switch (event)
	{
	case MP_SET_PLAY_TIME:
		sprintf(buf1, "%02d:%02d", minutes, seconds);
		ui.played_time->setText(buf1);
		ui.ProgressBar->setValue((int)playtime);
		break;
	case MP_INIT_TIME:
		ui.played_time->setText("00:00");
		ui.end_time->setText("00:00");
		ui.ProgressBar->setValue(0);
		break;
	case MP_SET_DURATION:
		sprintf(buf1, "%02d:%02d", minutes, seconds);
		ui.end_time->setText(buf1);
		ui.ProgressBar->setRange(0,(int)playtime);
	}
}

void CtrlBar::InitUi()
{
	//connect
	connect(ui.play_or_pause_Btn, &QPushButton::clicked, this,&CtrlBar::Play_Or_PauseBtnClick);
	connect(ui.endBtn, &QPushButton::clicked, this, &CtrlBar::EndBtnClick);
	connect(ui.voiceBtn, &QPushButton::clicked, this, &CtrlBar::VoiceBtnClick);
	connect(ui.fast_forwardBtn,&QPushButton::clicked, this, &CtrlBar::FastForwardBtnClick);
	connect(ui.back_offBtn, &QPushButton::clicked, this, &CtrlBar::FastForwardBtnClick);
	
	connect(ui.ProgressBar, &MySlider::SliderChange, this, &CtrlBar::SliderChange);
	connect(ui.volume_progressBar, &MySlider::SliderChange, this, &CtrlBar::Volume_Change);
	connect(ui.speed_comboBox, &QComboBox::textActivated, this, &CtrlBar::Speed_Change);

	//±¶ËÙ
	ui.speed_comboBox->addItem("0.25");
	ui.speed_comboBox->addItem("0.5");
	ui.speed_comboBox->addItem("0.75");
	ui.speed_comboBox->addItem("1.0");
	ui.speed_comboBox->addItem("1.25");
	ui.speed_comboBox->addItem("1.5");
	ui.speed_comboBox->addItem("1.75");
	ui.speed_comboBox->addItem("2.0");
	ui.speed_comboBox->setCurrentIndex(3);
}


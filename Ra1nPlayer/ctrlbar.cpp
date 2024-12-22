#include "ctrlbar.h"
#include "ffmsg_queue.h"
#include "ffmsg.h"

CtrlBar::CtrlBar(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.volume_progressBar->setValue(50);

	//倍速
	ui.speed_comboBox->addItem("0.25");
	ui.speed_comboBox->addItem("0.5");
	ui.speed_comboBox->addItem("0.75");
	ui.speed_comboBox->addItem("1.0");
	ui.speed_comboBox->addItem("1.25");
	ui.speed_comboBox->addItem("1.5");
	ui.speed_comboBox->addItem("1.75");
	ui.speed_comboBox->addItem("2.0");
	ui.speed_comboBox->setCurrentIndex(3);

	//connect
	connect(ui.play_or_pause_Btn, &QPushButton::clicked, this, &CtrlBar::Play_Or_PauseBtnClick);
	connect(ui.play_or_pause_Btn, &QPushButton::clicked, this, &CtrlBar::ChangePlay_Or_PauseBtnStyle);
	connect(ui.endBtn, &QPushButton::clicked, this, &CtrlBar::EndBtnClick);
	connect(ui.voiceBtn, &QPushButton::clicked, this, &CtrlBar::VoiceBtnClick);
	connect(ui.voiceBtn, &QPushButton::clicked, this, &CtrlBar::isVoiceMute);
	connect(ui.fast_forwardBtn, &QPushButton::clicked, this, &CtrlBar::FastForwardBtnClick);
	connect(ui.rewindBtn, &QPushButton::clicked, this, &CtrlBar::FastForwardBtnClick);

	connect(ui.ProgressBar, &MySlider::SliderChange, this, &CtrlBar::SliderChange);
	connect(ui.volume_progressBar, &MySlider::SliderChange, this, &CtrlBar::Volume_Change);
	connect(ui.volume_progressBar, &MySlider::SliderChange, this, &CtrlBar::ChangeVoiceBtnStyle);
	connect(ui.speed_comboBox, &QComboBox::textActivated, this, &CtrlBar::Speed_Change);
	

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

void CtrlBar::ChangePlay_Or_PauseBtnStyle()
{
	if (playBtn) {
		ui.play_or_pause_Btn->setIcon(playStyleBtn);
		playBtn = false;
	}
	else {
		ui.play_or_pause_Btn->setIcon(pauseStyleBtn);
		playBtn = true;
	}
}

void CtrlBar::isVoiceMute() {
	if (!voice_mute) {
		ui.volume_progressBar->setValue(0);
		ui.voiceBtn->setIcon(voice_x);
	}
	else {
		ui.volume_progressBar->setValue(voice_);
		ChangeVoiceBtnStyle();
	}
	voice_mute = !voice_mute;
}

void CtrlBar::ChangeVoiceBtnStyle()
{
	//如果静音的话 就不在更改声音值 而是存取上次的变量
	if (!voice_mute && (ui.volume_progressBar->value()>3)) {
	voice_ = ui.volume_progressBar->value();
	}
	if (voice_ >= 60) {
		ui.voiceBtn->setIcon(voice_2);
	}
	else if (voice_ < 60 && voice_>3) {
		ui.voiceBtn->setIcon(voice_1);
	}
	else
		ui.voiceBtn->setIcon(voice_x);
}

void CtrlBar::InitUi()
{
	//图片样式
	voice_btn = QIcon(":/ctrlbar/sources/ctrlbar/volume-1.svg");
	playStyleBtn = QIcon(":/ctrlbar/sources/ctrlbar/play.svg");
	pauseStyleBtn = QIcon(":/ctrlbar/sources/ctrlbar/pause.svg");
	endBtn = QIcon(":/ctrlbar/sources/ctrlbar/stop-circle.svg");
	rewindBtn = QIcon(":/ctrlbar/sources/ctrlbar/rewind.svg");
	fast_forwardBtn = QIcon(":/ctrlbar/sources/ctrlbar/fast-forward.svg");
	voice_x = QIcon(":/ctrlbar/sources/ctrlbar/volume-x.svg");
	voice_1 = QIcon(":/ctrlbar/sources/ctrlbar/volume-1.svg");
	voice_2 = QIcon(":/ctrlbar/sources/ctrlbar/volume-2.svg");

	ui.voiceBtn->setIcon(voice_btn);
	ui.play_or_pause_Btn->setIcon(playStyleBtn);
	ui.endBtn->setIcon(endBtn);
	ui.rewindBtn->setIcon(rewindBtn);
	ui.fast_forwardBtn->setIcon(fast_forwardBtn);
}


#include "title.h"
#include "globalhelper.h"
Title::Title(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	InitUi();
}

Title::~Title()
{}

QPushButton* Title::GetFullButton()
{
	return ui.fullBtn;
}

void Title::InitUi()
{
	//Menu
	MenuBar_ = new QMenu(this);
	OpenFile_ = new QAction(u8"打开文件", this);
	MenuBar_->addAction(OpenFile_);

	//connect
	connect(ui.fullBtn, &QPushButton::clicked, this, &Title::FullBtnClick);
	connect(ui.closeBtn,&QPushButton::clicked, this, &Title::CloseBtnClick);
	connect(ui.minBtn,  &QPushButton::clicked, this, &Title::MinBtnClick);
	connect(ui.ctrlMenuBtn, &QPushButton::clicked, this, &Title::MenuClick);
	connect(this->OpenFile_, &QAction::triggered, this, &Title::OpenFileClick);
	//qss
	this->setStyleSheet(GlobalHelper::GetQssStr(":/title/qss/title.css"));
	
	QIcon icon_close(":/title/sources/title/x.svg");
	QIcon icon_full(":/title/sources/title/maximize-2.svg");
	QIcon icon_min(":/title/sources/title/minus.svg");

	ui.closeBtn->setIcon(icon_close);
	ui.fullBtn->setIcon(icon_full);
	ui.minBtn->setIcon(icon_min);
}

void Title::ShowMenuBar()
{
	QPoint point = QCursor::pos();
	MenuBar_->exec(point);
}

void Title::SetName(const QString name)
{
	ui.movie_name->setText(name);
}

void Title::InitName()
{
	ui.movie_name->setText("");
}

#pragma once

#include <QWidget>
#include "ui_title.h"
#include <QAction>
#include <QMenu>
#include <QMouseEvent>

class Title : public QWidget
{
	Q_OBJECT

public:
	Title(QWidget *parent = nullptr);
	~Title();

	void ShowMenuBar();
	void SetName(const QString name);
	void InitName();
	QPushButton* GetFullButton();

private :
	void InitUi();
	
signals:
	void MenuClick();
	void MinBtnClick();
	void FullBtnClick();
	void CloseBtnClick();
	void OpenFileClick();
private:
	Ui::TitleClass ui;

	QMenu* MenuBar_ = nullptr;
	QAction*  OpenFile_= nullptr;

};

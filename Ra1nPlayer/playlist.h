#pragma once

#include <QWidget>
#include "ui_playlist.h"

class PlayList : public QWidget
{
	Q_OBJECT

public:
	PlayList(QWidget *parent = nullptr);
	~PlayList();

private:
	Ui::PlayListClass ui;
};

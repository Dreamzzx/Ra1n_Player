#pragma once

#include <QListWidget>
#include "ui_medialist.h"

class MediaList : public QListWidget
{
	Q_OBJECT

public:
	MediaList(QWidget *parent = nullptr);
	~MediaList();

private:
	Ui::MediaListClass ui;
};

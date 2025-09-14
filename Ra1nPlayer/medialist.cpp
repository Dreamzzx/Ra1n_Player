#include "medialist.h"

MediaList::MediaList(QWidget *parent)
	: QListWidget(parent)
{
	ui.setupUi(this);
}

MediaList::~MediaList()
{}

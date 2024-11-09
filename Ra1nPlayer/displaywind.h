#pragma once

#include <QWidget>
#include <SDL.h>
#include "ra1nplayer.h"

class DisplayWind  : public QWidget
{
	Q_OBJECT

public:
	DisplayWind(QWidget *parent);
	~DisplayWind();

	void setWinID(WId id);
	int getWinID();

	int Draw(const Frame* frame);
	void destory();
private:

	int win_width_;
	int win_height_;
	int video_width_;
	int video_height_;

	int showId_;
	ImageScaler* img_scaler_ = nullptr;
};

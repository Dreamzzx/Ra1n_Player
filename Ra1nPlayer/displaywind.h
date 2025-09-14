#pragma once

#include <QWidget>
#include <SDL.h>
#include "ra1nplayer.h"
#include "OpenglRender.h"


#define OPENGLRENDER

class DisplayWind  : public QWidget
{
	Q_OBJECT

public:
	DisplayWind(QWidget *parent);
	~DisplayWind();

	void setWinID(WId id);
	int getWinID();


#ifdef OPENGLRENDER
	int openglDraw(const Frame* frame);
#elif
	int sdlDraw(const Frame* frame);
#endif // OPENGLRENDER

	void destory();
private:

	int win_width_;
	int win_height_;
	int video_width_;
	int video_height_;

	int showId_;
#ifdef OPENGLRENDER
	std::unique_ptr<OpenglRender> openglRender;
#elif
	ImageScaler* img_scaler_ = nullptr;
#endif // OPENGLRENDER
};

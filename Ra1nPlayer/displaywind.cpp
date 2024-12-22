#include "displaywind.h"

DisplayWind::DisplayWind(QWidget *parent)
{}

DisplayWind::~DisplayWind()
{}

void DisplayWind::setWinID(WId id)
{
	this->showId_ = id;
}

int DisplayWind::getWinID()
{
	return showId_;
}

int DisplayWind::Draw(const Frame * vp)
{
	AVFrame* sp = nullptr;
	uint8_t* buffer = nullptr;
	if (!vp)
	{
		//Ã»ÓÐÖ¡äÖÈ¾Ò»Ö¡ºÚÆÁ
		SDL_SetRenderDrawColor(img_scaler_->render, 0, 0, 0, 255);
		SDL_RenderClear(img_scaler_->render);
		SDL_RenderPresent(img_scaler_->render);
		return -1;
	}
	if (!sp)
	{
		sp = av_frame_alloc();
		int buffer_size = av_image_get_buffer_size(AV_PIX_FMT_RGB24, vp->width, vp->height, 32);
		buffer = (uint8_t*)av_malloc(buffer_size * sizeof(uint8_t));
		av_image_fill_arrays(sp->data, sp->linesize, buffer, AV_PIX_FMT_RGB24, vp->width, vp->height, 32);
	
	}

		win_width_ = width();
		win_height_ = height();

	if (!img_scaler_)
	{
		img_scaler_ = new ImageScaler();
	}

	if (!img_scaler_->sws_ctx)
	{
		video_width_ = vp->width;
		video_height_ = vp->height;
		img_scaler_->srcFMT = (AVPixelFormat)vp->format;
	}

	img_scaler_->rect.x = 0;
	img_scaler_->rect.y = 0;
	img_scaler_->rect.w = video_width_;
	img_scaler_->rect.h = video_height_;

	if (!img_scaler_->window)
	{
		img_scaler_->window = SDL_CreateWindowFrom((const void*)getWinID());
		//img_scaler_->window = SDL_CreateWindow("1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);
	}
		
		if (!img_scaler_->render)
			img_scaler_->render = SDL_CreateRenderer(img_scaler_->window, -1, 0);

		if (!img_scaler_->texture)
			img_scaler_->texture = SDL_CreateTexture(img_scaler_->render,
				SDL_PIXELFORMAT_RGB24,
				SDL_TEXTUREACCESS_STREAMING,
				video_width_, video_height_);
		
		img_scaler_->dstFMT = AV_PIX_FMT_RGB24;

		if (!img_scaler_->sws_ctx)
		{
		    img_scaler_->sws_ctx = sws_getContext(video_width_, video_height_, (AVPixelFormat)vp->format,
				win_width_,win_height_, AV_PIX_FMT_RGB24,SWS_BILINEAR,NULL,NULL,NULL);
		}

		int ret = sws_scale(img_scaler_->sws_ctx,vp->frame->data, vp->frame->linesize,
				  0, video_height_,sp->data,sp->linesize);
		
		SDL_UpdateTexture(img_scaler_->texture, &img_scaler_->rect, sp->data[0],sp->linesize[0]);
		SDL_RenderClear(img_scaler_->render);
		SDL_RenderCopy(img_scaler_->render, img_scaler_->texture, NULL, &img_scaler_->rect);
		SDL_RenderPresent(img_scaler_->render);
		
		av_frame_unref(sp);
		av_freep(&buffer);

		if (img_scaler_->sws_ctx)
		{
		sws_freeContext(img_scaler_->sws_ctx);
		img_scaler_->sws_ctx = nullptr;
		}

		return 0;
}

void DisplayWind::destory()
{
	if (img_scaler_->texture)
	{
		SDL_DestroyTexture(img_scaler_->texture);
		img_scaler_->texture = nullptr;
	}

	if (img_scaler_->render)
	{
		SDL_DestroyRenderer(img_scaler_->render);
		img_scaler_->render = nullptr;
	}

	if (img_scaler_->window)
	{
		SDL_DestroyWindow(img_scaler_->window);
		img_scaler_->window = nullptr;
	}

	if (img_scaler_)
	{
		free(img_scaler_);
		img_scaler_ = nullptr;
	}
}

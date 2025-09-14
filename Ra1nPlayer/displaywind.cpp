#include "displaywind.h"
#include <QVBoxLayout>


DisplayWind::DisplayWind(QWidget *parent)
{
#ifdef OPENGLRENDER
    openglRender.reset(new OpenglRender(this));

    // 设置布局，让OpenGL渲染器填满整个窗口
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(openglRender.get());
    setLayout(layout);

    // 连接信号槽，确保OpenGL操作在主线程中执行
    connect(openglRender.get(), &OpenglRender::frameReady,
        openglRender.get(), &OpenglRender::onFrameReady, Qt::QueuedConnection);
#endif
}

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
#ifdef OPENGLRENDER
int DisplayWind::openglDraw(const Frame* frame) {
    if (frame) {
        openglRender->setYUVData(frame->frame);
    }
    return 0;
}
#elif
int DisplayWind::sdlDraw(const Frame* vp)
{
    AVFrame* sp = nullptr;
    uint8_t* buffer = nullptr;

    if (!img_scaler_)
    {
        img_scaler_ = new ImageScaler();
    }

    // 确保窗口和渲染器已正确初始化
    if (!img_scaler_->window)
    {
        img_scaler_->window = SDL_CreateWindowFrom((const void*)getWinID());
        if (!img_scaler_->window) {
            // 错误处理
            return -1;
        }
    }

    if (!img_scaler_->render)
    {
        img_scaler_->render = SDL_CreateRenderer(img_scaler_->window, -1, 0);
        if (!img_scaler_->render) {
            // 错误处理
            return -1;
        }
    }

    if (!vp)
    {
        // 没有帧渲染黑屏
        SDL_SetRenderDrawColor(img_scaler_->render, 52, 52, 52, 0);
        SDL_RenderClear(img_scaler_->render);
        SDL_RenderPresent(img_scaler_->render);
        return -1;
    }

    // 只在首次或视频尺寸/格式变化时创建AVFrame和缓冲区
    if (!sp || video_width_ != vp->width || video_height_ != vp->height ||
        img_scaler_->srcFMT != (AVPixelFormat)vp->format)
    {
        if (sp) {
            av_frame_free(&sp);
            av_freep(&buffer);
        }

        sp = av_frame_alloc();
        int buffer_size = av_image_get_buffer_size(AV_PIX_FMT_RGB24, vp->width, vp->height, 32);
        buffer = (uint8_t*)av_malloc(buffer_size * sizeof(uint8_t));
        av_image_fill_arrays(sp->data, sp->linesize, buffer, AV_PIX_FMT_RGB24, vp->width, vp->height, 32);

        video_width_ = vp->width;
        video_height_ = vp->height;
        img_scaler_->srcFMT = (AVPixelFormat)vp->format;

        // 释放旧的SWS上下文
        if (img_scaler_->sws_ctx) {
            sws_freeContext(img_scaler_->sws_ctx);
            img_scaler_->sws_ctx = nullptr;
        }
    }

    win_width_ = width();
    win_height_ = height();

    // 只在需要时创建SWS上下文
    if (!img_scaler_->sws_ctx)
    {
        img_scaler_->sws_ctx = sws_getContext(video_width_, video_height_, img_scaler_->srcFMT,
            video_width_, video_height_, AV_PIX_FMT_RGB24,
            SWS_BILINEAR, NULL, NULL, NULL);
        if (!img_scaler_->sws_ctx) {
            // 错误处理
            return -1;
        }
    }

    // 确保纹理已创建或尺寸匹配
    if (!img_scaler_->texture ||
        video_width_ != win_width_ ||
        video_height_ != win_height_)
    {
        if (img_scaler_->texture) {
            SDL_DestroyTexture(img_scaler_->texture);
        }

        img_scaler_->texture = SDL_CreateTexture(img_scaler_->render,
            SDL_PIXELFORMAT_RGB24,
            SDL_TEXTUREACCESS_STREAMING,
            video_width_, video_height_);
        //img_scaler_->textureWidth = video_width_;
        //img_scaler_->textureHeight = video_height_;

        if (!img_scaler_->texture) {
            // 错误处理
            return -1;
        }
    }

    img_scaler_->rect.x = 0;
    img_scaler_->rect.y = 0;
    img_scaler_->rect.w = win_width_;  // 使用窗口宽度
    img_scaler_->rect.h = win_height_; // 使用窗口高度

    int ret = sws_scale(img_scaler_->sws_ctx, vp->frame->data, vp->frame->linesize,
        0, video_height_, sp->data, sp->linesize);

    if (ret <= 0) {
        // 错误处理
        return -1;
    }

    SDL_UpdateTexture(img_scaler_->texture, NULL, sp->data[0], sp->linesize[0]);
    SDL_RenderClear(img_scaler_->render);
    SDL_RenderCopy(img_scaler_->render, img_scaler_->texture, NULL, &img_scaler_->rect);
    SDL_RenderPresent(img_scaler_->render);

    return 0;
}
#endif // OPENGLRENDER


void DisplayWind::destory()
{
#ifdef OPENGLRENDER
#elif
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
#endif // OPENGLRENDER

}

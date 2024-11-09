#ifndef FF_DATA_H
#define FF_DATA_H

#include <iostream>
#include <list>
#include <qwindowdefs.h>


extern "C"
{
#include <libavutil/avassert.h>
#include <libavutil/channel_layout.h>
#include <libavutil/opt.h>
#include <libavutil/mathematics.h>
#include <libavutil/timestamp.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#include <libavutil/time.h>
#include <libavutil/imgutils.h>
#include <SDL.h>
}

#define PKTS_MAX_SIZE       25//包队列最大限制
#define PICTURE_QUEUE_SIZE  3
#define SAMPLE_QUEUE_SIZE   9
#define FRAME_QUEUE_SIZE    9

#define REFRESN_RATE 0.01  //视频刷新线程频率

//typedef struct FFPlayerCtx
//{
//	AVFormatContext* ic = nullptr;
//	AVCodecContext* audioCtx = nullptr;
//	AVCodecContext* videoCtx = nullptr;
//
//	PacketQueue* video_queue = nullptr;
//	PacketQueue* audio_queue = nullptr;
//
//}FFPlayerCtx;

/*   PacketQueue   */
typedef struct PacketQueue
{
	std::list<AVPacket> pkts;
	volatile int abort_request;
	int duration;
	int size ;
	int nb_packets;
	SDL_cond* cond= nullptr;
	SDL_cond* not_full = nullptr;
	SDL_mutex* mutex= nullptr;
}PacketQueue;

//启用包队列
void packet_queue_start(PacketQueue* queue,AVPacket* flush_pkt);
//插入包
void packet_queue_put_pkt(PacketQueue* q,AVPacket* pkt);
//包队列初始化
int packet_queue_init(PacketQueue* q);
//包队列退出
void packet_queue_abort(PacketQueue* q);
//包队列清空并释放资源
void packet_queue_flush(PacketQueue* q);
//获取包 0为空 1为成功
int packet_queue_get_pkt(PacketQueue*q,AVPacket* pkt);
//销毁包队列
void packet_queue_destroy(PacketQueue *q);
//唤醒阻塞的包队列
void packet_queue_signal(PacketQueue* q);
typedef struct Clock
{
	double pts;           // 时钟基础

	double pts_drift;     // 当前pts与当前系统时钟的差值。

	double last_updated;  // 最后一次更新的系统时钟
}Clock;

//获取时钟,实际上是最后一帧的pts加上从处理最后一帧到现在的时间
double get_clock(Clock* c);
//设置时钟（实际）
void set_clock_at(Clock* c,double pts,double time);
//设置时钟（使用的）
void set_clock(Clock* c, double pts);
//初始化时钟
void init_clock(Clock* c);

enum {
	AV_SYNC_AUDIO_MASTER,  // 以音频为基准
	AV_SYNC_VIDEO_MASTER,  // 以音频为基准
};

typedef struct Frame
{
	AVFrame* frame;
	int serial; //判断是否是同一序列段
	double pts;
	double duration;
	//int64_t pos;
	int32_t width;
	int32_t height;
	int format;
	//AVRational sar;
}Frame;

typedef struct FrameQueue
{
	Frame queue[FRAME_QUEUE_SIZE];
	int rindex = 0;
	int windex = 0;
	int size;
	int max_size;
	SDL_mutex* mutex = nullptr;
	SDL_cond* cond = nullptr;
	PacketQueue* pktq;
}FrameQueue;

typedef struct AudioParams
{
	int  freq;						 // 采样率
	int  channels;					 // 通道数
	AVChannelLayout channels_layout; // 通道布局
	enum AVSampleFormat fmt;		 // 音频采样格式，比如AV_SAMPLE_FMT_S16表示有符号16bit深度,交错排列模式
	int  frame_size;				 // 一个采样单元占用的字节数(比如2通道时，则左右通道各采样一次合成一个采样单元)
	int  bytes_per_sec;				 // 一秒时间的字节数，比如采样率48Khz,2channels,16bit,则一秒48000*2*16/8=192000 
}AudioParams;

//帧队列初始化
int frame_queue_init(FrameQueue* q, PacketQueue* pktq_);
//帧队列销毁
void frame_queue_destroy(FrameQueue*q);
//写入一帧
Frame* frame_queue_peek_writable(FrameQueue* q);
//读取一帧
Frame* frame_queue_peek_readable(FrameQueue* q);
//索引增加
void frame_queue_push(FrameQueue* q);
//读取下一帧 
void frame_queue_next(FrameQueue* q);
//写入一帧视频帧到视频帧队列中
//int put_picture(FrameQueue*q,AVFrame * src_frame,double pts,double duration);
//释放掉此帧
void frame_queue_unref_item(Frame* frame);
//查看帧队列是否有帧
int frame_queue_nb_remaining(FrameQueue* q);
//唤醒阻塞的队列
void frame_queue_signal(FrameQueue* q);
//清空帧队列 并释放资源
void frame_queue_flush(FrameQueue* q);
//时钟相关

//刷新页面
typedef struct ImageScaler
{
	SwsContext* sws_ctx = nullptr;

	SDL_Texture* texture = nullptr;
	SDL_Renderer* render = nullptr;
	SDL_Window* window = nullptr;
	SDL_Rect rect;

	AVCodecContext* codecCtx = nullptr;
	AVPixelFormat srcFMT;
	AVPixelFormat dstFMT;
}ImageScaler;



#endif // !FF_DATA_H
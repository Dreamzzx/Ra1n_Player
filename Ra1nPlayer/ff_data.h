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

#define PKTS_MAX_SIZE       25//�������������
#define PICTURE_QUEUE_SIZE  3
#define SAMPLE_QUEUE_SIZE   9
#define FRAME_QUEUE_SIZE    9

#define REFRESN_RATE 0.01  //��Ƶˢ���߳�Ƶ��

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

//���ð�����
void packet_queue_start(PacketQueue* queue,AVPacket* flush_pkt);
//�����
void packet_queue_put_pkt(PacketQueue* q,AVPacket* pkt);
//�����г�ʼ��
int packet_queue_init(PacketQueue* q);
//�������˳�
void packet_queue_abort(PacketQueue* q);
//��������ղ��ͷ���Դ
void packet_queue_flush(PacketQueue* q);
//��ȡ�� 0Ϊ�� 1Ϊ�ɹ�
int packet_queue_get_pkt(PacketQueue*q,AVPacket* pkt);
//���ٰ�����
void packet_queue_destroy(PacketQueue *q);
//���������İ�����
void packet_queue_signal(PacketQueue* q);
typedef struct Clock
{
	double pts;           // ʱ�ӻ���

	double pts_drift;     // ��ǰpts�뵱ǰϵͳʱ�ӵĲ�ֵ��

	double last_updated;  // ���һ�θ��µ�ϵͳʱ��
}Clock;

//��ȡʱ��,ʵ���������һ֡��pts���ϴӴ������һ֡�����ڵ�ʱ��
double get_clock(Clock* c);
//����ʱ�ӣ�ʵ�ʣ�
void set_clock_at(Clock* c,double pts,double time);
//����ʱ�ӣ�ʹ�õģ�
void set_clock(Clock* c, double pts);
//��ʼ��ʱ��
void init_clock(Clock* c);

enum {
	AV_SYNC_AUDIO_MASTER,  // ����ƵΪ��׼
	AV_SYNC_VIDEO_MASTER,  // ����ƵΪ��׼
};

typedef struct Frame
{
	AVFrame* frame;
	int serial; //�ж��Ƿ���ͬһ���ж�
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
	int  freq;						 // ������
	int  channels;					 // ͨ����
	AVChannelLayout channels_layout; // ͨ������
	enum AVSampleFormat fmt;		 // ��Ƶ������ʽ������AV_SAMPLE_FMT_S16��ʾ�з���16bit���,��������ģʽ
	int  frame_size;				 // һ��������Ԫռ�õ��ֽ���(����2ͨ��ʱ��������ͨ��������һ�κϳ�һ��������Ԫ)
	int  bytes_per_sec;				 // һ��ʱ����ֽ��������������48Khz,2channels,16bit,��һ��48000*2*16/8=192000 
}AudioParams;

//֡���г�ʼ��
int frame_queue_init(FrameQueue* q, PacketQueue* pktq_);
//֡��������
void frame_queue_destroy(FrameQueue*q);
//д��һ֡
Frame* frame_queue_peek_writable(FrameQueue* q);
//��ȡһ֡
Frame* frame_queue_peek_readable(FrameQueue* q);
//��������
void frame_queue_push(FrameQueue* q);
//��ȡ��һ֡ 
void frame_queue_next(FrameQueue* q);
//д��һ֡��Ƶ֡����Ƶ֡������
//int put_picture(FrameQueue*q,AVFrame * src_frame,double pts,double duration);
//�ͷŵ���֡
void frame_queue_unref_item(Frame* frame);
//�鿴֡�����Ƿ���֡
int frame_queue_nb_remaining(FrameQueue* q);
//���������Ķ���
void frame_queue_signal(FrameQueue* q);
//���֡���� ���ͷ���Դ
void frame_queue_flush(FrameQueue* q);
//ʱ�����

//ˢ��ҳ��
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
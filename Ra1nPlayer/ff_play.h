#ifndef FFPLAY_H
#define FFPLAY_H

#include <thread>
#include <functional>
#include "ffmsg_queue.h"
#include "ff_data.h"
#include <libswscale/swscale.h>
//音频CB
typedef void(*audio_cb)(void* opaque, Uint8* stream, int len);

class Decoder
{
public:
	Decoder();
	~Decoder();

	//解码器初始化
	void decoder_init(AVCodecContext* avctx,FrameQueue *fq);
	
	//创建和启动解码线程
	int decoder_start(enum AVMediaType codec_type, const char* thread_name, void* arg);
	
	//解码器退出
	void decoder_abort();

	//解码器销毁
	void decoder_destory();

	//audio解码线程
	int audio_thread(void* arg);
	//video解码线程
	int video_thread(void*arg);

	int decoder_decode_frame(AVFrame* frame);

	//将解码后的视频帧放入帧队列
	int put_picture(FrameQueue* q, AVFrame* frame,double pts);
	//将解码后的音频帧放入帧队列
	int put_sample(FrameQueue* q, AVFrame* frame);

	AVCodecContext* getCtx()
	{
		return avctx_;
	}

	void setSerial(int serial) {
		serial_ = serial;
	}

private:
	AVPacket pkt_;
	FrameQueue* frame_queue_;  // 帧队列 
	AVCodecContext* avctx_;    // 解码器上下文 
	int pkt_serial_;		   // 包序列
	int finished_;			   // =0,解码器处于工作状态 !=0 解码器处于空闲状态
	std::thread *decoder_thread_ = nullptr; 
	int serial_ = 0;		   //序列号判断是否属于同一段落帧（用于seek）
};

class FFPlayer
{
public:
	FFPlayer();
	~FFPlayer();
	int ffp_create();
	void ffp_destroy();
	int ffp_prepare_async_l(char* filename);

	MessageQueue msg_queue_;

	AVStream* get_video_st()
	{
		return this->video_st;
	}

	void SetVideoRefreshCallBack(std::function<int (const Frame*)> callback);
	void SetCtrlCallBack(std::function<void(int, double)> callback);
	
	int stream_open(const char * file_name);
	void stream_close();
	int stream_component_open(int stream_index);
	void stream_component_close(int stream_index);

	inline void set_volume(int volume) { this->volume = volume; }
	////设置重采样后的音频帧音量大小
	//void set_volume(Frame *src_f,int channels, int nb_samples,float volume);
private:
	//读取线程
	int read_thread();
	// 打开音频输出
	int audio_open(AVChannelLayout wanted_channels_layout,
					int wanted_nb_channels,int wanted_sample_rate,
					struct AudioParams* audio_hw_params);


	//关闭音频输出设备
	void audio_close();
	//视频输出
	void video_display();
	int video_refresh_thread();
	void video_refresh(double* remaining_time);

public:
	char* input_filename_ = nullptr;
	int abort_request = 0;

	//主要上下文
	AVFormatContext* ic_ = nullptr;
	//音频解码器
	Decoder auddec;
	//视频解码器
	Decoder viddec;
	//音频包队列
	PacketQueue audio_queue_;
	//视频包队列
	PacketQueue video_queue_;
	//音频帧队列
	FrameQueue sample_queue_;
	//视频帧队列
	FrameQueue picture_queue_;

	int paused;// 0播放 1暂停
	bool volum_muted = false; // false没有静音 true静音
	bool seek_req = false; //查看是否有seek操作
	int serial = 0;

	int audio_stream_;
	int video_stream_;
	AVStream* audio_st;
	AVStream* video_st;

	std::function<void(int,double)> ctrl_callback_;
	
	int av_sync_type = AV_SYNC_AUDIO_MASTER; // 音频同步类型
	Clock audclk; //音频时钟
	Clock vidclk; //音频时钟
	int64_t duration = 0;
	double position = 0.0; //当前进度条播放时间

	double audio_clock = 0;//当前音频帧的pts + 当前帧的Duration

	//音频输出相关
	struct AudioParams audio_src;//音频frame参数
	struct AudioParams audio_tgt;//SDL支持的音频参数
	struct SwrContext* swr_ctx = nullptr;
	int audio_hw_buf_size = 0;   //SDL音频缓冲区大小 
	audio_cb audio_callback;     
	
	uint8_t* audio_buf = nullptr;  //指向SDL需要采样的数据
	uint8_t* audio_buf1 = nullptr; //指向重采样后的数据
	unsigned int audio_buf_size = 0;  //待播放的一帧音频数据
	unsigned int audio_buf1_size = 0; //申请到音频缓冲区的实际大小
	int audio_buf_index = 0; // 更新拷贝位置

	//float volumeFactor = 1.0;//音量因子
	int volume = 50;

	float volume_speed = 1.5;
	//视频输出相关
	std::function<int(const Frame*)> video_refresh_callback_ = nullptr;
	AVCodecContext* vidctx = nullptr;
	//read_thread 
	std::thread* read_thread_ = nullptr;
	std::thread* video_refresh_thread_ = nullptr;
};

inline static void ffp_notify_msg1(FFPlayer *ffp,int what)
{
	msg_queue_put_simple3(&ffp->msg_queue_,what,0,0);
}

inline static void ffp_notify_msg2(FFPlayer* ffp, int what,int arg1)
{
	msg_queue_put_simple3(&ffp->msg_queue_, what, arg1, 0);
}

inline static void ffp_notify_msg3(FFPlayer* ffp, int what,int arg1,int arg2)
{
	msg_queue_put_simple3(&ffp->msg_queue_, what, arg1, arg2);
}

inline static void ffp_notify_msg4(FFPlayer* ffp, int what, int arg1, int arg2,void *obj,int obj_len)
{
	msg_queue_put_simple4(&ffp->msg_queue_, what, arg1, arg2,obj,obj_len);
}

#endif // !FFPLAY_H
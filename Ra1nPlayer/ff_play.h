#ifndef FFPLAY_H
#define FFPLAY_H

#include <thread>
#include <functional>
#include "ffmsg_queue.h"
#include "ff_data.h"
#include <libswscale/swscale.h>
//��ƵCB
typedef void(*audio_cb)(void* opaque, Uint8* stream, int len);

class Decoder
{
public:
	Decoder();
	~Decoder();

	//��������ʼ��
	void decoder_init(AVCodecContext* avctx,FrameQueue *fq);
	
	//���������������߳�
	int decoder_start(enum AVMediaType codec_type, const char* thread_name, void* arg);
	
	//�������˳�
	void decoder_abort();

	//����������
	void decoder_destory();

	//audio�����߳�
	int audio_thread(void* arg);
	//video�����߳�
	int video_thread(void*arg);

	int decoder_decode_frame(AVFrame* frame);

	//����������Ƶ֡����֡����
	int put_picture(FrameQueue* q, AVFrame* frame,double pts);
	//����������Ƶ֡����֡����
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
	FrameQueue* frame_queue_;  // ֡���� 
	AVCodecContext* avctx_;    // ������������ 
	int pkt_serial_;		   // ������
	int finished_;			   // =0,���������ڹ���״̬ !=0 ���������ڿ���״̬
	std::thread *decoder_thread_ = nullptr; 
	int serial_ = 0;		   //���к��ж��Ƿ�����ͬһ����֡������seek��
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
	////�����ز��������Ƶ֡������С
	//void set_volume(Frame *src_f,int channels, int nb_samples,float volume);
private:
	//��ȡ�߳�
	int read_thread();
	// ����Ƶ���
	int audio_open(AVChannelLayout wanted_channels_layout,
					int wanted_nb_channels,int wanted_sample_rate,
					struct AudioParams* audio_hw_params);


	//�ر���Ƶ����豸
	void audio_close();
	//��Ƶ���
	void video_display();
	int video_refresh_thread();
	void video_refresh(double* remaining_time);

public:
	char* input_filename_ = nullptr;
	int abort_request = 0;

	//��Ҫ������
	AVFormatContext* ic_ = nullptr;
	//��Ƶ������
	Decoder auddec;
	//��Ƶ������
	Decoder viddec;
	//��Ƶ������
	PacketQueue audio_queue_;
	//��Ƶ������
	PacketQueue video_queue_;
	//��Ƶ֡����
	FrameQueue sample_queue_;
	//��Ƶ֡����
	FrameQueue picture_queue_;

	int paused;// 0���� 1��ͣ
	bool volum_muted = false; // falseû�о��� true����
	bool seek_req = false; //�鿴�Ƿ���seek����
	int serial = 0;

	int audio_stream_;
	int video_stream_;
	AVStream* audio_st;
	AVStream* video_st;

	std::function<void(int,double)> ctrl_callback_;
	
	int av_sync_type = AV_SYNC_AUDIO_MASTER; // ��Ƶͬ������
	Clock audclk; //��Ƶʱ��
	Clock vidclk; //��Ƶʱ��
	int64_t duration = 0;
	double position = 0.0; //��ǰ����������ʱ��

	double audio_clock = 0;//��ǰ��Ƶ֡��pts + ��ǰ֡��Duration

	//��Ƶ������
	struct AudioParams audio_src;//��Ƶframe����
	struct AudioParams audio_tgt;//SDL֧�ֵ���Ƶ����
	struct SwrContext* swr_ctx = nullptr;
	int audio_hw_buf_size = 0;   //SDL��Ƶ��������С 
	audio_cb audio_callback;     
	
	uint8_t* audio_buf = nullptr;  //ָ��SDL��Ҫ����������
	uint8_t* audio_buf1 = nullptr; //ָ���ز����������
	unsigned int audio_buf_size = 0;  //�����ŵ�һ֡��Ƶ����
	unsigned int audio_buf1_size = 0; //���뵽��Ƶ��������ʵ�ʴ�С
	int audio_buf_index = 0; // ���¿���λ��

	//float volumeFactor = 1.0;//��������
	int volume = 50;

	float volume_speed = 1.5;
	//��Ƶ������
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
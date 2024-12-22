#include <iostream>
#include "ff_play.h"
#include "ffmsg.h"

#define SDL_MIX_MAXVOLUME 128

static AVPacket flush_pkt;

static int audio_decode_frame(FFPlayer* is)
{
	int data_size, resample_data_size = 0;
	AVChannelLayout dec_channel_layout;
	int wanted_nb_samples;
	Frame* af;
	int ret = 0;

	if (is->audio_queue_.abort_request)
		return -1;

	if (is->paused)
		return -1;

	//读一帧数据
	af = frame_queue_peek_readable(&is->sample_queue_);

	////调整音频帧音量
	//is->set_volume(af,af->frame->channels,af->frame->nb_samples,1.5);

	//倍速设置
	af->frame->sample_rate = af->frame->sample_rate * is->volume_speed;

	if (!af)
		return -1;
	//得到一帧音频数据的大小
	data_size = av_samples_get_buffer_size(NULL, af->frame->ch_layout.nb_channels,
											af->frame->nb_samples,
											(AVSampleFormat)af->frame->format,1);
	//获取声道布局
	dec_channel_layout = af->frame->ch_layout;

	wanted_nb_samples = af->frame->nb_samples;

	//is->audio_tgt是SDL可接收的音频帧数
	if(af->frame->format  != is->audio_src.fmt || // 采样格式
		af->frame->sample_rate  != is->audio_src.freq //采样率
		)
	{
		swr_free(&is->swr_ctx);
		swr_alloc_set_opts2(&is->swr_ctx,
						&is->audio_tgt.channels_layout,
						is->audio_tgt.fmt,
						is->audio_tgt.freq,
						(const AVChannelLayout*)&dec_channel_layout,
						(enum AVSampleFormat)af->frame->format,
						af->frame->sample_rate,
						0,NULL);
		if (!is->swr_ctx || swr_init(is->swr_ctx) < 0)
		{
			swr_free(&is->swr_ctx);
		}
	}
	


	//重采样
	if (is->swr_ctx)
	{
			const uint8_t** in = (const uint8_t**)af->frame->extended_data; //data[0],data[1]
			uint8_t** out = &is->audio_buf1;
			int out_count = (int64_t)wanted_nb_samples * is->audio_tgt.freq / af->frame->sample_rate +256; // +256 的目的是重采样内部有一定的缓存
			
			//计算对应的样本数
			int out_size = av_samples_get_buffer_size(NULL, is->audio_tgt.channels, out_count, is->audio_tgt.fmt, 0);
			if (out_size < 0)
			{
				goto fail;
			}

			//快速分配空间 将数据分配给audio_buf1 将数据大小分配给audio_buf1_size
			av_fast_malloc(&is->audio_buf1,&is->audio_buf1_size, out_size);
			if (!is->audio_buf1)
			{
				goto fail;
			}
			int len2;
			//音频重采样 len2返回值是重采样后得到的音频数据中单个声道的样本数
			len2 = swr_convert(is->swr_ctx, out, out_count, in, af->frame->nb_samples);

			if (len2 < 0)
			{
				goto fail;
			}
			if (len2 == out_count)
			{
				//
			}
			is->audio_buf = is->audio_buf1;
			resample_data_size = len2 * is->audio_tgt.channels * av_get_bytes_per_sample(is->audio_tgt.fmt);
	}


	if (!isnan(af->pts))
	{
		is->audio_clock = af->pts* av_q2d(is->audio_st->time_base);
	}
	else
		is->audio_clock = NAN;

	frame_queue_next(&is->sample_queue_);
	return resample_data_size;
fail:
	return -1;
}


static void sdl_audio_callback(void* opaque, Uint8* stream, int len)
{
	FFPlayer* is = (FFPlayer *)opaque;
	int audio_size, len1;
	while (len > 0)
	{

		if (is->audio_buf_index >= is->audio_buf_size)
		{
			audio_size = audio_decode_frame(is);
			if (audio_size < 0)
			{
				is->audio_buf = nullptr;
			}
			else
			{
				is->audio_buf_size = audio_size;
			}
				is->audio_buf_index = 0;
		}

			len1 = is->audio_buf_size - is->audio_buf_index;
			if (len1 > len)
			{
				len1 = len;
			}
			if (is->audio_buf && !is->volum_muted)
			{
				memset(stream, 0, len1);
				//memcpy(stream,is->audio_buf+is->audio_buf_index, len1);
				SDL_MixAudio(stream, (uint8_t*)is->audio_buf + is->audio_buf_index, len1, is->volume);
			}
			else
			{
				//静音
				memset(stream,0,len1);
			}

			len -= len1;
			stream += len1;
			is->audio_buf_index += len1;
	}
	
	//更新时钟
	set_clock(&is->audclk, is->audio_clock);
	is->ctrl_callback_(MP_SET_PLAY_TIME, get_clock(&is->audclk));
}


/*    播放器内核部分    */
FFPlayer::FFPlayer()
{
	//初始化 MessageQueue 
	msg_queue_ = MessageQueue();
	msg_queue_init(&msg_queue_);
}

FFPlayer::~FFPlayer()
{
}

int FFPlayer::ffp_create()
{
	
	return 0;
}

void FFPlayer::ffp_destroy()
{
}

int FFPlayer::ffp_prepare_async_l(char* filename)
{
	printf("打开文件\n");
	input_filename_ = strdup(filename);//保存文件名字
	int ret = stream_open(input_filename_);
	paused = 0;
	return ret;
}

int FFPlayer::stream_open(const char* file_name)
{
	//初始化Frame帧队列
	if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_TIMER))
	{
		return -1;
	}
	//初始化Packet包队列
	if (packet_queue_init(&audio_queue_) < 0 || packet_queue_init(&video_queue_))
	{
		goto fail;
	}
	//初始化Frame帧队列 并绑定包队列
	if (frame_queue_init(&sample_queue_,&audio_queue_) < 0 || frame_queue_init(&picture_queue_,&video_queue_) < 0)
	{
		goto fail;
	}
	//初始化时钟
	init_clock(&audclk);
	init_clock(&vidclk);
	//读取线程  read_thread 
	read_thread_ = new std::thread(&FFPlayer::read_thread, this);
	//创建视频刷新线程
	video_refresh_thread_ = new std::thread(&FFPlayer::video_refresh_thread, this); 
	return 1;
fail:
	stream_close();
	return 0;
}

void FFPlayer::stream_close()
{
	abort_request = 1;

	packet_queue_abort(&audio_queue_);
	packet_queue_abort(&video_queue_);

	if (read_thread_ && read_thread_->joinable())
	{
		read_thread_->join();
		read_thread_ = nullptr;
		std::cout << "read_thread end" << std::endl;
	}
	if (video_refresh_thread_ && video_refresh_thread_->joinable())
	{
		video_refresh_thread_->join();
		video_refresh_thread_ = nullptr;
		std::cout << "video_refresh_thread_ end" << std::endl;
	}


	/* 关闭每一个流 */
	if (video_stream_ >= 0)
	{
		stream_component_close(video_stream_);
		std::cout << "video_thread end" << std::endl;
	}
	if (audio_stream_ >= 0)
	{
		stream_component_close(audio_stream_);
		std::cout << "audio_thread end" << std::endl;
	}
	// 释放packet队列
	packet_queue_destroy(&video_queue_);
	packet_queue_destroy(&audio_queue_);
	// 释放frame队列_destroy(&sample_queue_);
	frame_queue_destroy(&picture_queue_);
	frame_queue_destroy(&sample_queue_);

	if (input_filename_)
	{
		free(input_filename_);
		input_filename_ = nullptr;
	}
}

int FFPlayer::stream_component_open(int stream_index)
{
	AVCodecContext* avctx;
	const AVCodec* codec;
	int sample_rate;
	int nb_channels;
	AVChannelLayout channel_layout;
	int ret = 0;

	//判断stream_index是否合法
	if (stream_index < 0 || stream_index >= ic_->nb_streams)
		return -1;
	/* 为解码器分配一个上下文 */
	avctx = avcodec_alloc_context3(NULL);
	if (!avctx)
		return AVERROR(ENOMEM);

	/* 将码流中的编解码信息拷贝到新分配的编解码器上下文中 */
	ret = avcodec_parameters_to_context(avctx, ic_->streams[stream_index]->codecpar);
	if (ret < 0) {
		goto fail;
	}
	// 设置codec_id查找解码器
	codec = avcodec_find_decoder(avctx->codec_id);
	if (!codec) {
		printf("No decoder could be found for codec.\n");
		ret = AVERROR(EINVAL);
		goto fail;
	}
	//打开解码器
	if ((ret = avcodec_open2(avctx, codec, NULL)) != 0) {
		goto fail;
	}
	switch (avctx->codec_type)
	{
	case AVMEDIA_TYPE_AUDIO:
		//从avctx中获取参数
		sample_rate = avctx->sample_rate;// 采样率
		nb_channels = avctx->ch_layout.nb_channels;//通道数
		channel_layout = avctx->ch_layout;// 通道布局
		
		/*  准备音频输出  */
		if ((ret = audio_open(channel_layout, nb_channels, sample_rate, &audio_tgt))< 0)
			goto fail;
		audio_hw_buf_size = ret;
		audio_src = audio_tgt;
		audio_buf_size = 0;
		audio_buf_index = 0;

		audio_stream_ = stream_index;
		audio_st = ic_->streams[stream_index];// 获取aduio的stream指针
		
		//将deocder和帧队列、上下文绑定在一块
		auddec.decoder_init(avctx,&this->sample_queue_);
		//开启解码线程
		auddec.decoder_start(avctx->codec_type, "audio_thread", this);

		std::cout << audio_st->codecpar->codec_id << std::endl;

		//允许音频输出
		SDL_PauseAudio(0);
		break;
	case AVMEDIA_TYPE_VIDEO:

		video_stream_ = stream_index;
		video_st = ic_->streams[stream_index];// 获取video的stream指针
		
		vidctx = avctx;

		viddec.decoder_init(avctx,&this->picture_queue_);
		viddec.decoder_start(avctx->codec_type, "video_thread", this);
		break;
	}
	return ret;
fail:
	stream_component_close(stream_index);
	return ret;
}

void FFPlayer::stream_component_close(int stream_index)
{
	AVCodecParameters* codecpar;
	
	codecpar = ic_->streams[stream_index]->codecpar;
	if (stream_index < 0 || stream_index >= ic_->nb_streams)
		return ;

	switch(stream_index)
	{
	case AVMEDIA_TYPE_AUDIO:
		//请求中止解码器线程
		auddec.decoder_abort();
		//关闭音频设备
		audio_close();
		//销毁解码器
		auddec.decoder_destory();
		//释放重采样器
		if (swr_ctx)
		{
		swr_free(&swr_ctx);
		swr_ctx = nullptr;
		}
		if(audio_buf1)
		av_freep(&audio_buf1);

		audio_buf1_size = 0;
		audio_buf = nullptr;
		break;
		case AVMEDIA_TYPE_VIDEO:
			//请求中止解码器线程
			viddec.decoder_abort();
			viddec.decoder_destory();
			break;
	}

	switch (codecpar->codec_type)
	{
	case AVMEDIA_TYPE_VIDEO:
		video_stream_ = -1;
		video_st = nullptr;
		break;
	case AVMEDIA_TYPE_AUDIO:
		audio_stream_ = -1;
		audio_st = nullptr;
		break;
	}

	if(codecpar)
	av_freep(&codecpar);

	return;
}

int FFPlayer::read_thread()
{
	int err , ret;
	int st_index[AVMEDIA_TYPE_NB];//AVMEDIA_TYPE_VIDEO AVMEDIA_TYPE_AUDIO
	AVPacket pkt1;
	AVPacket* pkt = &pkt1;
	int serial = 0;

	memset(st_index, -1, sizeof(st_index));
	video_stream_ = -1;
	audio_stream_ = -1;

	//1.创建上下文
	ic_ = avformat_alloc_context();
	if (!ic_)
	{
		av_log(NULL, AV_LOG_FATAL, "Could not allocate ic_.\n");
		ret = AVERROR(ENOMEM);
		goto fail;
	}

	//2. 打开文件，主要是查看文件的协议类型
	err = avformat_open_input(&ic_, input_filename_, NULL, NULL);
	if (err < 0)
	{
		printf("avformat_open_input is failed.\n ");
		ret = -1;
		goto fail;
	}

	ffp_notify_msg1(this, RA1NP_MSG_OPEN_INPUT);
	std::cout << "read_thread FFP_MSG_OPEN_INPUT" << std::endl;

	/*3.探测媒体的类型，可得到当前文件的封装格式，音频编码等信息。
		本质上是做了decode_packet获取信息的工作*/
	err = avformat_find_stream_info(ic_, NULL);
	if (err < 0)
	{
		printf("avformat_find_stream_info is failed.\n ");
		ret = -1;
		goto fail;
	}

	//获取视频总时长
	duration = ic_->duration /AV_TIME_BASE;
	ctrl_callback_(MP_SET_DURATION, duration);

	ffp_notify_msg1(this, RA1NP_MSG_FIND_STREAM_INFO);
	std::cout << "read_thread FFP_MSG_FIND_STREAM_INFO" << std::endl;
	//4.利用av_find_best_stream查找流
	st_index[AVMEDIA_TYPE_VIDEO] =
			av_find_best_stream(ic_, AVMEDIA_TYPE_VIDEO,
								st_index[AVMEDIA_TYPE_VIDEO], 
								-1, NULL, 0);
	
	st_index[AVMEDIA_TYPE_AUDIO] =
		av_find_best_stream(ic_, AVMEDIA_TYPE_AUDIO,
							st_index[AVMEDIA_TYPE_AUDIO],
							st_index[AVMEDIA_TYPE_VIDEO],
							NULL, 0);

	//5.打开流的解码器，并创建相应的解码线程
	if (st_index[AVMEDIA_TYPE_VIDEO] >= 0) {
		stream_component_open( st_index[AVMEDIA_TYPE_VIDEO] );
	}
	if (st_index[AVMEDIA_TYPE_AUDIO] >= 0) {
		stream_component_open( st_index[AVMEDIA_TYPE_AUDIO] );
	}
	ffp_notify_msg1(this, RA1NP_MSG_COMPOMENT_OPEN);
	std::cout << "read_thread FFP_MSG_COMPOMENT_OPEN" << std::endl;

	if (video_stream_ < 0 && audio_stream_ < 0) {
		printf("open file is failed.\n ");
		ret = -1;
		goto fail;
	}

	//6.循环读取包数据，并将包放入相应的队列中
	for (;;)
	{
		if (abort_request)
		{
			break;
		}

		//seek操作
		if (seek_req)
		{
			int64_t pts = (position / duration) * ic_->duration;
			ret = avformat_seek_file(ic_, -1, INT64_MIN, pts, INT64_MAX, 0);
			if (ret >= 0)
			{
				//清空队列
				packet_queue_flush(&audio_queue_);
				packet_queue_put_pkt(&audio_queue_, &flush_pkt);
				packet_queue_flush(&video_queue_);
				packet_queue_put_pkt(&video_queue_, &flush_pkt);
				
			}
			audclk.pts = position;

			serial++;
			auddec.setSerial(serial);
			viddec.setSerial(serial);

			seek_req = false;
		}

		ret = av_read_frame(ic_,pkt);
		if (ret < 0)
		{
			printf("read_frame error or end of file\n");
			break;
		}
		if (pkt->stream_index == audio_stream_)
			packet_queue_put_pkt(&audio_queue_, pkt);
		else if(pkt->stream_index == video_stream_)
			packet_queue_put_pkt(&video_queue_,pkt);
		else
			av_packet_unref(pkt);
	}

	return ret;

fail:
	stream_close();
	return ret;
}

int FFPlayer::audio_open(AVChannelLayout wanted_channels_layout, int wanted_nb_channels, int wanted_sample_rate, AudioParams* audio_hw_params)
{
	SDL_AudioSpec wanted_spec;

	//绑定回调函数
	audio_callback = sdl_audio_callback;

	wanted_spec.freq = wanted_sample_rate;
	wanted_spec.channels = wanted_nb_channels;
	wanted_spec.format = AUDIO_S16SYS; //采样点格式
	wanted_spec.silence = 0;
	wanted_spec.samples = 2048;  // 多久会产生一次回调
	wanted_spec.callback = audio_callback;//回调函数
	wanted_spec.userdata = this;

	//打开音频设备
	if (SDL_OpenAudio(&wanted_spec, NULL) != 0)
	{
		printf("SDL Open Audio failed!\n");
		return -1;
	}

	//保存初始解码后的音频数据
	//wanted_spec是期望的参数，spec是实际的参数，wanted_spec和spec都是SDL里的参数
	//audio_hw_params保存的参数就是在做重采样后要转化的格式
	audio_hw_params->channels_layout = wanted_channels_layout;
	audio_hw_params->channels = wanted_nb_channels;
	audio_hw_params->freq = wanted_sample_rate;
	audio_hw_params->fmt = AV_SAMPLE_FMT_S16;
	//计算一个采样点所占用的字节数
	audio_hw_params->frame_size = av_samples_get_buffer_size(NULL, 
															audio_hw_params->channels, 
															1, 
															audio_hw_params->fmt,
															1);
	//1秒需要的字节数
	audio_hw_params->bytes_per_sec = av_samples_get_buffer_size(NULL,
																audio_hw_params->channels,
																audio_hw_params->freq,
																audio_hw_params->fmt,
																1);
	if (audio_hw_params->frame_size <= 0 || audio_hw_params->bytes_per_sec <= 0)
	{
		printf("frame_size <= 0 !\n");
		return -1;
	} 
	//SDL_OpenAudio 读进去后获得的size
	return wanted_spec.size;
}

//void FFPlayer::set_volume(Frame *src_f,int channels,int nb_samples,float volume)
//{
//	uint8_t** data = src_f->frame->extended_data;
//
//	//float* ret = static_cast<float>(data);
//	for (int i = 0; i < nb_samples; i++)
//	{
//		for (int j = 0; j < channels; j++)
//		{
//			// 获取当前样本值（根据样本格式进行转换）
//			int16_t* sample = (int16_t*)(data[j] + i * sizeof(int16_t));
//			// 调整音量
//			*sample = static_cast<int16_t>(*sample * 0.5);
//		}
//	}
//}

void FFPlayer::audio_close()
{
	SDL_CloseAudio();
}

void FFPlayer::SetVideoRefreshCallBack(std::function<int(const Frame*)> callback)
{
	video_refresh_callback_ = callback;
}

void FFPlayer::SetCtrlCallBack(std::function<void(int, double)> callback)
{
	ctrl_callback_ = callback;
}

void FFPlayer::video_display()
{

}

int FFPlayer::video_refresh_thread()
{
	double remaining_time = 0.0;
	while (!abort_request)
	{
		if (remaining_time > 0.0)
			av_usleep(remaining_time * 1000000.0);
		remaining_time = REFRESN_RATE;
		if(!this->paused)
		video_refresh(&remaining_time);
	}
	//结束后刷新一帧黑屏画面
	video_refresh_callback_(NULL);
	return 0;
}


void FFPlayer::video_refresh(double* remaining_time)
{
	Frame* vp = nullptr;
	//判断是否有视频画面
	if (this->video_st)
	{
		//没有帧直接返回
		if (frame_queue_nb_remaining(&picture_queue_) == 0)
			return ;

		vp = frame_queue_peek_readable(&picture_queue_);

		if (!vp)
			return;
		
		double aftime = get_clock(&audclk);
		double diff = vp->pts - aftime;

		//视频帧慢于音频帧直接丢弃掉，快于音频帧延迟一下
		if (diff > 0 && vp->serial != this->serial)
		{
			*remaining_time = FFMIN(*remaining_time, diff);
			return;  
		}
		if (video_refresh_callback_ && vp)
			video_refresh_callback_(vp);

		//刷新显示
		frame_queue_next(&this->picture_queue_);

	}

}

/*   解码器部分   */

Decoder::Decoder()
{
}

Decoder::~Decoder()
{
}

void Decoder::decoder_init(AVCodecContext* avctx,  FrameQueue* fq)
{
	avctx_ = avctx;
	frame_queue_ = fq;
}

int Decoder::decoder_start(enum AVMediaType codec_type, const char* thread_name, void* arg)
{
	//启用包队列
	packet_queue_start(frame_queue_->pktq,&flush_pkt);
	//创建线程
	if (codec_type == AVMEDIA_TYPE_AUDIO)
		decoder_thread_ = new std::thread(&Decoder::audio_thread,this,arg);
	else if (codec_type == AVMEDIA_TYPE_VIDEO)
		decoder_thread_ = new std::thread(&Decoder::video_thread,this,arg);
	else
		return -1;
	return 0;
}

void Decoder::decoder_abort()
{
	packet_queue_abort(frame_queue_->pktq);
	frame_queue_signal(frame_queue_); //唤醒阻塞的帧队列
	SDL_CondSignal(frame_queue_->cond);
	this->finished_ = 1;

	if (decoder_thread_ && decoder_thread_->joinable())
	{
		decoder_thread_->join();
		delete decoder_thread_;
		decoder_thread_ = nullptr;
	}
	packet_queue_flush(frame_queue_->pktq); //清空packet队列 ,并释放资源
}

void Decoder::decoder_destory()
{
	if (avctx_)
	{
	avcodec_free_context(&avctx_);
	avctx_ = nullptr;
	}
}

int Decoder::audio_thread(void *arg)
{
	FFPlayer* is = (FFPlayer*) arg;
	AVFrame* frame = av_frame_alloc();// 分配解码帧
	Frame* af;
	int got_frame = 0;// 是否读取到帧
	AVRational tb; // timebase
	int ret = 0;

	if (!frame)
		return AVERROR(ENOMEM);

	for(;;)
	{
		//中止
		if (frame_queue_->pktq->abort_request)
		{
			break;
		}
		//1.读取解码帧
		if ((got_frame = decoder_decode_frame(frame)) < 0)
		{
			printf("读取帧失败\n");
			goto the_end;
		}
		if (got_frame)
		{
			// 1设置sample_rate 为timebase
			tb ={ 1 , frame->sample_rate };
			//2.获取可写的Frame
			af = frame_queue_peek_writable(&is->sample_queue_);
			if (!af)
				break;

			af->pts = frame->pts;
			af->format = frame->format;
			af->duration = frame->duration;
			af->serial = this->serial_;

			//3.设置Frame并放入FrameQueue
			av_frame_move_ref(af->frame, frame);
			frame_queue_push(&is->sample_queue_);
		}
	}

	return 0;
the_end:
	return 0;
}

int Decoder::video_thread(void* arg)
{
	FFPlayer* is = (FFPlayer*)arg;
	AVFrame* frame = av_frame_alloc();
	double pts;
	double duration;
	int ret;
	AVRational tb = is->get_video_st()->time_base;
	AVRational frame_rate = av_guess_frame_rate(is->ic_,is->video_st,NULL);
	
	if (!frame)
		return AVERROR(ENOMEM);
	
	for (;;)
	{
		//中止线程
		if (frame_queue_->pktq->abort_request)
		{
			goto the_end;
		}

		ret = decoder_decode_frame(frame);
		if (ret < 0)
			goto the_end;
		if (!ret)
			continue;

		//计算帧持续时间并转换为pts值为秒
		duration = (frame_rate.num && frame_rate.den ? av_q2d({ frame_rate.num, frame_rate.den }) : 0);
		//根据timebase计算出pts值，单位为秒
		pts = (frame->pts == AV_NOPTS_VALUE)? NAN : frame->pts * av_q2d(tb);
		
		//将解码后的帧放入到队列中
		ret = put_picture(&is->picture_queue_,frame,pts);
		//索引增加
		frame_queue_push(&is->picture_queue_);
	}
	return 0;
the_end:
	return 0;
}

int Decoder::decoder_decode_frame(AVFrame* frame)
{
	int ret =0;
	Frame* f;
	for (;;)
	{
		if (frame_queue_->pktq->abort_request)
			return -1;
		AVPacket pkt;
		//1.尝试读一帧
		do 
		{
			switch (avctx_->codec_type)
			{
			case AVMEDIA_TYPE_AUDIO:

				ret = avcodec_receive_frame(avctx_, frame);
				if (ret >= 0)
				{
					AVRational tb = { 1,frame->sample_rate };
					if (frame->pts != AV_NOPTS_VALUE) {
						//转换成目标时间基准确保正确的时间顺序和同步
						frame->pts = av_rescale_q(frame->pts, avctx_->time_base, tb);
					}
				}
				else
				{
					char errStr[256] = { 0 };
					av_strerror(ret, errStr, sizeof(errStr));
				}
				break;
			case AVMEDIA_TYPE_VIDEO:
				ret = avcodec_receive_frame(avctx_, frame);

				if (ret >= 0)
				{
					//f = frame_queue_peek_writable(frame_queue_);
					/*if (!f)
						return -1;*/
				}
				else
				{
					char errStr[256] = { 0 };
					av_strerror(ret, errStr, sizeof(errStr));
				}
				break;
			}

			//1.2检查解码是否已经结束 返回0
			if (ret == AVERROR_EOF)
				return 0;

			//1.3正常解码返回1
			if (ret >= 0)
			{
				return 1;
			}

		} while ( ret >= 0 || ret == AVERROR_EOF);
		//2.阻塞式读取packet送给解码器
		if (packet_queue_get_pkt(frame_queue_->pktq, &pkt) < 0)
		{
			av_packet_unref(&pkt);
			return -1;
		}

		if (frame_queue_->pktq->abort_request)
		{
			//av_packet_unref(&pkt);
			break;
		}
		
		if(avcodec_send_packet(avctx_, &pkt) == AVERROR(EAGAIN))
		{
			av_packet_unref(&pkt);
			break;
		}
			av_packet_unref(&pkt);
	}
	return ret;
}

int Decoder::put_picture(FrameQueue* q, AVFrame* frame,double pts)
{
	Frame* vp = nullptr;

	//帧队满了需要阻塞等待使用
	SDL_LockMutex(q->mutex);
	if (q->size >= FRAME_QUEUE_SIZE)
	{
		SDL_CondWaitTimeout(q->cond, q->mutex, 500);
	}
	SDL_UnlockMutex(q->mutex);

	vp = frame_queue_peek_writable(q);

	if (vp != NULL)
	{
		vp->format = frame->format;
		vp->width = frame->width;
		vp->height = frame->height;
		vp->pts = pts;
		vp->duration = frame->duration;
		av_frame_move_ref(vp->frame, frame);
	}
	return 0;
}

int Decoder::put_sample(FrameQueue* q, AVFrame* frame)
{

	return 0;
}

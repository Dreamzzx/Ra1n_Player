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

	//��һ֡����
	af = frame_queue_peek_readable(&is->sample_queue_);

	////������Ƶ֡����
	//is->set_volume(af,af->frame->channels,af->frame->nb_samples,1.5);

	//��������
	af->frame->sample_rate = af->frame->sample_rate * is->volume_speed;

	if (!af)
		return -1;
	//�õ�һ֡��Ƶ���ݵĴ�С
	data_size = av_samples_get_buffer_size(NULL, af->frame->ch_layout.nb_channels,
											af->frame->nb_samples,
											(AVSampleFormat)af->frame->format,1);
	//��ȡ��������
	dec_channel_layout = af->frame->ch_layout;

	wanted_nb_samples = af->frame->nb_samples;

	//is->audio_tgt��SDL�ɽ��յ���Ƶ֡��
	if(af->frame->format  != is->audio_src.fmt || // ������ʽ
		af->frame->sample_rate  != is->audio_src.freq //������
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
	


	//�ز���
	if (is->swr_ctx)
	{
			const uint8_t** in = (const uint8_t**)af->frame->extended_data; //data[0],data[1]
			uint8_t** out = &is->audio_buf1;
			int out_count = (int64_t)wanted_nb_samples * is->audio_tgt.freq / af->frame->sample_rate +256; // +256 ��Ŀ�����ز����ڲ���һ���Ļ���
			
			//�����Ӧ��������
			int out_size = av_samples_get_buffer_size(NULL, is->audio_tgt.channels, out_count, is->audio_tgt.fmt, 0);
			if (out_size < 0)
			{
				goto fail;
			}

			//���ٷ���ռ� �����ݷ����audio_buf1 �����ݴ�С�����audio_buf1_size
			av_fast_malloc(&is->audio_buf1,&is->audio_buf1_size, out_size);
			if (!is->audio_buf1)
			{
				goto fail;
			}
			int len2;
			//��Ƶ�ز��� len2����ֵ���ز�����õ�����Ƶ�����е���������������
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
				//����
				memset(stream,0,len1);
			}

			len -= len1;
			stream += len1;
			is->audio_buf_index += len1;
	}
	
	//����ʱ��
	set_clock(&is->audclk, is->audio_clock);
	is->ctrl_callback_(MP_SET_PLAY_TIME, get_clock(&is->audclk));
}


/*    �������ں˲���    */
FFPlayer::FFPlayer()
{
	//��ʼ�� MessageQueue 
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
	printf("���ļ�\n");
	input_filename_ = strdup(filename);//�����ļ�����
	int ret = stream_open(input_filename_);
	paused = 0;
	return ret;
}

int FFPlayer::stream_open(const char* file_name)
{
	//��ʼ��Frame֡����
	if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_TIMER))
	{
		return -1;
	}
	//��ʼ��Packet������
	if (packet_queue_init(&audio_queue_) < 0 || packet_queue_init(&video_queue_))
	{
		goto fail;
	}
	//��ʼ��Frame֡���� ���󶨰�����
	if (frame_queue_init(&sample_queue_,&audio_queue_) < 0 || frame_queue_init(&picture_queue_,&video_queue_) < 0)
	{
		goto fail;
	}
	//��ʼ��ʱ��
	init_clock(&audclk);
	init_clock(&vidclk);
	//��ȡ�߳�  read_thread 
	read_thread_ = new std::thread(&FFPlayer::read_thread, this);
	//������Ƶˢ���߳�
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


	/* �ر�ÿһ���� */
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
	// �ͷ�packet����
	packet_queue_destroy(&video_queue_);
	packet_queue_destroy(&audio_queue_);
	// �ͷ�frame����_destroy(&sample_queue_);
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

	//�ж�stream_index�Ƿ�Ϸ�
	if (stream_index < 0 || stream_index >= ic_->nb_streams)
		return -1;
	/* Ϊ����������һ�������� */
	avctx = avcodec_alloc_context3(NULL);
	if (!avctx)
		return AVERROR(ENOMEM);

	/* �������еı������Ϣ�������·���ı�������������� */
	ret = avcodec_parameters_to_context(avctx, ic_->streams[stream_index]->codecpar);
	if (ret < 0) {
		goto fail;
	}
	// ����codec_id���ҽ�����
	codec = avcodec_find_decoder(avctx->codec_id);
	if (!codec) {
		printf("No decoder could be found for codec.\n");
		ret = AVERROR(EINVAL);
		goto fail;
	}
	//�򿪽�����
	if ((ret = avcodec_open2(avctx, codec, NULL)) != 0) {
		goto fail;
	}
	switch (avctx->codec_type)
	{
	case AVMEDIA_TYPE_AUDIO:
		//��avctx�л�ȡ����
		sample_rate = avctx->sample_rate;// ������
		nb_channels = avctx->ch_layout.nb_channels;//ͨ����
		channel_layout = avctx->ch_layout;// ͨ������
		
		/*  ׼����Ƶ���  */
		if ((ret = audio_open(channel_layout, nb_channels, sample_rate, &audio_tgt))< 0)
			goto fail;
		audio_hw_buf_size = ret;
		audio_src = audio_tgt;
		audio_buf_size = 0;
		audio_buf_index = 0;

		audio_stream_ = stream_index;
		audio_st = ic_->streams[stream_index];// ��ȡaduio��streamָ��
		
		//��deocder��֡���С������İ���һ��
		auddec.decoder_init(avctx,&this->sample_queue_);
		//���������߳�
		auddec.decoder_start(avctx->codec_type, "audio_thread", this);

		std::cout << audio_st->codecpar->codec_id << std::endl;

		//������Ƶ���
		SDL_PauseAudio(0);
		break;
	case AVMEDIA_TYPE_VIDEO:

		video_stream_ = stream_index;
		video_st = ic_->streams[stream_index];// ��ȡvideo��streamָ��
		
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
		//������ֹ�������߳�
		auddec.decoder_abort();
		//�ر���Ƶ�豸
		audio_close();
		//���ٽ�����
		auddec.decoder_destory();
		//�ͷ��ز�����
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
			//������ֹ�������߳�
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

	//1.����������
	ic_ = avformat_alloc_context();
	if (!ic_)
	{
		av_log(NULL, AV_LOG_FATAL, "Could not allocate ic_.\n");
		ret = AVERROR(ENOMEM);
		goto fail;
	}

	//2. ���ļ�����Ҫ�ǲ鿴�ļ���Э������
	err = avformat_open_input(&ic_, input_filename_, NULL, NULL);
	if (err < 0)
	{
		printf("avformat_open_input is failed.\n ");
		ret = -1;
		goto fail;
	}

	ffp_notify_msg1(this, RA1NP_MSG_OPEN_INPUT);
	std::cout << "read_thread FFP_MSG_OPEN_INPUT" << std::endl;

	/*3.̽��ý������ͣ��ɵõ���ǰ�ļ��ķ�װ��ʽ����Ƶ�������Ϣ��
		������������decode_packet��ȡ��Ϣ�Ĺ���*/
	err = avformat_find_stream_info(ic_, NULL);
	if (err < 0)
	{
		printf("avformat_find_stream_info is failed.\n ");
		ret = -1;
		goto fail;
	}

	//��ȡ��Ƶ��ʱ��
	duration = ic_->duration /AV_TIME_BASE;
	ctrl_callback_(MP_SET_DURATION, duration);

	ffp_notify_msg1(this, RA1NP_MSG_FIND_STREAM_INFO);
	std::cout << "read_thread FFP_MSG_FIND_STREAM_INFO" << std::endl;
	//4.����av_find_best_stream������
	st_index[AVMEDIA_TYPE_VIDEO] =
			av_find_best_stream(ic_, AVMEDIA_TYPE_VIDEO,
								st_index[AVMEDIA_TYPE_VIDEO], 
								-1, NULL, 0);
	
	st_index[AVMEDIA_TYPE_AUDIO] =
		av_find_best_stream(ic_, AVMEDIA_TYPE_AUDIO,
							st_index[AVMEDIA_TYPE_AUDIO],
							st_index[AVMEDIA_TYPE_VIDEO],
							NULL, 0);

	//5.�����Ľ���������������Ӧ�Ľ����߳�
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

	//6.ѭ����ȡ�����ݣ�������������Ӧ�Ķ�����
	for (;;)
	{
		if (abort_request)
		{
			break;
		}

		//seek����
		if (seek_req)
		{
			int64_t pts = (position / duration) * ic_->duration;
			ret = avformat_seek_file(ic_, -1, INT64_MIN, pts, INT64_MAX, 0);
			if (ret >= 0)
			{
				//��ն���
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

	//�󶨻ص�����
	audio_callback = sdl_audio_callback;

	wanted_spec.freq = wanted_sample_rate;
	wanted_spec.channels = wanted_nb_channels;
	wanted_spec.format = AUDIO_S16SYS; //�������ʽ
	wanted_spec.silence = 0;
	wanted_spec.samples = 2048;  // ��û����һ�λص�
	wanted_spec.callback = audio_callback;//�ص�����
	wanted_spec.userdata = this;

	//����Ƶ�豸
	if (SDL_OpenAudio(&wanted_spec, NULL) != 0)
	{
		printf("SDL Open Audio failed!\n");
		return -1;
	}

	//�����ʼ��������Ƶ����
	//wanted_spec�������Ĳ�����spec��ʵ�ʵĲ�����wanted_spec��spec����SDL��Ĳ���
	//audio_hw_params����Ĳ������������ز�����Ҫת���ĸ�ʽ
	audio_hw_params->channels_layout = wanted_channels_layout;
	audio_hw_params->channels = wanted_nb_channels;
	audio_hw_params->freq = wanted_sample_rate;
	audio_hw_params->fmt = AV_SAMPLE_FMT_S16;
	//����һ����������ռ�õ��ֽ���
	audio_hw_params->frame_size = av_samples_get_buffer_size(NULL, 
															audio_hw_params->channels, 
															1, 
															audio_hw_params->fmt,
															1);
	//1����Ҫ���ֽ���
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
	//SDL_OpenAudio ����ȥ���õ�size
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
//			// ��ȡ��ǰ����ֵ������������ʽ����ת����
//			int16_t* sample = (int16_t*)(data[j] + i * sizeof(int16_t));
//			// ��������
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
	//������ˢ��һ֡��������
	video_refresh_callback_(NULL);
	return 0;
}


void FFPlayer::video_refresh(double* remaining_time)
{
	Frame* vp = nullptr;
	//�ж��Ƿ�����Ƶ����
	if (this->video_st)
	{
		//û��ֱ֡�ӷ���
		if (frame_queue_nb_remaining(&picture_queue_) == 0)
			return ;

		vp = frame_queue_peek_readable(&picture_queue_);

		if (!vp)
			return;
		
		double aftime = get_clock(&audclk);
		double diff = vp->pts - aftime;

		//��Ƶ֡������Ƶֱ֡�Ӷ�������������Ƶ֡�ӳ�һ��
		if (diff > 0 && vp->serial != this->serial)
		{
			*remaining_time = FFMIN(*remaining_time, diff);
			return;  
		}
		if (video_refresh_callback_ && vp)
			video_refresh_callback_(vp);

		//ˢ����ʾ
		frame_queue_next(&this->picture_queue_);

	}

}

/*   ����������   */

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
	//���ð�����
	packet_queue_start(frame_queue_->pktq,&flush_pkt);
	//�����߳�
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
	frame_queue_signal(frame_queue_); //����������֡����
	SDL_CondSignal(frame_queue_->cond);
	this->finished_ = 1;

	if (decoder_thread_ && decoder_thread_->joinable())
	{
		decoder_thread_->join();
		delete decoder_thread_;
		decoder_thread_ = nullptr;
	}
	packet_queue_flush(frame_queue_->pktq); //���packet���� ,���ͷ���Դ
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
	AVFrame* frame = av_frame_alloc();// �������֡
	Frame* af;
	int got_frame = 0;// �Ƿ��ȡ��֡
	AVRational tb; // timebase
	int ret = 0;

	if (!frame)
		return AVERROR(ENOMEM);

	for(;;)
	{
		//��ֹ
		if (frame_queue_->pktq->abort_request)
		{
			break;
		}
		//1.��ȡ����֡
		if ((got_frame = decoder_decode_frame(frame)) < 0)
		{
			printf("��ȡ֡ʧ��\n");
			goto the_end;
		}
		if (got_frame)
		{
			// 1����sample_rate Ϊtimebase
			tb ={ 1 , frame->sample_rate };
			//2.��ȡ��д��Frame
			af = frame_queue_peek_writable(&is->sample_queue_);
			if (!af)
				break;

			af->pts = frame->pts;
			af->format = frame->format;
			af->duration = frame->duration;
			af->serial = this->serial_;

			//3.����Frame������FrameQueue
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
		//��ֹ�߳�
		if (frame_queue_->pktq->abort_request)
		{
			goto the_end;
		}

		ret = decoder_decode_frame(frame);
		if (ret < 0)
			goto the_end;
		if (!ret)
			continue;

		//����֡����ʱ�䲢ת��ΪptsֵΪ��
		duration = (frame_rate.num && frame_rate.den ? av_q2d({ frame_rate.num, frame_rate.den }) : 0);
		//����timebase�����ptsֵ����λΪ��
		pts = (frame->pts == AV_NOPTS_VALUE)? NAN : frame->pts * av_q2d(tb);
		
		//��������֡���뵽������
		ret = put_picture(&is->picture_queue_,frame,pts);
		//��������
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
		//1.���Զ�һ֡
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
						//ת����Ŀ��ʱ���׼ȷ����ȷ��ʱ��˳���ͬ��
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

			//1.2�������Ƿ��Ѿ����� ����0
			if (ret == AVERROR_EOF)
				return 0;

			//1.3�������뷵��1
			if (ret >= 0)
			{
				return 1;
			}

		} while ( ret >= 0 || ret == AVERROR_EOF);
		//2.����ʽ��ȡpacket�͸�������
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

	//֡��������Ҫ�����ȴ�ʹ��
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

#include "ra1nplayer.h"
#include <iostream>
#include "ffmsg.h"

Ra1nPlayer::Ra1nPlayer()
{
}

Ra1nPlayer::~Ra1nPlayer()
{
}

int Ra1nPlayer::ra1nmp_create(std::function<int(void*)> msg_loop)
{
	int ret = 0;
	ffplayer_ = new FFPlayer();
	if (!ffplayer_)
	{
		std::cout << "new ffplayer fialed!\n" << std::endl;
		return -1;
	}
	msg_loop_ = msg_loop;
	
	ret = ffplayer_->ffp_create();

	if (ret < 0)
		return -1;

	return 0;
}

int Ra1nPlayer::ra1nmp_set_data_source(const char* url)
{
	if (!url)
		return -1;
	data_source_ = strdup(url);//分配内存，并拷贝
	return 0;
}

int Ra1nPlayer::ra1nmp_prepare_async()
{
	//判断mp的状态

	//正在准备中
	mp_state_ = MP_STATE_ASYNC_PREPARING;
	//启用消息队列
	msg_queue_start(&ffplayer_->msg_queue_); 
	//创建循环线程
	msg_thread_ = new std::thread(&Ra1nPlayer::ra1nmp_msg_loop,this,this);
	//调用ffplayer
	int ret = ffplayer_->ffp_prepare_async_l(data_source_);
	paused_ = 0;
	if (ret < 0)
	{
		mp_state_ = MP_STATE_ERROR; 
		return -1;
	}
	return 0;
}

int Ra1nPlayer::ra1nmp_play()
{
	paused_ = 0;
	ffplayer_->paused = 0;
	return 0;
}

int Ra1nPlayer::ra1nmp_destroy()
{
	ffplayer_->stream_close();
	return 0;
}

int Ra1nPlayer::ra1nmp_pause()
{
	paused_ = 1;
	ffplayer_->paused = 1;
	return 0;
}

int Ra1nPlayer::rainmp_seek(double position)
{
	ffplayer_->seek_req = true;
	ffplayer_->position = position;
	return 0;
}

void Ra1nPlayer::ra1nmp_set_volume(float position)
{
	ffplayer_->volume = static_cast<int>(position);
}

int Ra1nPlayer::ra1nmp_get_msg(AVMessage* msg, int block)
{
	while (1)
	{
		int continue_wait_next_msg = 0;
		//取消息，如果没有消息则阻塞
		int retval = msg_queue_get(&ffplayer_->msg_queue_, msg, block);
		if (retval <= 0)
			return retval;

		switch (msg->what)
		{
		case FFP_MSG_START:
			continue_wait_next_msg = 1;
			break;
		case RA1NP_MSG_STOP:
			ffplayer_->stream_close();
			break;
		default:
			break;
		}
		if (continue_wait_next_msg)
		{
			msg_free_res(msg);
			continue;
		}
		return retval;
	}
	return 0;
}

int Ra1nPlayer::ra1nmp_msg_loop(void* arg)
{
	msg_loop_(arg);
	return 0;
}

void Ra1nPlayer::ra1nmp_set_volum_muted()
{
	ffplayer_->volum_muted = !ffplayer_->volum_muted;
}

void Ra1nPlayer::SetVideofreshCallback(std::function<int(const Frame*)> callback)
{
	ffplayer_->SetVideoRefreshCallBack(callback);
}

void Ra1nPlayer::SetCtrlCallBack(std::function<void(int ,double)> callback)
{
	ffplayer_->SetCtrlCallBack(callback);
}

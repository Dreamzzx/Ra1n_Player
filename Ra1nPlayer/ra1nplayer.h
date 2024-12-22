#ifndef RAINPLAYER_H
#define RAINPLAYER_H

#include <mutex>
#include <functional>
#include <thread>
#include "ffmsg_queue.h"
#include "ff_play.h"
#include "ffmsg.h"
class Ra1nPlayer
{
public:
	Ra1nPlayer();
	~Ra1nPlayer();

	int ra1nmp_create(std::function<int(void*)> msg_loop);
	//设置要播放的url
	int ra1nmp_set_data_source(const char* url);
	//准备播放
	int ra1nmp_prepare_async();
	//继续播放
	int ra1nmp_play();
	//暂停播放
	int ra1nmp_stop();
	//中止播放
	int ra1nmp_destroy();
	//暂停播放
	int ra1nmp_pause();
	//seek到指定位置
	int rainmp_seek(double position);
	//获取播放状态
	int rainmp_get_state();
	//是不是播放中
	bool ra1nmp_is_playing();
	//当前播放位置
	long ra1nmp_get_current_position();
	//总长度
	long ra1nmp_get_duration();
	//已经播放的长度
	long ra1nmp_get_playable_duration();
	//设置是否循环播放
	void ra1nmp_set_loop();
	//读取消息
	int ra1nmp_get_msg(AVMessage* msg, int block);
	//设置音量
	void ra1nmp_set_volume(float volume);
	//静音
	void ra1nmp_set_volum_muted();

	inline void ra1nmp_set_speed(const float& speed) {
		ffplayer_->volume_speed = speed;
	}
	
	//循环函数
	int ra1nmp_msg_loop(void *arg);
	
	void SetVideofreshCallback(std::function<int(const Frame*)>callback);

	void SetCtrlCallBack(std::function<void(int, double)> callback);

	FFPlayer* getffplayer()
	{
		return ffplayer_;
	}
	//查看视频是否暂停
	int is_paused()
	{
		return paused_;
	}
private:
	std::mutex mutex_; 
	FFPlayer *ffplayer_ = nullptr;

	//函数指针指向创建的message_loop,消息循环函数
	std::function<int(void*)> msg_loop_ = nullptr;
	std::thread* msg_thread_ = nullptr;
	char* data_source_ = nullptr;//url
	int mp_state_;//播放状态
	int paused_; // 0播放 1 暂停
};
#endif // !RAINPLAYER_H



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
	//����Ҫ���ŵ�url
	int ra1nmp_set_data_source(const char* url);
	//׼������
	int ra1nmp_prepare_async();
	//��������
	int ra1nmp_play();
	//��ͣ����
	int ra1nmp_stop();
	//��ֹ����
	int ra1nmp_destroy();
	//��ͣ����
	int ra1nmp_pause();
	//seek��ָ��λ��
	int rainmp_seek(double position);
	//��ȡ����״̬
	int rainmp_get_state();
	//�ǲ��ǲ�����
	bool ra1nmp_is_playing();
	//��ǰ����λ��
	long ra1nmp_get_current_position();
	//�ܳ���
	long ra1nmp_get_duration();
	//�Ѿ����ŵĳ���
	long ra1nmp_get_playable_duration();
	//�����Ƿ�ѭ������
	void ra1nmp_set_loop();
	//��ȡ��Ϣ
	int ra1nmp_get_msg(AVMessage* msg, int block);
	//��������
	void ra1nmp_set_volume(float volume);
	//����
	void ra1nmp_set_volum_muted();

	inline void ra1nmp_set_speed(const float& speed) {
		ffplayer_->volume_speed = speed;
	}
	
	//ѭ������
	int ra1nmp_msg_loop(void *arg);
	
	void SetVideofreshCallback(std::function<int(const Frame*)>callback);

	void SetCtrlCallBack(std::function<void(int, double)> callback);

	FFPlayer* getffplayer()
	{
		return ffplayer_;
	}
	//�鿴��Ƶ�Ƿ���ͣ
	int is_paused()
	{
		return paused_;
	}
private:
	std::mutex mutex_; 
	FFPlayer *ffplayer_ = nullptr;

	//����ָ��ָ�򴴽���message_loop,��Ϣѭ������
	std::function<int(void*)> msg_loop_ = nullptr;
	std::thread* msg_thread_ = nullptr;
	char* data_source_ = nullptr;//url
	int mp_state_;//����״̬
	int paused_; // 0���� 1 ��ͣ
};
#endif // !RAINPLAYER_H



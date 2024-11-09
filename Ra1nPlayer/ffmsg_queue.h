#ifndef FFMSG_QUEUE_H
#define FFMSG_QUEUE_H

#include <SDL.h>

typedef struct AVMessage
{
	int what; // 消息类型
	int arg1;
	int arg2;
	void* obj;
	void (*free_l)(void* obj);//函数指针释放obj内存
	struct AVMessage* next;
}AVMessage;

typedef struct MessageQueue
{
	AVMessage* first_msg, *last_msg;
	int nb_messages;//有多少个消息
	int abort_request;
	SDL_cond* cond;
	SDL_mutex* mutex;
	AVMessage* recycle_meg;
	int recycle_count; //循环的次数
	int alloc_count;//分配空间的次数
}MessageQueue;

//释放msg的obj资源
void msg_free_res(AVMessage* msg); 


//放入msg消息(内部)
int msg_queue_put_private(MessageQueue *q,AVMessage *msg);

//对外开放的put
int msg_queue_put(MessageQueue* q, AVMessage* msg);

//获取msg消息
int msg_queue_get(MessageQueue* q,AVMessage *msg,int block);

//初始化
void msg_init_msg(AVMessage* msg);

//放入队列消息
void msg_queue_put_simple1(MessageQueue* q, int what);
void msg_queue_put_simple2(MessageQueue* q, int what,int arg1);
void msg_queue_put_simple3(MessageQueue* q, int what,int arg1,int arg2);
void msg_queue_put_simple4(MessageQueue* q, int what,int arg1,int arg2,void *obj,int obj_len);

 //释放msg的obj资源
void msg_obj_free_l(void *obj);

//初始化消息队列
void msg_queue_init(MessageQueue* q);

//清空消息队列
void msg_queue_flush(MessageQueue* q);

//消息队列销毁
void msg_queue_destory(MessageQueue* q);

//暂停消息队列
void msg_queue_abort(MessageQueue* q);

//启动消息队列
void msg_queue_start(MessageQueue* q);

//删除消息
void msg_queue_remove(MessageQueue* q,int what);

#endif // !FFMSG_QUEUE_H



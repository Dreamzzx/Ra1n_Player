#ifndef FFMSG_QUEUE_H
#define FFMSG_QUEUE_H

#include <SDL.h>

typedef struct AVMessage
{
	int what; // ��Ϣ����
	int arg1;
	int arg2;
	void* obj;
	void (*free_l)(void* obj);//����ָ���ͷ�obj�ڴ�
	struct AVMessage* next;
}AVMessage;

typedef struct MessageQueue
{
	AVMessage* first_msg, *last_msg;
	int nb_messages;//�ж��ٸ���Ϣ
	int abort_request;
	SDL_cond* cond;
	SDL_mutex* mutex;
	AVMessage* recycle_meg;
	int recycle_count; //ѭ���Ĵ���
	int alloc_count;//����ռ�Ĵ���
}MessageQueue;

//�ͷ�msg��obj��Դ
void msg_free_res(AVMessage* msg); 


//����msg��Ϣ(�ڲ�)
int msg_queue_put_private(MessageQueue *q,AVMessage *msg);

//���⿪�ŵ�put
int msg_queue_put(MessageQueue* q, AVMessage* msg);

//��ȡmsg��Ϣ
int msg_queue_get(MessageQueue* q,AVMessage *msg,int block);

//��ʼ��
void msg_init_msg(AVMessage* msg);

//���������Ϣ
void msg_queue_put_simple1(MessageQueue* q, int what);
void msg_queue_put_simple2(MessageQueue* q, int what,int arg1);
void msg_queue_put_simple3(MessageQueue* q, int what,int arg1,int arg2);
void msg_queue_put_simple4(MessageQueue* q, int what,int arg1,int arg2,void *obj,int obj_len);

 //�ͷ�msg��obj��Դ
void msg_obj_free_l(void *obj);

//��ʼ����Ϣ����
void msg_queue_init(MessageQueue* q);

//�����Ϣ����
void msg_queue_flush(MessageQueue* q);

//��Ϣ��������
void msg_queue_destory(MessageQueue* q);

//��ͣ��Ϣ����
void msg_queue_abort(MessageQueue* q);

//������Ϣ����
void msg_queue_start(MessageQueue* q);

//ɾ����Ϣ
void msg_queue_remove(MessageQueue* q,int what);

#endif // !FFMSG_QUEUE_H



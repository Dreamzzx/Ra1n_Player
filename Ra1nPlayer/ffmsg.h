#ifndef FFMSG_H
#define FFMSG_H

#define FFP_MSG_FLUSH						 0
#define RA1NP_MSG_ERROR						100 // ���ִ��� arg1 = error
#define RA1NP_MSG_PREPARED					200 // ׼������
#define RA1NP_MSG_COMPLETED					300 // �������
#define RA1NP_MSG_VIDEO_SIZE_CHANGED			400 // ��Ƶ��С���ͱ任 arg1 = width arg2 = height
#define RA1NP_MSG_SAR_CHANGED					401 // arg1 = sar.num   arg2 = sar.den
#define RA1NP_MSG_VIDEO_RENDERING_START       402 // ��ʼ������Ⱦ
#define RA1NP_MSG_AUDIO_RENDERING_START       403 // ��ʼ�������
#define RA1NP_MSG_PROTATION_CHANGED			404 // arg1 = degree
#define RA1NP_MSG_AUDIO_DECODE_START			405 // ��ʼ��Ƶ����
#define RA1NP_MSG_VIDEO_DECODE_START			406 // ��ʼ��Ƶ����
#define RA1NP_MSG_OPEN_INPUT					407 // read_thread ������ avformat_open_input
#define RA1NP_MSG_FIND_STREAM_INFO			408 // read_thread ������ avformat_find_stream_info
#define RA1NP_MSG_COMPOMENT_OPEN				409 // read_thread ������ stream_compoment_open

#define FFP_MSG_START						20001// ��Ϣ���п�ʼ
#define RA1NP_MSG_STOP						20002// ��ֹ����

#define MP_STATE_ERROR						 -1
#define MP_STATE_ASYNC_PREPARING			500
#define MP_STATE_PLAY						501
#define MP_STATE_PAUSE						502

#define MP_SET_PLAY_TIME                    601//��ʱ�Ӹ��ĺ� �ı�ʱ���ǩ��ʱ��
#define MP_INIT_TIME						602//��ʼ��ʱ�� ������Ϊ0
#define MP_SET_DURATION						603//������Ƶ��ʱ�� ������siler��Χ
#define MP_FAST_SEEK						604//ǰ��
#define MP_BACK_SEEK						605//����
#define MP_SEEK_TO							60//seek��

#endif // !FFMSG_H
  

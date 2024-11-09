#ifndef FFMSG_H
#define FFMSG_H

#define FFP_MSG_FLUSH						 0
#define RA1NP_MSG_ERROR						100 // 出现错误 arg1 = error
#define RA1NP_MSG_PREPARED					200 // 准备好了
#define RA1NP_MSG_COMPLETED					300 // 播放完成
#define RA1NP_MSG_VIDEO_SIZE_CHANGED			400 // 视频大小发送变换 arg1 = width arg2 = height
#define RA1NP_MSG_SAR_CHANGED					401 // arg1 = sar.num   arg2 = sar.den
#define RA1NP_MSG_VIDEO_RENDERING_START       402 // 开始画面渲染
#define RA1NP_MSG_AUDIO_RENDERING_START       403 // 开始声音输出
#define RA1NP_MSG_PROTATION_CHANGED			404 // arg1 = degree
#define RA1NP_MSG_AUDIO_DECODE_START			405 // 开始音频解码
#define RA1NP_MSG_VIDEO_DECODE_START			406 // 开始视频解码
#define RA1NP_MSG_OPEN_INPUT					407 // read_thread 调用了 avformat_open_input
#define RA1NP_MSG_FIND_STREAM_INFO			408 // read_thread 调用了 avformat_find_stream_info
#define RA1NP_MSG_COMPOMENT_OPEN				409 // read_thread 调用了 stream_compoment_open

#define FFP_MSG_START						20001// 消息队列开始
#define RA1NP_MSG_STOP						20002// 中止播放

#define MP_STATE_ERROR						 -1
#define MP_STATE_ASYNC_PREPARING			500
#define MP_STATE_PLAY						501
#define MP_STATE_PAUSE						502

#define MP_SET_PLAY_TIME                    601//主时钟更改后 改变时间标签的时间
#define MP_INIT_TIME						602//初始化时间 都设置为0
#define MP_SET_DURATION						603//设置视频总时间 并设置siler范围
#define MP_FAST_SEEK						604//前进
#define MP_BACK_SEEK						605//后退
#define MP_SEEK_TO							60//seek到

#endif // !FFMSG_H
  

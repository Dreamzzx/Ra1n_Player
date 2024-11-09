#include "ff_data.h"

static AVPacket flush_pkt;

void packet_queue_start(PacketQueue* queue,AVPacket* flush_pkt)
{
	av_init_packet(flush_pkt);
	flush_pkt->data = (uint8_t*)&flush_pkt;
	SDL_LockMutex(queue->mutex);
	queue->abort_request = 0;
	packet_queue_put_pkt(queue, flush_pkt);
	SDL_UnlockMutex(queue->mutex); 
}

void packet_queue_put_pkt(PacketQueue* q,AVPacket* pkt)
{
	SDL_LockMutex(q->mutex);
	while (q->nb_packets >= PKTS_MAX_SIZE) {
		SDL_CondWait(q->not_full, q->mutex); // 等待条件变量，解锁并阻塞线程，直到被唤醒
	}
	q->pkts.push_back(*pkt);
	q->nb_packets++;
	q->size += pkt->size;

	SDL_CondSignal(q->cond); //通知get_pkt有资源可以获得
	SDL_UnlockMutex(q->mutex);

}

int packet_queue_get_pkt(PacketQueue* q,AVPacket* pkt)
{
	int ret = 0;
	SDL_LockMutex(q->mutex);
	while (!q->abort_request) //检查是否退出
	{
		if (!q->pkts.empty())
		{
			AVPacket& firstpkt = q->pkts.front();
			*pkt = firstpkt;
			q->size -= firstpkt.size;
			q->pkts.pop_front();
			q->nb_packets--;

			ret = 1;
			break;
		}
		else
		{
			SDL_CondWaitTimeout(q->cond,q->mutex,500);
		}
	}
	SDL_CondSignal(q->not_full); //唤醒put包
	SDL_UnlockMutex(q->mutex);

	if (q->abort_request) 
		ret = 1;

	return ret;
}

int packet_queue_init(PacketQueue* q)
{
	if (!q->mutex)
		q->mutex = SDL_CreateMutex();
	if (!q->cond)
		q->cond = SDL_CreateCond();
	if(!q->not_full)
		q->not_full = SDL_CreateCond();

	q->duration = 0;
	q->size = 0;
	q->nb_packets = 0;

	return 0;
}

void packet_queue_abort(PacketQueue* q)
{
	SDL_LockMutex(q->mutex);
	q->abort_request = 1;//请求退出
	SDL_CondSignal(q->cond); //释放一个请求信号
	SDL_CondSignal(q->not_full);
	SDL_UnlockMutex(q->mutex);
	return;
}

void packet_queue_flush(PacketQueue* q)
{
	AVPacket* pkt1 = nullptr;
	SDL_LockMutex(q->mutex);
	//遍历包队列并释放资源
	while (!q->pkts.empty())
	{
		pkt1 = &q->pkts.front();
		av_packet_unref(pkt1);
		q->pkts.pop_front();
	}
	q->size = 0;
	q->duration = 0;
	q->nb_packets = 0;
	SDL_UnlockMutex(q->mutex);
	if(pkt1)
		pkt1 = nullptr;
}


void packet_queue_destroy(PacketQueue*q)
{
	packet_queue_flush(q); //清空packet队列 ,并释放资源
	
	if (q->cond)
	{
		SDL_DestroyCond(q->cond);
		q->cond = nullptr;
	}

	if (q->not_full)
	{
		SDL_DestroyCond(q->not_full);
		q->not_full = nullptr;
	}

	if (q->mutex)
	{
		SDL_DestroyMutex(q->mutex);
		q->mutex = nullptr;
	}

}

void packet_queue_signal(PacketQueue* q)
{
	SDL_LockMutex(q->mutex);
	SDL_CondSignal(q->cond);
	SDL_UnlockMutex(q->mutex);
}

double get_clock(Clock* c)
{
	//获取微妙转化成秒
	double time = av_gettime_relative() / 1000000.0;
	return time + c->pts_drift;
}

void set_clock_at(Clock* c,double pts,double time)
{
	c->pts = pts;    //当前帧的pts
	c->last_updated = time; //最后更新的时间
	c->pts_drift = c->pts - time; //当前帧pts和系统时间的差值
}

void set_clock(Clock* c, double pts)
{
	double time = av_gettime_relative() / 1000000.0;
	set_clock_at(c, pts, time);
}

void init_clock(Clock* c)
{
	set_clock(c, NAN);
}

/*         FRAMEQUEUE         */
int frame_queue_init(FrameQueue* q,PacketQueue* pktq_)
{
	if (!q->mutex)
		q->mutex = SDL_CreateMutex();

	if (!q->cond)
		q->cond = SDL_CreateCond();

	q->pktq = pktq_;
	q->rindex = 0;
	q->windex = 0;
	q->size = 0;
	q->max_size = FRAME_QUEUE_SIZE;

	for (int i = 0; i < q->max_size; i++)
		q->queue[i].frame = av_frame_alloc();

	return 0;
}

void frame_queue_flush(FrameQueue* q)
{
	AVFrame* f1 = nullptr;
	SDL_LockMutex(q->mutex);
	for (int i = 0; i < FRAME_QUEUE_SIZE; i++)
	{
		if (q->queue[i].frame != nullptr)
		{
			f1 = q->queue[i].frame;
			av_frame_free(&f1);
		}
	}

	q->rindex = 0;
	q->windex = 0;
	q->size = 0;
	q->max_size = 0;
	SDL_UnlockMutex(q->mutex);
	if (f1)
		f1 = nullptr;
}

void frame_queue_destroy(FrameQueue*q)
{
	frame_queue_flush(q);

	if (q->mutex)
	{
		SDL_DestroyMutex(q->mutex);
		q->mutex = nullptr;
	}

	if (q->cond)
	{
		q->cond = SDL_CreateCond();
		q->cond = nullptr;
	}

	q->pktq = nullptr;
}

Frame* frame_queue_peek_writable(FrameQueue* q)
{
	SDL_LockMutex(q->mutex);
	while (q->size >=  q->max_size &&
			!q->pktq->abort_request )
	{
		SDL_CondWait(q->cond, q->mutex);
	}
	SDL_UnlockMutex(q->mutex);

	if (q->pktq->abort_request)
		return NULL;

	return &q->queue[q->windex];
}

Frame* frame_queue_peek_readable(FrameQueue* q)
{
	 SDL_LockMutex(q->mutex);
	while (q->size - q->rindex<= 0 && 
			q->pktq->abort_request != 1)
	{
		SDL_CondWait(q->cond, q->mutex);
	}

	SDL_UnlockMutex(q->mutex);

	if (q->pktq->abort_request)
	{
	std::cout << q->pktq->abort_request << std::endl;
		return NULL;
	}

	return &q->queue[q->rindex % FRAME_QUEUE_SIZE];
}

void frame_queue_push(FrameQueue* q)
{
	if (++q->windex == q->max_size)
		q->windex = 0;
	SDL_LockMutex(q->mutex);
	q->size++;
	SDL_CondSignal(q->cond); // 唤醒读取
	SDL_UnlockMutex(q->mutex);
}

void frame_queue_next(FrameQueue* q)
{
	//释放掉目前帧的资源 然后索引指向下一帧
	frame_queue_unref_item(&q->queue[q->rindex]);
	if (++q->rindex == q->max_size)
		q->rindex = 0;
	SDL_LockMutex(q->mutex);
	q->size--;
	SDL_CondSignal(q->cond);
	SDL_UnlockMutex(q->mutex);
}

void frame_queue_unref_item(Frame* frame)
{
	av_frame_unref(frame->frame);
}

int frame_queue_nb_remaining(FrameQueue* q)
{
	return q->size;
}

void frame_queue_signal(FrameQueue* q)
{
	SDL_LockMutex(q->mutex);
	SDL_CondSignal(q->cond);
	SDL_UnlockMutex(q->mutex);
}

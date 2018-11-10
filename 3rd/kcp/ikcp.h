//=====================================================================
//
// KCP - A Better ARQ Protocol Implementation
// skywind3000 (at) gmail.com, 2010-2011
//  
// Features:
// + Average RTT reduce 30% - 40% vs traditional ARQ like tcp.
// + Maximum RTT reduce three times vs tcp.
// + Lightweight, distributed as a single source file.
//
//=====================================================================
#ifndef __IKCP_H__
#define __IKCP_H__

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>


//=====================================================================
// 32BIT INTEGER DEFINITION 
//=====================================================================
#ifndef __INTEGER_32_BITS__
#define __INTEGER_32_BITS__
#if defined(_WIN64) || defined(WIN64) || defined(__amd64__) || \
	defined(__x86_64) || defined(__x86_64__) || defined(_M_IA64) || \
	defined(_M_AMD64)
	typedef unsigned int ISTDUINT32;
	typedef int ISTDINT32;
#elif defined(_WIN32) || defined(WIN32) || defined(__i386__) || \
	defined(__i386) || defined(_M_X86)
	typedef unsigned long ISTDUINT32;
	typedef long ISTDINT32;
#elif defined(__MACOS__)
	typedef UInt32 ISTDUINT32;
	typedef SInt32 ISTDINT32;
#elif defined(__APPLE__) && defined(__MACH__)
	#include <sys/types.h>
	typedef u_int32_t ISTDUINT32;
	typedef int32_t ISTDINT32;
#elif defined(__BEOS__)
	#include <sys/inttypes.h>
	typedef u_int32_t ISTDUINT32;
	typedef int32_t ISTDINT32;
#elif (defined(_MSC_VER) || defined(__BORLANDC__)) && (!defined(__MSDOS__))
	typedef unsigned __int32 ISTDUINT32;
	typedef __int32 ISTDINT32;
#elif defined(__GNUC__)
	#include <stdint.h>
	typedef uint32_t ISTDUINT32;
	typedef int32_t ISTDINT32;
#else 
	typedef unsigned long ISTDUINT32; 
	typedef long ISTDINT32;
#endif
#endif


//=====================================================================
// Integer Definition
//=====================================================================
#ifndef __IINT8_DEFINED
#define __IINT8_DEFINED
typedef char IINT8;
#endif

#ifndef __IUINT8_DEFINED
#define __IUINT8_DEFINED
typedef unsigned char IUINT8;
#endif

#ifndef __IUINT16_DEFINED
#define __IUINT16_DEFINED
typedef unsigned short IUINT16;
#endif

#ifndef __IINT16_DEFINED
#define __IINT16_DEFINED
typedef short IINT16;
#endif

#ifndef __IINT32_DEFINED
#define __IINT32_DEFINED
typedef ISTDINT32 IINT32;
#endif

#ifndef __IUINT32_DEFINED
#define __IUINT32_DEFINED
typedef ISTDUINT32 IUINT32;
#endif

#ifndef __IINT64_DEFINED
#define __IINT64_DEFINED
#if defined(_MSC_VER) || defined(__BORLANDC__)
typedef __int64 IINT64;
#else
typedef long long IINT64;
#endif
#endif

#ifndef __IUINT64_DEFINED
#define __IUINT64_DEFINED
#if defined(_MSC_VER) || defined(__BORLANDC__)
typedef unsigned __int64 IUINT64;
#else
typedef unsigned long long IUINT64;
#endif
#endif

#ifndef INLINE
#if defined(__GNUC__)

#if (__GNUC__ > 3) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1))
#define INLINE         __inline__ __attribute__((always_inline))
#else
#define INLINE         __inline__
#endif

#elif (defined(_MSC_VER) || defined(__BORLANDC__) || defined(__WATCOMC__))
#define INLINE __inline
#else
#define INLINE 
#endif
#endif

#if (!defined(__cplusplus)) && (!defined(inline))
#define inline INLINE
#endif


//=====================================================================
// QUEUE DEFINITION                                                  
//=====================================================================
#ifndef __IQUEUE_DEF__
#define __IQUEUE_DEF__

struct IQUEUEHEAD {
	struct IQUEUEHEAD *next, *prev;
};

typedef struct IQUEUEHEAD iqueue_head;


//---------------------------------------------------------------------
// queue init                                                         
//---------------------------------------------------------------------
#define IQUEUE_HEAD_INIT(name) { &(name), &(name) }
#define IQUEUE_HEAD(name) \
	struct IQUEUEHEAD name = IQUEUE_HEAD_INIT(name)

#define IQUEUE_INIT(ptr) ( \
	(ptr)->next = (ptr), (ptr)->prev = (ptr))

#define IOFFSETOF(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#define ICONTAINEROF(ptr, type, member) ( \
		(type*)( ((char*)((type*)ptr)) - IOFFSETOF(type, member)) )

#define IQUEUE_ENTRY(ptr, type, member) ICONTAINEROF(ptr, type, member)


//---------------------------------------------------------------------
// queue operation                     
//---------------------------------------------------------------------
#define IQUEUE_ADD(node, head) ( \
	(node)->prev = (head), (node)->next = (head)->next, \
	(head)->next->prev = (node), (head)->next = (node))

#define IQUEUE_ADD_TAIL(node, head) ( \
	(node)->prev = (head)->prev, (node)->next = (head), \
	(head)->prev->next = (node), (head)->prev = (node))

#define IQUEUE_DEL_BETWEEN(p, n) ((n)->prev = (p), (p)->next = (n))

#define IQUEUE_DEL(entry) (\
	(entry)->next->prev = (entry)->prev, \
	(entry)->prev->next = (entry)->next, \
	(entry)->next = 0, (entry)->prev = 0)

#define IQUEUE_DEL_INIT(entry) do { \
	IQUEUE_DEL(entry); IQUEUE_INIT(entry); } while (0)

#define IQUEUE_IS_EMPTY(entry) ((entry) == (entry)->next)

#define iqueue_init		IQUEUE_INIT
#define iqueue_entry	IQUEUE_ENTRY
#define iqueue_add		IQUEUE_ADD
#define iqueue_add_tail	IQUEUE_ADD_TAIL
#define iqueue_del		IQUEUE_DEL
#define iqueue_del_init	IQUEUE_DEL_INIT
#define iqueue_is_empty IQUEUE_IS_EMPTY

#define IQUEUE_FOREACH(iterator, head, TYPE, MEMBER) \
	for ((iterator) = iqueue_entry((head)->next, TYPE, MEMBER); \
		&((iterator)->MEMBER) != (head); \
		(iterator) = iqueue_entry((iterator)->MEMBER.next, TYPE, MEMBER))

#define iqueue_foreach(iterator, head, TYPE, MEMBER) \
	IQUEUE_FOREACH(iterator, head, TYPE, MEMBER)

#define iqueue_foreach_entry(pos, head) \
	for( (pos) = (head)->next; (pos) != (head) ; (pos) = (pos)->next )
	

#define __iqueue_splice(list, head) do {	\
		iqueue_head *first = (list)->next, *last = (list)->prev; \
		iqueue_head *at = (head)->next; \
		(first)->prev = (head), (head)->next = (first);		\
		(last)->next = (at), (at)->prev = (last); }	while (0)

#define iqueue_splice(list, head) do { \
	if (!iqueue_is_empty(list)) __iqueue_splice(list, head); } while (0)

#define iqueue_splice_init(list, head) do {	\
	iqueue_splice(list, head);	iqueue_init(list); } while (0)


#ifdef _MSC_VER
#pragma warning(disable:4311)
#pragma warning(disable:4312)
#pragma warning(disable:4996)
#endif

#endif


//---------------------------------------------------------------------
// WORD ORDER
//---------------------------------------------------------------------
#ifndef IWORDS_BIG_ENDIAN
    #ifdef _BIG_ENDIAN_
        #if _BIG_ENDIAN_
            #define IWORDS_BIG_ENDIAN 1
        #endif
    #endif
    #ifndef IWORDS_BIG_ENDIAN
        #if defined(__hppa__) || \
            defined(__m68k__) || defined(mc68000) || defined(_M_M68K) || \
            (defined(__MIPS__) && defined(__MIPSEB__)) || \
            defined(__ppc__) || defined(__POWERPC__) || defined(_M_PPC) || \
            defined(__sparc__) || defined(__powerpc__) || \
            defined(__mc68000__) || defined(__s390x__) || defined(__s390__)
            #define IWORDS_BIG_ENDIAN 1
        #endif
    #endif
    #ifndef IWORDS_BIG_ENDIAN
        #define IWORDS_BIG_ENDIAN  0
    #endif
#endif


/* 
 * 名词说明:
 * mtu: 最大传输单元,即每次发送的最大数据
 * rto: Retransmission TimeOut,重传超时间隔
 * cwnd: congestion window, 拥塞窗口,限制发送方可发送kcp包个数,与接收方窗口有关,与网络状况有关
 * rwnd: receiver window,接收方窗口，表示接收方还可以接收多少个kcp包
 *
 * 可靠性: 1.不丢包 2.收包有序
 * 通过对每个包编号+确认机制可以保证不丢包,通过滑动窗口可以保证收包有序
 * [rcv_nxt,rcv_nxt+rcv_wnd) 构成接收滑动窗口
 * [snd_una,snd_nxt) 构成发送滑动窗口
 * 消息包最大大小: 256*mss
 *
 * kcp支持多项配置,如:
 * 1. 极速模式/普通模式
 * 2. 关闭拥塞控制/拥塞控制
 * 3. 快速重传模式，默认0关闭，如设置2，2次跨越将立即重传
 * 4. 窗口大小
 * 5. mtu大小
 * 6. 流模式/报文模式
 * 等等
 *
 * 流模式和报文模式区别:
 * 不管哪种模式下,kcp都会按mtu对上层发包拆分成分片,真正发送时,连续的
 * 较小分片还会尽量合并成mtu大小一次发送。流模式下会把两次发送的数据衔接成一
 * 个kcp分片,以保证每个分片都达到mtu值,而报文模式下允许发送<mtu的包。流模式
 * 类似tcp,上层需要处理粘包问题，报文模式则不必
 */

//=====================================================================
// SEGMENT
//=====================================================================
struct IKCPSEG
{
	struct IQUEUEHEAD node;
	IUINT32 conv;			// 会话ID
	IUINT32 cmd;			// 命令
	IUINT32 frg;			// 分片ID
	IUINT32 wnd;			// 接收窗口剩余大小
	IUINT32 ts;				// 分片发送时间(毫秒为单位),对于ICKP_CMD_ACK会复制发送方的时间
	IUINT32 sn;				// 发包序列号
	IUINT32 una;			// sn < una的分片已收到
	IUINT32 len;			// 数据长度
	IUINT32 resendts;		// 分片下次重发时间点
	IUINT32 rto;			// 重传间隔
	IUINT32 fastack;		// ack被跳过次数(受到>自身序号ack包次数),重发后会清零
	IUINT32 xmit;			// 发送次数(未确认次数)
	char data[1];			// 分片数据
};


//---------------------------------------------------------------------
// IKCPCB
//---------------------------------------------------------------------
struct IKCPCB
{
	// conv--会话ID,mtu--最大传输大小,mss--去掉头部的最大传输大小,state--网络状态(-1:可能瘫痪,0--正常)
	IUINT32 conv, mtu, mss, state;
	// snd_una--发送方最小未确认包序号
	// snd_nxt--发送方最大包序号(排除此值),每发一个包会自增1(snd_buf每增加一个分片时)
	// rcv_nxt--每有序收到一个包自增1(rcv_queue每增加一个分片时)
	IUINT32 snd_una, snd_nxt, rcv_nxt;
	// ssthresh--cwnd的最大值
	// 其他值没用到
	IUINT32 ts_recent, ts_lastack, ssthresh;
	// rx_rto--重传间隔
	// 其他值均为中途调整rx_rto的中间值
	IINT32 rx_rttval, rx_srtt, rx_rto, rx_minrto;
	// snd_wnd--发送窗口大小
	// rcv_wnd--接收窗口大小
	// rmt_wnd--对方接受窗口可用大小
	// cwnd--拥塞控制大小,不开启拥塞控制下有用,正常情况由min(发送窗口大小,对方可接收大小)决定
	// 发送数据量,开启拥塞控制后,cwnd也会参与取小,而cwnd可以根据网络好坏调整大小
	// probe--记录下个tick需要发送的ICKP_ASK_SEND/IKCP_ASK_TELL
	IUINT32 snd_wnd, rcv_wnd, rmt_wnd, cwnd, probe;
	// current--当前时间戳(毫秒为单位)
	// interval--下次刷新时间间隔
	// ts_flush--下次刷新时间点
	// xmit--所有分片重发次数总和
	IUINT32 current, interval, ts_flush, xmit;
	// nrcv_buf--rcv_buf队列元素数量
	// nsnd_buf--snd_buf队列元素数量
	IUINT32 nrcv_buf, nsnd_buf;
	// nrcv_que--rcv_queue队列元素数量
	// nsnd_que--snd_queue队列元素数量
	IUINT32 nrcv_que, nsnd_que;
	// nodelay: 1--快速模式(该模式下最小重传间隔为30ms),0--普通模式(该模式下最小重传间隔为100ms)
	// updated--ikcp_update调用过后设置为1
	IUINT32 nodelay, updated;
	// ts_probe--大于该时间后,必要情况才发命令IKCP_CMD_WASK
	// probe_wait--多少间隔时间内不重复发送IKCP_CMD_WASK
	IUINT32 ts_probe, probe_wait;
	// dead_link--重发多少次未收到确认视为网络瘫痪
	// incr--用来控制cwnd增长速度
	IUINT32 dead_link, incr;
	//input->rcv_buf->rcv_queue->recv
	//send->snd_queue->snd_buf->output
	// 发包队列(待发送数据)
	struct IQUEUEHEAD snd_queue;
	// 收包队列(经过有序检查的数据,供上层获取)
	struct IQUEUEHEAD rcv_queue;
	// 发包缓存(以发送待确认数据)
	struct IQUEUEHEAD snd_buf;
	// 收包缓存(已收到数据)
	struct IQUEUEHEAD rcv_buf;
	// 回复ACK列表
	IUINT32 *acklist;
	// 回复ACK列表当前数量
	IUINT32 ackcount;
	// 回复ACK列表容量大小
	IUINT32 ackblock;
	// 用户数据
	void *user;
	// 内部buffer
	char *buffer;
	// 0--关闭快速重传,否则:faskack >= fastrecend时快速重传
	int fastresend;
	// nocwnd: 0--开启拥塞控制,1--关闭拥塞控制
	// stream: 0--流式模式,1--报文式模式
	int nocwnd,stream;
	// 日志掩码
	int logmask;
	// 输出回调(上层在这个回调中做自身逻辑,如用udp发送数据)
	int (*output)(const char *buf, int len, struct IKCPCB *kcp, void *user);
	// 日志回调
	void (*writelog)(const char *log, struct IKCPCB *kcp, void *user);
};


typedef struct IKCPCB ikcpcb;

#define IKCP_LOG_OUTPUT			1
#define IKCP_LOG_INPUT			2
#define IKCP_LOG_SEND			4
#define IKCP_LOG_RECV			8
#define IKCP_LOG_IN_DATA		16
#define IKCP_LOG_IN_ACK			32
#define IKCP_LOG_IN_PROBE		64
#define IKCP_LOG_IN_WINS		128
#define IKCP_LOG_OUT_DATA		25/
#define IKCP_LOG_OUT_ACK		512
#define IKCP_LOG_OUT_PROBE		1024
#define IKCP_LOG_OUT_WINS		2048

#ifdef __cplusplus
extern "C" {
#endif

//---------------------------------------------------------------------
// interface
//---------------------------------------------------------------------

// create a new kcp control object, 'conv' must equal in two endpoint
// from the same connection. 'user' will be passed to the output callback
// output callback can be setup like this: 'kcp->output = my_udp_output'
ikcpcb* ikcp_create(IUINT32 conv, void *user);

// release kcp control object
void ikcp_release(ikcpcb *kcp);

// set output callback, which will be invoked by kcp
void ikcp_setoutput(ikcpcb *kcp, int (*output)(const char *buf, int len, 
	ikcpcb *kcp, void *user));

// user/upper level recv: returns size, returns below zero for EAGAIN
int ikcp_recv(ikcpcb *kcp, char *buffer, int len);

// user/upper level send, returns below zero for error
int ikcp_send(ikcpcb *kcp, const char *buffer, int len);

// update state (call it repeatedly, every 10ms-100ms), or you can ask 
// ikcp_check when to call it again (without ikcp_input/_send calling).
// 'current' - current timestamp in millisec. 
void ikcp_update(ikcpcb *kcp, IUINT32 current);

// Determine when should you invoke ikcp_update:
// returns when you should invoke ikcp_update in millisec, if there 
// is no ikcp_input/_send calling. you can call ikcp_update in that
// time, instead of call update repeatly.
// Important to reduce unnacessary ikcp_update invoking. use it to 
// schedule ikcp_update (eg. implementing an epoll-like mechanism, 
// or optimize ikcp_update when handling massive kcp connections)
IUINT32 ikcp_check(const ikcpcb *kcp, IUINT32 current);

// when you received a low level packet (eg. UDP packet), call it
int ikcp_input(ikcpcb *kcp, const char *data, long size);

// flush pending data
void ikcp_flush(ikcpcb *kcp);

// check the size of next message in the recv queue
int ikcp_peeksize(const ikcpcb *kcp);

// change MTU size, default is 1400
int ikcp_setmtu(ikcpcb *kcp, int mtu);

// set maximum window size: sndwnd=32, rcvwnd=32 by default
int ikcp_wndsize(ikcpcb *kcp, int sndwnd, int rcvwnd);

// get how many packet is waiting to be sent
int ikcp_waitsnd(const ikcpcb *kcp);

// fastest: ikcp_nodelay(kcp, 1, 20, 2, 1)
// nodelay: 0:disable(default), 1:enable
// interval: internal update timer interval in millisec, default is 100ms 
// resend: 0:disable fast resend(default), 1:enable fast resend
// nc: 0:normal congestion control(default), 1:disable congestion control
int ikcp_nodelay(ikcpcb *kcp, int nodelay, int interval, int resend, int nc);


void ikcp_log(ikcpcb *kcp, int mask, const char *fmt, ...);

// setup allocator
void ikcp_allocator(void* (*new_malloc)(size_t), void (*new_free)(void*));

// read conv
IUINT32 ikcp_getconv(const void *ptr);


#ifdef __cplusplus
}
#endif

#endif



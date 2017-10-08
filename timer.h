#include <sys/timerfd.h>
#include <stdio.h>
#include "sta_info.h"
#include "list.h"

#define MX_EVNTS 10     // maxevents are returned by epoll_wait().
#define EPL_TOUT -1     // epoll_wait timeout

int eplfd = 0;  // epoll fd

struct param{
	int tfd;			// generate by "create_timerfd()"
        struct itimerspec its;		// timer expire config, it almost set by "add_timer()"
        struct sta_info sta_info;	// it use to remove iptable rule when timer expire
};

// stuct param common free memory function for "list"
void free_param(void *data)
{
	printf("%s: free stuct param, tfd=%d\n", __FUNCTION__, ((struct param*)data)->tfd);
	free((struct param *)data);
}

struct Node *t_head = NULL;	// timer param single linked list

/*
 *  main timer function
 *  you can use add_timer() to add timer 
 */
void *timer_main(void *arg);

/* 
 * Create a epoll file description and Create timer_main thread 
 * it always init before use timer function
 */
pthread_t epoll_init(void);


int create_timerfd(struct itimerspec *its,time_t interval);
int add_timer(struct sta_info *sta_info, time_t expire);
void destory_timer(struct param *pm);

#include <sys/epoll.h>
#include <sys/timerfd.h>
#include <time.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "timer.h"

//extern eplfd;

/*
 *  main timer function
 *  you can use add_timer() to add timer 
 */
void *timer_main(void *arg)
{
	struct epoll_event evnts[MX_EVNTS];
	int *eplfd = (int *)arg;
	struct param *pm = NULL;
	int n = -1;
	size_t i;
	while(1){
		n = epoll_wait(*eplfd, evnts, MX_EVNTS, EPL_TOUT);
		if(n == -1){
			perror("epoll_wait() error");
			break;
		}else if(n == 0){
			printf("time out %d sec expired\n",EPL_TOUT / 1000);
			break;
		}

		printf("%s: return of epoll_wait = %d\n", __FUNCTION__, n);
		for(i = 0; i < n;i++){
			// get timer parameter
			pm = (struct param *)(evnts[i].data.ptr);
			printf("tfd: %d\ninitial expiration: %ld\ninterval: %ld\n\n",
				pm->tfd,
				(long)(pm->its.it_value.tv_sec),
				(long)(pm->its.it_interval.tv_sec));
			/* Hint: Here should remove iptables rule. */

			destory_timer(pm);
		}
	}
	close(*eplfd);
	pthread_exit(NULL);
}

int create_timerfd(struct itimerspec *its,time_t interval)
{
	int tfd = timerfd_create(CLOCK_MONOTONIC,TFD_NONBLOCK);
	if(tfd < 0){
		perror("timerfd_create() error");
		return -2;
	}
	struct timespec nw;
	if(clock_gettime(CLOCK_MONOTONIC,&nw) != 0){
		perror("clock_gettime() error");
		return -1;
	}
	its->it_value.tv_sec = nw.tv_sec + interval;
	its->it_value.tv_nsec = 0;
	//its->it_interval.tv_sec = 3;		//periodic timer for interval
	its->it_interval.tv_sec = 0;		//once timer and expire on "it_value"
	its->it_interval.tv_nsec = 0;
	return tfd;
}

/* 
 * Create a epoll file description and Create timer_main thread 
 * it always init before use timer function
 */
pthread_t epoll_init(void)
{
	pthread_t pid;

	eplfd = epoll_create1(EPOLL_CLOEXEC);	// close eplfd when call exec()
	if(eplfd < 0){
		perror("epoll_create1() error");
		return -1;
	}

	// Create main timer thread
	if(pthread_create(&pid, NULL, timer_main, (void *)&eplfd) != 0){
		perror("pthread_create() error");
		return -1;
	}

	return pid;
}

/* there is variable life cycle issue */
int add_timer(struct sta_info *sta_info, time_t expire)
{
	int tfd = 0;	// timer fd;
	struct itimerspec its;
	struct epoll_event ev;	// epoll moniter event
        struct param pm;	// timer parameter

        // Create a timer fd
        tfd = create_timerfd(&its, expire);
        if(tfd < 0){
                perror("create_timerfd() error");
                return -1;
        }

        // assign param for station
	pm.its = its;
	pm.tfd = tfd;
	if(sta_info != NULL)
		memcpy(&(pm.sta_info), sta_info, sizeof(struct sta_info));

	if(timerfd_settime(pm.tfd, TFD_TIMER_ABSTIME, &(pm.its), NULL) != 0){
		perror("timerfd_settime() error");
		return -1;
	}

	// add to timer linked list, you must free the memory when remove from list
	add_node(&t_head, &pm, sizeof(struct param), free_param);

	// set epoll monitor condition
	ev.events = EPOLLIN;    // Level Trigger ==> when timer fd status is readable
	ev.data.ptr = t_head->data;
	if(epoll_ctl(eplfd, EPOLL_CTL_ADD, pm.tfd, &ev) != 0){
		perror("epoll_ctl() error");
		return -1;
	}

	return 0;
}

/* del epoll and destory corresponding linked list */
void destory_timer(struct param *pm)
{
	// remove timer from epoll
	if(epoll_ctl( eplfd, EPOLL_CTL_DEL, pm->tfd, NULL) != 0){
		perror("epoll_ctl(DEL) error in thread");
	}

	// close timer fd
	if(pm->tfd)
		close(pm->tfd);

	del_node(&t_head, (void *)pm, sizeof(struct param));
}

#if 0
int main(void){

	pthread_t pid = epoll_init();
	if(pid <= 0){
		perror("epoll_init() error");
		return -1;
	}

	add_timer(NULL, 3);
	add_timer(NULL, 1);
	add_timer(NULL, 5);
	add_timer(NULL, 9);

	// Wait main timer thread join
	if(pthread_join(pid,NULL) != 0){
		perror("pthread_join() error");
		return -1;
	}

	return 0;
}
#endif

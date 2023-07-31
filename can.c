#include <stdlib.h>
#include <unistd.h>
#include <net/if.h>
#include <pthread.h>

#include "can.h"
#include "tcp.h"
int main(void)
{
	int sockfd = -1;
	sockfd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
	can_init(sockfd);
	//创建can发送线程
	pthread_t can_send_thread;
	pthread_create(&can_send_thread, NULL, can_send, (void *)&sockfd);
	pthread_join(can_send_thread, NULL);
	//创建can接收线程
	pthread_t can_recv_thread;
	pthread_create(&can_recv_thread, NULL, can_recv, (void *)&sockfd);
	pthread_join(can_recv_thread, NULL);
	//初始化tcp
	int socktcp = tcp_init();
	//创建tcp发送线程
	pthread_t tcp_send_thread;
	pthread_create(&tcp_send_thread, NULL, tcp_send, (void *)&socktcp);
	pthread_join(tcp_send_thread, NULL);
	//创建tcp接收线程
	pthread_t tcp_recv_thread;
	pthread_create(&tcp_recv_thread, NULL, tcp_recv, (void *)&socktcp);
	pthread_join(tcp_recv_thread, NULL);
	while(1)
	{
		sleep(1);
	}
	/* 关闭套接字 */
	close(sockfd);
	exit(EXIT_SUCCESS);
}

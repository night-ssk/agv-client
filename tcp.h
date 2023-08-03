#include <pthread.h>
extern pthread_mutex_t tcp_mutex;
extern int32_t spd_buf[2];
int tcp_init();
void* tcp_send(void* sockfd);
void* tcp_recv(void* sockfd);
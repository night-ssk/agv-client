#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#define BACKLOG  1

int client_id = 1;
pthread_mutex_t client_mutex;

int tcp_init()
{
    int socktcp;
    int ret;
    struct sockaddr_in server_addr;

    socktcp = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(1234);
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
	memset(server_addr.sin_zero, 0, 8);

    ret = bind(socktcp, (const struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(socktcp, BACKLOG);

    return socktcp;
}
void tcp_connect(int socktcp)
{
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(struct sockaddr);
    pthread_mutex_lock(&client_mutex);
    client_id = accept(socktcp, (struct sockaddr *)&client_addr, &addr_len);
    while (client_id < 0) 
    {
        client_id = accept(socktcp, (struct sockaddr *)&client_addr, &addr_len);
    }
    pthread_mutex_unlock(&client_mutex);
    printf("client connect success\n");
    printf("Get connect from client %s\n", inet_ntoa(client_addr.sin_addr));
}
void* tcp_send(void* socktcp)
{
    while (1)
    {
        unsigned char ucRecvBuf[1000];
        pthread_mutex_lock(&client_mutex);
        int iRecvLen = recv((intptr_t)client_id, ucRecvBuf, 1000, 0);
        pthread_mutex_unlock(&client_mutex);
        if (iRecvLen > 0)
        {
            printf("recv data from client: %s\n", ucRecvBuf);
        }
        else 
        {
            printf("client disconnect\n");
            tcp_connect((intptr_t)socktcp);//重新连接
        }
    }
}
void* tcp_recv(void* socktcp)
{
    while (1)
    {
        char sendbuf[1000];
        pthread_mutex_lock(&client_mutex);
        int iSendLen = send(client_id, sendbuf, strlen(sendbuf), 0);
        pthread_mutex_unlock(&client_mutex);
        if (iSendLen > 0)
        {
            printf("send data to client: %s\n", sendbuf);
        }
        else 
        {
            printf("client disconnect\n");
            tcp_connect((intptr_t)socktcp);//重新连接
        }
        usleep(10000);      //10ms
    }
}
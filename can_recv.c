#include "can.h"
#include <stdio.h>
#include <unistd.h>
void* can_recv(void* sockfd)
{
    while (1)
    {
        struct can_frame recv_frame;
        /* 读取电机反馈 */
        // read(*(int*)sockfd, &recv_frame, sizeof(struct can_frame));
        // for (int i = 0; i < recv_frame.can_dlc; i++)
        //     printf("%02x ", recv_frame.data[i]);
        // printf("\n");
        // int32_t pose = 0,spd = 0;
        // switch (recv_frame.can_id) 
        // {
        //     case 0x181:
        //     {
        //         pose = recv_frame.data[0] + (recv_frame.data[1] << 8) + (recv_frame.data[2] << 16) + (recv_frame.data[3] << 24);
        //         spd = recv_frame.data[4] + (recv_frame.data[5] << 8) + (recv_frame.data[6] << 16) + (recv_frame.data[7] << 24);
        //         printf("pose = %d   spd = %d \n",pose,spd);
        //         break;
        //     }
        // }
        usleep(10000);		//10ms
    }
}
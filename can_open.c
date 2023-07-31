#include "can_open.h"
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
//SDO控制
void sdo_send(int fd, SDO_PARAM *sdo_param, uint8_t* data)
{
    struct can_frame send_frame = {0};
    int byte_id = 47 - (sdo_param->byte_size-1)*4;

    send_frame.data[0] = byte_id; //发送字节大小对应ID
    send_frame.data[1] = (sdo_param->can_address & 0x00FF0000) >> 16;
    send_frame.data[2] = (sdo_param->can_address & 0xFF000000) >> 24;
    send_frame.data[3] = (sdo_param->can_address & 0x0000FF00) >> 8;

    for (int i = 0; i < sdo_param->byte_size; i++)
    {
        send_frame.data[4 + i] = data[i];
    }

    send_frame.can_dlc = 4 + sdo_param->byte_size;
    send_frame.can_id = 0x600 + sdo_param->node_id;

    write(fd, &send_frame,sizeof(send_frame));
    //打印发送的数据
    printf("%x ",send_frame.can_id);
    for (int i = 0;i < (4 + sdo_param->byte_size); i++)
    {
    	printf("%x ",send_frame.data[i]);
    }
    printf("\n");
}

//PDO控制
void pdo_send_acc(int fd, ACC_DEC* pdo_param)
{
    struct can_frame send_frame = {0};
    send_frame.data[0] = (pdo_param->acc&(0x000000FF));
    send_frame.data[1] = (pdo_param->acc&(0x0000FF00))>>8;
    send_frame.data[2] = (pdo_param->acc&(0x00FF0000))>>16;
    send_frame.data[3] = (pdo_param->acc&(0xFF000000))>>24;
    send_frame.data[4] = (pdo_param->dec&(0x000000FF));
    send_frame.data[5] = (pdo_param->dec&(0x0000FF00))>>8;
    send_frame.data[6] = (pdo_param->dec&(0x00FF0000))>>16;
    send_frame.data[7] = (pdo_param->dec&(0xFF000000))>>24;
    send_frame.can_dlc = 8;
    send_frame.can_id = pdo_param->pod_id + pdo_param->node_id;
    write(fd, &send_frame, sizeof(send_frame));
}
void pdo_send_spd(int fd, SPD* pdo_param)
{
    struct can_frame send_frame = {0};
    send_frame.data[0] = (pdo_param->spd&(0x000000FF));
    send_frame.data[1] = (pdo_param->spd&(0x0000FF00))>>8;
    send_frame.data[2] = (pdo_param->spd&(0x00FF0000))>>16;
    send_frame.data[3] = (pdo_param->spd&(0xFF000000))>>24;
    send_frame.can_dlc = 4;
    send_frame.can_id = pdo_param->pod_id + pdo_param->node_id;
    write(fd, &send_frame, sizeof(send_frame));
    printf("---------------------------- \n");
    printf("%x ",send_frame.can_id);
    for (int i = 0;i < 4; i++)
    {
    	printf("%x ",send_frame.data[i]);
    }
    printf("\n");
}
//can发送
void can_send(int fd, uint32_t can_id, uint8_t can_dlc, uint8_t* data)
{
    struct can_frame send_frame = {0};
    send_frame.can_id = can_id;
    send_frame.can_dlc = can_dlc;
    for (int i = 0; i < can_dlc; i++)
    {
        send_frame.data[i] = data[i];
    }
    write(fd, &send_frame, sizeof(send_frame));
    printf("---------------------------- \n");
    printf("%x ",send_frame.can_id);
    for (int i = 0;i < 8; i++)
    {
    	printf("%x ",send_frame.data[i]);
    }
    printf("\n");
}
//PDO设置

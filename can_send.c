#include "can.h"
#include <unistd.h>
void* can_send(void* sockfd)
{
    ACC_DEC acc_dec_pdo_left;
    ACC_DEC acc_dec_pdo_right;
    SPD spd_pdo_left;
    SPD spd_pdo_right;
    motor_init(*(int*)sockfd, 0x01, &acc_dec_pdo_left, &spd_pdo_left, 0x3F, 60000, 0x03);
    motor_init(*(int*)sockfd, 0x02, &acc_dec_pdo_right, &spd_pdo_right, 0x3F, 60000, 0x03);
    while (1) 
    {
		/* 动态设置电机加减速度 */
		// pdo_send_acc(*sockfd, &acc_dec_pdo_left);
		// pdo_send_acc(*sockfd, &acc_dec_pdo_right);
		/* 设置电机速度 */
		spd_pdo_left.spd = 10000;
		spd_pdo_right.spd = 0;
		// pdo_send_spd(*(int*)sockfd, &spd_pdo_left);
		// pdo_send_spd(*(int*)sockfd, &spd_pdo_right);
        //usleep(10000);        //10ms
    }
    return 0;
}
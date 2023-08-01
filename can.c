#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>

#include "can_open.h"
int main(void)
{
	struct ifreq ifr = {0};
	struct sockaddr_can can_addr = {0};
	struct can_frame send_frame = {0};
	struct can_frame recv_frame = {0};
	struct can_filter rfilter[3];
	int sockfd = -1;
	int ret;

	/* can初始化 */
	sockfd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
	strcpy(ifr.ifr_name, "can0");
	ioctl(sockfd, SIOCGIFINDEX, &ifr);
	can_addr.can_family = AF_CAN;
	can_addr.can_ifindex = ifr.ifr_ifindex;
	ret = bind(sockfd, (struct sockaddr *)&can_addr, sizeof(can_addr));
	//setsockopt(sockfd, SOL_CAN_RAW, CAN_RAW_FILTER, NULL, 0);

	/* sdo参数初始化 */
	//控制字
	SDO_PARAM control_word_sdo_left = {control_word_d, left_node, two_byte};
	SDO_PARAM control_word_sdo_right = {control_word_d, right_node, two_byte};
	//工作模式
	SDO_PARAM work_mode_sdo_left = {work_mode_d, left_node, one_byte};
	SDO_PARAM work_mode_sdo_right = {work_mode_d, right_node, one_byte};
	//加速度设置
	SDO_PARAM acc_sdo_left = {trap_acc_d, left_node, four_byte};
	SDO_PARAM acc_sdo_right = {trap_acc_d, right_node, four_byte};
	//减速度设置
	SDO_PARAM dec_sdo_left = {trap_dec_d, left_node, four_byte};
	SDO_PARAM dec_sdo_right = {trap_dec_d, right_node, four_byte};
	
	/* pdo参数初始化 */
	//加减速设置
	ACC_DEC acc_dec_pdo_left = {acc_dec_d, left_node, 0, 0};//{pod_id, node_id, acc, dec}
	ACC_DEC acc_dec_pdo_right = {acc_dec_d, right_node, 0, 0};//{pod_id, node_id, acc, dec}
	//速度设置
	SPD spd_pdo_left = {spd_d, left_node, 0};//{pod_id, node_id, spd, 0x00}
	SPD spd_pdo_right = {spd_d, right_node, 0};//{pod_id, node_id, spd, 0x00}

	/* 初始化电机 */
	//uint16_t control_word[4] = {0x0F, 0x1F, 0x2F, 0x3F}; //电机上电 -> 原点定位 -> 准备运行 -> 进入绝对定位
	uint16_t control_word[4] = {0x0F, 0x1F, 0x4F, 0x5F}; //电机上电 -> 原点定位 -> 准备运行 -> 进入相对定位
	for (int i = 0; i < sizeof(control_word)/sizeof(*control_word); i++)
	{
		int get_control_word = 0;
		sdo_send(sockfd, &control_word_sdo_left, (uint8_t *)&control_word[i]);
		 while(get_control_word != control_word[i])
		 {
			read(sockfd, &recv_frame, sizeof(struct can_frame));
			get_control_word = recv_frame.data[4] + (recv_frame.data[5] << 8);
			usleep(500000);
			printf("get_control_word = %d\ncontrol_word[i] = %d \n \n",get_control_word,control_word[i]);
		 }
	}
	/* 设置电机模式 */
	int8_t mode = 3;
	sdo_send(sockfd, &work_mode_sdo_left,(uint8_t *)&mode);
	/* 设置电机梯形加速度 */
	int32_t acc = 60000;
	int32_t dec = 60000;
	sdo_send(sockfd, &acc_sdo_left, (uint8_t *)&acc);
	//sdo_send(sockfd, &acc_sdo_right, (uint8_t *)&acc);
	 sdo_send(sockfd, &dec_sdo_left, (uint8_t *)&dec);
	// sdo_send(sockfd, &dec_sdo_right, (uint8_t *)&dec);
	/*设置pdo地址*/
	SDO_PARAM pdo_num = {0x16010001, left_node, one_byte};
	SDO_PARAM pdo_1 = {0x16010120, left_node, four_byte};
	SDO_PARAM pdo_node = {0x14010120, left_node, four_byte};
	SDO_PARAM pdo_type = {0x14010210, left_node, one_byte};
	SDO_PARAM pdo_times = {0x14010310, left_node, two_byte};
	uint8_t num = 1;
	uint32_t addr = 0x60FF0020;
	uint32_t node = 0x301;
	uint8_t type = 254;
	uint16_t times = 0;
	sdo_send(sockfd, &pdo_num, (uint8_t *)&num);
	sdo_send(sockfd, &pdo_1, (uint8_t *)&addr);
	sdo_send(sockfd, &pdo_node, (uint8_t *)&node);
	sdo_send(sockfd, &pdo_type, (uint8_t *)&type);
	sdo_send(sockfd, &pdo_times, (uint8_t *)&times);

	/*设置pdo地址*/
	/* 设置PDO */
	/* 开启节点 */
	uint8_t start_node[2] = {0x01,0x01};
	can_send(sockfd, 0x000, 2, start_node);
	printf("ok \n");
	int run_times = 0;
	spd_pdo_left.spd = 25000;
	while(1)
	{
		// /* 动态设置电机加减速度 */
		// pdo_send_acc(sockfd, &acc_dec_pdo_left);
		// pdo_send_acc(sockfd, &acc_dec_pdo_right);
		/* 设置电机速度 */
		// if(run_times++ > 500)
		// {
		// 	run_times = 0;
		// 	spd_pdo_left.spd = -spd_pdo_left.spd;
		// }
		printf("spd = %d",spd_pdo_left.spd);
		//spd_pdo_right.spd = 0;
		pdo_send_spd(sockfd, &spd_pdo_left);
		//pdo_send_spd(sockfd, &spd_pdo_right);
		/* 读取电机反馈 */
		read(sockfd, &recv_frame, sizeof(struct can_frame));
		for (int i = 0; i < recv_frame.can_dlc; i++)
			printf("%02x ", recv_frame.data[i]);
		printf("\n");
		// int32_t pose = 0,spd = 0;
		// switch (recv_frame.can_id) {
		// 	case 0x181:
		// 	{
		// 		pose = recv_frame.data[0] + (recv_frame.data[1] << 8) + (recv_frame.data[2] << 16) + (recv_frame.data[3] << 24);
		// 		spd = recv_frame.data[4] + (recv_frame.data[5] << 8) + (recv_frame.data[6] << 16) + (recv_frame.data[7] << 24);
		// 		printf("pose = %d   spd = %d \n",pose,spd);
		// 		break;
		// 	}
		// }
		usleep(10);		//10ms
	}
	/* 关闭套接字 */
	close(sockfd);
	exit(EXIT_SUCCESS);
}

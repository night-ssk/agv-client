#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include "can.h"
void can_init(int sockfd)
{
	int ret = 0;

	struct ifreq ifr = {0};
	struct sockaddr_can can_addr = {0};
	//can初始化
	strcpy(ifr.ifr_name, "can0");
	ioctl(sockfd, SIOCGIFINDEX, &ifr);
	can_addr.can_family = AF_CAN;
	can_addr.can_ifindex = ifr.ifr_ifindex;
	ret = bind(sockfd, (struct sockaddr *)&can_addr, sizeof(can_addr));
	printf("can_init ok \n");
}
void pdo_init(int sockfd, uint8_t node_id)
{
	/*设置pdo地址*/
	SDO_PARAM spd_rpdo_addr[5] = 
		{{0x16010001, node_id, one_byte},  //pdo数据数量
		{0x16010120, node_id, four_byte}, //pdo2映射对象1
		{0x14010120, node_id, four_byte}, //pdo2站号
		{0x14010210, node_id, one_byte},  //pdo2类型
		{0x14010310, node_id, two_byte}}; //pdo2禁止时间
	uint8_t num = 1;
	uint32_t addr = 0x60FF0020;
	uint32_t node = 0x300 + node_id;
	uint8_t type = 254;
	uint16_t times = 0;
	sdo_send(sockfd, &spd_rpdo_addr[0], (uint8_t *)&num);
	sdo_send(sockfd, &spd_rpdo_addr[1], (uint8_t *)&addr);
	sdo_send(sockfd, &spd_rpdo_addr[2], (uint8_t *)&node);
	sdo_send(sockfd, &spd_rpdo_addr[3], (uint8_t *)&type);
	sdo_send(sockfd, &spd_rpdo_addr[4], (uint8_t *)&times);
}
void motor_init(int sockfd, uint8_t node_id, ACC_DEC* acc_dec_pdo, SPD *spd_pdo, uint16_t position_mode, int32_t acc, int8_t mode)
{
	/* sdo参数初始化 */
	SDO_PARAM control_word_sdo = {control_word_d, node_id, two_byte};	//控制字
	SDO_PARAM work_mode_sdo = {work_mode_d, node_id, one_byte};	//工作模式
	SDO_PARAM acc_sdo = {trap_acc_d, node_id, four_byte};	//加速度设置
	SDO_PARAM dec_sdo = {trap_dec_d, node_id, four_byte};	//减速度设置

	/* pdo参数初始化 */
	acc_dec_pdo -> pod_id = acc_dec_d;
	acc_dec_pdo -> node_id = node_id;
	acc_dec_pdo -> acc = 0;
	acc_dec_pdo -> dec = 0;
	spd_pdo -> pod_id = spd_d;
	spd_pdo -> node_id = node_id;
	spd_pdo -> spd = 0;
	/* 复位节点 */
	uint8_t reset_node[2] = {0x81,node_id};
	bit_send(sockfd, 0x000, 2, reset_node);
	uint8_t init_node[2] = {0x80,node_id};
	bit_send(sockfd, 0x000, 2, init_node);

	/* 初始化电机 */
	uint16_t control_word[4] = {0x0F, 0x1F, 0x4F, position_mode}; //0x5F 进入相对定位,0x3F 进入绝对定位
	for (int i = 0; i < sizeof(control_word)/sizeof(*control_word); i++)
	{
		int get_control_word = 0;
		sdo_send(sockfd, &control_word_sdo, (uint8_t *)&control_word[i]);
		 while(get_control_word != control_word[i])
		 {
			struct can_frame recv_frame;
			read(sockfd, &recv_frame, sizeof(struct can_frame));
			get_control_word = recv_frame.data[4] + (recv_frame.data[5] << 8);
			printf("get_control_word = %d\n control_word[i] = %d \n \n",get_control_word,control_word[i]);
		  }
	}
	/* 设置电机模式 */
	sdo_send(sockfd, &work_mode_sdo,(uint8_t *)&mode);
	/* 设置电机梯形加速度 */
	sdo_send(sockfd, &acc_sdo, (uint8_t *)&acc);
	sdo_send(sockfd, &dec_sdo, (uint8_t *)&acc);
	pdo_init(sockfd, node_id);
	/* 开启节点 */
	uint8_t start_node[2] = {0x01,node_id};
	bit_send(sockfd, 0x000, 2, start_node);
	printf("motor_init ok \n");
}

#include "can_open.h"
extern void* can_send(void* sockfd);
void* can_recv(void* sockfd);
extern void can_init(int sockfd);
extern void motor_init(int sockfd, uint8_t node_id, ACC_DEC* acc_dec_pdo, SPD *spd_pdo, uint16_t position_mode, int32_t acc, int8_t mode);
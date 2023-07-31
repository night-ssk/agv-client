
#include "can_open.h"

extern void can_init();
extern void motor_init(int sockfd, uint8_t node_id, ACC_DEC* acc_dec_pdo, SPD *spd_pdo, uint16_t position, int32_t acc, int8_t mode);
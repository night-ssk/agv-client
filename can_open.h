#include <stdint.h>
#include <linux/can.h>
enum object_dictionary //对象字典
{
    //模式及控制
    control_word_d = 0x60400010, //控制字
    status_word_d = 0x60410010, //状态字
    work_mode_d = 0x60600008, //工作模式
    mode_of_position_control_d = 0x20200F, //绝对｜相对位置模式选择

    //测量数据
    position_d = 0x60630020, //实际位置
    velocity_d = 0x606C0020, //实际速度
    current_d = 0x60780010, //实际电流
    input_status_d = 0x60FD0020,
    //目标对象(0x607A)
    direction_d = 0x607E0008, //电机运动方向
    target_position_d = 0x607A0020, //目标位置
    trapezoidal_speed_d = 0x60810020, //梯形速度
    target_velocity_d = 0x60FF0020, //目标速度
    max_velocity_d = 0x60800010, //最大速度限制
    trap_acc_d = 0x60830020, //梯形加速度
    trap_dec_d = 0x60840020, //梯形减速度
    target_torque_d = 0x60710010, //目标力矩
    target_current_d = 0x60F60810, //目标电流
    max_current_d = 0x60730010, //最大电流限制
};
enum pdo_id
{
    acc_dec_d = 0x200, //加减速控制
    spd_d = 0x300, //速度控制
};
enum sdo_id
{
    one_byte = 1,//0x2F, //1字节 47 - (x-1)*4
    two_byte = 2,//0x2B, //2字节
    four_byte = 4//0x23 //4字节
};
enum NODE_ID
{
    left_node = 0x01,
    right_node = 0x02
};
//SDO参数
typedef struct
{
    uint32_t can_address;
    uint8_t node_id;
    uint8_t byte_size;
}SDO_PARAM;

//PDO参数
typedef struct 
{
    uint32_t pod_id; 
    uint8_t node_id;
    uint32_t acc;
    uint32_t dec;
}ACC_DEC;
typedef struct 
{
    uint32_t pod_id;
    uint8_t node_id;
    int32_t spd;
}SPD;
extern void sdo_send(int fd, SDO_PARAM *sdo_param, uint8_t* data);
extern void can_send(int fd, uint32_t can_id, uint8_t can_dlc, uint8_t* data);
extern void pdo_send_acc(int fd, ACC_DEC* pdo_param);
extern void pdo_send_spd(int fd, SPD* pdo_param);
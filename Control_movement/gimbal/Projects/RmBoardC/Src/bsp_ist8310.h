#ifndef BSP_IST8310_HPP
#define BSP_IST8310_HPP

#include "stm32f4xx.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define IST8310_IIC_ADDRESS (0x0E<<1)
#define IST8310_WHO_AM_I 0x00       //ist8310 who am I �Ĵ���
#define IST8310_WHO_AM_I_VALUE 0x10 //�豸 ID

void bsp_ist8310_init(uint8_t* p_data);
void bsp_ist8310_timer(void);
void bsp_ist8310_RST_H(void);
void bsp_ist8310_RST_L(void);
uint8_t bsp_get_update_flag(void);
void bsp_clear_update_flag(void);
void bsp_start_read_data(void);
uint8_t bsp_ist8310_read_single_reg(uint8_t);
void bsp_ist8310_read_single_reg_timer_IT(uint8_t);
void bsp_ist8310_write_single_reg(uint8_t,uint8_t);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

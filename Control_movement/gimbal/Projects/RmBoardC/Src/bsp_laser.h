#ifndef BSP_LASER_H
#define BSP_LASER_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "stm32f4xx.h"

void bsp_laser_init(void);
void bsp_laser_on(void);
void bsp_laser_off(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

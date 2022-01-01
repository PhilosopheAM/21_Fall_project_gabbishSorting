#ifndef BSP_BMI088_HPP
#define BSP_BMI088_HPP

#include "stm32f4xx.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void bsp_bmi088_init(void);
void bsp_bmi088_clk_init(uint16_t arr, uint16_t pcs);
void bsp_bmi088_start_clk(void);
void bsp_bmi088_stop_clk(void);
void bsp_select_bmi088_acc(void);
void bsp_deselect_bmi088_acc(void);
void bsp_select_bmi088_gyro(void);
void bsp_deselect_bmi088_gyro(void);
uint32_t bsp_get_bmi088_clk(void);
FlagStatus bsp_spi_check_timeout(uint32_t timeout_us);
FlagStatus bsp_spi_send(SPI_TypeDef *SPIx, uint8_t *pData, uint32_t len, uint32_t timeout_us);
FlagStatus bsp_spi_receive(SPI_TypeDef *SPIx, uint8_t *pData, uint32_t len, uint32_t timeout_us);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

#include "bsp_bmi088.h"

void bsp_bmi088_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    SPI_InitTypeDef SPI_InitStruct;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB |
        RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_3;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_SetBits(GPIOC, GPIO_Pin_4);
	GPIO_SetBits(GPIOC, GPIO_Pin_5);

    GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_SPI1);

    SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
    SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
    SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_Init(SPI1,&SPI_InitStruct);
	
	//SPI_I2S_ITConfig(SPI1,SPI_I2S_IT_RXNE,ENABLE);
	
	SPI_Cmd(SPI1,ENABLE);
	
	// NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
	// NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	// NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	// NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	// NVIC_Init(&NVIC_InitStructure);
}

void bsp_bmi088_clk_init(uint16_t arr, uint16_t pcs)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);

    TIM_TimeBaseInitTypeDef TIM_InitStruct;

    TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_InitStruct.TIM_Period = arr - 1;
    TIM_InitStruct.TIM_Prescaler = pcs - 1;

    TIM_TimeBaseInit(TIM10, &TIM_InitStruct);
}

void bsp_select_bmi088_acc(void)
{
    GPIO_ResetBits(GPIOA, GPIO_Pin_4);
}

void bsp_deselect_bmi088_acc(void)
{
    GPIO_SetBits(GPIOA, GPIO_Pin_4);
}

void bsp_select_bmi088_gyro(void)
{
    GPIO_ResetBits(GPIOB, GPIO_Pin_0);
}

void bsp_deselect_bmi088_gyro(void)
{
    GPIO_SetBits(GPIOB, GPIO_Pin_0);
}

void bsp_bmi088_start_clk(void)
{
    TIM10->CNT = 0;
    TIM_Cmd(TIM10, ENABLE);
}

void bsp_bmi088_stop_clk(void)
{
    TIM_Cmd(TIM10, DISABLE);
    TIM10->CNT = 0;
}

uint32_t bsp_get_bmi088_clk(void)
{
    return TIM10->CNT;
}

FlagStatus bsp_spi_check_timeout(uint32_t timeout_us)
{
     if(bsp_get_bmi088_clk() > timeout_us)
    {
        return SET;
    }
    return RESET;
}

FlagStatus bsp_spi_send(SPI_TypeDef *SPIx, uint8_t *pData, uint32_t len, uint32_t timeout_us)
{
    bsp_bmi088_start_clk();

    for (int i = 0; i < len; ++i)
    {
        while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET)
        {
            if(bsp_spi_check_timeout(timeout_us) == SET)
            {
                bsp_bmi088_stop_clk();
                return SET;
            }
        }

        SPIx->DR = pData[i];
    }
    
    while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY) == SET)
    {
        if(bsp_spi_check_timeout(timeout_us) == SET)
        {
            bsp_bmi088_stop_clk();
            return SET;
        }
    }

    int16_t tmpreg = SPIx->DR;
    bsp_bmi088_stop_clk();

    return RESET;
}

FlagStatus bsp_spi_receive(SPI_TypeDef *SPIx, uint8_t *pData, uint32_t len, uint32_t timeout_us)
{
    bsp_bmi088_start_clk();
    for(int i = 0;i < len; ++i)
    {
        while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET)
        {
            if(bsp_spi_check_timeout(timeout_us) == SET)
            {
                bsp_bmi088_stop_clk();
                return SET;
            }
        }
        
		for (uint32_t i = 0; i < 1680; i++)
		{}
		
        SPIx->DR = 0x55;

       while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET)
       {
           if(bsp_spi_check_timeout(timeout_us) == SET)
           {
               bsp_bmi088_stop_clk();
               return SET;
           }
       }

       pData[i] = SPIx->DR;
    }

    while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY) == SET)
    {
        if(bsp_spi_check_timeout(timeout_us) == SET)
        {
            bsp_bmi088_stop_clk();
            return SET;
        }
    }

    bsp_bmi088_stop_clk();

    return RESET;
}

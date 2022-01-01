#include "bsp_ist8310.h"

static uint32_t tim9_time_count = 0;
static uint8_t reg_read_count = 0;
static uint8_t* ist8310_data_buffer = 0;
static uint8_t receive_flag = 0;
static uint8_t updated_flag = 0;

void bsp_ist8310_init(uint8_t* p_data)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    I2C_InitTypeDef I2C_InitStruct;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOG |
        RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C3, ENABLE);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOG, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_I2C3);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_I2C3);

	I2C_SoftwareResetCmd(I2C3,ENABLE);
	I2C_SoftwareResetCmd(I2C3,DISABLE);

    I2C_InitStruct.I2C_ClockSpeed = 100000;
    I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStruct.I2C_OwnAddress1 = 0;
    I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
    I2C_Init(I2C3,&I2C_InitStruct);

    I2C_Cmd(I2C3,ENABLE);
    ist8310_data_buffer = p_data;
}

void bsp_ist8310_timer(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);

    TIM_TimeBaseInitTypeDef TIM_InitStruct;

    TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_InitStruct.TIM_Period = 10 - 1;
    TIM_InitStruct.TIM_Prescaler = 168 - 1;
    TIM_TimeBaseInit(TIM9, &TIM_InitStruct);

    TIM_ITConfig(TIM9,TIM_IT_Update,ENABLE);
	
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel=TIM1_BRK_TIM9_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0; //中断抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;    //子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void bsp_ist8310_RST_L(void)
{
    GPIO_ResetBits(GPIOG, GPIO_Pin_6);
}

void bsp_ist8310_RST_H(void)
{
    GPIO_SetBits(GPIOG, GPIO_Pin_6);
}

void TIM1_BRK_TIM9_IRQHandler()
{
    if(TIM_GetITStatus(TIM9,TIM_IT_Update)!=RESET)
    {
        if(updated_flag==0)
        {
            tim9_time_count++; //10us
        }
        TIM_ClearITPendingBit(TIM9,TIM_IT_Update);
    }

    if(updated_flag==0&&receive_flag==1&&tim9_time_count==19)
    {
        ist8310_data_buffer[reg_read_count] = I2C3->DR;
        reg_read_count++;

        if(reg_read_count==6)
        {
            updated_flag = 1;
            tim9_time_count = 0;
			reg_read_count = 0;
        }

        receive_flag = 0;
    }

    if(tim9_time_count==20)
    {
        bsp_ist8310_read_single_reg_timer_IT(0x03+reg_read_count);
        tim9_time_count = 0;
        receive_flag = 1;
    }
}

uint8_t bsp_get_update_flag(void)
{
    return updated_flag;
}

void bsp_clear_update_flag(void)
{
    updated_flag = 0;
}

void bsp_start_read_data(void)
{
    TIM_Cmd(TIM9,ENABLE);
}

uint8_t bsp_ist8310_read_single_reg(uint8_t address)
{
    while(I2C_GetFlagStatus(I2C3,I2C_FLAG_BUSY));

	I2C_AcknowledgeConfig(I2C3,ENABLE);
    I2C_GenerateSTART(I2C3,ENABLE);
    while (!I2C_CheckEvent(I2C3,I2C_EVENT_MASTER_MODE_SELECT));

	I2C_Send7bitAddress(I2C3,IST8310_IIC_ADDRESS,I2C_Direction_Transmitter);
	while (!I2C_CheckEvent(I2C3,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_ClearFlag(I2C3,I2C_FLAG_ADDR);
	
    I2C_SendData(I2C3,address);
	
	while(!I2C_GetFlagStatus(I2C3,I2C_FLAG_TXE));
	
    //while (!I2C_CheckEvent(I2C3,I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    I2C_GenerateSTART(I2C3,ENABLE);
    while (!I2C_CheckEvent(I2C3,I2C_EVENT_MASTER_MODE_SELECT));

    I2C_Send7bitAddress(I2C3,IST8310_IIC_ADDRESS,I2C_Direction_Receiver);
    while (!I2C_CheckEvent(I2C3,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

	I2C_AcknowledgeConfig(I2C3,DISABLE);
	I2C_ClearFlag(I2C3,I2C_FLAG_ADDR);
	I2C_GenerateSTOP(I2C3,ENABLE);
	
	for(int i=0;i<3500;i++); //fuck delay, 3000<i
	
	uint8_t data = 0;
	
	data = I2C3->DR;
	
	return data;
}

void bsp_ist8310_read_single_reg_timer_IT(uint8_t address)
{
    while(I2C_GetFlagStatus(I2C3,I2C_FLAG_BUSY));

	I2C_AcknowledgeConfig(I2C3,ENABLE);
    I2C_GenerateSTART(I2C3,ENABLE);
    while (!I2C_CheckEvent(I2C3,I2C_EVENT_MASTER_MODE_SELECT));

	I2C_Send7bitAddress(I2C3,IST8310_IIC_ADDRESS,I2C_Direction_Transmitter);
	while (!I2C_CheckEvent(I2C3,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_ClearFlag(I2C3,I2C_FLAG_ADDR);
	
    I2C_SendData(I2C3,address);
	
	while(!I2C_GetFlagStatus(I2C3,I2C_FLAG_TXE));
	
    //while (!I2C_CheckEvent(I2C3,I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    I2C_GenerateSTART(I2C3,ENABLE);
    while (!I2C_CheckEvent(I2C3,I2C_EVENT_MASTER_MODE_SELECT));

    I2C_Send7bitAddress(I2C3,IST8310_IIC_ADDRESS,I2C_Direction_Receiver);
    while (!I2C_CheckEvent(I2C3,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

	I2C_AcknowledgeConfig(I2C3,DISABLE);
	I2C_ClearFlag(I2C3,I2C_FLAG_ADDR);
	I2C_GenerateSTOP(I2C3,ENABLE);
}

void bsp_ist8310_write_single_reg(uint8_t address,uint8_t value)
{
    I2C_GenerateSTART(I2C3,ENABLE);
    while (!I2C_CheckEvent(I2C3,I2C_EVENT_MASTER_MODE_SELECT));

	I2C_Send7bitAddress(I2C3,IST8310_IIC_ADDRESS,I2C_Direction_Transmitter);
	while (!I2C_CheckEvent(I2C3,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_ClearFlag(I2C3,I2C_FLAG_ADDR);
	
    I2C_SendData(I2C3,address);
	
	while(!I2C_GetFlagStatus(I2C3,I2C_FLAG_TXE));
	
	I2C_SendData(I2C3,value);
	
	while(!I2C_GetFlagStatus(I2C3,I2C_FLAG_BTF));
	I2C_GenerateSTOP(I2C3,ENABLE);
}

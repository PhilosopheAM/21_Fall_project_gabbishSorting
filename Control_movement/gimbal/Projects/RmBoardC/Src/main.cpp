#include "Led.hpp"
#include "Buzzer.hpp"
#include "Dr16.hpp"
#include "CanManager.hpp"
#include "CanMsgDispatcher.hpp"

#include "BoardPacket.hpp"
#include "HostPacket.hpp"
#include "HostCom.hpp"

#include "Time.hpp"

#include "Testbot.hpp"

#include "IST8310.hpp"
#include "BMI088.hpp"
#include "AHRSEstimator.hpp"

extern "C" void SysTick_Handler(void);
extern "C" void CAN1_RX0_IRQHandler(void);
extern "C" void CAN2_RX0_IRQHandler(void);
extern "C" void USART1_IRQHandler(void);
extern "C" void USART6_IRQHandler(void);

const uint32_t MS_PRE_TICK = 1;

//Test stuff
Testbot testbot;

HostPacketManager* hm =  HostPacketManager::Instance();
BoardPacketManager* pBPM = BoardPacketManager::Instance();

AHRSEstimator* mahonyEstimator = AHRSEstimator::Instance();
BMI088* bmi088 = BMI088::Instance();

static uint8_t bmi088_init_flag = 0;
static uint32_t bmi088_count = 0;

void SysTick_Handler(void)
{
	//__set_PRIMASK(1);

	Time::Tick();

	Dr16::Instance()->Update();	
	HostCom::Instance()->Update();
	
	BoardPacketManager::Instance()->Update();
	HostPacketManager::Instance()->Update();

	CanMsgDispatcher::Instance()->Update();
		
	if(bmi088_init_flag == 0)
	{
		uint8_t result = bmi088->AccInit();
		
		if(result == 1)
		{
			bmi088_init_flag = 1;
		}
	}
	else if(bmi088_init_flag == 1)
	{
		uint8_t result = bmi088->GyroInit();

		if(result == 1)
		{
			bmi088_init_flag = 2;
			mahonyEstimator->SetImu(bmi088);
			mahonyEstimator->Init();
		}
	}
	else if(bmi088_count % 5 == 0)
	{
		bmi088->ReadData();
		mahonyEstimator->Update();
	}
	
	bmi088_count++;
	
	testbot.Tick();
	
	BoardPacketManager::Instance()->FlushSendBuffer();
	HostPacketManager::Instance()->FlushSendBuffer();
	
	DjiCanMotorCommander::Instance()->Update();
	CanManager::Instance()->Update();

	//__set_PRIMASK(0);
}

void CAN1_RX0_IRQHandler(void)
{
	CanRxMsg _rxMsg;

	if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET)
	{
		CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
		CAN_Receive(CAN1, CAN_FIFO0, &_rxMsg);
	}

	CanManager::Instance()->MsgQueue(0)->Enqueue(&_rxMsg);
}

void CAN2_RX0_IRQHandler(void)
{
	CanRxMsg _rxMsg;

	if (CAN_GetITStatus(CAN2, CAN_IT_FMP0) != RESET)
	{
		CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);
		CAN_Receive(CAN2, CAN_FIFO0, &_rxMsg);
	}

	CanManager::Instance()->MsgQueue(1)->Enqueue(&_rxMsg);
}

void USART6_IRQHandler(void)
{
    __set_PRIMASK(1);
	if(USART_GetFlagStatus(USART6, USART_IT_RXNE) != RESET)
	{
        USART_ClearITPendingBit(USART6, USART_IT_RXNE);
        uint8_t _data = USART6->DR;
        HostPacketManager::Instance()->Enqueue(0, &_data, 1);
	}
    
	else if(USART_GetFlagStatus(USART6, USART_IT_IDLE) != RESET)
    {
        uint8_t clear = USART6->SR;
        clear = USART6->DR;
    }
    else if(USART_GetFlagStatus(USART6, USART_IT_NE) != RESET)
    {
        USART_ReceiveData(USART6);
    }
    __set_PRIMASK(0);
}

void SysTick_Init(uint32_t tick_ms)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);

	uint32_t reload = SystemCoreClock / 8000 * tick_ms;

	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk; // Enable SysTick Interrupt
	SysTick->LOAD = reload;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; // Enable SysTick
}

Dr16* pDr16 = Dr16::Instance();

int main(void)
{
	// NVIC setup. Remember, no Preemption.
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	Dr16::Instance()->Init();
	Buzzer::Instance()->Init();
	
	CanManager::Instance()->Init();
	
	BoardPacketManager::Instance()->Init();
	HostPacketManager::Instance()->Init(); 
	HostCom::Instance()->Init();
	
	bmi088->BspInit();
	
	testbot.Init();
	
	SysTick_Init(MS_PRE_TICK);
	
	while (1)
	{
	}
}

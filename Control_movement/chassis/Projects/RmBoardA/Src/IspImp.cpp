#include "IspImp.hpp"

#include "CanManager.hpp"
#include "CanMsgDispatcher.hpp"

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
	if(USART_GetFlagStatus(USART6, USART_IT_RXNE))
	{
    	uint8_t _data = USART6->DR;
		_data = USART6->DR;
	}
    if(USART_GetFlagStatus(USART6, USART_IT_NE))
	{
		USART_ReceiveData(USART6);
	}

		uint16_t _data = USART6->SR;
		_data = USART6->DR;

}

#ifndef CANMSGHANDLER_H
#define CANMSGHANDLER_H

#include "stm32f4xx.h"
#include "Time.hpp"
#include <string.h>

class CanMsgHandler
{
protected:
    CanRxMsg lastCanMsg;
    uint32_t m_LastReceivedTick;
public:
    virtual void HandleNewCanRxMsg(CanRxMsg* _msg)
    {
        memcpy(&lastCanMsg, _msg, sizeof(CanRxMsg));
        m_LastReceivedTick = Time::GetTick();
    }
};

#endif

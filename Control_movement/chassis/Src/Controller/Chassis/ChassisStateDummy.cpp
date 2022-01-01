#include "ChassisStateDummy.hpp"

void ChassisStateDummy::Init(ChassisController* pOwner)
{
    m_pBoardManager = BoardPacketManager::Instance();
}

void ChassisStateDummy::Enter(ChassisController* pOwner)
{
    ;
}

void ChassisStateDummy::Execute(ChassisController* pOwner)
{
    ChassisCtrlMsg& ctrlMsg = m_pBoardManager->GetChassisCtrlMsg();
    if(!ctrlMsg.IsMsgTimeout())
    {
        pOwner->SetVx(ctrlMsg.GetVx());
        pOwner->SetVy(ctrlMsg.GetVy());
        pOwner->SetVw(ctrlMsg.GetVw());
    }
    else
    {
        pOwner->Stop();
    }
}

void ChassisStateDummy::Exit(ChassisController* pOwner)
{
    ;
}

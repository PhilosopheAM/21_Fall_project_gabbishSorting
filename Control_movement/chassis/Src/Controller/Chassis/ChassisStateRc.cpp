#include "ChassisStateRc.hpp"

void ChassisStateRc::Init(ChassisController* pOwner)
{
    ;
}

void ChassisStateRc::Enter(ChassisController* pOwner)
{
    ;
}

void ChassisStateRc::Execute(ChassisController* pOwner)
{
    pOwner->SetVx(Dr16::Instance()->GetLVAxis() * 30.0f);
	pOwner->SetVy(Dr16::Instance()->GetLHAxis() * 10.0f);
	pOwner->SetVw(Dr16::Instance()->GetRHAxis() * -10.0f);
}

void ChassisStateRc::Exit(ChassisController* pOwner)
{
    ;
}

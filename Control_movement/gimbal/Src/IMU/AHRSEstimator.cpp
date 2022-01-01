#include "AHRSEstimator.hpp"

void AHRSEstimator::Init()
{
    for(int i = 0;i<3;i++)
    {
        m_acc_lp_filter[i].SetTau(0.5f);
        m_acc_lp_filter[i].SetUpdatePeriod(5);
    }

	m_q[0] = m_q[1] = m_q[2] = m_q[3] = 0;
    AHRS_init(m_q,m_a,m_mag);
	
	gyro_filter[0].SetUpdatePeriod(2);
	gyro_filter[1].SetUpdatePeriod(2);
	gyro_filter[2].SetUpdatePeriod(2);
	
	gyro_filter[0].SetTau(10.0f);
	gyro_filter[1].SetTau(10.0f);
	gyro_filter[2].SetTau(10.0f);
}

void AHRSEstimator::Update()
{
    //get accel
    tmp_m_a[0] = m_pImuOnChip->GetAccY();
    tmp_m_a[1] = -m_pImuOnChip->GetAccX();
    tmp_m_a[2] = m_pImuOnChip->GetAccZ();
	

	//accel filter
    accel_fliter_1[0] = accel_fliter_2[0];
    accel_fliter_2[0] = accel_fliter_3[0];

    accel_fliter_3[0] = accel_fliter_2[0] * fliter_num[0] + accel_fliter_1[0] * fliter_num[1] + tmp_m_a[0] * fliter_num[2];

    accel_fliter_1[1] = accel_fliter_2[1];
    accel_fliter_2[1] = accel_fliter_3[1];

    accel_fliter_3[1] = accel_fliter_2[1] * fliter_num[0] + accel_fliter_1[1] * fliter_num[1] + tmp_m_a[1] * fliter_num[2];

    accel_fliter_1[2] = accel_fliter_2[2];
    accel_fliter_2[2] = accel_fliter_3[2];

    accel_fliter_3[2] = accel_fliter_2[2] * fliter_num[0] + accel_fliter_1[2] * fliter_num[1] + tmp_m_a[2] * fliter_num[2];

    //angular speed update
	
	gyro_filter[0].SetInput(m_pImuOnChip->GetWy());
	gyro_filter[1].SetInput(-m_pImuOnChip->GetWx());
	gyro_filter[2].SetInput(m_pImuOnChip->GetWz());
	
	gyro_filter[0].Update();
	gyro_filter[1].Update();
	gyro_filter[2].Update();
	
    tmp_m_gyro[0] = m_pImuOnChip->GetWy() + 0.00263f;
    tmp_m_gyro[1] = -m_pImuOnChip->GetWx() + 0.00454f;
    tmp_m_gyro[2] = m_pImuOnChip->GetWz() + -0.000455f;
	
	
	//  mag update
//	  tmp_m_mag[0] = m_pIST8310->m_mag_x;
//    tmp_m_mag[1] = m_pIST8310->m_mag_y;
//    tmp_m_mag[2] = m_pIST8310->m_mag_z;
	
    
	//  Installation Pose Fix
	for(int i = 0; i < 3; i ++){
		m_gyro[i] = installSpinMatrix[i][0] * tmp_m_gyro[0] + installSpinMatrix[i][1] * tmp_m_gyro[1] + installSpinMatrix[i][2] * tmp_m_gyro[2];
		m_a[i] = installSpinMatrix[i][0] * accel_fliter_3[0] + installSpinMatrix[i][1] * accel_fliter_3[1] + installSpinMatrix[i][2] * accel_fliter_3[2];
		m_mag[i] = installSpinMatrix[i][0] * tmp_m_mag[0] + installSpinMatrix[i][1] * tmp_m_mag[1] + installSpinMatrix[i][2] * tmp_m_mag[2];
	}
	
    //updaate quaternion
	AHRS_update(m_q,0.005,m_gyro,m_a,m_mag);//see main : gyro updated every 5 ms.

    //calculate AHRS from quaternion
	m_Euler[0] = get_RPY_yaw(m_q);
    m_Euler[1] = get_RPY_pitch(m_q);
    m_Euler[2] = get_RPY_roll(m_q);
	
	float delta_yaw = m_Euler[0]-m_lastYaw;
	
	if(delta_yaw<-3.14f)
	{
		delta_yaw = 6.28f + delta_yaw;
	}
	else if(delta_yaw>3.14f)
	{
		delta_yaw = 6.28f - delta_yaw;
	}
	
	m_AbsYaw += delta_yaw;
	
	m_lastYaw = m_Euler[0];
    
}   

float get_RPY_yaw(float m_q[4]){

    float q0 = m_q[0];
    float q1 = m_q[1];
    float q2 = m_q[2];
    float q3 = m_q[3];

    return atan2f(2.0f*(q0*q3-q1*q2),2.0f*(q0*q0+q2*q2)-1.0f);

}

float get_RPY_pitch(float m_q[4]){
    
    float q0 = m_q[0];
    float q1 = m_q[1];
    float q2 = m_q[2];
    float q3 = m_q[3];

    return atan2f(2.0f*(q0*q2-q1*q3),2.0f*(q0*q0+q3*q3)-1.0f);

}

float get_RPY_roll(float m_q[4]){
    
    float q0 = m_q[0];
    float q1 = m_q[1];
    float q2 = m_q[2];
    float q3 = m_q[3];

    return asinf(2.0f*q0*q1+2.0f*q2*q3);
}

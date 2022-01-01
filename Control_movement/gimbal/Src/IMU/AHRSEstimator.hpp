#ifndef AHRS_ESTIMATOR_HPP
#define AHRS_ESTIMATOR_HPP

#include "PoseEstimator.hpp"
#include "math_first_order_filter.h"
#include "MahonyAHRS.h"
#include "math.h"
#include "AHRS.h"

static fp32 accel_fliter_1[3] = {0.0f, 0.0f, 0.0f};
static fp32 accel_fliter_2[3] = {0.0f, 0.0f, 0.0f};
static fp32 accel_fliter_3[3] = {0.0f, 0.0f, 0.0f};
static const fp32 fliter_num[3] = {1.929454039488895f, -0.93178349823448126f, 0.002329458745586203f};

static float installSpinMatrix[3][3] = 
{
	{0.0f, -1.0f, 0.0f},
	{1.0f, 0.0f, 0.0f},
	{0.0f, 0.0f, 1.0f}
};
 


class AHRSEstimator : public PoseEstimator
{
private :
	AHRSEstimator(){;}

    FirstOrderFilter m_acc_lp_filter[3];

	FirstOrderFilter gyro_filter[3];

	float tmp_m_a[3];
	float tmp_m_mag[3];
	float tmp_m_gyro[3];
	
    float m_q[4];
	
    float m_a[3];
    float m_mag[3];
    float m_gyro[3];
		
public:

	static AHRSEstimator* Instance()
	{
		static AHRSEstimator instance;
		return &instance;
	}

    virtual void Init();
    virtual void Update();



};

	//These alternative functions returns euler angle in roll-pitch-yaw format.
    float get_RPY_yaw(float m_q[4]);

    float get_RPY_pitch(float m_q[4]);

    float get_RPY_roll(float m_q[4]);

#endif

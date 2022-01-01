#ifndef BMI088_HPP
#define BMI088_HPP

#include "bsp_bmi088.h"

enum
{
    BMI088_NO_ERROR = 0x00,
    BMI088_ACC_PWR_CTRL_ERROR = 0x01,
    BMI088_ACC_PWR_CONF_ERROR = 0x02,
    BMI088_ACC_CONF_ERROR = 0x03,
    BMI088_ACC_SELF_TEST_ERROR = 0x04,
    BMI088_ACC_RANGE_ERROR = 0x05,
    BMI088_INT1_IO_CTRL_ERROR = 0x06,
    BMI088_INT_MAP_DATA_ERROR = 0x07,
    BMI088_GYRO_RANGE_ERROR = 0x08,
    BMI088_GYRO_BANDWIDTH_ERROR = 0x09,
    BMI088_GYRO_LPM1_ERROR = 0x0A,
    BMI088_GYRO_CTRL_ERROR = 0x0B,
    BMI088_GYRO_INT3_INT4_IO_CONF_ERROR = 0x0C,
    BMI088_GYRO_INT3_INT4_IO_MAP_ERROR = 0x0D,

    BMI088_SELF_TEST_ACCEL_ERROR = 0x80,
    BMI088_SELF_TEST_GYRO_ERROR = 0x40,
    BMI088_NO_SENSOR = 0xFF,
};

class BMI088
{
public:
    static BMI088* Instance()
    {
        static BMI088 instance;
        return &instance;
    }

    ~BMI088(){}

    enum BMI088Cs
    {
        CS_Acc,
        CS_Gyro
    };

    void BspInit();
    int AccInit();
    int GyroInit();
    void ReadData();

    float GetAccX(){return m_ax;}
    float GetAccY(){return m_ay;}
    float GetAccZ(){return m_az;}
    float GetAccAngleX(){return m_acc_angle_x;}
    float GetAccAngleY(){return m_acc_angle_y;}
    float GetAccAngleZ(){return m_acc_angle_z;}
	float GetWx(){return m_wx;}
	float GetWy(){return m_wy;}
	float GetWz(){return m_wz;}
private:
    BMI088():m_send_buffer_count(0),m_time_ms_count(0){}
    void ReadRegister(uint8_t _address, uint8_t* _pData, uint32_t _len,BMI088Cs cs);
    void WriteRegister(uint8_t _address, uint8_t* _pData, uint32_t _len,BMI088Cs cs);
    uint8_t m_send_buffer[16];

    uint8_t m_send_buffer_count;
    int m_time_ms_count;

    float m_ax;
    float m_ay;
    float m_az;
    float m_acc;
    float m_acc_angle_x;
    float m_acc_angle_y;
    float m_acc_angle_z;
		
	float m_wx;
	float m_wy;
	float m_wz;
};

#endif

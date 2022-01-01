#include "IST8310.hpp"

static uint8_t IST8310_Init_count = 0;
static int IST8310_Init_time_count = 0;
static uint8_t test_buffer[4] ={0}; 

static uint8_t ist8310_write_reg_data_error[4][3] =
    {
        {0x0B, 0x08, 0x01},
        {0x41, 0x09, 0x02},
        {0x42, 0xC0, 0x03},
        {0x0A, 0x0B, 0x04}};

uint8_t IST8310::Init()
{
    if(IST8310_Init_count==0)
    {
        bsp_ist8310_init(data_buffer);
		bsp_ist8310_timer();
        IST8310_Init_count = 1;
        bsp_ist8310_RST_L();
    }
    else if(IST8310_Init_count==1)
    {
        IST8310_Init_time_count++;

        if(IST8310_Init_time_count>150)
        {
            IST8310_Init_time_count = 0;
            IST8310_Init_count = 2;
            bsp_ist8310_RST_H();
        }
    }
    else if(IST8310_Init_count==2)
    {
        IST8310_Init_time_count++;

        if(IST8310_Init_time_count>150)
        {
            IST8310_Init_time_count = 0;
            IST8310_Init_count = 3;
        }
    }
    else if(IST8310_Init_count==3)
    {
        uint8_t res = bsp_ist8310_read_single_reg(IST8310_WHO_AM_I);

        if (res != IST8310_WHO_AM_I_VALUE)
        {
            return 0;
        }
        IST8310_Init_count = 4;
    }
    else if(IST8310_Init_count==4)
    {
        bsp_ist8310_write_single_reg(
            ist8310_write_reg_data_error[IST8310_Init_time_count][0],
            ist8310_write_reg_data_error[IST8310_Init_time_count][1]);

        IST8310_Init_time_count++;

        if(IST8310_Init_time_count == 4)
        {
            IST8310_Init_time_count = 0;
            IST8310_Init_count = 5;
        }
    }
    else if(IST8310_Init_count == 5)
    {
        test_buffer[IST8310_Init_time_count] =
            bsp_ist8310_read_single_reg(ist8310_write_reg_data_error[IST8310_Init_time_count][0]);
		IST8310_Init_time_count++;
        if(IST8310_Init_time_count == 4)
        {
            IST8310_Init_time_count = 0;
            IST8310_Init_count = 6;
        }
    }
	else if(IST8310_Init_count == 6)
	{
		//Check test_buffer TODO
        bsp_start_read_data();
		return 1;
	}
    return 0;
}

void IST8310::Update()
{
    if(bsp_get_update_flag())
    {
        int16_t temp_ist8310_data = 0;
        temp_ist8310_data = (int16_t)((data_buffer[1] << 8) | data_buffer[0]);
        m_mag_x = 0.3f * temp_ist8310_data;

        temp_ist8310_data = (int16_t)((data_buffer[3] << 8) | data_buffer[2]);
        m_mag_y = 0.3f * temp_ist8310_data;

        temp_ist8310_data = (int16_t)((data_buffer[5] << 8) | data_buffer[4]);
        m_mag_z = 0.3f * temp_ist8310_data;
        bsp_clear_update_flag();
    }
}


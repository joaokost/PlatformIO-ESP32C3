#include <stdint.h>
#include <stdbool.h>
#include "i2c_driver.h"
#include "IS32.h"


uint8_t global_current = 0x80;
uint8_t channel_scaling = 0x80;

void IS32_Init(void)
{
	//Init I2C
	i2c_init();
	//Set All to 0
	IS32_Reset();
	//Set Control Register (Normal Operation, 8 bit PWM Resolution, Oscillator Freq. 31 kHz) 
	IS32_SetControlRegister(IS32_CONTROL_SSD_NORMAL_OPERATION, IS32_CONTROL_PWM_RESOLUTION_8BIT, IS32_CONTROL_OSC_FREQUENCY_31KHZ);
	//Set Global Current Control Register
	IS32_SetGlobalCurrentRegister(global_current);
	//Set Scaling Register for every Channel
	IS32_SetScaling(1, channel_scaling);
	IS32_SetScaling(2, channel_scaling);
	IS32_SetScaling(3, channel_scaling);
	IS32_SetScaling(4, channel_scaling);
	IS32_SetScaling(5, channel_scaling);
	IS32_SetScaling(6, channel_scaling);
	IS32_SetScaling(7, channel_scaling);
	IS32_SetScaling(8, channel_scaling);
	IS32_SetScaling(9, channel_scaling);
	IS32_SetScaling(10, channel_scaling);
	IS32_SetScaling(11, channel_scaling);
	IS32_SetScaling(12, channel_scaling);
	IS32_SetScaling(13, channel_scaling);
	IS32_SetScaling(14, channel_scaling);
	IS32_SetScaling(15, channel_scaling);
	IS32_SetScaling(16, channel_scaling);
	IS32_SetScaling(17, channel_scaling);
	IS32_SetScaling(18, channel_scaling);
	IS32_SetScaling(19, channel_scaling);
	IS32_SetScaling(20, channel_scaling);
	IS32_SetScaling(21, channel_scaling);
	IS32_SetScaling(22, channel_scaling);
	IS32_SetScaling(23, channel_scaling);
	IS32_SetScaling(24, channel_scaling);
	IS32_SetScaling(25, channel_scaling);
	IS32_SetScaling(26, channel_scaling);
	IS32_SetScaling(27, channel_scaling);
	IS32_SetScaling(28, channel_scaling);
	IS32_SetScaling(29, channel_scaling);
	IS32_SetScaling(30, channel_scaling);
	IS32_SetScaling(31, channel_scaling);
	IS32_SetScaling(32, channel_scaling);
	IS32_SetScaling(33, channel_scaling);
	IS32_SetScaling(34, channel_scaling);
	IS32_SetScaling(35, channel_scaling);
	IS32_SetScaling(36, channel_scaling);
	
}

void IS32_WriteRegister(uint8_t reg, uint8_t value)
{
	uint8_t data[2] = {reg, value};
	i2c_write(IS32_SLAVE_ADDRESS, data, 2);
}

uint8_t IS32_ReadRegister(uint8_t reg)
{
	uint8_t data[1] = {reg};
	i2c_write(IS32_SLAVE_ADDRESS, data, 1);
	i2c_read(IS32_SLAVE_ADDRESS, data, 1);
	return data[0];
}

void IS32_SetControlRegister(uint8_t osc, uint8_t pms, uint8_t ssd)
{
	uint8_t register_value = 0;
	osc &= 0x07;
	pms &= 0x03;
	ssd &= 0x01;
	//Build value with variables
	register_value = (osc << 6) | (pms << 1) | ssd;
	//Set Control Register
	IS32_WriteRegister(IS32_CONTROL_REGISTER, register_value);
}
	
void IS32_SetPWM(uint8_t channel, uint16_t pwm)
{
	uint8_t pwm_low = 0;
	uint8_t pwm_high = 0;
	pwm_low = (uint8_t)(pwm & 0x00FF);
	pwm_high = ((pwm & 0xFF00) >> 8);
	
	if(channel < 1)
	{
		channel = 1;
	}
	if(channel > 36)
	{
		channel = 36;
	}
	switch(channel)
	{
		case 1:
			IS32_WriteRegister(IS32_CH1_PWM_L_REGISTER, pwm_low);
			IS32_WriteRegister(IS32_CH1_PWM_H_REGISTER, pwm_high);
		break;
		
		case 2:
			IS32_WriteRegister(IS32_CH2_PWM_L_REGISTER, pwm_low);
			IS32_WriteRegister(IS32_CH2_PWM_H_REGISTER, pwm_high);
		break;
		
		case 3:
			IS32_WriteRegister(IS32_CH3_PWM_L_REGISTER, pwm_low);
			IS32_WriteRegister(IS32_CH3_PWM_H_REGISTER, pwm_high);
		break;
		
		case 4:
			IS32_WriteRegister(IS32_CH4_PWM_L_REGISTER, pwm_low);
			IS32_WriteRegister(IS32_CH4_PWM_H_REGISTER, pwm_high);
		break;
		
		case 5:
			IS32_WriteRegister(IS32_CH5_PWM_L_REGISTER, pwm_low);
			IS32_WriteRegister(IS32_CH5_PWM_H_REGISTER, pwm_high);
		break;
		
		case 6:
			IS32_WriteRegister(IS32_CH6_PWM_L_REGISTER, pwm_low);
			IS32_WriteRegister(IS32_CH6_PWM_H_REGISTER, pwm_high);
		break;
		
		case 7:
			IS32_WriteRegister(IS32_CH7_PWM_L_REGISTER, pwm_low);
			IS32_WriteRegister(IS32_CH7_PWM_H_REGISTER, pwm_high);
		break;
		
		case 8:
			IS32_WriteRegister(IS32_CH8_PWM_L_REGISTER, pwm_low);
			IS32_WriteRegister(IS32_CH8_PWM_H_REGISTER, pwm_high);
		break;
		
		case 9:
			IS32_WriteRegister(IS32_CH9_PWM_L_REGISTER, pwm_low);
			IS32_WriteRegister(IS32_CH9_PWM_H_REGISTER, pwm_high);
		break;
		
		case 10:
			IS32_WriteRegister(IS32_CH10_PWM_L_REGISTER, pwm_low);
			IS32_WriteRegister(IS32_CH10_PWM_H_REGISTER, pwm_high);
		break;
		
		case 11:
			IS32_WriteRegister(IS32_CH11_PWM_L_REGISTER, pwm_low);
			IS32_WriteRegister(IS32_CH11_PWM_H_REGISTER, pwm_high);
		break;
		
		case 12:
			IS32_WriteRegister(IS32_CH12_PWM_L_REGISTER, pwm_low);
			IS32_WriteRegister(IS32_CH12_PWM_H_REGISTER, pwm_high);
		break;
		
		case 13:
			IS32_WriteRegister(IS32_CH13_PWM_L_REGISTER, pwm_low);
			IS32_WriteRegister(IS32_CH13_PWM_H_REGISTER, pwm_high);
		break;
		
		case 14:
			IS32_WriteRegister(IS32_CH14_PWM_L_REGISTER, pwm_low);
			IS32_WriteRegister(IS32_CH14_PWM_H_REGISTER, pwm_high);
		break;
		
		case 15:
			IS32_WriteRegister(IS32_CH15_PWM_L_REGISTER, pwm_low);
			IS32_WriteRegister(IS32_CH15_PWM_H_REGISTER, pwm_high);
		break;
		
		case 16:
			IS32_WriteRegister(IS32_CH16_PWM_L_REGISTER, pwm_low);
			IS32_WriteRegister(IS32_CH16_PWM_H_REGISTER, pwm_high);
		break;
		
		case 17:
			IS32_WriteRegister(IS32_CH17_PWM_L_REGISTER, pwm_low);
			IS32_WriteRegister(IS32_CH17_PWM_H_REGISTER, pwm_high);
		break;
		
		case 18:
			IS32_WriteRegister(IS32_CH18_PWM_L_REGISTER, pwm_low);
			IS32_WriteRegister(IS32_CH18_PWM_H_REGISTER, pwm_high);
		break;
		
		case 19:
			IS32_WriteRegister(IS32_CH19_PWM_L_REGISTER, pwm_low);
			IS32_WriteRegister(IS32_CH19_PWM_H_REGISTER, pwm_high);
		break;
		
		case 20:
			IS32_WriteRegister(IS32_CH20_PWM_L_REGISTER, pwm_low);
			IS32_WriteRegister(IS32_CH20_PWM_H_REGISTER, pwm_high);
		break;
		
		case 21:
			IS32_WriteRegister(IS32_CH21_PWM_L_REGISTER, pwm_low);
			IS32_WriteRegister(IS32_CH21_PWM_H_REGISTER, pwm_high);
		break;
		
		case 22:
			IS32_WriteRegister(IS32_CH22_PWM_L_REGISTER, pwm_low);
			IS32_WriteRegister(IS32_CH22_PWM_H_REGISTER, pwm_high);
		break;
		
		case 23:
			IS32_WriteRegister(IS32_CH23_PWM_L_REGISTER, pwm_low);
			IS32_WriteRegister(IS32_CH23_PWM_H_REGISTER, pwm_high);
		break;
		
		case 24:
			IS32_WriteRegister(IS32_CH24_PWM_L_REGISTER, pwm_low);
			IS32_WriteRegister(IS32_CH24_PWM_H_REGISTER, pwm_high);
		break;
		
		case 25:
			IS32_WriteRegister(IS32_CH25_PWM_L_REGISTER, pwm_low);
			IS32_WriteRegister(IS32_CH25_PWM_H_REGISTER, pwm_high);
		break;
		
		case 26:
			IS32_WriteRegister(IS32_CH26_PWM_L_REGISTER, pwm_low);
			IS32_WriteRegister(IS32_CH26_PWM_H_REGISTER, pwm_high);
		break;
		
		case 27:
			IS32_WriteRegister(IS32_CH27_PWM_L_REGISTER, pwm_low);
			IS32_WriteRegister(IS32_CH27_PWM_H_REGISTER, pwm_high);
		break;
		
		case 28:
			IS32_WriteRegister(IS32_CH28_PWM_L_REGISTER, pwm_low);
			IS32_WriteRegister(IS32_CH28_PWM_H_REGISTER, pwm_high);
		break;
		
		case 29:
			IS32_WriteRegister(IS32_CH29_PWM_L_REGISTER, pwm_low);
			IS32_WriteRegister(IS32_CH29_PWM_H_REGISTER, pwm_high);
		break;
		
		case 30:
			IS32_WriteRegister(IS32_CH30_PWM_L_REGISTER, pwm_low);
			IS32_WriteRegister(IS32_CH30_PWM_H_REGISTER, pwm_high);
		break;
		
		case 31:
			IS32_WriteRegister(IS32_CH31_PWM_L_REGISTER, pwm_low);
			IS32_WriteRegister(IS32_CH31_PWM_H_REGISTER, pwm_high);
		break;
		
		case 32:
			IS32_WriteRegister(IS32_CH32_PWM_L_REGISTER, pwm_low);
			IS32_WriteRegister(IS32_CH32_PWM_H_REGISTER, pwm_high);
		break;
		
		case 33:
			IS32_WriteRegister(IS32_CH33_PWM_L_REGISTER, pwm_low);
			IS32_WriteRegister(IS32_CH33_PWM_H_REGISTER, pwm_high);
		break;
		
		case 34:
			IS32_WriteRegister(IS32_CH34_PWM_L_REGISTER, pwm_low);
			IS32_WriteRegister(IS32_CH34_PWM_H_REGISTER, pwm_high);
		break;
		
		case 35:
			IS32_WriteRegister(IS32_CH35_PWM_L_REGISTER, pwm_low);
			IS32_WriteRegister(IS32_CH35_PWM_H_REGISTER, pwm_high);
		break;
		
		case 36:
			IS32_WriteRegister(IS32_CH36_PWM_L_REGISTER, pwm_low);
			IS32_WriteRegister(IS32_CH36_PWM_H_REGISTER, pwm_high);
		break;
	}	
	IS32_Update();
}

void IS32_Update(void)
{
	//Write 0x00 to Update Register
	IS32_WriteRegister(IS32_UPDATE_REGISTER, 0x00);
}

void IS32_SetScaling(uint8_t channel, uint8_t value)
{
	if(channel < 1)
	{
		channel = 1;
	}
	if(channel > 36)
	{
		channel = 36;
	}
	switch(channel)
	{
		case 1:
			IS32_WriteRegister(IS32_CH1_LED_SCALING_REGISTER, value);
		break;
		
		case 2:
			IS32_WriteRegister(IS32_CH2_LED_SCALING_REGISTER, value);
		break;
		
		case 3:
			IS32_WriteRegister(IS32_CH3_LED_SCALING_REGISTER, value);
		break;
		
		case 4:
			IS32_WriteRegister(IS32_CH4_LED_SCALING_REGISTER, value);
		break;
		
		case 5:
			IS32_WriteRegister(IS32_CH5_LED_SCALING_REGISTER, value);
		break;
		
		case 6:
			IS32_WriteRegister(IS32_CH6_LED_SCALING_REGISTER, value);
		break;
		
		case 7:
			IS32_WriteRegister(IS32_CH7_LED_SCALING_REGISTER, value);
		break;
		
		case 8:
			IS32_WriteRegister(IS32_CH8_LED_SCALING_REGISTER, value);
		break;
		
		case 9:
			IS32_WriteRegister(IS32_CH9_LED_SCALING_REGISTER, value);
		break;
		
		case 10:
			IS32_WriteRegister(IS32_CH10_LED_SCALING_REGISTER, value);
		break;
		
		case 11:
			IS32_WriteRegister(IS32_CH11_LED_SCALING_REGISTER, value);
		break;
		
		case 12:
			IS32_WriteRegister(IS32_CH12_LED_SCALING_REGISTER, value);
		break;
		
		case 13:
			IS32_WriteRegister(IS32_CH13_LED_SCALING_REGISTER, value);
		break;
		
		case 14:
			IS32_WriteRegister(IS32_CH14_LED_SCALING_REGISTER, value);
		break;
		
		case 15:
			IS32_WriteRegister(IS32_CH15_LED_SCALING_REGISTER, value);
		break;
		
		case 16:
			IS32_WriteRegister(IS32_CH16_LED_SCALING_REGISTER, value);
		break;
		
		case 17:
			IS32_WriteRegister(IS32_CH17_LED_SCALING_REGISTER, value);
		break;
		
		case 18:
			IS32_WriteRegister(IS32_CH18_LED_SCALING_REGISTER, value);
		break;
		
		case 19:
			IS32_WriteRegister(IS32_CH19_LED_SCALING_REGISTER, value);
		break;
		
		case 20:
			IS32_WriteRegister(IS32_CH20_LED_SCALING_REGISTER, value);
		break;
		
		case 21:
			IS32_WriteRegister(IS32_CH21_LED_SCALING_REGISTER, value);
		break;
		
		case 22:
			IS32_WriteRegister(IS32_CH22_LED_SCALING_REGISTER, value);
		break;
		
		case 23:
			IS32_WriteRegister(IS32_CH23_LED_SCALING_REGISTER, value);
		break;
		
		case 24:
			IS32_WriteRegister(IS32_CH24_LED_SCALING_REGISTER, value);
		break;
		
		case 25:
			IS32_WriteRegister(IS32_CH25_LED_SCALING_REGISTER, value);
		break;
		
		case 26:
			IS32_WriteRegister(IS32_CH26_LED_SCALING_REGISTER, value);
		break;
		
		case 27:
			IS32_WriteRegister(IS32_CH27_LED_SCALING_REGISTER, value);
		break;
		
		case 28:
			IS32_WriteRegister(IS32_CH28_LED_SCALING_REGISTER, value);
		break;
		
		case 29:
			IS32_WriteRegister(IS32_CH29_LED_SCALING_REGISTER, value);
		break;
		
		case 30:
			IS32_WriteRegister(IS32_CH30_LED_SCALING_REGISTER, value);
		break;
		
		case 31:
			IS32_WriteRegister(IS32_CH31_LED_SCALING_REGISTER, value);
		break;
		
		case 32:
			IS32_WriteRegister(IS32_CH32_LED_SCALING_REGISTER, value);
		break;
		
		case 33:
			IS32_WriteRegister(IS32_CH33_LED_SCALING_REGISTER, value);
		break;
		
		case 34:
			IS32_WriteRegister(IS32_CH34_LED_SCALING_REGISTER, value);
		break;
		
		case 35:
			IS32_WriteRegister(IS32_CH35_LED_SCALING_REGISTER, value);
		break;
		
		case 36:
			IS32_WriteRegister(IS32_CH36_LED_SCALING_REGISTER, value);
		break;	
	}	
}

void IS32_SetGlobalCurrentRegister(uint8_t value)
{
	uint8_t register_value = 0;
	//Build value with variables
	register_value = value;
	//Set Register
	IS32_WriteRegister(IS32_GLOBAL_CURRENT_CONTROL_REGISTER, register_value);
}

uint8_t IS32_GetGlobalCurrentRegister(void)
{
	uint8_t register_value = 0;
	//Read Global Current Control Register
	register_value = IS32_ReadRegister(IS32_GLOBAL_CURRENT_CONTROL_REGISTER);
	return register_value;
}

void IS32_SetPhaseDelayAndClockPhase(uint8_t phase_delay, uint8_t clock_phase_select)
{
	uint8_t register_value = 0;
	phase_delay &= 0x01;
	clock_phase_select &= 0x1F;
	//Build value with variables
	register_value = (phase_delay << 7) | clock_phase_select;
	//Set Register
	IS32_WriteRegister(IS32_PHASE_DELAY_COCK_PHASE_REGISTER, register_value);
}

void IS32_SetOpenShortDetectEnableRegister(uint8_t value)
{
	uint8_t register_value = 0;
	value &= 0x03;
	//Build value with variables
	register_value = value;
	//Set Register
	IS32_WriteRegister(IS32_OPEN_SHORT_DETECT_ENABLE_REGISTER, register_value);
}

void IS32_GetOpenShort(uint8_t* channels[5])
{
	uint8_t register_value = 0;
	//Read Open/Short Registers and save on array
	for(uint8_t i = 0; i < 5; i++)
	{
		register_value = IS32_ReadRegister(IS32_LED_OPEN_SHORT_REGISTER_1 + i);
		*(channels + i) = register_value;
	}
}

uint8_t IS32_GetTemperatureSensorRegister(void)
{
	uint8_t register_value = 0;
	//Read Register
	register_value = IS32_ReadRegister(IS32_TEMPERATURE_SENSOR_REGISTER);
	return register_value;
}


void IS32_SetSpreadSpectrumRegister(uint8_t dcpwm_ch1_12, uint8_t dcpwm_ch13_24, uint8_t dcpwm_ch25_36, uint8_t ssp, uint8_t rng, uint8_t clt)
{
	uint8_t register_value = 0;
	dcpwm_ch1_12 &= 0x01;
	dcpwm_ch13_24 &= 0x01;
	dcpwm_ch25_36 &= 0x01;
	ssp &= 0x01;
	rng &= 0x03;
	clt &= 0x03;
	//Build value with variables
	register_value = (dcpwm_ch25_36 << 3) | (dcpwm_ch13_24 << 3) | (dcpwm_ch1_12 << 3) | (ssp << 2) | (rng << 1) | clt;
	//Set Register
	IS32_WriteRegister(IS32_SPREAD_SPECTRUM_REGISTER, register_value);
}

void IS32_Reset(void)
{
	//Write 0x00 to Reset Register
	IS32_WriteRegister(IS32_RESET_REGISTER, 0x00);
}

void IS32_setRGB(uint8_t led, uint8_t red, uint8_t green, uint8_t blue)
{
	if(led < 1)
	{
		led = 1;
	}
	if(led > 12)
	{
		led = 12;
	}
	//RED  and  BLUE are other way around!
	switch(led)
	{
			case 1:
				IS32_SetPWM(1, blue);
				IS32_SetPWM(2, green);
				IS32_SetPWM(3, red);
			break;
			
			case 2:
				IS32_SetPWM(4, blue);
				IS32_SetPWM(5, green);
				IS32_SetPWM(6, red);
			break;
			
			case 3:
				IS32_SetPWM(7, blue);
				IS32_SetPWM(8, green);
				IS32_SetPWM(9, red);
			break;
			
			case 4:
				IS32_SetPWM(10, blue);
				IS32_SetPWM(11, green);
				IS32_SetPWM(12, red);
			break;
			
			case 5:
				IS32_SetPWM(13, blue);
				IS32_SetPWM(14, green);
				IS32_SetPWM(15, red);
			break;
			
			case 6:
				IS32_SetPWM(16, blue);
				IS32_SetPWM(17, green);
				IS32_SetPWM(18, red);
			break;
			
			case 7:
				IS32_SetPWM(19, blue);
				IS32_SetPWM(20, green);
				IS32_SetPWM(21, red);
			break;
			
			case 8:
				IS32_SetPWM(22, blue);
				IS32_SetPWM(23, green);
				IS32_SetPWM(24, red);
			break;
			
			case 9:
				IS32_SetPWM(25, blue);
				IS32_SetPWM(26, green);
				IS32_SetPWM(27, red);
			break;
			
			case 10:
				IS32_SetPWM(28, blue);
				IS32_SetPWM(29, green);
				IS32_SetPWM(30, red);
			break;
			
			case 11:
				IS32_SetPWM(31, blue);
				IS32_SetPWM(32, green);
				IS32_SetPWM(33, red);
			break;
			
			case 12:
				IS32_SetPWM(34, blue);
				IS32_SetPWM(35, green);
				IS32_SetPWM(36, red);
			break;
	}
}























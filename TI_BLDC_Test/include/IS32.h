#include <stdint.h>
#include <stdbool.h>


//Define Registers
#define IS32_CONTROL_REGISTER							0x00
#define IS32_CH1_PWM_L_REGISTER							0x01
#define IS32_CH1_PWM_H_REGISTER							0x02
#define IS32_CH2_PWM_L_REGISTER							0x03
#define IS32_CH2_PWM_H_REGISTER							0x04
#define IS32_CH3_PWM_L_REGISTER							0x05
#define IS32_CH3_PWM_H_REGISTER							0x06
#define IS32_CH4_PWM_L_REGISTER							0x07
#define IS32_CH4_PWM_H_REGISTER							0x08
#define IS32_CH5_PWM_L_REGISTER							0x09
#define IS32_CH5_PWM_H_REGISTER							0x0A
#define IS32_CH6_PWM_L_REGISTER							0x0B
#define IS32_CH6_PWM_H_REGISTER							0x0C
#define IS32_CH7_PWM_L_REGISTER							0x0D
#define IS32_CH7_PWM_H_REGISTER							0x0E
#define IS32_CH8_PWM_L_REGISTER							0x0F
#define IS32_CH8_PWM_H_REGISTER							0x10
#define IS32_CH9_PWM_L_REGISTER							0x11
#define IS32_CH9_PWM_H_REGISTER							0x12
#define IS32_CH10_PWM_L_REGISTER						0x13
#define IS32_CH10_PWM_H_REGISTER						0x14
#define IS32_CH11_PWM_L_REGISTER						0x15
#define IS32_CH11_PWM_H_REGISTER						0x16
#define IS32_CH12_PWM_L_REGISTER						0x17
#define IS32_CH12_PWM_H_REGISTER						0x18
#define IS32_CH13_PWM_L_REGISTER						0x19
#define IS32_CH13_PWM_H_REGISTER						0x1A
#define IS32_CH14_PWM_L_REGISTER						0x1B
#define IS32_CH14_PWM_H_REGISTER						0x1C
#define IS32_CH15_PWM_L_REGISTER						0x1D
#define IS32_CH15_PWM_H_REGISTER						0x1E
#define IS32_CH16_PWM_L_REGISTER						0x1F
#define IS32_CH16_PWM_H_REGISTER						0x20
#define IS32_CH17_PWM_L_REGISTER						0x21
#define IS32_CH17_PWM_H_REGISTER						0x22
#define IS32_CH18_PWM_L_REGISTER						0x23
#define IS32_CH18_PWM_H_REGISTER						0x24
#define IS32_CH19_PWM_L_REGISTER						0x25
#define IS32_CH19_PWM_H_REGISTER						0x26
#define IS32_CH20_PWM_L_REGISTER						0x27
#define IS32_CH20_PWM_H_REGISTER						0x28
#define IS32_CH21_PWM_L_REGISTER						0x29
#define IS32_CH21_PWM_H_REGISTER						0x2A
#define IS32_CH22_PWM_L_REGISTER						0x2B
#define IS32_CH22_PWM_H_REGISTER						0x2C
#define IS32_CH23_PWM_L_REGISTER						0x2D
#define IS32_CH23_PWM_H_REGISTER						0x2E
#define IS32_CH24_PWM_L_REGISTER						0x2F
#define IS32_CH24_PWM_H_REGISTER						0x30
#define IS32_CH25_PWM_L_REGISTER						0x31
#define IS32_CH25_PWM_H_REGISTER						0x32
#define IS32_CH26_PWM_L_REGISTER						0x33
#define IS32_CH26_PWM_H_REGISTER						0x34
#define IS32_CH27_PWM_L_REGISTER						0x35
#define IS32_CH27_PWM_H_REGISTER						0x36
#define IS32_CH28_PWM_L_REGISTER						0x37
#define IS32_CH28_PWM_H_REGISTER						0x38
#define IS32_CH29_PWM_L_REGISTER						0x39
#define IS32_CH29_PWM_H_REGISTER						0x3A
#define IS32_CH30_PWM_L_REGISTER						0x3B
#define IS32_CH30_PWM_H_REGISTER						0x3C
#define IS32_CH31_PWM_L_REGISTER						0x3D
#define IS32_CH31_PWM_H_REGISTER						0x3E
#define IS32_CH32_PWM_L_REGISTER						0x3F
#define IS32_CH32_PWM_H_REGISTER						0x40
#define IS32_CH33_PWM_L_REGISTER						0x41
#define IS32_CH33_PWM_H_REGISTER						0x42
#define IS32_CH34_PWM_L_REGISTER						0x43
#define IS32_CH34_PWM_H_REGISTER						0x44
#define IS32_CH35_PWM_L_REGISTER						0x45
#define IS32_CH35_PWM_H_REGISTER						0x46
#define IS32_CH36_PWM_L_REGISTER						0x47
#define IS32_CH36_PWM_H_REGISTER						0x48
#define IS32_UPDATE_REGISTER							0x49
#define IS32_CH1_LED_SCALING_REGISTER					0x4A
#define IS32_CH2_LED_SCALING_REGISTER					0x4B
#define IS32_CH3_LED_SCALING_REGISTER					0x4C
#define IS32_CH4_LED_SCALING_REGISTER					0x4D
#define IS32_CH5_LED_SCALING_REGISTER					0x4E
#define IS32_CH6_LED_SCALING_REGISTER					0x4F
#define IS32_CH7_LED_SCALING_REGISTER					0x50
#define IS32_CH8_LED_SCALING_REGISTER					0x51
#define IS32_CH9_LED_SCALING_REGISTER					0x52
#define IS32_CH10_LED_SCALING_REGISTER					0x53
#define IS32_CH11_LED_SCALING_REGISTER					0x54
#define IS32_CH12_LED_SCALING_REGISTER					0x55
#define IS32_CH13_LED_SCALING_REGISTER					0x56
#define IS32_CH14_LED_SCALING_REGISTER					0x57
#define IS32_CH15_LED_SCALING_REGISTER					0x58
#define IS32_CH16_LED_SCALING_REGISTER					0x59
#define IS32_CH17_LED_SCALING_REGISTER					0x5A
#define IS32_CH18_LED_SCALING_REGISTER					0x5B
#define IS32_CH19_LED_SCALING_REGISTER					0x5C
#define IS32_CH20_LED_SCALING_REGISTER					0x5D
#define IS32_CH21_LED_SCALING_REGISTER					0x5E
#define IS32_CH22_LED_SCALING_REGISTER					0x5F
#define IS32_CH23_LED_SCALING_REGISTER					0x60
#define IS32_CH24_LED_SCALING_REGISTER					0x61
#define IS32_CH25_LED_SCALING_REGISTER					0x62
#define IS32_CH26_LED_SCALING_REGISTER					0x63
#define IS32_CH27_LED_SCALING_REGISTER					0x64
#define IS32_CH28_LED_SCALING_REGISTER					0x65
#define IS32_CH29_LED_SCALING_REGISTER					0x66
#define IS32_CH30_LED_SCALING_REGISTER					0x67
#define IS32_CH31_LED_SCALING_REGISTER					0x68
#define IS32_CH32_LED_SCALING_REGISTER					0x69
#define IS32_CH33_LED_SCALING_REGISTER					0x6A
#define IS32_CH34_LED_SCALING_REGISTER					0x6B
#define IS32_CH35_LED_SCALING_REGISTER					0x6C
#define IS32_CH36_LED_SCALING_REGISTER					0x6D
#define IS32_GLOBAL_CURRENT_CONTROL_REGISTER			0x6E
#define IS32_PHASE_DELAY_COCK_PHASE_REGISTER			0x70
#define IS32_OPEN_SHORT_DETECT_ENABLE_REGISTER			0x71
#define IS32_LED_OPEN_SHORT_REGISTER_1					0x72
#define IS32_LED_OPEN_SHORT_REGISTER_2					0x73
#define IS32_LED_OPEN_SHORT_REGISTER_3					0x74
#define IS32_LED_OPEN_SHORT_REGISTER_4					0x75
#define IS32_LED_OPEN_SHORT_REGISTER_5					0x76
#define IS32_TEMPERATURE_SENSOR_REGISTER				0x77
#define IS32_SPREAD_SPECTRUM_REGISTER					0x78
#define IS32_RESET_REGISTER								0x7F

#define IS32_CONTROL_SSD_SOFTWARE_SHUTDOWN_MODE			0x00
#define IS32_CONTROL_SSD_NORMAL_OPERATION				0x01
#define IS32_CONTROL_PWM_RESOLUTION_8BIT				0x00
#define IS32_CONTROL_PWM_RESOLUTION_10BIT				0x01
#define IS32_CONTROL_PWM_RESOLUTION_12BIT				0x02
#define IS32_CONTROL_PWM_RESOLUTION_16BIT				0x03
#define IS32_CONTROL_OSC_FREQUENCY_16MHZ				0x00
#define IS32_CONTROL_OSC_FREQUENCY_8MHZ					0x01
#define IS32_CONTROL_OSC_FREQUENCY_1MHZ					0x02
#define IS32_CONTROL_OSC_FREQUENCY_500KHZ				0x03
#define IS32_CONTROL_OSC_FREQUENCY_250KHZ				0x04
#define IS32_CONTROL_OSC_FREQUENCY_125KHZ				0x05
#define IS32_CONTROL_OSC_FREQUENCY_62KHZ				0x06
#define IS32_CONTROL_OSC_FREQUENCY_31KHZ				0x07
#define IS32_PHASE_DELAY_DISABLE						0x00
#define IS32_PHASE_DELAY_ENABLE							0x01
#define IS32_OPENSHORT_DETECT_DISABLE					0x00
#define IS32_SHORT_DETECT_ENABLE						0x02
#define IS32_OPEN_DETECT_ENABLE							0x03
#define IS32_SPREADSPECTRUM_DCPWM_CH1_12_DISABLE		0x00
#define IS32_SPREADSPECTRUM_DCPWM_CH1_12_ENABLE			0x01
#define IS32_SPREADSPECTRUM_DCPWM_CH13_24_DISABLE		0x00
#define IS32_SPREADSPECTRUM_DCPWM_CH13_24_ENABLE		0x01
#define IS32_SPREADSPECTRUM_DCPWM_CH25_36_DISABLE		0x00
#define IS32_SPREADSPECTRUM_DCPWM_CH25_36_ENABLE		0x01
#define IS32_SPREADSPECTRUM_SSP_DISABLE					0x00
#define IS32_SPREADSPECTRUM_SSP_ENABLE					0x01
#define IS32_SPREADSPECTRUM_CLT_1980US					0x00
#define IS32_SPREADSPECTRUM_CLT_1200US					0x01
#define IS32_SPREADSPECTRUM_CLT_820US					0x02
#define IS32_SPREADSPECTRUM_CLT_660US					0x03
#define IS32_SPREADSPECTRUM_RNG_5PCT					0x00
#define IS32_SPREADSPECTRUM_RNG_15PCT					0x01
#define IS32_SPREADSPECTRUM_RNG_24PCT					0x02
#define IS32_SPREADSPECTRUM_RNG_34PCT					0x03
#define IS32_SLAVE_ADDRESS								0x34	//7 bit Address (has to be shifted 1 left to add R/W bit)
#define IS32_ISET_VALUE											//In mA - This value is set by R_ISET Resistor per Hardware 



void IS32_Init(void);
void IS32_WriteRegister(uint8_t reg, uint8_t value);
uint8_t IS32_ReadRegister(uint8_t reg);
void IS32_SetControlRegister(uint8_t osc, uint8_t pms, uint8_t ssd);	
void IS32_SetPWM(uint8_t channel, uint16_t pwm);
void IS32_Update(void);
void IS32_SetScaling(uint8_t channel, uint8_t value);
void IS32_SetGlobalCurrentRegister(uint8_t value);
uint8_t IS32_GetGlobalCurrentRegister(void);
void IS32_SetPhaseDelayAndClockPhase(uint8_t phase_delay, uint8_t clock_phase_select);
void IS32_SetOpenShortDetectEnableRegister(uint8_t value);
void IS32_GetOpenShort(uint8_t* channels[5]);
uint8_t IS32_GetTemperatureSensorRegister(void);
void IS32_SetSpreadSpectrumRegister(uint8_t dcpwm_ch1_12, uint8_t dcpwm_ch13_24, uint8_t dcpwm_ch25_36, uint8_t ssp, uint8_t rng, uint8_t clt);
void IS32_Reset(void);
void IS32_setRGB(uint8_t led, uint8_t red, uint8_t green, uint8_t blue);






















/*

Test Driver for Texas Instruments MCF8316A FOC Motor CHIP.

It uses I2C Bus to read/write MCF8316A Registers, so this Read/Write
Function through I2C must be made available or overwritten with the
funtional I2C Driver. 
I2C Freq: 100 kHz Standard Mode, 400 kHz Fast Mode

*/


/*  INCLUDE HEADER FILES    */
#include <stdint.h>
#include <stdbool.h>


/*  REGISTER ADDRESSES  */
//ALGORITHM CONFIGURATION REGISTERS
#define MCF8316A_REG_ADDR_ISD_CONFIG                    0x80
#define MCF8316A_REG_ADDR_REV_DRIVE_CONFIG              0x82
#define MCF8316A_REG_ADDR_MOTOR_STARTUP1                0x84
#define MCF8316A_REG_ADDR_MOTOR_STARTUP2                0x86
#define MCF8316A_REG_ADDR_CLOSED_LOOP1                  0x88
#define MCF8316A_REG_ADDR_CLOSED_LOOP2                  0x8A
#define MCF8316A_REG_ADDR_CLOSED_LOOP3                  0x8C
#define MCF8316A_REG_ADDR_CLOSED_LOOP4                  0x8E
#define MCF8316A_REG_ADDR_SPEED_PROFILES1               0x94
#define MCF8316A_REG_ADDR_SPEED_PROFILES2               0x96
#define MCF8316A_REG_ADDR_SPEED_PROFILES3               0x98
#define MCF8316A_REG_ADDR_SPEED_PROFILES4               0x9A
#define MCF8316A_REG_ADDR_SPEED_PROFILES5               0x9C
#define MCF8316A_REG_ADDR_SPEED_PROFILES6               0x9E

//FAULT CONFIGURATION REGISTERS
#define MCF8316A_REG_ADDR_FAULT_CONFIG1                 0x90
#define MCF8316A_REG_ADDR_FAULT_CONFIG2                 0x92

//HARDWARE CONFIGURATION REGISTERS
#define MCF8316A_REG_ADDR_PIN_CONFIG                    0xA4
#define MCF8316A_REG_ADDR_DEVICE_CONFIG1                0xA6
#define MCF8316A_REG_ADDR_DEVICE_CONFIG2                0xA8
#define MCF8316A_REG_ADDR_PERI_CONFIG                   0xAA
#define MCF8316A_REG_ADDR_GD_CONFIG1                    0xAC
#define MCF8316A_REG_ADDR_GD_CONFIG2                    0xAE

//INTERNAL ALGORITHM CONFIGURATION REGISTERS
#define MCF8316A_REG_ADDR_INT_AGLO1                     0xA0
#define MCF8316A_REG_ADDR_INT_ALGO2                     0xA2

//FAULT STATUS REGISTERS
#define MCF8316A_REG_ADDR_GATE_DRIVER_FAULT_STATUS      0xE0
#define MCF8316A_REG_ADDR_CONTROLLER_FAULT_STATUS       0xE2

//SYSTEM STATUS REGISTERS
#define MCF8316A_REG_ADDR_ALGO_STATUS                   0xE4
#define MCF8316A_REG_ADDR_MTR_PARAMS                    0xE6
#define MCF8316A_REG_ADDR_ALGO_STATUS_MPET              0xE8

//DEVICE CONTROL REGISTERS
#define MCF8316A_REG_ADDR_DEV_CTRL                      0xEA

//ALGORITHM CONTROL REGISTERS
#define MCF8316A_REG_ADDR_ALGO_CTRL1                    0xEC
#define MCF8316A_REG_ADDR_ALGO_CTRL2                    0xEE
#define MCF8316A_REG_ADDR_CURRENT_PI                    0xF0
#define MCF8316A_REG_ADDR_SPEED_PI                      0xF2

//ALGORITHM VARIABLES REGISTERS
#define MCF8316A_REG_ADDR_ALGORITHM_STATE               0x210
#define MCF8316A_REG_ADDR_FG_SPEED_FDBK                 0x216
#define MCF8316A_REG_ADDR_BUS_CURRENT                   0x410
#define MCF8316A_REG_ADDR_PHASE_CURRENT_A               0x43E
#define MCF8316A_REG_ADDR_PHASE_CURRENT_B               0x440
#define MCF8316A_REG_ADDR_PHASE_CURRENT_C               0x442
#define MCF8316A_REG_ADDR_CSA_GAIN_FEEDBACK             0x466
#define MCF8316A_REG_ADDR_VOLTAGE_GAIN_FEEDBACK         0x476
#define MCF8316A_REG_ADDR_VM_VOLTAGE                    0x478
#define MCF8316A_REG_ADDR_PHASE_VOLTAGE_VA              0x47E
#define MCF8316A_REG_ADDR_PHASE_VOLTAGE_VB              0x480
#define MCF8316A_REG_ADDR_PHASE_VOLTAGE_VC              0x482
#define MCF8316A_REG_ADDR_SIN_COMMUTATION_ANGLE         0x4BA
#define MCF8316A_REG_ADDR_COS_COMMUTATION_ANGLE         0x4BC
#define MCF8316A_REG_ADDR_IALPHA                        0x4D4
#define MCF8316A_REG_ADDR_IBETA                         0x4D6
#define MCF8316A_REG_ADDR_VALPHA                        0x4D8
#define MCF8316A_REG_ADDR_VBETA                         0x4DA
#define MCF8316A_REG_ADDR_ID                            0x4E4
#define MCF8316A_REG_ADDR_IQ                            0x4E6
#define MCF8316A_REG_ADDR_VD                            0x4E8
#define MCF8316A_REG_ADDR_VQ                            0x4EA
#define MCF8316A_REG_ADDR_IQ_REF_ROTOR_ALIGN            0x524
#define MCF8316A_REG_ADDR_SPEED_REF_OPEN_LOOP           0x53A
#define MCF8316A_REG_ADDR_IQ_REF_OPEN_LOOP              0x548
#define MCF8316A_REG_ADDR_SPEED_REF_CLOSED_LOOP         0x5CC
#define MCF8316A_REG_ADDR_ID_REF_CLOSED_LOOP            0x5FC
#define MCF8316A_REG_ADDR_IQ_REF_CLOSED_LOOP            0x5FE
#define MCF8316A_REG_ADDR_ISD_STATE                     0x67A
#define MCF8316A_REG_ADDR_ISD_SPEED                     0x684
#define MCF8316A_REG_ADDR_IPD_STATE                     0x6B8
#define MCF8316A_REG_ADDR_IPD_ANGLE                     0x6FC
#define MCF8316A_REG_ADDR_ED                            0x742
#define MCF8316A_REG_ADDR_EQ                            0x744
#define MCF8316A_REG_ADDR_SPEED_FDBK                    0x752
#define MCF8316A_REG_ADDR_THETA_EST                     0x756

//MOTOR RESISTANCE LOOK-UP TABLE
#define MCF8316A_RES_6mOhm                              0x01
#define MCF8316A_RES_7mOhm                              0x02
#define MCF8316A_RES_8mOhm                              0x03
#define MCF8316A_RES_9mOhm                              0x04
#define MCF8316A_RES_10mOhm                             0x05
#define MCF8316A_RES_11mOhm                             0x06
#define MCF8316A_RES_12mOhm                             0x07
#define MCF8316A_RES_13mOhm                             0x08
#define MCF8316A_RES_14mOhm                             0x09
#define MCF8316A_RES_15mOhm                             0x0A
#define MCF8316A_RES_16mOhm                             0x0B
#define MCF8316A_RES_17mOhm                             0x0C
#define MCF8316A_RES_18mOhm                             0x0D
#define MCF8316A_RES_19mOhm                             0x0E
#define MCF8316A_RES_20mOhm                             0x0F

#define MCF8316A_RES_22mOhm                             0x10
#define MCF8316A_RES_24mOhm                             0x11
#define MCF8316A_RES_26mOhm                             0x12
#define MCF8316A_RES_28mOhm                             0x13
#define MCF8316A_RES_30mOhm                             0x14
#define MCF8316A_RES_32mOhm                             0x15
#define MCF8316A_RES_34mOhm                             0x16
#define MCF8316A_RES_36mOhm                             0x17
#define MCF8316A_RES_38mOhm                             0x18
#define MCF8316A_RES_40mOhm                             0x19
#define MCF8316A_RES_42mOhm                             0x1A
#define MCF8316A_RES_44mOhm                             0x1B
#define MCF8316A_RES_46mOhm                             0x1C
#define MCF8316A_RES_48mOhm                             0x1D
#define MCF8316A_RES_50mOhm                             0x1E
#define MCF8316A_RES_52mOhm                             0x1F

#define MCF8316A_RES_54mOhm                             0x20
#define MCF8316A_RES_56mOhm                             0x21
#define MCF8316A_RES_58mOhm                             0x22
#define MCF8316A_RES_60mOhm                             0x23
#define MCF8316A_RES_62mOhm                             0x24
#define MCF8316A_RES_64mOhm                             0x25
#define MCF8316A_RES_66mOhm                             0x26
#define MCF8316A_RES_68mOhm                             0x27
#define MCF8316A_RES_70mOhm                             0x28
#define MCF8316A_RES_72mOhm                             0x29
#define MCF8316A_RES_74mOhm                             0x2A
#define MCF8316A_RES_76mOhm                             0x2B
#define MCF8316A_RES_78mOhm                             0x2C
#define MCF8316A_RES_80mOhm                             0x2D
#define MCF8316A_RES_82mOhm                             0x2E
#define MCF8316A_RES_84mOhm                             0x2F

#define MCF8316A_RES_86mOhm                             0x30
#define MCF8316A_RES_88mOhm                             0x31
#define MCF8316A_RES_90mOhm                             0x32
#define MCF8316A_RES_92mOhm                             0x33
#define MCF8316A_RES_94mOhm                             0x34
#define MCF8316A_RES_96mOhm                             0x35
#define MCF8316A_RES_98mOhm                             0x36
#define MCF8316A_RES_100mOhm                            0x37
#define MCF8316A_RES_105mOhm                            0x38
#define MCF8316A_RES_110mOhm                            0x39
#define MCF8316A_RES_115mOhm                            0x3A
#define MCF8316A_RES_120mOhm                            0x3B
#define MCF8316A_RES_125mOhm                            0x3C
#define MCF8316A_RES_130mOhm                            0x3D
#define MCF8316A_RES_135mOhm                            0x3E
#define MCF8316A_RES_140mOhm                            0x3F

#define MCF8316A_RES_145mOhm                            0x40
#define MCF8316A_RES_150mOhm                            0x41
#define MCF8316A_RES_155mOhm                            0x42
#define MCF8316A_RES_160mOhm                            0x43
#define MCF8316A_RES_165mOhm                            0x44
#define MCF8316A_RES_170mOhm                            0x45
#define MCF8316A_RES_175mOhm                            0x46
#define MCF8316A_RES_180mOhm                            0x47
#define MCF8316A_RES_185mOhm                            0x48
#define MCF8316A_RES_190mOhm                            0x49
#define MCF8316A_RES_195mOhm                            0x4A
#define MCF8316A_RES_200mOhm                            0x4B
#define MCF8316A_RES_205mOhm                            0x4C
#define MCF8316A_RES_210mOhm                            0x4D
#define MCF8316A_RES_215mOhm                            0x4E
#define MCF8316A_RES_220mOhm                            0x4F

#define MCF8316A_RES_225mOhm                            0x50
#define MCF8316A_RES_230mOhm                            0x51
#define MCF8316A_RES_235mOhm                            0x52
#define MCF8316A_RES_240mOhm                            0x53
#define MCF8316A_RES_245mOhm                            0x54
#define MCF8316A_RES_250mOhm                            0x55
#define MCF8316A_RES_255mOhm                            0x56
#define MCF8316A_RES_260mOhm                            0x57
#define MCF8316A_RES_265mOhm                            0x58
#define MCF8316A_RES_270mOhm                            0x59
#define MCF8316A_RES_275mOhm                            0x5A
#define MCF8316A_RES_280mOhm                            0x5B
#define MCF8316A_RES_285mOhm                            0x5C
#define MCF8316A_RES_290mOhm                            0x5D
#define MCF8316A_RES_295mOhm                            0x5E
#define MCF8316A_RES_300mOhm                            0x5F

#define MCF8316A_RES_305mOhm                            0x60
#define MCF8316A_RES_310mOhm                            0x61
#define MCF8316A_RES_315mOhm                            0x62
#define MCF8316A_RES_320mOhm                            0x63
#define MCF8316A_RES_325mOhm                            0x64
#define MCF8316A_RES_330mOhm                            0x65
#define MCF8316A_RES_335mOhm                            0x66
#define MCF8316A_RES_340mOhm                            0x67
#define MCF8316A_RES_345mOhm                            0x68
#define MCF8316A_RES_350mOhm                            0x69
#define MCF8316A_RES_355mOhm                            0x6A
#define MCF8316A_RES_360mOhm                            0x6B
#define MCF8316A_RES_365mOhm                            0x6C
#define MCF8316A_RES_370mOhm                            0x6D
#define MCF8316A_RES_375mOhm                            0x6E
#define MCF8316A_RES_380mOhm                            0x6F

#define MCF8316A_RES_385mOhm                            0x70
#define MCF8316A_RES_390mOhm                            0x71
#define MCF8316A_RES_395mOhm                            0x72
#define MCF8316A_RES_400mOhm                            0x73
#define MCF8316A_RES_405mOhm                            0x74
#define MCF8316A_RES_410mOhm                            0x75
#define MCF8316A_RES_415mOhm                            0x76
#define MCF8316A_RES_420mOhm                            0x77
#define MCF8316A_RES_425mOhm                            0x78
#define MCF8316A_RES_430mOhm                            0x79
#define MCF8316A_RES_435mOhm                            0x7A
#define MCF8316A_RES_440mOhm                            0x7B
#define MCF8316A_RES_445mOhm                            0x7C
#define MCF8316A_RES_450mOhm                            0x7D
#define MCF8316A_RES_455mOhm                            0x7E
#define MCF8316A_RES_460mOhm                            0x7F

#define MCF8316A_RES_465mOhm                            0x80
#define MCF8316A_RES_470mOhm                            0x81
#define MCF8316A_RES_475mOhm                            0x82
#define MCF8316A_RES_480mOhm                            0x83
#define MCF8316A_RES_485mOhm                            0x84
#define MCF8316A_RES_490mOhm                            0x85
#define MCF8316A_RES_495mOhm                            0x86
#define MCF8316A_RES_500mOhm                            0x87
#define MCF8316A_RES_510mOhm                            0x88
#define MCF8316A_RES_520mOhm                            0x89
#define MCF8316A_RES_530mOhm                            0x8A
#define MCF8316A_RES_540mOhm                            0x8B
#define MCF8316A_RES_550mOhm                            0x8C
#define MCF8316A_RES_560mOhm                            0x8D
#define MCF8316A_RES_570mOhm                            0x8E
#define MCF8316A_RES_580mOhm                            0x8F

#define MCF8316A_RES_590mOhm                            0x90
#define MCF8316A_RES_600mOhm                            0x91
#define MCF8316A_RES_610mOhm                            0x92
#define MCF8316A_RES_620mOhm                            0x93
#define MCF8316A_RES_630mOhm                            0x94
#define MCF8316A_RES_640mOhm                            0x95
#define MCF8316A_RES_650mOhm                            0x96
#define MCF8316A_RES_660mOhm                            0x97
#define MCF8316A_RES_670mOhm                            0x98
#define MCF8316A_RES_680mOhm                            0x99
#define MCF8316A_RES_690mOhm                            0x9A
#define MCF8316A_RES_700mOhm                            0x9B
#define MCF8316A_RES_720mOhm                            0x9C
#define MCF8316A_RES_740mOhm                            0x9D
#define MCF8316A_RES_760mOhm                            0x9E
#define MCF8316A_RES_780mOhm                            0x9F

#define MCF8316A_RES_800mOhm                            0xA0
#define MCF8316A_RES_820mOhm                            0xA1
#define MCF8316A_RES_840mOhm                            0xA2
#define MCF8316A_RES_860mOhm                            0xA3
#define MCF8316A_RES_880mOhm                            0xA4
#define MCF8316A_RES_900mOhm                            0xA5
#define MCF8316A_RES_920mOhm                            0xA6
#define MCF8316A_RES_940mOhm                            0xA7
#define MCF8316A_RES_960mOhm                            0xA8
#define MCF8316A_RES_980mOhm                            0xA9
#define MCF8316A_RES_1000mOhm                           0xAA
#define MCF8316A_RES_1050mOhm                           0xAB
#define MCF8316A_RES_1100mOhm                           0xAC
#define MCF8316A_RES_1150mOhm                           0xAD
#define MCF8316A_RES_1200mOhm                           0xAE
#define MCF8316A_RES_1250mOhm                           0xAF

#define MCF8316A_RES_1300mOhm                           0xB0
#define MCF8316A_RES_1350mOhm                           0xB1
#define MCF8316A_RES_1400mOhm                           0xB2
#define MCF8316A_RES_1450mOhm                           0xB3
#define MCF8316A_RES_1500mOhm                           0xB4
#define MCF8316A_RES_1550mOhm                           0xB5
#define MCF8316A_RES_1600mOhm                           0xB6
#define MCF8316A_RES_1650mOhm                           0xB7
#define MCF8316A_RES_1700mOhm                           0xB8
#define MCF8316A_RES_1750mOhm                           0xB9
#define MCF8316A_RES_1800mOhm                           0xBA
#define MCF8316A_RES_1850mOhm                           0xBB
#define MCF8316A_RES_1900mOhm                           0xBC
#define MCF8316A_RES_1950mOhm                           0xBD
#define MCF8316A_RES_2000mOhm                           0xBE
#define MCF8316A_RES_2050mOhm                           0xBF

#define MCF8316A_RES_2100mOhm                           0xC0
#define MCF8316A_RES_2200mOhm                           0xC1
#define MCF8316A_RES_2300mOhm                           0xC2
#define MCF8316A_RES_2400mOhm                           0xC3
#define MCF8316A_RES_2500mOhm                           0xC4
#define MCF8316A_RES_2600mOhm                           0xC5
#define MCF8316A_RES_2700mOhm                           0xC6
#define MCF8316A_RES_2800mOhm                           0xC7
#define MCF8316A_RES_2900mOhm                           0xC8
#define MCF8316A_RES_3000mOhm                           0xC9
#define MCF8316A_RES_3200mOhm                           0xCA
#define MCF8316A_RES_3400mOhm                           0xCB
#define MCF8316A_RES_3600mOhm                           0xCC
#define MCF8316A_RES_3800mOhm                           0xCD
#define MCF8316A_RES_4000mOhm                           0xCE
#define MCF8316A_RES_4200mOhm                           0xCF

#define MCF8316A_RES_4400mOhm                           0xD0
#define MCF8316A_RES_4600mOhm                           0xD1
#define MCF8316A_RES_4800mOhm                           0xD2
#define MCF8316A_RES_5000mOhm                           0xD3
#define MCF8316A_RES_5200mOhm                           0xD4
#define MCF8316A_RES_5400mOhm                           0xD5
#define MCF8316A_RES_5600mOhm                           0xD6
#define MCF8316A_RES_5800mOhm                           0xD7
#define MCF8316A_RES_6000mOhm                           0xD8
#define MCF8316A_RES_6200mOhm                           0xD9
#define MCF8316A_RES_6400mOhm                           0xDA
#define MCF8316A_RES_6600mOhm                           0xDB
#define MCF8316A_RES_6800mOhm                           0xDC
#define MCF8316A_RES_7000mOhm                           0xDD
#define MCF8316A_RES_7200mOhm                           0xDE
#define MCF8316A_RES_7400mOhm                           0xDF

#define MCF8316A_RES_7600mOhm                           0xE0
#define MCF8316A_RES_7800mOhm                           0xE1
#define MCF8316A_RES_8000mOhm                           0xE2
#define MCF8316A_RES_8200mOhm                           0xE3
#define MCF8316A_RES_8400mOhm                           0xE4
#define MCF8316A_RES_8600mOhm                           0xE5
#define MCF8316A_RES_8800mOhm                           0xE6
#define MCF8316A_RES_9000mOhm                           0xE7
#define MCF8316A_RES_9200mOhm                           0xE8
#define MCF8316A_RES_9400mOhm                           0xE9
#define MCF8316A_RES_9600mOhm                           0xEA
#define MCF8316A_RES_9800mOhm                           0xEB
#define MCF8316A_RES_10000mOhm                          0xEC
#define MCF8316A_RES_10500mOhm                          0xED
#define MCF8316A_RES_11000mOhm                          0xEE
#define MCF8316A_RES_11500mOhm                          0xEF

#define MCF8316A_RES_12000mOhm                          0xF0
#define MCF8316A_RES_12500mOhm                          0xF1
#define MCF8316A_RES_13000mOhm                          0xF2
#define MCF8316A_RES_13500mOhm                          0xF3
#define MCF8316A_RES_14000mOhm                          0xF4
#define MCF8316A_RES_14500mOhm                          0xF5
#define MCF8316A_RES_15000mOhm                          0xF6
#define MCF8316A_RES_15500mOhm                          0xF7
#define MCF8316A_RES_16000mOhm                          0xF8
#define MCF8316A_RES_16500mOhm                          0xF9
#define MCF8316A_RES_17000mOhm                          0xFA
#define MCF8316A_RES_17500mOhm                          0xFB
#define MCF8316A_RES_18000mOhm                          0xFC
#define MCF8316A_RES_18500mOhm                          0xFD
#define MCF8316A_RES_19000mOhm                          0xFE
#define MCF8316A_RES_20000mOhm                          0xFF


void MCF8316A_Init(void);
void MCF8316A_WriteRegister(uint32_t reg, uint8_t value);
uint8_t MCF8316A_ReadRegister(uint32_t reg);
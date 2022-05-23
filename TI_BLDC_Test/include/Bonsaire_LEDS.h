#include <stdint.h>
#include <stdbool.h>


//Bonsaire LEDS Function ENUM
typedef enum {
    BLINK_LEDS, 
    DIMM_UP, 
    DIMM_DOWN,
    STAR_1,
    STAR_2,
    BONSAIRE_1
}bonsaire_leds_function_t;

void Bonsaire_LEDS_Init(void);
void Bonsaire_LEDS_TurnAllOn(uint8_t red, uint8_t green, uint8_t blue);
void Bonsaire_LEDS_TurnAllOff(void);
void Bonsaire_LEDS_BlinkLeds(uint8_t leds_rgb[12][3], uint16_t interval_ms);
void Bonsaire_LEDS_DimmUp(bool red, bool green, bool blue, uint16_t interval_ms, uint8_t steps);
void Bonsaire_LEDS_DimmDown(bool red, bool green, bool blue, uint16_t interval_ms, uint8_t steps);
void Bonsaire_LEDS_Star1(uint8_t red, uint8_t green, uint8_t blue, uint16_t interval_ms);
void Bonsaire_LEDS_Star2(uint8_t red, uint8_t green, uint8_t blue, uint16_t interval_ms);
void Bonsaire_LEDS_Bonsaire1(uint8_t rounds, uint16_t interval_ms);
void Bonsaire_LEDS_Run(void);
void Bonsaire_LEDS_Blink_Function(void);
void Bonsaire_LEDS_Star1_Function(void);
void Bonsaire_LEDS_Star2_Function(void);
void Bonsaire_LEDS_DimmUp_Function(void);
void Bonsaire_LEDS_DimmDown_Function(void);
void Bonsaire_LEDS_Bonsaire1_Function(void);

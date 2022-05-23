#include <stdint.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "IS32.h"
#include "Bonsaire_LEDS.h"


uint8_t bonsaire_leds_run_state = 0;
uint8_t bonsaire_leds_change_run_state = 0;
uint8_t bonsaire_leds_function_run = 0;
TimerHandle_t bonsaire_leds_run_timer;
uint8_t bonsaire_leds_blink_state = 0;
uint8_t bonsaire_leds_dimmUp_state = 0;
uint8_t bonsaire_leds_dimmDown_state = 0;
uint8_t bonsaire_leds_star1_state = 0;
uint8_t bonsaire_leds_star2_state = 0;
uint8_t bonsaire_leds_bonsaire1_state = 0;
uint8_t bonsaire_red_state = 0;
uint8_t bonsaire_green_state = 0;
uint8_t bonsaire_blue_state = 0;
uint8_t bonsaire_dimm_steps = 1;
uint8_t bonsaire_rgb_state[12][3] = {{0}};
uint8_t bonsaire_rounds = 1;
uint8_t bonsaire_rounds_made = 0;



void vbonsaireLedsTimersCallback(TimerHandle_t xTimer)
{
    if(pvTimerGetTimerID(xTimer) == pvTimerGetTimerID(bonsaire_leds_run_timer))
    {
        bonsaire_leds_change_run_state = 1;
    }
}

void Bonsaire_LEDS_Init(void){
    IS32_Init();
    bonsaire_leds_run_timer = xTimerCreate("Timer", pdMS_TO_TICKS(1000), pdTRUE, (void *) 0, vbonsaireLedsTimersCallback);
}

void Bonsaire_LEDS_TurnAllOn(uint8_t red, uint8_t green, uint8_t blue){
    bonsaire_leds_run_state = 0;
    xTimerStop(bonsaire_leds_run_timer, 0);
    for(uint8_t i = 1; i <= 12; i++){
        IS32_setRGB(i, red, green, blue);
    }
}

void Bonsaire_LEDS_TurnAllOff(void){
    bonsaire_leds_run_state = 0;
    xTimerStop(bonsaire_leds_run_timer, 0);
    for(uint8_t i = 1; i <= 12; i++){
        IS32_setRGB(i, 0, 0, 0);
    }
}

void Bonsaire_LEDS_BlinkLeds(uint8_t leds_rgb[12][3], uint16_t interval_ms){
    Bonsaire_LEDS_TurnAllOff();
    for(uint8_t i = 0; i < 12; i++){
        for(uint8_t j = 0; j < 3; j++){
            bonsaire_rgb_state[i][j] = leds_rgb[i][j];
        }
    }
    bonsaire_leds_function_run = BLINK_LEDS;
    xTimerChangePeriod(bonsaire_leds_run_timer, pdMS_TO_TICKS(interval_ms), 0);
    xTimerStart(bonsaire_leds_run_timer, 0);
    bonsaire_leds_blink_state = 0;
    bonsaire_leds_run_state = 1; 
}

void Bonsaire_LEDS_DimmUp(bool red, bool green, bool blue, uint16_t interval_ms, uint8_t steps){
    Bonsaire_LEDS_TurnAllOff();
    bonsaire_red_state = (uint8_t) red;
    bonsaire_green_state = (uint8_t) green;
    bonsaire_blue_state = (uint8_t) blue;
    bonsaire_dimm_steps = steps;
    if(bonsaire_dimm_steps > 20){
        bonsaire_dimm_steps = 20;
    }
    if(bonsaire_dimm_steps == 0){
        bonsaire_dimm_steps = 1;
    }
    bonsaire_leds_function_run = DIMM_UP;
    xTimerChangePeriod(bonsaire_leds_run_timer, pdMS_TO_TICKS(interval_ms), 0);
    xTimerStart(bonsaire_leds_run_timer, 0);
    bonsaire_leds_dimmUp_state = 0;
    bonsaire_leds_run_state = 1; 
}

void Bonsaire_LEDS_DimmDown(bool red, bool green, bool blue, uint16_t interval_ms, uint8_t steps){
    Bonsaire_LEDS_TurnAllOff();
    bonsaire_red_state = (uint8_t) red;
    bonsaire_green_state = (uint8_t) green;
    bonsaire_blue_state = (uint8_t) blue;
    bonsaire_dimm_steps = steps;
     if(bonsaire_dimm_steps > 20){
        bonsaire_dimm_steps = 20;
    }
    if(bonsaire_dimm_steps == 0){
        bonsaire_dimm_steps = 1;
    }
    bonsaire_leds_function_run = DIMM_DOWN;
    xTimerChangePeriod(bonsaire_leds_run_timer, pdMS_TO_TICKS(interval_ms), 0);
    xTimerStart(bonsaire_leds_run_timer, 0);
    bonsaire_leds_dimmDown_state = 255;
    bonsaire_leds_run_state = 1; 
}

void Bonsaire_LEDS_Star1(uint8_t red, uint8_t green, uint8_t blue, uint16_t interval_ms){
    Bonsaire_LEDS_TurnAllOff();
    bonsaire_red_state = red;
    bonsaire_green_state = green;
    bonsaire_blue_state = blue;
    bonsaire_leds_function_run = STAR_1;
    xTimerChangePeriod(bonsaire_leds_run_timer, pdMS_TO_TICKS(interval_ms), 0);
    xTimerStart(bonsaire_leds_run_timer, 0);
    bonsaire_leds_star1_state = 0;
    bonsaire_leds_run_state = 1;
}

void Bonsaire_LEDS_Star2(uint8_t red, uint8_t green, uint8_t blue, uint16_t interval_ms){
    Bonsaire_LEDS_TurnAllOff();
    bonsaire_red_state = red;
    bonsaire_green_state = green;
    bonsaire_blue_state = blue;
    bonsaire_leds_function_run = STAR_2;
    xTimerChangePeriod(bonsaire_leds_run_timer, pdMS_TO_TICKS(interval_ms), 0);
    xTimerStart(bonsaire_leds_run_timer, 0);
    bonsaire_leds_star2_state = 0;
    bonsaire_leds_run_state = 1;
}

void Bonsaire_LEDS_Bonsaire1(uint8_t rounds, uint16_t interval_ms){
    Bonsaire_LEDS_TurnAllOff();
    bonsaire_rounds = rounds;
    bonsaire_rounds_made = 0;
    bonsaire_leds_function_run = BONSAIRE_1;
    xTimerChangePeriod(bonsaire_leds_run_timer, pdMS_TO_TICKS(interval_ms), 0);
    xTimerStart(bonsaire_leds_run_timer, 0);
    bonsaire_leds_bonsaire1_state = 0;
    bonsaire_leds_run_state = 1;
}

void Bonsaire_LEDS_Run(void)
{
    //Run if state is true
    if(bonsaire_leds_run_state){
        switch(bonsaire_leds_function_run)
        {
            case BLINK_LEDS: 
                Bonsaire_LEDS_Blink_Function();
            break;

            case DIMM_UP: 
                Bonsaire_LEDS_DimmUp_Function();
            break;

            case DIMM_DOWN: 
                Bonsaire_LEDS_DimmDown_Function();
            break;

            case STAR_1: 
                Bonsaire_LEDS_Star1_Function();
            break;

            case STAR_2: 
                Bonsaire_LEDS_Star2_Function();
            break;

            case BONSAIRE_1: 
                Bonsaire_LEDS_Bonsaire1_Function();
            break;

            default:
                Bonsaire_LEDS_TurnAllOff();
        }
    }
}

void Bonsaire_LEDS_Blink_Function(void){
    uint8_t i = 0;
    if(bonsaire_leds_change_run_state){
        bonsaire_leds_change_run_state = 0;
        if(bonsaire_leds_blink_state){
           for(i = 1; i <= 12; i++){
                IS32_setRGB(i, 0, 0, 0);
            } 
            bonsaire_leds_blink_state = 0;
        }
        else{
            for(i = 0; i < 12; i++){
                IS32_setRGB((i + 1), bonsaire_rgb_state[i][0], bonsaire_rgb_state[i][1], bonsaire_rgb_state[i][2]);
            }
            bonsaire_leds_blink_state = 1;
        }
    }
}

void Bonsaire_LEDS_Star1_Function(void){
    if(bonsaire_leds_change_run_state){
        bonsaire_leds_change_run_state = 0;
        switch(bonsaire_leds_star1_state){
            case 0:
                IS32_setRGB(5, 0, 0, 0);
                IS32_setRGB(6, 0, 0, 0);
                IS32_setRGB(2, bonsaire_red_state, bonsaire_green_state, bonsaire_blue_state);
                IS32_setRGB(4, bonsaire_red_state, bonsaire_green_state, bonsaire_blue_state);
                bonsaire_leds_star1_state++;
            break;

            case 1:
                IS32_setRGB(2, 0, 0, 0);
                IS32_setRGB(4, 0, 0, 0);
                IS32_setRGB(1, bonsaire_red_state, bonsaire_green_state, bonsaire_blue_state);
                IS32_setRGB(3, bonsaire_red_state, bonsaire_green_state, bonsaire_blue_state);
                bonsaire_leds_star1_state++;
            break;

            case 2:
                IS32_setRGB(1, 0, 0, 0);
                IS32_setRGB(3, 0, 0, 0);
                IS32_setRGB(7, bonsaire_red_state, bonsaire_green_state, bonsaire_blue_state);
                IS32_setRGB(12, bonsaire_red_state, bonsaire_green_state, bonsaire_blue_state);
                bonsaire_leds_star1_state++;
            break;

            case 3:
                IS32_setRGB(7, 0, 0, 0);
                IS32_setRGB(12, 0, 0, 0);
                IS32_setRGB(8, bonsaire_red_state, bonsaire_green_state, bonsaire_blue_state);
                IS32_setRGB(11, bonsaire_red_state, bonsaire_green_state, bonsaire_blue_state);
                bonsaire_leds_star1_state++;
            break;

            case 4:
                IS32_setRGB(8, 0, 0, 0);
                IS32_setRGB(11, 0, 0, 0);
                IS32_setRGB(9, bonsaire_red_state, bonsaire_green_state, bonsaire_blue_state);
                IS32_setRGB(10, bonsaire_red_state, bonsaire_green_state, bonsaire_blue_state);
                bonsaire_leds_star1_state++;
            break;

            case 5:
                IS32_setRGB(9, 0, 0, 0);
                IS32_setRGB(10, 0, 0, 0);
                IS32_setRGB(5, bonsaire_red_state, bonsaire_green_state, bonsaire_blue_state);
                IS32_setRGB(6, bonsaire_red_state, bonsaire_green_state, bonsaire_blue_state);
                bonsaire_leds_star1_state = 0;
            break;

            default:
                bonsaire_leds_star1_state = 0;

        }
    }
}

void Bonsaire_LEDS_Star2_Function(void){
    if(bonsaire_leds_change_run_state){
        bonsaire_leds_change_run_state = 0;
        switch(bonsaire_leds_star2_state){
            case 0:
                IS32_setRGB(1, 0, 0, 0);
                IS32_setRGB(3, 0, 0, 0);
                IS32_setRGB(2, bonsaire_red_state, bonsaire_green_state, bonsaire_blue_state);
                IS32_setRGB(4, bonsaire_red_state, bonsaire_green_state, bonsaire_blue_state);
                bonsaire_leds_star2_state++;
            break;

            case 1:
                IS32_setRGB(2, 0, 0, 0);
                IS32_setRGB(4, 0, 0, 0);
                IS32_setRGB(5, bonsaire_red_state, bonsaire_green_state, bonsaire_blue_state);
                IS32_setRGB(6, bonsaire_red_state, bonsaire_green_state, bonsaire_blue_state);
                bonsaire_leds_star2_state++;
            break;

            case 2:
                IS32_setRGB(5, 0, 0, 0);
                IS32_setRGB(6, 0, 0, 0);
                IS32_setRGB(9, bonsaire_red_state, bonsaire_green_state, bonsaire_blue_state);
                IS32_setRGB(10, bonsaire_red_state, bonsaire_green_state, bonsaire_blue_state);
                bonsaire_leds_star2_state++;
            break;

            case 3:
                IS32_setRGB(9, 0, 0, 0);
                IS32_setRGB(10, 0, 0, 0);
                IS32_setRGB(8, bonsaire_red_state, bonsaire_green_state, bonsaire_blue_state);
                IS32_setRGB(11, bonsaire_red_state, bonsaire_green_state, bonsaire_blue_state);
                bonsaire_leds_star2_state++;
            break;

            case 4:
                IS32_setRGB(8, 0, 0, 0);
                IS32_setRGB(11, 0, 0, 0);
                IS32_setRGB(7, bonsaire_red_state, bonsaire_green_state, bonsaire_blue_state);
                IS32_setRGB(12, bonsaire_red_state, bonsaire_green_state, bonsaire_blue_state);
                bonsaire_leds_star2_state++;
            break;

            case 5:
                IS32_setRGB(7, 0, 0, 0);
                IS32_setRGB(12, 0, 0, 0);
                IS32_setRGB(1, bonsaire_red_state, bonsaire_green_state, bonsaire_blue_state);
                IS32_setRGB(3, bonsaire_red_state, bonsaire_green_state, bonsaire_blue_state);
                bonsaire_leds_star2_state = 0;
            break;

            default:
                bonsaire_leds_star1_state = 0;

        }
    }
}

void Bonsaire_LEDS_DimmUp_Function(void){
    if(bonsaire_leds_change_run_state){
        bonsaire_leds_change_run_state = 0;
        for(uint8_t i = 1; i <=12; i++){
            IS32_setRGB(i, (bonsaire_red_state * bonsaire_leds_dimmUp_state), (bonsaire_green_state * bonsaire_leds_dimmUp_state), (bonsaire_blue_state * bonsaire_leds_dimmUp_state));
        }
        bonsaire_leds_dimmUp_state += bonsaire_dimm_steps;
    }
}

void Bonsaire_LEDS_DimmDown_Function(void){
    if(bonsaire_leds_change_run_state){
        bonsaire_leds_change_run_state = 0;
        for(uint8_t i = 1; i <=12; i++){
            IS32_setRGB(i, (bonsaire_red_state * bonsaire_leds_dimmUp_state), (bonsaire_green_state * bonsaire_leds_dimmUp_state), (bonsaire_blue_state * bonsaire_leds_dimmUp_state));
        }
        bonsaire_leds_dimmUp_state -= bonsaire_dimm_steps;
    }
}

void Bonsaire_LEDS_Bonsaire1_Function(void){
     if(bonsaire_leds_change_run_state){
        bonsaire_leds_change_run_state = 0;
        uint8_t white_border = 0;
        uint8_t purple_air = 200;
        uint8_t blue_air = 200;
        switch(bonsaire_leds_bonsaire1_state){
            case 0:
                IS32_setRGB(1, white_border, white_border, white_border);
                IS32_setRGB(12, white_border, white_border, white_border);
                IS32_setRGB(11, white_border, white_border, white_border);
                IS32_setRGB(10, white_border, white_border, white_border);
                IS32_setRGB(5, white_border, white_border, white_border);

                bonsaire_leds_bonsaire1_state++;
            break;

            case 1:
                IS32_setRGB(2, purple_air, 0, purple_air);
                bonsaire_leds_bonsaire1_state++;
            break;

            case 2:
                IS32_setRGB(2, purple_air, 0, purple_air);
                IS32_setRGB(6, purple_air, 0, purple_air);
                bonsaire_leds_bonsaire1_state++;
            break;

            case 3:
                IS32_setRGB(2, purple_air, 0, purple_air);
                IS32_setRGB(6, purple_air, 0, purple_air);
                IS32_setRGB(9, purple_air, 0, purple_air);
                bonsaire_leds_bonsaire1_state++;
            break;

            case 4:
                IS32_setRGB(2, 0, 0, 0);
                IS32_setRGB(6, purple_air, 0, purple_air);
                IS32_setRGB(9, purple_air, 0, purple_air);
                IS32_setRGB(8, purple_air, 0, purple_air);
                IS32_setRGB(7, 0, 0, 0);
                IS32_setRGB(3, 0, 0, 0);
                IS32_setRGB(4, 0, 0, 0);
                bonsaire_leds_bonsaire1_state++;
            break;

            case 5:
                IS32_setRGB(6, 0, 0, 0);
                IS32_setRGB(9, purple_air, 0, purple_air);
                IS32_setRGB(8, purple_air, 0, purple_air);
                IS32_setRGB(7, purple_air, 0, purple_air);
                IS32_setRGB(3, 0, 0, 0);
                IS32_setRGB(4, 0, 0, 0);
                bonsaire_leds_bonsaire1_state++;
            break;

            case 6:
                IS32_setRGB(6, 0, 0, 0);
                IS32_setRGB(9, 0, 0, 0);
                IS32_setRGB(8, purple_air, 0, purple_air);
                IS32_setRGB(7, purple_air, 0, purple_air);
                IS32_setRGB(3, purple_air, 0, purple_air);
                IS32_setRGB(4, 0, 0, 0);
                bonsaire_leds_bonsaire1_state++;
            break;

            case 7:
                IS32_setRGB(6, 0, 0, 0);
                IS32_setRGB(9, 0, 0, 0);
                IS32_setRGB(8, 0, 0, 0);
                IS32_setRGB(7, purple_air, 0, purple_air);
                IS32_setRGB(3, purple_air, 0, purple_air);
                IS32_setRGB(4, purple_air, 0, purple_air);
                bonsaire_leds_bonsaire1_state++;
            break;

            case 8:
                IS32_setRGB(6, purple_air, 0, purple_air);
                IS32_setRGB(9, 0, 0, 0);
                IS32_setRGB(8, 0, 0, 0);
                IS32_setRGB(7, 0, 0, 0);
                IS32_setRGB(3, purple_air, 0, purple_air);
                IS32_setRGB(4, purple_air, 0, purple_air);
                bonsaire_leds_bonsaire1_state++;
            break;

            case 9:
                IS32_setRGB(6, purple_air, 0, purple_air);
                IS32_setRGB(9, purple_air, 0, purple_air);
                IS32_setRGB(8, 0, 0, 0);
                IS32_setRGB(7, 0, 0, 0);
                IS32_setRGB(3, 0, 0, 0);
                IS32_setRGB(4, purple_air, 0, purple_air);
                bonsaire_rounds_made++;
                if(bonsaire_rounds_made >= bonsaire_rounds){
                    bonsaire_rounds_made = 0;
                    bonsaire_leds_bonsaire1_state++;
                }
                else{
                    bonsaire_leds_bonsaire1_state = 4;
                }
            break;

             case 10:
                IS32_setRGB(2, 0, 0, 0);
                IS32_setRGB(6, purple_air, 0, purple_air);
                IS32_setRGB(9, purple_air, 0, purple_air);
                IS32_setRGB(8, 0, 0, blue_air);
                IS32_setRGB(7, 0, 0, 0);
                IS32_setRGB(3, 0, 0, 0);
                IS32_setRGB(4, 0, 0, 0);
                bonsaire_leds_bonsaire1_state++;
            break;

            case 11: 
                IS32_setRGB(6, 0, 0, 0);
                IS32_setRGB(9, purple_air, 0, purple_air);
                IS32_setRGB(8, 0, 0, blue_air);
                IS32_setRGB(7, 0, 0, blue_air);
                IS32_setRGB(3, 0, 0, 0);
                IS32_setRGB(4, 0, 0, 0);
                bonsaire_leds_bonsaire1_state++;
            break;

            case 12:
                IS32_setRGB(6, 0, 0, 0);
                IS32_setRGB(9, 0, 0, 0);
                IS32_setRGB(8, 0, 0, blue_air);
                IS32_setRGB(7, 0, 0, blue_air);
                IS32_setRGB(3, 0, 0, blue_air);
                IS32_setRGB(4, 0, 0, 0);
                bonsaire_leds_bonsaire1_state++;
            break;

            case 13:
                IS32_setRGB(6, 0, 0, 0);
                IS32_setRGB(9, 0, 0, 0);
                IS32_setRGB(8, 0, 0, 0);
                IS32_setRGB(7, 0, 0, blue_air);
                IS32_setRGB(3, 0, 0, blue_air);
                IS32_setRGB(4, 0, 0, blue_air);
                bonsaire_leds_bonsaire1_state++;
            break;

            case 14:
                IS32_setRGB(6, 0, 0, blue_air);
                IS32_setRGB(9, 0, 0, 0);
                IS32_setRGB(8, 0, 0, 0);
                IS32_setRGB(7, 0, 0, 0);
                IS32_setRGB(3, 0, 0, blue_air);
                IS32_setRGB(4, 0, 0, blue_air);
                bonsaire_leds_bonsaire1_state++;
            break;

            case 15:
                IS32_setRGB(6, 0, 0, blue_air);
                IS32_setRGB(9, 0, 0, blue_air);
                IS32_setRGB(8, 0, 0, 0);
                IS32_setRGB(7, 0, 0, 0);
                IS32_setRGB(3, 0, 0, 0);
                IS32_setRGB(4, 0, 0, blue_air);
                bonsaire_leds_bonsaire1_state++;
            break;

            case 16:
                IS32_setRGB(6, 0, 0, blue_air);
                IS32_setRGB(9, 0, 0, blue_air);
                IS32_setRGB(8, 0, 0, blue_air);
                IS32_setRGB(7, 0, 0, 0);
                IS32_setRGB(3, 0, 0, 0);
                IS32_setRGB(4, 0, 0, 0);
                bonsaire_leds_bonsaire1_state++;
            break;

            case 17:
                IS32_setRGB(6, 0, 0, 0);
                IS32_setRGB(9, 0, 0, blue_air);
                IS32_setRGB(8, 0, 0, blue_air);
                IS32_setRGB(7, 0, 0, blue_air);
                IS32_setRGB(3, 0, 0, 0);
                IS32_setRGB(4, 0, 0, 0);
                bonsaire_rounds_made++;
                if(bonsaire_rounds_made >= bonsaire_rounds){
                    bonsaire_rounds_made = 0;
                    bonsaire_leds_bonsaire1_state++;
                }
                else{
                    bonsaire_leds_bonsaire1_state = 12;
                }
            break;

             case 18:
                bonsaire_leds_bonsaire1_state++;
            break;

             case 19:
                IS32_setRGB(9, 0, 0, 0);
                IS32_setRGB(8, 0, 0, blue_air);
                IS32_setRGB(7, 0, 0, blue_air);
                IS32_setRGB(3, 0, 0, blue_air);
                IS32_setRGB(4, 0, 0, 0);
                bonsaire_leds_bonsaire1_state++;
            break;

            case 20:
                IS32_setRGB(8, 0, 0, 0);
                IS32_setRGB(7, 0, 0, blue_air);
                IS32_setRGB(3, 0, 0, blue_air);
                IS32_setRGB(4, 0, 0, blue_air);
                bonsaire_leds_bonsaire1_state++;
            break;

            case 21:
                IS32_setRGB(6, 0, 0, blue_air);
                IS32_setRGB(7, 0, 0, 0);
                IS32_setRGB(3, 0, 0, blue_air);
                IS32_setRGB(4, 0, 0, blue_air);
                bonsaire_leds_bonsaire1_state++;
            break;

            case 22:
                IS32_setRGB(2, 0, 0, blue_air);
                IS32_setRGB(6, 0, 0, blue_air);
                IS32_setRGB(3, 0, 0, 0);
                IS32_setRGB(4, 0, 0, blue_air);
                bonsaire_leds_bonsaire1_state++;
            break;

            case 23:
                IS32_setRGB(2, 0, 0, blue_air);
                IS32_setRGB(6, 0, 0, blue_air);
                IS32_setRGB(4, 0, 0, 0);
                bonsaire_leds_bonsaire1_state++;
            break;

            case 24:
                IS32_setRGB(2, 0, 0, blue_air);
                IS32_setRGB(6, 0, 0, 0);
                bonsaire_leds_bonsaire1_state++;
            break;

            case 25:
                IS32_setRGB(2, 0, 0, 0);
                bonsaire_leds_bonsaire1_state++;
            break;

            case 26:
                bonsaire_leds_bonsaire1_state++;
            break;

            case 27:
                bonsaire_leds_bonsaire1_state = 0;
            break;

            default:
                bonsaire_leds_bonsaire1_state = 0;
        }
     }
}

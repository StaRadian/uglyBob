#pragma once
#include "stm32g4xx_hal.h"

class Motor {
private:
    
public:
    Motor() {
        TIM5->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E | TIM_CCER_CC4E;
        TIM5->CR1 |= TIM_CR1_CEN;
    }
    ~Motor();
    void SetRight(int32_t duty);
    void SetLeft(int32_t duty);
};
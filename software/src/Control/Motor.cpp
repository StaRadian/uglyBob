#include "Motor.h"
#include "Core/Device.h"

void Motor::SetRight(int32_t duty) {
    if(duty > 0) {
        if(duty > 99) {
            TIM5->CCR3 = 0;
            TIM5->CCR4 = 99;
        } else {
            TIM5->CCR3 = 0;
            TIM5->CCR4 = duty;
        }
        // Device::display -> Print("+");
    } else {
        if(duty < -99) {
            TIM5->CCR1 = 99;
            TIM5->CCR2 = 0;
        } else {
            TIM5->CCR3 = duty * -1;
            TIM5->CCR4 = 0;
        }
        // Device::display -> Print("   -");
    }
}

void Motor::SetLeft(int32_t duty) {
    if(duty > 0) {
        if(duty > 99) {
            TIM5->CCR1 = 0;
            TIM5->CCR2 = 99;
        } else {
            TIM5->CCR1 = 0;
            TIM5->CCR2 = duty;
        }
        // Device::display -> Print("+");
    } else {
        if(duty < -99) {
            TIM5->CCR1 = 99;
            TIM5->CCR2 = 0;
        } else {
            TIM5->CCR1 = duty * -1;
            TIM5->CCR2 = 0;
        }
        // Device::display -> Print("   -");
    }
}
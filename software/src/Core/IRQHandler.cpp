#include "IRQHandler.h"
#include "Device.h"

// 1ms
void HAL_HRTIM_RepetitionEventCallback(HRTIM_HandleTypeDef * hhrtim, uint32_t TimerIdx) {
    if (TimerIdx == HRTIM_TIMERINDEX_MASTER)
    {
        Device::rEncoder -> Update();
        Device::lEncoder -> Update();
        Device::value++;
    }
}

void Error_Handler(void) {
    __disable_irq();
    while (1)
    {
        Device::display -> Print("Err!");
    }
}
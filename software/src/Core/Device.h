#pragma once

#include "Sensor/Encoder.h"
#include "Interface/HCMSDisplay.h"
#include "stm32g4xx_hal.h"

namespace Device {
    
    extern Encoder* rEncoder;
    extern Encoder* lEncoder;
    extern HCMSDisplay* display;
    
    extern int value;
}

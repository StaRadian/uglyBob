#pragma once

#include "stm32g4xx_hal.h"

class Encoder {
private:
    TIM_TypeDef* m_tim;

    int16_t m_data_temp;
    int16_t m_tick = 0;
    uint32_t m_count = 1; // mm
    int32_t m_distance_tick = 0; // mm
    float m_speed;    // mm/s
    
    const float m_cst_resolution = 1000.0;
    const float m_cst_wheel_circumference = 13.0 * 3.14159265359;
    const float m_cst_sample_hz;
    const uint32_t m_cst_count_max = 1000;
public:
    Encoder(TIM_TypeDef* tim, const float sample_hz) : m_tim(tim), m_cst_sample_hz(sample_hz) {
        m_tim -> CCER |= 0x00000011;
        m_tim -> CR1 |= TIM_CR1_CEN;
        m_data_temp = m_tim -> CNT;
    }
    ~Encoder();
    void Update();
    int16_t GetTick() { return m_tick; }
    float GetDistance() { return m_distance_tick * m_cst_wheel_circumference / m_cst_resolution; }
    float GetSpeed() { return m_speed; }
};
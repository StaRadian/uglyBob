#pragma once

#include "stm32g4xx_hal.h"

class Encoder {
private:
    TIM_TypeDef* m_tim;

    int16_t m_data_temp;
    int16_t m_tick = 0;
    int16_t m_tick_temp = 0;
    // uint16_t m_count = 1;
    int16_t m_distance_tick = 0; // mm
    double m_velocity;    // mm/s
    
    const double m_cst_resolution = 1000.0;
    const double m_cst_wheel_circumference = 13.0 * 3.14159265359;
    const double m_cst_sample_hz;
    const int16_t m_cst_count_max = 1000;
public:
    Encoder(TIM_TypeDef* tim, const double sample_hz) : m_tim(tim), m_cst_sample_hz(sample_hz) {
        m_tim -> CCER |= 0x00000011;
        m_tim -> CR1 |= TIM_CR1_CEN;
        m_data_temp = m_tim -> CNT;
    }
    ~Encoder();
    void Update();
    int16_t GetTick() { return m_tick; }
    int32_t GetDistanceTick() { return m_distance_tick; }
    double GetDistanceReward() { return m_cst_wheel_circumference / m_cst_resolution; }
    double GetDistance() { return m_distance_tick * m_cst_wheel_circumference / m_cst_resolution; }
    double GetSpeed() { return m_velocity; }
};
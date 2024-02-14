#include "Encoder.h"

// void Encoder::Update() {
//     int32_t data = m_tim -> CNT; 
//     m_tick = data - m_data_temp;
//     m_data_temp = data;
//     if(m_tick != 0) {
//         m_distance_tick += m_tick;
//         m_speed = m_tick * m_cst_sample_hz * m_cst_wheel_circumference / m_cst_resolution;
//         m_count = 1;
//     } else {
//         if(m_cst_count_max > m_count) {
//             m_speed = m_cst_sample_hz * m_cst_wheel_circumference / m_cst_resolution / (double)m_count;
//             m_count++;
//         } else {
//             m_speed = 0;
//         }
//         // m_speed = 0;
//     }
// }

void Encoder::Update() {
    int16_t data = m_tim -> CNT; 
    m_tick = data - m_data_temp;
    if(m_tick - m_tick_temp > 10) m_tick +=10;
    else if(m_tick - m_tick_temp < -10) m_tick -=10;
    m_tick_temp = m_tick;
    m_data_temp = data;
    m_distance_tick += m_tick;
    m_velocity = m_tick * m_cst_sample_hz * m_cst_wheel_circumference / m_cst_resolution;
}
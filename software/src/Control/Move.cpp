#include "Move.h"
#include "Core/Device.h"

void Move::Update() {
    MoveStopUpdate();
    SpeedUpdate();
    MotorControl::SetRightVelocity(m_r_current_velocity);
    MotorControl::SetLeftVelocity(m_l_current_velocity);
    MotorControl::Update();
}

void Move::SpeedUpdate() {
    double r_err = m_r_target_velocity - m_r_current_velocity;
    double l_err = m_l_target_velocity - m_l_current_velocity;

    if(r_err > m_r_accel) r_err = m_r_accel;
    else if(r_err < m_r_accel * -1) r_err = m_r_accel * -1;
    if(l_err > m_l_accel) l_err = m_l_accel;
    else if(l_err < m_l_accel * -1) l_err = m_l_accel * -1;

    m_r_current_velocity += r_err;
    m_l_current_velocity += l_err;
}

void Move::MoveStopUpdate() {
    if(m_r_end_mission == false) {
        m_r_current_distance = (double)(m_r_encoder->GetDistanceTick() - m_r_distance_tick) * m_r_encoder->GetDistanceReward();
        double r_remaining_distance = m_r_target_distance - m_r_current_distance;

        if(m_r_target_distance > 0) {
            if(r_remaining_distance <= 0) m_r_end_mission = true;
            if(m_movestop == true) {
                if(r_remaining_distance <= m_r_current_velocity * m_r_current_velocity / (2 * m_r_accel * m_cst_sample_hz)) {
                    m_r_target_velocity = 0;
                }
            }
        } else {
            if(r_remaining_distance >= 0) m_r_end_mission = true;
            if(m_movestop == true) {
                if(r_remaining_distance >= m_r_current_velocity * m_r_current_velocity / (2 * m_r_accel * m_cst_sample_hz) * -1) {
                    m_r_target_velocity = 0;
                }
            }
        }
    }
    if(m_l_end_mission == false) {
        m_l_current_distance = (double)(m_l_encoder->GetDistanceTick() - m_l_distance_tick) * m_l_encoder->GetDistanceReward();
        double r_remaining_distance = m_l_target_distance - m_l_current_distance;

        if(m_l_target_distance > 0) {
            if(r_remaining_distance <= 0) m_l_end_mission = true;
            if(m_movestop == true) {
                if(r_remaining_distance <= m_l_current_velocity * m_l_current_velocity / (2 * m_l_accel * m_cst_sample_hz)) {
                    m_l_target_velocity = 0;
                }
            }
        } else {
            if(r_remaining_distance >= 0) m_l_end_mission = true;
            if(m_movestop == true) {
                if(r_remaining_distance >= m_l_current_velocity * m_l_current_velocity / (2 * m_l_accel * m_cst_sample_hz) * -1) {
                    m_l_target_velocity = 0;
                }
            }
        }
    }
}

void Move::MoveStop(double r_distance, double l_distance) {
    m_r_distance_tick = m_r_encoder->GetDistanceTick();
    m_l_distance_tick = m_l_encoder->GetDistanceTick();
    m_r_target_distance = r_distance;
    m_l_target_distance = l_distance;
    m_r_current_distance = 0;
    m_l_current_distance = 0;
    m_r_end_mission = false;
    m_l_end_mission = false;
    m_movestop = true;
}

void Move::MoveStop(double r_distance, double r_velocity, double l_distance, double l_velocity) {
    m_r_target_velocity = r_velocity;
    m_l_target_velocity = l_velocity;
    MoveStop(r_distance, l_distance);
}
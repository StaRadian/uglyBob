#pragma once
#include "MotorControl.h"

class Move : protected MotorControl {
private:
    double m_r_target_velocity = 0;
    double m_l_target_velocity = 0;
    double m_r_current_velocity = 0;
    double m_l_current_velocity = 0;

    double m_r_accel = 1000/m_cst_sample_hz;
    double m_l_accel = 1000/m_cst_sample_hz;

    bool m_r_end_mission = true;
    bool m_l_end_mission = true;
    bool m_movestop = false;

    int32_t m_r_distance_tick = 0;
    int32_t m_l_distance_tick = 0;
    double m_r_target_distance = 0;
    double m_l_target_distance = 0;
    double m_r_current_distance = 0;
    double m_l_current_distance = 0;

    void SpeedUpdate();
    void MoveStopUpdate();

public:
    Move(Encoder* r_encoder, Encoder* l_encoder) 
        : MotorControl(r_encoder, l_encoder) { }
    ~Move();
    void Update();
    void Start() { MotorControl::Start(); }
    void Stop() { MotorControl::Stop(); }

    void SetRightVelocity(double r_velocity) { m_r_target_velocity = r_velocity; }
    void SetLeftVelocity(double l_velocity) { m_l_target_velocity = l_velocity; }
    void SetRightAccel(double r_accel) { m_r_accel = r_accel/m_cst_sample_hz; }
    void SetLeftAccel(double l_accel) { m_l_accel = l_accel/m_cst_sample_hz; }

    void MoveStop(double r_distance, double l_distance);
    void MoveStop(double r_distance, double r_velocity, double l_distance, double l_velocity);
};
#pragma once
#include "Motor.h"
#include "Sensor/Encoder.h"

class MotorControl : private Motor {
private:
    const double m_cst_r_kp = 4.0e-2; //5.2e-3
    const double m_cst_r_ki = 8.0e-1 / m_cst_sample_hz; //4.0e-1
    const double m_cst_r_kd = 0.0e-7 * m_cst_sample_hz; //8.0e-7

    const double m_cst_l_kp = 4.0e-2; //1.7e-1
    const double m_cst_l_ki = 8.0e-1 / m_cst_sample_hz; //8.0e-7
    const double m_cst_l_kd = 0.0e-8 * m_cst_sample_hz; //1.0e-2

    double m_r_integral = 0;
    double m_l_integral = 0;

    double m_r_target_velocity = 0;
    double m_l_target_velocity = 0;

    double m_r_err_pre = 0;
    double m_l_err_pre = 0;

    bool m_active_flag = false;

    

protected:
    Encoder* m_r_encoder;
    Encoder* m_l_encoder;

    uint16_t m_cst_sample_hz = 1000;

public:
    void SetRightVelocity(double right_velocity) {
        m_r_target_velocity = right_velocity;
    }
    void SetLeftVelocity(double left_velocity) {
        m_l_target_velocity = left_velocity;
    }

    double GetRightVelocity(void) {
        return m_r_target_velocity;
    }
    double GetLeftVelocity(void) {
        return m_l_target_velocity;
    }

    MotorControl(Encoder* r_encoder, Encoder* l_encoder)
        : m_r_encoder(r_encoder), m_l_encoder(l_encoder) {};
    ~MotorControl();
    void Start() { m_active_flag = true; }
    void Stop() {
        m_active_flag = false;
        SetRight(0);
        SetLeft(0);
    }
    void Update();
};
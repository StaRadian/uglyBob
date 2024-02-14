#include "MotorControl.h"
#include "Core/Device.h"

void MotorControl::Update() {
    if(m_active_flag == false) {
        m_r_integral = 0;
        m_l_integral = 0;
        m_r_err_pre = 0;
        m_l_err_pre = 0;
        return;
    }
    
    double r_err = m_r_target_velocity - m_r_encoder->GetSpeed();
    double l_err = m_l_target_velocity - m_l_encoder->GetSpeed();

    // if(r_err > 0) Device::display -> Print(" +  ");
    // else Device::display -> Print("  - ");
    // if(l_err > 0) Device::display -> Print(" +  ");
    // else Device::display -> Print("  - ");

    m_r_integral += m_cst_r_ki * r_err;
    m_l_integral += m_cst_l_ki * l_err;

    float r_output = r_err * m_cst_r_kp + m_r_integral + m_cst_r_kd * (r_err - m_r_err_pre);
    float l_output = l_err * m_cst_l_kp + m_l_integral + m_cst_l_kd * (l_err - m_l_err_pre);

    m_r_err_pre = r_err;
    m_l_err_pre = l_err;
    
    SetRight((int32_t)0);
    SetLeft((int32_t)0);
}
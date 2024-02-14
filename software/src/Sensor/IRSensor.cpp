#include "IRSensor.h"

#include "hrtim.h"

void IRSensor::Start() {
    HAL_HRTIM_WaveformCountStart_IT(m_hhrtim, HRTIM_TIMERID_TIMER_A + HRTIM_TIMERID_TIMER_B + HRTIM_TIMERID_TIMER_C + HRTIM_TIMERID_TIMER_D);
    HAL_HRTIM_WaveformOutputStart(m_hhrtim, HRTIM_OUTPUT_TA1 + HRTIM_OUTPUT_TB1 + HRTIM_OUTPUT_TC1 + HRTIM_OUTPUT_TD1);
    SET_BIT(OPAMP2->CSR, OPAMP_CSR_OPAMPxEN);
    SET_BIT(OPAMP3->CSR, OPAMP_CSR_OPAMPxEN);
    SET_BIT(OPAMP4->CSR, OPAMP_CSR_OPAMPxEN);
    SET_BIT(OPAMP6->CSR, OPAMP_CSR_OPAMPxEN);
    HAL_ADC_Start_IT(m_fl_hadc);
    HAL_ADC_Start_IT(m_sl_hadc);
    HAL_ADC_Start_IT(m_sr_hadc);
    HAL_ADC_Start_IT(m_fr_hadc);
}

void IRSensor::Stop() {
    HAL_HRTIM_WaveformOutputStop(m_hhrtim, HRTIM_OUTPUT_TA1 + HRTIM_OUTPUT_TB1 + HRTIM_OUTPUT_TC1 + HRTIM_OUTPUT_TD1);
    HAL_HRTIM_WaveformCountStop_IT(m_hhrtim, HRTIM_TIMERID_TIMER_A + HRTIM_TIMERID_TIMER_B + HRTIM_TIMERID_TIMER_C + HRTIM_TIMERID_TIMER_D);
    CLEAR_BIT(OPAMP2->CSR, OPAMP_CSR_OPAMPxEN);
    CLEAR_BIT(OPAMP3->CSR, OPAMP_CSR_OPAMPxEN);
    CLEAR_BIT(OPAMP4->CSR, OPAMP_CSR_OPAMPxEN);
    CLEAR_BIT(OPAMP6->CSR, OPAMP_CSR_OPAMPxEN);
    HAL_ADC_Stop_IT(m_fl_hadc);
    HAL_ADC_Stop_IT(m_sl_hadc);
    HAL_ADC_Stop_IT(m_sr_hadc);
    HAL_ADC_Stop_IT(m_fr_hadc);
}

void IRSensor::Update() {
    m_fl_data = m_fl_hadc->Instance->DR;
    m_sl_data = m_sl_hadc->Instance->DR;
    m_sr_data = m_sr_hadc->Instance->DR;
    m_fr_data = m_fr_hadc->Instance->DR;

    m_filter_cnt = (m_filter_cnt + 1) % m_cnst_filter;
    int16_t last_count = (m_filter_cnt + 1) % m_cnst_filter;

    m_fl_data_buff[m_filter_cnt] = m_fl_data / m_cnst_filter;
    m_sl_data_buff[m_filter_cnt] = m_sl_data / m_cnst_filter;
    m_sr_data_buff[m_filter_cnt] = m_sr_data / m_cnst_filter;
    m_fr_data_buff[m_filter_cnt] = m_fr_data / m_cnst_filter;

    m_fl_flit_data += m_fl_data_buff[m_filter_cnt];
    m_sl_flit_data += m_sl_data_buff[m_filter_cnt];
    m_sr_flit_data += m_sr_data_buff[m_filter_cnt];
    m_fr_flit_data += m_fr_data_buff[m_filter_cnt];

    m_fl_flit_data -= m_fl_data_buff[last_count];
    m_sl_flit_data -= m_sl_data_buff[last_count];
    m_sr_flit_data -= m_sr_data_buff[last_count];
    m_fr_flit_data -= m_fr_data_buff[last_count];
}
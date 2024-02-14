#pragma once

#include "stm32g4xx_hal.h"

class IRSensor {
private:
    HRTIM_HandleTypeDef *m_hhrtim;
    ADC_HandleTypeDef *m_fl_hadc;
    ADC_HandleTypeDef *m_sl_hadc;
    ADC_HandleTypeDef *m_sr_hadc;
    ADC_HandleTypeDef *m_fr_hadc;
    uint32_t m_fl_data;
    uint32_t m_sl_data;
    uint32_t m_sr_data;
    uint32_t m_fr_data;

    uint32_t m_fl_flit_data = 0;
    uint32_t m_sl_flit_data = 0;
    uint32_t m_sr_flit_data = 0;
    uint32_t m_fr_flit_data = 0;

    const int16_t m_cnst_filter = 20;
    int16_t m_filter_cnt = 0;

    uint32_t* m_fl_data_buff;
    uint32_t* m_sl_data_buff;
    uint32_t* m_sr_data_buff;
    uint32_t* m_fr_data_buff;

    

    uint32_t m_test[20];
    uint32_t m_p_test;
    int16_t m_count = 0;
    uint32_t m_result = 0;
    bool m_trigger = false;
    
public:
    IRSensor(HRTIM_HandleTypeDef *hhrtim,
        ADC_HandleTypeDef *fl_hadc,
        ADC_HandleTypeDef *sl_hadc,
        ADC_HandleTypeDef *sr_hadc,
        ADC_HandleTypeDef *fr_hadc) :m_hhrtim(hhrtim),
        m_fl_hadc(fl_hadc),
        m_sl_hadc(sl_hadc),
        m_sr_hadc(sr_hadc),
        m_fr_hadc(fr_hadc) {
        HAL_HRTIM_WaveformCountStart_IT(m_hhrtim, HRTIM_TIMERID_MASTER);
        m_fl_data_buff = new uint32_t[m_cnst_filter];
        m_sl_data_buff = new uint32_t[m_cnst_filter];
        m_sr_data_buff = new uint32_t[m_cnst_filter];
        m_fr_data_buff = new uint32_t[m_cnst_filter];
        for(int i =0; i<m_cnst_filter; i++) {
            m_fl_data_buff[i] = 0;
            m_sl_data_buff[i] = 0;
            m_sr_data_buff[i] = 0;
            m_fr_data_buff[i] = 0;
        }
    }
    void Start();
    void Stop();
    void Update();
    uint32_t GetData() {
        return m_sr_flit_data;
    }

};
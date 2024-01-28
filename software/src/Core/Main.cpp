#include "stm32g4xx_hal.h"
#include "Device.h"

#include "adc.h"
#include "gpio.h"
#include "i2c.h"
#include "opamp.h"
#include "hrtim.h"
#include "tim.h"

#include <iomanip>

void SystemClock_Config(void) 
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
    RCC_OscInitStruct.PLL.PLLN = 85;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        // Error_Handler();
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
    {
        // Error_Handler();
    }
}

void Init(void) 
{
    HAL_Init();
    SystemClock_Config();


    MX_ADC1_Init();
    MX_ADC2_Init();
    MX_ADC3_Init();
    MX_ADC4_Init();
    MX_ADC5_Init();

    MX_GPIO_Init();
    MX_HRTIM1_Init();
    MX_I2C1_Init();

    MX_OPAMP2_Init();
    MX_OPAMP3_Init();
    MX_OPAMP4_Init();
    MX_OPAMP6_Init();

    MX_TIM3_Init(); //Encoder
    MX_TIM4_Init(); //Encoder
    MX_TIM5_Init(); //Moter

    Device::rEncoder = new Encoder(TIM3, 1000);
    Device::lEncoder = new Encoder(TIM4, 1000);

    Device::display = new HCMSDisplay(
        GPIOC ,GPIO_PIN_15,
        GPIOB ,GPIO_PIN_10,
        GPIOF ,GPIO_PIN_1,
        GPIOC ,GPIO_PIN_14,
        4,
        1, 1
    );

}

int value = 0;

int main(void) 
{
    Init();
    Device::display->Print("Bob!");

    // HAL_HRTIM_WaveformOutputStart(&hhrtim1, HRTIM_OUTPUT_TA1 + HRTIM_OUTPUT_TB1);
    // HAL_HRTIM_WaveformCountStart(&hhrtim1, HRTIM_TIMERID_MASTER + HRTIM_TIMERID_TIMER_A + HRTIM_TIMERID_TIMER_B);

    // HAL_OPAMP_Start(&hopamp1);
    // HAL_ADC_Start(&hadc1);
    htim5.Instance->CCR1 = 0;
    htim5.Instance->CCR2 = 0;
    htim5.Instance->CCR3 = 0;
    htim5.Instance->CCR4 = 0;
    HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_4);

    HAL_HRTIM_WaveformOutputStart(&hhrtim1, HRTIM_OUTPUT_TA1 + HRTIM_OUTPUT_TB1 + HRTIM_OUTPUT_TC1 + HRTIM_OUTPUT_TD1);
    // HAL_HRTIM_WaveformCountStart(&hhrtim1, HRTIM_OUTPUT_TA1 + HRTIM_OUTPUT_TB1 + HRTIM_OUTPUT_TC1 + HRTIM_OUTPUT_TD1);
    HAL_HRTIM_WaveformCountStart_IT(&hhrtim1, HRTIM_TIMERID_MASTER + HRTIM_TIMERID_TIMER_A + HRTIM_TIMERID_TIMER_B + HRTIM_TIMERID_TIMER_C + HRTIM_TIMERID_TIMER_D);

    HAL_OPAMP_Start(&hopamp2);
    HAL_OPAMP_Start(&hopamp3);
    HAL_OPAMP_Start(&hopamp4);
    HAL_OPAMP_Start(&hopamp6);
    // HAL_ADC_Start(&hadc4);


    // HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
    uint8_t buffer[4] = {};
    uint8_t tx_data[4] = {0xC,};

    HAL_ADC_Start_IT(&hadc2);   // FR
    HAL_ADC_Start_IT(&hadc3);   // SR
    HAL_ADC_Start_IT(&hadc4);   // FL
    HAL_ADC_Start_IT(&hadc5);   // SL
    HAL_I2C_Mem_Write(&hi2c1, 0b01101001 << 1, 0x4E, I2C_MEMADD_SIZE_8BIT, tx_data, 1, HAL_MAX_DELAY);
    
    while (1)
    {
        HAL_I2C_Mem_Read(&hi2c1, 0b01101001 << 1, 0x29, I2C_MEMADD_SIZE_8BIT, buffer, 2, HAL_MAX_DELAY);
        // htim5.Instance->CCR3 = value;
        // value = right.GetDistance();
        // vfd.Print("%4f", value);
        // vfd.Print((int)right.GetSpeed());
        // vfd.Print(buffer[0]);
        // Device::lEncoder -> Update();
        Device::display -> Print((int)Device::rEncoder -> GetSpeed());
        // Device::display -> Print((int)Device::value);
        HAL_Delay(100);
        // HAL_ADC_PollForConversion(&hadc4, HAL_MAX_DELAY);
        // vfd.Print((int)(ADC2 -> DR)/10);

        // HAL_ADC_Stop(&hadc4);
        // Device::vfd.Print((int)right.GetSpeed());
        // HAL_ADC_Stop(&hadc4);
    }

}
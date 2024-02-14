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

    Device::sensor = new IRSensor(&hhrtim1, &hadc4, &hadc5, &hadc3, &hadc2);

    Device::rEncoder = new Encoder(TIM3, 1000);
    Device::lEncoder = new Encoder(TIM4, 1000);

    Device::motor = new Move(Device::rEncoder, Device::lEncoder);

    Device::display = new HCMSDisplay(
        GPIOC ,GPIO_PIN_15,
        GPIOB ,GPIO_PIN_10,
        GPIOF ,GPIO_PIN_1,
        GPIOC ,GPIO_PIN_14,
        4
        // 1, 1
    );

}

int value = 0;

int main(void) 
{
    Init();
    Device::display->Print("Bob!");
    Device::sensor -> Start();
    Device::motor -> Start();
    HAL_Delay(3000);
    Device::display->Print("Start!");
    HAL_Delay(500);
    Device::motor -> SetRightAccel(1000);
    Device::motor -> SetLeftAccel(1000);
    // Device::motor -> SetRightVelocity(600);
    // Device::motor -> SetLeftVelocity(600);
    Device::motor -> MoveStop(100, 200, 100, 200);
    // HAL_Delay(20000);
    // Device::motor -> SetRightVelocity();
    // Device::motor -> SetRightVelocity(0);
    // Device::motor -> SetLeftVelocity(0);


    uint8_t buffer[4] = {};
    uint8_t tx_data[4] = {0xC,};

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
        // if(Device::rEncoder -> GetSpeed() >= 9500) {
        //     HAL_Delay(1000);
        //     Device::motor -> SetRightVelocity(0);
        // }
        // if(Device::lEncoder -> GetSpeed() >= 9500) {
        //     HAL_Delay(1000);
        //     Device::motor -> SetLeftVelocity(0);
        // }

        HAL_Delay(100);
    }

}
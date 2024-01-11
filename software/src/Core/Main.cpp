#include "main.h"

#include "adc.h"
#include "gpio.h"
#include "i2c.h"
#include "opamp.h"
#include "spi.h"
#include "usart.h"
#include "hrtim.h"

#include "Interface/HCMSDisplay.h"
#include "Interface/UartPrint.h"
// #include "Interface/SST25Flash.h"

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
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

int main(void)
{
  HAL_Init();
  SystemClock_Config();

  MX_ADC1_Init();
  MX_GPIO_Init();
  MX_HRTIM1_Init();
  // MX_I2C2_Init();
  MX_OPAMP1_Init();
  MX_SPI1_Init();
  MX_SPI3_Init();
  MX_USART1_UART_Init();
  MX_USART1_UART_Init();

  HCMSDisplay vfd(
    GPIOE ,GPIO_PIN_5,
    GPIOB ,GPIO_PIN_6,
    GPIOB ,GPIO_PIN_7,
    GPIOE ,GPIO_PIN_6,
    GPIOB ,GPIO_PIN_9
  );

  UartPrint tx(&huart1, 9600);

  vfd.Print("Hi bart");
  
  tx.Print("Welcome to the M4");
  // SST25Flash rom(Flash_CE_GPIO_Port, Flash_CE_Pin);
  // rom.getData(0x000000, 4);

  uint8_t tx_data[2] = {0x80 | 0x75, 0x00};
  uint8_t rx_data[2] = {};

  // HAL_StatusTypeDef a = HAL_SPI_TransmitReceive(&hspi1, tx_data, rx_data, 1, 1000);
  // HAL_StatusTypeDef a = HAL_SPI_TransmitReceive(&hspi1, tx_data, rx_data, 2, 0xFFFF);
  // HAL_StatusTypeDef a = HAL_SPI_TransmitReceive(&hspi2, tx_data, rx_data, 2, 0xFFFF);
  uint8_t buffer[4] = {};
  uint8_t data[4] = {0x75,};

  // HAL_I2C_Master_Transmit(&hi2c2, 0b01101001 << 1, data, 1, HAL_MAX_DELAY);
  // HAL_I2C_Mem_Read(&hi2c2, 0b01101001 << 1, 0x75, I2C_MEMADD_SIZE_8BIT, buffer, 1, HAL_MAX_DELAY);

  // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
  // HAL_SPI_Transmit(&hspi1, tx_data, 2, HAL_MAX_DELAY);
  // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
  HAL_HRTIM_WaveformOutputStart(&hhrtim1, HRTIM_OUTPUT_TA1 + HRTIM_OUTPUT_TB1);
  HAL_HRTIM_WaveformCountStart(&hhrtim1, HRTIM_TIMERID_MASTER + HRTIM_TIMERID_TIMER_A + HRTIM_TIMERID_TIMER_B);

  HAL_OPAMP_Start(&hopamp1);
  HAL_ADC_Start(&hadc1);

  while (1)
  {
    uint32_t value = ADC1->DR;
    // HAL_ADC_Stop(&hadc1);
    // HAL_ADC_Start(&hadc1);
    vfd.Print("%8d", value);
    // HAL_Delay(100);
  }
}

void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
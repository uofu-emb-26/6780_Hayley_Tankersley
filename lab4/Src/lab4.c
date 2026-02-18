#include "main.h"
#include "stm32f0xx_hal.h"
#include "hal_gpio4.h"
#include "assert.h"

void initUSART(void);

void SystemClock_Config(void);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  /* Configure the system clock */
  SystemClock_Config();

  My_HAL_RCC_GPIOC_CLK_Enable();

  GPIO_InitTypeDef initStr = { GPIO_PIN_6 , GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_LOW, GPIO_NOPULL};

  My_HAL_GPIO_Init(GPIOC, &initStr);

  // Set up alt function on pins PC10 (USART3 TX) and PC11 (USART3 RX)
  My_HAL_GPIO_AltFunction();

  // Init USART 3
  initUSART();

  while (1)
  {

    My_HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_6);
    HAL_Delay(600);
 
  }
  return -1;
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* User can add their own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
}

void initUSART(void)
{

  // Set Baud Rate to 115200 bits/second

  //Baud_TXRX = fCLK / USART_BRR = 115200

  USART3->BRR = HAL_RCC_GetHCLKFreq() / 115200;

  // Enable transmitter hardware

  //USART_CR1 TE

  USART3->CR1 |= USART_CR1_TE;

  // Enable receiver hardware

  //USART_CR1 RE

  USART3->CR1 |= USART_CR1_RE;

  // Enable peripheral control bit

  //USART_CR1 UE

  USART3->CR1 |= USART_CR1_UE;

}

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add their own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
}
#endif /* USE_FULL_ASSERT */

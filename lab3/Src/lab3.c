#include "main.h"
#include "stm32f0xx_hal.h"
#include "hal_gpio3.h"
#include "assert.h"

void SystemClock_Config(void);

void TIM2_IRQHandler(void);

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

  GPIO_InitTypeDef initStr = { GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_LOW, GPIO_NOPULL};

  My_HAL_GPIO_Init(GPIOC, &initStr);
  //init_User_Button(GPIOA);

  My_HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET);
  

  Set_TIM2();
  Set_TIM3();

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


void Set_TIM2(void)
{
  
  TIM2->PSC |= 0b0001111100111111; // set PSC to 7999, this sets clock freq to 1 kHz, which has T = 1 ms
  
  assert(TIM2->PSC == 0b0001111100111111);
  // ARR = 8MHz / ( (7999+1) * 4 ) = 250

  TIM2->ARR &= 0x0;
  TIM2->ARR |= 0b0000000011111010;

  assert(TIM2->ARR == 0b0000000011111010);

  // Use DIER to enable interrupt

  TIM2->DIER |= TIM_DIER_UIE;

  //assert(TIM2->ARR == 0b01);


  // Enable Tim2 in control reg

  TIM2->CR1 |= TIM_CR1_CEN;
  //assert(TIM2->CR1 == 0b01);


  //Enable NVIC IRQ Handler

  NVIC_EnableIRQ(TIM2_IRQn);
  NVIC_SetPriority(TIM2_IRQn,3);

}

void Set_TIM3(void)
{
  
  TIM3->PSC |= 0b0000000001001111; // set PSC to 79, this sets clock freq to 100 kHz, which has T = 10 us
  
  assert(TIM3->PSC == 0b0000000001001111);
  
  // ARR = 8MHz / ( (79+1) * 800 ) = 125

  TIM3->ARR &= 0x0;
  TIM3->ARR |= 0b0000000001111101;

  assert(TIM3->ARR == 0b0000000001111101);


  // Enable Tim3 in control reg

  //TIM3->CR1 |= TIM_CR1_CEN;

  // Set Up PWM mode
  TIM3->CCMR1 &= ~TIM_CCMR1_CC1S; // Set CC1S to output mode [00]
  TIM3->CCMR1 &= ~TIM_CCMR1_CC2S; // Set CC2S to output mode [00]

  TIM3->CCMR1 |= TIM_CCMR1_OC1M; // Set OC1M to PWM mode 2 [111]

  TIM3->CCMR1 &= ~TIM_CCMR1_OC2M; // Clear OC2M before setting to PWM mode 3 [110]
  TIM3->CCMR1 |= TIM_CCMR1_OC2M_1; // Set bit 1 of OC2M
  TIM3->CCMR1 |= TIM_CCMR1_OC2M_2; // Set bit 2 of OC2M

  assert((TIM3->CCMR1 &= TIM_CCMR1_OC2M) == 0b01100000000000000000);

  // NEXT STEP: 3.2 step 3 part e "enable output compare preload for both channels"

}


void  TIM2_IRQHandler(void)
{

  My_HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_8);
  My_HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_9);

  TIM2->SR &= ~TIM_SR_UIF;

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

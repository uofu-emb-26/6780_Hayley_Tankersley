#include "main.h"
#include "stm32f0xx_hal.h"
#include "hal_gpio6.h"

void SystemClock_Config(void);
void InitADC(void);
void ADCCheck(void);
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
  
  GPIO_InitTypeDef initStr = { GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_9 | GPIO_PIN_8, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_LOW, GPIO_NOPULL};
  HAL_GPIO_Init(GPIOC, &initStr);

  // Set PC0 for analog input
  GPIO_InitTypeDef initStr2 = { GPIO_PIN_0,GPIO_MODE_ANALOG, GPIO_SPEED_FREQ_LOW, GPIO_NOPULL};
  HAL_GPIO_Init(GPIOC, &initStr2);

  InitADC();

  while (1)
  {
 
    HAL_Delay(500);

    ADCCheck();

  }
  return -1;
}

/**
  * @brief System Clock Configuration
  * @retval None
  */


void ADCCheck(void)
{

    int adc_output = ADC1->DR;

    if(adc_output > 0b00110100) // above 52
    {
      My_HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,1);
    }
    else
    {
      My_HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,0);
    }

    if(adc_output > 0b01101000) // above 104
    {
      My_HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,1);
    }
    else
    {
      My_HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,0);
    }

    if(adc_output > 0b10011100) // above 156
    {
      My_HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,1);
    }
    else
    {
      My_HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,0);
    }

    if(adc_output > 0b11010000) // above 208
    {
      My_HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,1);
    }
    else
    {
      My_HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,0);
    }

}


void InitADC(void)
{
  // Enable ADC clock

  RCC->APB2ENR |= RCC_APB2ENR_ADCEN;

  // Set to 8 bit resolution RES = [10]

  ADC1->CFGR1 &= ~ADC_CFGR1_RES;
  ADC1->CFGR1 |= (1 << 4);

  // Set to continuous conversion mode

  ADC1->CFGR1 |= ADC_CFGR1_CONT;

  // Disable Hardware Triggers

  ADC1->CFGR1 &= ~ADC_CFGR1_EXTEN;

  // Select / Enable input pins channel (PC0 => ADC_IN10)

  ADC1->CHSELR |= (1<<10);

  // Calibrate

    // 1. Ensure that ADEN = 0 and DMAEN = 0
  ADC1->CR &= ~ADC_CR_ADEN;
  ADC1->CFGR1 &= ~ADC_CFGR1_DMAEN;
    // 2. Set ADCAL = 1

  ADC1->CR |=  ADC_CR_ADCAL;
    // 3. Wait until ADCAL = 0
  while((ADC1->CR & ADC_CR_ADCAL) != 0)
  {
    // wait
  }

  // After Calibration, set peripheral enable

  ADC1->CR |= ADC_CR_ADEN;

  // Start ADC Conversion (continuous)

  ADC1->CR |= ADC_CR_ADSTART;

}
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

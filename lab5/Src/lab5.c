#include "main.h"
#include "stm32f0xx_hal.h"
#include "hal_gpio5.h"
#include "assert.h"

void SystemClock_Config(void);
void InitI2C(void);

void StartI2CTransaction(uint8_t address, uint8_t rw, uint8_t num_bytes);

void TransmitI2C(void);

void ReceiveI2C(void);



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


  // Set up pin 6 to observe system status
  My_HAL_RCC_GPIOC_CLK_Enable();
  
  GPIO_InitTypeDef initStr = { GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_9, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_LOW, GPIO_NOPULL};
  HAL_GPIO_Init(GPIOC, &initStr);


  // Init I2C2
    // Co

    InitI2C();

    //My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, 1);

    StartI2CTransaction(0x69,0,0x1);

  while (1)
  {

    // Toggle pin 6 to observe system status
    My_HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_7);
    HAL_Delay(600);

    //I2C2->ICR |= (1 << 5) | (1 << 4);
    StartI2CTransaction(0x69,0,0x1);
    //while((I2C2->TXIS) && )

    while(((I2C2->ISR & I2C_ISR_TXIS) == 0b0) && ((I2C2->ISR & I2C_ISR_NACKF) == 0b0))
    {
      
    }
    if((I2C2->ISR & I2C_ISR_TXIS) == (1 << 1))
    {
      My_HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,1);
      
    }
    else if((I2C2->ISR & I2C_ISR_NACKF) == (1 << 4))
    {
      My_HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,1);
    }

    HAL_Delay(5000);

 
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

void InitI2C(void)
{
  // Configure I2C GPIO pins

    GPIO_InitTypeDef initStr1 = { GPIO_PIN_11, GPIO_MODE_AF_OD,GPIO_SPEED_FREQ_LOW,GPIO_NOPULL};
    HAL_GPIO_Init(GPIOB, &initStr1);

    // set AFR register I2C2_SDA to AF1 [0001]
    GPIOB->AFR[1] &=  ~GPIO_AFRH_AFSEL11;//(0b0001 << 12);
    GPIOB->AFR[1] |= (0b0001 << 12);

    GPIO_InitTypeDef initStr2 = { GPIO_PIN_13, GPIO_MODE_AF_OD,GPIO_SPEED_FREQ_LOW,GPIO_NOPULL};  
    HAL_GPIO_Init(GPIOB,&initStr2);
    // set AFR register I2C2_SCL to AF5 [0101]
    GPIOB->AFR[1] &=  ~GPIO_AFRH_AFSEL13;
    GPIOB->AFR[1] |= (0b0101 << 20);

    GPIO_InitTypeDef initStr3 = { GPIO_PIN_14, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_LOW,GPIO_NOPULL};
    HAL_GPIO_Init(GPIOB, &initStr3);
    My_HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 1);

    GPIO_InitTypeDef initStr4 = { GPIO_PIN_0, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_LOW,GPIO_NOPULL};
    HAL_GPIO_Init(GPIOC, &initStr4);
    My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, 1);

    GPIO_InitTypeDef initStr5 = { GPIO_PIN_15, GPIO_MODE_OUTPUT_OD, GPIO_SPEED_FREQ_LOW};
    HAL_GPIO_Init(GPIOB, &initStr5);
    My_HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, 1);

  // Enable RCC clock for I2C system

    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
    assert((RCC->APB1ENR & RCC_APB1ENR_I2C2EN) == (1<<22));

  // Configure bus iming using I2Cx_TIMINGR register
    // We want standard mode (Sm) at 100kHz

    // Clear 
    I2C2->TIMINGR = 0;

    // PRESC = 1

    I2C2->TIMINGR |= (0x1 << 28);

    // SCLL = 0x13

    I2C2->TIMINGR |= (0x13 << 0);

    // SCLH = 0xF

    I2C2->TIMINGR |= (0xF << 8);

    // SDADEL = 0x2

    I2C2->TIMINGR |= (0x2 << 16);

    // SCLDEL = 0x4

    I2C2->TIMINGR |= (0x4 << 20);

  // Enable I2C Peripheral using CR1 register PE bit

    I2C2->CR1 |= 0b1;
}

void StartI2CTransaction(uint8_t address,uint8_t rw, uint8_t num_bytes)
{
    // This function starts an I2C Transaction

    // address = slave address
    // rw = read/write command, 0b0 = write, 0b1 = read
    // num_bytes = number of data bytes to be transmitted


    // Set the slave address in SADD[7:1]

    I2C2->CR2 &= ~(0x3FF<<0);
    I2C2->CR2 |= (address << 1);

    // Set number of data bytes to be transmitted in NBYTES[7:0]

    I2C2->CR2 &= ~(0xFF << 16);
    I2C2->CR2 |= (num_bytes << 16);

    // Configure RD_WRN to indicate read/write

    if(rw == 0b0)
    {
        I2C2->CR2 &=  ~I2C_CR2_RD_WRN;// ~(1 << 10);
    }
    else if (rw == 0b1)
    {
        I2C2->CR2 |= ~I2C_CR2_RD_WRN;//(1 << 10);
    }
    
    // Set START bit [locks transaction parameters temporarily]

    I2C2->CR2 |= (1 << 13);

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

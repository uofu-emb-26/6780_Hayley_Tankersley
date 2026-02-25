#include "main.h"
#include "math.h"
#include "stm32f0xx_hal.h"
#include "hal_gpio4.h"
#include "assert.h"

void initUSART(void);
void transmitCharUSART(char char2send);
void transmitStringUART(char str[]);

void USART3_4_IRQHandler(void);

void SystemClock_Config(void);

uint8_t data;
uint8_t led;
uint8_t data_flag = 0b0;
uint8_t cmd_num = 0b0;

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
  My_HAL_GPIO_AltFunction();

  GPIO_InitTypeDef initStr = { GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_LOW, GPIO_NOPULL};

  HAL_GPIO_Init(GPIOC, &initStr);

  // Init USART 3
  initUSART();

  while (1)
  {
    // transmitStringUART("test\n\0");
    // HAL_Delay(500);


    if (data_flag == 0b1)
    {
      //uint8_t chartoreceive = (uint8_t)(USART3->RDR); /* Receive data, clear flag */
      
      if(cmd_num == 1)
      {
        if(data == 'r' || data == 'o' || data == 'b' || data == 'g')
        {
          led = data;
          transmitStringUART("Submit command\r\n\0");
        }
        else
        {
          transmitStringUART("Invalid Command, Submit LED\r\n\0");
          cmd_num = 0;
        }
      }
      else if(cmd_num == 2)
      {
        cmd_num = 0;

        if(data == '0') // turn off LED
        {
          if(led == 'r')
          {
            My_HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,0);

          }
          else if(led == 'b')
          {
            My_HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,0);
          }
          else if(led == 'o')
          {
            My_HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,0);
          }
          else if(led == 'g')
          {
            My_HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,0);

          }

        }
        else if(data == '1') // Turn on LED
        {
          if(led == 'r')
          {
            My_HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,1);
          }
          else if(led == 'b')
          {

            My_HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,1);
          }
          else if(led == 'o')
          {

            My_HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,1);
          }
          else if(led == 'g')
          {
            My_HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,1);
          }
          
        }
        else if (data == '2') // toggle LED
        {
          if(led == 'r')
          {
            My_HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_6);

          }
          else if(led == 'b')
          {
            My_HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_7);
          }
          else if(led == 'o')
          {
            My_HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_8);
          }
          else if(led == 'g')
          {
            My_HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_9);
          }

        }
        else
        {
          transmitStringUART("Invalid Command, Submit LED\r\n\0");
        }
      }
      data_flag = 0b0;
    }

      // if(data == 'r')
      // {
      //   // PC6 = red
      //   My_HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_6);
      // }
      // else if(data == 'b')
      // {
      //   // PC7 = blue
      //   My_HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_7);
      // }
      // else if(data == 'o')
      // {
      //   // PC8 = orange
      //   My_HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_8);
      // }
      // else if(data == 'g')
      // {
      //   // PC9 = green
      //   My_HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_9);
      // }
      // else
      // {
      //   transmitStringUART("Invalid character - enter r, b, o, or g\r\n\0");
      // }
      // data_flag = 0b0;
      // }
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
void transmitCharUSART(char char2send)
{
  // Check USART status flag indicating that transmit register is empty

    // USART_ISR, TXE
    // while loop that exits when flag is set (TXE = 1)
    
    while((USART3->ISR & (1<<7)) == 00) 
    {

    }
    

  // Write char into transmit register
    // USART_TDR [7:0]

    //USART3->TDR &= char2send;

    USART3->TDR = char2send;//char2send;

    // My_HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_6);
    // HAL_Delay(500);
  //
}

void transmitStringUART(char str[])
{

  int i = 0;
  while(1)
  {
    if(str[i] == '\0')
    {
      USART3->ICR |= USART_ICR_TCCF;
      return;
    }
    transmitCharUSART(str[i]);
    i = i + 1;
  }

}

void initUSART(void)
{

  // Set Baud Rate to 115200 bits/second

  //Baud_TXRX = fCLK / USART_BRR = 115200

  RCC->APB1ENR |= (1 << 18);
  USART3->CR1 = 0;

  USART3->BRR = (uint32_t)(HAL_RCC_GetPCLK1Freq() / 115200);

  USART3->CR1 |= USART_CR1_RXNEIE;

  USART3->CR1 |= USART_CR1_TE | USART_CR1_RE;
  
  USART3->CR1 |= USART_CR1_UE;
  

  NVIC_EnableIRQ(USART3_4_IRQn);
  NVIC_SetPriority(USART3_4_IRQn,1);
}

void USART3_4_IRQHandler(void)
{
  
  data = (uint8_t)(USART3->RDR);
  data_flag = 0b1;
  cmd_num = cmd_num + 1;

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

#include <stdint.h>
#include "stm32f0xx_hal.h"
#include "hal_gpio5.h"
#include "i2c_functions.h"

void InitI2C(void)
{


  // Configure I2C GPIO pins

    GPIO_InitTypeDef initStr1 = { GPIO_PIN_11, GPIO_MODE_AF_OD};
    HAL_GPIO_Init(GPIOB, &initStr1);

    // set AFR register I2C2_SDA to AF1 [0001]
    GPIOB->AFR[1] |= (1 << 20);

    GPIO_InitTypeDef initStr2 = { GPIO_PIN_13, GPIO_MODE_AF_OD};  
    HAL_GPIO_Init(GPIOB,&initStr2);
    // set AFR register I2C2_SCL to AF5 [0101]
    GPIOB->AFR[1] |= (1 << 20);
    GPIOB->AFR[1] |= (1 << 22);

    GPIO_InitTypeDef initStr3 = { GPIO_PIN_14, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_LOW};
    HAL_GPIO_Init(GPIOB, &initStr3);
    My_HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 1);

    GPIO_InitTypeDef initStr4 = { GPIO_PIN_0, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_LOW};
    HAL_GPIO_Init(GPIOC, &initStr4);
    My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, 1);

  // Enable RCC clock for I2C system

    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;

  // Configure bus iming using I2Cx_TIMINGR register
    // We want standard mode (Sm) at 100kHz

    // Clear 
    I2C2->TIMINGR = 0;

    // PRESC = 1

    I2C2->TIMINGR |= (1 << 28);

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
    // rw = read/write command
    // num_bytes = number of data bytes to be transmitted


    // Set the slave address in SADD[7:1]

    // Set number of data bytes to be transmitted in NBYTES[7:0]

    // Configure RD_WRN to indicate read/write
    
    // Set START bit [locks transaction parameters temporarily]
}

void TransmitI2C(void)
{

    // FIRST CONTACT

      // poll BOTH TXIS, NACKF status bits

      // if(NACKF) - slave device did not acknowledge the address frame, config issue
        // clear NACKF flag
        // revise init
        // attempt new transaction

      // if(TXIS) - address frame completed successfully
        // Write to TXDR
    
    // SEND ALL BYTES
      // for n in num_bytes
        // poll NACKF, TXIS then write
        
    // NO MORE DATA:
      // poll TC
        // set STOP bit in CR2

}

void ReceiveI2C(void)
{

}
#include <stdint.h>
#include <stdio.h>
#include <stm32f0xx_hal.h>
#include <stm32f0xx_hal_gpio.h>
#include "assert.h"
int32_t get_2bit_pin_mask(uint32_t GPIO_Pin);
void My_HAL_RCC_GPIOC_CLK_Enable(void)
{
    // Enable GPIOC, GPIOB clocks
    //RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
}


void My_HAL_GPIO_Init(void) /*, GPIO_InitTypeDef *GPIO_Init */
{   
    // PB15: Input mode, OD
    // PB14: Output mode, PP, init high
    // PB13: Alt func mode, OD, I2C2_SCL = AF5
    // PB11: Alt func mode, OD, I2C2_SDA = AF1
    // PC0: Output mode, PP, init high

}

void init_User_Button(GPIO_TypeDef  *GPIOx)
{
    // BUTTON PA0

    GPIOx->MODER &= ~((1<<0) | (1<<1) ); // 
    //assert(GPIOx->MODER == 0b0000000000000000000000000000000);

    GPIOx->OSPEEDR &= ~((1<<0) | (1<<1) );
    //assert(GPIOx->OSPEEDR == 0b0000000000000000000000000000000);

    GPIOx->PUPDR |= 0b0000000000000000000000000000010;
    GPIOx->PUPDR &= ~((1<<0));
    //assert(GPIOx->PUPDR == 0b0000000000000000000000000000010);
}

void My_HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
    if(PinState)
    {
        GPIOx->ODR |= GPIO_Pin;
    }
    else
    {
        GPIOx->ODR &= ~GPIO_Pin;
    }
}


GPIO_PinState My_HAL_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{

    // IDR bit array: GPIOx->IDR 
    
    //Isolate relevant bit

    uint16_t isolated_state = (GPIOx->IDR)&GPIO_Pin;

    if(isolated_state == GPIO_Pin)
    {
        return GPIO_PIN_SET;
    }
    else
    {
        return GPIO_PIN_RESET;
    }

    return -1;
}



void My_HAL_GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{

    GPIOx->ODR ^= GPIO_Pin;
}

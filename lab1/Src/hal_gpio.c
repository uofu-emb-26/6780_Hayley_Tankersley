#include <stdint.h>
#include <stm32f0xx_hal.h>
#include <stm32f0xx_hal_gpio.h>
#include "assert.h"

void My_HAL_RCC_GPIOC_CLK_Enable()
{
    //RCC->AHBENR |= RCC_AHBENR_GPIOBEN // Enable peripheral clock to TIMER2 GPIOB

    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;


}


void My_HAL_GPIO_Init(GPIO_TypeDef  *GPIOx) /*, GPIO_InitTypeDef *GPIO_Init */
{
    // Modify PC8, PC9

    // MODER -> General-purpose output mode [01]

    GPIOx->MODER |= 0b00000000000000000101000000000000;


    assert(GPIOx->MODER == 0b00000000000000000101000000000000);

    // OTYPER -> Push-pull output type [0]

    GPIOx->OTYPER |=0b0000000000000000;
    assert(GPIOx->OTYPER ==0b0000000000000000);

    // OSPEEDR -> low speed [x0]
    GPIOx->OSPEEDR |= 0b00000000000000000000000000000000;
    assert(GPIOx->OSPEEDR == 0b00000000000000000000000000000000);

    // PUPDR -> no pull-up/down [00]

    GPIOx->PUPDR |= 0b00000000000000000000000000000000;
    assert(GPIOx->PUPDR == 0b00000000000000000000000000000000);
    
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

/*
void My_HAL_GPIO_DeInit(GPIO_TypeDef  *GPIOx, uint32_t GPIO_Pin)
{
}
*/

/*
GPIO_PinState My_HAL_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    return -1;
}
*/


void My_HAL_GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    // 0b 0000 0001 0000 0000 GPIO_PIN_8
    // 0b 0000 0010 0000 0000 GPIO_PIN_9

    GPIOx->ODR ^= GPIO_Pin;


}


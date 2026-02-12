#include <stdint.h>
#include <stm32f0xx_hal.h>
#include <stm32f0xx_hal_gpio.h>
#include "assert.h"
int32_t get_2bit_pin_mask(uint32_t GPIO_Pin);
void My_HAL_RCC_GPIOC_CLK_Enable(void)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

    // Enable Timer 2,3 RCC
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

}


void My_HAL_GPIO_Init(GPIO_TypeDef  *GPIOx, GPIO_InitTypeDef *GPIO_Init) /*, GPIO_InitTypeDef *GPIO_Init */
{

    GPIOx->MODER |= 0b00000000000001010101000000000000;
    assert(GPIOx->MODER == 0b00000000000001010101000000000000);

    // OTYPER -> Push-pull output type [0]

    GPIOx->OTYPER &=0b0000000000000000; //0b0000000000000000;
    assert(GPIOx->OTYPER ==0b0000000000000000);

    // OSPEEDR -> low speed [x0]
    GPIOx->OSPEEDR |= (GPIO_Init->Speed & GPIO_Init->Pin); 
    assert(GPIOx->OSPEEDR == 0b00000000000000000000000000000000);

    // PUPDR -> no pull-up/down [00]

    GPIOx->PUPDR |= (GPIO_Init->Pull & GPIO_Init->Pin); 
    assert(GPIOx->PUPDR == 0b00000000000000000000000000000000);

    
    
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

/*
void My_HAL_GPIO_DeInit(GPIO_TypeDef  *GPIOx, uint32_t GPIO_Pin)
{
}
*/


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

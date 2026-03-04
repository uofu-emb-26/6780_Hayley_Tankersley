#include <stdint.h>

void InitI2C(void)
{
  // Enable RCC clock for I2C GPIO pins

  // Configure I2C GPIO pins
    // output type: OD
    // alternate function

  // Enable RCC clock for I2C system

  // Configure bus iming using I2Cx_TIMINGR register
    // We want standard mode (Sm) at 100kHz

    // PRESC = 1

    // SCLL = 0x13

    // SCLH = 0xF

    // SDADEL = 0x2

    // SCLDEL = 0x4

  // Enable I2C Peripheral using CR1 register PE bit
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

}

void ReceiveI2C(void)
{
    
}
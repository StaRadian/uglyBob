#include "IRQHandler.h"

// IRQHANDLER::SPI03::SPI03(GPIO_TypeDef* _CE_base, const uint32_t _CE_pin)
//     : m_CE_base(_CE_base), m_CE_pin(_CE_pin) { }

// void SPI3_IRQHandler(void) {
//     if(SPI3->SR & 0x01) //RX
//     {
//         spi3_rx = SPI3->DR;
//         SPI3->DR = 0;
//     }
//     else    //TX
//     {
//         SPI3->DR = 
//     }
// }
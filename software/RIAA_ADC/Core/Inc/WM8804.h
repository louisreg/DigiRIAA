/*
 * WM8804.h
 *
 *  Created on: Sep 30, 2022
 *      Author: louisregnacq
 */

#ifndef INC_WM8804_H_
#define INC_WM8804_H_


#include "stm32f1xx_hal.h"

class WM8804 {
  private:
	SPI_HandleTypeDef *SPI;
	GPIO_TypeDef *CS_port;
	uint16_t CS_pin;
	GPIO_TypeDef *RES_port;
	uint16_t RES_pin;
	void SPIselect();
	void SPIunselect();
	uint8_t clearBit(uint8_t bit, uint8_t add, uint8_t val);
	uint8_t setBit(uint8_t bit, uint8_t add, uint8_t val);

	uint8_t PWRDN;
	uint8_t SPDTX;
	uint8_t AIFRX;
	uint8_t PLL1;
	uint8_t PLL2;
	uint8_t PLL3;
	uint8_t PLL4;
	uint8_t PLL5;



  public:

	WM8804() = default;
	WM8804(SPI_HandleTypeDef* SPI,GPIO_TypeDef* CS_port,uint16_t CS_pin,GPIO_TypeDef* RES_port,uint16_t RES_pin);
	void reset();
	void enable();
	void writeReg(uint8_t reg,uint8_t val);
	uint8_t readReg(uint8_t reg);

	void enableOSC(bool enable);
	void enablePLL(bool enable);
	void enableSpdifRX(bool enable);
	void enableSpdifTX(bool enable);
	void enableAIF(bool enable);
	void setFreqMode(uint8_t div);
	void setSpidfTX_src(uint8_t source);
	void AIF_MasterMode(bool enable);
	void enableMCLKDIV(bool enable);
	void setPLL(uint32_t K, uint8_t N);
	void setAIFsize(uint8_t size);

	void init();

};


#define SPDIF_src	 	0
#define AIF_src 		1

#define PWRDN_ADD 			0x1E
#define PWRDN_Default 		0x07
#define enablePLL_bit 		0
#define enableSpdifRX_bit 	1
#define enableSpdifTX_bit 	2
#define enableOSC_bit 		3
#define AIF_bit				4

#define SPDTX4_ADD			0x15
#define SPDTX4_default		0x71
#define TXSRC_bit			6

#define AIFRX_ADD			0x1C
#define AIFRX_default		0x06
#define AIF_MS_bit			6
#define AIFRX_WL_bit		2
#define AIFRX_WL_mask		0x03

#define PLL1_ADD			0x03
#define PLL2_ADD			0x04
#define PLL3_ADD			0x05
#define PLL4_ADD			0x06
#define PLL5_ADD			0x07

#define PLL1_default		0x21
#define PLL2_default		0xFD
#define PLL3_default		0x36
#define PLL4_default		0x07
#define PLL5_default		0x16

#define FREQMODE_bit		0
#define FREQMODE_mask		0x03
#define MCLKDIV_bit			3

#define PLL_N_mask 			0x0F

#endif /* INC_WM8804_H_ */

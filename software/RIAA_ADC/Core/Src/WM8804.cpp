/*
 * WM8804.cpp
 *
 *  Created on: Sep 30, 2022
 *      Author: louisregnacq
 */


#include <WM8804.h>


WM8804::WM8804(SPI_HandleTypeDef *SPI,GPIO_TypeDef *CS_port,uint16_t CS_pin,GPIO_TypeDef* RES_port,uint16_t RES_pin){

  this->SPI = SPI;
  this->CS_port = CS_port;
  this->CS_pin = CS_pin;
  this->RES_port = RES_port;
  this->RES_pin = RES_pin;

  this -> PWRDN = PWRDN_Default;
  this -> SPDTX = SPDTX4_default;
  this -> AIFRX = AIFRX_default;
  this -> PLL1 = PLL1_default;
  this -> PLL2 = PLL2_default;
  this -> PLL3 = PLL3_default;
  this -> PLL4 = PLL4_default;
  this -> PLL5 = PLL5_default;

}

void WM8804::SPIselect(){
	HAL_GPIO_WritePin(this->CS_port, this->CS_pin, GPIO_PIN_RESET);
}

void WM8804::SPIunselect(){
	HAL_GPIO_WritePin(this->CS_port, this->CS_pin, GPIO_PIN_SET);
}


void WM8804::reset(){
	HAL_GPIO_WritePin(this->RES_port, this->RES_pin, GPIO_PIN_RESET);
}

void WM8804::enable(){
	HAL_GPIO_WritePin(this->RES_port, this->RES_pin, GPIO_PIN_SET);
}

void WM8804::writeReg(uint8_t reg,uint8_t val){

	this->SPIselect();
	uint8_t buff[2];
	buff[0] = reg;
	buff[1] = val;
	HAL_SPI_Transmit(this->SPI, buff, sizeof(buff), 1);
	this->SPIunselect();
}

uint8_t WM8804::readReg(uint8_t reg){
	this->SPIselect();
	reg += 0x80; //a verifier ça
	uint8_t reg_val = 1;
	HAL_SPI_Transmit(this->SPI, (uint8_t*)(&reg), 1, 1);
	HAL_SPI_Receive(this->SPI, (uint8_t*) &reg_val,1, 1);
	this->SPIunselect();
	return(reg_val);
}

uint8_t WM8804::setBit(uint8_t bit, uint8_t add, uint8_t val){
	val |= 1 << bit;
	this->writeReg(add,val);
	return (val);
}

uint8_t WM8804::clearBit(uint8_t bit, uint8_t add, uint8_t val){
	val &= ~(1 << bit);
	this->writeReg(add,val);
	return (val);
}


void WM8804::enableOSC(bool enable){
	if (enable)
		this -> PWRDN =  this -> clearBit(enableOSC_bit,PWRDN_ADD,this -> PWRDN);		//PLL is enable when enablePLL_bit is low
	else
		this -> PWRDN =  this -> setBit(enableOSC_bit,PWRDN_ADD,this -> PWRDN);
}


void WM8804::enablePLL(bool enable){
	if (enable)
		this -> PWRDN =  this -> clearBit(enablePLL_bit,PWRDN_ADD,this -> PWRDN);		//PLL is enable when enablePLL_bit is low
	else
		this -> PWRDN =  this -> setBit(enablePLL_bit,PWRDN_ADD,this -> PWRDN);
}



void WM8804::enableSpdifRX(bool enable){
	if (enable)
		this -> PWRDN =  this -> clearBit(enableSpdifRX_bit,PWRDN_ADD,this -> PWRDN);	//enableSpdifRX is enable when enableSpdifRX_bit_bit is low
	else
		this -> PWRDN =  this -> setBit(enableSpdifRX_bit,PWRDN_ADD,this -> PWRDN);
}

void WM8804::enableSpdifTX(bool enable){
	if (enable)
		this -> PWRDN =  this -> clearBit(enableSpdifTX_bit,PWRDN_ADD,this -> PWRDN);	//enableSpdifTX is enable when enableSpdifTX_bit_bit is low
	else
		this -> PWRDN =  this -> setBit(enableSpdifTX_bit,PWRDN_ADD,this -> PWRDN);
}

void WM8804::enableAIF(bool enable){
	if (enable)
		this -> PWRDN =  this -> clearBit(AIF_bit,PWRDN_ADD,this -> PWRDN);				//AIF is enable when AIF_bit is low
	else
		this -> PWRDN =  this -> setBit(AIF_bit,PWRDN_ADD,this -> PWRDN);
}

void WM8804::setSpidfTX_src(uint8_t source){
	if (SPDIF_src)
		this -> SPDTX =  this -> clearBit(TXSRC_bit,SPDTX4_ADD,this -> SPDTX);
	else
		this -> SPDTX =  this -> setBit(TXSRC_bit,SPDTX4_ADD,this -> SPDTX);
}

void WM8804::AIF_MasterMode(bool enable){
	if (enable)
		this -> AIFRX =  this -> setBit(AIF_MS_bit,AIFRX_ADD,this -> AIFRX);
	else
		this -> AIFRX =  this -> clearBit(AIF_MS_bit,AIFRX_ADD,this -> AIFRX);	//else AIF is in slave mode


}

void WM8804::setFreqMode(uint8_t div){
	this -> PLL5 = (this -> PLL5 & ~FREQMODE_mask) | (div & FREQMODE_mask);
	this->writeReg(PLL5_ADD,this -> PLL5);

}

void WM8804::enableMCLKDIV(bool enable){
	if (enable)
		this -> PLL5 =  this -> setBit(MCLKDIV_bit,PLL5_ADD,this -> PLL5);
	else
		this -> PLL5 =  this -> clearBit(MCLKDIV_bit,PLL5_ADD,this -> PLL5);


}

void WM8804::setAIFsize(uint8_t size){
	this -> AIFRX = (this -> AIFRX & ~AIFRX_WL_mask) | (size & AIFRX_WL_mask);
	this->writeReg(AIFRX_ADD,this -> AIFRX);
}


void WM8804::setPLL(uint32_t K, uint8_t N){
	this -> PLL1 = K & 0xFF;
	this -> PLL2 = (K>>8) & 0xFF;
	this -> PLL3 = (K>>16) & 0xFF;
	this -> PLL4 = (this -> PLL4 & ~PLL_N_mask) | (N & PLL_N_mask);

	this->writeReg(PLL1_ADD,this -> PLL1);
	this->writeReg(PLL2_ADD,this -> PLL2);
	this->writeReg(PLL3_ADD,this -> PLL3);
	this->writeReg(PLL4_ADD,this -> PLL4);


}

void WM8804::init(){
	this->SPIunselect();
	this->reset();
	HAL_Delay(1);
	this->enable();
	HAL_Delay(1);
	this->writeReg(0,0); 	//Dummy read, apparently needed

}





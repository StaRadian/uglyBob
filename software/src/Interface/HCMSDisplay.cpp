#include "HCMSDisplay.h"

//constructor Init
void HCMSDisplay::Init( const uint8_t brightness_percent, 
    const uint8_t peak_current_percent, 
    const bool sleep_mode) {
	CE_H();
	RS_L();
	CLK_H();

	CtrlRegLoad( 0b1000001 );
	CtrlRegSet( brightness_percent, peak_current_percent, sleep_mode );
}

/*
	7: 0 -> Select Control
	6: Sleep mode
	5:4 Peak Current Brightness Control
	3:0 PWM Brightness Control
*/
void HCMSDisplay::CtrlRegLoad( uint8_t ctrl_reg ) {
    RS_H();
    CLK_H();
    CE_L();
	
	//Load control register
	for( int16_t cnt = 0 ; cnt < 8 ; cnt++ ) {
		CLK_L();
		
		if( ctrl_reg & 0x80 )	DA_H();
		else					DA_L();

		CLK_H();
		ctrl_reg <<= 1;
	}
	
	//Latch data to control word
	CE_H();
	CLK_L();
	RS_L();
}

void HCMSDisplay::DataLoad( const std::string& str ) {
    CLK_H();
    CE_L();
    for( int16_t i = 0 ; i < m_section ; i++ )
	{
        char str_temp = str[i];
        for( int16_t j = 4 ; j >= 0 ; j-- ) {
            CLK_L();
            uint8_t data = FontData7x5[str_temp][j];
            CLK_H();
        
            for( int16_t k = 0 ; k < 7 ; k++) {
                CLK_L();
                if(data & 0x01)	DA_H();
                else			DA_L();
                CLK_H();
                data >>= 1;
            }
        }
    }
    CE_H();
    CLK_L();
}

void HCMSDisplay::CtrlRegSet(
    const uint8_t brightness_percent, 
    const uint8_t peak_current_percent, 
    const bool sleep_mode ) {
    uint8_t result;
    if(sleep_mode)  result = 0b00000000;
    else            result = 0b01000000;

    if(brightness_percent < 3.3)        result |= 0b00000001;
    else if(brightness_percent < 5.0)   result |= 0b00000010;
    else if(brightness_percent < 6.7)   result |= 0b00000011;
    else if(brightness_percent < 8.3)   result |= 0b00000100;
    else if(brightness_percent < 11.7)  result |= 0b00000101;
    else if(brightness_percent < 15)    result |= 0b00000110;
    else if(brightness_percent < 18)    result |= 0b00000111;
    else if(brightness_percent < 23)    result |= 0b00001000;
    else if(brightness_percent < 30)    result |= 0b00001001;
    else if(brightness_percent < 37)    result |= 0b00001010;
    else if(brightness_percent < 47)    result |= 0b00001011;
    else if(brightness_percent < 60)    result |= 0b00001100;
    else if(brightness_percent < 80)    result |= 0b00001101;
    else if(brightness_percent < 100)   result |= 0b00001110;
    else if(brightness_percent == 100)  result |= 0b00001111;

    if(peak_current_percent < 50)           result |= 0b00100000;   // 4.0mA
    else if(peak_current_percent < 73)      result |= 0b00010000;   // 6.4mA
    else if(peak_current_percent < 100);  //result |= 0b00000000;   // 9.3mA
    else if(peak_current_percent == 100)    result |= 0b00110000;   // 12.8mA

    m_reg = result;
    CtrlRegLoad(result);
}

void HCMSDisplay::Print( const std::string& text ) {
    DataLoad( text );
}

void HCMSDisplay::Print( const int data ) {
    std::string str = std::to_string(data);

    int16_t paddingSize = 4 - str.length();
    if (paddingSize > 0) {
        str.insert(0, paddingSize, ' ');
    }

    DataLoad(str);
}
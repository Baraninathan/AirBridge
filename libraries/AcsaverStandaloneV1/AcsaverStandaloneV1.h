#ifndef AcsaverStandaloneV1_h
#define AcsaverStandaloneV1_h
#include <Arduino.h>
#include <EEPROM.h>
#include <pgmspace.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <IRsend.h>


#define khz 38// Frequenz of Infrared
#define tolerance 100// Tolerance of ir-rx



//--------------------------------------------------------------------------------- All Driver saved in PROGMEM

// heier
const char driver_0[] PROGMEM = "haier,code_format=MSB,code_long=112,additional_mark/space_cout=2,additional_mark/space_pos=:0.:0.,additional_mark=:3100.:3100.,additional_space=:3000.:4300.,bit_mark=590,one_space=1550,zero_space=500,check_sum_type=calc_every_8_bit,check_sum_pos=105,additional_check_sum=0,check_sum_long=8,on/off_pos=33,on/off_length=2,ON=01,OFF=00,mode_pos=57,mode_length=4,AUTO=0000,COOL=0010,DRY=0100,FAN=1100,temp_pos=9,temp_length=4,T30=1110,T29=1101,T28=1100,T27=1011,T26=1010,T25=1001,T24=1000,T23=0111,T22=0110,T21=0101,T20=0100,T19=0011,T18=0010,T17=0001,T16=0000,fan_pos=41,fan_length=4,FA=1010,F1=0010,F2=0100,F3=0110,vane_pos=13,vane_length=8,VH=00001110,V1=00100000,V2=01000000,V3=01100000,V4=10000000,prefix_bit=10100110xxxxxxxx0000101100000000xx111101xxxx000000000000xxxx00000000000000000000000000000000000000000000xxxxxxxx,";

// lg
const char driver_1[] PROGMEM = "lg,code_format=MSB,code_long=28,additional_mark/space_cout=1,additional_mark/space_pos=:0.,additional_mark=:8540.,additional_space=:4200.,bit_mark=570,one_space=1560,zero_space=520,check_sum_type=calc_every_8_bit,check_sum_pos=25,additional_check_sum=0,check_sum_long=4,on/off_pos=9,on/off_length=2,ON=00,OFF=11,mode_pos=13,mode_length=4,AUTO=1011,AUTO2=0011,COOL=1000,COOL2=0000,DRY=1001,DRY2=0001,temp_pos=17,temp_length=4,T31=0000,T30=1111,T29=1110,T28=1101,T27=1100,T26=1011,T25=1010,T24=1001,T23=1000,T22=0111,T21=0110,T20=0101,T19=0100,T18=0011,T++=0100,T+=0011,T*=0010,T-=0001,T--=0000,fan_pos=21,fan_length=4,FA=0101,F1=0000,F2=0010,F3=0100,vane_pos=8,vane_length=1,VR=0,prefix_bit=100010000000000010110101000X,";

// mitsubishi
const char driver_2[] PROGMEM = "mitsubishi,code_format=LSB8,code_long=112,additional_mark/space_cout=1,additional_mark/space_pos=:0.,additional_mark=:3400.,additional_space=:1750.,bit_mark=450,one_space=1300,zero_space=420,check_sum_type=calc_every_8_bit,check_sum_pos=105,additional_check_sum=0,check_sum_long=8,on/off_pos=41,on/off_length=8,ON=10100100,OFF=10100000,mode_pos=49,mode_length=8,AUTO=00001000,COOL=00000011,DRY=00000010,FAN=00000111,temp_pos=57,temp_length=8,T31=00011111,T30=00011110,T29=00011101,T28=00011100,T27=00011011,T26=00011010,T25=00011001,T24=00011000,T23=00010111,T22=00010110,T21=00010101,T20=00010100,T19=00010011,T18=00010010,T17=00010001,T16=00010000,T15=00001111,fan_pos=70,fan_length=3,FA=000,F1=010,F2=011,F3=101,vane_pos=65,vane_length=5,VA=00000,V1=00001,V2=00010,V3=00011,V4=00100,V5=00101,VR=00111,prefix_bit=0010001111001011001001100000000100000000000000000000000000000000000000000000000000000000000000000000000000000000,";

// panasonic
const char driver_3[] PROGMEM = "panasonic,code_format=LSB8,code_long=216,additional_mark/space_cout=2,additional_mark/space_pos=:0.:65.,additional_mark=:3550.:480.,additional_space=:1650.:76900.,bit_mark=480,one_space=1280,zero_space=400,check_sum_type=calc_every_8_bit,check_sum_pos=209,additional_check_sum=-12,check_sum_long=8,on/off_pos=112,on/off_length=1,ON=1,OFF=0,mode_pos=105,mode_length=4,AUTO=0000,COOL=0011,DRY=0100,FAN=0010,temp_pos=113,temp_length=8,T31=00111110,T30=00111100,T29=00111010,T28=00111000,T27=00110110,T26=00110100,T25=00110010,T24=00110000,T23=00101110,T22=00101100,T21=00101010,T20=00101000,T19=00100110,T18=00100100,T17=00100010,T16=00100000,fan_pos=129,fan_length=4,FA=1010,F1=0011,F2=0101,F3=0111,vane_pos=133,vane_length=4,VA=1111,V1=0101,V2=0100,V3=0011,V4=0010,V5=0001,prefix_bit=000000100010000011100000000001000000000000000000000000000000011000000010001000001110000000000100000000000011100000110010100000001010111100000000000000000000111011100000000000000000000010000001000000000000000000001110,";

// turbocool
const char driver_4[] PROGMEM = "turbocool,code_format=LSB8,code_long=112,additional_mark/space_cout=1,additional_mark/space_pos=:0.,additional_mark=:3150.,additional_space=:1580.,bit_mark=535,one_space=1050,zero_space=280,check_sum_type=calc_every_8_bit,check_sum_pos=105,additional_check_sum=13,check_sum_long=8,on/off_pos=45,on/off_length=4,ON=1011,OFF=1111,mode_pos=53,mode_length=4,AUTO=0111,COOL=1100,DRY=1101,FAN=1000,Light=1110,temp_pos=61,temp_length=4,T31=1111,T30=1110,T29=1101,T28=1100,T27=1011,T26=1010,T25=1001,T24=1000,T23=0111,T22=0110,T21=0101,T20=0100,T19=0011,T18=0010,T17=0001,T16=0000,fan_pos=70,fan_length=3,FA=111,F1=101,F2=100,F3=010,vane_pos=65,vane_length=5,VR=10111,V1=10000,V2=10000,V3=10000,V4=10000,V5=10111,VA=10000,prefix_bit=11011100001101001101100111111110111111111101xxxx1111xxxx1111xxxxxxxxxxxx11111111111111111111111101111111xxxxxxxx,";

// daikin
const char driver_5[] PROGMEM = "daikin,code_format=LSB8,code_long=64,additional_mark/space_cout=3,additional_mark/space_pos=:0.:0.:0.,additional_mark=:9820.:9820.:4650.,additional_space=:9780.:9780.:2450.,bit_mark=410,one_space=910,zero_space=330,check_sum_type=calc_every_4_bit,check_sum_pos=57,additional_check_sum=4,check_sum_calc_until_bit=56,check_sum_long=4,on/off_pos=61,on/off_length=2,ON=11,OFF=01,mode_pos=13,mode_length=4,COOL=0010,DRY=0001,FAN=0100,temp_pos=49,temp_length=8,T30=00110000,T29=00101001,T28=00101000,T27=00100111,T26=00100110,T25=00100101,T24=00100100,T23=00100011,T22=00100010,T21=00100001,T20=00100000,T19=00011001,T18=00011000,T17=00010111,T16=00010110,fan_pos=9,fan_length=4,FA=0001,F1=0010,F2=0100,F3=1000,FT=0011,vane_pos=63,vane_length=2,V1=01,V2=01,V3=01,V4=01,V5=01,VH=00,prefix_bit=00010110xxxxxxxx00000000000000000001000001010011xxxxxxxxxxxxxxxx,";

// uni-air, *waiting for check_sum
//const char driver_6[] PROGMEM = "uni-air,code_format=LSB8,code_long=67,additional_mark/space_cout=2,additional_mark/space_pos=:0.:35.,additional_mark=:9000.:670.,additional_space=:4400.:19870.,bit_mark=680,one_space=1610,zero_space=520,check_sum_type=calc_every_8_bit,check_sum_pos=105,additional_check_sum=13,check_sum_long=8,on/off_pos=45,on/off_length=4,ON=1011,OFF=1111,mode_pos=53,mode_length=4,AUTO=0111,COOL=1100,DRY=1101,FAN=1000,Light=1110,temp_pos=61,temp_length=4,T31=1111,T30=1110,T29=1101,T28=1100,T27=1011,T26=1010,T25=1001,T24=1000,T23=0111,T22=0110,T21=0101,T20=0100,T19=0011,T18=0010,T17=0001,T16=0000,fan_pos=70,fan_length=3,FA=111,F1=101,F2=100,F3=010,vane_pos=65,vane_length=5,VR=10111,V1=10000,V2=10000,V3=10000,V4=10000,V5=10111,VA=10000,prefix_bit=11011100001101001101100111111110111111111101xxxx1111xxxx1111xxxxxxxxxxxx11111111111111111111111101111111xxxxxxxx,";

// table of driver
const char* const driver_table[] PROGMEM = {driver_0, driver_1 ,driver_2, driver_3, driver_4, driver_5};
//All Driver saved in PROGMEM


class acsaver {
	String driver, driver_name, ac_status, AC_State;
	//int tolerance = 200;
	public:
		void set_driver(String);
		void eeprom(char, char, String);
		void ir_send(String);
		void ir_send_final(String);
		void ir_translate(String, int);
		String driver_read(String);
		String ac_now();
		void server();		
};

#endif	

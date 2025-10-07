#include "AcsaverStandaloneV1.h"


//---------------------------------------------------------------------------- set driver
void acsaver::set_driver(String driver_name_from_user){
	String _temp;//temp
	int i;
	if (driver_name_from_user.length() !=0){
        for (i = 0; i < sizeof(driver_table)/4; i++) {
          if (String(FPSTR(driver_table[i])).indexOf(driver_name_from_user) != -1){//found driver in table
            driver_name = driver_name_from_user;
            //-------------------------------------------------------------------------------- Find Driver_Position and Write to EEPROM
            eeprom('w','d',String(FPSTR(driver_table[i])));
            //--------------------------------------------------------------------------------
      
            Serial.println("-------------------------------------------");
            Serial.println("Setup a new Driver ---> Success !!");
            Serial.println("-------------------------------------------");
            break;
          }
          if (i == (sizeof(driver_table)/4)-1) {
            
          Serial.println("-------------------------------------------");
          Serial.println("Cannot find Driver, --> Restart !!");
          Serial.println("-------------------------------------------");   
          ESP.restart();//if no setup driver
          }
        }
      }
      else {//not found driver in table, then read from eeprom
          for (i = 64; i < 96; ++i){
          _temp += char(EEPROM.read(i));
        }
        _temp = _temp.substring(0,_temp.indexOf(","));
        if (_temp.length() != 0) {
          driver_name = _temp;
          Serial.println("-------------------------------------------");
          Serial.print("AC-Saver have driver ---> ");
          Serial.println(driver_name);
          Serial.println("-------------------------------------------");      
        }
        else{//not found driver in eeprom, then restart
          Serial.println("-------------------------------------------");
          Serial.println("No Driver !!, Please setup AC-Saver after next restart !!");
          Serial.println("-------------------------------------------");
          ESP.restart();//if no setup driver
        }
      }
}
//set_driver

//---------------------------------------------------------------------------- EEPROM_RWC, reading, writing and clear eeprom
void acsaver::eeprom(char command, char var, String value) {
  String _temp;//temp
  int i;
  switch (command) {
    case 'r':
      switch (var) {
        case 'd':
          for (i = 64; i < 1024; ++i){
            _temp += char(EEPROM.read(i));
            if (char(EEPROM.read(i)) == ';'){
              break;
            }
            }
            driver = _temp;
            Serial.println("-------------------------------------------");              
            Serial.print("Driver: ");
            Serial.println(driver);
            Serial.println("-------------------------------------------");  
          break;
        case 's':
        
          break;
        case 'p':
        
          break;
      }
      break;
    case 'w':
      switch (var) {
        case 'd':
          //------------------------------------------------------------ write driver (64:511) - process
          /*
           * clear then write
           * start position = 64
          */
          for (i = 64; i < 1024; ++i) {
            EEPROM.write(i, 0); 
          }
          EEPROM.commit();
          Serial.println("-------------------------------------------");          
          Serial.print("Wrote: ");
          for (i = 64; i < value.length()+64; ++i)
            {
              EEPROM.write(i, value[i-64]);
              Serial.print(value[i-64]); 
            }
          EEPROM.commit();
          //------------------------------------------------------------- End process 


          //------------------------------------------------------------- Des
          Serial.println("Writing Driver in EEPROM ---> Success !!");
          Serial.println("-------------------------------------------");
          //------------------------------------------------------------- End des
          
          break;
        case 's':
          //------------------------------------------------------------ write ssid (0:31) - process
          /*
           * clear then write
           * start position = 0
          */
          for (i = 0; i < 32; ++i) {
            EEPROM.write(i, 0); 
          }
          EEPROM.commit();
          Serial.println("-------------------------------------------");          
          Serial.print("Wrote: ");
          for (i = 0; i < value.length(); ++i)
            {
              EEPROM.write(i, value[i]);
              Serial.print(value[i]); 
            }
          EEPROM.commit();
          //------------------------------------------------------------- End process 


          //------------------------------------------------------------- Des
          Serial.println();
          Serial.println("Writing SSID in EEPROM ---> Success !!");
          Serial.println("-------------------------------------------");
          //------------------------------------------------------------- End des        
        
          break;
        case 'p':
          //------------------------------------------------------------ write pass (32:63) - process
          /*
           * clear then write
           * start position = 32
          */
          for (i = 32; i < 64; ++i) {
            EEPROM.write(i, 0); 
          }
          EEPROM.commit();
          Serial.println("-------------------------------------------");          
          Serial.print("Wrote: ");
          for (i = 32; i < value.length()+32; ++i)
            {
              EEPROM.write(i, value[i-32]);
              Serial.print(value[i-32]); 
            }
          EEPROM.commit();
          //------------------------------------------------------------- End process 


          //------------------------------------------------------------- Des
          Serial.println("Writing WiFi-Password in EEPROM ---> Success !!");
          Serial.println("-------------------------------------------");
          //------------------------------------------------------------- End des        
                
          break;
      }
      break;
    case 'c':
      switch (var) {
        case 'd':
        
          break;
        case 's':
        
          break;
        case 'p':
        
          break;
      }
      break;
    default:

      break;
  }
}
//eeprom

//---------------------------------------------------------------------------- driver_calc
void acsaver::ir_send(String ac_status_from_user){
  
  //--------------------------------------------------------\\
  //This function will calulate a bit_code before sending it\\
  //--------------------------------------------------------\\
  
	int pos_of_parm;//index of parameter
  	int pos_of_comma;//index of next ,
  	String val_of_parm;//value of parameter
  	String bit_code;//bit_code
  	String bit_code_temp;//bit_code_temp
  	int i, j;

  	bit_code = driver_read("prefix_bit");// prefix_bit

  	//---------------------------------------------------------- ON/OFF
  	bit_code_temp = bit_code.substring(0, driver_read("on/off_pos").toInt()-1);//replacing   
  	pos_of_comma = ac_status_from_user.indexOf(",");//read_status
  	val_of_parm = ac_status_from_user.substring(0,pos_of_comma);//power_status ON or OFF
  	if (driver_read(val_of_parm).length() <= 8){  				
		bit_code_temp += driver_read(val_of_parm);	  
	}
	else{
		Serial.print("no function > ");
		Serial.println(val_of_parm);
		return;
	} 
  	bit_code_temp += bit_code.substring(bit_code_temp.length());
  	bit_code = bit_code_temp;
  	//ON/OFF

  	//---------------------------------------------------------- MODE
  	bit_code_temp = bit_code.substring(0,driver_read("mode_pos").toInt()-1);//replacing
  	pos_of_comma = ac_status_from_user.indexOf(',');
  	ac_status_from_user = ac_status_from_user.substring(pos_of_comma+1);//There is only (MODE, TEMP, FAN, VANE)   
  	pos_of_comma = ac_status_from_user.indexOf(',');
  	val_of_parm = ac_status_from_user.substring(0,pos_of_comma);//power_status ON or OFF
  	if (driver_read(val_of_parm).length() <= 8){  				
		bit_code_temp += driver_read(val_of_parm);	  
	}
	else{
		Serial.print("no function > ");
		Serial.println(val_of_parm);
		return;
	}        
  	bit_code_temp += bit_code.substring(bit_code_temp.length());
  	bit_code = bit_code_temp;
  	//MODE
    
  	//---------------------------------------------------------- TEMP
  	bit_code_temp = bit_code.substring(0,driver_read("temp_pos").toInt()-1);//replacing
  	pos_of_comma = ac_status_from_user.indexOf(',');
  	ac_status_from_user = ac_status_from_user.substring(pos_of_comma+1);//There is only (TEMP, FAN, VANE)      
  	pos_of_comma = ac_status_from_user.indexOf(',');
  	val_of_parm = ac_status_from_user.substring(0,pos_of_comma);//temp_status
  	if (driver_read(val_of_parm).length() <= 8){  				
		bit_code_temp += driver_read(val_of_parm);	  
	}
	else{
		Serial.print("no function > ");
		Serial.println(val_of_parm);
		return;
	}
	bit_code_temp += bit_code.substring(bit_code_temp.length());
  	bit_code = bit_code_temp;   
  	//TEMP
  
  	//---------------------------------------------------------- FAN
  	bit_code_temp = bit_code.substring(0,driver_read("fan_pos").toInt()-1);//replacing
  	pos_of_comma = ac_status_from_user.indexOf(',');
  	ac_status_from_user = ac_status_from_user.substring(pos_of_comma+1);//There is only (FAN, VANE)     
  	pos_of_comma = ac_status_from_user.indexOf(',');
  	val_of_parm = ac_status_from_user.substring(0,pos_of_comma);//
  	if (driver_read(val_of_parm).length() <= 8){  				
		bit_code_temp += driver_read(val_of_parm);	  
	}
	else{
		Serial.print("no function > ");
		Serial.println(val_of_parm);
		return;
	}     
  	bit_code_temp += bit_code.substring(bit_code_temp.length());
	bit_code = bit_code_temp;
  	//FAN
  
  	//---------------------------------------------------------- VANE
  	bit_code_temp = bit_code.substring(0,driver_read("vane_pos").toInt()-1);//replacing
  	pos_of_comma = ac_status_from_user.indexOf(',');
  	ac_status_from_user = ac_status_from_user.substring(pos_of_comma+1);//There is only (VANE)      
  	pos_of_comma = ac_status_from_user.indexOf(',');
  	val_of_parm = ac_status_from_user.substring(0,pos_of_comma);//
  	if (driver_read(val_of_parm).length() <= 8){  				
		bit_code_temp += driver_read(val_of_parm);	  
	}
	else{
		Serial.print("no function > ");
		Serial.println(val_of_parm);
		return;
	}
  	bit_code_temp += bit_code.substring(bit_code_temp.length());
  	bit_code = bit_code_temp;//code_without_check_sum       
  	//VANE

  	Serial.print("calc_code before check_sum: ");
  	Serial.println(bit_code);      

  	//------------------------------------------------------------- Check_sum
  	String check_sum_type, check_sum_pos, check_sum_long, code_long, _temp, sum_bin, additional_check_sum;
  	int sum = 0;//sum
  	check_sum_type = driver_read("check_sum_type");
  	check_sum_pos = driver_read("check_sum_pos");  
  	check_sum_long = driver_read("check_sum_long");
  	code_long = driver_read("code_long");
  	bit_code_temp = bit_code.substring(0,check_sum_pos.toInt()-1);//replacing  
  	if (check_sum_type == "calc_every_8_bit"){
    	for (i = 0; i < (code_long.toInt() - check_sum_long.toInt())/8; i++){
      		_temp = bit_code.substring(i*8,i*8+8);//copy_bit to calculate
      		for (j = 0; j < 8; j++) {
        		sum += _temp.substring(7-j,7-j+1).toInt()*pow(2,j);
      		}
    	}
  	}
  	else if (check_sum_type == "calc_every_4_bit"){
    	for (i = 0; i < (driver_read("check_sum_calc_until_bit").toInt())/4; i++){
      		_temp = bit_code.substring(i*4,i*4+4);//copy_bit to calculate
      		for (j = 0; j < 4; j++) {
        		sum += _temp.substring(3-j,3-j+1).toInt()*pow(2,j);
      	}
    	}
  	}
    
  	else if (check_sum_type == "sum_of_1"){
    	//????
  	}
  	sum += driver_read("additional_check_sum").toInt(); 
  	Serial.print("sum: ");
  	Serial.println(sum);
  	sum_bin = String(sum, BIN);
  	sum_bin = sum_bin.substring(String(sum_bin).length()-check_sum_long.toInt());
  	Serial.print("sum_bin: ");
  	Serial.println(sum_bin);
  	bit_code_temp += sum_bin;
  	bit_code_temp += bit_code.substring(bit_code_temp.length());
  	bit_code = bit_code_temp;//code_with_check_sum         
  	//Checksum

  	//--------------------------------------------------------------- MSB or LSB8
  	val_of_parm = driver_read("code_format");
  	if (val_of_parm == "LSB8"){
    	Serial.println("LSB8_code need to be transform in MSB");
    	bit_code_temp = "";
    	for (i = 0; i < code_long.toInt()/8; i++){
      		for (j = 0; j < 8; j++){
        	bit_code_temp += bit_code.substring(i*8, (i+1)*8).substring(7-j,7-j+1);
      		}		      
    	}
    	bit_code = bit_code_temp;
  	}
  	Serial.print("calc_code: ");
  	Serial.println(bit_code);  
  	//MSB or LSB8

  	ir_send_final(bit_code);
}
//ir_send

//---------------------------------------------------------------------------- bit_code_send
void acsaver::ir_send_final(String bit_code){
  
  //---------------------------------------------------\\
  //This function calculate IR_Array and send IR_Signal\\
  //---------------------------------------------------\\

  String additional_mark_space_cout,additional_mark_space_pos,additional_mark,additional_space, code_long, bit_mark, one_space, zero_space;//value of parameter  
  uint16_t *ir_array;
	int i, j, x;
  //---------------------------------------------------------------------------- Reading of Parameter
  additional_mark_space_cout = driver_read("additional_mark/space_cout");
  additional_mark_space_pos = driver_read("additional_mark/space_pos");
  additional_mark = driver_read("additional_mark");
  additional_space = driver_read("additional_space");
  code_long = driver_read("code_long");
  bit_mark = driver_read("bit_mark");
  one_space = driver_read("one_space");
  zero_space = driver_read("zero_space");
  //Reading of Parameter

       
  ir_array = (uint16_t*)malloc(((code_long.toInt() + additional_mark_space_cout.toInt())*2 +1)*sizeof(int));
  j = 0;//pointer of ir_array
  for (i = 0; i < code_long.toInt(); i++){
    while ((i == additional_mark_space_pos.substring(additional_mark_space_pos.indexOf(":")+1,additional_mark_space_pos.indexOf(".")-1).toInt()) && (additional_mark_space_pos.length() != 0)){
      ir_array[j] = additional_mark.substring(additional_mark.indexOf(":")+1,additional_mark.indexOf(".")).toInt() - 100;
      additional_mark = additional_mark.substring(additional_mark.indexOf(".")+1);//delete first mark
      j++;
      ir_array[j] = additional_space.substring(additional_space.indexOf(":")+1,additional_space.indexOf(".")).toInt() + 80;
      additional_space = additional_space.substring(additional_space.indexOf(".")+1);//delete first space
      j++;      
      additional_mark_space_pos = additional_mark_space_pos.substring(additional_mark_space_pos.indexOf(".")+1);//delete first pos
    }
    if (bit_code.substring(x,1) == "0"){
      ir_array[j] = bit_mark.toInt() - 100;
      j++;
      ir_array[j] = zero_space.toInt() + 80;
      j++;      
      bit_code = bit_code.substring(1);//delete first bit
    }
    else{
      ir_array[j] = bit_mark.toInt() - 100;
      j++;
      ir_array[j] = one_space.toInt() + 80;
      j++;      
      bit_code = bit_code.substring(1);//delete first bit
    }
  }
  ir_array[j] = bit_mark.toInt() - 100;//adding last mark
  j++;
  for (i = 0; i < (code_long.toInt() + additional_mark_space_cout.toInt())*2; i++) {
    Serial.print(ir_array[i]);
    Serial.print(",");
    }
	IRsend irsend(5);
	irsend.begin();	      
  irsend.sendRaw(ir_array, (code_long.toInt() + additional_mark_space_cout.toInt())*2+1, khz);//sending ir_signal
  free(ir_array);
}
//bit_code_send

////-------------------------------------------------------------------------- Start Driver_read, calculate, setup
void acsaver::ir_translate(String ir_signal, int bit_cout){
	/*1. Step deleteing Header&Space in mid*/
	
  int ir_signal_array[bit_cout], mark_array[bit_cout/2], space_array[bit_cout/2];
  int code_long;
  int i, j, x, y;  

  //----------------------------------------------------- String to int-array
  for (i = 0; i < bit_cout; i++) {
    ir_signal_array[i] = ir_signal.substring(0, ir_signal.indexOf(",")).toInt();
    ir_signal = ir_signal.substring(ir_signal.indexOf(",") + 1);       
  }
  //String to int-array
  
  //----------------------------------------------------- finding bit_mark&space
  for (i = 0; i < bit_cout/2; i++) {
    mark_array[i] = ir_signal_array[i*2];
	space_array[i] = ir_signal_array[i*2+1];      
  }
  
  int everage_mark;//bit_mark
  everage_mark = mark_array[0] ;
  int repeat_mark = 0;
  for (i = 0; i < bit_cout/2; i++) { 
  	    if (everage_mark - tolerance <= mark_array[i] <= everage_mark + tolerance){
  	    	repeat_mark = repeat_mark+1;
  	    	if (repeat_mark > 8){
  	    		break;
			  }
		  }
		else
			everage_mark = mark_array[i];  
	}//bit_mark
	
	int everage_space[4];
	everage_space[1] = 0,everage_space[2] = 0,everage_space[3] = 0;//bit_space
	everage_space[0] = space_array[0];
	for (i = 0; i < bit_cout/2; i++) {
		if (!(everage_space[0] - tolerance <= space_array[i] <= everage_space[0] + tolerance)){
			everage_space[1] = space_array[i];
			break;
		}		
	}
	for (i = 0; i < bit_cout/2; i++) {
		if (!(everage_space[0] - tolerance <= space_array[i] <= everage_space[0] + tolerance) && !(everage_space[1] - tolerance <= space_array[i] <= everage_space[1] + tolerance)){
			everage_space[2] = space_array[i];
			break;
		}
	}
	for (i = 0; i < bit_cout/2; i++) {
		if (!(everage_space[0] - tolerance <= space_array[i] <= everage_space[0] + tolerance) && !(everage_space[1] - tolerance <= space_array[i] <= everage_space[1] + tolerance)&& !(everage_space[2] - tolerance <= space_array[i] <= everage_space[2] + tolerance)){
			everage_space[3] = space_array[i];
			break;
		}
	}
	int zero_space = everage_space[0];
	for (i = 0; i < 4; i++){
		if ((zero_space > everage_space[i]) && (everage_space[i] != 0)){
			zero_space = everage_space[i];
		}
	}
	int one_space = everage_space[0];
	for (i = 0; i < 4; i++){
		if ((one_space > everage_space[i]) && (everage_space[i] != 0) && (everage_space[i] != zero_space)){
			one_space = everage_space[i];
		}
	}
	
	Serial.print("Bit_Mark = ");
	Serial.println(everage_mark);
	Serial.print("Zero_Space = ");
	Serial.println(zero_space);
	Serial.print("One_Space = ");
	Serial.println(one_space);
	//finding bit_mark&space			

	//--------------------------------------------------- signal to bit
	String bit_calc = "";
	for (i = 0; bit_cout/2; i++){
		if (zero_space - tolerance <= space_array[i] <= zero_space + tolerance){
			bit_calc += "0";
		}
		else if (one_space - tolerance <= space_array[i] <= one_space + tolerance){
			bit_calc += "1";
		}
		else{
			bit_calc += "?";
		}			
	}
	
	int repeat_bit=0, repeat_min = 8;//delete mark&space before header
	for (i = 0; bit_cout/2; i++){
		if ((bit_calc.substring(i) == "0") || (bit_calc.substring(i) == "1")) {
			repeat_min = repeat_min + 1;
		}
		else{
			if (repeat_min < 8){
				bit_calc = bit_calc.substring(i+1);
			}
		}
	}
	String bit_calc_new = "";
	for (i = 0; bit_calc.length(); i++){
		if (bit_calc.substring(i) == "0"){
			bit_calc_new += "0";
		}
		else if (bit_calc.substring(i) == "1"){
			bit_calc_new += "1";
		}
	}
	bit_calc = bit_calc_new;
	
	/*2. Step finding Driver*/
	String bit_code_temp = "";
  	code_long = bit_calc.length();
	switch (code_long){
		case 28:
			driver = "LG";break;
		case 56:
			driver = "Samsung";break;
		case 64:
			driver = "Daikin_Uni";break;
		case 96:
			driver = "Toshiba";break;
		case 104:
			driver = "Sharp";break;
		case 112:
			if (bit_calc.indexOf("111100100000") != -1){
				driver = "Carrier_Toshiba";break;
			}
			else if (bit_calc.indexOf("10100110") != -1){
				driver = "Heier";break;
			}
			else{
				String bit_code_temp = "";
			    for (i = 0; i < code_long; i++){
			      for (j = 0; j < 8; j++){
			        bit_code_temp += bit_calc.substring(i*8, (i+1)*8).substring(7-j,7-j+1);
			      }      
			    }
				}
				if (bit_code_temp.indexOf("1101110000110100") != -1){
					driver = "Turbocool";break;
				}
				else{
					driver = "Samsumg";break;
				}
		case 128:
			driver = "Panasonic_C2600"; break;		
		case 134:
			driver = "Mitsubishi_002CP"; break;
		case 144:
			driver = "Carrier_Toshiba";break;
		case 168:
			if (bit_calc.indexOf("111100100000") != -1){
				driver = "Carrier_Toshiba";break;
			}
			else{
				driver = "Samsung";break;
			}
		case 216:
		    for (i = 0; i < code_long; i++){
		      for (j = 0; j < 8; j++){
		        bit_code_temp += bit_calc.substring(i*8, (i+1)*8).substring(7-j,7-j+1);
		      }      
		    }
			if (bit_code_temp.indexOf("1101101000100111") != -1){
				driver = "Daikin_B71";break;
			}
			else{
				driver = "Panasonic_C2665";break;
			}
		case 280:
		    for (i = 0; i < code_long; i++){
		      for (j = 0; j < 8; j++){
		        bit_code_temp += bit_calc.substring(i*8, (i+1)*8).substring(7-j,7-j+1);
		      }      
		    }
			if (bit_code_temp.indexOf("1101101000100111") != -1){
				driver = "Daikin_B71";break;
			}
			else{
				driver = "Mitsubishi_Silver";break;
			}
		case 288:
			driver = "Mitsubishi_Air_12"; break;
		case 632:
			driver = "Hitashi_3U3"; break;
		default:
			driver = "Not Found"; break;																							
	}
	set_driver(driver);
	//Finding Driver

	/* 3. Step translate*/
	bool get_code = 0;
  	bool *get_code_pointer = &get_code;
	String additional_mark_space_cout,additional_mark_space_pos,additional_mark,additional_space;//value of parameter     
    //-------------------------------------------------------------- MSB or LSB8
    if (driver_read("code_format") == "LSB8") {
    Serial.println("MSB_code need to be transform in LSB8");
    String bit_code_temp = "";
    for (i = 0; i < driver_read("code_long").toInt()/8; i++){
      for (j = 0; j < 8; j++){
        bit_code_temp += bit_calc.substring(i*8, (i+1)*8).substring(7-j,7-j+1);
      }      
    }
    bit_calc = bit_code_temp;
	}
	// MSB or LSB8
    
    Serial.print("bit_code: ");// print, bit_code
    Serial.println(bit_calc);
    
    //-------------------------------------------------------------- bit translate
    ac_status = "";
    //--------------------------------------------- ON/OFF
    if (bit_calc.substring(driver_read("on/off_pos").toInt()-1, driver_read("on/off_pos").toInt()-1+driver_read("on/off_length").toInt()) == driver_read("ON")) {
		ac_status += "ON,";
	}
	else if (bit_calc.substring(driver_read("on/off_pos").toInt()-1,driver_read("on/off_pos").toInt()-1+driver_read("on/off_length").toInt()) == driver_read("OFF")) {
		ac_status += "OFF,";
	}
	else {
		ac_status += "?,";
	}
	// ON/OFF
	
	//--------------------------------------------- MODE
    if (bit_calc.substring(driver_read("mode_pos").toInt()-1, driver_read("mode_pos").toInt()-1+driver_read("mode_length").toInt()) == driver_read("AUTO")) {
		ac_status += "AUTO,";
	}
	else if (bit_calc.substring(driver_read("mode_pos").toInt()-1, driver_read("mode_pos").toInt()-1+driver_read("mode_length").toInt()) == driver_read("COOL")) {
		ac_status += "COOL,";
	}
	else if (bit_calc.substring(driver_read("mode_pos").toInt()-1, driver_read("mode_pos").toInt()-1+driver_read("mode_length").toInt()) == driver_read("DRY")) {
		ac_status += "DRY,";
	}
	else if (bit_calc.substring(driver_read("mode_pos").toInt()-1, driver_read("mode_pos").toInt()-1+driver_read("mode_length").toInt()) == driver_read("FAN")) {
		ac_status += "FAN,";
	}
    else if (bit_calc.substring(driver_read("mode_pos").toInt()-1, driver_read("mode_pos").toInt()-1+driver_read("mode_length").toInt()) == driver_read("AUTO2")) {
		ac_status += "AUTO,";
	}
	else if (bit_calc.substring(driver_read("mode_pos").toInt()-1, driver_read("mode_pos").toInt()-1+driver_read("mode_length").toInt()) == driver_read("COOL2")) {
		ac_status += "COOL,";
	}
	else if (bit_calc.substring(driver_read("mode_pos").toInt()-1, driver_read("mode_pos").toInt()-1+driver_read("mode_length").toInt()) == driver_read("DRY2")) {
		ac_status += "DRY,";
	}
	else if (bit_calc.substring(driver_read("mode_pos").toInt()-1, driver_read("mode_pos").toInt()-1+driver_read("mode_length").toInt()) == driver_read("FAN2")) {
		ac_status += "FAN,";
	}	
	else {
		ac_status += "?,";
	}	
	// MODE
	
	//--------------------------------------------- TEMP
	for (i = 15; i < 32 ;i++){
	    if (bit_calc.substring(driver_read("temp_pos").toInt()-1, driver_read("temp_pos").toInt()-1+driver_read("temp_length").toInt()) == driver_read("T"+String(i))) {
			ac_status += "T"+String(i)+",";
		}
		if (i == 32){
		    if (bit_calc.substring(driver_read("temp_pos").toInt()-1, driver_read("temp_pos").toInt()-1+driver_read("temp_length").toInt()) == driver_read("T++")) {
				ac_status += "++,";
			}
			else if (bit_calc.substring(driver_read("temp_pos").toInt()-1, driver_read("temp_pos").toInt()-1+driver_read("temp_length").toInt()) == driver_read("T+")) {
				ac_status += "+,";
			}
			else if (bit_calc.substring(driver_read("temp_pos").toInt()-1, driver_read("temp_pos").toInt()-1+driver_read("temp_length").toInt()) == driver_read("T*")) {
				ac_status += "--,";
			}			
			else if (bit_calc.substring(driver_read("temp_pos").toInt()-1, driver_read("temp_pos").toInt()-1+driver_read("temp_length").toInt()) == driver_read("T-")) {
				ac_status += "-,";
			}
			else if (bit_calc.substring(driver_read("temp_pos").toInt()-1, driver_read("temp_pos").toInt()-1+driver_read("temp_length").toInt()) == driver_read("T++")) {
				ac_status += "--,";
			}	
			else {
				ac_status += "?,";
			}			
		}
	}
	// TEMP
	
	//--------------------------------------------- FAN
    if (bit_calc.substring(driver_read("fan_pos").toInt()-1, driver_read("fan_pos").toInt()-1+driver_read("fan_length").toInt()) == driver_read("FA")) {
		ac_status += "FA,";
	}
	else if (bit_calc.substring(driver_read("fan_pos").toInt()-1, driver_read("fan_pos").toInt()-1+driver_read("fan_length").toInt()) == driver_read("F1")) {
		ac_status += "F1,";
	}
	else if (bit_calc.substring(driver_read("fan_pos").toInt()-1, driver_read("fan_pos").toInt()-1+driver_read("fan_length").toInt()) == driver_read("F2")) {
		ac_status += "F2,";
	}
	else if (bit_calc.substring(driver_read("fan_pos").toInt()-1, driver_read("fan_pos").toInt()-1+driver_read("fan_length").toInt()) == driver_read("F3")) {
		ac_status += "F3,";
	}	
	else {
		ac_status += "?,";
	}
	// FAN
	
	//--------------------------------------------- VANE
    if (bit_calc.substring(driver_read("vane_pos").toInt()-1, driver_read("vane_pos").toInt()-1+driver_read("vane_length").toInt()) == driver_read("VH")) {
		ac_status += "VH";
	}
	else if (bit_calc.substring(driver_read("vane_pos").toInt()-1, driver_read("vane_pos").toInt()-1+driver_read("vane_length").toInt()) == driver_read("V1")) {
		ac_status += "V1";
	}
	else if (bit_calc.substring(driver_read("vane_pos").toInt()-1, driver_read("vane_pos").toInt()-1+driver_read("vane_length").toInt()) == driver_read("V2")) {
		ac_status += "V2";
	}
	else if (bit_calc.substring(driver_read("vane_pos").toInt()-1, driver_read("vane_pos").toInt()-1+driver_read("vane_length").toInt()) == driver_read("V3")) {
		ac_status += "V3";
	}
	else if (bit_calc.substring(driver_read("vane_pos").toInt()-1, driver_read("vane_pos").toInt()-1+driver_read("vane_length").toInt()) == driver_read("V4")) {
		ac_status += "V4";
	}
	else if (bit_calc.substring(driver_read("vane_pos").toInt()-1, driver_read("vane_pos").toInt()-1+driver_read("vane_length").toInt()) == driver_read("V5")) {
		ac_status += "V5";
	}
	else if (bit_calc.substring(driver_read("vane_pos").toInt()-1, driver_read("vane_pos").toInt()-1+driver_read("vane_length").toInt()) == driver_read("VR")) {
		ac_status += "VR";
	}				
	else {
		ac_status += "?,";
	}
	// VANE
	
	ac_now();		
  }//get_code

//driver_read

//---------------------------------------------------------------------------- driver_read, Reading value from driver
String acsaver::driver_read(String var){
  int pos_of_parm = driver.indexOf(var + String("="));
  int pos_of_comma = driver.indexOf(",",pos_of_parm);
  String value = driver.substring(pos_of_parm + (var + String("=")).length(),pos_of_comma);  
  //Serial.print(var);
  //Serial.print(": ");
  //Serial.println(value);
  return value;    
}
//driver_read

//---------------------------------------------------------------------------- ac_now    
String acsaver::ac_now(){
    Serial.print("Driver > ");
    Serial.println(driver_name);
    Serial.print("AC-State > ");
    Serial.println(ac_status);
}
//ac_now






































































	

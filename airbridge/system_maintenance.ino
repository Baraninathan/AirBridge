
/*
 * ------ system_maintenance() ---------
 * 1. this function will be calling in main operating
 * 2. it looking for system error
 * 3. we check temp_sensor, dirty_air, power outage
 * 4. temp_sensor is a most important
 * 5. if battery low, then set yellow LED on and go state 1, and do not thing
 * 
 */

void clearing_eeprom_at_first_start(){
  uint16_t ptr = mem_clear_mrk_eeprom_pos; 
  if (!mem.read_eeprom_bool(&ptr)){
    mem.eeprom_clear(0,mem_clear_mrk_eeprom_pos);
    ptr = mem_clear_mrk_eeprom_pos;
    mem.write_eeprom_bool(&ptr, true);
    mem.eeprom_confirm();
  }
}

void restart_s_air_1time(){
  /*
   * ~~~~~~~~ restating s_air ~~~~~~~~~
   * restart once after new start, then set EEPROM at position 4094 to 1, then set to 0 after restart
   */
  uint16_t ptr = mem_restart_eeprom_pos;  
  if (!mem.read_eeprom_bool(&ptr)){
    ptr = mem_restart_eeprom_pos;
    mem.write_eeprom_bool(&ptr, true);
    mem.eeprom_confirm();
    if (show_data){
      Serial.println(F("NextAir needs to reset!"));
    }
    ESP.restart();
  }
  ptr = mem_restart_eeprom_pos;
  mem.write_eeprom_bool(&ptr, false);
  mem.eeprom_confirm();
}

void reset_or_not(){
  uint16_t ptr = mem_reset_vars_eeprom_pos;
  int i = 0; 
  if (mem.read_eeprom_bool(&ptr)){
    mem.eeprom_clear(mem_trreturn_pos, mem_reset_vars_eeprom_pos);
    mem.eeprom_confirm();
    if (show_data){
      Serial.println(F("restart NextAir!"));
    }
  }
  else{
    delay(2000);
    ptr = mem_reset_vars_eeprom_pos;
    mem.write_eeprom_bool(&ptr, true);
    mem.eeprom_confirm();
    led_yellow.hi();
    led_green.hi();
    if (show_data){
      Serial.println(F("time before returning memory, press reset now if you want to reset NextAir!"));
    }
    for (i = 3; i > 0; i--){
      if (show_data){
        Serial.println(i);
      }
      delay(1000);
    }
  }
  ptr = mem_reset_vars_eeprom_pos; 
  mem.write_eeprom_bool(&ptr, false);
  mem.eeprom_confirm();
}

/*
 * saving vars to eeprom for not lose vars-value
 */
void saving_vars_to_eeprom(){
  static float trreturn = 0.0;// marker for tr to return vars
  trreturn = 0.0;
  if (mem.tr[2] == 0.0){
    sensors.requestTemperatures();
    if (mem.tsensors_select){
      trreturn = get_temperature(1);//tr cabel
    }
    else{
      trreturn = get_temperature(0);//tr cabel
    }
  }
  else{
    trreturn = mem.tr[2];
  }
  static uint16_t ptr = mem.save_internal_vars(trreturn);
  ptr = mem.save_internal_vars(trreturn);
  if (ptr != 0){
    Serial.println("Recording Remote parameters to EPPROM"); // only for #testing
    mem.write_eeprom_int(&ptr, rmt.getNum());
    mem.write_eeprom_int(&ptr, rmt.getPower());
    mem.write_eeprom_int(&ptr, rmt.getMode());
    mem.write_eeprom_int(&ptr, rmt.getTrset());
    mem.write_eeprom_int(&ptr, rmt.getFan());
    mem.write_eeprom_int(&ptr, rmt.getVane());
    mem.eeprom_confirm();
  } 
}

/*
 * ~~~~~~~ return vars ~~~~~~
 * if strom outage s_air will restart automatically, 
 * so we nedd to retrun old vars, so that s_air is working again
 */
void returning_vars_from_eeprom(){
  sensors.requestTemperatures();
  bool return_success = 0;
  uint16_t ptr = mem.return_internal_vars(get_temperature(0), get_temperature(1), &return_success);
  rmt.setNum(mem.read_eeprom_int(&ptr));
  rmt.setPower(mem.read_eeprom_int(&ptr));
  rmt.setMode(mem.read_eeprom_int(&ptr));
  rmt.setTrset(mem.read_eeprom_int(&ptr));
  rmt.setFan(mem.read_eeprom_int(&ptr));
  rmt.setVane(mem.read_eeprom_int(&ptr));
  if (!return_success){
    //reset op vars to default
    mem.reset_internal_vars();
//    if (mem.user_need_email){
//      mem.state = 2;
//      if (show_data){
//        Serial.println(F("go to state 2"));
//      }
//    }
//    else{
      mem.state = 0;// only 0 ist working, because when user reset NextAir, it need to  go to state0
      if (show_data){
        Serial.println(F("go to state 0"));
      }
//    }
  }
}


/*
 * Check if compressor is off continously, it is possible that is turn off without RC signal, then move to standby state 1
 */
void check_power_off(){
  if ((mem.compressor_off_this_cycle >= power_off_detect_after_compressor_off_loop) && (mem.state_cnt >= power_off_detect_after_compressor_off_loop)){//check power
    Serial.println(F("power turn down"));
    //mem.state = 1;
    mem.compressor_off_this_cycle = 0; // reset compressor off
    MQTTtrigger_log(F("detect"),F("unexpected,power_off"));  // Sending info to server 
    rmt.setPower(0);
  }
}


void check_trset(){
  if (rmt.getTrset() == 0){//set trset if trset = 0
    rmt.setTrset(default_trset);
  }
}

   /*********************************
    * check_batt
    * 1. this function will check battery voltage and detect if s_air has been connected with usb-kabel, then s_air will start webserver instead sleep 20 second
    * ********************************
    */
//void check_batt(){
//  if ((system_adc_read() <= 16) || (system_adc_read() >= usb_voltage)){
//    mem.use_usb_cnt++;
//    if ((!mem.use_usb) && (mem.use_usb_cnt >= use_usb_off_set*one_mi)){
//      if (show_data){
//        Serial.print(F("use kabel -> start webserver!\r\n"));
//      }
//      mem.use_usb = 1;
//    }
//    if (mem.use_usb_cnt >= use_usb_off_set*one_mi){
//      mem.use_usb_cnt = use_usb_off_set*one_mi;
//    }
//    mem.low_batt_cnt = 0;
//  }
//  else if ((system_adc_read() > 16) && (system_adc_read() < 600)){
//    mem.low_batt_cnt++;
//    mem.use_usb_cnt--;
//    if (mem.low_batt_cnt >= low_batt_off_set*one_mi){
//      digitalWrite(12, HIGH);
//      if (show_data){
//        Serial.print(F("battery voltage is low\r\n"));
//      }
//      mem.low_batt_cnt = low_batt_off_set*one_mi;
//      mem.state = 1;
//      mem.use_usb = 0;
//    }    
//    if (mem.use_usb_cnt <= 0){
//      mem.use_usb_cnt = 0;
//    }
//  }
//  else{
//    mem.use_usb_cnt--;
//    if ((mem.use_usb) && (mem.use_usb_cnt <= 0)){
//      if (show_data){
//        Serial.print(F("no usb -> stop webserver!\r\n"));
//      }
//      mem.use_usb = 0;
//    }
//    if (mem.use_usb_cnt <= 0){
//      mem.use_usb_cnt = 0;
//    }
//    mem.low_batt_cnt = 0;
//  }
//
//  //start or close web_server hier
//  if (mem.use_usb && !mem.access_point_on){
//    start_web();
//  }
//  else if (!mem.use_usb && mem.access_point_on){
//    close_web();
//  }
//  else if (mem.use_usb && mem.access_point_on && (mem.loop_cnt_considering_cut%one_hr == 0)){
//    close_web();
//    start_web();
//  }
//}

void check_dirty(){
  /*
   * every time when start new cycle we need to check dirty air
   */
  static bool need_to_check_dirty = 0;
  if (mem.compressor_state[2] && !mem.compressor_state[1] && (mem.compressor_on_cnt >= 3*one_mi)){
    need_to_check_dirty = 1;      
  }

  if (need_to_check_dirty){
    if ((mem.lowest_tc >= (mem.bl_log.lowest_tc + dirty_max_tc_from_min_tc)) || ((mem.min_slope_tc[0]*dirty_slope_tc_potential) <= mem.min_slope_tc[2])){
      mem.dirty_cnt++;
      need_to_check_dirty = 0;
    }
    else if (!mem.compressor_state[2] && mem.compressor_state[1]){
      need_to_check_dirty = 0;      
      mem.dirty_cnt = 0;
    }
  }
}

void check_broke_temp(){//20181123
  //temperaturesensor is broken, if tr/tc > physical_sensor_boundary_tmax or < physical_sensor_boundary_tmin in config.h
  if ((((mem.tc[2] <= physical_sensor_boundary_tmin) || (mem.tc[2] >= physical_sensor_boundary_tmax) || (mem.tr[2] <= physical_sensor_boundary_tmin) || (mem.tr[2] >= physical_sensor_boundary_tmax)) && ((mem.tc[1] <= physical_sensor_boundary_tmin) || (mem.tc[1] >= physical_sensor_boundary_tmax) || (mem.tr[1] <= physical_sensor_boundary_tmin) || (mem.tr[1] >= physical_sensor_boundary_tmax))) && (mem.state != 9) && (rmt.getPower())){
    if ((mem.tc[2] > physical_sensor_boundary_tmin) && (mem.tc[2] < physical_sensor_boundary_tmax) && ((mem.tr[2] <= physical_sensor_boundary_tmin) || (mem.tr[2] >= physical_sensor_boundary_tmax))){ //tr is broken
      if (show_data){
        Serial.println(F("temp-sensor ist broken case 0 -> go state 9"));
      }
      mem.state_9_case = 0;
      rmt.setTrset(state_9_default_trset);
    }
    else if (((mem.tc[2] <= physical_sensor_boundary_tmin) || (mem.tc[2] >= physical_sensor_boundary_tmax)) && (mem.tr[2] > physical_sensor_boundary_tmin) && (mem.tr[2] < physical_sensor_boundary_tmax)){ //tc is broken
      if (show_data){
        Serial.println(F("temp-sensor ist broken case 1 -> go state 9"));
      }
      mem.state_9_case = 1;
    }
    else{ //tr, tc are broken
      if (show_data){
        Serial.println(F("temp-sensor ist broken case 2 -> go state 9"));
      }  
      mem.state_9_case = 2;
      rmt.setTrset(state_9_default_trset);
    }
    mem.state = 9;  mem.state_cnt = 0; 
    rmt.cool();
    mem.state_9_compressor_mrk = 1;
  }
  else if (((mem.tc[2] > physical_sensor_boundary_tmin) && (mem.tc[2] < physical_sensor_boundary_tmax) && (mem.tr[2] > physical_sensor_boundary_tmin) && (mem.tr[2] < physical_sensor_boundary_tmax)) && ((mem.tc[1] > physical_sensor_boundary_tmin) && (mem.tc[1] < physical_sensor_boundary_tmax) && (mem.tr[1] > physical_sensor_boundary_tmin) && (mem.tr[1] < physical_sensor_boundary_tmax)) && (mem.state == 9) && (rmt.getPower())){//check state 9, if temp-sensor ok, then go state 6
    rmt.cut();
    delay(1000);
    rmt.cut(); 
    mem.state = 5;  mem.state_cnt = 0;  
  }
}

void check_percent_to_cut(){//20181006
  int h = 0,i = 0,j = 0,k = 0,x = 0,y = 0,z = 0;
  /*
   * every time when c off we need to check a new cut percent
   */
  static bool need_to_check_percent_to_cut = 0;

  //
  if (!mem.compressor_state[2] && mem.compressor_state[1]){
    need_to_check_percent_to_cut = 1;
  }
  
  if (need_to_check_percent_to_cut){
    if ((mem.av_tr[2] > mem.av_tr[1]) && (mem.av_tr[1] <= mem.av_tr[0])){
      //--> it can faster smart cut, because it's too cold, Troom_min is too clod
      if (mem.av_tr[1] <= (float(rmt.getTrset()) + (diff_btw_tr_and_trset_to_start_cut/2.0))){
        for (i = 1; i < mem_percent_to_cut_table_size; i++){
          if (mem.percent_to_cut == mem_percent_to_cut_table[i]){
            mem.percent_to_cut = mem_percent_to_cut_table[i-1];
            if (show_data){
              Serial.print(F("faster smart cut!\r\n"));
            }
            break; 
          }
        }
      }
      else if (mem.av_tr[1] >= (float(rmt.getTrset()) + (diff_btw_tr_and_trset_to_start_cut/2.0))){
        for (i = 0; i < mem_percent_to_cut_table_size - 1; i++){
          if (mem.percent_to_cut == mem_percent_to_cut_table[i]){
            mem.percent_to_cut = mem_percent_to_cut_table[i+1];
            if (show_data){
              Serial.print(F("slower smart cut!\r\n"));
            }
            break; 
          }
        }
      }
      need_to_check_percent_to_cut = 0;
    }
    else if (mem.compressor_state[2] && !mem.compressor_state[1]){
      need_to_check_percent_to_cut = 0;
    }
  }
}

#include "memory_manager.h"

MemoryManager::MemoryManager(){
  int i = 0, j = 0;
  EEPROM.begin(4096);// EEPROM begin to read and write for 4096 caractors
  //init all memory
  
  //************* normal operating-part *******************
  for (i = 0; i < 3; i++){
    tc[i] = 0.0;
    tr[i] = 0.0;
    av_tc[i] = 0.0;      
    av_tr[i] = 0.0;     
    compressor_state[i] = 0;
  }
  tcstamp = 0.0;         
  for (i = 0; i < 6; i++){
    slope_tc[i] = 0.0;
    slope_tr[i] = 0.0;        
  }
  tsensors_select = 0;
  state_cnt = 0;
  loop_cnt_considering_cut = 0;
  //av_t_cnt = 0;
  slope_cnt = 0;
  total_cycle = 0;
  min_slope_tc[1] = 0.0;
  min_slope_tc[2] = 0.0;
  //trstart = 0.0;
  compressor_on_cnt = 0;
  compressor_off_cnt = 0;
  min_tc_to_detect_compressor_on = default_min_tc_to_detect_compressor_on;
  lowest_tc = default_val_intial_high;
  lowest_tr = default_val_intial_high;
  percent_to_cut = mem_percent_to_cut_table[15];
  heavy_cnt = 0;
  dirty_cnt = 0;
  
  for (i = 0; i < 3; i++){
    min_slope_tc[i] = 0;
  }
  for (i = 0; i < 2; i++){
    min_slope_tr[i] = 0;
    //min_tc[i] = 0;
    //min_tr[i] = 0;
  }  
  state_9_compressor_mrk = 0;
  state_9_case = 0;
  low_batt_cnt = 0;
  //use_usb = 1;
  access_point_on = 0;
  total_heavyduty_cut = 0;
  compressor_on_this_cycle = 0;
  compressor_off_this_cycle = 0;
  avgmax_tc = 0;
  min_tc_slope_considering_compressor_on = default_min_tc_slope_considering_compressor_on;
  min_tr_this_cycle = default_val_intial_high;
  max_tr_this_cycle = default_val_intial_high;
  //use_usb_cnt = 0;

  // Public warning
  trigging_warning_sys_ssid = false;
  trigging_warning_sys_mqtt = false;
  count_warning_sys_mqtt = 0;
  trigging_warning_op_heavy = false;
  trigging_warning_op_dirty = false;

  //************** esaving log-part ***************
  last_tstamp = 0;  
  sent_baseline_already = 0; // old script user_need_email = 0; 
  compressor_log = 0; 
  coff_log = 0; 
  trstart = 0;
  best_tc_slope = default_val_intial_high;
  //esaving_log_full_stack = 0;
  for (i = 0; i < mem_mo_log_lite_scale_cnt; i++){
     esaving_log_lite_stack[i] = 0;
  }

  //init bl_log
  bl_log.finish = 0;
  //bl_log.compressor_off_cnt = 0, bl_log.total_cycle = 0, bl_log.hr = 0;
  bl_log.compressor_off_cnt = 0, bl_log.total_cycle = 0;
  for (i = 0; i < mem_bl_length; i++){
    bl_log.trset[i] = 0;
    bl_log.duty[i] = 0;
  }
  bl_log.compressor_on_cnt = 0;
  bl_log.trstart = 0, bl_log.av_tr = 0,  bl_log.lowest_tc = default_val_intial_high, bl_log.best_tc_slope = default_val_intial_high;

  //init raw data  
//  for (i = 0; i < mem_raw_data_length; i++){
//    raw_data_log[i].tr = 0;
//    raw_data_log[i].tc = 0;
//    raw_data_log[i].trset = 0;
//    raw_data_log[i].slope_tc = 0;
//    raw_data_log[i].compressor_state = 0;
//    raw_data_log[i].algo = 0;
//  }
  raw_data_log_cnt = 0;
  //init hr_log_full
//  for (i = 0; i < mem_hr_log_full_length; i++){
//    hr_log_full[i].mo = 0, hr_log_full[i].dy = 0, hr_log_full[i].hr = 0, hr_log_full[i].trset = 0;
//    hr_log_full[i].yr = 0;
//    hr_log_full[i].op_duty = 0, hr_log_full[i].bl_duty = 0, hr_log_full[i].esaving = 0;
//  };
  //init dy_log_full  
//  for (i = 0; i < mem_dy_log_full_length; i++){
//    dy_log_full[i].mo = 0, dy_log_full[i].dy = 0;
//    dy_log_full[i].yr = 0;
//    dy_log_full[i].op_duty = 0, dy_log_full[i].bl_duty = 0, dy_log_full[i].esaving = 0;
//  };
  //init mo_log_full  
  for (i = 0; i < mem_mo_log_full_length; i++){
    mo_log_full[i].mo = 0;
    mo_log_full[i].yr = 0;
    mo_log_full[i].op_duty = 0, mo_log_full[i].bl_duty = 0, mo_log_full[i].esaving = 0;
  };
  //init hr_log_lite
  for (i = 0; i < mem_hr_log_lite_length; i++){
    hr_log_lite_esaving[i] = 0;
  }
  //init mem_mo_log_lite
  for (i = 0; i < mem_mo_log_lite_scale_cnt; i++){
    for (j = 0; j < mem_mo_log_lite_length; j++){
      mo_log_lite[i].esaving[j] = 0;
    }
  };
};



void MemoryManager::eeprom_clear(uint16_t start_pos, uint16_t end_pos){
  /*
 * eeprom_read/write
 * 1. read or write String arg at start_position until end_position
 * 2. arg1 = uint8_t, arg1 = uint16_t, arg1 = float,
 * example eeprom_write(100, 101, -111, 88,88);
 * or eeprom_write(100,101, 20, -111)
 */
  int i = 0;
  for (i = start_pos; i < end_pos; i++){
    EEPROM.write(i, 0);
  }
}

void MemoryManager::eeprom_confirm(){
  EEPROM.commit();
}

String MemoryManager::read_eeprom_string(uint16_t start_pos, uint16_t end_pos){
  String a_string = "";
  int i = 0;
  for (i = start_pos; i < end_pos; i++){
    a_string += char(EEPROM.read(i));
  }
  a_string = String(a_string.c_str()); // convert the original array of char with fix length 32, to string with exact String, then give as output
  return a_string;
}

void MemoryManager::write_eeprom_string(uint16_t start_pos, uint16_t end_pos, String string1){
  int i = 0;
  for (i = start_pos; i < start_pos + string1.length(); i++){
    EEPROM.write(i, string1.c_str()[i - start_pos]);
  }
}
 
void MemoryManager::write_eeprom_bool(uint16_t *start_pos, bool bool1){
  String str_tmp = String(bool1);
  EEPROM.write((*start_pos), str_tmp.c_str()[0]);
  *start_pos = *start_pos + 1;
}
 
void MemoryManager::write_eeprom_int(uint16_t *start_pos, int int1){
  String str_tmp = String(int1);
  int ptr = 0,i = 0;
  for (i = *start_pos; i < *start_pos + mem_eeprom_int_length; i++){
    EEPROM.write(i, str_tmp.c_str()[ptr]);
    ptr++;
  }
  *start_pos = *start_pos + mem_eeprom_int_length;
}

void MemoryManager::write_eeprom_uint32(uint16_t *start_pos, uint32_t uint32_1){
  String str_tmp = String(uint32_1);
  int ptr = 0,i = 0;
  for (i = *start_pos; i < *start_pos + mem_eeprom_uint32_length; i++){
    EEPROM.write(i, str_tmp.c_str()[ptr]);
    ptr++;
  }
  *start_pos = *start_pos + mem_eeprom_uint32_length;
}

void MemoryManager::write_eeprom_float(uint16_t *start_pos, float float1){
  String str_tmp = String(float1);
  int ptr = 0,i = 0;
  for (i = *start_pos; i < *start_pos + mem_eeprom_float_length; i++){
    EEPROM.write(i, str_tmp.c_str()[ptr]);
    ptr++;
  }
  *start_pos = *start_pos + mem_eeprom_float_length;
}

bool MemoryManager::read_eeprom_bool(uint16_t *start_pos){
  String str_tmp = "";
  str_tmp += char(EEPROM.read(*start_pos));
  *start_pos = *start_pos + 1;
  return bool(str_tmp.toInt());    
}

int MemoryManager::read_eeprom_int(uint16_t *start_pos){
  String str_tmp = "";
  int i = 0;
  for (i = *start_pos; i < *start_pos + mem_eeprom_int_length; i++){
    str_tmp += char(EEPROM.read(i));
  }
  *start_pos = *start_pos + mem_eeprom_int_length;
  return str_tmp.toInt();    
}

uint32_t MemoryManager::read_eeprom_uint32(uint16_t *start_pos){
  String str_tmp = "";
  int i = 0;
  for (i = *start_pos; i < *start_pos + mem_eeprom_uint32_length; i++){
    str_tmp += char(EEPROM.read(i));
  }
  *start_pos = *start_pos + mem_eeprom_uint32_length;
  return str_tmp.toInt();    
}

float MemoryManager::read_eeprom_float(uint16_t *start_pos){
  String str_tmp = "";
  int i = 0;
  for (i = *start_pos; i < *start_pos + mem_eeprom_float_length; i++){
    str_tmp += char(EEPROM.read(i));
  }
  *start_pos = *start_pos + mem_eeprom_float_length;
  return str_tmp.toFloat();
}

int MemoryManager::save_internal_vars(float trreturn){
  //int h = 0,i = 0,j = 0,k = 0,x = 0,y = 0,z = 0;
  int i = 0,j = 0;
  eeprom_clear(mem_trreturn_pos, mem_sensor_select_eeprom_pos); // Write 0 to eeprom
  eeprom_confirm(); // eeprom commit
  uint16_t ptr = mem_trreturn_pos;
  //trreturn
  write_eeprom_float(&ptr, trreturn);
  //esaving log-part
  //etc
  write_eeprom_uint32(&ptr, double(last_tstamp));
  write_eeprom_bool(&ptr, sent_baseline_already);
  write_eeprom_int(&ptr, compressor_log);
  write_eeprom_int(&ptr, coff_log);
  write_eeprom_float(&ptr,trstart);
  write_eeprom_float(&ptr,best_tc_slope);
  //base line
  write_eeprom_bool(&ptr, bl_log.finish);
  write_eeprom_int(&ptr, bl_log.compressor_on_cnt);
  write_eeprom_int(&ptr, bl_log.compressor_off_cnt);
  write_eeprom_int(&ptr, bl_log.total_cycle);
  //write_eeprom_int(&ptr, bl_log.hr);
  for (i = 0; i < mem_bl_length; i++){
    write_eeprom_int(&ptr, bl_log.trset[i]);
    write_eeprom_float(&ptr, bl_log.duty[i]);
  }
  write_eeprom_float(&ptr, bl_log.trstart);
  write_eeprom_float(&ptr, bl_log.av_tr);
  write_eeprom_float(&ptr, bl_log.lowest_tc);
  write_eeprom_float(&ptr, bl_log.best_tc_slope);
  
  //Serial.println(ptr);    
  //full data logger, this mode will storage date, saving, ... if email has been set
//  if (user_need_email){
//    write_eeprom_int(&ptr, esaving_log_full_stack);
//    for (i = 0; i < mem_hr_log_full_length; i++){
//      write_eeprom_int(&ptr, hr_log_full[i].yr);
//      write_eeprom_int(&ptr, hr_log_full[i].mo);
//      write_eeprom_int(&ptr, hr_log_full[i].dy);
//      write_eeprom_int(&ptr, hr_log_full[i].hr);
//      write_eeprom_int(&ptr, hr_log_full[i].trset);
//      write_eeprom_float(&ptr, hr_log_full[i].op_duty);
//      write_eeprom_float(&ptr, hr_log_full[i].bl_duty);
//      write_eeprom_float(&ptr, hr_log_full[i].esaving);
//    }
//    for (i = 0; i < mem_dy_log_full_length; i++){
//      write_eeprom_int(&ptr, dy_log_full[i].yr);
//      write_eeprom_int(&ptr, dy_log_full[i].mo);
//      write_eeprom_int(&ptr, dy_log_full[i].dy);
//      write_eeprom_float(&ptr, dy_log_full[i].op_duty);
//      write_eeprom_float(&ptr, dy_log_full[i].bl_duty);
//      write_eeprom_float(&ptr, dy_log_full[i].esaving);
//    }
//    for (i = 0; i < mem_mo_log_full_length; i++){
//      write_eeprom_int(&ptr, mo_log_full[i].yr);
//      write_eeprom_int(&ptr, mo_log_full[i].mo);
//      write_eeprom_float(&ptr, mo_log_full[i].op_duty);
//      write_eeprom_float(&ptr, mo_log_full[i].bl_duty);
//      write_eeprom_float(&ptr, mo_log_full[i].esaving);
//    }
//  }
  //lite data logger, this mode will storage a saving only for about lite_saving_length
//  else{
    for (i = 0; i < mem_mo_log_lite_scale_cnt; i++){
      write_eeprom_int(&ptr, esaving_log_lite_stack[i]);
    }
    for (i = 0; i < mem_hr_log_lite_length; i++){
      write_eeprom_int(&ptr, hr_log_lite_esaving[i]);
    }
    for (i = 0; i < mem_mo_log_lite_scale_cnt; i++){
      for (j = 0; j < mem_mo_log_lite_length; j++){
      write_eeprom_int(&ptr, mo_log_lite[i].esaving[j]);
      }
    }
//  }
  //Serial.println(ptr);  
  //2.2.3 normal operating-part
  write_eeprom_bool(&ptr, compressor_state[0]);
  write_eeprom_bool(&ptr, compressor_state[1]);
  write_eeprom_bool(&ptr, compressor_state[2]);
  uint16_t sensor_ptr = mem_sensor_select_eeprom_pos;
  write_eeprom_bool(&sensor_ptr, tsensors_select);
  write_eeprom_int(&ptr, loop_cnt_considering_cut);
  //write_eeprom_int(&ptr, av_t_cnt);
  write_eeprom_int(&ptr, slope_cnt);
  write_eeprom_int(&ptr, compressor_on_cnt);
  write_eeprom_int(&ptr, compressor_off_cnt);
  for (i = 0; i < 6; i++){
    write_eeprom_float(&ptr, slope_tc[i]);
    write_eeprom_float(&ptr, slope_tr[i]);
  }
  for (i = 0; i < 3; i++){
    write_eeprom_float(&ptr, tc[i]);
    write_eeprom_float(&ptr, tr[i]);
    write_eeprom_float(&ptr, av_tc[i]);
    write_eeprom_float(&ptr, av_tr[i]);
    write_eeprom_float(&ptr, min_slope_tc[i]);
  }
  write_eeprom_float(&ptr, tcstamp);
  write_eeprom_float(&ptr, min_slope_tr[0]);
  write_eeprom_float(&ptr, min_slope_tr[1]);
  //write_eeprom_float(&ptr, min_tc[0]);
  //write_eeprom_float(&ptr, min_tc[1]);
  //write_eeprom_float(&ptr, min_tr[0]);
  //write_eeprom_float(&ptr, min_tr[1]);
  write_eeprom_float(&ptr, lowest_tc);
  write_eeprom_float(&ptr, lowest_tr);
  write_eeprom_float(&ptr, min_tc_to_detect_compressor_on);
  write_eeprom_int(&ptr, state);
  write_eeprom_int(&ptr, state_cnt);
  write_eeprom_int(&ptr, total_cycle);
  write_eeprom_float(&ptr, percent_to_cut);
  //write_eeprom_bool(&ptr, use_usb);
  write_eeprom_bool(&ptr, state_9_compressor_mrk);
  write_eeprom_int(&ptr, dirty_cnt);
  write_eeprom_int(&ptr, state_9_case);
  write_eeprom_int(&ptr, low_batt_cnt);
  //write_eeprom_int(&ptr, use_usb_cnt);
  write_eeprom_int(&ptr, heavy_cnt);
  if (mem_show_data){
    Serial.print(("last saving eeprom position = ")); Serial.println(ptr);
  }
  return ptr;//if saving is success, then return ptr
}

void MemoryManager::reset_internal_vars(){//change date 20181025
/*
 * reset_variable
 * 1. reset all variable for normal operating
 * 2. reset energy-saving variable only when user need report
 */
  //int h = 0,i = 0,j = 0,k = 0,x = 0,y = 0,z = 0;
  int i = 0;
  if (mem_show_data){
    Serial.println(("rst_vars "));
  }
  next_op_tstamp = 0;
  for (i = 0; i < 3; i++){
    tc[i] = 0.0;
    tr[i] = 0.0;
    av_tc[i] = 0.0;      
    av_tr[i] = 0.0;     
    compressor_state[i] = 0;
  }
  tcstamp = 0.0;         
  for (i = 0; i < 6; i++){
    slope_tc[i] = 0.0;
    slope_tr[i] = 0.0;
  }
  state_cnt = 0;
  loop_cnt_considering_cut = 0;
  //av_t_cnt = 0;
  slope_cnt = 0;
  total_cycle = 0;
  min_slope_tc[1] = 0.0;
  min_slope_tc[2] = 0.0;
  trstart = 0.0;
  best_tc_slope = default_val_intial_high;
  compressor_on_cnt = 0;
  compressor_off_cnt = 0;
  lowest_tc = default_val_intial_high;
  lowest_tr = default_val_intial_high;
  min_tc_to_detect_compressor_on = default_min_tc_to_detect_compressor_on;
  percent_to_cut = mem_percent_to_cut_table[15];
  dirty_cnt = 0;
  heavy_cnt = 0;
  state_9_compressor_mrk = 0;
  state_9_case = 0;

  //reset Energy Saving variable
  compressor_log = 0;
  coff_log = 0;
  if (!bl_log.finish){
    bl_log.finish = 0;
    bl_log.compressor_on_cnt = 0;
    bl_log.compressor_off_cnt = 0;
    bl_log.total_cycle = 0;
    //bl_log.hr = 0;
    bl_log.trstart = 0.0;
    bl_log.av_tr = 0.0;
    bl_log.lowest_tc = default_val_intial_high;
    bl_log.best_tc_slope = default_val_intial_high;
    for (i = 0; i < mem_bl_length; i++){
      bl_log.trset[i] = 0;
      bl_log.duty[i] = 0.0;
    }
  }
//  for (i = 0; i < mem_raw_data_length; i++){
//    raw_data_log[i].tr = 0;
//    raw_data_log[i].tc = 0;
//    raw_data_log[i].trset = 0;
//    raw_data_log[i].slope_tc = 0;
//    raw_data_log[i].compressor_state = 0;
//    raw_data_log[i].algo = 0;
//  }
  raw_data_log_cnt = 0;
}

int MemoryManager::return_internal_vars(float trnow0, float trnow1, bool *return_success){
  //return_vars, then if diff(trnow-trreturn) < mem_return_vars_diff_troom, then go to last state, else go state 2
  uint16_t ptr = mem_trreturn_pos;
  //int h = 0,i = 0,j = 0,k = 0,x = 0,y = 0,z = 0;
  int i = 0,j = 0;
  float trreturn = read_eeprom_float(&ptr);
  //esaving log-part
  //etc
  last_tstamp = time_t(read_eeprom_uint32(&ptr));
  sent_baseline_already = read_eeprom_bool(&ptr);
  compressor_log = read_eeprom_int(&ptr);
  coff_log = read_eeprom_int(&ptr);
  trstart = read_eeprom_float(&ptr);
  best_tc_slope = read_eeprom_float(&ptr);
  //base line
  bl_log.finish = read_eeprom_bool(&ptr);
  bl_log.compressor_on_cnt = read_eeprom_int(&ptr);
  bl_log.compressor_off_cnt = read_eeprom_int(&ptr);
  bl_log.total_cycle = read_eeprom_int(&ptr);
  //bl_log.hr = read_eeprom_int(&ptr);
  for (i = 0; i < mem_bl_length; i++){
    bl_log.trset[i] = read_eeprom_int(&ptr);
    bl_log.duty[i] = read_eeprom_float(&ptr);
  }
  bl_log.trstart = read_eeprom_float(&ptr);
  bl_log.av_tr = read_eeprom_float(&ptr);
  bl_log.lowest_tc = read_eeprom_float(&ptr);
  bl_log.best_tc_slope = read_eeprom_float(&ptr);
  //Serial.println(ptr);    
  //full data loggerthis mode will storage date, saving, ... if email has been set
//  if (user_need_email){
//    esaving_log_full_stack = read_eeprom_int(&ptr);
//    for (i = 0; i < mem_hr_log_full_length; i++){
//      hr_log_full[i].yr = read_eeprom_int(&ptr);
//      hr_log_full[i].mo = read_eeprom_int(&ptr);
//      hr_log_full[i].dy = read_eeprom_int(&ptr);
//      hr_log_full[i].hr = read_eeprom_int(&ptr);
//      hr_log_full[i].trset = read_eeprom_int(&ptr);
//      hr_log_full[i].op_duty = read_eeprom_float(&ptr);
//      hr_log_full[i].bl_duty = read_eeprom_float(&ptr);
//      hr_log_full[i].esaving = read_eeprom_float(&ptr);
//    }
//    for (i = 0; i < mem_dy_log_full_length; i++){
//      dy_log_full[i].yr = read_eeprom_int(&ptr);
//      dy_log_full[i].mo = read_eeprom_int(&ptr);
//      dy_log_full[i].dy = read_eeprom_int(&ptr);
//      dy_log_full[i].op_duty = read_eeprom_float(&ptr);
//      dy_log_full[i].bl_duty = read_eeprom_float(&ptr);
//      dy_log_full[i].esaving = read_eeprom_float(&ptr);
//    }
//    for (i = 0; i < mem_mo_log_full_length; i++){
//      mo_log_full[i].yr = read_eeprom_int(&ptr);
//      mo_log_full[i].mo = read_eeprom_int(&ptr);
//      mo_log_full[i].op_duty = read_eeprom_float(&ptr);
//      mo_log_full[i].bl_duty = read_eeprom_float(&ptr);
//      mo_log_full[i].esaving = read_eeprom_float(&ptr);
//    }
//  }
  //lite data logger, this mode will storage a saving only for about lite_saving_length
//  else{
    for (i = 0; i < mem_mo_log_lite_scale_cnt; i++){
      esaving_log_lite_stack[i] = read_eeprom_int(&ptr);
    }
    for (i = 0; i < mem_hr_log_lite_length; i++){
      hr_log_lite_esaving[i] = read_eeprom_int(&ptr);
    }
    for (i = 0; i < mem_mo_log_lite_scale_cnt; i++){
      for (j = 0; j < mem_mo_log_lite_length; j++){
        mo_log_lite[i].esaving[j] = read_eeprom_int(&ptr);
      }
    }
//  }
  //Serial.println(ptr);  
  //2.2.3 normal operating-part
  compressor_state[0] = read_eeprom_bool(&ptr);
  compressor_state[1] = read_eeprom_bool(&ptr);
  compressor_state[2] = read_eeprom_bool(&ptr);
  uint16_t sensor_ptr = mem_sensor_select_eeprom_pos;
  tsensors_select = read_eeprom_bool(&sensor_ptr);
  loop_cnt_considering_cut = read_eeprom_int(&ptr);
  //av_t_cnt = read_eeprom_int(&ptr);
  slope_cnt = read_eeprom_int(&ptr);
  compressor_on_cnt = read_eeprom_int(&ptr);
  compressor_off_cnt = read_eeprom_int(&ptr);
  for (i = 0; i < 6; i++){
    slope_tc[i] = read_eeprom_float(&ptr);
    slope_tr[i] = read_eeprom_float(&ptr);
  }
  for (i = 0; i < 3; i++){
    tc[i] = read_eeprom_float(&ptr);
    tr[i] = read_eeprom_float(&ptr);
    av_tc[i] = read_eeprom_float(&ptr);
    av_tr[i] = read_eeprom_float(&ptr);
    min_slope_tc[i] = read_eeprom_float(&ptr);
  }
  tcstamp = read_eeprom_float(&ptr);
  min_slope_tr[0] = read_eeprom_float(&ptr);
  min_slope_tr[1] = read_eeprom_float(&ptr);
  //min_tc[0] = read_eeprom_float(&ptr); if( min_tc[0] == 0){min_tc[0] = default_val_intial_high;} // if reset, thenn set to 99
  //min_tc[1] = read_eeprom_float(&ptr); if( min_tc[1] == 0){min_tc[1] = default_val_intial_high;} // if reset, thenn set to 99
  //min_tr[0] = read_eeprom_float(&ptr); if( min_tr[0] == 0){min_tr[0] = default_val_intial_high;} // if reset, thenn set to 99
  //min_tr[1] = read_eeprom_float(&ptr); if( min_tr[1] == 0){min_tr[1] = default_val_intial_high;} // if reset, thenn set to 99
  lowest_tc = read_eeprom_float(&ptr);   if(lowest_tc == 0){lowest_tc = default_val_intial_high;} // if reset, thenn set to 99
  lowest_tr = read_eeprom_float(&ptr);   if(lowest_tr == 0){lowest_tr = default_val_intial_high;} // if reset, thenn set to 99
  min_tc_to_detect_compressor_on = read_eeprom_float(&ptr);
  if (min_tc_to_detect_compressor_on == 0.0){
    min_tc_to_detect_compressor_on = default_min_tc_to_detect_compressor_on;
  }
  state = read_eeprom_int(&ptr);
  state_cnt = read_eeprom_int(&ptr);
  total_cycle = read_eeprom_int(&ptr);
  percent_to_cut = read_eeprom_float(&ptr);
  if (percent_to_cut == 0.0){
    percent_to_cut = mem_percent_to_cut_table[15];
  }
  //use_usb = read_eeprom_bool(&ptr);
  state_9_compressor_mrk = read_eeprom_bool(&ptr);
  dirty_cnt = read_eeprom_int(&ptr);
  state_9_case = read_eeprom_int(&ptr);
  low_batt_cnt = read_eeprom_int(&ptr);
  //use_usb_cnt = read_eeprom_int(&ptr);
  heavy_cnt = read_eeprom_int(&ptr);
  if (mem_show_data){
    Serial.print(F("last restore eeprom position = ")); Serial.println(ptr);
  }
  if (((abs(trnow0 - trreturn) <= mem_return_vars_diff_troom) && (!tsensors_select)) || ((abs(trnow1 - trreturn) <= mem_return_vars_diff_troom) && (tsensors_select))){
    *return_success = 1;
    if (mem_show_data){
      Serial.println(F("returning vars is successfull!"));
    }  
  }
  else{
    *return_success = 0;
    if (mem_show_data){
      Serial.println(F("returning vars is failure!"));
    }
  }
  return ptr;//if returning is success, then return ptr
}

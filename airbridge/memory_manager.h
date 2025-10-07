#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H
#include <EEPROM.h>
#include <time.h>
#include <Arduino.h>
#include "config.h"

//do show data of all method in SAIRmemory
const bool mem_show_data = 0;

//esaving log part
const uint8_t mem_email_per_day = 1;//20181017, {1,2,3,4,6,12}
const uint8_t mem_normal_air_usage_per_day = 8;//this will use to calculate length of hr_log_lite = 28*6*normal_air_usage_per_day
const uint8_t mem_bl_length = 3; //duration of base line in hour
const uint16_t mem_raw_data_length = 1440/mem_email_per_day; // 60*24 stack = 24 hour = 1440 //20181017
//const uint8_t mem_hr_log_full_length = 24; // how long we save data in hour-format = 24 //20181017
//const uint8_t mem_dy_log_full_length = 32; // how long we save data in day-format
const uint8_t mem_mo_log_full_length = 6; // how long we save data in mon-format
const uint16_t mem_reset_vars_eeprom_pos = 4092;
const uint16_t mem_sensor_select_eeprom_pos = 4093;
const uint16_t mem_restart_eeprom_pos = 4094;
const uint16_t mem_clear_mrk_eeprom_pos = 4095;
//const uint8_t mem_percent_to_cut_table_size = 21;
//const float mem_percent_to_cut_table[mem_percent_to_cut_table_size] = {0.99, 0.95, 0.90, 0.85, 0.80, 0.75, 0.70, 0.65, 0.60, 0.55, 0.50, 0.45, 0.40, 0.35, 0.30, 0.25, 0.20, 0.15, 0.10, 0.05, 0.01};
const uint16_t mem_hr_log_lite_length = 28*mem_normal_air_usage_per_day; // how long data will storage for webserver in hour base on 8 hours per day for 28 day
const uint8_t mem_mo_log_lite_scale_cnt = 5; // 100% 80% --> 20%
const uint8_t mem_mo_log_lite_scale_value[5] = {100,80,60,40,20}; // 100% 80% --> 20%
const uint8_t mem_mo_log_lite_length = 6; // how long we save data in mon-format

//tr_diff for return vars
const float mem_return_vars_diff_troom = 0.5;// if s_air restart we reteurn old vars if diff. troom <= 0.5 °C
//mem length of vars in EEPROM
const uint8_t mem_ssid_max_length = 32;
const uint8_t mem_pass_max_lenght = 64;
const uint8_t mem_email_max_length = 254;
//const uint8_t mem_time_zone_max_lenght = 4;
const uint8_t mem_eeprom_int_length = 5;
const uint8_t mem_eeprom_uint32_length = 10;
const uint8_t mem_eeprom_float_length = 5;

//position of vars in EEPROM
//network-info
const uint16_t mem_ssid_log_pos[2] = {0, mem_ssid_max_length};
const uint16_t mem_pass_log_pos[2] = {mem_ssid_max_length, 32 + 64};
const uint16_t mem_email_log_pos[2] = {32 + 64, 32 + 64 + 254};
//const uint16_t mem_time_zone_log_pos[2] = {32 + 64 + 254, 32 + 64 + 254 + 5}; 
//trreturn
const uint16_t mem_trreturn_pos = 32 + 64 + 254 + 5;
//esaving_log&operating
const uint16_t mem_esaving_operating_pos = 32 + 64 + 254 + 5 + mem_eeprom_float_length;

class MemoryManager{
  private:
    struct bl_t{
      bool finish;
      //uint8_t hr, trset[mem_bl_length];
      uint8_t trset[mem_bl_length];
      uint16_t compressor_off_cnt, compressor_on_cnt, total_cycle;
      float trstart, duty[mem_bl_length], av_tr, lowest_tc, best_tc_slope;
    };
    
    struct raw_data_t{
      uint8_t tr, tc, trset, algo;
      int8_t slope_tc;
      bool compressor_state;
    };
    
    struct hr_log_full_t{
      uint8_t mo, dy, hr, trset;
      uint16_t yr;
      float op_duty, bl_duty, esaving;
    };
    
//    struct dy_log_full_t{
//      uint8_t mo, dy;
//      uint16_t yr;
//      float op_duty, bl_duty, esaving;
//    };
    
    struct mo_log_full_t{
      uint8_t mo;
      uint16_t yr;
      float op_duty, bl_duty, esaving;
    };

    struct mo_log_lite_t{
      uint8_t esaving[mem_mo_log_lite_length];
    };
    
  public:
    //1. vars, that no need to be saved
    bool need_to_check_ir, access_point_on, cut_now;
    uint32_t next_op_tstamp, last_cut_t;
    bool trigging_warning_sys_ssid, trigging_warning_sys_mqtt;
    byte count_warning_sys_mqtt;
    bool trigging_warning_op_heavy, trigging_warning_op_dirty;
    byte total_heavyduty_cut;
    uint16_t compressor_on_this_cycle, compressor_off_this_cycle;
    float avgmax_tc;
    float min_tc_slope_considering_compressor_on;
    float min_tr_this_cycle, max_tr_this_cycle;
    //byte av_t_cnt;

    //2. vars, that need to be saved
    //2.1 network-info, no vars it'll be only store in EEPROM and don't be save with save internal vars
    //2.2.1 Trreturn_stamp, no var it'll be only store in EEPROM, be save with save internal vars
    //2.2.2 esaving log-part, be save with save internal vars
      time_t last_tstamp;
      bool sent_baseline_already; // wheter the baseline parameters have been sent, old script was bool user_need_email;
      uint8_t compressor_log, coff_log;
      float trstart;
      float best_tc_slope;
      //base line
      bl_t bl_log;
      
      //full data logger, this mode will storage date, saving, ... if email has been set
      uint16_t raw_data_log_cnt;
      //uint8_t esaving_log_full_stack;
      //raw_data_t raw_data_log[mem_raw_data_length];
      //hr_log_full_t hr_log_full[mem_hr_log_full_length];
      //dy_log_full_t dy_log_full[mem_dy_log_full_length];
      mo_log_full_t mo_log_full[mem_mo_log_full_length];
  
      //lite data logger, this mode will storage a saving only for about lite_saving_length
      uint8_t esaving_log_lite_stack[mem_mo_log_lite_scale_cnt];
      int8_t hr_log_lite_esaving[mem_hr_log_lite_length];// web_saving_lite in hour 
      mo_log_lite_t mo_log_lite[mem_mo_log_lite_scale_cnt];
    
    //2.2.3 normal operating-part
      bool compressor_state[3], tsensors_select;
      uint16_t loop_cnt_considering_cut, slope_cnt, compressor_on_cnt, compressor_off_cnt;
      float slope_tc[6], slope_tr[6];
      float tc[3], tr[3], av_tc[3], av_tr[3], tcstamp;
      float min_slope_tc[3], min_slope_tr[2];
      //float min_tc[2], min_tr[2], lowest_tc, lowest_tr, min_tc_to_detect_compressor_on;
      float lowest_tc, lowest_tr, min_tc_to_detect_compressor_on;
      uint8_t state = 0;
      uint16_t state_cnt, total_cycle; // number of loop in a certain state continously, number of compressor On cycle and Off cycle (starting 1 with Compressor On, if finishe with odd like 2,4,6,8 then the last state is turning from On to Off)
      float percent_to_cut = mem_percent_to_cut_table[15];
      //bool use_usb = 1, state_9_compressor_mrk;
      bool state_9_compressor_mrk;
      //uint8_t dirty_cnt, state_9_case, low_batt_cnt, use_usb_cnt, heavy_cnt;
      uint8_t dirty_cnt, state_9_case, low_batt_cnt, heavy_cnt;

    MemoryManager();

    void eeprom_clear(uint16_t start_pos, uint16_t end_pos);
    String read_eeprom_string(uint16_t start_pos, uint16_t end_pos);
    bool read_eeprom_bool(uint16_t *start_pos);
    int read_eeprom_int(uint16_t *start_pos);
    uint32_t read_eeprom_uint32(uint16_t *start_pos);
    float read_eeprom_float(uint16_t *start_pos);
    void write_eeprom_string(uint16_t start_pos, uint16_t end_pos, String string1);
    void write_eeprom_bool(uint16_t *start_pos, bool bool1);
    void write_eeprom_int(uint16_t *start_pos, int int1);
    void write_eeprom_uint32(uint16_t *start_pos, uint32_t uint32_1);
    void write_eeprom_float(uint16_t *start_pos, float float1);
    void eeprom_confirm();
    int save_internal_vars(float trreturn);
    void reset_internal_vars();
    int return_internal_vars(float trnow0, float trnow1, bool *return_success);  
};

#endif

#ifndef IR_REMOTE_H
#define IR_REMOTE_H
#include <IRrecv.h>
#include <IRsend.h>
#include "ir_protocols.h"
#include "led_controller.h"
#include "memory_manager.h"

extern MemoryManager mem;

class IRRemote{
  private:
    //const bool show_data = 0;
    const bool show_signal = 0;
    const bool show_bit = 1;
    const uint8_t min_trset = 18;
    uint16_t ps_buffer[586], min_bit_length, rmt_num;
    uint32_t last_cut_t;
    decode_results results;
    struct air_status_t{
      uint8_t power;
      uint8_t mode;
      uint8_t Trset;
      uint8_t fan;
      uint8_t vane;
    };
    air_status_t air_status;
    
    //rx-part
    boolean dump_control();
    
    boolean dump(decode_results *results);

    String get_rmt_const(String var, String *rmt_data);
    
    boolean check_rmt_num_and_change_state(uint16_t ps_cnt);

    void trans_air_status(String *bit_, int *power_, int *mode_, int *Trset_, int *fan_, int *vane_);

    String get_rmt_data_from_eeprom(uint16_t *rmt_num);
    
    bool spc_bit_rx(String *spec_bit_, int *spec_power_, int *spec_mode_, int *spec_temp_, int *spec_fan_, int *spec_vane_, String *remote_data);

    uint16_t GetOne(uint16_t ps_cnt);

    uint16_t GetZero(uint16_t ps_cnt);

    //tx-part
    void send_ir(int Power, int Mode, int Temp, int Fan, int Vane);
    
    void bit_to_ps_then_send(String *bit_code_, String *remote_data);

    void get_checksum(String *bit_ir);

    String get_const_bit_fan(int Fan, int Vane);

    String get_F_minus(String bit_ir);
    
  public:
    IRrecv irrecv;
    IRsend irsend;
    const uint8_t tolerance, khz;
    IRRemote(uint8_t rx_pinAttach, uint8_t tx_pinAttach, uint8_t toleranceAttach, uint8_t frequencyAttach):irrecv(rx_pinAttach),irsend(tx_pinAttach),tolerance(toleranceAttach),khz(frequencyAttach){
      air_status.power = 0;
      air_status.mode = 0 ;
      air_status.Trset = 25;
      air_status.fan = 0;
      air_status.vane = 0;
    };
    
    uint32_t ps_length_in_mcu_cycle;
    bool rmt_detected;
    

    //setup
    void setup();
    void reset();
    int getNum();
    void setNum(int num);
    
    //rx-part
    boolean check_ir();

    //tx-part
    void cut();

    void cool();

    void incTrset();  

    uint8_t getPower();

    uint8_t getMode();

    uint8_t getTrset();

    uint8_t getFan();

    uint8_t getVane();

    void setPower(uint8_t power);

    void setMode(uint8_t mode);

    void setTrset(uint8_t Trset);

    void setFan(uint8_t fan);

    void setVane(uint8_t vane);
};

#endif

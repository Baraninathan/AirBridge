#ifndef CONFIG_H
#define CONFIG_H
/**********************************************************************************
 * AirBridge v2.0 - Open Source WiFi-to-IR Bridge for Air Conditioners
 *
 * Universal IR relay supporting 60+ AC protocols across 12 major brands.
 * This is the open-source edition.
 *
 * ORIGINAL DEVELOPMENT (2016-2019):
 * Co-developed by Werapol Bejranonda and Pathawee Somsak
 *
 * OPEN-SOURCE RELEASE (2025):
 * Reorganized for open-source by Werapol Bejranonda
 *
 * REMOVED FEATURES (Available in commercial version at https://wenext.net/):
 * - Smart energy-saving algorithms (20-50% savings)
 * - Baseline learning and adaptation
 * - Adaptive compressor detection
 * - Remote monitoring (MQTT)
 * - Email reporting and analytics
 *
 * This version provides basic IR signal relay functionality only.
 * ********************************************************************************
 */

/*
 * Firmware info / Version
 */
const uint8_t firmware_version = 2;
const uint8_t firmware_build = 0;
const char firmware_release = 'a';

/*
 * State Machine States - Simplified for v2.0
 * Only 3 states in open-source version
 */
enum SystemState : uint8_t {
  STATE_WEB_CONFIG = 0,      // WiFi configuration via web portal
  STATE_STANDBY = 1,          // AC is off, system idle
  STATE_IR_RELAY = 2          // AC on, relaying IR signals (no intelligence)
};

/*
 * Air Conditioner Modes
 */
enum ACMode : uint8_t {
  MODE_POWER_OFF = 0,
  MODE_COOL = 1,
  MODE_DRY = 2,
  MODE_FAN = 3,
  MODE_AUTO = 4
};

/*
 * Array Size Constants
 */
const uint8_t TEMP_ARRAY_SIZE = 3;        // Size of temperature arrays (for display only)

/*
 * Debug / Testing
 */
const bool show_data = true;              // Enable serial debug output
const bool test_rmt = 0;                  // IR testing mode
const bool show_hardware_warning = 0;     // Display sensor warnings

/*
 * Time control
 */
const byte finish_webserver_loop = 20;    // Close webserver after N loops (~20 loops)
const uint32_t sleep_period = 617330;     // Main loop period (~5 seconds)

/*
 * Sensor boundary validation
 */
const float physical_sensor_boundary_tmin = -5.0;   // Minimum valid temperature (°C)
const float physical_sensor_boundary_tmax = 50.0;   // Maximum valid temperature (°C)

/*
 * Hardware pin configuration
 */
const uint8_t ONE_WIRE_BUS = 2;           // DS18B20 temperature sensors pin
const uint8_t frequency = 38;             // IR frequency (38kHz)
const uint8_t tolerance = 150;            // IR receiver tolerance
const uint8_t RX_PIN = 4;                 // IR receiver pin
const uint8_t TX_PIN = 5;                 // IR transmitter pin

/*
 * Temperature defaults
 */
const uint8_t default_trset = 25;                    // Default temperature setpoint
const uint8_t default_temperature_when_no_sensor = 26;  // Fallback if sensor fails

/*
 * Temperature setpoint limits (safety bounds)
 */
const uint8_t min_trset = 18;             // Minimum allowed setpoint
const uint8_t max_trset = 30;             // Maximum allowed setpoint

/*
 * Webserver configuration
 */
const int8_t web_install_duration = 5;    // Web config duration (minutes)

/*
 * Utility constants
 */
const String rn = "\r\n";                 // Carriage return + newline
const int default_val_ini_zero = 0;       // Default integer zero
const float default_val_intial_high = 99; // Default high value for initialization

#endif

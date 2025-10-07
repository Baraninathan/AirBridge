#ifndef TIMER_H
#define TIMER_H
#include <Arduino.h>

/**
 * @brief Non-blocking delay timer utility class
 *
 * Replaces blocking delay() calls with millis()-based timing
 * to prevent blocking the main loop execution.
 *
 * Example usage:
 *   Timer myTimer(1000);  // 1 second timer
 *   myTimer.start();
 *
 *   // In loop:
 *   if (myTimer.elapsed()) {
 *     // Timer has elapsed, do something
 *     myTimer.start();  // Restart if needed
 *   }
 */
class Timer {
  private:
    unsigned long start_time;
    unsigned long duration_ms;
    bool is_running;

  public:
    /**
     * @brief Construct a new Timer object
     * @param duration_milliseconds Timer duration in milliseconds
     */
    Timer(unsigned long duration_milliseconds)
      : duration_ms(duration_milliseconds), start_time(0), is_running(false) {}

    /**
     * @brief Start or restart the timer
     */
    void start() {
      start_time = millis();
      is_running = true;
    }

    /**
     * @brief Stop the timer
     */
    void stop() {
      is_running = false;
    }

    /**
     * @brief Check if timer has elapsed
     * @return true if timer is running and duration has passed
     * @return false if timer not running or duration not yet passed
     */
    bool elapsed() {
      if (!is_running) return false;
      return (millis() - start_time) >= duration_ms;
    }

    /**
     * @brief Check if timer has elapsed and auto-restart if so
     * @return true if timer elapsed (and was restarted)
     * @return false if timer not yet elapsed
     */
    bool elapsedAndRestart() {
      if (elapsed()) {
        start();
        return true;
      }
      return false;
    }

    /**
     * @brief Get time remaining in milliseconds
     * @return unsigned long milliseconds remaining (0 if elapsed)
     */
    unsigned long remaining() {
      if (!is_running) return 0;
      unsigned long current = millis();
      if (current - start_time >= duration_ms) return 0;
      return duration_ms - (current - start_time);
    }

    /**
     * @brief Set new duration without restarting timer
     * @param new_duration_ms New duration in milliseconds
     */
    void setDuration(unsigned long new_duration_ms) {
      duration_ms = new_duration_ms;
    }

    /**
     * @brief Get current duration setting
     * @return unsigned long Duration in milliseconds
     */
    unsigned long getDuration() const {
      return duration_ms;
    }

    /**
     * @brief Check if timer is currently running
     * @return true if timer is running
     * @return false if timer is stopped
     */
    bool isRunning() const {
      return is_running;
    }
};

#endif

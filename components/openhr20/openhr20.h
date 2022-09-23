#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"

namespace esphome
{
  namespace openhr20
  {

    class OpenHR20Component : public Component, public uart::UARTDevice
    {
    public:
      OpenHR20Component() = default;

      float get_setup_priority() const override;

      void setup() override;
      void dump_config() override;
      void loop() override;

      void set_mode_sensor(sensor::Sensor *mode_sensor) { mode_sensor_ = mode_sensor; }
      void set_window_open_sensor(sensor::Sensor *window_open_sensor) { window_open_sensor_ = window_open_sensor; }
      void set_locked_sensor(sensor::Sensor *locked_sensor) { locked_sensor_ = locked_sensor; }
      void set_current_temperature_sensor(sensor::Sensor *current_temperature_sensor) { current_temperature_sensor_ = current_temperature_sensor; }
      void set_wanted_temperature_sensor(sensor::Sensor *wanted_temperature_sensor) { wanted_temperature_sensor_ = wanted_temperature_sensor; }

    protected:
      sensor::Sensor *mode_sensor_{nullptr};
      sensor::Sensor *window_open_sensor_{nullptr};
      sensor::Sensor *locked_sensor_{nullptr};
      sensor::Sensor *current_temperature_sensor_{nullptr};
      sensor::Sensor *wanted_temperature_sensor_{nullptr};

    private:
      uint8_t data_[101];
      uint8_t data_index_{0};

      bool hasReadLineFromSerial();
      void resetBuffer();
    };
  }
}

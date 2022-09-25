#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/climate/climate.h"

namespace esphome
{
  namespace openhr20
  {
    class OpenHR20Component : public Component, public uart::UARTDevice, public climate::Climate
    {
    public:
      OpenHR20Component() = default;

      float get_setup_priority() const override;

      void setup() override;
      void dump_config() override;
      void loop() override;

    protected:
      void control(const ClimateCall &call) override;
      ClimateTraits traits() override;

    private:
      uint8_t data_[101];
      uint8_t data_index_{0};

      bool hasReadLineFromSerial();
      void resetBuffer();
    };
  }
}

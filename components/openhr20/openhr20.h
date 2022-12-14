#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/climate/climate.h"

namespace esphome
{
  namespace openhr20
  {
    static const char *TAG = "openhr20";

    class OpenHR20Climate : public climate::Climate, public uart::UARTDevice, public Component
    {
    public:
      float get_setup_priority() const override;

      void setup() override;
      void dump_config() override;
      void loop() override;

    protected:
      void control(const climate::ClimateCall &call) override;
      climate::ClimateTraits traits() override;

    private:
      uint8_t data_[256];
      uint8_t data_index_{0};

      bool hasReadLineFromSerial();
      void interpretBuffer();
      void resetBuffer();
      float decodeXXXX(uint8_t data[]);
      std::string encodeXX(uint8_t i);
    };
  }
}

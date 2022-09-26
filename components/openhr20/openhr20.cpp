#include "openhr20.h"

namespace esphome {
namespace openhr20 {

float OpenHR20Climate::get_setup_priority() const { return setup_priority::DATA; }

void OpenHR20Climate::setup()
{
  // nothing to do here
}

void OpenHR20Climate::loop()
{
  if (this->hasReadLineFromSerial())
  {
    this->interpretBuffer();
    this->resetBuffer();
  }
}

bool OpenHR20Climate::hasReadLineFromSerial()
{
  while (this->available() != 0)
  {
    this->read_byte(&this->data_[this->data_index_]);

    if (this->data_[this->data_index_] == '\n')
    {
      return true;
    }
  }

  return false;
}

void OpenHR20Climate::resetBuffer()
{
  this->data_index_ = 0;
}
// D: d3 21.09.22 09:18:54 M V: 58 I: 1943 S: 2100 B: 3322 Is: 00000000 Ib: 00 Ic: 00 Ie: 00 E:04 X W L
// I: temp average
// S: temp wanted, 5,0 (OFF) - 30,5 (ON) degree, on boot: BOOT
// first 55chars fix!
// next 34 optional, depending on #DEFINE
// last 11 depend on state (E,X,W,L)
// detect L
// detect W
// detect E?
void OpenHR20Climate::interpretBuffer()
{
  this->data_[this->data_index_] = '\0';
  ESP_LOGD("openhr20", "%s", this->data_);
  if (this->data_[0] != 'D' || this->data_[1] != ':')
  {
    return;
  }
  ESP_LOGD("openhr20", "got D");

  const auto hr20Mode = this->data_[24];
  this->mode = hr20Mode == 'A' ? climate::CLIMATE_MODE_AUTO : (hr20Mode == 'M' ? climate::CLIMATE_MODE_HEAT : climate::CLIMATE_MODE_OFF);

  this->current_temperature = (float)(int(this->data_[35]) * 1000 + int(this->data_[36]) * 100 + int(this->data_[37]) * 10 + int(this->data_[38])) / 100;

  if (this->data_[43] != 'B') // BOOT
  {
    this->target_temperature = (float)(int(this->data_[43] - 0x30) * 1000 + int(this->data_[44] - 0x30) * 100 + int(this->data_[45] - 0x30) * 10 + int(this->data_[46] - 0x30)) / 100;
  }

  /*bool isWindowOpen = false;
  bool isLocked = false;

  for (uint8_t i = 55; i < this->data_index_; i++)
  {
    switch (this->data_[i])
    {
    case 'W':
      isWindowOpen = true;
      break;
    case 'L':
      isLocked = true;
      break;
    case 'X':
    default:
    }
  }

  this->window_open_sensor_->publish_state(isWindowOpen);
  this->locked_sensor_->publish_state(isLocked);*/

  this->publish_state();
}

void OpenHR20Climate::dump_config()
{
  ESP_LOGCONFIG("openhr20", "OpenHR20:");
  LOG_CLIMATE("  ", "OpenHR20", this);
  this->check_uart_settings(9600);
}

void OpenHR20Climate::control(const climate::ClimateCall &call)
{
  if (call.get_mode().has_value())
  {
    // User requested mode change
    climate::ClimateMode mode = *call.get_mode();
    // Send mode to hardware
    // ...

    // Publish updated state
    this->mode = mode;
    this->publish_state();
  }
  if (call.get_target_temperature().has_value())
  {
    // User requested target temperature change
    float temp = *call.get_target_temperature();
    // Send target temp to climate
    // ...
  }
}
climate::ClimateTraits OpenHR20Climate::traits()
{
  auto traits = climate::ClimateTraits();
  traits.set_supports_current_temperature(true);
  traits.set_supported_modes({climate::CLIMATE_MODE_OFF, climate::CLIMATE_MODE_HEAT, climate::CLIMATE_MODE_AUTO});
  //traits.set_supports_action({climate::CLIMATE_ACTION_OFF, climate::CLIMATE_ACTION_HEATING, climate::CLIMATE_ACTION_IDLE});
  traits.set_visual_min_temperature(5.0);
  traits.set_visual_max_temperature(30.5);
  traits.set_visual_temperature_step(0.5);
  return traits;
}

} // namespace openhr20
} // namespace esphome

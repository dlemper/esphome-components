#include "openhr20.h"

float SDS011Component::get_setup_priority() const { return setup_priority::DATA; }

void OpenHR20Component::setup()
{
  // nothing to do here
}

void OpenHR20Component::loop()
{
  if (hasReadLineFromSerial())
  {
    interpretBuffer();
    resetBuffer();
  }
}

bool OpenHR20Component::hasReadLineFromSerial()
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

void OpenHR20Component::resetBuffer()
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
void OpenHR20Component::interpretBuffer()
{
  if (this->data_[0] != 'D' || this->data_[1] != ':')
  {
    return;
  }

  this->mode_sensor_->publish_state(this->data_[24]);

  const float current_temperature = (int(this->data_[35]) * 1000 + int(this->data_[36]) * 100 + int(this->data_[37]) * 10 + int(this->data_[38])) / 100;
  this->current_temperature_sensor_->publish_state(current_temperature);

  if (this->data_[43] != 'B') // BOOT
  {
    const float wanted_temperature = (int(this->data_[43]) * 1000 + int(this->data_[44]) * 100 + int(this->data_[45]) * 10 + int(this->data_[46])) / 100;
    this->wanted_temperature_sensor_->publish_state(wanted_temperature);
  }

  bool isWindowOpen = false;
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
  this->locked_sensor_->publish_state(isLocked);
}

void OpenHR20Component::dump_config()
{
  ESP_LOGCONFIG(TAG, "OpenHR20:");
  LOG_SENSOR("  ", "Mode", this->mode_sensor_);
  LOG_SENSOR("  ", "Window open", this->window_open_sensor_);
  LOG_SENSOR("  ", "Locked", this->locked_sensor_);
  LOG_SENSOR("  ", "Current temperature", this->current_temperature_sensor_);
  LOG_SENSOR("  ", "Wanted temperature", this->wanted_temperature_sensor_);
  this->check_uart_settings(9600);
}
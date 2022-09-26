import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate, uart
from esphome.const import CONF_ID

AUTO_LOAD = ["climate"]

DEPENDENCIES = ["uart"]

openhr20_ns = cg.esphome_ns.namespace("openhr20")
OpenHR20Climate = openhr20_ns.class_("OpenHR20Climate", climate.Climate, cg.Component, uart.UARTDevice)

CONFIG_SCHEMA = climate.CLIMATE_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(OpenHR20Climate),
    }
).extend(cv.Component).extend(uart.UART_DEVICE_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await climate.register_climate(var, config)
    await uart.register_uart_device(var, config)

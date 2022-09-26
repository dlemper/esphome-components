import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate
from esphome.const import CONF_ID

AUTO_LOAD = ["climate"]

DEPENDENCIES = ["uart"]

openhr20_ns = cg.esphome_ns.namespace("openhr20")
OpenHR20Climate = openhr20_ns.class_("OpenHR20Component", uart.UARTDevice, component, climate.Climate)

CONFIG_SCHEMA = climate.CLIMATE_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(OpenHR20Climate),
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await climate.register_climate(var, config)

import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.const import CONF_ID
from esphome.components import text

CONF_NUMBER_OF_MODULES = "number_of_modules"

fallblatt_ns = cg.esphome_ns.namespace("fallblatt")

FallblattABC = fallblatt_ns.class_(
    "FallblattABC", cg.Component, text.Text
)

CONFIG_SCHEMA = text.TEXT_SCHEMA.extend({
    cv.Required(CONF_ID): cv.declare_id(FallblattABC),
    cv.Required(CONF_NUMBER_OF_MODULES):  cv.positive_int
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID], config[CONF_NUMBER_OF_MODULES])
    await cg.register_component(var, config)
    await text.register_text(var, config)

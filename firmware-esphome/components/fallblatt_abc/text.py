import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.const import CONF_ID
from esphome.components import text

fallblatt_ns = cg.esphome_ns.namespace("fallblatt")

FallblattABC = fallblatt_ns.class_(
    "FallblattABC", cg.Component, text.Text
)

CONFIG_SCHEMA = text.TEXT_SCHEMA.extend({
    cv.Required(CONF_ID): cv.declare_id(FallblattABC),
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await text.register_text(var, config)

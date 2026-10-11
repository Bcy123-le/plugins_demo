#include "plugincontext.h"
#include "typedbus.h"
#include "serviceregistry.h"

PluginContext::PluginContext(TypedBus *bus, ServiceRegistry *registry, QObject *host)
    : m_bus(bus)
    , m_registry(registry)
    , m_host(host)
{
}

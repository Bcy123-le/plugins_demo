#ifndef PLUGINCONTEXT_H
#define PLUGINCONTEXT_H

#include "common_global.h"

class TypedBus;
class ServiceRegistry;
class QObject;

// 插件上下文：宿主在插件 initialize() 时注入，
// 向插件开放宿主的公共服务（强类型信号总线、服务注册表、宿主对象）。
class COMMON_EXPORT PluginContext
{
public:
    PluginContext() = default;
    PluginContext(TypedBus *bus, ServiceRegistry *registry, QObject *host);

    TypedBus        *bus()      const { return m_bus; }       // 强类型信号总线
    ServiceRegistry *services() const { return m_registry; }  // 服务注册表
    QObject         *host()     const { return m_host; }      // 宿主对象

private:
    TypedBus        *m_bus      = nullptr;
    ServiceRegistry *m_registry = nullptr;
    QObject         *m_host     = nullptr;
};

#endif // PLUGINCONTEXT_H

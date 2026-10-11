#include "serviceregistry.h"

// 单例：同样位于 libcommon.so 内，全局唯一
ServiceRegistry *ServiceRegistry::instance()
{
    static ServiceRegistry reg;
    return &reg;
}

void ServiceRegistry::registerService(const QString &name, QObject *service)
{
    m_services.insert(name, service);
}

void ServiceRegistry::unregisterService(const QString &name)
{
    m_services.remove(name);
}

QObject *ServiceRegistry::service(const QString &name) const
{
    return m_services.value(name, nullptr);
}

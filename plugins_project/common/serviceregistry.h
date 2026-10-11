#ifndef SERVICEREGISTRY_H
#define SERVICEREGISTRY_H

#include "common_global.h"

#include <QObject>
#include <QHash>
#include <QString>

// 服务注册表：插件间“服务定位”式通信的核心。
// 插件可把自己注册为命名服务，供其它插件按名字查找并直接调用，
// 适用于需要同步返回值的强类型调用场景。
class COMMON_EXPORT ServiceRegistry : public QObject
{
    Q_OBJECT
public:
    static ServiceRegistry *instance();

    void registerService(const QString &name, QObject *service);
    void unregisterService(const QString &name);

    QObject *service(const QString &name) const;

    // 便捷：按名字查找并转型为具体类型
    template<typename T>
    T *find(const QString &name) const { return qobject_cast<T *>(service(name)); }

private:
    explicit ServiceRegistry(QObject *parent = nullptr) : QObject(parent) {}

    QHash<QString, QObject *> m_services;
};

#endif // SERVICEREGISTRY_H

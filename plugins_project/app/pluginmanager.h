#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include "iplugin.h"
#include "plugincontext.h"

#include <QObject>
#include <QVector>
#include <QPluginLoader>

// 插件管理器：扫描插件目录、加载插件、管理生命周期。
class PluginManager : public QObject
{
    Q_OBJECT
public:
    explicit PluginManager(QObject *parent = nullptr);

    // 扫描 dir 下所有可用插件并加载
    void loadAll(const QString &dir, PluginContext *ctx);

    const QVector<IPlugin *> &plugins() const { return m_plugins; }
    IPlugin *findPlugin(const QString &name) const;

    // 按安全顺序卸载全部插件
    void unloadAll();

private:
    QVector<IPlugin *>       m_plugins;
    QVector<QPluginLoader *> m_loaders;
};

#endif // PLUGINMANAGER_H

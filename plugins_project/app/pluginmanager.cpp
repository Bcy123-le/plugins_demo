#include "pluginmanager.h"

#include <QDir>
#include <QFileInfo>
#include <QLibrary>
#include <QDebug>

PluginManager::PluginManager(QObject *parent)
    : QObject(parent)
{
}

void PluginManager::loadAll(const QString &dir, PluginContext *ctx)
{
    QDir pluginsDir(dir);
    if (!pluginsDir.exists()) {
        qWarning() << "插件目录不存在:" << dir;
        return;
    }

    const QStringList entries = pluginsDir.entryList(QDir::Files);
    for (const QString &fileName : entries) {
        const QString path = pluginsDir.absoluteFilePath(fileName);

        // 只尝试加载“动态库”文件（.so）
        if (!QLibrary::isLibrary(path))
            continue;

        QPluginLoader *loader = new QPluginLoader(path, this);
        QObject *instance = loader->instance();
        if (!instance) {
            qWarning() << "加载失败:" << fileName << "-" << loader->errorString();
            delete loader;
            continue;
        }

        IPlugin *plugin = qobject_cast<IPlugin *>(instance);
        if (!plugin) {
            qWarning() << "非本系统插件，已忽略:" << fileName;
            loader->unload();
            delete loader;
            continue;
        }

        // 注入上下文并初始化
        if (!plugin->initialize(ctx)) {
            qWarning() << "插件初始化失败:" << plugin->name();
            loader->unload();
            delete loader;
            continue;
        }

        m_plugins.append(plugin);
        m_loaders.append(loader);
        qInfo() << "已加载插件:" << plugin->name() << "v" << plugin->version();
    }
}

IPlugin *PluginManager::findPlugin(const QString &name) const
{
    for (IPlugin *p : m_plugins) {
        if (p->name() == name)
            return p;
    }
    return nullptr;
}

void PluginManager::unloadAll()
{
    // ① 先关闭插件逻辑
    for (IPlugin *p : m_plugins)
        p->shutdown();

    // ② 再卸载 .so
    for (QPluginLoader *l : m_loaders)
        l->unload();

    m_plugins.clear();
    m_loaders.clear();
}

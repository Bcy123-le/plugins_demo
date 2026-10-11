#ifndef PLUGINHELLOPLUGIN_H
#define PLUGINHELLOPLUGIN_H

#include "pluginbase.h"
#include "plugincontext.h"

#include <QObject>

// 插件 plugin_hello 的工厂类（Qt 插件入口）。
// 继承 PluginBase 以获得 IPlugin 的通用默认实现；只需实现自己关心的部分。
class PluginHelloPlugin : public PluginBase
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID IPlugin_iid FILE "plugin_hello.json")
    Q_INTERFACES(IPlugin)

public:
    QString  name()    const override { return QStringLiteral("Hello"); }
    QString  version() const override { return QStringLiteral("1.0.0"); }

    QWidget *createWidget(QWidget *parent) override;
};

#endif // PLUGINHELLOPLUGIN_H

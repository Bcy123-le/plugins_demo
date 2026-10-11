#ifndef PLUGINDEMOB_H
#define PLUGINDEMOB_H

#include "iplugin.h"
#include "plugincontext.h"

#include <QObject>

// 插件 B 的工厂类（Qt 插件入口）
class PluginDemoB : public QObject, public IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID IPlugin_iid FILE "plugin_demo_b.json")
    Q_INTERFACES(IPlugin)

public:
    // ---- IPlugin 实现 ----
    QString  name()     const override { return QStringLiteral("Demo B"); }
    QString  version()  const override { return QStringLiteral("1.0.0"); }
    QString  category() const override { return QStringLiteral("示例"); }

    bool     initialize(PluginContext *ctx) override;
    void     shutdown() override;
    QWidget *createWidget(QWidget *parent) override;

private:
    PluginContext *m_ctx = nullptr;
};

#endif // PLUGINDEMOB_H

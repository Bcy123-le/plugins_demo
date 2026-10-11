#ifndef PLUGINDEMOA_H
#define PLUGINDEMOA_H

#include "iplugin.h"
#include "plugincontext.h"

#include <QObject>

// 插件 A 的工厂类（Qt 插件入口）。
// 同时继承 QObject 与 IPlugin，通过 Q_INTERFACES 暴露接口。
class PluginDemoA : public QObject, public IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID IPlugin_iid FILE "plugin_demo_a.json")
    Q_INTERFACES(IPlugin)

public:
    // ---- IPlugin 实现 ----
    QString  name()     const override { return QStringLiteral("Demo A"); }
    QString  version()  const override { return QStringLiteral("1.0.0"); }
    QString  category() const override { return QStringLiteral("示例"); }

    bool     initialize(PluginContext *ctx) override;
    void     shutdown() override;
    QWidget *createWidget(QWidget *parent) override;

    // ---- 对外提供的“服务”方法（供其它插件通过服务注册表调用）----
    Q_INVOKABLE int compute(int a, int b) const { return a + b; }

private:
    PluginContext *m_ctx = nullptr;
};

#endif // PLUGINDEMOA_H

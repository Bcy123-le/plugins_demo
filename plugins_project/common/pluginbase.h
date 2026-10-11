#ifndef PLUGINBASE_H
#define PLUGINBASE_H

#include "common_global.h"
#include "iplugin.h"
#include "plugincontext.h"

#include <QObject>

// 插件基类：预实现 IPlugin 的通用样板，减少每个插件的重复代码。
//
// 插件工厂类继承本类后，只需关注自己真正关心的部分（通常是 name/version/createWidget）。
// 默认已提供：initialize（保存 ctx）、shutdown（空）、category、icon、createWidget（返回 nullptr）。
//
// 注意：Q_PLUGIN_METADATA 与 Q_INTERFACES 必须保留在“具体插件类”上（基类无法替代这两个宏）。
// name() 与 version() 仍是纯虚，需由具体插件实现。
class COMMON_EXPORT PluginBase : public QObject, public IPlugin
{
    Q_OBJECT
public:
    // 默认实现：保存注入的上下文；需要自定义初始化（如注册服务）时可 override，
    // 并在其中调用 PluginBase::initialize(ctx)。
    bool initialize(PluginContext *ctx) override
    {
        m_ctx = ctx;
        return true;
    }

    void     shutdown() override {}
    QString  category() const override { return QStringLiteral("General"); }
    QIcon    icon()     const override { return QIcon(); }
    QWidget *createWidget(QWidget *parent = nullptr) override
    {
        Q_UNUSED(parent);
        return nullptr;   // 纯后台插件无需 UI，保持默认即可
    }

protected:
    PluginContext *m_ctx = nullptr;   // 供子类访问（protected）
};

#endif // PLUGINBASE_H

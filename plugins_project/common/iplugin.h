#ifndef IPLUGIN_H
#define IPLUGIN_H

#include "common_global.h"
#include "plugincontext.h"

#include <QtCore/qobjectdefs.h>
#include <QtGui/qicon.h>
#include <QtWidgets/qwidget.h>

// 所有插件必须实现的统一接口（纯虚接口，非 QObject）。
// 每个插件的工厂类通过“多继承 QObject + IPlugin + Q_INTERFACES(IPlugin)”暴露。
class COMMON_EXPORT IPlugin
{
public:
    virtual ~IPlugin() {}

    virtual QString  name()     const = 0;
    virtual QString  version()  const = 0;
    virtual QString  category() const { return QStringLiteral("General"); }
    virtual QIcon    icon()     const { return QIcon(); }

    // 初始化：宿主在此注入 PluginContext；返回 false 表示加载失败
    virtual bool     initialize(PluginContext *ctx) = 0;
    virtual void     shutdown() = 0;

    // UI 入口：返回一个控件，由宿主作为“子页面”嵌入（例如 QTabWidget）。
    // 纯后台插件保持默认返回 nullptr 即可。
    virtual QWidget *createWidget(QWidget *parent = nullptr)
    {
        Q_UNUSED(parent);
        return nullptr;
    }
};

#define IPlugin_iid "org.qproject.IPlugin/1.0"
Q_DECLARE_INTERFACE(IPlugin, IPlugin_iid)

#endif // IPLUGIN_H

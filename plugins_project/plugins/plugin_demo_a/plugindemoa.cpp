#include "plugindemoa.h"
#include "plugindemowidget.h"

#include "serviceregistry.h"

#include <QDebug>

bool PluginDemoA::initialize(PluginContext *ctx)
{
    m_ctx = ctx;

    // 把自己注册为命名服务，供其它插件按名字查找并调用 compute()
    if (ctx->services())
        ctx->services()->registerService("plugin_a", this);

    qInfo() << "PluginDemoA 初始化完成";
    return true;
}

void PluginDemoA::shutdown()
{
    if (m_ctx && m_ctx->services())
        m_ctx->services()->unregisterService("plugin_a");
}

QWidget *PluginDemoA::createWidget(QWidget *parent)
{
    // 返回插件自带的 UI 控件，由宿主作为子页面嵌入
    return new PluginDemoWidget(m_ctx, parent);
}

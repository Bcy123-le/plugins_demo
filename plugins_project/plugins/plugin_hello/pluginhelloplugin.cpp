#include "pluginhelloplugin.h"
#include "pluginhellowidget.h"

QWidget *PluginHelloPlugin::createWidget(QWidget *parent)
{
    // 返回插件自带的 UI 控件，由宿主作为子页面嵌入
    return new PluginHelloWidget(m_ctx, parent);
}

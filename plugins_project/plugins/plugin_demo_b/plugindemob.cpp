#include "plugindemob.h"
#include "plugindemowidgetb.h"

#include <QDebug>

bool PluginDemoB::initialize(PluginContext *ctx)
{
    m_ctx = ctx;
    qInfo() << "PluginDemoB 初始化完成";
    return true;
}

void PluginDemoB::shutdown()
{
}

QWidget *PluginDemoB::createWidget(QWidget *parent)
{
    return new PluginDemoWidgetB(m_ctx, parent);
}

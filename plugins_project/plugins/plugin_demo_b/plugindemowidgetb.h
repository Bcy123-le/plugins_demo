#ifndef PLUGINDEMOWIDGETB_H
#define PLUGINDEMOWIDGETB_H

#include "plugincontext.h"

#include <QWidget>

namespace Ui { class PluginDemoWidgetB; }

// 插件 B 的界面：订阅“data/changed”消息并显示；可调用插件 A 的服务。
class PluginDemoWidgetB : public QWidget
{
    Q_OBJECT

public:
    explicit PluginDemoWidgetB(PluginContext *ctx, QWidget *parent = nullptr);
    ~PluginDemoWidgetB();

private slots:
    void on_callServiceButton_clicked();

private:
    Ui::PluginDemoWidgetB *ui;
    PluginContext *m_ctx;
};

#endif // PLUGINDEMOWIDGETB_H

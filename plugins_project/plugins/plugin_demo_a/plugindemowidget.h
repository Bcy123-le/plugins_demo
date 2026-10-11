#ifndef PLUGINDEMOWIDGET_H
#define PLUGINDEMOWIDGET_H

#include "plugincontext.h"

#include <QWidget>

namespace Ui { class PluginDemoWidget; }

// 插件 A 的界面：独立设计，最终被宿主嵌入为主程序的一个子页面。
class PluginDemoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PluginDemoWidget(PluginContext *ctx, QWidget *parent = nullptr);
    ~PluginDemoWidget();

private slots:
    void on_sendButton_clicked();

private:
    Ui::PluginDemoWidget *ui;
    PluginContext *m_ctx;
};

#endif // PLUGINDEMOWIDGET_H

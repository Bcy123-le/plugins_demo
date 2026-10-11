#ifndef PLUGINHELLOWIDGET_H
#define PLUGINHELLOWIDGET_H

#include "plugincontext.h"

#include <QWidget>

namespace Ui { class PluginHelloWidget; }

// 插件 plugin_hello 的界面：独立设计，最终被宿主嵌入为主程序的一个子页面。
class PluginHelloWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PluginHelloWidget(PluginContext *ctx, QWidget *parent = nullptr);
    ~PluginHelloWidget();

private slots:
    void on_sendButton_clicked();

private:
    Ui::PluginHelloWidget *ui;
    PluginContext *m_ctx;
};

#endif // PLUGINHELLOWIDGET_H

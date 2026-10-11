#include "plugindemowidgetb.h"
#include "ui_plugindemowidgetb.h"

#include "typedbus.h"
#include "messages.h"
#include "serviceregistry.h"

#include <QDebug>

PluginDemoWidgetB::PluginDemoWidgetB(PluginContext *ctx, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PluginDemoWidgetB)
    , m_ctx(ctx)
{
    ui->setupUi(this);

    // ★ 通信方式一：强类型信号总线订阅
    // 监听 msgs::ValueChanged；receiver 传 this，销毁时自动退订
    if (m_ctx->bus()) {
        m_ctx->bus()->on<msgs::ValueChanged>(this,
            [this](const msgs::ValueChanged &m) {
                ui->receivedLabel->setText(
                    tr("收到来自 %1 的广播: value = %2")
                        .arg(m.source).arg(m.value));
            });
    }
}

PluginDemoWidgetB::~PluginDemoWidgetB()
{
    delete ui;
}

void PluginDemoWidgetB::on_callServiceButton_clicked()
{
    // ★ 通信方式二：服务定位（同步调用插件 A 的 compute()）
    if (!m_ctx->services())
        return;

    // 取回注册的服务对象（其真实类型是插件 A 的工厂对象，本身是 QObject）。
    // 这里仅按“名字 + 元对象系统”调用，无需在编译期依赖插件 A 的具体类型，
    // 因此插件 B 与插件 A 之间保持解耦。
    QObject *a = m_ctx->services()->service("plugin_a");
    if (!a) {
        ui->receivedLabel->setText(tr("未找到插件 A 的服务"));
        return;
    }

    // 通过元对象系统调用 A 的 Q_INVOKABLE int compute(int,int)
    const int x = ui->spinBoxX->value();
    const int y = ui->spinBoxY->value();
    int result = 0;
    const bool ok = QMetaObject::invokeMethod(
        a, "compute",
        Q_RETURN_ARG(int, result),
        Q_ARG(int, x),
        Q_ARG(int, y));

    if (ok)
        ui->receivedLabel->setText(
            tr("调用插件A.compute(%1, %2) = %3").arg(x).arg(y).arg(result));
    else
        ui->receivedLabel->setText(tr("调用插件 A 服务失败"));
}

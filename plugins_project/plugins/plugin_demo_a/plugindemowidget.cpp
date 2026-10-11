#include "plugindemowidget.h"
#include "ui_plugindemowidget.h"

#include "typedbus.h"
#include "messages.h"

PluginDemoWidget::PluginDemoWidget(PluginContext *ctx, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PluginDemoWidget)
    , m_ctx(ctx)
{
    ui->setupUi(this);
}

PluginDemoWidget::~PluginDemoWidget()
{
    delete ui;
}

void PluginDemoWidget::on_sendButton_clicked()
{
    // 通过强类型信号总线发布一条 ValueChanged 消息，
    // 订阅了该类型的插件（如 Demo B）或主程序都会收到。
    if (m_ctx->bus()) {
        msgs::ValueChanged msg;
        msg.value  = ui->spinBox->value();
        msg.source = QStringLiteral("A");
        m_ctx->bus()->publish<msgs::ValueChanged>(msg);
    }
}

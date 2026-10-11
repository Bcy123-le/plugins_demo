#include "pluginhellowidget.h"
#include "ui_pluginhellowidget.h"

#include "typedbus.h"
#include "messages.h"

PluginHelloWidget::PluginHelloWidget(PluginContext *ctx, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PluginHelloWidget)
    , m_ctx(ctx)
{
    ui->setupUi(this);
}

PluginHelloWidget::~PluginHelloWidget()
{
    delete ui;
}

void PluginHelloWidget::on_sendButton_clicked()
{
    // 示例：通过强类型信号总线发布一条 ValueChanged 消息
    if (m_ctx->bus()) {
        msgs::ValueChanged msg;
        msg.value  = ui->spinBox->value();
        msg.source = QStringLiteral("plugin_hello");
        m_ctx->bus()->publish<msgs::ValueChanged>(msg);
    }
}

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "typedbus.h"
#include "serviceregistry.h"

#include <QDir>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_ctx(TypedBus::instance(), ServiceRegistry::instance(), this)
{
    ui->setupUi(this);
    setWindowTitle(tr("QProject 插件化主程序"));

    loadPlugins();
}

void MainWindow::loadPlugins()
{
    // 插件目录：优先使用编译期注入的路径，其次相对可执行文件
    QString pluginsDir = QStringLiteral(PLUGINS_DIR);
    if (pluginsDir.isEmpty() || !QDir(pluginsDir).exists()) {
        pluginsDir = QCoreApplication::applicationDirPath() + "/../plugins";
    }
    pluginsDir = QDir::cleanPath(pluginsDir);
    qInfo() << "插件目录:" << pluginsDir;

    m_manager.loadAll(pluginsDir, &m_ctx);

    // 把每个带 UI 的插件挂到标签页，作为“子页面”显示
    for (IPlugin *plugin : m_manager.plugins()) {
        QWidget *page = plugin->createWidget(ui->tabPages);
        if (page) {
            const int idx = ui->tabPages->addTab(page, plugin->icon(), plugin->name());
            m_pluginTabs.insert(plugin->name(), idx);
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

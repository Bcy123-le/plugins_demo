#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "plugincontext.h"
#include "pluginmanager.h"

#include <QMainWindow>
#include <QMap>

namespace Ui { class MainWindow; }

// 宿主主窗口：持有公共服务（TypedBus 强类型总线 / ServiceRegistry），
// 加载插件并把每个插件的 UI 挂到标签页容器中显示。
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void loadPlugins();

    Ui::MainWindow *ui;
    PluginContext   m_ctx;      // 提供给所有插件的上下文
    PluginManager   m_manager;  // 插件管理
    QMap<QString, int> m_pluginTabs; // 插件名 -> 标签索引（便于卸载）
};

#endif // MAINWINDOW_H

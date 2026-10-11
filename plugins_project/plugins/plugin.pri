# plugins/plugin.pri —— 插件公共构建模板
#
# 每个插件的 .pro 只需：设置 PLUGIN_NAME，include 本文件，再列出自己的源文件。
# 本模板集中了 TEMPLATE/CONFIG/QT/TARGET/DESTDIR/INSTALLS 等公共配置，
# 并自动 include ../../common.pri（链接 libcommon.so、设置头文件路径与 RPATH）。
#
# 约定：
#   PLUGIN_NAME : 插件目标名（如 plugin_hello），决定 TARGET、.json 名、安装目录
#   PLUGIN_JSON : 元数据文件名（可选，默认 $$PLUGIN_NAME.json）
#   插件 .pro 自行追加 SOURCES / HEADERS / FORMS（不建议用 *.cpp 通配，
#   因为非影子构建下 moc_*.cpp、ui_*.h 会生成在插件目录，通配会误收导致问题）
#
# 用法示例（plugins/plugin_hello/plugin_hello.pro）：
#   PLUGIN_NAME = plugin_hello
#   include(../../plugins/plugin.pri
#   SOURCES += pluginhello.cpp pluginhellowidget.cpp
#   HEADERS += pluginhello.h  pluginhellowidget.h
#   FORMS   += pluginhellowidget.ui

isEmpty(PLUGIN_NAME): error("请先在 include plugin.pri 之前设置 PLUGIN_NAME，例如：PLUGIN_NAME = plugin_xxx")

# 链接通用组件共享库、头文件路径、RPATH、公共宏
# 注意：本 .pri 位于 plugins/ 目录，其 $$PWD 即 plugins/，故到工程根 common.pri 为 ../common.pri
include(../common.pri)

TEMPLATE = lib
CONFIG  += plugin

QT += core gui widgets

TARGET  = $$PLUGIN_NAME
DESTDIR = $$QP_BUILD_PLUGINS

isEmpty(PLUGIN_JSON): PLUGIN_JSON = $$PLUGIN_NAME.json
OTHER_FILES += $$PLUGIN_JSON

target.path = $$QP_BUILD_PLUGINS
INSTALLS += target

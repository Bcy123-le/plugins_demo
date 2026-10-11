# 插件 A：带 UI、注册命名服务、通过强类型总线发布消息
PLUGIN_NAME = plugin_demo_a

include(../../plugins/plugin.pri)

HEADERS += \
    plugin_demo_a_global.h \
    plugindemoa.h \
    plugindemowidget.h

SOURCES += \
    plugindemoa.cpp \
    plugindemowidget.cpp

FORMS += \
    plugindemowidget.ui

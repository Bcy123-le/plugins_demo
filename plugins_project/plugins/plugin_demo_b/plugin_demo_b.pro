# 插件 B：订阅强类型总线消息，并通过服务注册表调用插件 A 的服务
PLUGIN_NAME = plugin_demo_b

include(../../plugins/plugin.pri)

HEADERS += \
    plugin_demo_b_global.h \
    plugindemob.h \
    plugindemowidgetb.h

SOURCES += \
    plugindemob.cpp \
    plugindemowidgetb.cpp

FORMS += \
    plugindemowidgetb.ui

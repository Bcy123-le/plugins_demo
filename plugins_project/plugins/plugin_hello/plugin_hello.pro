# 插件 plugin_hello
PLUGIN_NAME = plugin_hello

include(../../plugins/plugin.pri)

HEADERS += \
    plugin_hello_global.h \
    pluginhelloplugin.h \
    pluginhellowidget.h

SOURCES += \
    pluginhelloplugin.cpp \
    pluginhellowidget.cpp

FORMS += \
    pluginhellowidget.ui

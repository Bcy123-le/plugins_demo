include(../common.pri)

# 主程序 host
TEMPLATE = app

QT += core gui widgets

TARGET  = qproject_app
DESTDIR = $$QP_BUILD_APP

# 将“插件输出目录”以宏注入，供运行时定位插件
DEFINES += PLUGINS_DIR=\\\"$$QP_BUILD_PLUGINS\\\"

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    pluginmanager.cpp

HEADERS += \
    mainwindow.h \
    pluginmanager.h

FORMS += \
    mainwindow.ui

target.path = $$QP_BUILD_APP
INSTALLS += target

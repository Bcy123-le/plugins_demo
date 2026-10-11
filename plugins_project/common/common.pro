# 标记“当前正在构建 common 自身”，供 common.pri 判断是否跳过 -lcommon 自链接
IS_COMMON_LIB = true

include(../common.pri)

# 通用组件：构建为共享库
TEMPLATE = lib
CONFIG  += shared

QT += core gui widgets

# 编译本库时定义 COMMON_LIBRARY，触发导出宏
DEFINES += COMMON_LIBRARY

TARGET  = common
DESTDIR = $$QP_BUILD_COMMON

HEADERS += \
    common_global.h \
    typedbus.h \
    messages.h \
    pluginbase.h \
    serviceregistry.h \
    plugincontext.h \
    iplugin.h

SOURCES += \
    typedbus.cpp \
    serviceregistry.cpp \
    plugincontext.cpp

# 安装到统一输出目录（可选）
target.path = $$QP_BUILD_COMMON
INSTALLS += target

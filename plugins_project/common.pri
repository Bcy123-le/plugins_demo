# common.pri —— 所有子项目共享的构建配置
#
# 关键点：本文件被 include 时，其内部的 $$PWD 指向“本 .pri 文件所在目录”，
# 即工程根目录（/home/bcy/istudy/customwidget/QProject）。据此可靠定位各路径，
# 无论是否使用影子构建（shadow build）均成立。

# ---------- 源码目录定位 ----------
QP_ROOT         = $$PWD
QP_COMMON       = $$QP_ROOT/common
QP_PLUGINS_SRC  = $$QP_ROOT/plugins
QP_3RD          = $$QP_ROOT/3rdparty

# ---------- 统一构建输出目录 ----------
QP_BUILD          = $$QP_ROOT/build
QP_BUILD_COMMON   = $$QP_BUILD/common
QP_BUILD_APP      = $$QP_BUILD/app
QP_BUILD_PLUGINS  = $$QP_BUILD/plugins

# ---------- 头文件搜索路径 ----------
# 通用组件头文件 + 第三方公共库头文件，供所有子项目使用
INCLUDEPATH += $$QP_COMMON
INCLUDEPATH += $$QP_3RD/include

# ---------- 链接通用组件共享库 ----------
# 注意：common 必须构建为共享库（shared），否则 TypedBus/ServiceRegistry 等单例
# 会被每个插件各自拷贝一份，插件之间将无法通信。
# 仅“使用者”（app 与各插件）链接 libcommon；common 自身不链接（避免自引用）。
isEmpty(IS_COMMON_LIB) {
    LIBS += -L$$QP_BUILD_COMMON -lcommon
    # 运行时查找 libcommon.so：使用绝对 RPATH，保证定位到唯一的 libcommon.so。
    # （如需“可移植发布”，可再追加 $ORIGIN/../common 等相对 RPATH。）
    QMAKE_RPATHDIR += $$QP_BUILD_COMMON
}

# ---------- 公共编译设置 ----------
CONFIG += c++14

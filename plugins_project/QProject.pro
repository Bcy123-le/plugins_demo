# QProject —— 主工程：以 subdirs 方式统一编排顶层子项目
# 构建顺序：先 common（共享库），再 app；插件经 plugins 中间层编排，
# 各插件构建时链接 libcommon.so。
TEMPLATE = subdirs
CONFIG  += ordered

SUBDIRS += \
    common \
    app \
    plugins

# 便于阅读的说明：
#  - common           : 通用组件（接口 IPlugin、上下文 PluginContext、事件总线、服务注册表）
#  - app              : 主程序 host，负责扫描/加载插件、把插件 UI 挂到标签页
#  - plugins          : 插件的中间编排层（见 plugins/plugins.pro），向下编排各独立插件
#  - plugin_demo_a/b  : 由 plugins 层编排的独立插件子项目，通过事件总线/服务注册表互相通信


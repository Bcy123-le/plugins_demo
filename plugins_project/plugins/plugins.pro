# plugins/plugins.pro —— 插件的中间编排层
# 由主工程 QProject.pro 引用（作为其 SUBDIRS 之一），向下编排各独立插件子项目。
# 本文件为“纯编排层”：自身不编译代码、不链接库，仅按顺序构建下列插件；
# 各插件通过 include(../../plugins/plugin.pri) 自行链接 libcommon.so。
#
# 说明：SUBDIRS 清单由脚手架脚本 tools/newplugin.sh 自动维护——
# 新增插件时运行该脚本，它会自动把插件目录名追加到 SUBDIRS 清单末尾，无需手动编辑。
TEMPLATE = subdirs
CONFIG  += ordered

# === BEGIN plugin list (auto-managed by tools/newplugin.sh) ===
SUBDIRS += plugin_demo_a
SUBDIRS += plugin_demo_b
SUBDIRS += plugin_hello
# === END plugin list ===




#!/usr/bin/env bash
# newplugin.sh —— 生成一个新插件骨架，并自动注册到 plugins/plugins.pro
#
# 用法：
#   ./tools/newplugin.sh <plugin_name> [显示名]
# 示例：
#   ./tools/newplugin.sh plugin_hello "Hello"
#
# 生成的插件默认：带一个 UI 页面、继承 PluginBase、通过 TypedBus 发布一条示例消息。
# 生成后只需重新 qmake + make 即可被主程序自动加载。

set -euo pipefail

# ---------- 参数解析 ----------
if [[ $# -lt 1 ]]; then
    echo "用法: $0 <plugin_name> [显示名]" >&2
    echo "示例: $0 plugin_hello \"Hello\"" >&2
    exit 1
fi

PLUGIN_NAME="$1"                 # 例如 plugin_hello（目录名/目标名/json名）
DISPLAY_NAME="${2:-$PLUGIN_NAME}"

if ! [[ "$PLUGIN_NAME" =~ ^[a-z][a-z0-9_]*$ ]]; then
    echo "错误: 插件名需为小写字母/数字/下划线，且以字母开头: $PLUGIN_NAME" >&2
    exit 1
fi

# ---------- 路径定位 ----------
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
PLUGINS_DIR="$ROOT_DIR/plugins"
TARGET_DIR="$PLUGINS_DIR/$PLUGIN_NAME"
PLUGINS_PRO="$PLUGINS_DIR/plugins.pro"

if [[ -e "$TARGET_DIR" ]]; then
    echo "错误: 目标目录已存在: $TARGET_DIR" >&2
    exit 1
fi

# ---------- 名称推导 ----------
to_pascal() { echo "$1" | awk -F'_' '{ out=""; for(i=1;i<=NF;i++){ out=out toupper(substr($i,1,1)) substr($i,2) } print out }'; }
upper()     { echo "$1" | tr 'a-z' 'A-Z'; }
lower_nous() { echo "$1" | tr -d '_' | tr 'A-Z' 'a-z'; }

CLASS_PREFIX="$(to_pascal "$PLUGIN_NAME")"     # PluginHello
PLUGIN_CLASS="${CLASS_PREFIX}Plugin"            # PluginHelloPlugin
WIDGET_CLASS="${CLASS_PREFIX}Widget"            # PluginHelloWidget
PLUGIN_FILE="$(lower_nous "$PLUGIN_CLASS")"    # pluginhelloplugin
WIDGET_FILE="$(lower_nous "$WIDGET_CLASS")"    # pluginhellowidget
GLOBAL_HEADER="${PLUGIN_NAME}_global.h"
GLOBAL_GUARD="$(upper "${PLUGIN_NAME}_global_h")"
MACRO="$(upper "$PLUGIN_NAME")"                 # PLUGIN_HELLO
PLUGIN_GUARD="$(upper "${PLUGIN_CLASS}_h")"
WIDGET_GUARD="$(upper "${WIDGET_CLASS}_h")"
JSON_FILE="${PLUGIN_NAME}.json"

echo "==> 生成插件: $PLUGIN_NAME (显示名: $DISPLAY_NAME)"
echo "    插件类: $PLUGIN_CLASS   界面类: $WIDGET_CLASS"

mkdir -p "$TARGET_DIR"

# ---------- 1) .pro ----------
cat > "$TARGET_DIR/$PLUGIN_NAME.pro" <<PRO
# 插件 $PLUGIN_NAME
PLUGIN_NAME = $PLUGIN_NAME

include(../../plugins/plugin.pri)

HEADERS += \\
    $GLOBAL_HEADER \\
    $PLUGIN_FILE.h \\
    $WIDGET_FILE.h

SOURCES += \\
    $PLUGIN_FILE.cpp \\
    $WIDGET_FILE.cpp

FORMS += \\
    $WIDGET_FILE.ui
PRO

# ---------- 2) 导出宏 ----------
cat > "$TARGET_DIR/$GLOBAL_HEADER" <<HDR
#ifndef $GLOBAL_GUARD
#define $GLOBAL_GUARD

#include <QtCore/qglobal.h>

#if defined(${MACRO}_LIBRARY)
#  define ${MACRO}_EXPORT Q_DECL_EXPORT
#else
#  define ${MACRO}_EXPORT Q_DECL_IMPORT
#endif

#endif // $GLOBAL_GUARD
HDR

# ---------- 3) 元数据 json ----------
cat > "$TARGET_DIR/$JSON_FILE" <<JSON
{
    "Keys": [ "$PLUGIN_NAME" ]
}
JSON

# ---------- 4) 插件工厂类 头 ----------
cat > "$TARGET_DIR/$PLUGIN_FILE.h" <<HDR
#ifndef $PLUGIN_GUARD
#define $PLUGIN_GUARD

#include "pluginbase.h"
#include "plugincontext.h"

#include <QObject>

// 插件 $PLUGIN_NAME 的工厂类（Qt 插件入口）。
// 继承 PluginBase 以获得 IPlugin 的通用默认实现；只需实现自己关心的部分。
class ${PLUGIN_CLASS} : public PluginBase
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID IPlugin_iid FILE "$JSON_FILE")
    Q_INTERFACES(IPlugin)

public:
    QString  name()    const override { return QStringLiteral("$DISPLAY_NAME"); }
    QString  version() const override { return QStringLiteral("1.0.0"); }

    QWidget *createWidget(QWidget *parent) override;
};

#endif // $PLUGIN_GUARD
HDR

# ---------- 5) 插件工厂类 实现 ----------
cat > "$TARGET_DIR/$PLUGIN_FILE.cpp" <<CPP
#include "$PLUGIN_FILE.h"
#include "$WIDGET_FILE.h"

QWidget *${PLUGIN_CLASS}::createWidget(QWidget *parent)
{
    // 返回插件自带的 UI 控件，由宿主作为子页面嵌入
    return new ${WIDGET_CLASS}(m_ctx, parent);
}
CPP

# ---------- 6) UI 头 ----------
cat > "$TARGET_DIR/$WIDGET_FILE.h" <<HDR
#ifndef $WIDGET_GUARD
#define $WIDGET_GUARD

#include "plugincontext.h"

#include <QWidget>

namespace Ui { class ${WIDGET_CLASS}; }

// 插件 $PLUGIN_NAME 的界面：独立设计，最终被宿主嵌入为主程序的一个子页面。
class ${WIDGET_CLASS} : public QWidget
{
    Q_OBJECT

public:
    explicit ${WIDGET_CLASS}(PluginContext *ctx, QWidget *parent = nullptr);
    ~${WIDGET_CLASS}();

private slots:
    void on_sendButton_clicked();

private:
    Ui::${WIDGET_CLASS} *ui;
    PluginContext *m_ctx;
};

#endif // $WIDGET_GUARD
HDR

# ---------- 7) UI 实现 ----------
cat > "$TARGET_DIR/$WIDGET_FILE.cpp" <<CPP
#include "$WIDGET_FILE.h"
#include "ui_$WIDGET_FILE.h"

#include "typedbus.h"
#include "messages.h"

${WIDGET_CLASS}::${WIDGET_CLASS}(PluginContext *ctx, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::${WIDGET_CLASS})
    , m_ctx(ctx)
{
    ui->setupUi(this);
}

${WIDGET_CLASS}::~${WIDGET_CLASS}()
{
    delete ui;
}

void ${WIDGET_CLASS}::on_sendButton_clicked()
{
    // 示例：通过强类型信号总线发布一条 ValueChanged 消息
    if (m_ctx->bus()) {
        msgs::ValueChanged msg;
        msg.value  = ui->spinBox->value();
        msg.source = QStringLiteral("$PLUGIN_NAME");
        m_ctx->bus()->publish<msgs::ValueChanged>(msg);
    }
}
CPP

# ---------- 8) UI 界面 ----------
cat > "$TARGET_DIR/$WIDGET_FILE.ui" <<UI
<?xml version="1.0" encoding="UTF-8"?>
<ui version="4.0">
 <class>${WIDGET_CLASS}</class>
 <widget class="QWidget" name="${WIDGET_CLASS}">
  <property name="windowTitle">
   <string>$DISPLAY_NAME</string>
  </property>
  <layout class="QVBoxLayout" name="verticalLayout">
   <item>
    <widget class="QLabel" name="label">
     <property name="text">
      <string>我是插件 $DISPLAY_NAME 的界面。点击按钮通过强类型总线广播 ValueChanged。</string>
     </property>
     <property name="wordWrap">
      <bool>true</bool>
     </property>
    </widget>
   </item>
   <item>
    <layout class="QHBoxLayout" name="horizontalLayout">
     <item>
      <widget class="QLabel" name="labelValue">
       <property name="text">
        <string>数值：</string>
       </property>
      </widget>
     </item>
     <item>
      <widget class="QSpinBox" name="spinBox">
       <property name="maximum">
        <number>9999</number>
       </property>
       <property name="value">
        <number>1</number>
       </property>
      </widget>
     </item>
     <item>
      <widget class="QPushButton" name="sendButton">
       <property name="text">
        <string>发送广播</string>
       </property>
      </widget>
     </item>
    </layout>
   </item>
   <item>
    <spacer name="verticalSpacer">
     <property name="orientation">
      <enum>Qt::Vertical</enum>
     </property>
     <property name="sizeHint" stdset="0">
      <size>
       <width>20</width>
       <height>40</height>
      </size>
     </property>
    </spacer>
   </item>
  </layout>
 </widget>
 <resources/>
 <connections/>
</ui>
UI

# ---------- 9) 自动注册到 plugins.pro 的插件清单 ----------
MARK_BEGIN="# === BEGIN plugin list"
MARK_END="# === END plugin list"

if ! grep -q "$MARK_BEGIN" "$PLUGINS_PRO" || ! grep -q "$MARK_END" "$PLUGINS_PRO"; then
    echo "警告: 未在 plugins.pro 找到管理标记，请手动将 $PLUGIN_NAME 加入 SUBDIRS" >&2
elif grep -qE "^SUBDIRS \+= $PLUGIN_NAME\$" "$PLUGINS_PRO"; then
    echo "==> $PLUGIN_NAME 已在 plugins.pro 中，跳过注册"
else
    # 在 END 标记行之前插入一行 "SUBDIRS += <name>"（用包含匹配，容忍行尾差异）
    awk -v name="$PLUGIN_NAME" -v endm="$MARK_END" '
        index($0, endm) && !ins {
            print "SUBDIRS += " name
            ins=1
        }
        { print }
    ' "$PLUGINS_PRO" > "$PLUGINS_PRO.tmp" && mv "$PLUGINS_PRO.tmp" "$PLUGINS_PRO"
    echo "==> 已注册到 plugins/plugins.pro"
fi

echo ""
echo "完成！接下来："
echo "  cd $ROOT_DIR"
echo "  qmake QProject.pro && make -j\$(nproc)"
echo "  ./build/app/qproject_app"


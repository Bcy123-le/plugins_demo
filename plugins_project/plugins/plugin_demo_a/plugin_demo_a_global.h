#ifndef PLUGIN_DEMO_A_GLOBAL_H
#define PLUGIN_DEMO_A_GLOBAL_H

#include <QtCore/qglobal.h>

// 本插件导出宏（按需使用；多数插件符号由 Qt 插件机制在内部使用）
#if defined(PLUGIN_DEMO_A_LIBRARY)
#  define PLUGIN_DEMO_A_EXPORT Q_DECL_EXPORT
#else
#  define PLUGIN_DEMO_A_EXPORT Q_DECL_IMPORT
#endif

#endif // PLUGIN_DEMO_A_GLOBAL_H

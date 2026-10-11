#ifndef COMMON_GLOBAL_H
#define COMMON_GLOBAL_H

#include <QtCore/qglobal.h>

// common 共享库的导出/导入宏：
//  - 编译 common 本身时定义 COMMON_LIBRARY -> 导出符号
//  - 被 app / 插件使用时 -> 导入符号
#if defined(COMMON_LIBRARY)
#  define COMMON_EXPORT Q_DECL_EXPORT
#else
#  define COMMON_EXPORT Q_DECL_IMPORT
#endif

#endif // COMMON_GLOBAL_H

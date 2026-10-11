#ifndef PLUGIN_DEMO_B_GLOBAL_H
#define PLUGIN_DEMO_B_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PLUGIN_DEMO_B_LIBRARY)
#  define PLUGIN_DEMO_B_EXPORT Q_DECL_EXPORT
#else
#  define PLUGIN_DEMO_B_EXPORT Q_DECL_IMPORT
#endif

#endif // PLUGIN_DEMO_B_GLOBAL_H

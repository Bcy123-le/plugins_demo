# 3rdparty —— 第三方公共库

本文件夹用于存放被 `common` 通用组件及各插件子项目**共同依赖**的第三方库
（二进制文件、头文件、源码或预编译包）。

## 推荐目录结构

```
3rdparty/
├── include/        # 第三方库头文件（已在 common.pri 中加入 INCLUDEPATH）
├── lib/            # 第三方库的 .so / .a
└── <some_lib>/     # 或按库分子目录，附带其自带说明
```

## 如何接入（以某个第三方库 `foo` 为例）

1. 把头文件放到 `3rdparty/include/`，把库文件放到 `3rdparty/lib/`。
2. 由于 `common.pri` 已经把 `3rdparty/include` 加入 `INCLUDEPATH`，
   源码中可直接 `#include <foo/foo.h>`。
3. 在需要链接该库的子项目 `.pro` 中追加链接（示例）：
   ```qmake
   LIBS += -L$$QP_3RD/lib -lfoo
   ```
   其中 `$$QP_3RD` 已在 `common.pri` 中定义为 `3rdparty` 目录。

## 说明

- 本示例工程未实际集成任何第三方库，`3rdparty/` 目前仅为占位与规范说明。
- 将第三方库集中放置，便于统一版本管理、避免各插件重复拷贝、便于后续按需裁剪。

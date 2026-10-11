# QProject —— Qt C++ 插件化软件架构

一个使用 **qmake（subdirs）+ Qt 5.15** 实现的多插件软件架构示例。
主工程分层编排多个子项目：`common`（通用组件共享库）、`app`（主程序）、
以及由 `plugins` 中间层统一编排的若干**相互独立**的插件子项目；
插件可带 UI 并作为主程序的**子页面**显示，
插件之间通过**强类型信号总线（TypedBus）**与**服务注册表（ServiceRegistry）**解耦通信。

## 目录结构

采用**两层 subdirs 分层编排**：主工程编排到 `plugins` 中间层，
`plugins/plugins.pro` 再向下编排各独立插件。

```
QProject/
├── QProject.pro            # 主工程：TEMPLATE=subdirs，编排 common / app / plugins
├── common.pri              # 公共构建配置（路径定位、链接 common、RPATH、公共宏）
├── 3rdparty/               # 第三方公共库（头文件/库/说明），被 common 与插件共用
│   └── README.md
├── common/                 # 通用组件 -> 编译为共享库 libcommon.so
│   ├── common.pro
│   ├── common_global.h     #   导出/导入宏
│   ├── iplugin.h           #   插件接口 IPlugin（含 createWidget UI 入口）
│   ├── pluginbase.h        #   插件基类 PluginBase（IPlugin 通用默认实现）
│   ├── plugincontext.h/.cpp#   上下文：向插件提供服务
│   ├── typedbus.h/.cpp     #   强类型信号总线（on<T> 订阅 / publish<T> 发布）
│   ├── messages.h          #   插件间共享的强类型消息契约
│   └── serviceregistry.h/.cpp  # 服务注册表（服务定位）
├── app/                    # 主程序 host
│   ├── app.pro
│   ├── main.cpp
│   ├── mainwindow.h/.cpp/.ui   # 宿主主窗口（QTabWidget 作为插件子页面容器）
│   └── pluginmanager.h/.cpp    # 扫描/加载/管理插件生命周期
├── plugins/                # 插件总目录
│   ├── plugins.pro         #   插件中间编排层：subdirs 向下编排各插件（纯编排，不编译）
│   ├── plugin.pri          #   插件构建模板：每个插件 .pro 仅设置 PLUGIN_NAME 后 include
│   ├── plugin_demo_a/      #   示范插件A：带UI、注册服务、publish 强类型消息
│   ├── plugin_demo_b/      #   示范插件B：on 订阅强类型消息、调用A的服务
│   └── plugin_hello/       #   由脚手架脚本生成的示范插件（继承 PluginBase）
├── tools/                  # 开发辅助脚本
│   └── newplugin.sh        #   脚手架：一条命令生成新插件并自动注册
└── build/                  # 构建输出（自动生成）
    ├── common/libcommon.so*
    ├── app/qproject_app
    └── plugins/*.so
```

## 架构要点

- **分层 subdirs 编排**：
  - `QProject.pro`（`TEMPLATE=subdirs` + `CONFIG+=ordered`）编排 `common`、`app`、`plugins`；
  - `plugins/plugins.pro`（同样是 subdirs + ordered）作为**中间编排层**，向下编排各独立插件。
  - `ordered` 保证 `common` 先于 `app` 与各插件构建（后三者链接 `libcommon.so`）。
  - `plugins/plugins.pro` 是“纯编排层”：自身不编译代码、不链接库，
    每个插件通过 `include(../../common.pri)` 自行链接 `libcommon.so`。
    这种分层让“新增插件”只需改 `plugins/plugins.pro`，主工程保持不变。
- **共享库而非静态库**：`common` 必须构建为 `shared`。这样 `TypedBus` /
  `ServiceRegistry` 的单例在整个程序内**全局唯一**，插件之间才能互相通信。
  若改为静态库，单例会被各插件各自拷贝，通信将失效。
- **解耦通信**：插件互不直接依赖，统一通过宿主注入的 `PluginContext` 通信：
  - **强类型信号总线 `TypedBus`（首选）**：`on<T>(receiver, fn)` 订阅、
    `publish<T>(msg)` 发布。类型化、编译期检查、无字符串 topic、无 QVariantMap
    装箱拷贝，同线程高效；订阅者随 QObject 销毁自动退订。
  - **服务注册表 `ServiceRegistry`**：按名查找 + 元对象系统调用，
    适合需要**同步返回值**的强类型请求/调用。
  - 两者分工：`TypedBus` 管“事件流/通知”，`ServiceRegistry` 管“请求/调用”。
- **共享消息契约 `messages.h`**：跨插件的强类型消息结构体统一定义在 `common` 内，
  各插件仅依赖 `common` 而不互相依赖，从而保持解耦。
- **插件 UI 作为子页面**：宿主用 `QTabWidget` 做容器，调用插件的
  `createWidget()` 拿到控件后 `addTab()` 嵌入；纯后台插件可返回 `nullptr`。

## 构建与运行

```bash
cd /home/bcy/istudy/customwidget/QProject
qmake QProject.pro       # 生成构建文件
make -j$(nproc)          # 编译 common -> app -> 各插件
./build/app/qproject_app # 运行（图形界面）
```

> 运行环境：本机 `qmake` 指向 Qt 5.15.2 / gcc_64。
> 若需可移植发布，可在 `common.pri` 中为 RPATH 追加相对路径 `$ORIGIN/../common`，
> 并将 `build/` 下的 `libcommon.so*`、可执行文件与 `plugins/*.so` 一并打包。

## 如何新增一个插件（一条命令）

本工程提供脚手架脚本，**一条命令**即可生成新插件全套文件，并自动注册到构建清单：

```bash
cd /home/bcy/istudy/customwidget/QProject
./tools/newplugin.sh plugin_xxx "显示名"     # 例如：./tools/newplugin.sh plugin_hello "Hello"
qmake QProject.pro && make -j$(nproc)        # 重新构建（新增子项目需重新 qmake）
./build/app/qproject_app                     # 运行，新插件页面自动加载
```

脚本会自动完成：
1. 在 `plugins/plugin_xxx/` 生成 `.pro`、`_global.h`、`.json`、工厂类、UI 页面；
2. 工厂类已**继承 `PluginBase`**（免写 `initialize/shutdown/category` 等样板）；
3. 已内置一次 **TypedBus 通信示例**（`publish<msgs::ValueChanged>`）；
4. 自动把插件名追加到 `plugins/plugins.pro` 的清单（无需手动改任何文件）。

> 脚本具备防重入保护：同名插件重复执行会安全提示并退出，不会重复注册。

### 手动新增（不依赖脚本）

若需手动创建，按以下要点即可（细节可参考 `plugins/plugin_hello/`）：

1. 在 `plugins/` 下新建目录，例如 `plugins/plugin_xxx/`。
2. 新建 `plugin_xxx.pro`：
   ```qmake
   PLUGIN_NAME = plugin_xxx
   include(../../plugins/plugin.pri)
   HEADERS += plugin_xxx_global.h pluginxxx.h pluginxxxwidget.h
   SOURCES += pluginxxx.cpp pluginxxxwidget.cpp
   FORMS   += pluginxxxwidget.ui
   ```
3. 新建 `plugin_xxx.json`：`{ "Keys": [ "plugin_xxx" ] }`。
4. 实现工厂类：继承 `PluginBase`（而非直接 `QObject+IPlugin`），加上
   `Q_PLUGIN_METADATA(IID IPlugin_iid FILE "plugin_xxx.json")` 与
   `Q_INTERFACES(IPlugin)`；只需实现 `name()/version()/createWidget()` 等关心的部分。
5. 在 **`plugins/plugins.pro`** 的 `BEGIN/END plugin list` 之间加入一行
   `SUBDIRS += plugin_xxx`（主工程 `QProject.pro` 无需改动）。
6. 重新 `qmake && make`。运行主程序即可看到新插件页面被自动加载。

## 通信示例（本工程已内置）

- **强类型信号总线（TypedBus）**：插件 A 界面点“发送广播” →
  `bus()->publish<msgs::ValueChanged>({value, "A"})` → 插件 B 界面
  `bus()->on<msgs::ValueChanged>(this, ...)` 实时显示“收到来自 A 的广播: value = …”。
- **服务定位（ServiceRegistry）**：插件 B 界面点“调用插件 A 的服务 compute()” →
  通过服务注册表找到 A 并用 `QMetaObject::invokeMethod` 调用其
  `Q_INVOKABLE int compute(int,int)`，结果显示在 B 的界面。

### TypedBus 用法速查

```cpp
// 1) 在 common/messages.h 定义共享消息类型
struct MyEvent { int id; QString text; };

// 2) 发布（任意插件或主程序）
ctx->bus()->publish<MyEvent>({1, "hello"});

// 3) 订阅（receiver 传 QObject*，其销毁时自动退订）
ctx->bus()->on<MyEvent>(this, [](const MyEvent &e){
    qDebug() << e.id << e.text;
});
```

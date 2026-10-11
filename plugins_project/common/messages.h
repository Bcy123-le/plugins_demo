#ifndef MESSAGES_H
#define MESSAGES_H

#include <QString>

// 插件间共享的消息类型契约（强类型）。
//
// 说明：TypedBus 采用“强类型消息”，发送方与接收方需引用同一消息结构体。
// 为避免插件之间互相依赖，所有跨插件消息类型统一定义在此（common 内），
// 各插件仅依赖 common，而不依赖彼此。新增消息类型在此添加结构体即可。
namespace msgs {

// 示例：数据变更通知。由插件 A 发布，插件 B / 主程序订阅。
struct ValueChanged
{
    int     value  = 0;
    QString source;   // 来源标识，便于区分发布方
};

} // namespace msgs

#endif // MESSAGES_H

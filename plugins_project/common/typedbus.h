#ifndef TYPEDBUS_H
#define TYPEDBUS_H

#include "common_global.h"

#include <QObject>
#include <QMutex>
#include <QSet>

#include <algorithm>
#include <functional>
#include <typeindex>
#include <unordered_map>
#include <vector>

// 强类型信号总线：类型化的发布/订阅。
//
// 相比“字符串 topic + QVariantMap”的通用总线，TypedBus 具备：
//   - 用法极简：一行 bus()->emit<T>(msg) 发布、一行 bus()->on<T>(recv, fn) 订阅；
//   - 强类型：消息用普通结构体，编译期检查，字段可随意扩展；
//   - 高效：同线程直接回调，无字符串比较、无 QVariant 装箱拷贝。
//
// 该单例位于 libcommon.so 内，主程序与所有插件共享同一实例，
// 因此“主↔插件、插件↔插件”天然对等，且跨插件通信成立。
class COMMON_EXPORT TypedBus : public QObject
{
    Q_OBJECT
public:
    static TypedBus *instance();

    // 订阅：Msg 为消息结构体类型；fn 形如 void(const Msg&)。
    // receiver 用于生命周期绑定：receiver 销毁时自动退订。
    template<typename Msg, typename Fn>
    void on(QObject *receiver, Fn &&fn)
    {
        QMutexLocker lock(&m_mutex);
        Sub sub;
        sub.receiver = receiver;
        sub.call = [fn](const void *p) { fn(*static_cast<const Msg *>(p)); };
        m_subs[std::type_index(typeid(Msg))].push_back(std::move(sub));
        watch(receiver);
    }

    // 发布：直接派发强类型消息。（注意：不能用 emit，它是 Qt 关键字。）
    template<typename Msg>
    void publish(const Msg &msg)
    {
        std::vector<Sub> snapshot;
        {
            QMutexLocker lock(&m_mutex);
            auto it = m_subs.find(std::type_index(typeid(Msg)));
            if (it == m_subs.end())
                return;
            snapshot = it->second;   // 先快照、出锁后再回调，避免回调内再 on/publish 死锁
        }
        for (const Sub &s : snapshot)
            if (s.call)
                s.call(&msg);
    }

private:
    struct Sub {
        QObject *receiver = nullptr;
        std::function<void(const void *)> call;
    };

    explicit TypedBus(QObject *parent = nullptr) : QObject(parent) {}

    void watch(QObject *receiver);        // 记录并连接 destroyed，自动退订（调用方已持锁）
    void removeSubscriber(QObject *r);    // 从所有类型表移除该 receiver

    QMutex m_mutex;
    std::unordered_map<std::type_index, std::vector<Sub>> m_subs;
    QSet<QObject *> m_watched;            // 已绑定 destroyed 的 receiver（避免重复连接）
};

#endif // TYPEDBUS_H

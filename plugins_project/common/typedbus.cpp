#include "typedbus.h"

// 单例：静态局部量位于 libcommon.so 内，全程序唯一。
// 主程序与所有插件链接的是同一个 libcommon.so，故拿到的是同一实例，
// 这是跨插件强类型通信成立的前提。
TypedBus *TypedBus::instance()
{
    static TypedBus bus;
    return &bus;
}

void TypedBus::watch(QObject *receiver)
{
    if (!receiver || m_watched.contains(receiver))
        return;
    m_watched.insert(receiver);

    // receiver 销毁后自动退订，避免悬垂回调。
    // 使用 QueuedConnection：确保在对象析构完成后、于事件循环中安全清理。
    connect(receiver, &QObject::destroyed, this, [this](QObject *obj) {
        QMutexLocker lock(&m_mutex);
        removeSubscriber(obj);
        m_watched.remove(obj);
    }, Qt::QueuedConnection);
}

void TypedBus::removeSubscriber(QObject *r)
{
    for (auto it = m_subs.begin(); it != m_subs.end(); ) {
        std::vector<Sub> &vec = it->second;
        vec.erase(std::remove_if(vec.begin(), vec.end(),
                                 [r](const Sub &s) { return s.receiver == r; }),
                  vec.end());
        it = vec.empty() ? m_subs.erase(it) : std::next(it);
    }
}

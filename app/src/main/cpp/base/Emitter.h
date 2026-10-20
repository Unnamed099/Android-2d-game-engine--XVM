#ifndef CPP_BASE_EMITTER_H
#define CPP_BASE_EMITTER_H

#include <map>
#include <string>
#include <vector>

class Emitter {
public:
    class Listener {
    public:
        virtual ~Listener() {}
        virtual void onEvent(const std::vector<void*>& args) = 0;
    };

private:
    std::map<std::string, std::vector<Listener*>> listeners;
    // 由 once() 创建、由本 Emitter 负责释放的 wrapper
    std::vector<Listener*> ownedWrappers;
    // 延迟释放队列（等所有 emit 退出后再真正 delete）
    std::vector<Listener*> pendingDeletes;
    // 当前 emit 的嵌套深度
    int emitDepth;

    void removeOnce(Listener* wrapper);
    void scheduleDelete(Listener* wrapper);
    void flushPendingDeletes();
    void destroyWrappersIn(const std::vector<Listener*>& list);

public:
    Emitter();
    ~Emitter();

    void on(const std::string& event, Listener* listener);

    void once(const std::string& event0, Listener* listener);

    void off(const std::string& event, Listener* listener);

    virtual void off(const std::string& event);

    void emit(const std::string& event, const std::vector<void*>& args);

    void clear();

    int listenerCount(const std::string& event);

    bool hasListeners(const std::string& event);
};

#endif
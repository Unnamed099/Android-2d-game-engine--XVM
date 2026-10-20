#ifndef CPP_BASE_WHILEFRAMETASK_H
#define CPP_BASE_WHILEFRAMETASK_H

#include <functional>
#include <map>
#include <string>

class WhileFrameTask {
public:
    using OnExecute  = std::function<bool(int, std::map<std::string, void*>&)>;
    using OnComplete = std::function<void(std::map<std::string, void*>&)>;
    using OnProgress = std::function<void(int, int, float, std::map<std::string, void*>&)>;

    explicit WhileFrameTask(int totalSteps);

    WhileFrameTask& put(const std::string& key, void* value);
    WhileFrameTask& putAll(const std::map<std::string, void*>& data);

    void set(const std::string& key, void* value);

    template<typename T>
    T* get(const std::string& key) {
        auto it = dataMap.find(key);
        if (it != dataMap.end()) {
            return static_cast<T*>(it->second);
        }
        return nullptr;
    }

    WhileFrameTask& onExecute(OnExecute callback);
    WhileFrameTask& onComplete(OnComplete callback);
    WhileFrameTask& onProgress(OnProgress callback);

    void start();

    /// 分帧执行：最多执行 maxSteps 步。
    /// 返回 true 表示任务已完成（正常完成或回调要求结束），false 表示还有剩余步骤。
    bool doFrame(int maxSteps);

    /// 单步执行。返回 true 表示成功推进一步；false 表示无法推进（未运行/暂停/完成/回调终止）。
    bool stepOnce();

    /// 批量执行，返回实际完成的步数。
    int doSteps(int maxSteps);

    void pause();
    void resume();
    void cancel();

    // ---- 状态查询 ----
    bool  isRunning()   const;
    bool  isCompleted() const;
    float getProgress() const;
    int   getCurrentStep() const;

private:
    void complete();

    std::map<std::string, void*> dataMap;
    int  currentStep = 0;
    int  totalSteps  = 0;
    bool running     = false;
    bool paused      = false;
    bool completed   = false;

    OnExecute  onExecute_;
    OnComplete onComplete_;
    OnProgress onProgress_;
};

#endif // CPP_BASE_WHILEFRAMETASK_H
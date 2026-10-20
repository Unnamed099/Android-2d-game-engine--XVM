#ifndef POOL_H
#define POOL_H

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <stdexcept>
#include <memory>

/**
 * 简易的对象池实现
 * 用于对象的存贮和重复使用
 * 可以有效减少对象创建开销和避免频繁的垃圾回收
 * 提高游戏性能
 * C++版本
 */
class Pool {
private:
    /**
     * 用于标识对象池的私有内部类
     */
    class PoolKey {
    public:
        std::string name;

        PoolKey(const std::string& name) : name(name) {}

        bool operator==(const PoolKey& other) const {
            return name == other.name;
        }
    };

    /**
     * PoolKey的哈希函数
     */
    struct PoolKeyHash {
        size_t operator()(const PoolKey& key) const {
            return std::hash<std::string>()(key.name);
        }
    };

    // 对象池字典
    std::unordered_map<PoolKey, std::vector<void*>, PoolKeyHash> poolDic;

    /**
     * 根据对象标识符获取对应的对象池（不存在则创建）
     * 对应 Java 的 private List<Object> getPoolBySign(String name)
     * @param name 对象标识符
     * @return 对象池列表引用
     */
    std::vector<void*>& getPoolBySign(const std::string& name);

public:
    /**
     * 对象工厂接口
     */
    using ObjectFactory = std::function<void*()>;

    /**
     * 默认构造函数
     */
    Pool();

    /**
     * 析构函数
     */
    ~Pool();

    /**
     * 根据对象标识符和工厂，从对象池获取实例
     * 对象池为空调用工厂创建新的实例，否则从对象池中取
     * @param name 对象标识符
     * @param factory 工厂函数
     * @return 对象实例
     */
    void* getItemByFactory(const std::string& name, ObjectFactory factory);

    /**
     * 获取对象（如果存在）
     * @param name 对象标识符
     * @return 对象实例（不存在返回nullptr）
     */
    void* getItemIfExists(const std::string& name);

    /**
     * 将对象回收到对象池
     * @param name 对象标识符
     * @param instance 要回收的对象实例
     */
    void recover(const std::string& name, void* instance);

    /**
     * 清空指定类型的对象池
     * @param name 对象标识符
     */
    void clearPool(const std::string& name);

    /**
     * 清空所有对象池
     */
    void clearAll();

    /**
     * 获取指定对象池的大小
     * @param name 对象标识符
     * @return 对象池中对象的数量
     */
    int getPoolSize(const std::string& name);

    /**
     * 获取对象池总数（不同类型的对象池数量）
     * @return 对象池数量
     */
    int getPoolCount();

    /**
     * 预创建对象并放入对象池（使用工厂）
     * @param name 对象标识符
     * @param factory 对象工厂
     * @param count 预创建数量
     */
    void preCreateObjects(const std::string& name, ObjectFactory factory, int count);

    /**
     * 安全回收对象，避免重复回收
     * @param name 对象标识符
     * @param instance 要回收的对象实例
     * @return 是否成功回收
     */
    bool safeRecover(const std::string& name, void* instance);

    /**
     * 模板版本：根据类型获取对象（需要对象有默认构造函数）
     * 对应Java的getItemByClass
     * @param name 对象标识符
     * @return 对象实例指针
     */
    template <typename T>
    T* getItemByClass(const std::string& name) {
        ObjectFactory factory = []() -> void* {
            return new T();
        };
        return static_cast<T*>(getItemByFactory(name, factory));
    }

    /**
     * 模板版本：预创建对象
     * 对应Java的preCreateObjects(String, Class<T>, int)
     */
    template <typename T>
    void preCreateObjects(const std::string& name, int count) {
        ObjectFactory factory = []() -> void* {
            return new T();
        };
        preCreateObjects(name, factory, count);
    }
};

#endif // POOL_H
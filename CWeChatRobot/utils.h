#pragma once
#include <set>
#include <chrono>

#define CHRONO std::chrono
typedef unsigned long long ull;

/*
 * 一个简单的带过期时间检查和清理的集合，interval为毫秒
 * ExpireSet es(1000); // 过期时间1秒
 * es.CheckIfDuplicatedAndAdd(1); true, 未重复
 * es.CheckIfDuplicatedAndAdd(1); false, 重复
 * After 1s;
 * es.CheckIfDuplicatedAndAdd(2); 添加<2>并清理过期的元素<1>
 */
class ExpireSet
{
public:
    ull interval; // 毫秒
    ExpireSet(ull interval)
    {
        this->interval = interval;
        this->expires_at = 0;
    }
    bool CheckIfDuplicatedAndAdd(ull id)
    {
        bool ok = true;
        if (ids.count(id) != 0)
        {
            ok = false;
        }
        Add(id);
        return ok;
    }

private:
    std::set<ull> ids;
    ull expires_at;

    void Add(ull id)
    {
        // 毫秒
        auto now_ts = CHRONO::time_point_cast<CHRONO::milliseconds>(CHRONO::system_clock::now()).time_since_epoch().count();
#pragma warning(disable : 4018)
        if (expires_at < now_ts)
        {
            ids.clear();
            expires_at = now_ts + interval;
        }
#pragma warning(default : 4018)
        ids.insert(id);
    }
};

#include "safe_exit.hpp"

#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <csignal>
#include <mutex>
#include <vector>

inline namespace Common
{
// 安全な終了処理を提供する
namespace ThreadRoom
{

std::atomic_bool to_exit = false;
std::vector<std::thread::id> id_list;
std::mutex mutex_id_list;
std::condition_variable cond_id_empty;

void sigint_handler(int)
{
    to_exit = true;

    std::unique_lock<std::mutex> lock{mutex_id_list};
    cond_id_empty.wait(lock, [&id_list] {
        return id_list.empty();
    });

    std::exit(EXIT_SUCCESS);
}

void setExitHandler()
{
    std::signal(SIGINT, sigint_handler);
}

void enter()
{
    std::thread::id id = std::this_thread::get_id();
    std::lock_guard<std::mutex>
        lock{mutex_id_list};
    if (std::find(id_list.begin(), id_list.end(), id) == id_list.end()) {
        id_list.push_back(id);
    }
}

bool toExit()
{
    if (to_exit) {
        throw thread_abort{};
    }
    return to_exit;
}

std::thread::id exit()
{
    std::thread::id id = std::this_thread::get_id();
    std::lock_guard<std::mutex> lock{mutex_id_list};
    auto iter = std::find(id_list.begin(), id_list.end(), id);
    if (iter != id_list.end()) {
        id_list.erase(iter);
    }
    cond_id_empty.notify_one();
    return id;
}

}  // namespace ThreadRoom
}  // namespace Common

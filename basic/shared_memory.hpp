#pragma once
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/shared_memory_object.hpp>

#include <string>
#include <type_traits>

inline namespace Basic
{

namespace SharedMemory
{

// usingしたいのでimplに閉じ込める
namespace Impl
{
using namespace boost::interprocess;

struct shm_remover {
    std::string name;

    shm_remover(std::string name) : name(name)
    {
        shared_memory_object::remove(name.c_str());
    }
    ~shm_remover()
    {
        shared_memory_object::remove(name.c_str());
    }
};

template <class T, bool create>
class ShmImpl
{
};

template <class T>
class ShmImpl<T, false>
{
    using ptr_t = std::add_pointer_t<T>;

    shm_remover remover;
    shared_memory_object shm_obj;
    mapped_region region;
    ptr_t ptr;

public:
    ShmImpl(std::string name)
        : remover{name},
          shm_obj{open_only, name.c_str(), read_only},
          region{shm_obj, read_only},
          ptr{static_cast<ptr_t>(region.get_address())}
    {
    }

    ~ShmImpl()
    {
    }

    virtual ptr_t operator()() { return ptr; }
};

template <class T>
class ShmImpl<T, true>
{
    using ptr_t = std::add_pointer_t<T>;

    shm_remover remover;
    shared_memory_object shm_obj;
    mapped_region region;
    ptr_t ptr;

public:
    ShmImpl(std::string name)
        : remover{name},
          shm_obj{create_only, name.c_str(), read_write},
          region{[this]() {
              this->shm_obj.truncate(sizeof(T));
              return mapped_region{this->shm_obj, read_write};
          }()},
          ptr(static_cast<ptr_t>(region.get_address()))
    {
        std::memset(region.get_address(), 0, region.get_size());
    }

    ~ShmImpl()
    {
    }

    virtual ptr_t operator()() { return ptr; }
};
}  // namespace Impl

template <class T>
using ReadOnly = Impl::ShmImpl<T, false>;
template <class T>
using ReadWrite = Impl::ShmImpl<T, true>;

}  // namespace SharedMemory

}  // namespace Basic

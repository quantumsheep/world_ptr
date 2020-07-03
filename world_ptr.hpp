#include <cstddef>
#include <map>
#include <memory>
#include <type_traits>

static std::map<void *, size_t> pointers;

template <typename T>
class world_ptr
{
private:
    T *ptr = nullptr;

public:
    world_ptr(T *ptr_) : ptr(ptr_)
    {
        auto it = pointers.find(this->ptr);

        if (it != pointers.end())
        {
            it->second++;
        }
        else
        {
            pointers.insert(std::make_pair<void *, size_t>(this->ptr, 1UL));
        }
    }

    world_ptr(const world_ptr &wptr) : ptr(wptr.get())
    {
        auto &size = pointers.at(this->ptr);
        size++;
    }

    template <typename... Args>
    static world_ptr<T> make(Args &&... args)
    {
        static_assert(std::is_constructible_v<Args...>, "Can't construct object in world_ptr<T>::make");
        return world_ptr<T>(new T(args...));
    }

    T *get() const
    {
        return this->ptr;
    }

    world_ptr &reset(const world_ptr &wptr)
    {
        auto &size = pointers.at(this->ptr);
        size--;

        this->ptr = wptr->ptr;
        auto &size_new = pointers.at(this->ptr);
        size++;
    }

    ~world_ptr()
    {
        auto it = pointers.find(this->ptr);

        if (it->second == 1UL)
        {
            delete reinterpret_cast<T *>(it->first);
            pointers.erase(this->ptr);
        }
        else
        {
            it->second--;
        }
    }

    inline world_ptr &operator=(const world_ptr &wptr)
    {
        return this->reset(wptr);
    }

    inline T *operator->() const
    {
        return this->ptr;
    }

    inline T &operator*() const
    {
        return *this->ptr;
    }
};

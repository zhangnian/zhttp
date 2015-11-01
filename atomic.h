#ifndef ATOMIC_H
#define ATOMIC_H

#include <stdint.h>


namespace zhttp
{
    template<typename T>
    class AtomicIntegerT
    {
    public:
        AtomicIntegerT() : value_(0)
        {

        }

        T get()
        {
            return __sync_val_compare_and_swap(&value_, 0, 0);
        }

        T getAndSet(T newValue)
        {
            return __sync_lock_test_and_set(&value_, newValue);
        }

        T getAndAdd(T x)
        {
            return __sync_fetch_and_add(&value_, x);
        }

        T addAndGet(T x)
        {
            return getAndAdd(x) + x;
        }

        void increment()
        {
            addAndGet(1);
        }

        void decrement()
        {
            addAndGet(-1);
        }

    private:
        volatile T value_;
    };
}


typedef zhttp::AtomicIntegerT<int32_t> AtomicInt32;
typedef zhttp::AtomicIntegerT<int64_t> AtomicInt64;


#endif

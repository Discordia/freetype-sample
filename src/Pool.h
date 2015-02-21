#pragma once

#include <Log.h>
#include <cassert>
#include <string.h>

//#define LOG_TAG "Cache"

template <class T>
class Pool
{
public:
    Pool()
    {
        size = 0;
        pool = NULL;
        used = 0;
    }

    ~Pool()
    {
        release();
    }

    void release()
    {
        if (pool != NULL)
        {
            free(pool);
            size = 0;
            pool = NULL;
            used = 0;
        }
    }

    void init(int size)
    {
        if (this->size < size)
        {
            T* newPool = (T*) malloc(size * sizeof(T));

            if (newPool == NULL)
            {
  //              LOGE("Not enough memory ");
            }

            if (used > 0)
            {
                memcpy(newPool, pool, used * sizeof(T));
            }

            release();
            pool = newPool;
            this->size = size;
        }
    }

    void destroy()
    {
        used = 0;
    }

    T* allocate()
    {
        if (used >= this->size)
        {
    //        LOGW("Alocating when no memory left in pool, returning NULL");
            assert(false);

            return NULL;
        }

        return &pool[used++];
    }

    void deallocate()
    {
        used--;
    }

    T* getCache()
    {
        return pool;
    }

    T& get(int index)
    {
        return pool[index];
    }

    T* getPtr(int index)
    {
        return &pool[index];
    }

private:
    T* pool;
    int size;
    int used;
};

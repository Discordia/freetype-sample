#pragma once

#include <Log.h>
#include <cassert>
#include <string.h>

//#define LOG_TAG "Cache"

template <class T>
class Cache
{
public:
    Cache()
    {
        size = 0;
        cache = NULL;
        used = 0;
    }

    ~Cache()
    {
        release();
    }

    void release()
    {
        if (cache != NULL)
        {
            free(cache);
            size = 0;
            cache = NULL;
            used = 0;
        }
    }

    void init(int size)
    {
        if (this->size < size)
        {
            T* newCache = (T*) malloc(size * sizeof(T));

            if (newCache == NULL)
            {
  //              LOGE("Not enough memory ");
            }

            if (used > 0)
            {
                memcpy(newCache, cache, used * sizeof(T));
            }

            release();
            cache = newCache;
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
    //        LOGW("Alocating when no memory left in cache, returning NULL");
            assert(false);

            return NULL;
        }

        return &cache[used++];
    }

    void deallocate()
    {
        used--;
    }

    T* getCache()
    {
        return cache;
    }

    T& get(int index)
    {
        return cache[index];
    }

    T* getPtr(int index)
    {
        return &cache[index];
    }

private:
    T* cache;
    int size;
    int used;
};

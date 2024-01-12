#ifndef _CUSTOM_RAND_H
#define _CUSTOM_RAND_H

#include <random>
#include <chrono>

class TRand{

    TRand(const TRand&) = delete;
    TRand(TRand&&) = delete;
    TRand& operator=(const TRand&) = delete;
    TRand& operator=(TRand&&) = delete;
    TRand(const int s):seed_{s},generator(seed_),distribution{1,100}{}

public:
    static inline TRand& instace(){
        static TRand tr(std::chrono::system_clock::now().time_since_epoch().count());
        return tr;
    }
    inline operator int(){
        return distribution(generator);
    }

private:
    const int seed_{};
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution;
};

static inline int rand_()
{
    return TRand::instace();
}

#endif

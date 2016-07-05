//
//  RandomDistribution
//
//
//  Created by Ricardo Amores Hernández on 5/6/16.
//
//

#ifndef RandomDistribution_hpp
#define RandomDistribution_hpp

#include <random>


class RandomDistribution
{
public:

    RandomDistribution(int low, int high);
    RandomDistribution(int low, int high, uint32_t seed);
    
    int next() const;
    
    uint32_t seed() const { return mSeed; }
private:
    uint32_t mSeed;
    
    mutable std::mt19937 mMTRandomEngine;
    mutable std::uniform_int_distribution<int> mUniformIntegerDistribution;
};

#endif /* RandomDistribution_hpp */

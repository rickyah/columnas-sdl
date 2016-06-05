//
//  RandomDistribution
//  Columns
//
//  Created by Ricardo Amores Hernández on 5/6/16.
//
//

#include "RandomDistribution.hpp"

RandomDistribution::RandomDistribution(int low, int high)
:RandomDistribution(low, high, std::random_device()())
{
    
}

RandomDistribution::RandomDistribution(int low, int high, uint32_t seed)
    :mSeed(seed),
    mMTRandomEngine(std::mt19937(seed)),
    mUniformIntegerDistribution(std::uniform_int_distribution<int>(low, high))
{
}

int RandomDistribution::next() const
{
    return mUniformIntegerDistribution.operator()(mMTRandomEngine);
}

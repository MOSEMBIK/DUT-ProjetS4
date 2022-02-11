#pragma once
#include <vector>
#include <functional>
#include <iostream>
#include <iterator>

namespace Data
{
    template<typename T>
    std::vector<T> Generate(const std::vector<unsigned int>& dimension, std::function<T(const std::vector<unsigned int>&)> f)
    {
        unsigned long n = 1;
        std::vector<unsigned int> dividers(dimension.size());
        for(long i = dimension.size() - 1; i >= 0; i--)
        {
            n *= dimension[i];
            dividers[i] = n;
        }

        std::vector<T> result;
        for(unsigned long k = 0; k < n; k++)
        {
            std::vector<unsigned int> values;
            for(unsigned long m = 0 ; m < dividers.size(); m++)
            {
                unsigned int a = k;
                if(m < dividers.size() - 1)
                    a /= dividers[m + 1];
                values.push_back(a % dimension[m]);
            }
                
            result.push_back(f(values));
        }
        return result;
    }

    template <typename T>
    std::vector<T> Flatten(const std::vector<std::vector<T>>& v) 
    {
        std::size_t total_size = 0;
        for (const auto& sub : v)
            total_size += sub.size();
        std::vector<T> result;
        result.reserve(total_size);
        for (const auto& sub : v)
            result.insert(result.end(), sub.begin(), sub.end());
        return result;
    }   
}
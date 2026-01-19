#pragma once
#include <deque>
#include <iterator>

template <class T, class Comp>
std::deque<T> Merge(const std::deque<T>& half1,
    const std::deque<T>& half2,
    const Comp& comparator)
{
    std::deque<T> res;

    auto it1 = half1.begin();
    auto it2 = half2.begin();

    while (it1 != half1.end() && it2 != half2.end()) {
        if (comparator(*it1, *it2)) {
            res.push_back(*it1);
            ++it1;
        }
        else {
            res.push_back(*it2);
            ++it2;
        }
    }

    while (it1 != half1.end()) {
        res.push_back(*it1);
        ++it1;
    }

    while (it2 != half2.end()) {
        res.push_back(*it2);
        ++it2;
    }

    return res;
}

template <class T, class Comp>
std::deque<T> MergeSort(const std::deque<T>& src, const Comp& comparator)
{
    if (src.size() <= 1) {
        return src;
    }

    const size_t mid = src.size() / 2;

    std::deque<T> left(src.begin(), std::next(src.begin(), static_cast<long>(mid)));
    std::deque<T> right(std::next(src.begin(), static_cast<long>(mid)), src.end());

    left = MergeSort<T>(left, comparator);
    right = MergeSort<T>(right, comparator);

    return Merge<T>(left, right, comparator);
}

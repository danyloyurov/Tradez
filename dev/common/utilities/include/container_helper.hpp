#ifndef UTILITIES_CONTAINER_HELPER_HPP_
#define UTILITIES_CONTAINER_HELPER_HPP_

#include <vector>
#include <algorithm>

template<typename T>
class Searcher {
    public:
        static typename std::vector<T>::const_iterator Search(const std::vector<T>& vector, const T& element);
};

template<typename T>
typename std::vector<T>::const_iterator Searcher<T>::Search(const std::vector<T>& vector, const T& element) {
    typename std::vector<T>::const_iterator end_iterator = vector.end();

    typename std::vector<T>::const_iterator left_iterator = vector.begin();
    typename std::vector<T>::const_iterator right_iterator = end_iterator;

    if (vector.size() == 0 || vector.front() > element || vector.back() < element) {
        return end_iterator;
    }

    while (std::distance(left_iterator, right_iterator) > 0) {
        typename std::vector<T>::const_iterator middle_iterator = left_iterator + std::distance(left_iterator, right_iterator) / 2;

        if (element <= *middle_iterator) {
            right_iterator = middle_iterator;
        } else {
            left_iterator = middle_iterator + 1;
        }
    }

    if (*right_iterator == element) {
        return right_iterator;
    }

    return end_iterator;
}

template<typename T>
class Sorter {
    public:
        static typename std::vector<T> Sort(const std::vector<T>& vector);
};

template<typename T>
typename std::vector<T> Sorter<T>::Sort(const std::vector<T>& vector) {
    auto comparator = [](const T& one, const T& two) 
    { 
        return one < two;
    };

    auto output_vector = vector;

    std::stable_sort(output_vector.begin(), output_vector.end(), comparator);

    return output_vector;
}

#endif // UTILITIES_CONTAINER_HELPER_HPP_

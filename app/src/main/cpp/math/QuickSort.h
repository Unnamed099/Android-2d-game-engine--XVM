#ifndef QUICKSORT_H
#define QUICKSORT_H

#include <vector>
#include "Units.h"

class QuickSort {
public:
    static std::vector<Units*> UnitsListSort(std::vector<Units*> l) ;

private:
    static void insertionSort(std::vector<int>& arr, std::vector<Units*>* list, int n);
};

#endif // QUICKSORT_H
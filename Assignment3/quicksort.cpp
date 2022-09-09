#include <ctime>
#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <algorithm>

#define SIZE 100000000

int singlePivotPartition(int* t, int v, int h);
int median3sort(int* t, int v, int h);
int dualPivotPartition(int* arr, int low, int high, int* lp);

//Method to make elements swap places from "Algoritmer og Datastrukturer med eksempler i C og Java" by Hafting & Ljosland (2014).
void swap(int* i, int* j) {
    int k = *j;
    *j = *i;
    *i = k;
}

//Quicksort method from "Algoritmer og Datastrukturer med eksempler i C og Java" by Hafting & Ljosland (2014).
void singlePivotQuicksort(int* t, int v, int h) {
    if (h - v > 2) {
        int dividePos = singlePivotPartition(t, v, h);
        singlePivotQuicksort(t, v, dividePos -1);
        singlePivotQuicksort(t, dividePos + 1, h);

    } else {
        median3sort(t, v, h);
    }
}

//Method to sort 3 numbers to find the median from "Algoritmer og Datastrukturer med eksempler i C og Java" by Hafting & Ljosland (2014).
int median3sort(int* t, int v, int h) {
    int m = (v + h) / 2;

    if (t[v] > t[m]) {
        swap(&t[v], &t[m]);
    }
    if (t[m] > t[h]) {
        swap(&t[m], &t[h]);
        if (t[v] > t[m]){
            swap(&t[v], &t[m]);
        }
    }
    
    return m;
}

//Method to find a divisor value and place the lower values on lower indexes, and higher values on higher indexes. 
//From "Algoritmer og Datastrukturer med eksempler i C og Java" by Hafting & Ljosland (2014).
int singlePivotPartition(int* t, int v, int h) {
    int iv, ih;
    int m = median3sort(t, v, h);
    int dv = t[m];
    swap(&t[m], &t[h - 1]);

    for (iv = v, ih = h - 1;;) {
        while (t[++iv] < dv);
        while (t[--ih] > dv);

        if (iv >= ih) {
            break;
        }

        swap(&t[iv], &t[ih]);
    }

    swap(&t[iv], &t[h - 1]);
    return iv;
}

//Method to perform a dual pivot quicksort.
//Code retrieved from https://www.geeksforgeeks.org/dual-pivot-quicksort/ on September 9th, 2022. 
void dualPivotQuicksort(int* arr, int low, int high) {
    if (low < high) {
        //lp means left pivot, and rp means right pivot.
        int lp, rp;
        rp = dualPivotPartition(arr, low, high, &lp);
        dualPivotQuicksort(arr, low, lp - 1);
        dualPivotQuicksort(arr, lp + 1, rp - 1);
        dualPivotQuicksort(arr, rp + 1, high);
    }
}

//Method to split and place the elements in the list using three different intervals.
//Code retrieved from https://www.geeksforgeeks.org/dual-pivot-quicksort/ on September 9th, 2022.
//Small edits implemented to compensate for weaknesses in the code.
int dualPivotPartition(int* arr, int low, int high, int* lp) {
    swap(&arr[low], &arr[low + (high - low) / 3]);
    swap(&arr[high], &arr[high - (high - low) / 3]);

    if (arr[low] > arr[high])
        swap(&arr[low], &arr[high]);
    //p is the left pivot, and q is the right pivot.
    int j = low + 1;
    int g = high - 1;
    int k = low + 1;
    int p = arr[low];
    int q = arr[high];

    while (k <= g) {
 
        //If elements are less than the left pivot
        if (arr[k] < p) {
            swap(&arr[k], &arr[j]);
            j++;
        }
 
        //If elements are greater than or equal
        //To the right pivot
        else if (arr[k] >= q) {
            while (arr[g] > q && k < g)
                g--;
            swap(&arr[k], &arr[g]);
            g--;
            if (arr[k] < p) {
                swap(&arr[k], &arr[j]);
                j++;
            }
        }
        k++;
    }
    j--;
    g++;
 
    //Bring pivots to their appropriate positions.
    swap(&arr[low], &arr[j]);
    swap(&arr[high], &arr[g]);
 
    //Returning the indices of the pivots.
    *lp = j; //Because we cannot return two elements
    //From a function.
 
    return g;
}

int main(int argc, char const *argv[]) {
    //Initialize two different lists of random integers with a size equal to the constant SIZE.
    srand(time(NULL));
    int* list1 = new int[SIZE];
    int* list2 = new int[SIZE];

    for (int i = 0; i < SIZE; i++) {
        list1[i] = rand();
        list2[i] = rand();
    }

    //Initialize two different lists of random integers with several duplicates among the elements.
    int* listWithDuplicates1 = new int[SIZE];
    int* listWithDuplicates2 = new int[SIZE];
    int range = 1000 - 50 + 1;

    for (int i = 0; i < SIZE; i++) {
        listWithDuplicates1[i] = rand() % range;
        listWithDuplicates2[i] = rand() % range; 
    }

    //Initialize two different lists of already sorted integers.
    int* sortedList1 = new int[SIZE];
    int* sortedList2 = new int[SIZE];
    
    std::sort(sortedList1, sortedList1 + sizeof sortedList1);
    std::sort(sortedList2, sortedList2 + sizeof sortedList2);

    //Measure time for the single pivot quicksort for a list of random integers.
    auto start = std::chrono::high_resolution_clock::now();
    singlePivotQuicksort(list1, 0, SIZE - 1);
    auto end = std::chrono::high_resolution_clock::now();
    auto timeUsedSinglePivotRandom = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    //Measure time for the dual pivot quicksort for a list of random integers..
    start = std::chrono::high_resolution_clock::now();
    dualPivotQuicksort(list2, 0, SIZE - 1);
    end = std::chrono::high_resolution_clock::now();
    auto timeUsedDualPivotRandom = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    //Measure time for the single pivot quicksort for a list of random integers with several duplicates.
    start = std::chrono::high_resolution_clock::now();
    singlePivotQuicksort(listWithDuplicates1, 0, SIZE - 1);
    end = std::chrono::high_resolution_clock::now();
    auto timeUsedSinglePivotDuplicate = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    //Measure time for the dual pivot quicksort for a list of random integers with several duplicates.
    start = std::chrono::high_resolution_clock::now();
    singlePivotQuicksort(listWithDuplicates2, 0, SIZE - 1);
    end = std::chrono::high_resolution_clock::now();
    auto timeUsedDualPivotDuplicate = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    //Measure time for the single pivot quicksort for a list that is already sorted.
    start = std::chrono::high_resolution_clock::now();
    singlePivotQuicksort(sortedList1, 0, SIZE - 1);
    end = std::chrono::high_resolution_clock::now();
    auto timeUsedSinglePivotSorted = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    //Measure time for the dual pivot quicksort for a list that is already sorted.
    start = std::chrono::high_resolution_clock::now();
    singlePivotQuicksort(sortedList2, 0, SIZE - 1);
    end = std::chrono::high_resolution_clock::now();
    auto timeUsedDualPivotSorted = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    //Print results.
    std::cout << "Time used for single pivot quicksort on list of random integers: " << timeUsedSinglePivotRandom.count() << " µs\n";
    std::cout << "Time used for dual pivot quicksort on list of random integers: " << timeUsedDualPivotRandom.count() << " µs\n\n";
    std::cout << "Time used for single pivot quicksort on list with duplicates: " << timeUsedSinglePivotDuplicate.count() << " µs\n";
    std::cout << "Time used for dual pivot quicksort on list with duplicates: " << timeUsedDualPivotDuplicate.count() << " µs\n\n";
    std::cout << "Time used for single pivot quicksort on a list that is already sorted: " << timeUsedSinglePivotSorted.count() << " µs\n";
    std::cout << "Time used for dual pivot quicksort on a list that is already sorted: " << timeUsedDualPivotSorted.count() << " µs\n" << std::endl;

    return 0;
}

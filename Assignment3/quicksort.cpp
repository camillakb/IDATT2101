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

//Method to find the sum of the elements in an array.
int checkSums(int *list, int n) {
    int sum = 0;

    for (int i = 0; i < n; i++) {
        sum += list[i];
    }

    return sum;
}

//Method to see if the elements in a list are in the correct order after being sorted.
bool checkOrder(int *list, int n) {
    for (int i = 0; i < n - 1; i++) {
        if (list[i + 1] < list [i]) {
            return false;
        }
    }

    return true;
}

int main(int argc, char const *argv[]) {
    //Test data for the checkSums()- and checkOrder()-methods.
    int testList1[10] = {8, 6, 3, 7, 4, 2, 6, 2, 8, 2};
    int testList2[10] = {2, 6, 4, 9, 1, 7, 4, 7, 1, 9};

    std::cout << "Test list for single pivot quicksort: \n";

    for (int i = 0; i < 10; i++) {
        std::cout << testList1[i] << ", ";
    } 

    std::cout << "\n\nSize: " << *(&testList1 + 1) - testList1;

    int checkSums1 = checkSums(testList1, (*(&testList1 + 1) - testList1));
    int checkSums2 = checkSums(testList2, (*(&testList2 + 1) - testList2));
    bool checkOrder1 = checkOrder(testList1, (*(&testList1 + 1) - testList1));
    bool checkOrder2 = checkOrder(testList2, (*(&testList2 + 1) - testList2));

    singlePivotQuicksort(testList1, 0, (*(&testList1 + 1) - testList1) - 1);
    dualPivotQuicksort(testList2, 0 , (*(&testList2 + 1) - testList2) - 1);

    //Printing the results of the tests concerning single pivot quicksort.
    std::cout << "\nSum before sorting the list using single pivot quicksort: " << checkSums1 << "\n";
    std::cout << "Sum after: " << checkSums(testList1, (*(&testList1 + 1) - testList1)) << "\n";
    std::cout << "Was the list in the correct order before using single pivot quicksort? " << checkOrder1 << "\n";
    std::cout << "Is the list in the correct order after using single pivot quicksort? " << checkOrder(testList1, (*(&testList1 + 1) - testList1)) << "\n\n";
    std::cout << "List after single pivot quicksort: \n";

    for (int i = 0; i < 10; i++) {
        std::cout << testList1[i] << ", ";
    }

    std::cout << "\n\nTest list for dual pivot quicksort: \n";

    for (int i = 0; i < 10; i++) {
        std::cout << testList2[i] << ", ";
    } 

    //Printing the results of the tests concerning dual pivot quicksort.
    std::cout << "\nSum before sorting the list using dual pivot quicksort: " << checkSums2 << "\n";
    std::cout << "Sum after: " << checkSums(testList2, (*(&testList2 + 1) - testList2)) << "\n";
    std::cout << "Was the list in the correct order before using dual pivot quicksort? " << checkOrder2 << "\n";
    std::cout << "Is the list in the correct order after using dual pivot quicksort? " << checkOrder(testList2, (*(&testList2 + 1) - testList2)) << "\n\n";
    std::cout << "List after dual pivot quicksort: \n";

    for (int i = 0; i < 10; i++) {
        std::cout << testList2[i] << ", ";
    }

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

    for (int i = 0; i < SIZE; i++) {
        listWithDuplicates1[i] = 50 + (rand() % 950);
        listWithDuplicates2[i] = 50 + (rand() % 950);
    }

    //Initialize two different lists of already sorted integers.
    int* sortedList1 = new int[SIZE];
    int* sortedList2 = new int[SIZE];

    for (int i = 0; i < SIZE; i++) {
        sortedList1[i] = rand();
        sortedList2[i] = rand(); 
    }
    
    std::sort(sortedList1, sortedList1 + SIZE);
    std::sort(sortedList2, sortedList2 + SIZE);    

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
    std::cout << "\n\nTime used for single pivot quicksort on list of random integers: " << timeUsedSinglePivotRandom.count() << " µs\n";
    std::cout << "Time used for dual pivot quicksort on list of random integers: " << timeUsedDualPivotRandom.count() << " µs\n\n";
    std::cout << "Time used for single pivot quicksort on list with duplicates: " << timeUsedSinglePivotDuplicate.count() << " µs\n";
    std::cout << "Time used for dual pivot quicksort on list with duplicates: " << timeUsedDualPivotDuplicate.count() << " µs\n\n";
    std::cout << "Time used for single pivot quicksort on a list that is already sorted: " << timeUsedSinglePivotSorted.count() << " µs\n";
    std::cout << "Time used for dual pivot quicksort on a list that is already sorted: " << timeUsedDualPivotSorted.count() << " µs" << std::endl;

    return 0;
}
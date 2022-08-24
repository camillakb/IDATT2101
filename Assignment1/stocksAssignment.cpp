#include <chrono>
#include <iostream>
#include <stdlib.h>

int main (int argc, char *argv[]) {
    
    int profit;
    int maxProfit = 0;
    int buyDay = 0;
    int sellDay = 0;
    int length = 1000;
    int stocksDaily[length];
    
    //Populating the stocksDaily array with random values between -10 and 10.
    for (int i = 0; i < length; i++) {
        stocksDaily[i] = rand()%21 - 10;
    }

    //Starting the timer for the calculations.
    auto start = std::chrono::high_resolution_clock::now();

    //Finding the most profitable days to buy and sell.
    for (int i = 0; i < length; i++) {
        profit = 0;

        for (int j = i + 1; j < length; j++) {
            profit += stocksDaily[j];

            if (profit > maxProfit) {
                maxProfit = profit;
                buyDay = i + 1;
                sellDay = j + 1;
            }
        }
    }

    //Stopping the timer for the calculation and saving the value in the timeUsed variable.
    auto end = std::chrono::high_resolution_clock::now();
    auto timeUsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    //Printing the results.
    std::cout << "The highest possible profit is " << maxProfit << std::endl;
    std::cout << "To achieve this profit, buy on day " << buyDay << " and sell on day " << sellDay <<std::endl;
    std::cout << "Time used to calculate: " << timeUsed.count() << " µs" << std::endl;
    
    //Can be run to show the daily stock values to make sure that the calculations are correct.
    //printf("\nThe daily stock values: ");
    //for (int i = 0; i < length; i++) {
    //    printf("\nDay %d: %d", i + 1, stocksDaily[i]);
    //}
}
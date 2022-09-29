#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <unordered_map>

#define TABLE_CAPACITY 13000027
#define NUM_ELEMENTS 10000000

//Defining the Hash Table.
class HashTable {
    int collisions;
    int element_count;
    std::vector<int> elements;

    public:
    HashTable (int capacity) : element_count(0), collisions(0) {
        elements = std::vector<int>();
        elements.resize(capacity);
    }

    //Method to suggest a position for the number. If the position is free, the number gets the position.
    int hash_first(int number) {
        return number % TABLE_CAPACITY;
    }

    //Method to decide how much further to move to find a new position for the number.
    int hash_next(int number) {
        return (number % (TABLE_CAPACITY - 1)) + 1;
    }

    //Method to insert into the HashTable. Uses hash_first to see if the position is free, if not
    //hash_next is used until the numberreaches a vacant position.
    void insert_to_table(int number) {
        this->element_count++;
        int position = hash_first(number);

        if (this->elements[position] == 0) {
            this->elements[position] = number;
            return;
        } 

        int jump = hash_next(number);

        for (;;) {
            position += jump;
            position %= TABLE_CAPACITY;

            this->collisions++;

            if (this->elements[position] == 0) {
                this->elements[position] = number;
                return;
            }
        }
    }

    //Calculate the load factor.
    double load_factor() {
        return (double) this->element_count / (double) TABLE_CAPACITY;
    }

    //Method to get the number of collisions.
    double collision_count() {
        return this->collisions;
    }
}; 

int main(int argc, char const *argv[]) {
    //Initializing a table and a list of random numbers to fill the table.
    std::unordered_map<int, int> table;

    std::vector<int> random_numbers;

    for (int i = 0; i < NUM_ELEMENTS; i++) {
        random_numbers.push_back(rand() + 1);
    }

    //Starting the timer for filling a table with random integers.
    auto start = std::chrono::high_resolution_clock::now();

    //Fill table with random integers.
    for (auto number : random_numbers) {
        table.insert({number, number});
    }

    //Stopping the timer for filling the table with random integers and savin the value in the time_used variable.
    auto end = std::chrono::high_resolution_clock::now();
    auto time_used = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    //Initializing a HashTable and a vector of random integers.
    HashTable hashtable = HashTable(TABLE_CAPACITY);

    //Starting the timer for filling the HashTable with random integers.
    auto start_hash = std::chrono::high_resolution_clock::now();

    //Fill HashTable with random integers.
    for (auto number : random_numbers) {
        hashtable.insert_to_table(number);
    }

    //Stopping the timer for filling the HashTable with random integers and saving the value in the time_used_hash variable.
    auto end_hash = std::chrono::high_resolution_clock::now();
    auto time_used_hash = std::chrono::duration_cast<std::chrono::milliseconds>(end_hash - start_hash);

    //Print collision count, load factor and time used (time for both tables to be filled with random integers)
    std::cout << "The time used to fill the table by using the predefined method in c++: " << time_used.count() << " ms" << std::endl;
    std::cout << "The time used to fill the table by using the implemented HashTable methods: " << time_used_hash.count() << " ms" << std::endl;
    std::cout << "The number of collisions for the HashTable methods: " << hashtable.collision_count() << std::endl;
    std::cout << "Load factor for the HashTable methods: " << hashtable.load_factor() << std::endl;
    return 0;
}
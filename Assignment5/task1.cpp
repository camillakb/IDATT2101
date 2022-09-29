#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <fstream>
#include <algorithm>

#define TABLE_CAPACITY 127

//Defining the Hash Table.
class HashTable {
    std::vector<std::list<std::string>> items;
    int element_count;
    int collisions;

    public:
    HashTable(int capacity) : element_count(0), collisions(0) {
        items = std::vector<std::list<std::string>>();
        items.resize(capacity, std::list<std::string>());
    }

    //Create hash from string.
    static int hash(std::string name) {
        unsigned long hash = 0;

        for (int i = 0; i < name.length(); i++) {
            hash = ((hash * 7) + name[i]) % TABLE_CAPACITY;
        }

        return hash;
    }
    
    //Method to insert name to hashtable.
    void insert(std::string name) {
        int namehash = HashTable::hash(name);

        if (!this->items[namehash].empty()) {
            this->collisions++;
            std::cout << name << " collided with ";

            for (auto collided : this->items[namehash]) {
                std::cout << collided << ", ";
            }

            std::cout << std::endl;
        }

        this->items[namehash].push_front(name);
        this->element_count++;
    }

    //Method to see if a name is in the hashtable.
    bool contains(std::string name) {
        auto bucket = this->items[HashTable::hash(name)];
        return std::find(bucket.begin(), bucket.end(), name) != bucket.end();
    }

    //Calculate the load factor.
    double load_factor() {
        return (double) this->element_count / (double) TABLE_CAPACITY;
    }

    //Calculate collisions per element.
    double collisions_per_element() {
        return (double) this->collisions / (double) this->element_count;
    }

};

//Read from file and return a vector containing all the individual lines from the file.
std::vector<std::string> read_to_vector(std::string filename) {
    std::ifstream filestream(filename);
    std::string linebuffer;
    std::vector<std::string> result; 

    while (getline(filestream, linebuffer)) {
        result.push_back(linebuffer);
    }

    return result;
}

int main(int argc, char const *argv[]) {
    HashTable table = HashTable(TABLE_CAPACITY);
    auto vector = read_to_vector("navn.txt");

    for (auto name : vector) {
        table.insert(name);
    }

    std::cout << std::endl;
    std::cout << "Am I in the table? " << (table.contains("Camilla Kristiansen Birkelund") ? "Yes" : "No") << std::endl;
    std::cout << "Load factor: " << table.load_factor() << std::endl;
    std::cout << "Collisions per insertion: " << table.collisions_per_element() << std::endl;

    return 0;
}
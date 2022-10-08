#include <vector>
#include <string>
#include <fstream>
#include <iterator>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <deque>

class Graph {
    std::vector<std::vector<int>> nodes;

    public:
    Graph(int capacity) {
        nodes = std::vector<std::vector<int>>(capacity);
    }

    // Method to add an edge.
    void add_edge(int from, int to) {
        nodes[from].push_back(to);
    }

    static Graph from_file(std::string filename) {
        // Stream of file contents from filename
        std::ifstream filestream(filename);

        // Buffer to read each line into
        std::string linebuffer;

        // Stream to iterate over the words in a line
        std::istringstream linestream;

        // Buffer to hold each word from a line
        std::vector<std::string> wordbuffer;
        
        // Read the first line from the file
        getline(filestream, linebuffer); 

        linestream = std::istringstream(linebuffer);

        // Copy each word from the line to the wordbuffer.
        copy(std::istream_iterator<std::string>(linestream),
            std::istream_iterator<std::string>(),
            std::back_inserter(wordbuffer));

        //Create a graph with the capacity given in the first line of the file
        Graph graph(std::stoi(wordbuffer[0]));

        int from, to;

        while(getline(filestream, linebuffer)) {
            linestream = std::istringstream(linebuffer);
            wordbuffer = std::vector<std::string>();

            copy(std::istream_iterator<std::string>(linestream),
                std::istream_iterator<std::string>(),
                std::back_inserter(wordbuffer));

            from = std::stoi(wordbuffer[0]);
            to = std::stoi(wordbuffer[1]);

            graph.add_edge(from, to);
        }

        return graph;
    }

    // Method to print the graph.
    void print() {
        for (int i = 0; i < nodes.size(); i++) {
            std::cout << i << ": ";

            for (auto node : nodes[i]) {
                std::cout << node << " ";
            }

            std::cout << std::endl;
        }
    }

    // Method to invert the original graph.
    Graph invert() {
        Graph inverted(nodes.size());

        for (int from = 0; from < nodes.size(); from++) {
            for (auto to : nodes[from]) {
                inverted.add_edge(to, from);
            }
        }
        return inverted;
    }

    // Method to keep track of which nodes have been visited.
    void visit(int node, std::deque<int> *order, std::vector<bool> *visited) {
        for (auto neighbour : nodes[node]) {
            if (!visited->at(neighbour)) {
                visited->at(neighbour) = true;
                visit(neighbour, order, visited);
                order->push_front(neighbour);
            }
        }
    }

    // Method to perform a depth first search.
    std::deque<int> dfs() {
        std::deque<int> order;
        std::vector<bool> visited(nodes.size(), false);

        for (int node = 0; node < nodes.size(); node++) {
            if (!visited[node]) {
                visited[node] = true;
                visit(node, &order, &visited);
                order.push_front(node);
            }
        }

        return order;
    }

    // Method to print strongly connected components.
    void printSCC() {

        auto order = this->dfs();
        auto inverted = this->invert();
        std::vector<std::deque<int>> components;
        int node;

        std::vector<bool> visited(nodes.size(), false);

        for (int i = 0; i < nodes.size(); ++i) {
            node = order[i];

            if (!visited[node]) {
                visited[node] = true;
                components.push_back(std::deque<int>());
                inverted.visit(node, &components[components.size() - 1], &visited);
                components[components.size() - 1].push_front(node);
            }
        }

        std::cout << "This graph has " << components.size() << " strongly connected components" << std::endl;

        if (nodes.size() < 100) {
            for (int i = 0; i < components.size(); ++i) {
                std::cout << "Component " << i + 1 << ": ";

                for (auto node : components[i]) {
                    std::cout << node << " ";
                }

                std::cout << std::endl;
            }
        }
    }
};

int main(int argc, char const *argv[]) {
    Graph graph = Graph::from_file("graphø6g6.txt");
    graph.printSCC();

    return 0;
}

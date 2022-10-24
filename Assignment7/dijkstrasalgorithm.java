import java.util.*;
import java.io.BufferedReader;
import java.nio.file.Files;
import java.nio.file.Path;

// Class to represent the nodes in the graph.
class Node {
    public int nodeID;

    //Constructor for the Node class.
    public Node(int nodeID) { 
        this.nodeID = nodeID;
    } 
}

// Class to represent the edges in the graph.
class Edge {
    Node to;
    Node from;
    int distance;

    // Constructor for the Edge class.
    public Edge(Node from, Node to, int distance) {
        this.from = from;
        this.to = to;
        this.distance = distance;
    }
}

// Class to represent the elements in the queue.
class QueueElement implements Comparable<QueueElement> {
    Node node;
    int distance;

    // Constructor for the QueueElement class.
    public QueueElement(Node node, int distance) {
        this.node = node;
        this.distance = distance;
    }

    // Method to compare two elements in the queue.
    @Override
    public int compareTo(QueueElement other) {
        return this.distance - other.distance;
    }

    // Method to see if two elements in the queue are the same (not using Hash values).
    @Override
    public boolean equals(Object other) {
        if (!(other instanceof QueueElement) || Objects.isNull(other)) {
            return false;
        }

        QueueElement otherElement = (QueueElement) other;

        return this.node.nodeID == otherElement.node.nodeID && this.distance == otherElement.distance;
    }
}

// Class to represent the result of Dijkstra's algorithm.
class DijkstraResult {
    ArrayList<Node> previous;
    ArrayList<Integer> distances;

    // Constructor for the DijkstraResult class.
    public DijkstraResult(ArrayList<Node> prev, ArrayList<Integer> distances) {
        this.previous = prev;
        this.distances = distances;
    }

    // toString method for the result of Dijkstra's algorithm.
    @Override
    public String toString() {
        StringBuilder buffer = new StringBuilder("Node    Previous  Distance\n");

        for (int i = 0; i < this.distances.size(); i++) {
            if (this.distances.get(i) == Integer.MAX_VALUE) {
                buffer.append(String.format("%6d %9s %s\n", i, "", "Not reachable"));
            
            } else if (Objects.isNull(this.previous.get(i))) {
                buffer.append(String.format("%6d %9s %d\n", i, "start", this.distances.get(i)));
            
            } else {
                buffer.append(String.format("%6d %9d %d\n", i, this.previous.get(i).nodeID, this.distances.get(i)));
            }
        }

        return buffer.toString();
    }
}

// Class to represent the graph.
class Graph {
    ArrayList<Node> nodes;
    ArrayList<Edge> edges;

    // Constructor for the graph class.
    public Graph() {
        this.nodes = new ArrayList<>();
        this.edges = new ArrayList<>();
    }

    // Method to read from file.
    public static Graph fromFile(String filename) {
        int nodeCount;
        int edgeCount;
        int distance;

        ArrayList<Edge> edges = new ArrayList<>();
        ArrayList<Node> nodes = new ArrayList<>();

        Node from, to;

        try (BufferedReader reader = Files.newBufferedReader(Path.of(filename))) {
            String line = reader.readLine().stripLeading(); // Removes any blank space at the start of a line.
            String[] words = line.split("\\s+"); // Splits line at blank spaces.

            // Find the number of nodes and edges in the graph.
            nodeCount = Integer.parseInt(words[0]);
            edgeCount = Integer.parseInt(words[1]);

            // Create nodes in the graph.
            for (int i = 0; i < nodeCount; i++) {
                nodes.add(new Node(i));
            }

            // Iterate through and add nodes in the graph.
            while ((line = reader.readLine()) != null) {
                line = line.stripLeading();
                words = line.split("\\s+");

                from = nodes.get(Integer.parseInt(words[0]));
                to = nodes.get(Integer.parseInt(words[1]));
                distance = Integer.parseInt(words[2]);

                edges.add(new Edge(from, to, distance));
            }

            if (edgeCount != edges.size()) {
                throw new IllegalArgumentException("Unexpected number of edges.");
            }
        
        } catch (Exception e) {
            System.out.println("The file provided does not follow the correct format.");
            System.exit(1);
        }

        // Create and return the graph.
        Graph graph = new Graph();
        graph.edges = edges;
        graph.nodes = nodes;

        return graph;
    }

    // Method to perform Dijkstra's algorithm.
    public DijkstraResult dijkstras(int startnode) {
        ArrayList<Node> previous = new ArrayList<>(Collections.nCopies(this.nodes.size(), null));
        ArrayList<Integer> distances = new ArrayList<>(Collections.nCopies(this.nodes.size(), null));
        PriorityQueue<QueueElement> queue = new PriorityQueue<>();

        distances.set(startnode, 0);

        // For-loop to find the startnode before continuing the algorithm.
        for (Node node : this.nodes) {
            if (node.nodeID == startnode) {
                continue;
            }

            distances.set(node.nodeID, Integer.MAX_VALUE);
            previous.set(node.nodeID, null);
        }

        queue.offer(new QueueElement(this.nodes.get(startnode), 0));

        while (!queue.isEmpty()) {
            Node node = queue.poll().node;

            for (Edge edge : this.edges) {
                // If the edge is connected to the same node (edge.from) as our current node (node).
                if (edge.from == node) {
                    Node neighbour = edge.to; // The neighbour node (the other node that the edge connects to).
                    int new_distance = distances.get(node.nodeID) + edge.distance; // Distance is saved as new_distance.

                    // If new_distance is shorter than the distance of the neighbour.
                    if (new_distance < distances.get(neighbour.nodeID)) {
                        int old_distance = distances.get(neighbour.nodeID); // Set old_distance to be the distance of the neighbour.
                        distances.set(neighbour.nodeID, new_distance); 
                        previous.set(neighbour.nodeID, node);

                        queue.remove(new QueueElement(neighbour, old_distance)); // Remove old_distance from the queue.
                        queue.offer(new QueueElement(neighbour, new_distance)); // Add new_distance to the queue.
                    }
                }
            }
        }

        return new DijkstraResult(previous, distances);
    }

    public static void main(String[] args) {
        System.out.println(Graph.fromFile("vg1").dijkstras(3));
    }
}


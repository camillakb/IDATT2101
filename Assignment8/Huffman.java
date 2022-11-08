import java.io.IOException;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.PriorityQueue;

/*
 * Class to represent a node in the Huffman tree.
 */
class Node implements Comparable {
    Byte value;
    int frequency;
    Node leftChild;
    Node rightChild;

    /**
     * Simple constructor for a Node object.
     * @param value the byte value the node contains.
     * @param frequency the frequency of a node.
     */
    public Node(Byte value, int frequency, Node leftChild, Node rightChild) {
        this.value = value;
        this.frequency = frequency;
        this.leftChild = leftChild;
        this.rightChild = rightChild;
    }

    /**
     * Method to create arraylist of bytes from integers.
     * @return an arraylist of bytes.
     */
    public ArrayList<Byte> toBytes() {
        ArrayList<Byte> result = new ArrayList<>();
        byte[] freqBytes = ByteBuffer.allocate(4).putInt(frequency).array();

        for (int i = 0; i < freqBytes.length; i++) {
            result.add(freqBytes[i]);
        }

        result.add(value);

        return result;
    }

    /**
     * Method to create a node from an arraylist of bytes.
     * @param bytes arraylist of exactly 5 bytes.
     * @return node made from arraylist of bytes.
     */
    public static Node fromBytes(ArrayList<Byte> bytes) {
        Node result;

        if (bytes.size() != 5) {
            throw new IllegalArgumentException("Tried to construct a node from an unexpected amount of bytes.");
        }

        byte[] freqBytes = new byte[4];

        // Converting bytes into an int to use for the frequency for the node.
        for (int i = 0; i < 4; i++) {
            freqBytes[i] = bytes.get(i);
        }

        int frequency = ByteBuffer.wrap(freqBytes).getInt();

        // Last element in the array (element 4) is the node value.
        result = new Node(bytes.get(4), frequency, null, null);

        return result;
    }

    /**
     * Method to retrieve the frequency of a node.
     * @return the frequency of a node.
     */
    public int getFrequency() {
        return frequency;
    }

    public enum Jump {
        LEFT, RIGHT
      };
    
    /**
     * Method to find path to a node value.
     * @param value value of the node.
     * @param path path to the node.
     * @return true if the node is in a path, false if not.
     */
    public boolean findPath(Byte value, ArrayList<Jump> path) {
        boolean result = false;
        
        if (this.value == value) {
            result = true;
        }
        
        if (this.leftChild != null) {
            path.add(Jump.LEFT);
            
            if (this.leftChild.findPath(value, path)) {
                result = true;
            
            } else {
                path.remove(path.size() - 1);
            }
        }
        
        if (this.rightChild != null) {
            path.add(Jump.RIGHT);
            
            if (this.rightChild.findPath(value, path)) {
                result = true;
            
            } else {
                path.remove(path.size() - 1);
            }
        }
        
        return result;
      }

    /**
     * Method to compare two nodes based on their frequency.
     * @param o object to compare to.
     * @return 
     */
    @Override
    public int compareTo(Object o) {
        Node other = (Node) o;

        return this.frequency - other.frequency;
    }
}

/*
 * Class to compress a file by using the Huffman algorithm and related methods.
 */
class Huffman {

    /**
     * Method to create a frequency queue of nodes.
     * @param byteArray array of bytes used to make the queue.
     * @return a priority queue of nodes.
     */
    public static PriorityQueue<Node> CountFrequency(ArrayList<Byte> byteArray) {
        PriorityQueue<Node> frequencyCount = new PriorityQueue<>();
        Byte node;
        boolean[] visited = new boolean[byteArray.size()]; // Default value of all elements will be false.

        for (int i = 0; i < byteArray.size(); i++) {
            node = byteArray.get(i);
            
            if (!visited[i]) {
                visited[i] = true;
                int frequency = 1; // Sets the frequency of the node to 1.

                for (int j = i + 1; j < byteArray.size(); j++) {
                    if (byteArray.get(i) == byteArray.get(j)) { 
                        if(!visited[j])  {
                            visited[j] = true;
                            frequency++; 
                        }
                    }
                }

                Node newNode = new Node(node, frequency, null, null);
                frequencyCount.add(newNode);
            } 
        }

        return frequencyCount; 
    }
    
    /**
     * Method to create a binary tree from a priority queue of nodes.
     * @param frequencyQueue the priority queue to make the binary tree from.
     * @return a binary tree containing the nodes from a priority queue.
     */
    public static Node HuffmanTree(PriorityQueue<Node> frequencyQueue) {

        while (frequencyQueue.size() > 1) { // Combining nodes until there is only one left in the queue.
            Node firstNode = frequencyQueue.poll(); // Getting the two nodes with the lowest frequency in  the queue.
            Node secondNode = frequencyQueue.poll();
            
            int frequency = firstNode.getFrequency() + secondNode.getFrequency(); // Getting the sum of the frequencies.
            
            Node newNode = new Node(null, frequency, firstNode, secondNode);  
            frequencyQueue.add(newNode); 
        }

        return frequencyQueue.poll();
    }


    /**
     * Method to perform compression using the Huffman algorithm.
     * @param arr the arraylist of bytes to compress.
     * @return an arraylist of compressed data.
     */
    public static ArrayList<Byte> compress(ArrayList<Byte> arr) {
        ArrayList<Byte> result = new ArrayList<>();
        ArrayList<Node.Jump> path = new ArrayList<>();
        ArrayList<Node.Jump> totalPath = new ArrayList<>();
        byte byteBuffer = 0;
        int bitCounter = 0;
        
        // Perform the CountFrequency method on the byte array.
        PriorityQueue<Node> frequencyQueue = CountFrequency(arr);

        // Turns the size of the frequency queue into a byte array and adds it to the result above the frequency queue.
        byte[] freqBytesSize = ByteBuffer.allocate(4).putInt(frequencyQueue.size()).array();

        for (int i = 0; i < freqBytesSize.length; i++) {
            result.add(freqBytesSize[i]);
        }

        // Converts the frequency queue into an arraylist of byte values, and adds it to the result.
        for (Node node : frequencyQueue) {
           for (Byte b : node.toBytes()) {
               result.add(b);
           }
        }

        // Create the Huffman tree from the frequency queue.
        Node huffmanTree = HuffmanTree(frequencyQueue);

        // Find the shortest paths to each byte.
        for (Byte b : arr) {
            if (!huffmanTree.findPath(b, path)) {
                System.err.println("Tried to process a byte value not found in the Huffman tree!");
                System.exit(1);
            }

            totalPath.addAll(path);
            path.clear();
        }

        // Turns the size of the path data into a byte array and adds it to the result above the actual data.
        byte[] pathSizeBytes = ByteBuffer.allocate(4).putInt(totalPath.size()).array();

        for (int i = 0; i < pathSizeBytes.length; i++) {
            result.add(pathSizeBytes[i]);
        }

        // Turns left and right jumps into 0 and 1 instead of "LEFT" and "RIGHT". 
        for (Node.Jump jump : totalPath) {
            if (jump == Node.Jump.LEFT) {
                byteBuffer += 0;
            } 

            if (jump == Node.Jump.RIGHT) {
                byteBuffer += 128 >> bitCounter; // Moves to the left to get the correct order.
            }

            bitCounter++;

            if (bitCounter == 8) { // When the bitCounter reaches 8, the for loop begins to create a new byte.
                result.add(byteBuffer);
                bitCounter = 0;
                byteBuffer = 0;
            }
        }

        if (bitCounter != 0) { // Adds the remaining bits if there's any left that couldn't fill an entire byte.
            result.add(byteBuffer);
        }
        return result;
    }

    /**
     * Method to decompress an arraylist of bytes.
     * @param data arraylist of data to decompress.
     * @return an arraylist of decompressed data.
     */
    public static ArrayList<Byte> decompress(ArrayList<Byte> data) {
        ArrayList<Byte> result = new ArrayList<>();
        int position = 0;
  
        ArrayList<Byte> multiByteBuffer = new ArrayList<>();

        // Read 4 bytes fom the current position in the input data
        for (int i = 0; i < 4; i++) {
            multiByteBuffer.add(data.get(position));
            position++;
        }

        // Parse those four bytes into a 32 bit signed integer,
        // which represents the size of the frequency table.
        int freqTableLength = Utility.byteToInt(multiByteBuffer);
        multiByteBuffer.clear();

        PriorityQueue<Node> frqTable = new PriorityQueue<>();

        while (frqTable.size() < freqTableLength) {
            // Read 5 bytes fom the current position in the input data
            for (int i = 0; i < 5; i++) {
                multiByteBuffer.add(data.get(position));
                position++;
            }

            Node newNode = Node.fromBytes(multiByteBuffer);
            frqTable.add(newNode);
            multiByteBuffer.clear();
        }

        Node root = HuffmanTree(frqTable);

        // Read 4 bytes fom the current position in the input data
        for (int i = 0; i < 4; i++) {
            multiByteBuffer.add(data.get(position));
            position++;
        }

        // Parse those four bytes into a 32 bit signed integer,
        // which represents the number of compressed bits.
        int totalBitCount = Utility.byteToInt(multiByteBuffer);
        int bitCounter = 0;
        int bitIndex = 0;
        int currentBit;
        Node currentNode = root;
    
        Byte currentByte = data.get(position);
        position++;

        while (bitCounter < totalBitCount) {
            currentBit = Math.abs(currentByte >> (7 - bitIndex)) % 2; // either 1 or 0
            bitIndex++;
            bitCounter++;

            if (currentBit == 0) {
                currentNode = currentNode.leftChild;
            }

            if (currentBit == 1) {
                currentNode = currentNode.rightChild;
            }

            if (currentNode.value != null) {
                result.add(currentNode.value);
                currentNode = root;
            }

            if (bitCounter % 8 == 0 && bitCounter < totalBitCount) {
                currentByte = data.get(position);
                position++;
                bitIndex = 0;
            }
        }

        return result;
    }



    public static void main(String [] args) throws IOException {
        // Test for the compress method.
        //ArrayList<Byte> bytearr = FileHandling.ReadFile("Name of the file you want to compress");
        //FileHandling.WriteToFile(compress(bytearr), "comp");
        

        
        // Test for the decompress method.
        //ArrayList<Byte> arr = FileHandling.ReadFile("comp");
        //FileHandling.WriteToFile(decompress(arr), "Name of the file you want to contain the decompressed data");
    }
}


import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Scanner;

/*
 * Class for file handling.
 */
class FileHandling {
    /**
     * Method to read from a file and return the bytes in the file.
     * 
     * @param filename name of the file we're reading from.
     * @return an arraylist of the bytes in the file.
     * @throws IOException
     */
    public static ArrayList<Byte> ReadFile(String filename) throws IOException {
        Path path = Paths.get(filename);
        byte[] bytes = Files.readAllBytes(path);

        ArrayList<Byte> data = new ArrayList<>();

        for (byte b : bytes) {
            data.add(b);
        }

        return data;
    }

    /**
     * Method to write to a file.
     * 
     * @param filedata the file's contents.
     * @param filename the name of the file.
     * @throws IOException
     */
    public static void WriteToFile(ArrayList<Byte> filedata, String filename) throws IOException {
        Path newFilePath = Paths.get(filename);
        byte[] bytes = new byte[filedata.size()];

        for (int i = 0; i < bytes.length; i++) {
            bytes[i] = filedata.get(i);
        }

        Files.write(newFilePath, bytes);
    }
}

class Utility {
    /**
     * Method to convert an array of bytes to an integer value.
     * 
     * @param byteArray the byte array to be converted.
     * @return an integer value of the byte array.
     */
    public static int byteToInt(ArrayList<Byte> bytes) {
        int result = 0;

        for (Byte currentByte : bytes) {
            result = (result << 8) + (currentByte & 0xFF);
        }

        return result;
    }
}

public class client {
    
    public static void main(String[] args) throws IOException {

        Scanner input = new Scanner(System.in);
        int userInput;
        String filename, newFilename;

        /*
         * Client program where the user can choose to compress or decompress a file.
         */
        do {
            System.out.println("What would you like to do?");
            System.out.println("\n1. Compress a file. \n2. Decompress a file. \n3. Quit.");

            userInput = input.nextInt();
            System.out.println();
            
            switch(userInput) {
                case 1: 
                    System.out.println("Which file would you like to compress?");
                    input.nextLine(); // This is to consume the extra \n character.
                    filename = input.nextLine();

                    System.out.println("What would you like to name the compressed file?");
                    newFilename = input.nextLine();
                    
                    ArrayList<Byte> compBytes = FileHandling.ReadFile(filename);
                    ArrayList<Byte> lzcompressed = LZ77Compress.compress(compBytes);

                    ArrayList<Byte> comp = Huffman.compress(lzcompressed);

                    FileHandling.WriteToFile(comp, newFilename);

                    System.out.println("Compression complete.");
                    break;

                case 2: 
                    System.out.println("Which file would you like to decompress?");
                    input.nextLine(); // This is to consume the extra \n character.
                    filename = input.nextLine();


                    System.out.println("What would you like to name the decompressed file?");
                    newFilename = input.nextLine();

                    ArrayList<Byte> decompBytes = FileHandling.ReadFile(filename);
                    ArrayList<Byte> huffmanDecomp = Huffman.decompress(decompBytes);

                    ArrayList<Byte> decomp = LZ77Decompress.decompress(huffmanDecomp);

                    FileHandling.WriteToFile(decomp, newFilename);

                    System.out.println("Decompression complete.");
                    break;
            }
        } 
        
        while (userInput != 3);
        input.close();
    } 
}

import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;

/*
 * Class for compressing a file by using the Lempel-Ziv 77 algorithm.
 */
class LZ77Compress {
    private static final int BUFFERSIZE = 512;
    private static final int REFERENCESIZE = 8;

    /**
     * Method to convert integer values to bytes.
     * 
     * @param n the integer to convert to byte.
     * @return an arraylist of bytes.
     */
    private static ArrayList<Byte> intToBytes(int n) {
        return new ArrayList<>(
                Arrays.asList((byte) ((n >> 24) & 0xff),
                        (byte) ((n >> 16) & 0xff),
                        (byte) ((n >> 8) & 0xff),
                        (byte) ((n >> 0) & 0xff)));
    }

    /**
     * Method to find matching sequences of bytes.
     * 
     * @param bytes    array of bytes.
     * @param position position in the array.
     * @return best length and best jump.
     */
    public static int[] FindMatch(ArrayList<Byte> bytes, int position) {
        int bSize = position < BUFFERSIZE ? position : BUFFERSIZE;
        byte[] buffer = new byte[bSize];

        int bufferStart = position < BUFFERSIZE ? 0 : position - BUFFERSIZE;

        int j = 0;
        for (int i = bufferStart; i < position; i++) {
            buffer[j++] = bytes.get(i);
        }

        int bufferPosition = 0;
        int matchPosition;
        int bestLength = 0;
        int bestJump = 0;

        while (bufferPosition < buffer.length) {

            if (buffer[bufferPosition] == bytes.get(position)) {
                matchPosition = 1;

                while (bufferPosition + matchPosition < buffer.length &&
                        position + matchPosition < bytes.size() &&
                        buffer[bufferPosition + matchPosition] == bytes.get(position + matchPosition)) {
                    matchPosition++;
                }

                if (matchPosition > REFERENCESIZE) {
                    bestLength = matchPosition;
                    bestJump = bSize - bufferPosition;
                }
            }

            bufferPosition++;
        }

        int[] result = { bestLength, bestJump };
        return result;
    }

    /**
     * Method to perform Lempel-Ziv 77.
     * 
     * @param data the array of bytes to perform Lempel-Ziv 77 on.
     * @return an arraylist of the result of the Lempel-Ziv 77 method.
     */
    public static ArrayList<Byte> compress(ArrayList<Byte> data) {
        ArrayList<Byte> result = new ArrayList<>();

        int jump, length;
        int[] match;
        int position = 0;

        while (position < data.size()) {
            match = FindMatch(data, position);
            length = match[0];
            jump = match[1];
            ArrayList<Byte> uncompressed = new ArrayList<>();

            if (length == 0) {
                uncompressed.clear();

                while (length == 0 && position < data.size()) {
                    uncompressed.add(data.get(position));
                    position++;

                    if (position == data.size()) {
                        break;
                    }

                    match = FindMatch(data, position);
                    length = match[0];
                    jump = match[1];
                }
            }

            // Adding the blocks that can't be compressed.
            ArrayList<Byte> uncompressedBlockLengthBytes = intToBytes(uncompressed.size());

            result.addAll(uncompressedBlockLengthBytes);
            result.addAll(uncompressed);

            if (length > 0) {
                // Adding the length of the blocks that can be compressed.
                ArrayList<Byte> compressedBlockLengthBytes = intToBytes(-length);
                result.addAll(compressedBlockLengthBytes);

                // Adding the content of the compressed blocks.
                ArrayList<Byte> compressedBlockJumpBytes = intToBytes(jump);
                result.addAll(compressedBlockJumpBytes);

                position += length;
            }
        }

        return result;
    }
}

/*
 * Class for decompressing a file by using the Lempel-Ziv 77 algorithm.
 */
class LZ77Decompress {

    /**
     * Method to decompress a file using the Lempel-Ziv 77 algorithm.
     * 
     * @param compressedData the compressed data that will be decompressed.
     * @return an arraylist of bytes containing the decompressed data.
     */
    public static ArrayList<Byte> decompress(ArrayList<Byte> compressedData) {
        ArrayList<Byte> result = new ArrayList<>();
        int position = 0;
        
        // While there's still unprocessed data in the input
        while (position < compressedData.size()) {
            ArrayList<Byte> byteBuffer = new ArrayList<>();

            // Read 4 bytes fom the current position in the input data
            for (int i = 0; i < 4; i++) {
                byteBuffer.add(compressedData.get(position));
                position++;
            }

            // Parse those four bytes into a 32 bit signed integer,
            // which represents the size of the following block of data
            int blocklength = Utility.byteToInt(byteBuffer);
            
            if (blocklength >= 0) {
                // If the block size is positive, the following block is uncompressed,
                // and we simply copy the data in the block from the input to the output
                for (int i = 0; i < blocklength; i++) {
                    result.add(compressedData.get(position));
                    position++;
                }
            } else {
                // If the block size is negative, the following block is a copy of earlier data
                // and needs to be dereferenced

                // Read four more bytes...
                for (int i = 0; i < 4; i++) {
                    byteBuffer.set(i, compressedData.get(position));
                    position++;
                }

                // ...and parse those bytes as another 32 bit integer.
                // This one represents how far back we need to look to find the data in this block
                int jumpLength = Utility.byteToInt(byteBuffer);

                // Finally, copy the right amount of data from the right place using the block length and jump length values
                for (int i = 0; i < -blocklength; i++) {
                    result.add(result.get(result.size() - jumpLength));
                }
            }
        }

        return result;
    }
}

public class LZ77 {
    public static void main(String[] args) throws IOException {
        // To test the compress method:
        /*
        ArrayList<Byte> data1 = FileHandling.ReadFile("Name of the file you want compressed");
         
        ArrayList<Byte> compressedObjs = LZ77Compress.compress(data1);
         
        FileHandling.WriteToFile(compressedObjs, "comptest");
         */

        
         // To test the decompress method:
         /* 
        ArrayList<Byte> data2 = FileHandling.ReadFile("comptest");

        ArrayList<Byte> decompressedObjs = LZ77Decompress.decompress(data2);

        FileHandling.WriteToFile(decompressedObjs, "Name of the file containing the decompressed data");
        */
    
    }
}
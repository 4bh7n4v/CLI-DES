#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <cmath>

using namespace std;

class HexConverter 
{
private:
    
    vector<vector<int>> hexToBinaryMap;
    vector<string> hexChars = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"};

public:
    HexConverter() 
    {
        //Representation of hexadecimal in Binary
        hexToBinaryMap = 
        {
            {0, 0, 0, 0}, // 0
            {0, 0, 0, 1}, // 1
            {0, 0, 1, 0}, // 2
            {0, 0, 1, 1}, // 3
            {0, 1, 0, 0}, // 4
            {0, 1, 0, 1}, // 5
            {0, 1, 1, 0}, // 6
            {0, 1, 1, 1}, // 7
            {1, 0, 0, 0}, // 8
            {1, 0, 0, 1}, // 9
            {1, 0, 1, 0}, // A 
            {1, 0, 1, 1}, // B 
            {1, 1, 0, 0}, // C 
            {1, 1, 0, 1}, // D 
            {1, 1, 1, 0}, // E 
            {1, 1, 1, 1}  // F 
        };
    }

    // Method to convert hexadecimal string to binary vector
    vector<int> convertHexToBinary(const string& hexString) 
    {
        vector<int> binaryVector;

        for (char hexChar : hexString) 
        {
            // Validate and convert the hex character to its decimal equivalent
            int hexValue = charToDecimal(hexChar);
            
            // Append the 4-bit binary equivalent to the vector
            binaryVector.insert(binaryVector.end(), hexToBinaryMap[hexValue].begin(), hexToBinaryMap[hexValue].end());
        }

        return binaryVector;
    }

    // Method to convert hexadecimal string to text
    string convertHexToText(const string& hexString) 
    {
        //Check Hex String of even Length
        if (hexString.length() % 2 != 0) 
        {
            throw invalid_argument("Hex string length must be even.");
        }

        string textOutput;
        for (size_t i = 0; i < hexString.length(); i += 2) 
        {
            string byteHex = hexString.substr(i, 2); // Extract two hex characters (one byte)
            char character = hexToChar(byteHex); // Convert the byte to a char
            textOutput += character; 
        }
        return textOutput;
    }

    // Method to convert binary vector to hexadecimal string
    string convertBinaryToHex(const vector<int>& binaryVector) 
    {
        stringstream hexStream;

        // Process the binary vector in chunks of 4 bits (1 hex character per 4 bits)
        for (size_t i = 0; i < binaryVector.size(); i += 4) 
        {
            int value = 0;
            for (int j = 0; j < 4; ++j) 
            {
                if (i + j < binaryVector.size()) 
                {
                    value |= binaryVector[i + j] << (3 - j); // Assemble the 4-bit value
                }
            }
            hexStream << hexChars[value]; // Convert to hex character
        }

        return hexStream.str();
    }

private:
    // method to convert a hexadecimal character to its decimal value
    int charToDecimal(char hexChar) 
    {
        if (hexChar >= '0' && hexChar <= '9') 
        {
            return hexChar - '0';
        } 
        else if (hexChar >= 'A' && hexChar <= 'F') 
        {
            return hexChar - 'A' + 10;
        }
        else if (hexChar >= 'a' && hexChar <= 'f') 
        {
            return hexChar - 'a' + 10;
        } 
        else 
        {
            throw invalid_argument("Invalid hexadecimal character: " + string(1, hexChar));
        }
    }

    //method to convert a pair of hex characters into a character
    char hexToChar(const string& hex) 
    {
        int byte;
        stringstream ss;
        ss << hex << std::hex; // Enable hex input
        ss >> byte; 

        return static_cast<char>(byte); // Convert the integer byte to a char
    }
};


class KeyScheduling 
{
protected:
    string Key;                      // Original or processed key
    vector<int> binaryBitsArray;     // Flattened binary representation
    vector<int> Left_key;            // Left half of the key
    vector<int> Right_key;           // Right half of the key
    vector<int> Second_Permutation;
    vector<int> LEFT_SHIFTS = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1}; //Shift by Position for KeySheduling


    // PC-1 Permutation Table
    vector<int> PC1 = 
    {
        57, 49, 41, 33, 25, 17, 9, 1,
        58, 50, 42, 34, 26, 18, 10, 2,
        59, 51, 43, 35, 27, 19, 11, 3,
        60, 52, 44, 36, 63, 55, 47, 39,
        31, 23, 15, 7, 62, 54, 46, 38,
        30, 22, 14, 6, 61, 53, 45, 37,
        29, 21, 13, 5, 28, 20, 12, 4
    };
    //PC-2 Permutation Table
    vector<int> PC2 = 
    {
        14, 17, 11, 24,  1, 5,  3, 28,
        15,  6, 21, 10, 23, 19, 12, 4,
        26,  8, 16,  7, 27, 20, 13, 2,
        41, 52, 31, 37, 47, 55, 30, 40,
        51, 45, 33, 48, 44, 49, 39, 56,
        34, 53, 46, 42, 50, 36, 29, 32
    };

public:
    static int SHIFT_Count;
    KeyScheduling():Key("")
    {
        SHIFT_Count=0;
    }
    KeyScheduling(string k) : Key(k) 
    {
        SHIFT_Count=0;
    }

    vector<int> &getLeftKey() {
        return Left_key; 
    }
    vector<int> &getRightKey() { 
        return Right_key; 
    }

    // Converts Text to Binary
    void convertToBinary() 
    {
        binaryBitsArray.clear();
        for (char ch : Key) 
        {
            vector<int> binaryBits;
            int asciiValue = static_cast<int>(ch);  // Convert character to its ASCII value

            // Convert the ASCII value to binary (8 bits)
            for (int i = 7; i >= 0; --i) 
            {
                binaryBits.push_back(asciiValue % 2); 
                asciiValue /= 2; 
            }

            reverse(binaryBits.begin(), binaryBits.end()); // Ensure the correct order of bits
            binaryBitsArray.insert(binaryBitsArray.end(), binaryBits.begin(), binaryBits.end());
        }
    }


    // Display binary representation
    void displayBinary(const string &label) const 
    {
        for (size_t i = 0; i < binaryBitsArray.size(); ++i) 
        {
            cout << binaryBitsArray[i];
            if ((i + 1) % 8 == 0) cout << " ";
        }
        cout << endl;
    }

    // Apply PC-1 Permutation
    void permutation1() 
    {
        // Ensure the input binaryBitsArray contains exactly 64 bits
        if (binaryBitsArray.size() != 64) 
        {
            cerr << "Error: binaryBitsArray must contain exactly 64 bits before permutation!" << endl;
            return;
        }

        vector<int> permutedArray(56);

        // Apply the PC-1 permutation
        for (int i = 0; i < 56; ++i) 
        {
            int position = PC1[i] - 1; 
            permutedArray[i] = binaryBitsArray[position];
        }

        binaryBitsArray = permutedArray;
    }

    // Partition key into left and right halves
    void keypartition() 
    {
        Left_key.assign(binaryBitsArray.begin(), binaryBitsArray.begin() + 28); //Assign Left Key
        Right_key.assign(binaryBitsArray.begin() + 28, binaryBitsArray.end()); //Assign Right Key

    }

    // Perform left circular shift
    void manualRotateLeft_LeftKey(vector<int> &vec, int shift) 
    {
        int n = vec.size();
        vector<int> temp(n); 

        // Shift elements and apply modulo for circular rotation
        for (int i = 0; i < n; ++i) 
        {
            temp[i] = vec[(i + shift) % n]; 
        }

        for (int i = 0; i < n; ++i) 
        {
            Left_key[i] = temp[i];  
        }
    }

    //Rotation of left and Right part of key individually
    void manualRotateLeft_RightKey(vector<int> &vec, int shift) 
    {
        int n = vec.size();
        vector<int> temp(n); 

        // Shift elements and apply modulo for circular rotation
        for (int i = 0; i < n; ++i) 
        {
            temp[i] = vec[(i + shift) % n];  
        }

        for (int i = 0; i < n; ++i) 
        {
            Right_key[i] = temp[i];  
        }
    }
    
    //Shifting Left key by Shift Amount
    void leftShift_LeftKey(vector<int> &vec) 
    {
        int shiftAmount = LEFT_SHIFTS[SHIFT_Count]; 
        manualRotateLeft_LeftKey(vec, shiftAmount);       
    }

    //Shift Right key by shift Amount
    void leftShift_RightKey(vector<int> &vec) 
    {
        int shiftAmount = LEFT_SHIFTS[SHIFT_Count];
        manualRotateLeft_RightKey(vec, shiftAmount);       
    }


    static int getShift(){
        return SHIFT_Count;
    }
    int getShiftPosition(int a){
        return LEFT_SHIFTS[a];
    }

    //Unite Left and Right Key into Single key
    void UniteLeft_Right()
    {
        binaryBitsArray.clear();
        for(int bit:Left_key)
        {
            binaryBitsArray.push_back(bit);
        }
        for(int bit:Right_key)
        {
            binaryBitsArray.push_back(bit);
        }
     
    }

    void permutation2() 
    {
        // Ensure the input binaryBitsArray contains exactly 64 bits
        if (binaryBitsArray.size() != 56) 
        {
            cerr << "Error: binaryBitsArray must contain exactly 56 bits before permutation!" << endl;
            return;
        }

        vector<int> permutedArray(48);

        // Apply the PC-2 permutation
        for (int i = 0; i < 48; ++i) 
        {
            int position = PC2[i] - 1; 
            permutedArray[i] = binaryBitsArray[position];
        }

        Second_Permutation = permutedArray;
        cout << endl;
    }

    //Display Key 
    void display_binaryBitsArray()
    {
        for(int bit :binaryBitsArray)
        {
            cout<<bit;
        }
    }

    vector<int> &Set_64Key(const vector<int> Temp){
        binaryBitsArray = Temp;
    }

    vector<int> &get_48key(){ 
        return Second_Permutation;
    }


};

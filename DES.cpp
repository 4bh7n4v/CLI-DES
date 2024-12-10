#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <bitset>
#include <cmath>

using namespace std;

class KeyScheduling {
protected:
    string Key;                      // Original or processed key
    vector<int> binaryBitsArray;     // Flattened binary representation
    vector<int> Left_key;            // Left half of the key
    vector<int> Right_key;           // Right half of the key
    vector<int> Second_Permutation;
    vector<int> LEFT_SHIFTS = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};


    // PC-1 Permutation Table
    vector<int> PC1 = {
        57, 49, 41, 33, 25, 17, 9, 1,
        58, 50, 42, 34, 26, 18, 10, 2,
        59, 51, 43, 35, 27, 19, 11, 3,
        60, 52, 44, 36, 63, 55, 47, 39,
        31, 23, 15, 7, 62, 54, 46, 38,
        30, 22, 14, 6, 61, 53, 45, 37,
        29, 21, 13, 5, 28, 20, 12, 4};
    //PC-2 Permutation Table
    vector<int> PC2 = {
        14, 17, 11, 24,  1, 5,  3, 28,
        15,  6, 21, 10, 23, 19, 12, 4,
        26,  8, 16,  7, 27, 20, 13, 2,
        41, 52, 31, 37, 47, 55, 30, 40,
        51, 45, 33, 48, 44, 49, 39, 56,
        34, 53, 46, 42, 50, 36, 29, 32
    };
    // Validate key length
    void validateKey(const string &k) {
        if (k.length() != 8) {
            throw invalid_argument("Key must be exactly 8 characters long");
        }
    }

public:
    static int SHIFT_Count;
    KeyScheduling(string k) : Key(k) {
        validateKey(k);
        SHIFT_Count=0;
        cout << "Parameterized Key constructed with: " << Key << endl;
    }

    vector<int> &getLeftKey() { return Left_key; }
    vector<int> &getRightKey() { return Right_key; }

    void convertToBinary() {
        binaryBitsArray.clear();

        for (char ch : Key) {
            vector<int> binaryBits;
            int asciiValue = static_cast<int>(ch);  // Convert character to its ASCII value

            // Convert the ASCII value to binary (8 bits)
            for (int i = 7; i >= 0; --i) {
                binaryBits.push_back(asciiValue % 2); 
                asciiValue /= 2; 
            }

            reverse(binaryBits.begin(), binaryBits.end()); // Ensure the correct order of bits
            binaryBitsArray.insert(binaryBitsArray.end(), binaryBits.begin(), binaryBits.end());
        }
    }


    // Display binary representation
    void displayBinary(const string &label) const {
        //cout << label << " \"" << Key << "\":" << endl;
        for (size_t i = 0; i < binaryBitsArray.size(); ++i) {
            cout << binaryBitsArray[i];
            if ((i + 1) % 8 == 0) cout << " ";
        }
        cout << endl;
    }

    // Apply PC-1 Permutation
    void permutation1() {
        // Ensure the input binaryBitsArray contains exactly 64 bits
        if (binaryBitsArray.size() != 64) {
            cerr << "Error: binaryBitsArray must contain exactly 64 bits before permutation!" << endl;
            return;
        }

        vector<int> permutedArray(56);

        // Apply the PC-1 permutation
        for (int i = 0; i < 56; ++i) {
            int position = PC1[i] - 1; 
            permutedArray[i] = binaryBitsArray[position];
        }

        binaryBitsArray = permutedArray;

        // Confirm size after permutation
        cout << "Permuted key size (should be 56): " << binaryBitsArray.size() << endl;
    }

    // Partition key into left and right halves
    void keypartition() {
        Left_key.assign(binaryBitsArray.begin(), binaryBitsArray.begin() + 28);
        Right_key.assign(binaryBitsArray.begin() + 28, binaryBitsArray.end());

        cout << "Left part of the key: ";
    }

    // Perform left circular shift
    void manualRotateLeft_LeftKey(vector<int> &vec, int shift) {
        int n = vec.size();
        vector<int> temp(n); 

        // Shift elements and apply modulo for circular rotation
        for (int i = 0; i < n; ++i) {
            temp[i] = vec[(i + shift) % n];  // Use modulo for wrapping around
        }

        for (int i = 0; i < n; ++i) {
            Left_key[i] = temp[i];  // Copy back the rotated values
        }
    }
    void manualRotateLeft_RightKey(vector<int> &vec, int shift) {
        int n = vec.size();
        vector<int> temp(n); 

        // Shift elements and apply modulo for circular rotation
        for (int i = 0; i < n; ++i) {
            temp[i] = vec[(i + shift) % n];  // Use modulo for wrapping around
        }

        for (int i = 0; i < n; ++i) {
            Right_key[i] = temp[i];  // Copy back the rotated values
        }
    }

    void leftShift_LeftKey(vector<int> &vec) {
        int shiftAmount = LEFT_SHIFTS[SHIFT_Count]; // Get the shift amount
        manualRotateLeft_LeftKey(vec, shiftAmount);        // Perform the rotation
    }
    void leftShift_RightKey(vector<int> &vec) {
        int shiftAmount = LEFT_SHIFTS[SHIFT_Count]; // Get the shift amount
        manualRotateLeft_RightKey(vec, shiftAmount);        // Perform the rotation
    }


    static int getShift(){return SHIFT_Count;}
    int getShiftPosition(int a){return LEFT_SHIFTS[a];}

    void UniteLeft_Right(){
        binaryBitsArray.clear();
        for(int bit:Left_key){
            binaryBitsArray.push_back(bit);
        }
        for(int bit:Right_key){
            binaryBitsArray.push_back(bit);
        }
        cout<<"size of United key :"<<binaryBitsArray.size()<<endl;
    }

    void permutation2() {
        // Ensure the input binaryBitsArray contains exactly 64 bits
        if (binaryBitsArray.size() != 56) {
            cerr << "Error: binaryBitsArray must contain exactly 56 bits before permutation!" << endl;
            return;
        }

        vector<int> permutedArray(48);

        // Apply the PC-2 permutation
        for (int i = 0; i < 48; ++i) {
            int position = PC2[i] - 1; 
            permutedArray[i] = binaryBitsArray[position];
        }

        Second_Permutation = permutedArray;
        for (int bit : Second_Permutation) {
            
            cout << bit;
        }
        cout << endl;
    }
    void display_binaryBitsArray(){
        for(int bit :binaryBitsArray){
            cout<<bit;
        }
    }
    vector<int> &get_48key(){ return Second_Permutation;}
};
class DESf_function 
{
protected:

    const int expansionTable[48] = 
    {
        32,  1,  2,  3,  4,  5,
         4,  5,  6,  7,  8,  9,
         8,  9, 10, 11, 12, 13,
        12, 13, 14, 15, 16, 17,
        16, 17, 18, 19, 20, 21,
        20, 21, 22, 23, 24, 25,
        24, 25, 26, 27, 28, 29,
        28, 29, 30, 31, 32,  1
    };

    const int sBox[8][4][16] = 
    {
        {
            {14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7},
            { 0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8},
            { 4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0},
            {15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13}
        },

        {
            {15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10},
            { 3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5},
            { 0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15},
            {13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9}
        },

        {
            {10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8},
            {13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1},
            {13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7},
            { 1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12}
        },

        {
            { 7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15},
            {13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9},
            {10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4},
            { 3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14}
        },

        {
            { 2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9},
            {14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6},
            { 4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14},
            {11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3}
        },

        {
            {12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11},
            {10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8},
            { 9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6},
            { 4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13}
        },

        {
            { 4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1},
            {13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6},
            { 1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2},
            { 6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12}
        },

        {
            {13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7},
            { 1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2},
            { 7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8},
            { 2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11}
        }
    };

    const int PERMUTATION_TABLE[32] = 
    {
        16, 7, 20, 21,
        29, 12, 28, 17,
        1, 15, 23, 26,
        5, 18, 31, 10,
        2, 8, 24, 14,
        32, 27, 3, 9,
        19, 13, 30, 6,
        22, 11, 4, 25
    };

public:

    vector<int> expand(const vector<int>& input32) 
    {
        vector<int> output48(48);
        for (int i = 0; i < 48; i++) {
            output48[i] = input32[expansionTable[i] - 1];
        }
        return output48;
    }

    int binaryToDecimal(string binary) 
    {
        int decimal = 0;
        int length = binary.length();
        for (int i = 0; i < length; i++) 
        {
            if (binary[length - 1 - i] == '1') 
            {
                decimal += pow(2, i);
            }
        }
        return decimal;
    }

    string decimalToBinary(int decimal) 
    {
        bitset<4> binary(decimal);
        return binary.to_string( );
    }

    string shorten(const vector<string>& take)
    {
        string temp = "";
        for(int i = 0; i < take.size( ); i++)
        {
            string a = "";
            a += take[i][0];
            a += take[i][5];

            string b= "";
            for(int j = 1; j < 5; j++)
            {
                b += take[i][j];
            }
        
            int num = sBox[i][binaryToDecimal(a)][binaryToDecimal(b)];
            temp += decimalToBinary(num);
        }
        return temp;
    }

    string permutation(const string& input)
    {
        string output = "";
        for(int i = 0; i < 32; i++)
        {
            output += input[PERMUTATION_TABLE[i] - 1];
        }
        return output;
    }
};
int KeyScheduling::SHIFT_Count = 0;
int main() {
    try {
        string key;
        cout << "Enter an 8-character key: ";
        cin >> key;

        KeyScheduling keyScheduler(key);
        keyScheduler.convertToBinary();
        keyScheduler.permutation1();
        keyScheduler.keypartition();

        for (int i = 0; i < 2; ++i) {
            cout << "\nIteration " << (i + 1) << " - Shift by " << keyScheduler.getShiftPosition(keyScheduler.getShift()) << " positions:" << endl;
            
            keyScheduler.leftShift_LeftKey(keyScheduler.getLeftKey());
            keyScheduler.leftShift_RightKey(keyScheduler.getRightKey());
            keyScheduler.SHIFT_Count++;
            keyScheduler.UniteLeft_Right();
            //keyScheduler.display_binaryBitsArray();
            keyScheduler.permutation2();
            {
                 vector<int> input32 = 
                {
                    1, 0, 1, 0, 1, 0, 1, 0,
                    0, 1, 0, 1, 0, 1, 0, 1,
                    1, 1, 0, 0, 1, 1, 0, 0,
                    0, 0, 1, 1, 0, 0, 1, 1
                };

                vector<int> key48;
                key48 = keyScheduler.get_48key();

                DESf_function test;

                string temp = "";

                vector <string> parts;

                if(input32.size() == 32)
                {
                    vector <int> output48 = test.expand(input32);
                    for(int bit :output48){
                        cout<<bit;
                    }
                    for(int i = 0; i < 48; i++)
                    {
                        output48[i] = output48[i] ^ key48[i];
                    }

                    for (int i = 0; i < 48; i++) 
                    {
                        temp += to_string(output48[i]);  
                        if (temp.length() == 6) 
                            {
                                parts.push_back(temp);  
                                temp = "";              
                            }
                    }
                    if (!temp.empty()) 
                    {
                        parts.push_back(temp);
                    }

                    cout<<"After xoring the thing is: "<<endl;
                    for(int i = 0; i< parts.size( ); i++)
                    {
                        cout<<parts[i]<<" ";
                    }
                    cout<<endl;

                    string bits32 = test.shorten(parts);
                    cout<<bits32<<endl;
                    
                    string final = test.permutation(bits32);
                    cout<<final<<endl;
                }
                else
                {
                    cout<<"The input is not 32 bit long\nThe size of the input is "<<input32.size()<<"\nIt must be 32 bit long"<<endl;
                    return 0;                
                }
                key48.clear();
            }
            
        }
    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    return 0;
}

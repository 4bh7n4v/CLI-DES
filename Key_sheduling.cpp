#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
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

        for (int i = 0; i < 16; ++i) {
            cout << "\nIteration " << (i + 1) << " - Shift by " << keyScheduler.getShiftPosition(keyScheduler.getShift()) << " positions:" << endl;
            
            keyScheduler.leftShift_LeftKey(keyScheduler.getLeftKey());
            keyScheduler.leftShift_RightKey(keyScheduler.getRightKey());
            keyScheduler.SHIFT_Count++;
            keyScheduler.UniteLeft_Right();
            //keyScheduler.display_binaryBitsArray();
            keyScheduler.permutation2();
            {




                
            }
        }
    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    return 0;
}

#include <iostream>
#include <bitset>
#include <string>
#include <vector>

using namespace std;

// Permutation tables as constants
const int INITIAL_PERM[64] = {
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7
};

const int FINAL_PERM[64] = {
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41, 9, 49, 17, 57, 25
};

// Utility class for helper functions
class Utility {
public:
    static string stringToBinary(const string& input) {
        string binaryString;
        for (char c : input) {
            bitset<8> binary(c);  // Convert each character to an 8-bit binary
            binaryString += binary.to_string();
        }
        return binaryString;
    }

    static string applyPermutation(const string& binaryInput, const int permTable[64]) {
        string permuted(64, '0');  // Create a 64-bit string initialized with '0'
        for (int i = 0; i < 64; i++) {
            permuted[i] = binaryInput[permTable[i] - 1];
        }
        return permuted;
    }

    static void printWithSpaces(const string& binary) {
        for (size_t i = 0; i < binary.size(); ++i) {
            cout << binary[i];
            if ((i + 1) % 8 == 0) {
                cout << " ";
            }
        }
        cout << endl;
    }
};

// Class for encryption
class DES {
private:
    string key;
    string left;
    string right;

public:
    DES(const string& key) : key(key) {}

    void initialize(const string& input) {
        // Perform initial permutation
        string binaryInput = Utility::stringToBinary(input);
        cout << "Binary representation: ";
        Utility::printWithSpaces(binaryInput);

        string permutedBinary = Utility::applyPermutation(binaryInput, INITIAL_PERM);
        cout << "After Initial Permutation: ";
        Utility::printWithSpaces(permutedBinary);
        cout << "\n";

        // Split into left and right halves
        left = permutedBinary.substr(0, 32);
        right = permutedBinary.substr(32, 32);

        cout << "Initial Left side: ";
        Utility::printWithSpaces(left);
        cout << "Initial Right side: ";
        Utility::printWithSpaces(right);
        cout << "\n";
    }

    void performRounds(int rounds = 16) {
        for (int round = 1; round <= rounds; ++round) {
            // XOR operation
            string newRight;
            for (int i = 0; i < 32; ++i) {
                newRight += (left[i] == key[i] ? '0' : '1');
            }

            // Swap sides
            string newLeft = right;
            right = newRight;
            left = newLeft;

            // Print after each round
            cout << "After Round " << round << ":\n";
            cout << "Left side: ";
            Utility::printWithSpaces(left);
            cout << "Right side: ";
            Utility::printWithSpaces(right);
            cout << "\n";
        }
    }

    void finalize() {
        string finalBinary = right + left;  // Combine halves
        cout << "The final text before permutation is: ";
        Utility::printWithSpaces(finalBinary);

        string finalPermuted = Utility::applyPermutation(finalBinary, FINAL_PERM);
        cout << "The final text after permutation is: ";
        Utility::printWithSpaces(finalPermuted);
    }
};

// Main class to control the application
class App {
public:
    void run() {
        string input, key = "111111110010100010001001000000100000000011111111010101111110011011111111";

        cout << "\nEnter a string (max 8 characters): ";
        getline(cin, input);
        cout << "\n";

        input = input.substr(0, 8);  // Limit to 8 characters

        DES des(key);
        des.initialize(input);      // Perform initial steps
        des.performRounds();        // Perform 16 rounds
        des.finalize();             // Apply final permutation
    }
};

// Main function
int main() {
    App app;
    app.run();
    return 0;
}

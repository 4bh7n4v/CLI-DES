#include <iostream>
#include <bitset>
#include <string>
#include <vector>

using namespace std;

// Permutation tables as constants
class DES: public HexConverter
{
protected:
    string key;        //stores the output from festial_function
    string left;       //stores the left part of the plain text
    string right;      //stores the right part of the plain text
    vector<int> Right_share;        //stores right
    
    //The standard intial permutation of plain text in DES
    const int INITIAL_PERM[64] =      
    {
        58, 50, 42, 34, 26, 18, 10, 2,
        60, 52, 44, 36, 28, 20, 12, 4,
        62, 54, 46, 38, 30, 22, 14, 6,
        64, 56, 48, 40, 32, 24, 16, 8,
        57, 49, 41, 33, 25, 17, 9, 1,
        59, 51, 43, 35, 27, 19, 11, 3,
        61, 53, 45, 37, 29, 21, 13, 5,
        63, 55, 47, 39, 31, 23, 15, 7
    };


    //The standard final permutation(the inverse of intial permutation)
    const int FINAL_PERM[64] = 
    {
        40, 8, 48, 16, 56, 24, 64, 32,
        39, 7, 47, 15, 55, 23, 63, 31,
        38, 6, 46, 14, 54, 22, 62, 30,
        37, 5, 45, 13, 53, 21, 61, 29,
        36, 4, 44, 12, 52, 20, 60, 28,
        35, 3, 43, 11, 51, 19, 59, 27,
        34, 2, 42, 10, 50, 18, 58, 26,
        33, 1, 41, 9, 49, 17, 57, 25
    };  

    static int round;  //calculates the rounds of des

public:
    DES(const string& key) : key(key) {}
    DES() : key("") {}

    // Utility Functions
    static string stringToBinary(const string& input) 
    {
        string binaryString;
        for (char c : input) 
        {
            bitset<8> binary(c);  // Convert each character to an 8-bit binary
            binaryString += binary.to_string();
        }
        return binaryString;
    }


    //permutes the intial plain text
    static string applyPermutation(const string& binaryInput, const int permTable[64]) 
    {
        string permuted(64, '0');  // Create a 64-bit string initialized with '0'
        for (int i = 0; i < 64; i++) 
        {
            permuted[i] = binaryInput[permTable[i] - 1];
        }
        return permuted;
    }
    

    //This function prints the binary with spaces after every 8 bits
    static void printWithSpaces(const string& binary) 
    {
        for (size_t i = 0; i < binary.size(); ++i) 
        {
            cout << binary[i];
            if ((i + 1) % 8 == 0) 
            {
                cout << " ";
            }
        }
        cout << endl;
    }

    // Binary to Vector Converter
    vector<int> toBinaryVector(const string& binaryString) const 
    {
        vector<int> binaryVector;
        for (char ch : binaryString) 
        {
            binaryVector.push_back(ch - '0');
        }
        return binaryVector;
    }


    //prints the binary
    void printBinaryVector() const 
    {
        try 
        {
            vector<int> binaryVector = toBinaryVector(right);
            cout << "Binary Vector: [ ";
            for (int bit : binaryVector) 
            {
                cout << bit << " ";
            }
            cout << "]" << endl;
        } 
        catch (const invalid_argument& e) 
        {
            cerr << e.what() << endl;
        }
    }

    // DES Functions
    void initialize(const string& input) 
    {
        // Perform initial permutation
        string binaryInput = stringToBinary(input);
        
        string permutedBinary = applyPermutation(binaryInput, INITIAL_PERM);
       
        // Split into left and right halves
        left = permutedBinary.substr(0, 32);
        right = permutedBinary.substr(32, 32);
    }


    //performs the each round operation of the DES
    void performRounds(string& a,string& b) 
    {
        // XOR operation
        string newRight;
        left = a;
        right = b;
        for (int i = 0; i < 32; ++i) 
        {
            if(left[i] == key[i])
            {
                newRight += '0'; 
            }
            else
            {
                newRight+='1';
            }
        }
       

        // Swap sides
        string temp = right;
        right = newRight;
        left = temp;

        a = left;
        b = right;

        
        cout << "L"<<round<<":";
        printWithSpaces(left);
        cout << "R"<<round<<":";
        printWithSpaces(right);
        round++;
    }


    //This function does the last swaping and print the cipher text 
    void finalize(string& left,string& right)
    {
        string finalBinary = right + left;  // Combine halves

        string finalPermuted = applyPermutation(finalBinary, FINAL_PERM);
        vector<int> Hex;
        for(char bit:finalPermuted)
        {
            Hex.push_back(bit-'0');
        }
        DES Bin2Hex;
        string Final_output;
        Final_output= Bin2Hex.convertBinaryToHex(Hex);
        cout<<endl;
        cout<<"Cipher text : "<<Final_output<<endl;
    }

    vector<int>& getRightShare() 
    {
        Right_share.clear();  // Clear previous values
        for (char c : right) 
        {
            Right_share.push_back(c - '0');  // Convert '0' or '1' to integer
        }
        return Right_share;
    }

    string getleft()
    {
        return left;
    }

    string getright()
    {
        return right;
    }

    void setKey(const string& k) 
    {
        key = k;
    }
};


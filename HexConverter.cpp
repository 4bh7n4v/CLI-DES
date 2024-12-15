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


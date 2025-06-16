#include <iostream>
#include <queue>
#include <unordered_map>

using namespace std;


struct Node {
    char ch;
    int freq;
    Node *left, *right;
    
    Node(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
};


struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }
};


void generateCodes(Node* root, string code, unordered_map<char, string>& huffmanCode) {
    if (!root) return;
 
    if (!root->left && !root->right) {
        huffmanCode[root->ch] = code;
    }

    generateCodes(root->left, code + "0", huffmanCode);
    generateCodes(root->right, code + "1", huffmanCode);
}


string huffmanEncode(const string& msg, unordered_map<char, string>& huffmanCode) {
    unordered_map<char, int> freqMap;
    

    for (char ch : msg) {
        freqMap[ch]++;
    }

  
    priority_queue<Node*, vector<Node*>, Compare> pq;
    
    for (auto& pair : freqMap) {
        pq.push(new Node(pair.first, pair.second));
    }

    while (pq.size() > 1) {
        Node *left = pq.top(); pq.pop();
        Node *right = pq.top(); pq.pop();
        
        Node *newNode = new Node('\0', left->freq + right->freq); 
        newNode->left = left;
        newNode->right = right;
        
        pq.push(newNode);
    }

    Node* root = pq.top(); 

  
    generateCodes(root, "", huffmanCode);

    string encodedMsg = "";
    for (char ch : msg) {
        encodedMsg += huffmanCode[ch];
    }

    return encodedMsg;
}

string huffmanDecode(const string& encodedMsg, Node* root) {
    string decodedMsg = "";
    Node* current = root;
    
    for (char bit : encodedMsg) {
        if (bit == '0') current = current->left;
        else current = current->right;

        
        if (!current->left && !current->right) {
            decodedMsg += current->ch;
            current = root;
        }
    }

    return decodedMsg;
}

void calculateBitsSaved(const string& msg, unordered_map<char, string>& huffmanCode) {
    int originalBits = msg.length() * 8; 
    int compressedBits = 0;

    for (char ch : msg) {
        compressedBits += huffmanCode[ch].length();
    }

    int bitsSaved = originalBits - compressedBits;
    double compressionRatio = ((double)compressedBits / originalBits) * 100;

    cout << "\nOriginal Bits (ASCII 8-bit per char): " << originalBits;
    cout << "\nCompressed Bits (Huffman Encoding): " << compressedBits;
    cout << "\nBits Saved: " << bitsSaved;
    cout << "\nCompression Ratio: " << compressionRatio << "%\n";
}

int main() {
    string msg;
    cout << "Enter a string to encode using Huffman Coding: ";
    getline(cin, msg);

    unordered_map<char, string> huffmanCode;
    string encodedMsg = huffmanEncode(msg, huffmanCode);

    cout << "\nHuffman Encoded Message: " << encodedMsg << endl;


    cout << "\nHuffman Codes:\n";
    for (auto& pair : huffmanCode) {
        cout << pair.first << " : " << pair.second << endl;
    }

    cout << "\nDecoded Message: " << msg << endl;

    calculateBitsSaved(msg, huffmanCode);

    return 0;
}

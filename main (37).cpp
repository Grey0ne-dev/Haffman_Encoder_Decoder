#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <sstream>
#include <iomanip>
#include <bitset>

using namespace std;

// Huffman Tree Node
struct HuffmanNode {
    char symbol;
    int frequency;
    HuffmanNode *left, *right;
    
    HuffmanNode(char s, int f) : symbol(s), frequency(f), left(nullptr), right(nullptr) {}
    HuffmanNode(int f) : symbol('\0'), frequency(f), left(nullptr), right(nullptr) {}
};

// Command Interface
class HuffmanCLI {
private:
    HuffmanNode* huffmanTree = nullptr;
    map<char, string> huffmanCodes;
    map<string, char> reverseCodes;

    // Build frequency table from text
    map<char, int> buildFrequencyTable(const string& text) {
        map<char, int> freqTable;
        for (char ch : text) {
            freqTable[ch]++;
        }
        return freqTable;
    }

    // Build Huffman tree from frequency table
    void buildTree(const map<char, int>& freqTable) {
        auto cmp = [](HuffmanNode* left, HuffmanNode* right) { 
            return left->frequency > right->frequency; 
        };
        priority_queue<HuffmanNode*, vector<HuffmanNode*>, decltype(cmp)> minHeap(cmp);

        // Create leaf nodes
        for (auto& pair : freqTable) {
            minHeap.push(new HuffmanNode(pair.first, pair.second));
        }

        // Build tree
        while (minHeap.size() > 1) {
            auto left = minHeap.top(); minHeap.pop();
            auto right = minHeap.top(); minHeap.pop();

            auto newNode = new HuffmanNode(left->frequency + right->frequency);
            newNode->left = left;
            newNode->right = right;
            minHeap.push(newNode);
        }

        huffmanTree = minHeap.top();
        generateCodes(huffmanTree, "");
    }

    // Generate Huffman codes
    void generateCodes(HuffmanNode* root, const string& code) {
        if (!root) return;

        if (!root->left && !root->right) {
            huffmanCodes[root->symbol] = code;
            reverseCodes[code] = root->symbol;
        }

        generateCodes(root->left, code + "0");
        generateCodes(root->right, code + "1");
    }

    // Serialize tree for storage
    string serializeTree(HuffmanNode* node) {
        if (!node) return "";

        if (!node->left && !node->right) {
            return string("1") + node->symbol;
        }

        return "0" + serializeTree(node->left) + serializeTree(node->right);
    }

    // Deserialize tree
    HuffmanNode* deserializeTree(const string& data, size_t& pos) {
        if (pos >= data.size()) return nullptr;

        if (data[pos] == '1') {
            pos++;
            return new HuffmanNode(data[pos++], 0);
        }

        pos++;
        auto left = deserializeTree(data, pos);
        auto right = deserializeTree(data, pos);
        auto node = new HuffmanNode(0);
        node->left = left;
        node->right = right;
        return node;
    }

    // Display code table
    void displayCodeTable() {
        cout << "\nHuffman Code Table:\n";
        cout << "┌─────────┬───────────────┐\n";
        cout << "│ Symbol  │ Code          │\n";
        cout << "├─────────┼───────────────┤\n";

        for (const auto& pair : huffmanCodes) {
            cout << "│ " << setw(7) << (pair.first == ' ' ? "SPACE" : string(1, pair.first)) 
                 << " │ " << setw(13) << pair.second << " │\n";
        }
        cout << "└─────────┴───────────────┘\n";
    }

public:
    // Encode text
    string encode(const string& text) {
        string encoded;
        for (char ch : text) {
            if (huffmanCodes.find(ch) == huffmanCodes.end()) {
                throw runtime_error("Symbol not in code table: " + string(1, ch));
            }
            encoded += huffmanCodes[ch];
        }
        return encoded;
    }

    // Decode binary string
    string decode(const string& binary) {
        string decoded;
        HuffmanNode* current = huffmanTree;
        
        for (char bit : binary) {
            current = (bit == '0') ? current->left : current->right;
            
            if (!current->left && !current->right) {
                decoded += current->symbol;
                current = huffmanTree;
            }
        }
        return decoded;
    }

    // Load codes from input
    void loadCodes(const vector<pair<char, string>>& codes) {
        huffmanCodes.clear();
        reverseCodes.clear();
        for (const auto& pair : codes) {
            huffmanCodes[pair.first] = pair.second;
            reverseCodes[pair.second] = pair.first;
        }
        // Note: This method doesn't build a tree, just uses the codes directly
    }

    // Build from text
    void buildFromText(const string& text) {
        auto freqTable = buildFrequencyTable(text);
        buildTree(freqTable);
    }

    // Build from manual frequencies
    void buildFromFrequencies(const vector<pair<char, int>>& frequencies) {
        map<char, int> freqTable;
        for (const auto& pair : frequencies) {
            freqTable[pair.first] = pair.second;
        }
        buildTree(freqTable);
    }

    // Save current coding system
    string save() {
        return serializeTree(huffmanTree);
    }

    // Load coding system
    void load(const string& saved) {
        size_t pos = 0;
        huffmanTree = deserializeTree(saved, pos);
        huffmanCodes.clear();
        reverseCodes.clear();
        generateCodes(huffmanTree, "");
    }

    // Command processor
    void processCommand(const string& command) {
        stringstream ss(command);
        string cmd;
        ss >> cmd;

        if (cmd == "encode") {
            string text;
            getline(ss >> ws, text);
            try {
                cout << "Encoded: " << encode(text) << endl;
            } catch (const exception& e) {
                cerr << "Error: " << e.what() << endl;
            }
        }
        else if (cmd == "decode") {
            string binary;
            getline(ss >> ws, binary);
            cout << "Decoded: " << decode(binary) << endl;
        }
        else if (cmd == "build_text") {
            string text;
            getline(ss >> ws, text);
            buildFromText(text);
            cout << "Huffman tree built from text\n";
            displayCodeTable();
        }
        else if (cmd == "build_freq") {
            vector<pair<char, int>> frequencies;
            cout << "Enter symbol-frequency pairs (symbol frequency), 'done' to finish:\n";
            string line;
            while (getline(cin, line) && line != "done") {
                stringstream ss(line);
                char symbol;
                int freq;
                if (ss >> symbol >> freq) {
                    frequencies.emplace_back(symbol, freq);
                }
            }
            buildFromFrequencies(frequencies);
            displayCodeTable();
        }
        else if (cmd == "import_codes") {
            vector<pair<char, string>> codes;
            cout << "Enter symbol-code pairs (symbol code), 'done' to finish:\n";
            string line;
            while (getline(cin, line) && line != "done") {
                stringstream ss(line);
                char symbol;
                string code;
                if (ss >> symbol >> code) {
                    codes.emplace_back(symbol, code);
                }
            }
            loadCodes(codes);
            cout << "Codes imported\n";
            displayCodeTable();
        }
        else if (cmd == "show_codes") {
            displayCodeTable();
        }
        else if (cmd == "save") {
            cout << "Saved system: " << save() << endl;
        }
        else if (cmd == "load") {
            string data;
            getline(ss >> ws, data);
            load(data);
            cout << "System loaded\n";
            displayCodeTable();
        }
        else if (cmd == "help") {
            cout << "\nAvailable commands:\n"
                 << "encode <text>       - Encode text\n"
                 << "decode <binary>      - Decode binary string\n"
                 << "build_text <text>    - Build from text\n"
                 << "build_freq           - Build from manual frequencies\n"
                 << "import_codes         - Import symbol-code pairs\n"
                 << "show_codes           - Display current codes\n"
                 << "save                 - Save current coding system\n"
                 << "load <data>          - Load coding system\n"
                 << "help                 - Show this help\n"
                 << "exit                 - Exit program\n\n";
        }
        else if (cmd == "exit") {
            exit(0);
        }
        else {
            cout << "Unknown command. Type 'help' for available commands.\n";
        }
    }

    void run() {
        cout << "Huffman Coding System (type 'help' for commands)\n";
        string command;
        while (true) {
            cout << "> ";
            getline(cin, command);
            processCommand(command);
        }
    }
};

int main() {
    HuffmanCLI cli;
    cli.run();
    return 0;
}
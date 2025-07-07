# **Huffman Coding CLI Tool**

## **Advanced Command-Line Huffman Encoder/Decoder**

A sophisticated implementation of Huffman coding with an interactive command interface that supports:
- Multiple input methods (text samples, manual frequencies, direct code import)
- Complete encoding/decoding pipeline
- System serialization for saving/loading coding schemes
- Interactive exploration of coding tables

## **Features**

### **1. Flexible Code Generation**
- **Automatic from text**: `build_text "your sample text"`
- **Manual frequency input**: `build_freq` (interactive)
- **Direct code import**: `import_codes` (interactive)

### **2. Encoding/Decoding Operations**
- **Encode text**: `encode "text to encode"`
- **Decode binary**: `decode "10101001101"`

### **3. System Management**
- **Save coding scheme**: `save`
- **Load existing scheme**: `load [serialized_data]`
- **Inspect codes**: `show_codes`

### **4. Advanced Functionality**
- Full Huffman tree generation
- Optimal prefix codes
- Binary string handling
- Error detection for invalid symbols

## **Installation & Usage**

1. **Compile** (requires C++11):
   ```bash
   g++ -std=c++11 huffman.cpp -o huffman
   ```

2. **Run interactive CLI**:
   ```bash
   ./huffman
   ```

3. **Example workflow**:
   ```bash
   > build_text "this is an example"
   > show_codes
   > encode "this test"
   > decode "0000010001101000001011100"
   > save
   ```

## **Command Reference**

| Command | Description | Example |
|---------|-------------|---------|
| `encode` | Encode text | `encode "sample"` |
| `decode` | Decode binary | `decode "10100"` |
| `build_text` | Build from text | `build_text "example"` |
| `build_freq` | Manual frequency input | (interactive) |
| `import_codes` | Import symbol-code pairs | (interactive) |
| `show_codes` | Display code table | `show_codes` |
| `save` | Serialize current system | `save` |
| `load` | Load serialized system | `load 01a1b1c...` |
| `help` | Show help | `help` |
| `exit` | Quit program | `exit` |

## **Technical Details**

### **Huffman Tree Construction**
1. Frequency analysis of input symbols
2. Priority queue-based tree building
3. Recursive code assignment

### **Serialization Format**
- `0` for internal nodes
- `1` followed by symbol for leaves
- Pre-order traversal

### **Performance**
- O(n log n) tree construction
- O(1) symbol lookup during encoding
- O(m) decoding (where m = bits in message)

## **Example Use Cases**

1. **Text Compression**:
   ```bash
   > build_text $(cat document.txt)
   > encode $(cat document.txt) > compressed.bin
   ```

2. **Custom Coding Schemes**:
   ```bash
   > import_codes
   A 00
   B 01
   C 1
   done
   > encode "AABBC"
   ```

3. **Education/Debugging**:
   ```bash
   > build_text "mississippi"
   > show_codes
   ```

## **Limitations**

1. **ASCII Focused**: Best with ASCII text
2. **Memory**: In-memory processing (large files may require streaming)
3. **Binary I/O**: Outputs binary as strings (not packed bits)

## **License**

MIT License - Free for educational and commercial use

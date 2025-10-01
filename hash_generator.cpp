#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cstdint>
#include <vector>
#include <chrono>

using namespace std;

class CustomHash {
private:
    // Pradiniai state'ai (prime numbers)
    static const uint64_t INIT_A = 0x428a2f98d728ae22ULL;
    static const uint64_t INIT_B = 0x7137449123ef65cdULL;
    static const uint64_t INIT_C = 0xb5c0fbcfec4d3b2fULL;
    static const uint64_t INIT_D = 0xe9b5dba58189dbbcULL;
    
    // Pirminiai skaičiai maišymui
    static const uint64_t PRIME1 = 0x9e3779b185ebca87ULL;
    static const uint64_t PRIME2 = 0xc2b2ae3d27d4eb4fULL;
    static const uint64_t PRIME3 = 0x165667b19e3779f9ULL;
    static const uint64_t PRIME4 = 0x85ebca77c2b2ae63ULL;
    
    // Rotacijos funkcija
    static uint64_t rotateLeft(uint64_t value, int shift) {
        return (value << shift) | (value >> (64 - shift));
    }
    
    static uint64_t rotateRight(uint64_t value, int shift) {
        return (value >> shift) | (value << (64 - shift));
    }
    
    // Maišymo funkcija
    static uint64_t mix(uint64_t a, uint64_t b, uint64_t c) {
        a ^= b;
        a = rotateLeft(a, 23);
        a *= PRIME1;
        a ^= c;
        a = rotateLeft(a, 41);
        return a;
    }
    
public:
    static string hash(const string& input) {
        uint64_t h1 = INIT_A;
        uint64_t h2 = INIT_B;
        uint64_t h3 = INIT_C;
        uint64_t h4 = INIT_D;
        
        size_t len = input.length();
        
        // Apdorojame kiekvieną baitą
        for (size_t i = 0; i < len; i++) {
            uint64_t byte_val = static_cast<uint64_t>(static_cast<unsigned char>(input[i]));
            uint64_t position = i + 1;
            
            // Pridedame pozicijos įtaką (lavinos efektui)
            byte_val ^= position * PRIME1;
            
            // Atnaujiname state'us rotacijos principu
            h1 ^= byte_val * PRIME1;
            h1 = rotateLeft(h1, 13);
            h1 *= PRIME2;
            
            h2 ^= byte_val * PRIME2;
            h2 = rotateRight(h2, 17);
            h2 += h1;
            
            h3 ^= byte_val * PRIME3;
            h3 = rotateLeft(h3, 31);
            h3 ^= h2;
            
            h4 ^= byte_val * PRIME4;
            h4 = rotateRight(h4, 19);
            h4 += h3;
            
            // Tarpusavio maišymas kas 4 baitus
            if (i % 4 == 3) {
                h1 = mix(h1, h2, h3);
                h2 = mix(h2, h3, h4);
                h3 = mix(h3, h4, h1);
                h4 = mix(h4, h1, h2);
            }
        }
        
        // Įtraukiame ilgio įtaką
        h1 ^= len * PRIME1;
        h2 ^= len * PRIME2;
        h3 ^= len * PRIME3;
        h4 ^= len * PRIME4;
        
        // Finalinis maišymas
        h1 = mix(h1, h2, h3);
        h2 = mix(h2, h3, h4);
        h3 = mix(h3, h4, h1);
        h4 = mix(h4, h1, h2);
        
        h1 += h2 + h3 + h4;
        h2 += h1;
        h3 += h1;
        h4 += h1;
        
        // Konvertuojame į hex string (256 bitai = 64 hex simboliai)
        stringstream ss;
        ss << hex << setfill('0');
        ss << setw(16) << h1;
        ss << setw(16) << h2;
        ss << setw(16) << h3;
        ss << setw(16) << h4;
        
        return ss.str();
    }
    
    static string hashFile(const string& filename) {
        ifstream file(filename, ios::binary);
        if (!file) {
            throw runtime_error("Cannot open file: " + filename);
        }
        
        stringstream buffer;
        buffer << file.rdbuf();
        return hash(buffer.str());
    }
};

void printUsage() {
    cout << "Usage:" << endl;
    cout << "  hash_generator <filename>  - Hash file contents" << endl;
    cout << "  hash_generator             - Enter text manually" << endl;
}

int main(int argc, char* argv[]) {
    try {
        if (argc == 2) {
            // Hash failą
            string filename = argv[1];
            auto start = chrono::high_resolution_clock::now();
            string hash_result = CustomHash::hashFile(filename);
            auto end = chrono::high_resolution_clock::now();
            
            auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
            
            cout << "File: " << filename << endl;
            cout << "Hash: " << hash_result << endl;
            cout << "Time: " << duration.count() << " microseconds" << endl;
        } else if (argc == 1) {
            // Įvedimas ranka
            cout << "Enter text to hash (press Enter twice to finish):" << endl;
            string input, line;
            while (getline(cin, line)) {
                if (line.empty() && !input.empty()) break;
                input += line + "\n";
            }
            
            if (!input.empty() && input.back() == '\n') {
                input.pop_back(); // Remove last newline
            }
            
            string hash_result = CustomHash::hash(input);
            cout << "\nHash: " << hash_result << endl;
        } else {
            printUsage();
            return 1;
        }
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}
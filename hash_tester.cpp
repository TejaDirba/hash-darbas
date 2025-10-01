#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cstdint>
#include <chrono>
#include <random>
#include <map>
#include <cmath>
#include <vector>

using namespace std;

// Hash funkcija (ta pati kaip hash_generator.cpp)
class CustomHash {
private:
    static const uint64_t INIT_A = 0x428a2f98d728ae22ULL;
    static const uint64_t INIT_B = 0x7137449123ef65cdULL;
    static const uint64_t INIT_C = 0xb5c0fbcfec4d3b2fULL;
    static const uint64_t INIT_D = 0xe9b5dba58189dbbcULL;
    static const uint64_t PRIME1 = 0x9e3779b185ebca87ULL;
    static const uint64_t PRIME2 = 0xc2b2ae3d27d4eb4fULL;
    static const uint64_t PRIME3 = 0x165667b19e3779f9ULL;
    static const uint64_t PRIME4 = 0x85ebca77c2b2ae63ULL;
    
    static uint64_t rotateLeft(uint64_t v, int s) { return (v << s) | (v >> (64 - s)); }
    static uint64_t rotateRight(uint64_t v, int s) { return (v >> s) | (v << (64 - s)); }
    static uint64_t mix(uint64_t a, uint64_t b, uint64_t c) {
        a ^= b; a = rotateLeft(a, 23); a *= PRIME1; a ^= c; return rotateLeft(a, 41);
    }
public:
    static string hash(const string& input) {
        uint64_t h1 = INIT_A, h2 = INIT_B, h3 = INIT_C, h4 = INIT_D;
        size_t len = input.length();
        for (size_t i = 0; i < len; i++) {
            uint64_t b = static_cast<uint64_t>(static_cast<unsigned char>(input[i])) ^ ((i + 1) * PRIME1);
            h1 ^= b * PRIME1; h1 = rotateLeft(h1, 13); h1 *= PRIME2;
            h2 ^= b * PRIME2; h2 = rotateRight(h2, 17); h2 += h1;
            h3 ^= b * PRIME3; h3 = rotateLeft(h3, 31); h3 ^= h2;
            h4 ^= b * PRIME4; h4 = rotateRight(h4, 19); h4 += h3;
            if (i % 4 == 3) {
                h1 = mix(h1, h2, h3); h2 = mix(h2, h3, h4);
                h3 = mix(h3, h4, h1); h4 = mix(h4, h1, h2);
            }
        }
        h1 ^= len * PRIME1; h2 ^= len * PRIME2; h3 ^= len * PRIME3; h4 ^= len * PRIME4;
        h1 = mix(h1, h2, h3); h2 = mix(h2, h3, h4); h3 = mix(h3, h4, h1); h4 = mix(h4, h1, h2);
        h1 += h2 + h3 + h4; h2 += h1; h3 += h1; h4 += h1;
        stringstream ss; ss << hex << setfill('0') << setw(16) << h1 << setw(16) << h2 << setw(16) << h3 << setw(16) << h4;
        return ss.str();
    }
};

string randStr(int len) {
    static random_device rd; static mt19937 gen(rd());
    static uniform_int_distribution<> dis(0, 61);
    const string c = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    string r; for(int i=0; i<len; i++) r += c[dis(gen)]; return r;
}

int bitDiff(const string& h1, const string& h2) {
    int d = 0;
    for (size_t i = 0; i < 64; i++) {
        int v1 = (h1[i]>='0'&&h1[i]<='9') ? h1[i]-'0' : h1[i]-'a'+10;
        int v2 = (h2[i]>='0'&&h2[i]<='9') ? h2[i]-'0' : h2[i]-'a'+10;
        int x = v1 ^ v2;
        while(x) { d += x&1; x >>= 1; }
    }
    return d;
}

int hexDiff(const string& h1, const string& h2) {
    int d = 0; for(size_t i=0; i<64; i++) if(h1[i]!=h2[i]) d++; return d;
}

void test1() {
    cout << "\n=== TESTAS 1: ISVEDIMO DYDIS ===" << endl;
    vector<pair<string,string>> tests = {{"","Tuscias"}, {"a","1 simbolis"}, {string(100,'a'),"100 sim"}, {string(1000,'a'),"1000 sim"}};
    for(auto& t : tests) {
        string h = CustomHash::hash(t.first);
        cout << t.second << " -> " << h.length() << " hex (" << h.length()*4 << " bit)" << endl;
    }
}

void test2() {
    cout << "\n=== TESTAS 2: DETERMINISTISKUMAS ===" << endl;
    vector<string> inp = {"test", "Lietuva", randStr(50)};
    for(auto& s : inp) {
        string h1 = CustomHash::hash(s);
        bool ok = true;
        for(int i=0; i<9; i++) if(CustomHash::hash(s) != h1) { ok=false; break; }
        cout << "'" << s.substr(0,15) << "...' -> " << (ok?"✓ PASS":"✗ FAIL") << endl;
    }
}

void test3(int n=5000) {
    cout << "\n=== TESTAS 3: LAVINOS EFEKTAS (" << n << " poru) ===" << endl;
    vector<int> bDiff, hDiff;
    for(int i=0; i<n; i++) {
        if(i%1000==0 && i>0) cout << "  " << i << "/" << n << endl;
        string s1 = randStr(50);
        string s2 = s1; s2[25] = (s2[25]=='a'?'b':'a');
        bDiff.push_back(bitDiff(CustomHash::hash(s1), CustomHash::hash(s2)));
        hDiff.push_back(hexDiff(CustomHash::hash(s1), CustomHash::hash(s2)));
    }
    int minB=*min_element(bDiff.begin(),bDiff.end()), maxB=*max_element(bDiff.begin(),bDiff.end());
    double avgB=0; for(int v:bDiff) avgB+=v; avgB/=n;
    cout << "Bitu: min=" << minB << " max=" << maxB << " avg=" << fixed << setprecision(1) << avgB << " (" << avgB/256*100 << "%)" << endl;
    int minH=*min_element(hDiff.begin(),hDiff.end()), maxH=*max_element(hDiff.begin(),hDiff.end());
    double avgH=0; for(int v:hDiff) avgH+=v; avgH/=n;
    cout << "Hex: min=" << minH << " max=" << maxH << " avg=" << avgH << endl;
}

void test4(int n=5000) {
    cout << "\n=== TESTAS 4: KOLIZIJOS (" << n << " testu) ===" << endl;
    map<string,string> hashes; int col=0;
    for(int i=0; i<n; i++) {
        if(i%1000==0 && i>0) cout << "  " << i << "/" << n << endl;
        string s = randStr(100);
        string h = CustomHash::hash(s);
        if(hashes.count(h)) { col++; cout << "KOLIZIJA!" << endl; }
        else hashes[h] = s;
    }
    cout << "Unikaliu: " << hashes.size() << ", Koliziju: " << col << endl;
}

void test5() {
    cout << "\n=== TESTAS 5: EFEKTYVUMAS ===" << endl;
    cout << "| Simboliai | Laikas (ms) |" << endl;
    for(int sz : {100, 1000, 10000, 50000}) {
        string s = randStr(sz);
        vector<double> times;
        for(int i=0; i<5; i++) {
            auto t1 = chrono::high_resolution_clock::now();
            CustomHash::hash(s);
            auto t2 = chrono::high_resolution_clock::now();
            times.push_back(chrono::duration_cast<chrono::microseconds>(t2-t1).count()/1000.0);
        }
        double avg=0; for(double t:times) avg+=t; avg/=5;
        cout << "| " << setw(9) << sz << " | " << fixed << setprecision(3) << avg << " |" << endl;
    }
}

void test6() {
    cout << "\n=== TESTAS 6: PAVYZDZIAI ===" << endl;
    for(string s : {"lietuva", "Lietuva", "Lietuva!"}) {
        cout << s << " -> " << CustomHash::hash(s) << endl;
    }
}

int main() {
    cout << "=== HASH TESTAVIMAS ===" << endl;
    test1(); test2(); test6(); test3(); test4(); test5();
    cout << "\n=== BAIGTA ===" << endl;
    return 0;
}
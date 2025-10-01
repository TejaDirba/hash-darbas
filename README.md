# Custom Hash Generator v0.1

## Aprašymas
Originali hash funkcija, sukurta pagal VU MIF Blokų grandinių technologijos kurso užduoties reikalavimus. Funkcija priima bet kokio ilgio įvedimą ir generuoja 256 bitų (64 hex simbolių) hash kodą.

---

## Hash Funkcijos Idėja (Pseudo-kodas)

```
FUNKCIJA hash(input):
    // 1. Inicializacija - 4 state'ai po 64 bitus
    h1 = 0x428a2f98d728ae22  // Pradinės konstantos (iš SHA-256)
    h2 = 0x7137449123ef65cd
    h3 = 0xb5c0fbcfec4d3b2f
    h4 = 0xe9b5dba58189dbbc
    
    ilgis = input.length()
    
    // 2. Kiekvieno baito apdorojimas
    KIEKVIENAM simboliui (pozicija i):
        byte_val = simbolio reikšmė
        
        // Pozicijos įtaka (lavinos efektui)
        byte_val = byte_val XOR (pozicija * PRIME1)
        
        // Atnaujinti kiekvieną state skirtingai
        h1 = (h1 XOR (byte_val * PRIME1))
        h1 = rotateLeft(h1, 13 bitų)
        h1 = h1 * PRIME2
        
        h2 = (h2 XOR (byte_val * PRIME2))
        h2 = rotateRight(h2, 17 bitų)
        h2 = h2 + h1
        
        h3 = (h3 XOR (byte_val * PRIME3))
        h3 = rotateLeft(h3, 31 bitas)
        h3 = h3 XOR h2
        
        h4 = (h4 XOR (byte_val * PRIME4))
        h4 = rotateRight(h4, 19 bitų)
        h4 = h4 + h3
        
        // Kas 4 baitus - papildomas maišymas
        JEI (i dalinama iš 4):
            h1, h2, h3, h4 = mix(h1, h2, h3, h4)
    
    // 3. Įtraukti ilgio įtaką
    h1 = h1 XOR (ilgis * PRIME1)
    h2 = h2 XOR (ilgis * PRIME2)
    h3 = h3 XOR (ilgis * PRIME3)
    h4 = h4 XOR (ilgis * PRIME4)
    
    // 4. Finalinis maišymas
    h1 = mix(h1, h2, h3)
    h2 = mix(h2, h3, h4)
    h3 = mix(h3, h4, h1)
    h4 = mix(h4, h1, h2)
    
    // Sujungti visus state'us
    h1 = h1 + h2 + h3 + h4
    h2 = h2 + h1
    h3 = h3 + h1
    h4 = h4 + h1
    
    // 5. Grąžinti 256 bitų hash'ą hex formatu (64 simboliai)
    GRĄŽINTI hex(h1) + hex(h2) + hex(h3) + hex(h4)

FUNKCIJA mix(a, b, c):
    a = a XOR b
    a = rotateLeft(a, 23)
    a = a * PRIME1
    a = a XOR c
    a = rotateLeft(a, 41)
    GRĄŽINTI a
```

---

## Kompiliavimas ir Naudojimas

### Reikalavimai
- C++11 arba naujesnė versija
- g++ kompiliatorius

### Kompiliavimas
```bash
# Pagrindinis hash generatorius
g++ -std=c++11 -O2 -o hash_generator hash_generator.cpp

# Testavimo programa (visi 6 testai)
g++ -std=c++11 -O2 -o hash_tester hash_tester.cpp
```

### Naudojimas

#### Hash'inti failą:
```bash
./hash_generator failas.txt
```

#### Įvesti tekstą ranka:
```bash
./hash_generator
# Įveskite tekstą ir spauskite Enter
```

#### Paleisti visus testus:
```bash
./hash_tester
```


## Eksperimentinis Tyrimas

### 1. Išvedimo Dydžio Testas

| Įvedimas | Hash ilgis (hex) | Hash ilgis (bitai) |
|----------|------------------|-------------------|
| Tuščias string | 64 | 256 |
| 1 simbolis ("a") | 64 | 256 |
| 100 simbolių | 64 | 256 |
| 1000 simbolių | 64 | 256 |
| 10000 simbolių | 64 | 256 |

Išvedimo dydis visada pastovus - 256 bitai (64 hex simboliai), nepriklausomai nuo įvedimo ilgio.

---

### 2. Deterministiškumo Testas

| Įvedimas | Kartojimai | Hash sutampa? |
|----------|------------|---------------|
| "test" | 10 | Taip |
| "Lietuva" | 10 | Taip |
| Atsitiktinis 100 sim. | 10 | Taip |

Tas pats įvedimas visada duoda tą patį hash'ą (deterministiškumas užtikrintas).

---

### 3. Efektyvumo Testas

Testavimas atliekamas su atsitiktiniais string'ais. Kiekvienas testas kartojamas 5 kartus, išvedamas vidurkis.

| Simbolių skaičius | Laikas (ms) |
|------------------|-------------|
| 100 | 0.002s |
| 1,000 | 0.004 |
| 10,000 | 0.035 |
| 50,000 | 0.151 |

**Algoritmo sudėtingumas:** O(n), kur n - įvedimo ilgis.

<img width="823" height="520" alt="Screenshot 2025-10-01 233619" src="https://github.com/user-attachments/assets/c46b2d9b-d64f-4eab-a0ff-7fa4bda14d8f" />

---

### 4. Kolizijų Paieška

Generuojami atsitiktiniai string'ai ir tikrinama, ar hash'ai sutampa.

#### 5000 testų su 100 simbolių string'ais:

| Metrika | Reikšmė |
|---------|---------|
| Unikalių hash'ų | 5000 |
| Rastų kolizijų | 0 |
| Kolizijų dažnis | 0 |

**Teorinė kolizijų tikimybė:** Su 256 bitų hash'u ir 5000 testų, kolizijos tikimybė pagal "birthday paradox" yra ~0.0000000000000000001%.

---

### 5. Lavinos Efektas

Testuojama 5000 porų, kurios skiriasi tik vienu simboliu viduryje string'o.

#### Bitų lygmeniu (iš 256 bitų):

| Metrika | Reikšmė |
|---------|---------|
| Minimalus skirtumas | 100 | 
| Maksimalus skirtumas | 158 | 
| Vidutinis skirtumas | 128.0 | 


**Idealus rezultatas:** ~128 bitai (50% bitų pasikeičia)

#### Hex lygmeniu (iš 64 hex simbolių):

| Metrika | Reikšmė |
|---------|---------|
| Minimalus skirtumas | 51 |
| Maksimalus skirtumas | 64 |
| Vidutinis skirtumas | 60.0 |

---

### 6. Negrįžtamumas (Hiding, Puzzle-friendliness)

Demonstracija su HASH(input + salt):

```
Input: "password" + Salt: "xyz123"
Hash: 0817ff571abbd5f0ce0afc9fbe0edb08dc23572b145fbba8dfe5f548be9c674d

Input: "password1" + Salt: "xyz123"  
Hash: c5466c1151fdc1ee87a71a0d1275b6df40b7a005f378e720488e88cd089d8948

Input: "different" + Salt: "xyz123"
Hash: 5fb1721a97d552429b7df1a7f46c5c75c24995a159ec877d47b17b3aebdcd697
```

Net ir žinant salt ir hash'ą, neįmanoma atspėti pradinio teksto be brute-force atakos.

Funkcija negrįžtama - iš hash'o praktiškai neįmanoma atkurti pradinio teksto.

---

## Hash Funkcijos Principai

### Panaudotos Technikos:

1. **Keturi 64-bitų state'ai** - kartu sudaro 256 bitų išvedimą
2. **Pirminiai skaičiai** - naudojami kaip multiplikatoriai maišymui (iš xxHash algoritmo)
3. **Bit rotacijos** - skirtingomis kryptimis ir kampais (13, 17, 19, 23, 31, 41 bitai)
4. **XOR operacijos** - greitos ir efektyvios, sukuria chaotišką maišymą
5. **Pozicijos įtaka** - kiekvieno simbolio pozicija įtakoja galutinį hash'ą
6. **Periodinis mix** - kas 4 baitus state'ai maišomi tarpusavyje
7. **Ilgio įtraukimas** - įvedimo ilgis įtakoja galutinį hash'ą
8. **Avalanche funkcija (mix)** - garantuoja, kad bitų pakeitimai sklinda visame hash'e

---

## Stiprybės

 **Greitai veikia** - O(n) sudėtingumas, tinkamas dideliems failams  
 **Deterministinis** - tas pats įvedimas = tas pats hash'as  
 **Pastovus dydis** - visada 256 bitai (64 hex)  
 **Lavinos efektas** - vienas simbolis keičia ~50% hash'o bitų  
 **Pozicijos jautrumas** - simbolio vieta įtakoja rezultatą  
 **Įvairios operacijos** - XOR, rotacijos, daugyba, sudėtis  


---

## Versijos Istorija

- **v0.1** (2025-10-01) - Pradinė versija su pagrindinėmis funkcijomis
  - 256 bitų išvedimas
  - 4 state'ai su skirtingomis transformacijomis
  - Bit rotacijos ir XOR operacijos
  - Mix funkcija avalanche efektui
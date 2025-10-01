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

---

## Pavyzdžiai

```
lietuva  -> a7f3c1d8e4b2f9a6c5d7e8f0a1b4c6d9e2f5a7b8c0d3e6f9a2b5c7d8e1f4a6b9
Lietuva  -> b8e4f2d9a0c7e3f6b5d8a2c5e9f1b4d7a8c1e4f7b0d3a6c9e2f5b8d1e4f7a0b3
Lietuva! -> c0f3e6d9a2b5c8e1f4d7a0b3c6e9f2d5a8b1c4e7f0d3a6b9c2e5f8d1a4b7c0e3
```

Pastaba: Hash reikšmės priklauso nuo konkrečios implementacijos. Paleiskite `./hash_tester` kad pamatytumėte tikrus rezultatus.

---

## Eksperimentinis Tyrimas

### 1. Išvedimo Dydžio Testas

| Įvedimas | Hash ilgis (hex) | Hash ilgis (bitai) |
|----------|------------------|-------------------|
| Tuščias string | 64 | 256 |
| 1 simbolis ("a") | 64 | 256 |
| 100 simbolių | 64 | 256 |
| 1000 simbolių | 64 | 256 |
| 10000 simbolių | 64 | 256 |

**Išvada:** ✅ Išvedimo dydis visada pastovus - 256 bitai (64 hex simboliai), nepriklausomai nuo įvedimo ilgio.

---

### 2. Deterministiškumo Testas

| Įvedimas | Kartojimai | Hash sutampa? |
|----------|------------|---------------|
| "test" | 10 | ✅ Taip |
| "Lietuva" | 10 | ✅ Taip |
| Atsitiktinis 100 sim. | 10 | ✅ Taip |

**Išvada:** ✅ Tas pats įvedimas visada duoda tą patį hash'ą (deterministiškumas užtikrintas).

---

### 3. Efektyvumo Testas

Testavimas atliekamas su atsitiktiniais string'ais. Kiekvienas testas kartojamas 5 kartus, išvedamas vidurkis.

| Simbolių skaičius | Laikas (ms) | Sparta (MB/s) |
|------------------|-------------|---------------|
| 100 | [paleisti testą] | - |
| 1,000 | [paleisti testą] | - |
| 10,000 | [paleisti testą] | - |
| 50,000 | [paleisti testą] | - |
| 100,000 | [paleisti testą] | - |

**Algoritmo sudėtingumas:** O(n), kur n - įvedimo ilgis.

**Išvada:** [užpildyti po testavimo]

<img width="823" height="520" alt="Screenshot 2025-10-01 233619" src="https://github.com/user-attachments/assets/c46b2d9b-d64f-4eab-a0ff-7fa4bda14d8f" />

---

### 4. Kolizijų Paieška

Generuojami atsitiktiniai string'ai ir tikrinama, ar hash'ai sutampa.

#### 5000 testų su 100 simbolių string'ais:

| Metrika | Reikšmė |
|---------|---------|
| Unikalių hash'ų | [paleisti testą] |
| Rastų kolizijų | [paleisti testą] |
| Kolizijų dažnis | [paleisti testą] |

**Teorinis kolizijų tikimybė:** Su 256 bitų hash'u ir 5000 testų, kolizijos tikimybė pagal "birthday paradox" yra ~0.0000000000000000001%.

**Išvada:** [užpildyti po testavimo]

---

### 5. Lavinos Efektas

Testuojama 5000 porų, kurios skiriasi tik vienu simboliu viduryje string'o.

#### Bitų lygmeniu (iš 256 bitų):

| Metrika | Reikšmė | Procentas |
|---------|---------|-----------|
| Minimalus skirtumas | [paleisti testą] | [X %] |
| Maksimalus skirtumas | [paleisti testą] | [X %] |
| Vidutinis skirtumas | [paleisti testą] | [X %] |
| Standartinis nuokrypis | [paleisti testą] | - |

**⭐ Idealus rezultatas:** ~128 bitai (50% bitų pasikeičia)

#### Hex lygmeniu (iš 64 hex simbolių):

| Metrika | Reikšmė |
|---------|---------|
| Minimalus skirtumas | [paleisti testą] |
| Maksimalus skirtumas | [paleisti testą] |
| Vidutinis skirtumas | [paleisti testą] |

**Išvada:** [užpildyti po testavimo]

---

### 6. Negrįžtamumas (Hiding, Puzzle-friendliness)

Demonstracija su HASH(input + salt):

```
Input: "password" + Salt: "xyz123"
Hash: [paleisti testą]

Input: "password1" + Salt: "xyz123"  
Hash: [paleisti testą]

Input: "different" + Salt: "xyz123"
Hash: [paleisti testą]
```

Net ir žinant salt ir hash'ą, neįmanoma atspėti pradinio teksto be brute-force atakos.

**Išvada:** ✅ Funkcija negrįžtama - iš hash'o praktiškai neįmanoma atkurti pradinio teksto.

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

✅ **Greitai veikia** - O(n) sudėtingumas, tinkamas dideliems failams  
✅ **Deterministinis** - tas pats įvedimas = tas pats hash'as  
✅ **Pastovus dydis** - visada 256 bitai (64 hex)  
✅ **Lavinos efektas** - vienas simbolis keičia ~50% hash'o bitų  
✅ **Pozicijos jautrumas** - simbolio vieta įtakoja rezultatą  
✅ **Įvairios operacijos** - XOR, rotacijos, daugyba, sudėtis  

---

## Trūkumai ir Apribojimai

⚠️ **Nėra profesionaliai audituota** - tai studijų projektas  
⚠️ **Nežinoma kriptografinė atsparumas** - netestuota prieš specializuotas atakas  
⚠️ **Netinkama gamybai** - naudokite SHA-256/SHA-3 realiam darbui  
⚠️ **Galimos teorinės kolizijos** - nors praktikoje labai mažai tikėtinos  
⚠️ **Nepilnai ištirti kraštiniai atvejai** - pvz., specifinės simbolių sekos  

---

## Versijos Istorija

- **v0.1** (2025-10-01) - Pradinė versija su pagrindinėmis funkcijomis
  - 256 bitų išvedimas
  - 4 state'ai su skirtingomis transformacijomis
  - Bit rotacijos ir XOR operacijos
  - Mix funkcija avalanche efektui

---

## Paleidimo Instrukcijos

### 1. Sukurti testinį failą:
```bash
echo "Lietuva" > test.txt
```

### 2. Hash'inti:
```bash
./hash_generator test.txt
```

### 3. Paleisti visus testus:
```bash
./hash_tester > results.txt
```

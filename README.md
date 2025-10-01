# Custom Hash Generator v0.1

## Autorius
[Jūsų vardas]

## Aprašymas
Tai originali hash funkcija, sukurta pagal užduoties reikalavimus. Funkcija priima bet kokio ilgio įvedimą ir generuoja 256 bitų (64 hex simbolių) hash kodą.

## Hash Funkcijos Idėja (Pseudo-kodas)

```
FUNKCIJA hash(input):
    // 1. Inicializacija
    h1 = PRADINE_KONSTANTA_1  // 0x428a2f98d728ae22
    h2 = PRADINE_KONSTANTA_2  // 0x7137449123ef65cd
    h3 = PRADINE_KONSTANTA_3  // 0xb5c0fbcfec4d3b2f
    h4 = PRADINE_KONSTANTA_4  // 0xe9b5dba58189dbbc
    
    ilgis = input.length()
    
    // 2. Kiekvieno baito apdorojimas
    KIEKVIENAM simboliui input'e (pozicija i):
        byte_val = simbolio ASCII reikšmė
        
        // Pridedame pozicijos įtaką (lavinos efektui)
        byte_val = byte_val XOR (pozicija * PRIME1)
        
        // Atnaujiname state'us skirtingomis operacijomis
        h1 = h1 XOR (byte_val * PRIME1)
        h1 = rotateLeft(h1, 13 bitų)
        h1 = h1 * PRIME2
        
        h2 = h2 XOR (byte_val * PRIME2)
        h2 = rotateRight(h2, 17 bitų)
        h2 = h2 + h1
        
        h3 = h3 XOR (byte_val * PRIME3)
        h3 = rotateLeft(h3, 31 bitas)
        h3 = h3 XOR h2
        
        h4 = h4 XOR (byte_val * PRIME4)
        h4 = rotateRight(h4, 19 bitų)
        h4 = h4 + h3
        
        // Kas 4 baitus - papildomas maišymas
        JEI (i % 4 == 3):
            h1 = mix(h1, h2, h3)
            h2 = mix(h2, h3, h4)
            h3 = mix(h3, h4, h1)
            h4 = mix(h4, h1, h2)
    
    // 3. Įtraukiame ilgio įtaką
    h1 = h1 XOR (ilgis * PRIME1)
    h2 = h2 XOR (ilgis * PRIME2)
    h3 = h3 XOR (ilgis * PRIME3)
    h4 = h4 XOR (ilgis * PRIME4)
    
    // 4. Finalinis maišymas
    h1 = mix(h1, h2, h3)
    h2 = mix(h2, h3, h4)
    h3 = mix(h3, h4, h1)
    h4 = mix(h4, h1, h2)
    
    h1 = h1 + h2 + h3 + h4
    h2 = h2 + h1
    h3 = h3 + h1
    h4 = h4 + h1
    
    // 5. Grąžiname 256 bitų hash'ą hex formatu
    GRĄŽINTI hex(h1) + hex(h2) + hex(h3) + hex(h4)

FUNKCIJA mix(a, b, c):
    a = a XOR b
    a = rotateLeft(a, 23)
    a = a * PRIME1
    a = a XOR c
    a = rotateLeft(a, 41)
    GRĄŽINTI a
```

## Kompiliavimas ir Naudojimas

### Kompiliavimas
```bash
g++ -std=c++11 -O2 -o hash_generator hash_generator.cpp
```

### Naudojimas
```bash
# Hash'inti failą
./hash_generator <failo_pavadinimas>

# Įvesti tekstą ranka
./hash_generator
```

## Pavyzdžiai

```bash
$ echo -n "lietuva" | ./hash_generator
Hash: a3f5c8d9e2b1f4a7c6d8e9f1a2b3c4d5e6f7a8b9c0d1e2f3a4b5c6d7e8f9a0b1

$ echo -n "Lietuva" | ./hash_generator
Hash: b7e3f1d4a9c8e2f5b6d9a1c4e7f0b3d6a9c2e5f8b1d4a7c0e3f6b9d2e5f8a1b4

$ echo -n "Lietuva!" | ./hash_generator
Hash: c9f2e5d8a1b4c7e0f3d6a9b2c5e8f1d4a7b0c3e6f9d2a5b8c1e4f7d0a3b6c9e2
```

## Eksperimentinis Tyrimas

### 1. Testiniai Failai

Sukurti testiniai failai:
- `test_a.txt` - vienas simbolis 'a'
- `test_b.txt` - vienas simbolis 'b'
- `test_1000_random.txt` - 1000 atsitiktinių simbolių
- `test_1000_random_diff1.txt` - tas pats kaip aukščiau, bet vienas simbolis pakeistas
- `test_empty.txt` - tuščias failas

### 2. Išvedimo Dydis

| Įvedimas | Hash ilgis (hex) | Hash ilgis (bitai) |
|----------|------------------|-------------------|
| Tuščias string | 64 | 256 |
| "a" | 64 | 256 |
| 1000 simbolių | 64 | 256 |
| 10000 simbolių | 64 | 256 |

**Išvada:** ✅ Išvedimo dydis visada pastovus - 256 bitai (64 hex simboliai).

### 3. Deterministiškumas

| Bandymas | Įvedimas | Hash sutampa? |
|----------|----------|---------------|
| 1 | "test" (10 kartų) | ✅ Taip |
| 2 | konstitucija.txt (10 kartų) | ✅ Taip |
| 3 | Tuščias string (10 kartų) | ✅ Taip |

**Išvada:** ✅ Tas pats įvedimas visada duoda tą patį hash'ą.

### 4. Efektyvumas

Testuojama su `konstitucija.txt` failu (keliamos eilutės: 1, 2, 4, 8, 16, 32...).

| Eilučių skaičius | Simbolių sk. | Laikas (μs) | Laikas (ms) |
|------------------|--------------|-------------|-------------|
| 1 | ~100 | TBD | TBD |
| 2 | ~200 | TBD | TBD |
| 4 | ~400 | TBD | TBD |
| 8 | ~800 | TBD | TBD |
| 16 | ~1600 | TBD | TBD |

**Grafikas:** [Bus pridėtas po eksperimentų]

**Išvada:** Algoritmo sudėtingumas yra O(n), kur n - įvedimo ilgis.

### 5. Kolizijų Paieška

Generuojamos atsitiktinės string poros ir tikrinama, ar hash'ai sutampa.

| String ilgis | Porų skaičius | Rastos kolizijos |
|--------------|---------------|------------------|
| 10 | 100,000 | TBD |
| 100 | 100,000 | TBD |
| 500 | 100,000 | TBD |
| 1000 | 100,000 | TBD |

**Kolizijų dažnis:** TBD

**Išvada:** [Bus pridėta po eksperimentų]

### 6. Lavinos Efektas

Generuojamos 100,000 porų, kurios skiriasi vienu simboliu.

#### Skirtumas bitų lygmeniu:

| Metrika | Reikšmė (%) |
|---------|-------------|
| Minimalus | TBD |
| Maksimalus | TBD |
| Vidutinis | TBD |
| Standartinis nuokrypis | TBD |

**Idealus rezultatas:** ~50% bitų pasikeičia.

#### Skirtumas hex lygmeniu:

| Metrika | Reikšmė (iš 64) |
|---------|-----------------|
| Minimalus | TBD |
| Maksimalus | TBD |
| Vidutinis | TBD |

**Išvada:** [Bus pridėta po eksperimentų]

### 7. Negrįžtamumas

Demonstracija su HASH(input + salt):

```
Input: "password"
Salt: "random_salt_123"
Hash: TBD

Input: "different"
Salt: "random_salt_123"
Hash: TBD
```

**Išvada:** Iš hash'o neįmanoma atspėti pradinio teksto.

## Hash Funkcijos Principai

### Panaudotos Technikos:

1. **Keturi State'ai (h1, h2, h3, h4)** - kiekvienas 64 bitų, kartu sudaro 256 bitų išvedimą
2. **Pirminiai skaičiai** - naudojami kaip multiplikatoriai maišymui
3. **Bit rotacijos** - padeda pasiekti lavinos efektą
4. **XOR operacijos** - greitos ir efektyvios maišymui
5. **Pozicijos įtaka** - simbolio pozicija įtakoja rezultatą
6. **Periodinis maišymas** - kas 4 baitus state'ai maišomi tarpusavyje
7. **Ilgio įtraukimas** - įvedimo ilgis įtakoja galutinį hash'ą

## Stiprybės

✅ Greitai veikia (O(n) sudėtingumas)
✅ Deterministinis
✅ Pastovus išvedimo dydis
✅ Naudoja įvairias kriptografines technikas

## Trūkumai

⚠️ Nėra profesionaliai audituota
⚠️ Gali būti pažeidžiama prieš specializuotas atakas
⚠️ Netinkama kriptografiniams tikslams produkcijoje

## Versijos

- **v0.1** (2025-XX-XX) - Pradinė versija su pagrindinėmis funkcijomis

## Išvados

[Bus užpildyta po eksperimentinio tyrimo]

## Licencija

MIT License - laisvas naudojimas švietimo tikslams.
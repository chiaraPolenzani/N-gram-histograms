# Elaborato OpenMP — Calcolo di N-grammi

Questo progetto in C++ utilizza OpenMP per analizzare grandi collezioni di testi e calcolare **n-grammi** di parole e caratteri (bigrammi, trigrammi, ecc.). 

## 🔧 Requisiti

- CMake
- Compiler compatibile con OpenMP 
- [CLion](https://www.jetbrains.com/clion/) (opzionale, per sviluppo)

## 🛠️ Compilazione

```bash
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
mingw32-make

```

## ▶️ Esecuzione
``` ./elaborato_OpenMP.exe ```

## ⚙️ Esperimenti

Nel file main.cpp è presente una variabile booleana experiment1:

experiment1 = true → Esperimento 1: misura lo speedup in funzione del numero di thread. Il numero di file di testo è fisso.

experiment1 = false → Esperimento 2: mantiene costante il numero di thread e aumenta il numero di file di testo analizzati.

🔁 Il codice utilizza una funzione generica per il calcolo degli n-grammi, quindi è sufficiente cambiare il valore di n per calcolare quadgrammi, 5-grammi, ecc., 
sia per caratteri che per parole.

# N-gram Computation - OpenMP Project

This C++ project uses OpenMP to analyze large collections of text and compute n-grams of words and characters 
(bigrams, trigrams, etc.).

## 🔧 Requirements

- CMake
- Compiler compatible with OpenMP
- [CLion](https://www.jetbrains.com/clion/) (optional, for development)
  
## 🛠️ Build Instructions

```bash
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
mingw32-make

```

## ▶️ Execution
``` ./elaborato_OpenMP.exe ```

## ⚙️ Experiments

In main.cpp, there's a boolean variable named experiment1:

experiment1 = true → Experiment 1: measures speedup as a function of the number of threads. The number of text files is fixed.

experiment1 = false → Experiment 2: keeps the number of threads constant and increases the number of text files analyzed.

🔁 The code uses a generic function to compute n-grams, so you can simply change the value of n 
to compute 4-grams, 5-grams, etc., both for characters and words.



// main.cpp
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <chrono>
#include <unordered_set>
#include <iomanip>

#include "SimpleBloomFilter.h"

using namespace std;

int main() {
    const size_t N_WORDS_TO_INSERT = 500000;
    const double TARGET_FPR = 0.01;

    cout << "--- Benchmark Simple: Filtro de Bloom vs HashSet ---" << endl;
    cout << "Insertando y consultando " << N_WORDS_TO_INSERT << " palabras." << endl;
    cout << "------------------------------------------------------" << endl;

    // Generación de datos
    cout << "Generando datos..." << endl;
    vector<string> words_to_insert;
    vector<string> words_not_in_set;
    words_to_insert.reserve(N_WORDS_TO_INSERT);
    words_not_in_set.reserve(N_WORDS_TO_INSERT);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> length_dist(5, 15);
    for (size_t i = 0; i < N_WORDS_TO_INSERT; ++i) {
        words_to_insert.push_back("palabra_" + to_string(i));
    }
    for (size_t i = 0; i < N_WORDS_TO_INSERT; ++i) {
        words_not_in_set.push_back("test_" + to_string(i));
    }
    cout << "Datos listos." << endl;
    cout << "------------------------------------------------------" << endl;

    // Creación de las Estructuras
    SimpleBloomFilter<string> bf(N_WORDS_TO_INSERT, TARGET_FPR);
    unordered_set<string> hs;
    cout << "Memoria Filtro de Bloom: " << (bf.size_in_bits() / 8.0 / 1024.0) << " KB" << endl;
    cout << "------------------------------------------------------" << endl;

    // Benchmark de Inserción
    cout << "--- FASE 1: Inserción ---" << endl;
    auto start = chrono::high_resolution_clock::now();
    for (const auto& word : words_to_insert) { bf.add(word); }
    auto end = chrono::high_resolution_clock::now();
    cout << "Bloom Filter: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;

    start = chrono::high_resolution_clock::now();
    for (const auto& word : words_to_insert) { hs.insert(word); }
    end = chrono::high_resolution_clock::now();
    cout << "HashSet:      " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;
    cout << "------------------------------------------------------" << endl;

    // Benchmark de Consultas Positivas
    cout << "--- FASE 2: Consultas Positivas ---" << endl;
    size_t bf_found = 0, hs_found = 0;
    start = chrono::high_resolution_clock::now();
    for (const auto& word : words_to_insert) { if (bf.contains(word)) bf_found++; }
    end = chrono::high_resolution_clock::now();
    cout << "Bloom Filter: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms (Encontrados: " << bf_found << "/" << N_WORDS_TO_INSERT << ")" << endl;

    start = chrono::high_resolution_clock::now();
    for (const auto& word : words_to_insert) { if (hs.count(word)) hs_found++; }
    end = chrono::high_resolution_clock::now();
    cout << "HashSet:      " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms (Encontrados: " << hs_found << "/" << N_WORDS_TO_INSERT << ")" << endl;
    cout << "------------------------------------------------------" << endl;

    // Benchmark de Consultas Negativas
    cout << "--- FASE 3: Consultas Negativas ---" << endl;
    size_t bf_false_positives = 0, hs_false_positives = 0;
    start = chrono::high_resolution_clock::now();
    for (const auto& word : words_not_in_set) { if (bf.contains(word)) bf_false_positives++; }
    end = chrono::high_resolution_clock::now();
    double actual_fpr = static_cast<double>(bf_false_positives) / N_WORDS_TO_INSERT;
    cout << "Bloom Filter: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms (Falsos Positivos: " << bf_false_positives << ", Tasa: " << fixed << setprecision(4) << actual_fpr * 100.0 << "%)" << endl;

    start = chrono::high_resolution_clock::now();
    for (const auto& word : words_not_in_set) { if (hs.count(word)) hs_false_positives++; }
    end = chrono::high_resolution_clock::now();
    cout << "HashSet:      " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms (Falsos Positivos: " << hs_false_positives << ")" << endl;
    cout << "------------------------------------------------------" << endl;

    return 0;
}

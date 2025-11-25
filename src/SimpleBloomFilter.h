// SimpleBloomFilter.h

#ifndef SIMPLE_BLOOM_FILTER_H
#define SIMPLE_BLOOM_FILTER_H

#include <vector>
#include <string>
#include <cstdint> // Para uint64_t
#include <cmath>   // Para std::log, std::ceil

// Declaramos la función hash aquí para que la clase sepa que existe
uint64_t MurmurHash2(const void* key, int len, uint64_t seed);

template <typename T>
class SimpleBloomFilter {
public:
    // Constructor
    SimpleBloomFilter(size_t capacity, double error_rate);

    // Métodos públicos para usar el filtro
    void add(const T& item);
    bool contains(const T& item) const;

    // Método "getter" para obtener el tamaño
    size_t size_in_bits() const;

private:
    // Variables miembro privadas
    size_t m_size;
    size_t m_hash_count;
    std::vector<uint64_t> m_bits;

    // Métodos auxiliares privados
    std::pair<const char*, size_t> get_bytes(const std::string& item) const;
    template <typename U> std::pair<const char*, size_t> get_bytes(const U& item) const;
    void set_bit(size_t index);
    bool get_bit(size_t index) const;
};

#endif // SIMPLE_BLOOM_FILTER_H

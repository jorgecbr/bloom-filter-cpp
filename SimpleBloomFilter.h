// SimpleBloomFilter.h

#ifndef SIMPLE_BLOOM_FILTER_H
#define SIMPLE_BLOOM_FILTER_H

#include <vector>
#include <string>
#include <cstdint>
#include <cmath>
#include <type_traits>

// --- Implementación de la función hash (está bien aquí) ---
uint64_t MurmurHash2(const void* key, int len, uint64_t seed) {
    const uint64_t m = 0xc6a4a7935bd1e995; const int r = 47;
    uint64_t h = seed ^ (len * m);
    const uint64_t* data = (const uint64_t*)key; const uint64_t* end = data + (len / 8);
    while (data != end) { uint64_t k = *data++; k *= m; k ^= k >> r; k *= m; h ^= k; h *= m; }
    const unsigned char* data2 = (const unsigned char*)data;
    switch (len & 7) {
        case 7: h ^= ((uint64_t)data2[6]) << 48;
        case 6: h ^= ((uint64_t)data2[5]) << 40;
        case 5: h ^= ((uint64_t)data2[4]) << 32;
        case 4: h ^= ((uint64_t)data2[3]) << 24;
        case 3: h ^= ((uint64_t)data2[2]) << 16;
        case 2: h ^= ((uint64_t)data2[1]) << 8;
        case 1: h ^= ((uint64_t)data2[0]); h *= m;
    }
    h ^= h >> r; h *= m; h ^= h >> r; return h;
}

// --- Declaración e Implementación de la clase (todo junto) ---
template <typename T>
class SimpleBloomFilter {
public:
    // Constructor
    SimpleBloomFilter(size_t capacity, double error_rate) {
        double size_d = -(static_cast<double>(capacity) * std::log(error_rate)) / (std::log(2) * std::log(2));
        double hash_count_d = (size_d / capacity) * std::log(2);
        m_size = static_cast<size_t>(std::ceil(size_d));
        m_hash_count = static_cast<size_t>(std::ceil(hash_count_d));
        m_bits.resize((m_size + 63) / 64, 0);
    }

    // Métodos públicos
    void add(const T& item) {
        auto data = get_bytes(item);
        uint64_t h1 = MurmurHash2(data.first, data.second, 0);
        uint64_t h2 = MurmurHash2(data.first, data.second, h1);
        for (size_t i = 0; i < m_hash_count; ++i) { set_bit((h1 + i * h2) % m_size); }
    }

    bool contains(const T& item) const {
        auto data = get_bytes(item);
        uint64_t h1 = MurmurHash2(data.first, data.second, 0);
        uint64_t h2 = MurmurHash2(data.first, data.second, h1);
        for (size_t i = 0; i < m_hash_count; ++i) { if (!get_bit((h1 + i * h2) % m_size)) return false; }
        return true;
    }

    size_t size_in_bits() const { return m_size; }

private:
    // Variables miembro
    size_t m_size;
    size_t m_hash_count;
    std::vector<uint64_t> m_bits;

    // Métodos auxiliares (implementados aquí mismo)
    std::pair<const char*, size_t> get_bytes(const T& item) const {
        if constexpr (std::is_same_v<T, std::string>) {
            return {item.data(), item.size()};
        } else {
            return {reinterpret_cast<const char*>(&item), sizeof(T)};
        }
    }

    void set_bit(size_t index) { m_bits[index / 64] |= (1ULL << (index % 64)); }
    bool get_bit(size_t index) const { return (m_bits[index / 64] & (1ULL << (index % 64))) != 0; }
};

#endif // SIMPLE_BLOOM_FILTER_H

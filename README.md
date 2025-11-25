# Filtro de Bloom en C++

Una implementación simple y eficiente de un Filtro de Bloom en C++, junto con un benchmark que compara su rendimiento y uso de memoria contra un `std::unordered_set`.

## ¿Qué es un Filtro de Bloom?

Un Filtro de Bloom es una estructura de datos probabilística y compacta diseñada para verificar de manera muy eficiente si un elemento pertenece a un conjunto.

### Características Clave

- **No tiene falsos negativos**: Si el filtro dice que un elemento **NO está** en el conjunto, puedes estar 100% seguro de que así es.
- **Puede tener falsos positivos**: Si el filtro dice que un elemento **SÍ está** en el conjunto, hay una pequeña probabilidad de que se equivoque.

En resumen: te da una respuesta "definitivamente no" o "probablemente sí".

## La Matemática detrás del Filtro

La belleza del Filtro de Bloom es que podemos calcular el tamaño óptimo del array de bits (`m`) y el número óptimo de funciones hash (`k`) basándonos en:
- `n`: Número de elementos esperados.
- `p`: Tasa de falsos positivos deseada.

Las fórmulas implementadas en este proyecto son:

- **Tamaño del arreglo de bits (`m`)**:
  `m = - (n * ln(p)) / (ln(2)^2)`
- **Número de funciones hash (`k`)**:
  `k = (m/n) * ln(2)`

## Estructura del Proyecto

El proyecto consta de dos archivos principales:

1.  **`SimpleBloomFilter.h`**: Contiene la implementación de la plantilla de clase `SimpleBloomFilter`.
    - Utiliza `MurmurHash2` como función hash.
    - Almacena los bits en un `std::vector<uint64_t>` para mayor eficiencia.
    - Emplea la técnica de "doble hash" para generar múltiples índices de hash.
2.  **`main.cpp`**: Un programa de benchmark que:
    - Crea un Filtro de Bloom y un `std::unordered_set`.
    - Mide el tiempo de inserción de 500,000 elementos.
    - Mide el tiempo de consultas positivas (elementos que están en el conjunto).
    - Mide el tiempo de consultas negativas (elementos que no están) y calcula la tasa de falsos positivos real del filtro.

## Cómo Compilar y Ejecutar

Puedes compilar el proyecto con un compilador de C++17 o superior (se usa `if constexpr`).

```bash
g++ -std=c++17 -O3 main.cpp -o bloom_benchmark
```

Luego, ejecuta el binario:

```bash
./bloom_benchmark
```

## Resultados Esperados

Al ejecutar el benchmark, deberías ver una salida similar a esta:

```
--- Benchmark Simple: Filtro de Bloom vs HashSet ---
Insertando y consultando 500000 palabras.
------------------------------------------------------
Generando datos...
Datos listos.
------------------------------------------------------
Memoria Filtro de Bloom: 599.984 KB
------------------------------------------------------
--- FASE 1: Inserción ---
Bloom Filter: 45 ms
HashSet:      120 ms
------------------------------------------------------
--- FASE 2: Consultas Positivas ---
Bloom Filter: 35 ms (Encontrados: 500000/500000)
HashSet:      50 ms (Encontrados: 500000/500000)
------------------------------------------------------
--- FASE 3: Consultas Negativas ---
Bloom Filter: 40 ms (Falsos Positivos: 5089, Tasa: 1.0178%)
HashSet:      55 ms (Falsos Positivos: 0)
------------------------------------------------------
```

Esto demuestra que el Filtro de Bloom es significativamente más rápido en inserciones y consultas, y que su tasa de falsos positivos real coincide con la tasa objetivo (`TARGET_FPR`).

También se deja un ARTÍCULO por si es de su interés profundizar en el tema.

Cualquier duda o inquietud puede escribir al gmail: jorgecarlosburgos4@gmail.com

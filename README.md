# Tabla Hash en C

Implementación independiente de una tabla hash en C utilizando listas enlazadas para manejar las colisiones.

## Características

* Implementación completamente independiente en un solo archivo `.c`.
* No requiere archivos `.h` externos.
* Función hash FNV-1a de 64 bits.
* Manejo de colisiones mediante listas enlazadas.
* Inserción de elementos.
* Búsqueda de elementos.
* Eliminación de elementos.
* Actualización del valor de una clave existente.
* Redimensionamiento automático de la tabla.
* Factor de carga máximo aproximado del 75%.
* Liberación de memoria.
* Interfaz mediante menú interactivo en la terminal.

## Estructura del proyecto

```text
hashtable-c/
├── hashtable.c
├── README.md
└── .gitignore
```

## Compilación

Para compilar el programa utilizando GCC:

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic hashtable.c -o hashtable
```

## Ejecución

Después de compilar:

```bash
./hashtable
```

## Funcionamiento

El programa implementa una tabla hash que almacena pares:

```text
clave → valor
```

Las claves son cadenas de caracteres y los valores son números enteros.

Por ejemplo:

```text
Gabriel → 100
Carlos  → 250
Maria   → 500
```

Las colisiones entre claves se manejan mediante listas enlazadas.

La tabla comienza con una capacidad inicial de 8 posiciones y aumenta automáticamente su capacidad cuando el factor de carga supera el límite establecido.

## Operaciones disponibles

El programa permite realizar las siguientes operaciones:

1. Insertar un elemento.
2. Buscar un elemento.
3. Eliminar un elemento.
4. Mostrar la tabla.
5. Mostrar la cantidad de elementos.
6. Salir del programa.

## Ejemplo

Al ejecutar el programa:

```text
========================================
          TABLA HASH EN C
========================================

1. Insertar elemento
2. Buscar elemento
3. Eliminar elemento
4. Mostrar tabla
5. Mostrar cantidad de elementos
0. Salir
```

El usuario puede introducir una clave y un valor:

```text
Clave: Gabriel
Valor: 100
```

Posteriormente puede buscar:

```text
Clave: Gabriel
```

Y obtener:

```text
Clave encontrada: Gabriel
Valor: 100
```

## Tecnologías utilizadas

* Lenguaje C
* GCC
* Git
* GitHub
* Listas enlazadas
* Tablas hash
* Función hash FNV-1a

## Autor

Gabriel

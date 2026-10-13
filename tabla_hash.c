
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANIO 10
#define TAM_CLAVE 100

// Nodo para la lista enlazada (manejo de colisiones)
typedef struct HashNode {
    char *clave;
    int valor;
    struct HashNode *siguiente;
} HashNode;

// Estructura de la tabla hash
typedef struct {
    HashNode *tabla[TAMANIO];
} HashTable;

// Inicializar la tabla hash
void hash_inicializar(HashTable *ht) {
    for (int i = 0; i < TAMANIO; i++) {
        ht->tabla[i] = NULL;
    }
}

// Funcion hash para cadenas de caracteres
unsigned int funcion_hash(const char *clave) {
    unsigned int hash = 0;

    while (*clave) {
        hash = (hash * 31) + (unsigned char)*clave;
        clave++;
    }

    return hash % TAMANIO;
}

// Insertar un elemento o actualizar su valor
int hash_insertar(HashTable *ht, const char *clave, int valor) {
    unsigned int indice = funcion_hash(clave);
    HashNode *actual = ht->tabla[indice];

    // Verificar si la clave ya existe
    while (actual != NULL) {
        if (strcmp(actual->clave, clave) == 0) {
            actual->valor = valor;
            return 1;
        }

        actual = actual->siguiente;
    }

    // Crear un nuevo nodo
    HashNode *nuevo = malloc(sizeof(HashNode));

    if (nuevo == NULL) {
        printf("Error al reservar memoria para el nodo.\n");
        return 0;
    }

    // Reservar memoria para la clave (sin utilizar strdup)
    nuevo->clave = malloc(strlen(clave) + 1);

    if (nuevo->clave == NULL) {
        free(nuevo);
        printf("Error al reservar memoria para la clave.\n");
        return 0;
    }

    // Copiar la clave a la memoria reservada
    strcpy(nuevo->clave, clave);

    nuevo->valor = valor;

    // Insertar al principio de la lista enlazada
    nuevo->siguiente = ht->tabla[indice];
    ht->tabla[indice] = nuevo;

    return 1;
}

// Buscar un elemento
int hash_buscar(HashTable *ht, const char *clave,
                int *encontrado) {
    unsigned int indice = funcion_hash(clave);
    HashNode *actual = ht->tabla[indice];

    while (actual != NULL) {
        if (strcmp(actual->clave, clave) == 0) {
            *encontrado = 1;
            return actual->valor;
        }

        actual = actual->siguiente;
    }

    *encontrado = 0;
    return -1;
}

// Borrar una clave especifica de la tabla
int hash_borrar(HashTable *ht, const char *clave) {
    unsigned int indice = funcion_hash(clave);
    HashNode *actual = ht->tabla[indice];
    HashNode *anterior = NULL;

    while (actual != NULL) {
        if (strcmp(actual->clave, clave) == 0) {
            // Si el nodo es el primero de la lista
            if (anterior == NULL) {
                ht->tabla[indice] = actual->siguiente;
            } else {
                anterior->siguiente = actual->siguiente;
            }

            // Liberar memoria
            free(actual->clave);
            free(actual);

            return 1;
        }

        anterior = actual;
        actual = actual->siguiente;
    }

    return 0;
}

// Mostrar todos los elementos de la tabla
void hash_mostrar(HashTable *ht) {
    printf("\n========== TABLA HASH ==========\n");

    for (int i = 0; i < TAMANIO; i++) {
        HashNode *actual = ht->tabla[i];

        printf("[%d] -> ", i);

        while (actual != NULL) {
            printf("(%s, %d) -> ",
                   actual->clave, actual->valor);

            actual = actual->siguiente;
        }

        printf("NULL\n");
    }

    printf("================================\n");
}

// Liberar toda la memoria de la tabla hash
void hash_destruir(HashTable *ht) {
    for (int i = 0; i < TAMANIO; i++) {
        HashNode *actual = ht->tabla[i];

        while (actual != NULL) {
            HashNode *temporal = actual;
            actual = actual->siguiente;

            free(temporal->clave);
            free(temporal);
        }

        ht->tabla[i] = NULL;
    }
}

int main(void) {
    HashTable HT;
    int opcion;
    int valor;
    int encontrado;
    char clave[TAM_CLAVE];

    // Inicializar la tabla antes de utilizarla
    hash_inicializar(&HT);

    do {
        printf("\n====== IMPLEMENTACION HASH ======\n");
        printf("1. Insertar elemento\n");
        printf("2. Buscar elemento\n");
        printf("3. Borrar elemento\n");
        printf("4. Mostrar tabla\n");
        printf("0. Salir\n");
        printf("Selecciona una opcion: ");

        if (scanf("%d", &opcion) != 1) {
            printf("Entrada no valida.\n");
            break;
        }

        switch (opcion) {
            case 1:
                printf("Introduce la clave: ");

                if (scanf("%99s", clave) != 1) {
                    goto finalizar;
                }

                printf("Introduce el valor entero: ");

                if (scanf("%d", &valor) != 1) {
                    printf("Valor no valido.\n");
                    goto finalizar;
                }

                if (hash_insertar(&HT, clave, valor)) {
                    printf("Elemento insertado o actualizado.\n");
                }
                break;

            case 2:
                printf("Clave a buscar: ");

                if (scanf("%99s", clave) != 1) {
                    goto finalizar;
                }

                valor = hash_buscar(&HT, clave, &encontrado);

                if (encontrado) {
                    printf("Clave: %s, valor: %d\n",
                           clave, valor);
                } else {
                    printf("Clave no encontrada.\n");
                }
                break;

            case 3:
                printf("Clave a borrar: ");

                if (scanf("%99s", clave) != 1) {
                    goto finalizar;
                }

                if (hash_borrar(&HT, clave)) {
                    printf("Elemento borrado correctamente.\n");
                } else {
                    printf("Clave no encontrada.\n");
                }
                break;

            case 4:
                hash_mostrar(&HT);
                break;

            case 0:
                printf("Finalizando programa...\n");
                break;

            default:
                printf("Opcion no valida.\n");
        }

    } while (opcion != 0);

finalizar:
    // Liberar la memoria antes de terminar
    hash_destruir(&HT);

    return 0;
}

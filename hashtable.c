#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAPACIDAD_INICIAL 8u
#define CARGA_NUM 3u
#define CARGA_DEN 4u
#define MAX_CLAVE 100

typedef struct Nodo {
    char *clave;
    size_t hash;
    int valor;
    struct Nodo *siguiente;
} Nodo;

typedef struct {
    Nodo **cubetas;
    size_t capacidad;
    size_t tamano;
} TablaHash;

static size_t hash_cadena(const char *s)
{
    uint64_t h = 14695981039346656037ULL;
    for (; *s; s++) {
        h ^= (unsigned char)*s;
        h *= 1099511628211ULL;
    }
    return (size_t)h;
}

static size_t indice(size_t hash, size_t capacidad)
{
    return hash & (capacidad - 1);
}

static char *copiar_cadena(const char *s)
{
    size_t n = strlen(s) + 1;
    char *copia = malloc(n);
    if (copia)
        memcpy(copia, s, n);
    return copia;
}

static Nodo *buscar_nodo(const TablaHash *t, const char *clave, size_t h)
{
    size_t idx = indice(h, t->capacidad);
    for (Nodo *n = t->cubetas[idx]; n; n = n->siguiente)
        if (n->hash == h && strcmp(n->clave, clave) == 0)
            return n;
    return NULL;
}

static int redimensionar(TablaHash *t)
{
    if (t->capacidad > SIZE_MAX / (2 * sizeof(Nodo *)))
        return 0;

    size_t nueva_capacidad = t->capacidad * 2;
    Nodo **nuevas = calloc(nueva_capacidad, sizeof(Nodo *));
    if (!nuevas)
        return 0;

    for (size_t i = 0; i < t->capacidad; i++) {
        Nodo *n = t->cubetas[i];
        while (n) {
            Nodo *siguiente = n->siguiente;
            size_t idx = indice(n->hash, nueva_capacidad);
            n->siguiente = nuevas[idx];
            nuevas[idx] = n;
            n = siguiente;
        }
    }

    free(t->cubetas);
    t->cubetas = nuevas;
    t->capacidad = nueva_capacidad;
    return 1;
}

static TablaHash *crear_tabla(void)
{
    TablaHash *t = malloc(sizeof(*t));
    if (!t)
        return NULL;

    t->capacidad = CAPACIDAD_INICIAL;
    t->tamano = 0;
    t->cubetas = calloc(t->capacidad, sizeof(Nodo *));

    if (!t->cubetas) {
        free(t);
        return NULL;
    }
    return t;
}

static int insertar(TablaHash *t, const char *clave, int valor)
{
    if (!t || !clave || clave[0] == '\0')
        return 0;

    size_t h = hash_cadena(clave);
    Nodo *existente = buscar_nodo(t, clave, h);

    if (existente) {
        existente->valor = valor;
        return 1;
    }

    if ((t->tamano + 1) * CARGA_DEN > t->capacidad * CARGA_NUM)
        redimensionar(t);

    Nodo *nuevo = malloc(sizeof(*nuevo));
    if (!nuevo)
        return 0;

    nuevo->clave = copiar_cadena(clave);
    if (!nuevo->clave) {
        free(nuevo);
        return 0;
    }

    nuevo->hash = h;
    nuevo->valor = valor;

    size_t idx = indice(h, t->capacidad);
    nuevo->siguiente = t->cubetas[idx];
    t->cubetas[idx] = nuevo;
    t->tamano++;

    return 1;
}

static int buscar(const TablaHash *t, const char *clave, int *valor)
{
    if (!t || !clave)
        return 0;

    Nodo *n = buscar_nodo(t, clave, hash_cadena(clave));
    if (!n)
        return 0;

    if (valor)
        *valor = n->valor;
    return 1;
}

static int eliminar(TablaHash *t, const char *clave)
{
    if (!t || !clave)
        return 0;

    size_t h = hash_cadena(clave);
    size_t idx = indice(h, t->capacidad);
    Nodo *anterior = NULL;
    Nodo *actual = t->cubetas[idx];

    while (actual) {
        if (actual->hash == h && strcmp(actual->clave, clave) == 0) {
            if (anterior)
                anterior->siguiente = actual->siguiente;
            else
                t->cubetas[idx] = actual->siguiente;

            free(actual->clave);
            free(actual);
            t->tamano--;
            return 1;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
    return 0;
}

static void imprimir_tabla(const TablaHash *t)
{
    if (!t)
        return;

    printf("\n========================================\n");
    printf("             TABLA HASH\n");
    printf("========================================\n");
    printf("Elementos : %zu\n", t->tamano);
    printf("Capacidad : %zu\n", t->capacidad);
    printf("========================================\n");

    for (size_t i = 0; i < t->capacidad; i++) {
        printf("[%2zu] ", i);
        Nodo *n = t->cubetas[i];

        if (!n) {
            printf("NULL");
        } else {
            while (n) {
                printf("(%s : %d)", n->clave, n->valor);
                if (n->siguiente)
                    printf(" -> ");
                n = n->siguiente;
            }
        }
        printf("\n");
    }
    printf("========================================\n");
}

static void destruir_tabla(TablaHash *t)
{
    if (!t)
        return;

    for (size_t i = 0; i < t->capacidad; i++) {
        Nodo *n = t->cubetas[i];
        while (n) {
            Nodo *siguiente = n->siguiente;
            free(n->clave);
            free(n);
            n = siguiente;
        }
    }

    free(t->cubetas);
    free(t);
}

static void limpiar_buffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

int main(void)
{
    TablaHash *tabla = crear_tabla();

    if (!tabla) {
        fprintf(stderr, "Error: no se pudo crear la tabla hash.\n");
        return EXIT_FAILURE;
    }

    int opcion;

    do {
        printf("\n========================================\n");
        printf("          TABLA HASH EN C\n");
        printf("========================================\n");
        printf("1. Insertar elemento\n");
        printf("2. Buscar elemento\n");
        printf("3. Eliminar elemento\n");
        printf("4. Mostrar tabla\n");
        printf("5. Mostrar cantidad de elementos\n");
        printf("0. Salir\n");
        printf("========================================\n");
        printf("Seleccione una opcion: ");

        if (scanf("%d", &opcion) != 1) {
            printf("Entrada invalida.\n");
            limpiar_buffer();
            continue;
        }

        limpiar_buffer();

        switch (opcion) {
        case 1: {
            char clave[MAX_CLAVE];
            int valor;

            printf("\nIngrese la clave: ");
            if (!fgets(clave, sizeof(clave), stdin))
                break;

            clave[strcspn(clave, "\n")] = '\0';

            printf("Ingrese el valor entero: ");
            if (scanf("%d", &valor) != 1) {
                printf("Valor invalido.\n");
                limpiar_buffer();
                break;
            }
            limpiar_buffer();

            if (insertar(tabla, clave, valor))
                printf("Elemento insertado/actualizado correctamente.\n");
            else
                printf("Error al insertar el elemento.\n");
            break;
        }

        case 2: {
            char clave[MAX_CLAVE];
            int valor;

            printf("\nIngrese la clave que desea buscar: ");
            if (!fgets(clave, sizeof(clave), stdin))
                break;

            clave[strcspn(clave, "\n")] = '\0';

            if (buscar(tabla, clave, &valor)) {
                printf("\nElemento encontrado.\n");
                printf("Clave : %s\n", clave);
                printf("Valor : %d\n", valor);
            } else {
                printf("\nLa clave \"%s\" no se encuentra en la tabla.\n", clave);
            }
            break;
        }

        case 3: {
            char clave[MAX_CLAVE];

            printf("\nIngrese la clave que desea eliminar: ");
            if (!fgets(clave, sizeof(clave), stdin))
                break;

            clave[strcspn(clave, "\n")] = '\0';

            if (eliminar(tabla, clave))
                printf("Elemento eliminado correctamente.\n");
            else
                printf("La clave \"%s\" no se encuentra en la tabla.\n", clave);
            break;
        }

        case 4:
            imprimir_tabla(tabla);
            break;

        case 5:
            printf("\nCantidad de elementos: %zu\n", tabla->tamano);
            printf("Capacidad actual: %zu\n", tabla->capacidad);
            break;

        case 0:
            printf("\nSaliendo del programa...\n");
            break;

        default:
            printf("\nOpcion invalida.\n");
        }

    } while (opcion != 0);

    destruir_tabla(tabla);
    return EXIT_SUCCESS;
}

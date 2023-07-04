#include "ptree.h"
#include "../Cola/cola.c"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define PTREE_TAMANIO_DICCIONARIO 256

typedef struct _PtreeNode
{
    int estaLaPalabra;
    int profundidad;
    struct _PtreeNode *hijos[PTREE_TAMANIO_DICCIONARIO];
    struct _PtreeNode *padre;
    struct _PtreeNode *link_sufijo;
    int valor_arista_incidente;
} PtreeNode;

Ptree ptree_crear_nodo(Ptree padre, char valor_arista_incidente)
{
    Ptree t = (Ptree)malloc(sizeof(PtreeNode));
    assert(t != NULL);
    t->estaLaPalabra = 0;
    t->padre = padre;
    t->link_sufijo = NULL;
    t->valor_arista_incidente = valor_arista_incidente;
    t->profundidad = (padre == NULL) ? 0 : padre->profundidad + 1;
    for (int i = 0; i < PTREE_TAMANIO_DICCIONARIO; i++)
        t->hijos[i] = NULL;
    return t;
}

Ptree ptree_crear()
{
    return ptree_crear_nodo(NULL, '\0');
}

void ptree_destruir(Ptree root)
{
    for (int i = 0; i < PTREE_TAMANIO_DICCIONARIO; i++)
        if (ptree_link_disponible(root, i))
            ptree_destruir(root->hijos[i]);
    free(root);
}

Ptree ptree_crear_de_archivo(char *filename)
{
    FILE *arch = fopen(filename, "r");
    if (arch == NULL)
        exit(1);
    Ptree root = ptree_crear();
    PtreeNode *ptr = root;
    while (!feof(arch))
    {
        char c = fgetc(arch);
        if (c == '\n' || c == EOF)
        {
            ptr->estaLaPalabra = 1;
            ptr = root;
        }
        else
        {
            int valor_arista_hijo = (int)c;
            if (!ptree_link_disponible(ptr, valor_arista_hijo))
                ptr->hijos[valor_arista_hijo] = ptree_crear_nodo(ptr, c);
            ptr = ptr->hijos[valor_arista_hijo];
        }
    }
    fclose(arch);
    return root;
}

int ptree_link_disponible(Ptree tree, int i)
{
    // printf("%c ",i);
    assert(0 <= i && i < PTREE_TAMANIO_DICCIONARIO);
    return (tree->hijos[i]) != NULL;
}

Ptree ptree_insertar(Ptree root, char *string)
{
    if (string != NULL)
    {
        if (string[0] == '\0')
            root->estaLaPalabra = 1;
        else
        {
            int valor_arista_hijo = (int)string[0];
            if (!ptree_link_disponible(root, valor_arista_hijo))
                root->hijos[valor_arista_hijo] = ptree_crear_nodo(root, string[0]);
            root->hijos[valor_arista_hijo] = ptree_insertar(root->hijos[valor_arista_hijo], string + 1);
        }
    }
    return root;
}

Ptree ptree_calcular_link_sufijo(Ptree node, Ptree root)
{
    // Calcula el link de sufijo del nodo
    if (node == root || node->padre == root || node->estaLaPalabra)
        node->link_sufijo = root;
    else
    {
        Ptree node_ptr = node->padre->link_sufijo;
        int valor_arista_incidente = (int)node->valor_arista_incidente;

        while (node_ptr != root && !ptree_link_disponible(node_ptr, valor_arista_incidente))
            node_ptr = node_ptr->link_sufijo;

        if (ptree_link_disponible(node_ptr, valor_arista_incidente))
            node->link_sufijo = node_ptr->hijos[valor_arista_incidente];
        else
            node->link_sufijo = root;
    }
    return node;
}

void *copy_id(void *dato)
{
    return dato;
}

void void_destroy(void *dato)
{
    dato = dato;
}

Ptree ptree_actualizar_links_sufijos(Ptree root)
{
    Cola c = cola_crear(copy_id, void_destroy);
    c = cola_push(c, root);
    while (!cola_empty(c))
    {
        Ptree node = (PtreeNode *)cola_front(c);
        assert(node != NULL);
        c = cola_pop(c);

        node = ptree_calcular_link_sufijo(node, root);

        for (int i = 0; i < PTREE_TAMANIO_DICCIONARIO; i++)
            if (ptree_link_disponible(node, i))
                c = cola_push(c, node->hijos[i]);
    }
    return root;
}

void printNode(PtreeNode *n)
{
    // assert(n != NULL);
    if (n->valor_arista_incidente == EOF)
        printf("EOF");
    else if (n == NULL || n->valor_arista_incidente == '\0')
        printf("\\0");
    else
        printf("%c", n->valor_arista_incidente);
}

void ptree_preorder(Ptree t)
{
    printf("(");
    printNode(t);
    printf(" ");
    printNode(t->link_sufijo);
    printf(" %d ) ", t->estaLaPalabra);
    for (int i = 0; i < PTREE_TAMANIO_DICCIONARIO; i++)
        if (ptree_link_disponible(t, i))
            ptree_preorder(t->hijos[i]);
}

Ptree ptree_follow(Ptree t, char c)
{
    PtreeNode *node = t;
    while (node->padre != NULL && !ptree_link_disponible(node, c) /*!node->linkEnabled[(int)c]*/)
        node = node->link_sufijo;
    if (ptree_link_disponible(node, c))
        node = node->hijos[(int)c];
    return node;
}

int positivo(int x)
{
    return x < 0 ? 0 : x;
}

char *process(Ptree t, char *str)
{
    PtreeNode *node = t;
    int l = 0;
    int indice_ultima_palabra_encontrada = -1;
    for (int i = 0; str[i] != '\0'; i++)
    {
        //printf("%d ", node->estaLaPalabra);
        if (node->estaLaPalabra)
            indice_ultima_palabra_encontrada = i - 1;
        if (!ptree_link_disponible(node, str[i]) && indice_ultima_palabra_encontrada != -1)
        {
            for (int j = l; j <= indice_ultima_palabra_encontrada; j++)
                putchar(str[j]);
            putchar(' ');
            indice_ultima_palabra_encontrada = -1;
        }
        int prev_profundidad = node->profundidad;
        node = ptree_follow(node, str[i]);
        l = l + positivo(prev_profundidad - node->profundidad + 1);
        //printf("((%d %c) (%d %c))\n", l, str[l], i, str[i]);
        // printf("(%c,",str[i]);
        // printNode(node);
        // printf(") ");
    }
    for (int j = l; (indice_ultima_palabra_encontrada != -1 && j <= indice_ultima_palabra_encontrada) || (node->estaLaPalabra && str[j] != '\0'); j++)
        putchar(str[j]);
    putchar(' ');
    return NULL;
}
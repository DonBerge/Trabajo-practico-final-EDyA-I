#include "./Estructuras/Ptree/ptree.c"
#include <stdio.h>

int main()
{
    char* s = "quienrecibiradolaresyrecibos";
    Ptree t = ptree_crear_de_archivo("diccionario.txt");
    t = ptree_actualizar_links_sufijos(t);
    //ptree_preorder(t);
    putchar('\n');
    process(t,s);
    ptree_destruir(t);
    return 0;
}
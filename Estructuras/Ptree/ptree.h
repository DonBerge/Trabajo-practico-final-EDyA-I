typedef struct _PtreeNode* Ptree;

Ptree ptree_crear();
Ptree ptree_crear_de_archivo(char* nombre_archivo);
void ptree_destruir(Ptree);
Ptree ptree_insertar(Ptree, char*);
Ptree ptree_calcular_nodo_sufijo(Ptree);
Ptree ptree_actualizar_links_sufijos(Ptree);
int ptree_link_disponible(Ptree, int);
Ptree ptree_seguir_link(Ptree, int);
Ptree ptree_valor_arista_incidente(Ptree);
void procesar_string(Ptree, char*);
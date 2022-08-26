#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_VERTICES 100

typedef struct s_matriz{
	int *matriz;
	int total_vertices;
	char *nombre_vertices;
} MATRIZ_ADYACENCIA;

typedef struct s_nodo{
	char vertice;
	struct s_nodo *siguiente;
} *NODO_VERTICE;

typedef struct s_elemento{
	char vertice;
	struct s_nodo *conectado;
	struct s_elemento *siguiente;
} *LISTA_ADYACENCIA;

/* Prototipo de funciones para trabajar con la matriz de adyacencia */
MATRIZ_ADYACENCIA crearMatrizGrafo(char *);
void verMatrizGrafo(MATRIZ_ADYACENCIA);
MATRIZ_ADYACENCIA cargaAristas(MATRIZ_ADYACENCIA);
int getIndice(char *, char);

/* Prototipo de funciones para trabajar con la lista de adyacencia */
LISTA_ADYACENCIA crearListaAdyacencia(char *);
void verListaAdyacencia(LISTA_ADYACENCIA);
void agregarAristas(LISTA_ADYACENCIA);
void agregaVertice(LISTA_ADYACENCIA, char, char);

/* Funcion principal */
int main(){
	MATRIZ_ADYACENCIA gx;

	// Abro el archivo con los indicadores de las puertas y guardo la cadena de texto con los vertices del grafo
	FILE *fpuertas = fopen("puertas.txt", "r");
	int n = 5;
	char puertas[n];
	
	// Forma (1)
	fscanf(fpuertas, "%s", puertas);
	fclose(fpuertas);
	printf("%s\n", puertas);
	
	// Forma (2)
	//strcpy(puertas,"abcde");

	/* Usando una matri< de adyacencia */
	gx = crearMatrizGrafo(puertas);
	//printf("Matriz creada!!\n");
	//verMatrizGrafo(gx);
	gx = cargaAristas(gx);
	printf("Aristas cargadas!!\n");
	verMatrizGrafo(gx);

	/* Ahora se usa la lista de adyacencia */
	LISTA_ADYACENCIA gy = crearListaAdyacencia(puertas);
	agregarAristas(gy);
	verListaAdyacencia(gy);
	
	return 0;
}

/*
	@objetivo. Crear la matriz de adyacencia
	@parametros
		- vertices: lista con nombres de los vertices
	@return: matriz de adyacencia del grafo
*/
MATRIZ_ADYACENCIA crearMatrizGrafo(char *vertices){
	MATRIZ_ADYACENCIA m;
	int i, j, total_vertices = strlen(vertices); 

	m.nombre_vertices = (char *) malloc(sizeof(char)*total_vertices);
	strcpy(m.nombre_vertices, vertices);
	m.total_vertices = total_vertices;	
	m.matriz = (int *) malloc(sizeof(int)*total_vertices*total_vertices);
	for(i=0; i < total_vertices; i++){
		for(j=0; j < total_vertices; j++){
			*(m.matriz+(i*total_vertices)+j) = 0;			
		}
	}
	return m;
}

/*
	@objetivo. Mostrar el grafo representado con la matriz de adyacencia
	@parametros
		- g: grafo a ser mostrado
*/
void verMatrizGrafo(MATRIZ_ADYACENCIA g){
	int i, j;
	/* Imprime el nombre de los vertices */
	printf("<< MATRIZ DE ADYACENCIA >>\n\t");
	for(i=0; i < g.total_vertices; i++){
		printf("%c\t", g.nombre_vertices[i]);
	}
	printf("\n\t");
	for(i=0; i < g.total_vertices; i++){
		printf("-\t");
	}
	printf("\n");
	for(i=1; i <= g.total_vertices; i++){
		printf("%c\t", g.nombre_vertices[i-1]);
		for(j=0; j < g.total_vertices; j++){
			printf("%d\t", *(g.matriz + ((i-1)*g.total_vertices)+j));
		}
		printf("\n");
	}
}

/*
	@objetivo. Cargar las aristas en la matriz de adyacencia
	@parametros
		- m: matriz del grafo donde se cargaran las aristas
	@return: matriz de adyacencia del grafo con las aristas
*/
MATRIZ_ADYACENCIA cargaAristas(MATRIZ_ADYACENCIA m){
	FILE *archivo;

	char inicio, fin;
	int indice_inicio, indice_fin, auxPeso;
	if ((archivo = fopen("distancias.txt", "r")) == NULL){
		printf("Error al leer archivo\n");
		return m;
	}
	
	while (!feof(archivo)){	
		fscanf(archivo, "%c-%c %d\n", &inicio, &fin, &auxPeso);			
		//fscanf(archivo, "%c-%c\n", &inicio, &fin);			
		indice_inicio = getIndice(m.nombre_vertices, inicio);
		indice_fin = getIndice(m.nombre_vertices, fin);
		//fscanf(archivo, "%d\n", &auxPeso); // Puede fallar el \n
		//printf("\tArista %c-%c con peso %d\n", inicio, fin, auxPeso);
		//printf("\tArista %c-%c en [%d,%d] peso %d\n", inicio, fin, indice_inicio, indice_fin, auxPeso);
		*(m.matriz + ((indice_inicio)*m.total_vertices) + indice_fin) = auxPeso;
		*(m.matriz + ((indice_fin)*m.total_vertices) + indice_inicio) = auxPeso;
	}
	fclose(archivo);	
	return m;
}

/*
	@objetivo. Retornar el indice de un vertice
	@parametros
		- cadena: nombre de los vertices
		- c: vertice buscado
	@return: indice del vertice
*/
int getIndice(char *cadena, char c){
	int i;
	for(i=0; i <= strlen(cadena); i++){
		if (cadena[i] == c) return i;
	}
	return -1;
}

/*
	@objetivo. Crear la lista de adyacencia
	@parametros
		- vertices: lista con nombres de los vertices
	@return: lista de adyacencia del grafo
*/
LISTA_ADYACENCIA crearListaAdyacencia(char *vertices){
	int i;
	LISTA_ADYACENCIA vertice, ultimo, lx = NULL;
	for(i=0; i < strlen(vertices); i++){
		vertice = (LISTA_ADYACENCIA) malloc(sizeof(struct s_elemento));
		vertice->vertice = vertices[i];
		vertice->siguiente = NULL; // +
		vertice->conectado = NULL; // +
		if (lx == NULL){
			lx = vertice;
			ultimo = lx;
		}
		else{
			ultimo->siguiente = vertice;
			ultimo = ultimo->siguiente;
		}
	}
	return lx;
}

/*
	@objetivo. Cargar las aristas en la lista de adyacencia
	@parametros
		- lista: lista de adyacencia que sera modificada	
*/
void agregarAristas(LISTA_ADYACENCIA lista){
	FILE *archivo;

	char inicio, fin;
	int auxPeso;
	if ((archivo = fopen("distancias.txt", "r")) == NULL){
		printf("Error al leer archivo\n");
		return;
	}
	
	while (!feof(archivo)){	
		fscanf(archivo, "%c-%c", &inicio, &fin);
		fscanf(archivo, "%d\n", &auxPeso);
		/* Agrega los vértices a la lista */
		//printf("aviso (1)\n");		
		agregaVertice(lista, inicio, fin);
		agregaVertice(lista, fin, inicio);
		//printf("aviso (2)\n");	
		printf("\tArista %c-%c con peso %d cargada\n", inicio, fin, auxPeso);
	}
	fclose(archivo);	
}

/*
	@objetivo. Agrega las aristas a la lista de adyacencia
	@parametros
		- lista: lista de adyacencia que sera modificada
		- inicio: extremo inicial de la arista
		- fin: extremo final de la arista	
*/
void agregaVertice(LISTA_ADYACENCIA lista, char inicio, char fin){
	LISTA_ADYACENCIA auxiliar;
	NODO_VERTICE nodo_fin, auxiliar_vertice;

	auxiliar = lista;
	nodo_fin = (NODO_VERTICE) malloc(sizeof(struct s_nodo));
	nodo_fin->vertice = fin;
	nodo_fin->siguiente = NULL; // +
	/* Busca el vértice de inicio */	
	while(auxiliar->vertice != inicio){
		auxiliar = auxiliar->siguiente;
	}
	/* Asocia el extremo */
	if (auxiliar->conectado == NULL){
		auxiliar->conectado = nodo_fin;
	}
	else{
		auxiliar_vertice = auxiliar->conectado;
		while (auxiliar_vertice->siguiente != NULL){
			auxiliar_vertice = auxiliar_vertice->siguiente;
		}	
		auxiliar_vertice->siguiente = nodo_fin;
	}	
}

/*
	@objetivo. Mostrar la lista de adyacencia
	@parametros
		-lista: lista de adyacencia del grafo
*/
void verListaAdyacencia(LISTA_ADYACENCIA lista){
	LISTA_ADYACENCIA auxiliar = lista;
	NODO_VERTICE aux_vertice;

	printf("<< LISTA DE ADYACENCIA >>\n");
	while(auxiliar != NULL){
		printf("\t%c ==> ", auxiliar->vertice);
		aux_vertice = auxiliar->conectado;
		while (aux_vertice != NULL){
			printf("%c", aux_vertice->vertice);
			aux_vertice = aux_vertice->siguiente;
			printf("%s", (aux_vertice == NULL?"\n":" -> "));
		}		
		auxiliar = auxiliar->siguiente;
	}
}

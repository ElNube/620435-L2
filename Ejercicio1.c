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

/* Prototipo de funciones especiales */

void cargarPuertas(char *, char *);
int buscarPuertas(char *, char);
int cantidadPuertas(char *);

/* Obtiene el total de puertas usando la representaci´on de la matriz de adyacencia */

int get_total_puertas(MATRIZ_ADYACENCIA, char);

/* Obtiene el total de puertas usando la representaci´on de la lista de adyacencia */

int get_totalPuertasLA(LISTA_ADYACENCIA, char);

/* Funcion principal */
int main(){
	MATRIZ_ADYACENCIA gy;

	// Abro el archivo con los indicadores de las puertas y guardo la cadena de texto con los vertices del grafo
	
	int n = cantidadPuertas("puertas.txt") + 1;
	if (n < 4)
	{
		printf("Error: El formato de puertas.txt debe ser eNs, con N >= 1 cantidad de caracteres\n");
		return 0;
	}
	
	
	char puertas[n], auxpuerta;
	// Forma (1)
	cargarPuertas(puertas, "puertas.txt");
	printf("Puertas cargadas : %s\n", puertas);

	// Forma (2)
	//strcpy(puertas,"abcde");

	// Usando una matri< de adyacencia
	gy = crearMatrizGrafo(puertas);
	//printf("Matriz creada!!\n");
	//verMatrizGrafo(gx);
	gy = cargaAristas(gy);
	printf("Aristas cargadas!!\n");
	verMatrizGrafo(gy);

	// Ahora se usa la lista de adyacencia
	LISTA_ADYACENCIA gx = crearListaAdyacencia(puertas);
	agregarAristas(gx);
	verListaAdyacencia(gx);
	do
	{
		printf("Puerta a consultar : ");
		scanf("%c", &auxpuerta);
		if (getIndice(puertas, auxpuerta) == -1)
		{
			printf("Ingrese un valor valido\n.");
		}
	} while (getIndice(puertas, auxpuerta) == -1);
//	} while (buscarPuertas(puertas, auxpuerta) != 1);
	
	printf("Usando la representacion de la matriz de adyacentes\nDesde la puerta %c hay %d puertas disponibles\n", auxpuerta, get_total_puertas(gy, auxpuerta));
	printf("Usando la representacion de la lista de adyacentes\nDesde la puerta %c hay %d puertas disponibles\n", auxpuerta, get_totalPuertasLA(gx, auxpuerta));
	system("pause");
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
		fscanf(archivo, "%c-%c %d\n", &inicio, &fin, &auxPeso);
		/* Agrega los vértices a la lista */
		agregaVertice(lista, inicio, fin);
		agregaVertice(lista, fin, inicio);
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

void cargarPuertas(char * puertas, char * direccion){
	FILE *fpuertas;
	if ((fpuertas = fopen(direccion, "r")) == NULL)
	{
		printf("Error al leer archivo\n");
		return;
	}
	while (!feof(fpuertas))
	{
		fscanf(fpuertas, "%s", puertas);
	}
	fclose(fpuertas);
}


int buscarPuertas(char * puertas, char buscar){
	//char *auxPuertas = puertas;
	for (int i = 0; i < strlen(puertas); i++)
	{
		if (puertas[i] == buscar)
			return 1;
	}
	return 0;
}


/**
 * @brief Abrir un archivo y contar la cantidad de caracteres que se desean reservar, tambien sirve para validar la cantidad de puertas
 * 
 * @param direccion del archivo
 * @return Entero cantidad de caracteres
 */
int cantidadPuertas(char *direccion){
	//Define un archivo y un array de caracteres
	FILE *filepuertas;
	char puertas[50];

	//Se abre el archivo con permisos de 'r' read (lectura) y si esta vacio o no lo encuentra muestra un error y salimos de la funcion
	if ((filepuertas = fopen(direccion, "r")) == NULL)
	{
		printf("Error al leer archivo\n");
		return 0;
	}

	// Recorremos linea a linea la informacion del archivo
	while (!feof(filepuertas))
	{
		// Escaneamos del archivo 'filepuertas', una cadena "%s", y lo guardamos en el array 'puertas'
		fscanf(filepuertas, "%s\n", puertas); // buscar el porque esta linea dice ser no utilizada en el codigo
	}

	// Cerramos el archivo
	fclose(filepuertas);
		
	// retornamos el tamaño del array puertas modificado
	return strlen(puertas);
}

/**
 * @brief Obtiene la cantidad de puertas que le siguen a una determinada puerta
 * 
 * @param m Matriz
 * @param elegido puerta elegida 
 * @return La cantidad de puertas que le siguen en n entero
 */
int get_total_puertas(MATRIZ_ADYACENCIA m, char elegido){
	int contador = 0;
	for(int i=1; i <= m.total_vertices; i++){
		if (m.nombre_vertices[i-1] == elegido)
		{
			for(int j=0; j < m.total_vertices; j++)
			{
				if (!(i>j && *(m.matriz + ((i-1)*m.total_vertices)+j) > 0))
				{
					if (*(m.matriz + ((i-1)*m.total_vertices)+j) > 0)
					{
						contador++;
					}
				}
				
			}
			break;

		}
	}
	return contador;
}

int get_totalPuertasLA(LISTA_ADYACENCIA la, char elegido){
	return 0;
}
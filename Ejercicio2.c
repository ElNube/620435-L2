/*
	Implementacion de tabla hash usando:
		Metodo de la division para definir la funcion HASH
		Hash abierto para la resolucion de colisiones
	@autor - JMH - Agosto 2020
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//define CAPACIDAD 11

typedef struct s_lista{
	int valor;
	struct s_lista *siguiente;
} *ELEMENTO;

typedef struct s_tabla{
	int clave;
	ELEMENTO entradas;
} ENTRADA;

int hash(int);
void verTablaHash(ENTRADA[]);
void setearTabla(ENTRADA[]);
void agregarClave(ENTRADA[], int);
int buscar(ENTRADA[], int);

int main(){
	int i, aleatorio, capacidad;
	printf("Capacidad de la tabla : ");
	scanf("%d", &capacidad);
	ENTRADA tabla[capacidad];
	setearTabla(tabla);
	/* Simula las claves aleatorias */
	srand (time(NULL));
	printf("Tabla seteada\n");
	verTablaHash(tabla);
	for(i=1; i <= 50; i++){		
		do{
			aleatorio = rand() % 50 + 1;
			printf("Genera a %d ... ", aleatorio);
			/* Verifica que no exista */
			if (buscar(tabla, aleatorio) == -1){
				agregarClave(tabla, aleatorio);
				printf("%d agregado ...\n", aleatorio);		
				break;
			}
		} while(1);
	}
	verTablaHash(tabla);
	return 0;
}

/*
	Objetivo. Retornar el codigo HASH
	@parametros
		k - Clave a la cual se aplica la funcion
	@retorno - Codigo hash asociado a la clave
*/
int hash(int k, int capacidad){
	return (k % capacidad + 5) % capacidad;
}

/*
	Objetivo. Ver la tabla HASH
	@parametros
		tabla - Tabla hash que se imprime 
*/
void verTablaHash(ENTRADA tabla[]){
	int indice;
	ELEMENTO auxiliar;
	printf("\n ** TABLA HASH **\n");
	for(indice=0; indice < CAPACIDAD; indice++){		
		if (tabla[indice].entradas == NULL){
			printf("%d | {}\n", tabla[indice].clave);	
		}
		else{
			printf("%d | {", tabla[indice].clave);
			/* Muestra las entradas de la clave */
			auxiliar = tabla[indice].entradas;
			while (auxiliar != NULL){
				printf("%d", auxiliar->valor);
				auxiliar = auxiliar->siguiente;
				printf("%s",(auxiliar != NULL?",":"}\n"));
			}
		}
	}
}

/*
	Objetivo. Setear la tabla HASH
	@parametros
		tabla - Tabla hash a setear 
*/
void setearTabla(ENTRADA tabla[]){
	int indice;
	for(indice=0; indice < CAPACIDAD; indice++){
		tabla[indice].clave = indice;
		tabla[indice].entradas = NULL;
	}
}

/*

	Objetivo. Agregar entrada a la tabla HASH
	@parametros
		tabla - Tabla hash a la cual se agrega la entrada
		valor - Entrada que se agrega a la tabla
*/
void agregarClave(ENTRADA tabla[], int valor){
	int u_hash;
	ELEMENTO auxiliar;
	u_hash = hash(valor);

	ELEMENTO nuevo = (ELEMENTO) malloc(sizeof(struct s_lista));
	nuevo->valor = valor;
	nuevo->siguiente = NULL;

	if (tabla[u_hash].entradas == NULL){
		tabla[u_hash].entradas = nuevo;
	}
	else{		
		auxiliar = tabla[u_hash].entradas;
		while(auxiliar->siguiente != NULL){
			auxiliar = auxiliar->siguiente;
		}
		auxiliar->siguiente = nuevo;
	}
}

/*
	Objetivo. Buscar entrada a la tabla HASH
	@parametros
		tabla - Tabla hash donde se busca
		target - Entrada que se busca en la tabla
	@return
		1 : en caso de exito
		-1 : en caso de fracaso
*/
int buscar(ENTRADA tabla[], int target){	
	ELEMENTO auxiliar;
	auxiliar = tabla[hash(target)].entradas;
	
	while(auxiliar != NULL){
		if (auxiliar->valor == target) return 1;
		auxiliar = auxiliar->siguiente;
	}
	return -1;	
}
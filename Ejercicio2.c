/**
 * @file Ejercicio2.c
 * @author Marcelo Alfredo Paz Pezo
 * @firma …ᘛ⁐̤ᕐᐷ ICINF UBB
 * @brief Dado un archivo con puntos, se desea crear una tabla hash de tamaño N, y luego se desea pasar la tabla a un archivo para guardar los datos.
 * @version 0.0.4
 * @date 2022-08-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Estructuras */

typedef struct s_punto{
	int valorx, valory;
} P;

typedef struct s_lista{
	P valor;
	struct s_lista *siguiente;
} *ELEMENTO;

typedef struct s_tabla{
	int clave;
	ELEMENTO entradas;
} ENTRADA;

/* Prototipo de funciones  */

int hash(P, int);
void verTablaHash(ENTRADA[], int);
void setearTabla(ENTRADA[], int);
void agregarClave(ENTRADA[], P, int);
int buscar(ENTRADA[], P, int);

int main(){
	/**
	 * @param menu Validacion para tener un menu para buscar un punto en concreto 
	 * @param capacidad Capacidad de la tabla
	 */
	int menu, capacidad;

	/* Validar que la capacidad de la tabla hash sea un valor positivo y sobre 2, pues en teoria se recomienda tener numeros primos como capacidad*/ 
	do
	{
		printf("Capacidad de la tabla : ");
		scanf("%d", &capacidad);
		if (capacidad < 2)
			printf("Valor invalido\n");
	} while (capacidad < 2);
	
	/* Declaramos la tabla y la iniciamos */
	ENTRADA tabla[capacidad];
	setearTabla(tabla, capacidad);
	printf("Tabla Iniciada\n");
	
	/* Declaramos auxiliares para leer nuestro archivo */
	printf("Cargando archivo puntos.txt a la tabla\n");
	P punto;
	FILE *archivo;

	/* Si el archivo falla nos mostrara un error por consola y finalizara el programa */
	if ((archivo = fopen("puntos.txt", "r")) == NULL)
	{
		printf("Error al leer archivo\n");
		return 0;
	}
	
	/* Copiamos todas las lineas del archivo y las agregamos a la tabla */
	while (!feof(archivo))
	{	
		fscanf(archivo, "%d,%d\n", &punto.valorx, &punto.valory);			
		agregarClave(tabla, punto, capacidad);

	}
	fclose(archivo);

	/* Declaramos auxiliares para cargar la tabla en un archivo */
	printf("Cargando tabla a el archivo hash.txt\n");
	ELEMENTO auxiliar;

	/* Si el archivo falla nos mostrara un error por consola y finalizara el programa */
	if ((archivo = fopen("hash.txt", "w")) == NULL){
		printf("Error al leer archivo\n");
		return 0;
	}

	/* Imprimimos en el archivo la tabla hash */
	fprintf(archivo,"\n ** TABLA HASH **\n");
	for(int i=0; i < capacidad; i++)
	{		
		if (tabla[i].entradas == NULL)
		{
			fprintf(archivo,"%d | {}\n", tabla[i].clave);	
		}
		else
		{
			fprintf(archivo,"%d | {", tabla[i].clave);
			/* Muestra las entradas de la clave */
			auxiliar = tabla[i].entradas;
			while (auxiliar != NULL)
			{
				fprintf(archivo,"(%d, %d)", auxiliar->valor.valorx, auxiliar->valor.valory);
				auxiliar = auxiliar->siguiente;
				fprintf(archivo,"%s",(auxiliar != NULL?",":"}\n"));
			}
		}
	}
	fclose(archivo);

	/* Buscamos la existencia de un punto dentro de la tabla hash */
	do
	{
		printf("----MENU----\n");
		printf("(0) Salir\n");
		printf("(1) Buscar punto en la tabla hash\n---> ");
		scanf("%d", &menu);
		switch (menu)
		{
			case 1:
				printf("Buscar punto\n");
				printf("\tX : ");
				scanf("%d", &punto.valorx);
				printf("\tY : ");
				scanf("%d", &punto.valory);
				if (buscar(tabla,punto,capacidad) == 1)
				{
					printf("Exito\n");
				}
				else
					printf("Fracaso\n");
				break;
		}
	} while (menu != 0);
	
	/* Fin del programa */
	printf("\nPROGRAMA FINALIZADO\n");
	
	return 0;
}

/**
 * @brief Retornar el codigo HASH
 * 
 * @param k Clave a la cual se aplica la funcion
 * @param capacidad Capacidad de la tabla
 * @return Codigo hash asociado a la clave
 */
int hash(P k, int capacidad){
	return ((k.valorx % capacidad + 5) + k.valory) % capacidad;
}

/**
 * @brief Ver la tabla HASH
 * 
 * @param tabla Tabla hash que se imprime 
 * @param capacidad Capacidad de la tabla
 */
void verTablaHash(ENTRADA tabla[], int capacidad){
	int indice;
	ELEMENTO auxiliar;
	printf("\n ** TABLA HASH **\n");
	for(indice=0; indice < capacidad; indice++){		
		if (tabla[indice].entradas == NULL){
			printf("%d | {}\n", tabla[indice].clave);	
		}
		else{
			printf("%d | {", tabla[indice].clave);
			/* Muestra las entradas de la clave */
			auxiliar = tabla[indice].entradas;
			while (auxiliar != NULL){
				printf("(%d, %d)", auxiliar->valor.valorx, auxiliar->valor.valory);
				auxiliar = auxiliar->siguiente;
				printf("%s",(auxiliar != NULL?",":"}\n"));
			}
		}
	}
}

/**
 * @brief Setear la tabla HASH
 * 
 * @param tabla Tabla hash a setear 
 * @param capacidad Capacidad de la tabla
 */
void setearTabla(ENTRADA tabla[], int capacidad){
	int indice;
	for(indice=0; indice < capacidad; indice++){
		tabla[indice].clave = indice;
		tabla[indice].entradas = NULL;
	}
}

/**
 * @brief Agregar entrada a la tabla HASH
 * 
 * @param tabla Tabla hash a la cual se agrega la entrada
 * @param valor Entrada que se agrega a la tabla
 * @param capacidad Capacidad de la tabla
 */
void agregarClave(ENTRADA tabla[], P valor, int capacidad){
	int u_hash;
	ELEMENTO auxiliar;
	u_hash = hash(valor, capacidad);

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

/**
 * @brief Buscar entrada a la tabla HASH
 * 
 * @param tabla Tabla hash donde se busca
 * @param target Entrada que se busca en la tabla
 * @param capacidad Capacidad de la tabla
 * @return: 1 : en caso de exito -1 : en caso de fracaso
 */
int buscar(ENTRADA tabla[], P target, int capacidad){	
	ELEMENTO auxiliar;
	auxiliar = tabla[hash(target, capacidad)].entradas;
	
	while(auxiliar != NULL){
		if (auxiliar->valor.valorx == target.valorx && auxiliar->valor.valory == target.valory) return 1;
		auxiliar = auxiliar->siguiente;
	}
	return -1;	
}
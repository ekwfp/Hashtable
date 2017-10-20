#include "lista.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#define C_INICIAL 0

struct nodo{
	void * dato;
	struct nodo * siguiente;
};

typedef struct nodo nodo_t;

struct lista {
	size_t cantidad;
    nodo_t* primero;
	nodo_t* ultimo;
}; //lista_t


struct lista_iter {
	lista_t* lista;
	nodo_t* actual;
	nodo_t* memoria;//para insertar
}; //lista_iter_t

// ******************************************************************
//                    PRIMITIVAS DE LA LISTA
// ******************************************************************


// Crea una lista.
// Post: Da una lista vacia.
lista_t *lista_crear(void){
    
    lista_t* lista = (lista_t*) calloc (1, sizeof(lista_t));
    if (lista == NULL) { free(lista); return NULL; }
    
        lista->primero = NULL;
        lista->ultimo = NULL;
        lista->cantidad = C_INICIAL;
    return lista;
    
}

// Devuelve verdadero si no tiene elementos, falso en caso contrario.
// Pre: La lista fue creada.
bool lista_esta_vacia(const lista_t *lista){
    return (lista!=NULL && (lista->primero == NULL)); 
}

// Agrega un nuevo elemento al comienzo de la lista. Si hay error devuelve false.
// Pre: La lista fue creada.
// Post: se agrego un nuevo elemento a la lista en la primera posicion.
bool lista_insertar_primero(lista_t *lista, void *dato){
    
	if (lista != NULL){
        
		nodo_t *act = (nodo_t *) calloc (1,sizeof(nodo_t));
        
        if (act == NULL){  free(act);  return false;  }
        
		act->dato = dato;
        
		if(!lista_esta_vacia(lista)){
			act->siguiente = lista->primero; // encolo
			lista->primero = act; //seteo ultimo
			
		}else{ //seteo primero
            
			act->siguiente= NULL;
			lista->primero = act;
			lista->ultimo = act;
            
		}
    lista->cantidad++;
	return true;
	}
	return false;
}

// Agrega un nuevo elemento al final de la lista. Si hay error devuelve false.
// Pre: La lista fue creada.
// Post: se agrego un nuevo elemento a la lista en la ultima posicion.
bool lista_insertar_ultimo(lista_t *lista, void *dato){
    
	if (lista != NULL){
        
		nodo_t *act = (nodo_t *) calloc (1,sizeof(nodo_t));
        
        if (act == NULL){  free(act);  return false;  }
        
		act->dato = dato;
        act->siguiente= NULL;
        
		if(!lista_esta_vacia(lista)){
            
			lista->ultimo->siguiente = act; //seteo ultimo
            lista->ultimo=act;
            
			
		}else{ //seteo primero
            
			lista->primero = act;
			lista->ultimo = act;
		}
        lista->cantidad++;
	   return true;
	}
	return false;

    
}

// Se devuelve el valor del primer elemento de la lista y se lo quita de la misma.
// Si la lista esta vacia devuelve NULL.
// Pre: la lista fue creada.
// Post: Se devuelve el elemento eliminado, o null si estaba vacia.
void *lista_borrar_primero(lista_t *lista){
    if (!lista_esta_vacia(lista)) {
    	
		nodo_t *nodito = lista->primero;
		void * valor = nodito->dato;//recupero el dato
		lista->primero = nodito->siguiente; 
		
        if (nodito == lista->ultimo) lista->ultimo = nodito->siguiente;
        
        nodito->siguiente=NULL;
		free(nodito);
		
        lista->cantidad--;
		return valor;
	}
    
    return NULL;
}




// Devuelve null si esta vacia, si tiene elementos devuelve el primero sin quitarlo de la lista.
// Pre: La lista fue creada.
// Post: Devuelve el primer elemento cuando la lista no esta vacia. 

void *lista_ver_primero(const lista_t *lista){
    if (!lista_esta_vacia(lista)) return lista->primero->dato;
    return NULL;
}

// Devuelve null si esta vacia, si tiene elementos devuelve el primero sin quitarlo de la lista.
// Pre: La lista fue creada.
// Post: Devuelve el primer elemento cuando la lista no esta vacia. 

void *lista_ver_ultimo(const lista_t *lista){
    if (!lista_esta_vacia(lista)) return lista->ultimo->dato;
    return NULL;
    
}

// Devuelve la cantidad de elementos de la lista.
// Pre: La lista fue creada.
// Post:  Devuelve la cantidad de elementos.
size_t lista_largo(const lista_t *lista){
	if (lista_esta_vacia(lista)) {
		return C_INICIAL;
	}
    return (lista->cantidad);
}

// Destruye la lista. Si se recibe la funcionn destruir_dato por parametro,
// para cada uno de los elementos de la lista llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una funcionn capaz de destruir
// los datos de la lista, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void destruir_dato(void *)){
    void *a_destr;
	
	while (!lista_esta_vacia(lista)){
        
			a_destr= lista_borrar_primero(lista);//destruye el ultimo nodo y devuelve su dato	
			if(destruir_dato != NULL)
			 destruir_dato(a_destr);// destruyo dato
	}//no quedan nodos
   
   lista->cantidad=C_INICIAL;
	free(lista);
	
}

void lista_destruir_hash(lista_t *lista, void* hash, void destruir_dato(void *, void*)){
	
	void* acceso_dato;
	while (!lista_esta_vacia(lista)){
		
		
		acceso_dato = lista_borrar_primero(lista);
		if (destruir_dato != NULL) destruir_dato(acceso_dato,hash);
	/*	acceso_dato = (nodo_hash_t*)lista_borrar_primero(lista);
		free(acceso_dato->clave);
		if (hash->func_destruct != NULL) hash->func_destruct(acceso_dato->dato);
		free(acceso_dato);*/
			
	}

	free (lista);
	
	
}


// ******************************************************************
//                    PRIMITIVAS DE ITERADOR INTERNO
// ******************************************************************

// Itera a traves de la lista hasta que la funcion visitar devuelva false o el iterador llegue al final
// Pre: La lista fue creada. extra es un puntero apuntando a algo accesible.
void lista_iterar(lista_t *lista, bool (*visitar)(void *dato, void *extra), void *extra){
    
    lista_iter_t * iter = lista_iter_crear(lista);
    
    
    while (!lista_iter_al_final(iter)){
    if (visitar!=NULL)
        if (!visitar(iter->actual->dato,extra)) { 
		lista_iter_destruir(iter); return; 
	}
    
    lista_iter_avanzar(iter);
    }
    
  
    lista_iter_destruir(iter);
    
    
}

// ******************************************************************
//                    PRIMITIVAS DEL ITERADOR
// ******************************************************************




// Crea un iterador apuntando al inicio de la lista.
// Pre: Lista creada.
// Post: El iterador fue creado al inicio de la lista.
lista_iter_t *lista_iter_crear(lista_t *lista){
    
    lista_iter_t* iter = (lista_iter_t*) calloc (1,sizeof(lista_iter_t));
    if (iter==NULL) {free(iter); return NULL;}
    
    iter->lista= lista;
    iter->memoria= NULL;
    iter->actual= iter->lista->primero;
    
    return iter;
}

// Devuelve true si el iterador esta al final de la lista o false.
// Pre: Lista e iterador creados.
bool lista_iter_al_final(const lista_iter_t *iter){
    return ((iter->memoria &&iter->actual == NULL) || (iter->memoria == NULL && iter->actual == NULL));
}

// El iterador avanza una posicion en la lista.
// Pre: Lista e iterador creados.
// Post: Devuelve true si se pudo avanzar o false si hubo un problema.
bool lista_iter_avanzar(lista_iter_t *iter){
	
    if (!lista_iter_al_final(iter)){
    	
    iter->memoria = iter->actual;
    iter->actual= iter->actual->siguiente;
    return true;
	} 
	
    return false;
}

// Devuelve el dato del nodo de la lista donde actualmente se encuentra el iterador.
// Pre: Lista e iterador creados.
// Post: Devuelve el puntero al dato.
void *lista_iter_ver_actual(const lista_iter_t *iter){
    if (!lista_iter_al_final(iter))
		  return iter->actual->dato;
    return NULL;
    }


// Destruye el iterador.
// Pre: Iterador creado.
// Post: Iterador destruido.
void lista_iter_destruir(lista_iter_t *iter){
	
    free(iter);
}

// En la posicion en la que se encuentra el iterador se inserta un elemento en la lista.
// Pre: Lista e iterador creados.
// Post: Devuelve true si se pudo insertar el elemento, o false de haber un problema.


bool lista_iter_insertar(lista_iter_t *iter, void *dato){
    
    bool insertado = false;
        if(iter->actual == iter->lista->primero)	{  //insertar al principio
			
        	 insertado = lista_insertar_primero(iter->lista,dato);
        	if(insertado){ // se comporta igual
        		 iter->actual = iter->lista->primero;
        			iter->memoria = NULL;
        			return true;
				}
		 else return false;
		 }
				
		if (lista_iter_al_final(iter)){// insertar al final
			iter->memoria = iter->lista->ultimo;
			insertado = lista_insertar_ultimo(iter->lista,dato);
			if (insertado){ // se comporta igual
				
				
				iter->actual = iter->lista->ultimo;
				return true;
			}
			else {
				iter->actual = NULL;
				return false;
			}
		}
		
		//insertar "en medio"
		
		 nodo_t* nodito = (nodo_t*) calloc (1, sizeof(nodo_t));
        if (nodito == NULL) { free(nodito); return false;}
		
		nodito->dato =dato;
		nodito->siguiente = iter->actual;
		iter->memoria->siguiente = nodito;
		iter->actual = nodito;
		
		iter->lista->cantidad++;
		return true;
		
        
    }

// Borra de la lista el elemento al que apunta el iterador y devuelve su dato.
// Devuelve Null si estaba vacia o si el iterador estaba al final.
// Pre: Lista e iterador creados.
// Post: El iterador queda apuntado a la siguiente posicion.

void *lista_iter_borrar(lista_iter_t *iter){
	
	void * valor;
	
	if(lista_iter_al_final(iter)|| lista_esta_vacia(iter->lista)) return NULL;
	
	if(iter->actual == iter->lista->primero){
		valor = lista_borrar_primero(iter->lista);//se comporta igual
		iter->actual = iter->lista->primero;
		iter->memoria = NULL;
	}
	
	else {
		valor = iter->actual->dato;
		nodo_t * nodito = iter->actual;
		
		iter->memoria->siguiente= iter->actual->siguiente;
		iter->actual = iter->actual->siguiente;
		
		if (iter->actual == iter->lista->ultimo) iter->lista->ultimo = iter->memoria;
		
		free(nodito);
		iter->lista->cantidad--;
	}
	
	return valor;
}




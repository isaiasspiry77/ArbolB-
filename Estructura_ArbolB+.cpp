#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define p printf

typedef struct pagina{
	int orden; //contador de elementos que abra en cada pagina 
	struct pagina *izq;
	struct list *inicio;
}Pagina;

typedef struct nodo{
	int clave;	//clave o la informacion ecencial 
	struct pagina *der;
}Nodo;

typedef struct list{
	struct nodo *raiz;
	struct list *sig;
}Lista;


Pagina* CreaPagina(){
	Pagina *nuevo=(Pagina*)malloc(sizeof(Pagina));
	if(!nuevo) return NULL;
	
	nuevo->inicio=NULL;
	nuevo->izq=NULL;
	nuevo->orden=0;
	
	return nuevo;
}

Nodo* CreaNodo(int dato){
	Nodo *nuevo=(Nodo*) malloc (sizeof(Nodo));
	if(!nuevo) return NULL;
	
	nuevo->clave=dato;
	nuevo->der=NULL;
	
	return nuevo;
}

int CreaLista(Lista **inicio, int dato){
	if(!(*inicio) || (*inicio)->raiz->clave>dato){ //si no hay una lista de nodos o si el dato del nodo existente es menor al dato nuevo se crea un nueva 
		Lista *nuevo=(Lista*)malloc(sizeof(Lista));//rama de la lista y se le cuelga un nodo con clave   
		if(!nuevo) return 0;
		
		Nodo *aux= CreaNodo(dato);
		if(!aux) return 0;
		
		nuevo->raiz=aux;
		nuevo->sig=(*inicio);
		(*inicio)=nuevo;
		return 1;
	}
	return CreaLista(&(*inicio)->sig,dato);
}

Lista* Busca_Nodo_Lista(Lista **inicio, int dmitad){
	if(!inicio) return NULL;
	if(dmitad ==0){
		Lista *aux= (*inicio);//creo una apuntador de tipo lista el cual apuntara al nodo de enmedio
		(*inicio)=NULL; //la flecha inicio que apunta al nodo de enmedio la desligo dejando a los datos restantes siendo apuntados por aux
		return aux; //retorno aux
	}
	return Busca_Nodo_Lista(&(*inicio)->sig, dmitad-1);
}

Lista* Busca_NodoIndice_Insertar(Lista **inicio, int db){
	if(!(*inicio)->sig) return (*inicio); //si no tiene un sig quiere decir que es el unico indice en donde se puede insertar el dato
	//si el dato nuevo es mayor al dato del nodo de la pagina indice y a la ves el dato nuevo es menor al siguiente dato de la pagina indice regresa inicio
	if((*inicio)->raiz->clave < db && db < (*inicio)->sig->raiz->clave) return (*inicio);
	//si no se cumple la condicion de arriba se buscara en el siguiente dato de la pagina de indices
	return Busca_NodoIndice_Insertar(&(*inicio)->sig,db);
}

int DividePagina(Pagina **raiz, int dmitad){
		//se buscara el nodo de enmedio el cual se ara una copia para subirlo como indice
		Lista *aux=Busca_Nodo_Lista(&(*raiz)->inicio,dmitad);
		if(!aux) return 0;
		//se crean las 2 paginas que contendran a las nuevas claves
		Pagina *pag1= CreaPagina();
		if(!pag1) return 0;
		Pagina *pag2=CreaPagina();
		if(!pag2) return 0;
	
		//la pagina 1 sera la raiz y la raiz sera la pagina que estara a asu izquierda ya que contendra a los valores menores a la raiz
		pag1->izq=(*raiz);
		//se crea una nueva cajita de lista la cual contendra la copia del nodo de enmedio
		int i=CreaLista(&pag1->inicio, aux->raiz->clave);
		if(i==0) return 0;
		//el apuntador a lista de la pagina 2 sera lo que contenga aux  
		pag2->inicio=aux;	
		//como la pagina 1 sera la nueva raiz, a la derecha de su nodo se le colgara la vieja raiz
		pag1->inicio->raiz->der=pag2;
		//como la raiz sera la pagina que estara a la izquierda su orden sera de 2 
		(*raiz)->orden=2;
		//la pagina 1 sera la nueva raiz y como contendra un solo elemento sera de orden 1
		pag1->orden=1;
		//la pagina 2 sera la pagina que estara a la derecha del nodo de la pagina 1 entonces su orden sera de 3
		pag2->orden=3;
		//asignamos como raiz a la pagina 1
		(*raiz)= pag1;
		return 1;
}

int DivideRaiz(Pagina **raiz, int dmitad){
	//Busca el nodo de enmedio de la pagina raiz indice 
	Lista *aux=Busca_Nodo_Lista(&(*raiz)->inicio,dmitad);
	if(!aux) return 0;
	//se crena las 2 paginas nuevas que contendran a los datos menores y mayores de la raizde indice
	Pagina *pag1= CreaPagina();
	if(!pag1) return 0;
	Pagina *pag2= CreaPagina();
	if(!pag2) return 0;
	//se enlaza la pagina 1 que sera la nueva raiz de indices con su apuntador izq a la raiz principal 
	pag1->izq=(*raiz);
	//como un arbol b la pagina 2 su inicio sera el dato sig del nodo de enmedio
	pag2->inicio=aux->sig;
	//la izquierda la pagina 2 apuntara a los datos mayores al nodo de la mitad 
	pag2->izq=aux->raiz->der;
	//con el fin de que solo el nodo de enmedio quede como raiz se le corta su sig
	aux->sig=NULL;
	//la pagina 1 tomara el lugar de la raiz de indice por lo cual en su lista debe ir el dato de enmedio 
	pag1->inicio=aux;
	//una ves teniendo el dato de enmedio en la pagina 1 la derecha de ese nodo apuntara a la pagina 2 que contiene a los datos mayores a el
	pag1->inicio->raiz->der=pag2;
	//se modificara el orden tanto de las paginas nuevas como el de la antigua raiz
	(*raiz)->orden =2;
	pag1->orden=1;
	pag2->orden=2;
	//la pagina 1 toma el lugar como raiz
	(*raiz)=pag1;
	return 1;
}

int Inserta(Pagina **raiz, int dato, int grado){
	int m= (2*grado)+1;//numero maximo de elementos
	
	if(!(*raiz)->izq){ //si no hay izquierda entonces quiere decir que la pagina esta vacia o que aun hay espacio en la pagina  
		(*raiz)->orden++; //se ira incrementando el orden de la pagina hasta llegar a (grado*2)+1
		int i=CreaLista(&(*raiz)->inicio,dato);
		if(i==0) return 0;
		
		if((*raiz)->orden == m){//si el grado de la pagina raiz es igual al limite de elementos permitidos se partira la pagina
			//dividir pagina
			DividePagina(raiz,grado);
			return 2;//retorna 2 si la pagina de datos se rompe
		}
		return 1;//retorna 1 si la insercion fue simple y no es necesario partir la pagina de datos
	}
	
	if(dato < (*raiz)->inicio->raiz->clave){ //si el dato a ingresar es menor al dato que esta en el indice de la pagina raiz, se colocara en la pagina de la izquierda
		int j=Inserta(&(*raiz)->izq,dato,grado);//se insertan los datos con normalidad en solo que ahora lo hacen en la pagina izq de la raiz inidce la cual son los menores
		if(j==2){//si la pagina a la izq de la raiz de inidices se divide 
			//se crea un aux de tipo lista el cual apuntara al inicio de la lista de la pagina raiz inidce creada por la division de la pagina izq de la raiz inidice principal
			Lista *aux= (*raiz)->izq->inicio; 
			//se liga la cajita de la lista de la pagina raiz indice que esta a la izq con la lista de la pagina raiz indice principal
			aux->sig=(*raiz)->inicio;
			//como el dato que esta en la lista apuntada por aux es menor al de la pagna principal inidice, aux tomo el inicio de la lista
			(*raiz)->inicio=aux;
			//el apuntador izq de la pagina principal indice ahora apuntara a la pagina que esta a la izq de la pagina raiz indice izq (ya que dicha no nos interesa)
			(*raiz)->izq=(*raiz)->izq->izq;
			//como la pagina raiz indice tiene una nueva clave se le aumenta el orden 
			(*raiz)->orden++;
			return 1;
		}
		return 1;
	}
	//si el dato es mayor al del inicio de la lista de la pagina raiz de indices entonces mando a buscar el indice adecuado para insertar el nuevo dato
	Lista *aux2= Busca_NodoIndice_Insertar(&(*raiz)->inicio,dato);
	if(!aux2) return 0;
	//una ves encontrando el el nodo indice adecuado prosigue la insercion a su pagina de claves derecha
	int j=Inserta(&aux2->raiz->der, dato, grado);
	//si la pagina derecha de la raiz de indice se divide 
	if(aux2->raiz->der->orden==1){
		aux2->raiz->der->inicio->sig =aux2->sig;
		aux2->sig=aux2->raiz->der->inicio;
		aux2->raiz->der=aux2->raiz->der->izq;
		(*raiz)->orden++;
		return 1;
	}

	//si la raiz de indices alcanza el numero maximo de nodos 
	if((*raiz)->orden==m){
		//divide la raiz 
		DivideRaiz(raiz,grado);
		return 2;
	}
}
void Imprime(Pagina *raiz);

void ImprimePaginas(Lista *inicio){
	if(!inicio) return;
	Imprime(inicio->raiz->der);
	p("\n");
	ImprimePaginas(inicio->sig);
}

void ImprimeLista(Lista *inicio){
	if(!inicio) return;
	//imprimo el dato clave de la lista de nodos de la raiz
	p("clave-> %d ", inicio->raiz->clave);
	return ImprimeLista(inicio->sig);
}
void Imprime(Pagina *raiz){
	if(!raiz) return;
	//primero imprimo el orden de mi pagina
	p("orden-> %d ", raiz->orden);
	//llamo a la funcion que imprime la informacion de los nodos de la raiz
	ImprimeLista(raiz->inicio);
	//despues de imprimir la informacion del nodo de la raiz o la raiz de indices preguntamos si existe izquierda
	if(raiz->izq){
		p("\n\n");
		//si existe izquierda imprimimos los datos que estan a la izq
		Imprime(raiz->izq);
		p("\n");
		//despues de imprimir los datos de la raiz izq volvemos a la raiz e imprimimos lo que tiene la pagina der
		ImprimePaginas(raiz->inicio);
	}
	return;
}

Nodo* BuscaNodo(Pagina **raiz, int db, int op);

Nodo* BuscaLista(Lista **inicio,int db,int op){
	if(!(*inicio))return NULL;
	//si el dato que se va a buscar es igual al dato de la raiz y si el nodo raiz no tiene derecha se retornara el nodo 
	if((*inicio)->raiz->clave==db && (*inicio)->raiz->der==NULL){
			if(op==0) return (*inicio)->raiz;//si la op es 0 quiere decir que solo necesitamos regresar el nodo ya sea para mostrar o para editar su informacion
			if(op==1){//si la opcion es 1 entonces se requiere el nodo para eliminarlo
				//antes de eliminar el nodo se debe desenlazar la lista que contiene al nodo 
				Lista *aux=(*inicio);
				(*inicio)=aux->sig;
				//una ves que esta desenlazado la lista el nodo es guardado en una aux de tipo nodo
				Nodo *auxNodo= aux->raiz;
				//procedemos a liberar la memoria que ocupaba el nodo de tipo lista
				free(aux);
				return auxNodo;
			}
	}
	//si no exste siguiente quiere decir que es el unico nodo en la pagina raiz de indices por lo cual se buscara en sus hojas 
	if(!(*inicio)->sig)
		return BuscaNodo(&(*inicio)->raiz->der, db,op);
	//si el dato que estoy buscando es mayor al dato del nodo raiz y a la ves menor que el datos sig del nodo raiz quiere decir que en ese intermedio a la derecha tengo que insertar
	if(db>(*inicio)->raiz->clave && db<(*inicio)->sig->raiz->clave)
		return BuscaNodo(&(*inicio)->raiz->der,db,op);
	//si se llega a este punto se pregunta si la el dato del nodo raiz es el mismo que el dato que quiero encontrar (es la etiqueta copia) por lo cual el dato debe estar en su derecha
	if(db==(*inicio)->raiz->clave)
		return BuscaNodo(&(*inicio)->raiz->der,db,op);
	//si no se cumple ninguna condicion anterior quiere decir que hay mas elementos en la lista a los cuales hay que recorrerlos
	return BuscaLista(&(*inicio)->sig,db,op);
}

Nodo* BuscaNodo(Pagina **raiz, int db, int op){
	int bandera=0;
	
	if(!(*raiz)) return NULL;
	//si el dato que estoy buscando es menor al primer dato de la lista de nodos se ira a la izq que es donde estan los datos menores
	if(db < (*raiz)->inicio->raiz->clave){
		bandera=1;
		Nodo *aux=BuscaNodo(&(*raiz)->izq,db,op);
		if(!aux) return NULL;
		return aux;
	}
	//si el dato que estoy buscando es mayor al primer dato que esta en la lista de nodos se buscara en la lista de nodos de la pagina raiz 
	Nodo *aux= BuscaLista(&(*raiz)->inicio,db,op);
	if(!aux) return NULL;
	//aqui deberia ir la reduccion del orden de la pagina pero si lo hago le bajaria el grado a todo
//	if(bandera==1)
//		(*raiz)->orden=(*raiz)->orden-1;
	
	return aux;
}

int Modifica(Pagina **raiz, int datomodif){
	//reutilizamos la funcion la funcion de buscar 
	Nodo *aux= BuscaNodo(&(*raiz),datomodif,0);
	if(!aux)return 0;
	//una ves encontrado el nodo que andamos buscando procedemos a sustituirlo por el nuevo dato
	p("\n Ingresa la nueva clave: ");
	scanf("%d", &aux->clave);
}

int Elimina_Hoja(Pagina **raiz, int datoeli){//esta funcion solo elimina hojas y no reestructura
	Nodo *aux=BuscaNodo(&(*raiz),datoeli,1);
	if(!aux) return 0;
	free(aux);
	return 1;
}

int main(int argc, char *argv[]){
	int grado=2;
	Pagina *raiz=CreaPagina();
	if(!raiz) return 0;
	
	Inserta(&raiz,10,grado);
	Inserta(&raiz,20,grado);
	Inserta(&raiz,5,grado);
	Inserta(&raiz,30,grado);
	Inserta(&raiz,40,grado);
	Inserta(&raiz,1,grado);
	Inserta(&raiz,2,grado);
	Inserta(&raiz,22,grado);
	Inserta(&raiz,3,grado);
	Inserta(&raiz,4,grado);
	Inserta(&raiz,0,grado);
	Inserta(&raiz,11,grado);
	Inserta(&raiz,23,grado);
	Inserta(&raiz,41,grado);
	Inserta(&raiz,42,grado);
	Inserta(&raiz,12,grado);
	
	Imprime(raiz);
	
//	Nodo *aux=BuscaNodo(&raiz,42,0);
//	if(!aux) p(" no cacho nada");
//	p("dato encontrado= %d", aux->clave);
//	
//	if(Modifica(&raiz,12)==1)
//		p("\nEl dato fue modificado exitosamente\n");
//	else p("\nEl dato a modificar no es el correcto");
//	
//	Imprime(raiz);
	
	if(Elimina_Hoja(&raiz,42)==1)
		p("\nEl dato fue eliminado exitosamente\n");
	else p("\nEl dato no fue encontrado");
	
	Imprime(raiz);
	//p("hola soy un arbol b+");
	return 0;
}

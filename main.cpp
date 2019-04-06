/*
autor: Juan Chozas

Envasado de objetos

Se tiene una colección denobjetos “moldeables”, cada uno con un volumen vi, para i entre 1 y n, que hay que empaquetar utilizando envases decapacidad E. 
Queremos conocer el empaquetamiento óptimo, es decir, que minimice la cantidad de envases utilizados, teniendo en cuenta que los objetosno se pueden fraccionar.

Supondremos que todo objeto cabe en un envase vacío. 
Ya que necesitamos como mucho n envases, podemos numerar los envases del 1 al n.
Las soluciones podemos representarlas en tuplas de la forma(x1, . . . ,xn), siendo xi el envase donde hemos colocado el objeto i.
Como todos los envases vacíos son iguales, para cada objeto se puede usar uno de los envases ya ocupados, si cabe en alguno, o coger uno cualquiera sin usar (vacío). 
El primer objeto siempre se colocará en el primer envase (x1=1).


---------------



Normas para la presentaci ́on de la pr ́actica

5.  Se ejecutar ́a el programa con al menostres juegos de datos distintos, con datos lo mas volu-minosos posibles.

6.  Para podar, se utilizar ́an 
    al menos dos estimaciones optimistas y dos pesimistas (si son aplicables):
        una ing ́enua y poco costosa, 
        y otra m ́as ajustada y posiblemente m ́as costosa.

7.  Se  imprimir ́an  el  n ́umero  de nodos  explorados cuando  no  se  utiliza m ́as  poda  que  
    la  de factibilidad y los explorados cuando se utiliza cada una de las dos podas.

8.  Tambi ́en se dar ́a el tiempo total y el tiempo medio por nodo explorado en cada uno de los tres casos.

9.  Se entregar ́an las fuentes del programa con suficientes comentarios, los ficheros de prueba, los resultados obtenidos, 
    y una brevememoria(m ́aximo 3 p ́aginas) con las explicaciones adicionalesque se consideren necesarias y las conclusiones personales obtenidas.

10.  La entrega se har ́a subiendo un fichero comprimido al campus virtuala lo sumo el mi ́ercoles15 de mayo. 
    Las pr ́acticas no subidas en esa fecha se dar ́an por no entregadas.

11.  Se  podr ́an  fijar  fechas  durante  mayo  o  junio  para  hacer  una  demostraci ́on  ante  el  profesor  yresponder a sus preguntas.

*/


#include <iostream>     //IO
#include <vector>       //LISTAS
#include <iterator>     //ITERADOR
#include <queue>        //COLA DE PRIORIDAD
#include <algorithm>    //ORDENACION

using std::vector;
using std::priority_queue;
using std::iterator;
using std::min;
using std::cout;
using std::endl;

using t_vect = vector<int>;

struct Nodo { 
    int k;
    int nenvases_real;
    int nenvases_optimista;
    t_vect sol;
}; 
struct Comparacion_Nodos {
    bool operator()(const Nodo* l, const Nodo* r) const {
        if(l->nenvases_optimista == r->nenvases_optimista) 
            return l->nenvases_real < r->nenvases_real;
        else         
            return l->nenvases_optimista > r->nenvases_optimista;
    } 
};

using t_prioq = priority_queue<Nodo*, vector<Nodo*>, Comparacion_Nodos>;

void printv(const t_vect &vol) {
    t_vect::const_iterator i = vol.cbegin();
    while(i != vol.cend()) 
        cout << *i++ << " ";
    cout << endl;
}

int cota_pesimista_one() {
    int retval = 0;
  
    return retval;
}

int cota_pesimista_two() {
    int retval = 0;
  
    return retval;
}

int optimista_one() {
    int retval = 0;
  
    return retval;
}

int optimista_two() {
    int retval = 0;
  
    return retval;
}


Nodo* envases(const int E, const t_vect &vol) {
    Nodo* x;
    Nodo* y = new Nodo;    
    Nodo* sol_mejor = new Nodo;
    
    t_prioq pq = t_prioq();

    y->nenvases_real = 0;
    y->k = 0;
    y->nenvases_optimista =
    y->sol = t_vect(vol.size());
}

void printsol(Nodo* &nod) {
    cout << nod->nenvases_real << " envases utilizados: " << nod->sol[0];
    printv(nod->sol);
}

int main() {
    // n = 8 volumenes
    t_vect volumen = { 1, 5, 2, 10, 20, 50, 100, 200 };

    // generación de datos datos
    t_vect cantidad = t_vect(volumen.size());
    for(int i = 0; i < volumen.size(); i++) 
        cantidad[i] = 1 + (rand() % 15);
    
    Nodo* solnod = envases(10, volumen);

    printsol(solnod);
   
    return 0;
}
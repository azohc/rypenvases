/*
autor: Juan Chozas

Envasado de objetos

Se tiene una colección denobjetos “moldeables”, cada uno con un volumen vi, para i entre 1 y n, que hay que empaquetar utilizando envase decapacidad E. 
Queremos conocer el empaquetamiento óptimo, es decir, que minimice la cantidad de envase utilizados, teniendo en cuenta que los objetosno se pueden fraccionar.

Supondremos que todo objeto cabe en un envase vacío. 
Ya que necesitamos como mucho n envase, podemos numerar los envase del 1 al n.
Las soluciones podemos representarlas en tuplas de la forma(x1, . . . ,xn), siendo xi el envase donde hemos colocado el objeto i.
Como todos los envase vacíos son iguales, para cada objeto se puede usar uno de los envase ya ocupados, si cabe en alguno, o coger uno cualquiera sin usar (vacío). 
El primer objeto siempre se colocará en el primer envase (x1=1).


---------------



Normas para la presentaci ́on de la pr ́actica

5.  Se ejecutar ́a el programa con al menos tres juegos de datos distintos, con datos lo mas volu-minosos posibles.

6.  Para podar, se utilizaran 
    al menos dos estimaciones optimistas y dos pesimistas (si son aplicables):
        una ingenua y poco costosa, 
        y otra mas ajustada y posiblemente mas costosa.

7.  Se  imprimiran  el  numero  de nodos explorados cuando no se utiliza mas poda que  
    la  de factibilidad y los explorados cuando se utiliza cada una de las dos podas.

8.  Tambien se dara el tiempo total y el tiempo medio por nodo explorado en cada uno de los tres casos.

9.  Se entregaran las fuentes del programa con suficientes comentarios, los ficheros de prueba, los resultados obtenidos, 
    y una brevememoria(maximo 3 paginas) con las explicaciones adicionalesque se consideren necesarias y las conclusiones personales obtenidas.

10.  La entrega se har ́a subiendo un fichero comprimido al campus virtuala lo sumo el mi ́ercoles15 de mayo. 
    Las pr ́acticas no subidas en esa fecha se dar ́an por no entregadas.

11.  Se  podr ́an  fijar  fechas  durante  mayo  o  junio  para  hacer  una  demostraci ́on  ante  el  profesor  yresponder a sus preguntas.

*/


#include <iostream>     //IO
#include <string>       //IO
#include <fstream>      //ENTRADA FICHERO
#include <iterator>     //ITERADOR
#include <queue>        //COLA DE PRIORIDAD
#include <vector>       //ESTRUCTURA PARA LA COLA
#include <math.h>       //CEIL

using std::vector;
using std::priority_queue;
using std::iterator;
using std::max;
using std::cout;
using std::cin;
using std::endl;
using std::ifstream;
using std::string;

using t_vect = vector<int>;
int N;  // Numero de objetos a envasar

struct Nodo {   // estructura Nodo
    int k;                      // profundidad en el arbol
    int n_envases_real;         // la cantidad de envases utilizados en el nodo
    int n_envases_optimista;    // cota inferior de n_envases_real para la configuracion de envases
    int capacidad_restante;     // capacidad restante de los envases utilizados: para un calculo menos costoso de la cota inferior
    t_vect *v_envases;          // volumenes asociados a los envases [0, n)
    t_vect *sol;                // envases asociados a los objetos [0, n)
}; 
struct Comparacion_Nodos {      
    bool operator()(const Nodo* l, const Nodo* r) const {   // comparacion en funcion del n_envases_optimista
        if (l->n_envases_optimista == r->n_envases_optimista)       // si hay dos nodos con la misma cota inferior,
            return l->n_envases_real > r->n_envases_real;           // se expande el que tenga menor n_envases_real
        else         
            return l->n_envases_optimista > r->n_envases_optimista; // expandiremos antes el que tenga menor cota inferior
    } 
};

// cola de prioridad: ordena punteros a nodos en un vector con la comparacion definida
using t_prioq = priority_queue<Nodo*, vector<Nodo*>, Comparacion_Nodos>;    

void printv(const t_vect *v) {
    t_vect::const_iterator i = v->cbegin();
    while (i != v->cend()) 
        cout << *i++ << " ";
    cout << endl;
}

// borra un nodo despues de borrar sus vectores
void borraNodo(Nodo* &nod) {
    nod->v_envases->~vector();
    nod->sol->~vector();
    delete nod->v_envases;
    delete nod->sol;
    delete nod;
}

// dado un nodo, devuelve un puntero a una copia del nodo
Nodo* copiaNodo(Nodo* &n) {
    Nodo *nod = new Nodo; 
    nod->k = n->k;
    nod->n_envases_real = n->n_envases_real;
    nod->n_envases_optimista = n->n_envases_optimista;
    nod->capacidad_restante = n->capacidad_restante;

    nod->v_envases = new t_vect(N);
    *nod->v_envases = *n->v_envases;
    nod->sol = new t_vect(N);
    *nod->sol = *n->sol;
    return nod;
}

// cota superior sencilla: cada objeto es emparejado con un envase 
int empaq_pesimista_sencillo(Nodo* &nod) {
    return N;
}

// cota superior mas elaborada: calcula el numero de envases de forma voraz
// para los objetos restantes, cada objeto se coloca en el primer envase que tenga sitio para el
int empaq_pesimista_voraz(const int E, Nodo* n, const t_vect *vol) {
    Nodo *nod = copiaNodo(n);   // nod = copia del nodo: para poder manipularlo

    int abiertos = nod->n_envases_real;

    // para cada objeto que queda por envasar
    for (int i = nod->k; i < N; i++) {
        if (!abiertos) {    // si no hay envases abiertos
            abiertos++;                 // se abre un envase
            nod->sol->at(i) = 0;            // se mete al envase vacio
            nod->v_envases->at(0) += vol->at(i);
        } else {            // si hay envases abiertos
            int j;
            for (j = 0; j < abiertos; j++) {    // miramos si cabe en alguno
                if (vol->at(i) + nod->v_envases->at(j) <= E) {      // si cabe en un envase abierto
                    nod->sol->at(i) = j;                        
                    nod->v_envases->at(j) += vol->at(i);            // se mete mete en el primero que tenga sitio
                    break;
                }
            }
            if (j == abiertos) {   // si no cupo en un envase abierto
                nod->sol->at(i) = j;                // se mete a uno nuevo
                nod->v_envases->at(j) = vol->at(i);
                abiertos++;
            }
        }
    }
    borraNodo(nod);     // borrar el nodo copiado
    return abiertos;
}

// cota inferior optima: volumen total de los objetos dividido por la capacidad de un envase
int empaq_optimista_sencillo(const int E, const t_vect *vol) {
    float retval = 0.0;
    
    for (int i = 0; i < N; i++)
        retval += vol->at(i);
    retval = retval/E;

    return ceil(retval);
}

// cota inferior mas realista: tiene en cuenta el volumen restante 
int empaq_optimista_realista(const int E, Nodo* n, const t_vect *vol, const t_vect *v_pendiente) {
    float d = 0.0;
    d += (v_pendiente->at(n->k) - n->capacidad_restante) / E;
    return n->n_envases_real + (0 >= ceil(d)) ? 0 : ceil(d);
}


Nodo* envase(const int E, const t_vect *vol, int &explf, int &explp, const t_vect *v_pendiente) {
    // declaracion cola prioridad y nodos
    Nodo* x;
    Nodo* y = new Nodo;    
    Nodo* sol_mejor = new Nodo;
    t_prioq pq = t_prioq();

    y->k = 0;
    y->n_envases_real = 0;
    y->capacidad_restante = 0;
    y->sol = new t_vect(N, -1);
    y->v_envases = new t_vect(N, 0);
    y->n_envases_optimista = empaq_optimista_realista(E, y, vol, v_pendiente);
    // y->n_envases_optimista = empaq_optimista_sencillo(E, vol);
    
    explf++;
    pq.push(y);
    int n_envases_mejor = empaq_pesimista_voraz(E, y, vol);

    while (!pq.empty() && pq.top()->n_envases_optimista < n_envases_mejor) {
        y = pq.top();
        pq.pop();

        // para los hijos del nodo y, el objeto se puede meter en el envase i
        for (int i = 0; i <= y->n_envases_real; i++) {  //  0 <= i <= n_envases_real
            if (vol->at(y->k) + y->v_envases->at(i) <= E) {          // si cabe en el envase abierto i
               
                x = copiaNodo(y);
                explf++;

                if(i == y->n_envases_real) {        // si i == n_envases_real, significa que se esta usando un envase nuevo
                    x->n_envases_real++;        //incrementamos los envases abiertos
                    x->capacidad_restante += E; //sumamos a la capacidad restante de los envases abiertos
                }

                x->k++;
                x->capacidad_restante -= vol->at(y->k); //restamos el volumen del objeto de la capacidad restante de los envases abiertos
                x->sol->at(y->k) = i;
                x->v_envases->at(i) += vol->at(y->k);
                x->n_envases_optimista = empaq_optimista_realista(E, y, vol, v_pendiente);
                // x->n_envases_optimista = empaq_optimista_sencillo(E, vol);

            } else {
                continue;
            }
            if (x->k == N){     // es-solucion
                if(x->n_envases_real < n_envases_mejor) {
                    n_envases_mejor = x->n_envases_real;
                    sol_mejor = x;
                }
            } else {                        // !es-solucion
                if(x->n_envases_optimista < n_envases_mejor) {  // merece la pena expandirlo
                    explp++;
                    pq.push(x);
                    int pesimista = empaq_pesimista_voraz(E, x, vol);
                    (n_envases_mejor > pesimista) ? n_envases_mejor = pesimista : n_envases_mejor = n_envases_mejor;
                } else {                                        
                    borraNodo(x);   
                }
            }
        }
        borraNodo(y);
    }
    return sol_mejor;
}

void printsol(const int E, Nodo* &nod, const t_vect *vol) {
    cout << "distribucion de " << N << " objetos en envases de capacidad " << E << endl;
    cout << "volumenes:\t\t";
    printv(vol);
    cout << nod->n_envases_real << " envases utilizados:\t";
    printv(nod->sol);
    cout << "estado de los envases: ";
    printv(nod->v_envases);
}

// lee fichero input_n=16.txt, input_n=32.txt, o input_n=64.txt de la carpeta /inputs
t_vect* readinputfile() {
    t_vect *v = new t_vect();
    ifstream f;
    int n = -1;
    cout << "1: n = 16. 2: n = 32. 3: n = 64." << endl << "elige un fichero (del 1 al 3): ";
    string path;
    while(n < 1 || n > 3) {
        cin >> n;
        
        if(n == 1) {
            path = "inputs/input_n=16.txt";
            N = 16;
        } else if (n == 2) {
            path = "inputs/input_n=32.txt";
            N = 32;
        } else if (n == 3) {
            path = "inputs/input_n=64.txt";
            N = 64;
        } else {
            cout << "error. elige un numero del 1 al 3: ";
        }
    }
    f.open(path);
    if(f.is_open())
        cout << "cargando fichero " << path << "." << endl;
    else { 
        cout << "error en la lectura del fichero " << path << "." << endl 
            << "devolviendo vector vacio..." << endl;
        return v;
    }
    
    while(!f.eof()) {
        f >> n;
        v->push_back(n);
    }
    
    f.close();
    cout << endl;

    return v;
}



int main() {    
    const int E = 10;
    t_vect *vol = readinputfile();
    t_vect *v_pendiente = new t_vect(N);
    int aux = 0;
    for(int i = N - 1; i != -1; i--) {
        v_pendiente->at(i) = vol->at(i) + aux;
        aux += vol->at(i);
    }

    int explfactibles = 0, explpodados = 0;

    Nodo* solnod = envase(E, vol, explfactibles, explpodados, v_pendiente);

    printsol(E, solnod, vol);
    cout << "nodos explorados (factibles): " << explfactibles << endl;
    cout << "nodos explorados (no podados): " << explpodados << endl;
   
    return 0;
}
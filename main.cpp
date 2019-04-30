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


Normas para la presentacion de la practica

5.  Se ejecutara el programa con al menos tres juegos de datos distintos, con datos lo mas voluminosos posibles.

6.  Para podar, se utilizaran 
    al menos dos estimaciones optimistas y dos pesimistas (si son aplicables):
        una ingenua y poco costosa, 
        y otra mas ajustada y posiblemente mas costosa.

7.  Se  imprimiran  el  numero  de nodos  explorados cuando  no  se  utiliza mas  poda  que  
    la  de factibilidad y los explorados cuando se utiliza cada una de las dos podas.

8.  Tambien se dara el tiempo total y el tiempo medio por nodo explorado en cada uno de los tres casos.

9.  Se entregaran las fuentes del programa con suficientes comentarios, los ficheros de prueba, los resultados obtenidos, 
    y una breve memoria (maximo 3 paginas) con las explicaciones adicionales que se consideren necesarias y las conclusiones personales obtenidas.

10.  La entrega se hara subiendo un fichero comprimido al campus virtual a lo sumo el miercoles 15 de mayo. 
    Las practicas no subidas en esa fecha se daran por no entregadas.

11.  Se  podran  fijar  fechas  durante  mayo  o  junio  para  hacer  una  demostracion  ante  el  profesor  y responder a sus preguntas.

*/


#include <iostream>     //IO
#include <string>       //IO
#include <fstream>      //ENTRADA FICHERO
#include <iterator>     //ITERADOR
#include <queue>        //COLA DE PRIORIDAD
#include <vector>       //ESTRUCTURA PARA LA COLA
#include <math.h>       //CEIL
#include <sys/time.h>   //MEDIDAS DE TIEMPO

using std::vector;
using std::priority_queue;
using std::iterator;
using std::min;
using std::cout;
using std::cin;
using std::endl;
using std::ifstream;
using std::string;

// Para medir el tiempo 
typedef unsigned long long utime_t;
static utime_t get_time ()      
{
    struct timeval now;
    gettimeofday(&now, NULL);
    return  now.tv_usec + (utime_t)now.tv_sec * 1000000;
}


using t_vect = vector<int>;
int N;  // numero de objetos a envasar

struct Nodo {   // estructura Nodo
    int k;                      // profundidad en el arbol
    int n_envases_real;         // la cantidad de envases utilizados en el nodo
    int n_envases_optimista;    // cota inferior de n_envases_real para la configuracion de envases
    t_vect v_envases;          // volumenes asociados a los envases [0, n)
    t_vect sol;                // envases asociados a los objetos [0, n)
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

void printv(const t_vect &vol) {
    t_vect::const_iterator i = vol.cbegin();
    while (i != vol.cend()) 
        cout << *i++ << " ";
    cout << endl;
}

// cota superior sencilla: devuelve N, asociando cada objeto a un envase
int empaq_pesimista_sencilla(Nodo* &nod) {
    return N;
}

/* cota superior mas elaborada: 
calcula los envases necesarios de forma voraz para los objetos restantes
cada objeto se coloca en el primer envase que tenga sitio para el
*/
int empaq_pesimista_elaborada(const int E, Nodo* n, const t_vect vol) {
    t_vect v_envases_aux = n->v_envases;

    int abiertos = n->n_envases_real;
    // para cada objeto que queda por envasar
    for (int i = n->k; i < N; i++) {
        if (!abiertos) {    // si no hay envases abiertos
            abiertos++;                     // se abre un envase
            v_envases_aux[0] += vol[i];    // y se mete al primero
        } else {                                        // si hay envases abiertos
            int j;
            for (j = 0; j < abiertos; j++) {    // miramos si cabe en alguno
                if (vol[i] + v_envases_aux[j] <= E) {   // si cabe en un envase abierto
                    v_envases_aux[j] += vol[i];             // se mete mete en primero que tenga sitio
                    break;
                }
            }
            if (j == abiertos) {   // si no cupo en un envase abierto
                v_envases_aux[j] = vol[i]; // se mete a uno nuevo
                abiertos++;
            }
        }
    }
    return abiertos;
}

/* cota inferior sencilla 
da lugar a la solución óptima: volumen total de los objetos dividido por la capacidad de un envase
*/
int empaq_optimista_sencilla(const int E, const t_vect &vol) {
    float retval = 0.0;
    
    for (int i = 0; i < N; i++)
        retval += vol[i];
    retval = retval/E;

    return ceil(retval);
}

// cota inferior mas elaborada: calcula el numero de envases contemplando los objetos restantes
// y los envases utilizados. considera que los objetos son NO fraccionables
int empaq_optimista_elaborada(const int E, Nodo* n, const t_vect &vol) {
    return 0;
}


Nodo* envase(const int E, const t_vect &vol, int &exp_factibles, int &explp, double &t) {
    Nodo* x;
    Nodo* y = new Nodo;    
    Nodo* sol_mejor = new Nodo;
    t_prioq pq = t_prioq();

    y->k = 0;
    y->n_envases_real = 0;
    y->sol = t_vect(N, -1);
    y->v_envases = t_vect(N, 0);
    // y->n_envases_optimista = empaq_optimista_elaborada(E, y, vol);
    y->n_envases_optimista = empaq_optimista_sencilla(E, vol);
    
    bool encontrada = false;
    int optimo = empaq_optimista_sencilla(E, vol);

    pq.push(y);
    int n_envases_mejor = empaq_pesimista_elaborada(E, y, vol);
                
    utime_t t0, t1;

    // n_envases_optimista <= n_envases_mejor: <= en vez de <: si n_envases mejor = numero de envases óptimo, es imposible que n_envases_optimista sea menor
    while (!pq.empty() && !encontrada &&  pq.top()->n_envases_optimista <= n_envases_mejor) { 
        y = pq.top();
        pq.pop();

        // para los hijos del nodo y, el objeto se puede meter en el envase i
        for (int i = 0; !encontrada && i <= y->n_envases_real; i++) {  // y->n_envases_real >= i >= 0 : i puede ser = a y->n_envases_real para contemplar el caso en el que se abre un envase nuevo
            if (vol[y->k] + y->v_envases[i] <= E) {          // es factible si cabe en el envase i
                exp_factibles++;


                t0 = get_time();    // Mido el tiempo que tarda en generarse el nodo
                x = new Nodo;
                x->k = y->k + 1;
                x->sol = y->sol;
                x->sol[y->k] = i;
                x->v_envases = y->v_envases;
                x->v_envases[i] += vol[y->k];
                x->n_envases_real = y->n_envases_real;
                x->n_envases_optimista = empaq_pesimista_elaborada(E, x, vol);
                // x->n_envases_optimista = y->n_envases_optimista;
                if(i == y->n_envases_real)
                    x->n_envases_real++;
                t1 = get_time();
                t += (t1 - t0)/1000.0L;
            } else {    // si no es factible cabe continuamos: no queremos generar el nodo x si no puede llegar a ser solucion
                continue;
            }
            t0 = get_time(); // tambien mido lo que tarda en ejecutar este if else
            if (x->k == N){     // es-solucion
                if(x->n_envases_real <= n_envases_mejor) { //<= en vez de <: si n_envases mejor = numero de envases óptimo, es imposible que n_envases_real sea menor
                    n_envases_mejor = x->n_envases_real;
                    sol_mejor = x;
                    encontrada = x->n_envases_real == optimo;
                }
            } else {                        // !es-solucion
                if(x->n_envases_optimista <= n_envases_mejor) {  // merece la pena expandirlo
                    explp++;
                    pq.push(x);
                    int pesimista = empaq_pesimista_elaborada(E, y, vol);
                    n_envases_mejor = min(pesimista, n_envases_mejor);
                } else {                                        
                    delete x;
                }
            }
            t1 = get_time();
            t += (t1 - t0)/1000.0L;
        }
        delete y;
    }
    return sol_mejor;
}

void printsol(const int E, Nodo* &nod, const t_vect &vol) {
    t_vect env = nod->v_envases;
    env.resize(nod->n_envases_real);
    cout << "distribucion de " << N << " objetos en envases de capacidad " << E << endl;
    cout << nod->n_envases_real << " envases utilizados:" << endl;

    cout << "volumenes de los objetos:\t\t";      printv(vol);
    cout << "envases asignados a los objetos:\t"; printv(nod->sol);
    cout << "estado de los envases:\t\t\t";       printv(env);
}

// lee fichero input_n=16.txt, input_n=32.txt, o input_n=64.txt de la carpeta inputs
t_vect readinputfile() {
    t_vect v;
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
        v.push_back(n);
    }
    
    f.close();
    cout << endl;

    return v;
}

int main() {
    
    const int E = 10;
    t_vect vol = readinputfile();
     
    int exp_factibles = 0, exp_podados = 0; // almacena los nodos explorados
    double t = 0;                           // almacena el tiempo de ejecución transcurrido en explorar los nodos

    Nodo* solnod = envase(E, vol, exp_factibles, exp_podados, t);

    printsol(E, solnod, vol);
    cout << "nodos explorados (factibles): " << exp_factibles << endl;
    cout << "nodos explorados (no podados): " << exp_podados << endl;

    cout << "tiempo transcurrido en explorar " << exp_factibles << " nodos: " << t << " ms." << endl;
    cout << "promedio del tiempo transcurrido en explorar un nodo: " << t/exp_factibles << endl;
   
    return 0;
}
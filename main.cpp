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

10.  Tambien se dara el tiempo total y el tiempo medio por nodo explorado en cada uno de los tres casos.

9.  Se entregaran las fuentes del programa con suficientes comentarios, los ficheros de prueba, los resultados obtenidos, 
    y una breve memoria (maximo 3 paginas) con las explicaciones adicionales que se consideren necesarias y las conclusiones personales obtenidas.

*/


#include <iostream>     //IO
#include <string>       //IO
#include <fstream>      //ENTRADA FICHERO
#include <iterator>     //ITERADOR
#include <queue>        //COLA DE PRIORIDAD
#include <vector>       //ESTRUCTURA PARA LA COLA
#include <math.h>       //CEIL
#include <sys/time.h>   //MEDIDAS DE TIEMPO

#include <stdlib.h>     /* atoi */

using std::vector;
using std::priority_queue;
using std::queue;
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
int E = 10;  // volumen de los envases

// tipos de cotas
const int SIN_COTAS = 0;
const int SENCILLAS = 1; 
const int ELABORADAS = 2;

struct Nodo {   // estructura Nodo
    int k;                      // profundidad en el arbol
    int n_envases_real;         // la cantidad de envases utilizados en el nodo
    int n_envases_optimista;    // cota inferior de n_envases_real para la configuracion de envases
    t_vect v_envases;           // volumenes asociados a los envases [0, n)
    t_vect sol;                 // envases asociados a los objetos [0, n)
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

// devuelve la solucion optima, que se halla tratando a los objetos como fraccionables
int sol_optima(const t_vect &vol){
    float retval = 0.0;
    
    for (int i = 0; i < N; i++)
        retval += vol[i];
    retval = retval/E;

    return ceil(retval);
}

// cota superior sencilla: devuelve N, asociando cada objeto a un envase
int empaq_pesimista_sencilla(Nodo* &nod) {
    return N;
}

/* 
cota superior mas elaborada: 
calcula los envases necesarios de forma voraz para los objetos restantes
cada objeto se coloca en el primer envase que tenga sitio para el
*/
int empaq_pesimista_elaborada(Nodo* n, const t_vect vol) {
    t_vect v_envases_aux = n->v_envases;

    int abiertos = n->n_envases_real;
    // para cada objeto que queda por envasar
    for (int i = n->k; i < N; i++) {
        if (!abiertos) {    // si no hay envases abiertos
            abiertos++;                     // se abre un envase
            v_envases_aux[0] += vol[i];    // y se mete al primero
        } else {                                        // si hay envases abiertos
            int j;
            for (j = abiertos - 1; j >= 0; j--) {    // miramos si cabe en alguno
                if (vol[i] + v_envases_aux[j] <= E) {   // si cabe en un envase abierto
                    v_envases_aux[j] += vol[i];             // se mete mete en primero que tenga sitio
                    break;
                }
            }
            if (j == -1) {   // si no cupo en un envase abierto
                v_envases_aux[abiertos] = vol[i]; // se mete a uno nuevo
                abiertos++;
            }
        }
    }
    return abiertos;
}

/* 
cota inferior sencilla: en el mejor de los casos, no se utilizan mas envases
*/
int empaq_optimista_sencilla(Nodo* n) {
    return n->n_envases_real;
}

/* 
cota inferior mas elaborada: estima el numero de envases contemplando los objetos restantes
y los envases utilizados. trata a los objetos como fraccionables
*/
int empaq_optimista_elaborada(Nodo* n, const t_vect &vol) {

    // calcular capacidad restante en los envases que ya han sido utilizados
    int capacidad_env_abiertos = 0;

    for(int i = 0; i < n->n_envases_real; i++) {
        capacidad_env_abiertos += E - n->v_envases[i];  // capacidad = E - volumen
    }
    
    // calcular volumen total de los objetos que aun no han sido envasados
    float vol_envases_restantes = 0.0;    
    for (int i = n->k; i < N; i++)
        vol_envases_restantes += vol[i];

    // se utiliza la capacidad de los envases abiertos.
    vol_envases_restantes -= capacidad_env_abiertos;

    // si despues de llenar los envases abiertos con volumen de objetos restantes
    if(vol_envases_restantes <= 0) // queda un volumen de objetos restantes <= 0
        return n->n_envases_real; // no hace falta utilizar mas envases

    // en caso de que siga habiendo un volumen de objetos restantes mayor que cero
    float estimacion = vol_envases_restantes/E; // se envasan con trato fraccionable

    // se devuelve el numero de envases actual + estimacion
    return n->n_envases_real + ceil(estimacion); 
}


Nodo* envase(int cotas, const t_vect &vol, int &explorados) {
    Nodo* x;
    Nodo* y = new Nodo;    
    Nodo* sol_mejor = new Nodo;
    t_prioq pq = t_prioq();

    y->k = 0;
    y->n_envases_real = 0;
    y->sol = t_vect(N, -1);
    y->v_envases = t_vect(N, 0);
    y->n_envases_optimista = (cotas == SENCILLAS) ? empaq_optimista_sencilla(y) : empaq_optimista_elaborada(y, vol);

    pq.push(y);
    int optimo = sol_optima(vol); // solucion optima: sirve para terminar de buscar en caso de encontrarse

    int n_envases_mejor = (cotas == SENCILLAS) ? empaq_pesimista_sencilla(y) : empaq_pesimista_elaborada(y, vol);
                
    // n_envases_optimista <= n_envases_mejor: <= en vez de <: si n_envases mejor = numero de envases óptimo, es imposible que n_envases_optimista sea menor
    while (!pq.empty() && pq.top()->n_envases_optimista <= n_envases_mejor) { 
        y = pq.top();
        pq.pop();
        explorados++;

        // para los hijos del nodo y, el objeto se puede meter en el envase i
        for (int i = 0; i <= y->n_envases_real; i++) {  // y->n_envases_real >= i >= 0 : i puede ser = a y->n_envases_real para contemplar el caso en el que se abre un envase nuevo
            if (vol[y->k] + y->v_envases[i] <= E) {          // es factible si cabe en el envase i

                x = new Nodo;
                x->k = y->k + 1;
                x->sol = y->sol;
                x->sol[y->k] = i;
                x->v_envases = y->v_envases;
                x->v_envases[i] += vol[y->k];
                x->n_envases_real = y->n_envases_real;
                if(i == y->n_envases_real)
                    x->n_envases_real++;

                x->n_envases_optimista = (cotas == SENCILLAS) ? empaq_optimista_sencilla(x) : empaq_optimista_elaborada(x, vol);

            } else {    // si no es factible cabe continuamos: no queremos generar el nodo x si no puede llegar a ser solucion
                continue;
            }
            if (x->k == N){     // es-solucion
                if(x->n_envases_real <= n_envases_mejor) { //<= en vez de <: si n_envases mejor = numero de envases óptimo, es imposible que n_envases_real sea menor
                    n_envases_mejor = x->n_envases_real;
                    sol_mejor = x;
                    if(x->n_envases_real == optimo)
                        return sol_mejor;
                }
            } else {                        // !es-solucion
                if(x->n_envases_optimista <= n_envases_mejor) {  // merece la pena expandirlo
                    pq.push(x);
                    int pesimista = (cotas == SENCILLAS) ? empaq_pesimista_sencilla(y) : empaq_pesimista_elaborada(y, vol);
                    n_envases_mejor = min(pesimista, n_envases_mejor);
                } else {                                        
                    delete x;
                }
            }
        }
        delete y;
    }
    return sol_mejor;
}

Nodo* envase_sin_cotas(int cotas, const t_vect &vol, int &explorados) {
    
    Nodo* x;
    Nodo* y = new Nodo;    
    queue<Nodo*> q;
    
    Nodo* sol_mejor = new Nodo;

    y->k = 0;
    y->n_envases_real = 0;
    y->sol = t_vect(N, -1);
    y->v_envases = t_vect(N, 0);
    

    q.push(y);
    int optimo = sol_optima(vol); // solucion optima: sirve para terminar de buscar en caso de encontrarse

    int n_envases_mejor = __INT_MAX__; // solucion mejor es +infinito para empezar
                
    // n_envases_optimista <= n_envases_mejor: <= en vez de <: si n_envases mejor = numero de envases óptimo, es imposible que n_envases_optimista sea menor
    while (!q.empty()) { 
        y = q.front();
        q.pop();
        explorados++;

        // para los hijos del nodo y, el objeto se puede meter en el envase i
        for (int i = 0; i <= y->n_envases_real; i++) {  // y->n_envases_real >= i >= 0 : i puede ser = a y->n_envases_real para contemplar el caso en el que se abre un envase nuevo
            if (vol[y->k] + y->v_envases[i] <= E) {          // es factible si cabe en el envase i

                x = new Nodo;
                x->k = y->k + 1;
                x->sol = y->sol;
                x->sol[y->k] = i;
                x->v_envases = y->v_envases;
                x->v_envases[i] += vol[y->k];
                x->n_envases_real = y->n_envases_real;
                if(i == y->n_envases_real)
                    x->n_envases_real++;


            } else {    // si no es factible cabe continuamos: no queremos generar el nodo x si no puede llegar a ser solucion
                continue;
            }
            if (x->k == N){     // es-solucion
                if(x->n_envases_real <= n_envases_mejor) { //<= en vez de <: si n_envases mejor = numero de envases óptimo, es imposible que n_envases_real sea menor
                    n_envases_mejor = x->n_envases_real;
                    sol_mejor = x;
                    if(x->n_envases_real == optimo)
                        return sol_mejor;
                }
            } else {                        // !es-solucion
                q.push(x);
            }
        }
        delete y;
    }
    return sol_mejor;
}

void printsol(Nodo* &nod, const t_vect &vol) {
    t_vect env = nod->v_envases;
    env.resize(nod->n_envases_real);
    cout << "distribucion de " << N << " objetos en envases de capacidad " << E << endl;
    cout << nod->n_envases_real << " envases utilizados" << endl;

    cout << "\t" << "volumenes de los objetos:\t";      printv(vol);
    cout << "\t" << "envases asignados a objetos:\t";    printv(nod->sol);
    cout << "\t" << "estado de los envases:\t\t";       printv(env);
}

// lee fichero de la carpeta inputs
t_vect readinputfile(int arg) {
    t_vect v;
    ifstream f;
    int n = -1;
    string path;

    if(!arg){
        cout << "1: n = 12. 2: n = 18. 3: n = 24" << endl << "elige un fichero (del 1 al 3): ";
        while(n < 1 || n > 3) {
            cin >> n;
            
            if(n == 1) {
                path = "inputs/input_n=12.txt";
                N = 12;
            }else if (n == 2) {
                path = "inputs/input_n=18.txt";
                N = 18;
            } else if (n == 3) {
                path = "inputs/input_n=24.txt";
                N = 24;
            } else {
                cout << "error. elige un numero del 1 al 3: ";
            }
        }
    }
    else {
        if(arg == 12) {
            path = "inputs/input_n=12.txt";
            N = 12;
        } else if (arg == 18) {
            path = "inputs/input_n=18.txt";
            N = 18;
        } else if (arg == 24) {
            path = "inputs/input_n=24.txt";
            N = 24;
        } else {
            cout << "error. no existe el fichero de entrada con N=" << arg << endl;
            exit;
        }
    }
    f.open(path);
    if(f.is_open())
        cout << "cargando fichero " << path << "...";
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
    cout << " fichero cargado." << endl;

    return v;
}
int eleccioncotas(int argc, char* argv[]) {
    int cotas = argc > 2 ? atoi(argv[2]) : -1;  // si no hay parametros en argv, pedirlas por stdin
    while (cotas != SENCILLAS && cotas != ELABORADAS && cotas != SIN_COTAS) {
        cout << "0: sin cotas, 1: cotas sencillas, 2: cotas elaboradas" << endl;
        cout << "elige las cotas (0, 1, o 2): ";
        cin >> cotas;
    }
    if(cotas == SENCILLAS) {
        cout << "usando cotas sencillas...";
    } else if (cotas == ELABORADAS) {
        cout << "usando cotas elaboradas...";
    } else {
        cout << "usando solamente la poda de factibilidad...";
    }
    cout << endl << endl;
    
    return cotas;
}

int main(int argc, char* argv[]) {
    t_vect vol;

    // se puede pasar N = 12, 18, 24 como primer argumento (argv[1])
    // se puede pasar 0 (sin cotas), 1 (cotas sencillas) o 2 (cotas elaboradas) como segundo argumento (argv[2])
    if(argc >= 2) 
        vol = readinputfile((atoi(argv[1])));
    else    // sin argumentos se utiliza entrada estandar para elegir N y tipo de cotas
        vol = readinputfile(0);
     
    int explorados = 0; // almacena los nodos explorados

    int cotas = eleccioncotas(argc, argv);

    double tcotas = 0, t = 0;       // almacena el tiempo de ejecución transcurrido en explorar los nodos
    utime_t t1, t0; 
    Nodo* solnod;

    // ejecucion del algoritmo en try-catch para poder imprimir el numero de nodos expandidos cuando salta una excepcion
    try {
        if(cotas == SENCILLAS || cotas == ELABORADAS) {
            // con cotas
            t0 = get_time();
            solnod = envase(cotas, vol, explorados);
            t1 = get_time();
        } else {
            // sin cotas
            t0 = get_time();
            solnod = envase_sin_cotas(cotas, vol, explorados);
            t1 = get_time();
        } 
    } catch (const std::bad_alloc& e) {
        cout << "nodos explorados antes de lanzarse la excepcion " << e.what() << ": " << explorados << endl;
        return -1;
    }

    t += (t1 - t0)/1000.0L;

    printsol(solnod, vol);
    cout << endl;
    cout << "nodos explorados: " << explorados << endl;

    cout << "tiempo transcurrido en explorar " << explorados << " nodos: " << t << " ms." << endl;
    cout << "promedio del tiempo por nodo explorado: " << t/explorados << " ms." << endl 
        << "______________________________________________________________________________________" << endl;

    return explorados;
}
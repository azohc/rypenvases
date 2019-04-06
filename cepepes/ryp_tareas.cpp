#include <iostream>
#include <vector>
#include <queue>
#include <iterator>
#include <algorithm>

using std::vector;
using std::priority_queue;
using std::iterator;
using std::cout;
using std::endl;

using t_vect = vector<int>;

struct Nodo { 
    int k; 
    int tiempo_real;
    int tiempo_optimista;
    
    // tiempo total de ejecucion para las tareas asignadas al procesador i. 
    t_vect sol; 

}; 
struct Comparacion_Nodos {
    bool operator()(const Nodo* l, const Nodo* r) const {
        if(l->tiempo_optimista == r->tiempo_optimista) 
            return l->tiempo_real < r->tiempo_real;
        else         
            return l->tiempo_optimista < r->tiempo_optimista;
    } 
};

using t_prioq = priority_queue<Nodo*, vector<Nodo*>, Comparacion_Nodos>;

void printv(const t_vect &v) {
    t_vect::const_iterator i = v.cbegin();
    while(i != v.cend()) 
        cout << *i++ << " ";
    cout << endl;
}

int max_indice(const t_vect &v) {
    int max = INTMAX_MIN;
    int retval = 0;
    for(int i : v) 
        if(v[i] > max) {
            retval = i;    
            max = v[i];
        }    
    return retval;
}

int min_indice(const t_vect &v) {
    int min = INTMAX_MAX;
    int retval = 0;
    for(int i : v) 
        if(v[i] < min) {
            retval = i;    
            min = v[i];
        }          
    return retval;
}

// devuelve el tiempo total de ejecucion para 
// el procesador que ejecuta la ultima tarea == el que mas tarde acaba 
int tiempo_voraz(Nodo* nod, const t_vect &t, const int m) {
    t_vect auxsol = nod->sol;

    // asigna las tareas restantes al procesador con min tiempo de ejecucion
    for(int i = nod->k; i < t.size(); i++) 
        auxsol[min_indice(auxsol)] = t[i];
  
    return auxsol[max_indice(auxsol)];
}

void tareas(Nodo* &solmejor, const t_vect &t, const int m) {
    Nodo *x, *y;
    t_prioq pq;

    x = new Nodo;
    y = new Nodo;

    y->k = 0;
    y->sol = t_vect(m);
    y->tiempo_real = 0;

    x->tiempo_optimista = 5;
    y->tiempo_optimista = 7;

    pq.push(x);
    pq.push(y);
    cout << endl;
    cout << pq.top();

    
}

int main() {
    // n = 8 trabajos
    t_vect t = { 4, 9, 6, 3, 5, 8, 6, 2 };
    std::sort(t.begin(), t.end(), std::greater<int>());
    // m procesadores
    const int m = 4;

    Nodo *solnod;

    tareas(solnod, t, m);
    
    printv(solnod->sol);

    printv(t);
    return 0;
}
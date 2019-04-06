#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iterator>

using std::vector;
using std::priority_queue;
using std::iterator;
using std::min;
using std::cout;
using std::endl;

struct Nodo { 
    int k;
    int c_pagada;
    int nummonedas_real;
    int nummonedas_optimista;
    vector<int> sol;
}; 
struct Comparacion_Nodos {
    bool operator()(const Nodo* l, const Nodo* r) const {
        if(l->nummonedas_optimista == r->nummonedas_optimista) 
            return l->nummonedas_real < r->nummonedas_real;
        else         
            return l->nummonedas_optimista > r->nummonedas_optimista;
    } 
};

using t_prioq = priority_queue<Nodo*, vector<Nodo*>, Comparacion_Nodos>;

void printv(const vector<int> &v) {
    vector<int>::const_iterator i = v.cbegin();
    while(i != v.cend()) 
        cout << *i++ << " ";
    cout << endl;
}

int cantidad_voraz(const int c, Nodo* &nod, const vector<int> &m, vector<int> cantidad) {
    for(int i = 0; i < nod->k; i++) 
        cantidad[i] - nod->sol[i];
    
    int retval = 0;
    int c_restante = c - nod->c_pagada; 
    int i = nod->k;
    while(c_restante && i < m.size()) {
        if(m[i] > c_restante || !cantidad[i])
            i++;
        else {
            if(cantidad[i]){
                cantidad[i]--;
                c_restante -= m[i];
                retval++;
            }
        }
    }
    return retval;
}

bool monedas(Nodo* &solmejor, const int c, const vector<int> &m, const vector<int> &cantidad) {
    bool hasSol = false;
    Nodo *x, *y = new Nodo; 
    y->k = 0;
    y->c_pagada = 0.0;
    y->nummonedas_real = 0;
    y->sol = vector<int>(m.size());
    y->nummonedas_optimista = cantidad_voraz(c, y, m, cantidad);

    t_prioq pq = t_prioq();
        
    pq.push(y);
    int nummonedas_mejor = __INT_MAX__;

    while(!pq.empty() && pq.top()->nummonedas_optimista < nummonedas_mejor) {
        y = pq.top();
        pq.pop();

        for(int i = 0; i <= cantidad[y->k] && y->c_pagada + i*m[y->k] <= c; i++) { // para todo hijo de y
            x = new Nodo;  
            x->k = y->k + 1;
            x->nummonedas_real = y->nummonedas_real + i;
            x->c_pagada = y->c_pagada + i*m[y->k];
            x->nummonedas_optimista = x->nummonedas_real + cantidad_voraz(c, x, m, cantidad);
            x->sol = y->sol;
            x->sol[y->k] = i;

            if(x->c_pagada == c) { // es-solucion
                if(x->nummonedas_real < nummonedas_mejor) {
                    //cout << "solmejor actualizado: de " << nummonedas_mejor << " monedas a " << x->nummonedas_real << " monedas." << endl;
                    nummonedas_mejor = x->nummonedas_real;
                    solmejor = x;
                    hasSol = true;
                    // if(x->nummonedas_real == c)
                    //     return;
                }
            } else {  // no es-solucion
                if(x->c_pagada <= c && x->nummonedas_optimista < nummonedas_mejor) {
                    pq.push(x);
                    // int pes = llenado_pesimista(x->k, x->llenado_real, D, p);
                    // (llenado_mejor < pes) ? llenado_mejor = pes : llenado_mejor = llenado_mejor;
                    
                } else { // borrar x si no vale la pena guardarlo
                    delete x;
                }
            }
        }
    }
    return hasSol;
}

void printsol(Nodo* &nod, const int c, const vector<int> &m, const vector<int> &cant) {
    cout << "puedo pagar " << c << " con " << nod->nummonedas_real << " monedas:" <<  endl;
    bool first = true;
    for(int i = 0; i < m.size(); i++) {
        if(!first)
            cout << " + " << nod->sol[i] << "*" << m[i];
        else {
            cout << nod->sol[i] << "*" << m[i];        
            first = false;
        }
    }
    cout << " = " << nod->c_pagada << endl;
}

int main() {

    // valores, ordenados descendientemente
    vector<int> m = { 1, 5, 2, 10, 20, 50, 100, 200 };
    std::sort(m.begin(), m.end(), std::greater<int>());

    // cantidades, generadas aleatoriamente
    vector<int> cantidad = vector<int>(m.size());
    for(int i = 0; i < m.size(); i++) 
        cantidad[i] = 1 + (rand() % 15);

    // cantidad a pagar
    int c = 50 + (rand() % 3000);    
    
    Nodo* solnod;

    if(monedas(solnod, c, m, cantidad)) {
        printsol(solnod, c, m, cantidad);
    } else {
        cout << "no puedo pagar " << c << " con estas monedas: " << endl;
        printv(m);
        cout << "y estas cantidades " << endl;
        printv(cantidad);
    }
   
    return 0;
}
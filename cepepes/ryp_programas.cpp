#include <iostream>
#include <vector>
#include <queue>
#include <iterator>

using std::vector;
using std::priority_queue;
using std::iterator;
using std::min;
using std::cout;
using std::endl;


using t_vect = vector<int>;

struct Node { 
    int k; 
    int llenado_real;
    int llenado_optimista;
    t_vect sol;
}; 
struct Node_Comparison {
    bool operator()(const Node* l, const Node* r) const {
        if(l->llenado_optimista == r->llenado_optimista) 
            return l->llenado_real < r->llenado_real;
        else         
            return l->llenado_optimista < r->llenado_optimista;
    } 
};

using t_prioq = priority_queue<Node*, vector<Node*>, Node_Comparison>;

void printv(const t_vect &v) {
    t_vect::const_iterator i = v.cbegin();
    while(i != v.cend()) 
        cout << *i++ << " ";
    cout << endl;
}

void printsol(Node* &n, const t_vect &v) {
    int i = 0, acc = 0, first = 1;
    while(i < v.size()) {
        if(n->sol[i]) {
            if(!first)
                cout << " + " << v[i];
            else {
                cout << v[i];
                first = 0;
            }
        }
        acc += v[i] * n->sol[i++];
    }
    cout << " = " << acc << endl;
}

t_vect acum_tam_restante(t_vect P) {
    t_vect acumulado = t_vect(P.size());
    t_vect::reverse_iterator a = acumulado.rbegin();
    t_vect::reverse_iterator r = P.rbegin();
    int acc = 0;

    while(r != P.rend()) {
        acc += *r++;
        *a++ = acc;
    }
    return acumulado;
}

int llenado_pesimista(int k, int tam, int D, const t_vect &p) {
    t_vect::const_iterator s = p.cbegin() + k;
    while(s != p.cend()) {
        if(tam + *s <= D)
            tam += *s;
        s++;
    }
    return tam;
}

void programas(const int D, Node* &sol_mejor, const t_vect &p, const t_vect &pac) {
    Node* x;
    Node* y = new Node;    
    t_prioq pq = t_prioq();
    
    y->llenado_real = 0;
    y->k = 0;
    y->llenado_optimista = min(pac[0], D);
    y->sol = t_vect(p.size());
    
    pq.push(y);
    int llenado_mejor = llenado_pesimista(y->k, y->llenado_real, D, p);

    while(!pq.empty() && pq.top()->llenado_optimista >= llenado_mejor) {
        y = pq.top();
        pq.pop();

        for(int i = 0; i < 2; i++) { // para todo hijo de y
            x = new Node;
            if(i == 0) {    // no se coge el programa
                x->k = y->k + 1;
                x->llenado_real = y->llenado_real;
                x->llenado_optimista = min(D, x->llenado_real + pac[x->k]);
                x->sol = y->sol;
                x->sol[y->k] = 0;
            } else {        // se coge el programa
                x->k = y->k + 1;
                x->llenado_real = y->llenado_real + p[y->k];
                x->llenado_optimista = min(D, x->llenado_real + pac[x->k]);
                x->sol = y->sol;
                x->sol[y->k] = 1;
            }            
            if(x->k == p.size() && x->llenado_real <= D) { // es-solucion
                if(x->llenado_real >= llenado_mejor) {
                    // cout << "solmejor actualizado, llenado real = " << x->llenado_real << ": "; printsol(x, p);
                    llenado_mejor = x->llenado_real;
                    sol_mejor = x;
                    if(x->llenado_real == D)
                        return;
                }
            } else {  // no es-solucion
                if(x->llenado_real <= D && x->llenado_optimista >= llenado_mejor) {
                    pq.push(x);
                    int pes = llenado_pesimista(x->k, x->llenado_real, D, p);
                    (llenado_mejor < pes) ? llenado_mejor = pes : llenado_mejor = llenado_mejor;
                    
                } else { // borrar x si no vale la pena guardarlo
                    delete x;
                }
            }
        }
    }
}


int main() {

    int D = 31;
    t_vect P = { 12, 4, 18, 5, 25, 6, 4, 11 };
    t_vect Pacumulado = acum_tam_restante(P);
    Node* solnod;

    programas(D, solnod, P, Pacumulado);

    cout << "Solucion con mayor espacio ocupado encontrada: " << solnod->llenado_real << endl;  
    cout << "P: "; printv(P);
    printsol(solnod, P);

    return 0;
}
#include "Cola.h"

Cola::Cola() {
    frente = NULL;
    final = NULL;
    tam = 0;
}

// Destructor: vacia la cola borrando nodo por nodo.
Cola::~Cola() {
    while (frente != NULL) {
        NodoCola* aux = frente;
        frente = frente->sig;
        delete aux;
    }
    final = NULL;
    tam = 0;
}

// Encolar: el nuevo nodo siempre se enlaza detras del ultimo.
void Cola::encolar(const Paciente& p) {
    NodoCola* nuevo = new NodoCola;
    nuevo->dato = p;
    nuevo->sig = NULL;

    if (estaVacia()) {
        frente = nuevo;          // era el unico
    } else {
        final->sig = nuevo;      // se cuelga del ultimo
    }
    final = nuevo;
    tam++;
}

// Desencolar: se saca el nodo del frente (el que llego primero).
bool Cola::desencolar(Paciente& salida) {
    if (estaVacia()) {
        return false;
    }
    NodoCola* aux = frente;
    salida = aux->dato;
    frente = frente->sig;

    if (frente == NULL) {        // la cola quedo vacia
        final = NULL;
    }
    delete aux;
    tam--;
    return true;
}

bool Cola::estaVacia() const {
    return frente == NULL;
}

int Cola::getTam() const {
    return tam;
}

// Recorre los nodos sin sacarlos. Sirve para detectar IDs duplicados al cargar.
bool Cola::contieneId(int id) const {
    NodoCola* actual = frente;
    while (actual != NULL) {
        if (actual->dato.id == id) {
            return true;
        }
        actual = actual->sig;
    }
    return false;
}

#ifndef COLA_H
#define COLA_H

#include "Paciente.h"

// Nodo de la cola: guarda un paciente y apunta al siguiente.
struct NodoCola {
    Paciente dato;
    NodoCola* sig;
};

// Cola FIFO implementada a mano con nodos y punteros (sin STL).
class Cola {
private:
    NodoCola* frente;   // primero en salir
    NodoCola* final;    // ultimo en llegar
    int tam;

    // Se dejan privados (sin definir) para evitar copias accidentales
    // que provocarian doble liberacion de memoria.
    Cola(const Cola& otra);
    Cola& operator=(const Cola& otra);

public:
    Cola();
    ~Cola();

    void encolar(const Paciente& p);      // agrega al final
    bool desencolar(Paciente& salida);    // saca del frente, false si esta vacia
    bool estaVacia() const;
    int getTam() const;
    bool contieneId(int id) const;        // recorre la cola buscando un ID
};

#endif

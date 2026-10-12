// main.cpp - VERSION PROVISORIA (Commit 1)
// Solo prueba la carga del archivo y el orden FIFO de la Cola.
// El menu completo se agrega en commits posteriores.

#include <iostream>
#include <string>
#include "Cola.h"
#include "Lector.h"

using namespace std;

int main(int argc, char* argv[]) {
    string ruta = "pacientes.txt";
    if (argc > 1) {
        ruta = argv[1];
    }

    Cola cola;
    int cargados = cargarPacientes(ruta, cola);
    if (cargados < 0) {
        return 1;
    }

    cout << endl << "--- Prueba de la cola (orden de llegada) ---" << endl;
    Paciente p;
    while (cola.desencolar(p)) {
        cout << "Sale: " << p.id << " | " << p.nombre
             << " | edad " << p.edad << endl;
    }
    cout << "Pacientes restantes en cola: " << cola.getTam() << endl;

    return 0;
}

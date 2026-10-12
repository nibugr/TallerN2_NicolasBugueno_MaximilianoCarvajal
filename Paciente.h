#ifndef PACIENTE_H
#define PACIENTE_H

#include <string>

// Estructura que guarda los datos de un paciente.
// Se usa tanto en la Cola como (mas adelante) en el AVL.
struct Paciente {
    int id;                    // identificador unico
    std::string nombre;
    int edad;                  // en anios
    float temperatura;         // en grados Celsius
    int presionArterial;       // presion sistolica (mmHg), definido por el grupo
    int dolor;                 // escala 0 a 10
    std::string tipoDolor;     // pecho, cabeza, abdomen, extremidad u otro
    int trauma;                // 0 = no, 1 = si
    int sintomasNeurologicos;  // 0 = no, 1 = si
    int requiereCirugia;       // 0 = no, 1 = si
    int diasHospitalizacion;   // dias estimados
};

#endif

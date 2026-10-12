#ifndef LECTOR_H
#define LECTOR_H

#include <string>
#include "Cola.h"

// Lee el archivo de pacientes (campos separados por '|') y los encola
// respetando el orden de llegada (orden de las lineas del archivo).
//
// Retorna la cantidad de pacientes cargados, o -1 si no se pudo abrir el archivo.
// Las lineas invalidas o con ID repetido se informan y se omiten.
int cargarPacientes(const std::string& ruta, Cola& cola);

#endif

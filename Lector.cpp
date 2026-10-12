#include "Lector.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>

using namespace std;

static const int NUM_CAMPOS = 11;

// Quita espacios al inicio y al final.
static string recortar(const string& s) {
    size_t ini = 0;
    size_t fin = s.size();
    while (ini < fin && isspace((unsigned char)s[ini])) ini++;
    while (fin > ini && isspace((unsigned char)s[fin - 1])) fin--;
    return s.substr(ini, fin - ini);
}

static string aMinusculas(string s) {
    for (size_t i = 0; i < s.size(); i++) {
        s[i] = (char)tolower((unsigned char)s[i]);
    }
    return s;
}

// Convierte texto a entero. Devuelve false si no es un numero entero valido.
static bool convertirEntero(const string& texto, int& valor) {
    try {
        size_t pos = 0;
        valor = stoi(texto, &pos);
        return pos == texto.size();
    } catch (...) {
        return false;
    }
}

static bool convertirDecimal(const string& texto, float& valor) {
    try {
        size_t pos = 0;
        valor = stof(texto, &pos);
        return pos == texto.size();
    } catch (...) {
        return false;
    }
}

static bool tipoDolorValido(const string& t) {
    return t == "pecho" || t == "cabeza" || t == "abdomen" ||
           t == "extremidad" || t == "otro";
}

// Intenta armar un Paciente a partir de una linea.
// Si algo esta mal, deja el motivo en "error" y devuelve false.
static bool parsearLinea(const string& linea, Paciente& p, string& error) {
    string campos[NUM_CAMPOS];
    stringstream ss(linea);
    string trozo;
    int n = 0;

    while (getline(ss, trozo, '|')) {
        if (n >= NUM_CAMPOS) {
            error = "tiene mas de 11 campos";
            return false;
        }
        campos[n] = recortar(trozo);
        n++;
    }
    if (n != NUM_CAMPOS) {
        error = "se esperaban 11 campos y hay " + to_string(n);
        return false;
    }

    // ID
    if (!convertirEntero(campos[0], p.id) || p.id <= 0) {
        error = "ID invalido";
        return false;
    }
    // Nombre
    if (campos[1].empty()) {
        error = "nombre vacio";
        return false;
    }
    p.nombre = campos[1];
    // Edad
    if (!convertirEntero(campos[2], p.edad) || p.edad < 0 || p.edad > 120) {
        error = "edad fuera de rango (0-120)";
        return false;
    }
    // Temperatura
    if (!convertirDecimal(campos[3], p.temperatura) ||
        p.temperatura < 30.0f || p.temperatura > 45.0f) {
        error = "temperatura fuera de rango (30-45)";
        return false;
    }
    // Presion arterial (sistolica)
    if (!convertirEntero(campos[4], p.presionArterial) ||
        p.presionArterial < 40 || p.presionArterial > 250) {
        error = "presion arterial fuera de rango (40-250)";
        return false;
    }
    // Dolor
    if (!convertirEntero(campos[5], p.dolor) || p.dolor < 0 || p.dolor > 10) {
        error = "dolor fuera de rango (0-10)";
        return false;
    }
    // Tipo de dolor
    p.tipoDolor = aMinusculas(campos[6]);
    if (!tipoDolorValido(p.tipoDolor)) {
        error = "tipo de dolor no reconocido";
        return false;
    }
    // Banderas 0/1
    if (!convertirEntero(campos[7], p.trauma) || (p.trauma != 0 && p.trauma != 1)) {
        error = "Trauma debe ser 0 o 1";
        return false;
    }
    if (!convertirEntero(campos[8], p.sintomasNeurologicos) ||
        (p.sintomasNeurologicos != 0 && p.sintomasNeurologicos != 1)) {
        error = "SintomasNeurologicos debe ser 0 o 1";
        return false;
    }
    if (!convertirEntero(campos[9], p.requiereCirugia) ||
        (p.requiereCirugia != 0 && p.requiereCirugia != 1)) {
        error = "RequiereCirugia debe ser 0 o 1";
        return false;
    }
    // Dias de hospitalizacion
    if (!convertirEntero(campos[10], p.diasHospitalizacion) || p.diasHospitalizacion < 0) {
        error = "DiasHospitalizacion invalido";
        return false;
    }
    return true;
}

int cargarPacientes(const string& ruta, Cola& cola) {
    ifstream archivo(ruta.c_str());
    if (!archivo.is_open()) {
        cout << "[Error] No se pudo abrir el archivo '" << ruta << "'." << endl;
        return -1;
    }

    string linea;
    int numLinea = 0;
    int cargados = 0;
    int omitidos = 0;

    while (getline(archivo, linea)) {
        numLinea++;
        linea = recortar(linea);

        // Se ignoran lineas vacias y comentarios
        if (linea.empty() || linea[0] == '#') {
            continue;
        }

        Paciente p;
        string error;
        if (!parsearLinea(linea, p, error)) {
            cout << "[Aviso] Linea " << numLinea << " omitida: " << error << endl;
            omitidos++;
            continue;
        }
        if (cola.contieneId(p.id)) {
            cout << "[Aviso] Linea " << numLinea << " omitida: el ID " << p.id
                 << " ya esta registrado" << endl;
            omitidos++;
            continue;
        }

        cola.encolar(p);
        cargados++;
    }
    archivo.close();

    cout << "Lectura terminada -> pacientes en cola: " << cargados
         << " | lineas descartadas: " << omitidos << endl;
    return cargados;
}

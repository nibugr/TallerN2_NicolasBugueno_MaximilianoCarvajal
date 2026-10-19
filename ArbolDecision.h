#ifndef ARBOLDECISION_H
#define ARBOLDECISION_H

#include <string>
#include "Paciente.h"
#include "Servicio.h"

// Atributo del paciente que se evalua en una pregunta del arbol.
enum Atributo {
    ATR_EDAD,
    ATR_TEMPERATURA,
    ATR_PRESION,
    ATR_DOLOR,
    ATR_TIPO_DOLOR,
    ATR_TRAUMA,
    ATR_NEURO,
    ATR_CIRUGIA,
    ATR_DIAS
};

// Tipo de comparacion que hace la pregunta.
enum Operador {
    OP_MENOR,         // atributo <  umbral
    OP_MAYOR_IGUAL,   // atributo >= umbral
    OP_IGUAL          // atributo == umbral (o texto igual, para TipoDolor)
};

// Nodo del arbol de decision.
// - Si esHoja es true: el nodo guarda el servicio resultante.
// - Si no: es una pregunta con dos hijos (si / no).
struct NodoDecision {
    bool esHoja;

    // Datos de una pregunta
    Atributo atributo;
    Operador op;
    float umbral;
    std::string texto;        // solo para TipoDolor
    std::string etiquetaSi;   // como se muestra en la ruta si la respuesta es SI
    std::string etiquetaNo;   // como se muestra en la ruta si la respuesta es NO

    // Dato de una hoja
    Servicio servicio;

    NodoDecision* si;
    NodoDecision* no;
};

class ArbolDecision {
private:
    NodoDecision* raiz;

    // Se dejan privados para evitar copias (doble liberacion de memoria).
    ArbolDecision(const ArbolDecision& otro);
    ArbolDecision& operator=(const ArbolDecision& otro);

    // Creacion de nodos
    NodoDecision* crearHoja(Servicio s);
    NodoDecision* crearPregunta(Atributo a, Operador op, float umbral,
                                const std::string& texto,
                                const std::string& etiquetaSi,
                                const std::string& etiquetaNo,
                                NodoDecision* si, NodoDecision* no);

    // Evaluacion de una pregunta sobre un paciente
    bool responder(const NodoDecision* n, const Paciente& p) const;

    // Recorridos y utilidades (recursivos)
    void preorden(const NodoDecision* n) const;
    void inorden(const NodoDecision* n) const;
    void postorden(const NodoDecision* n) const;
    void imprimirIndentado(const NodoDecision* n, int nivel, const std::string& rama) const;
    std::string descripcion(const NodoDecision* n) const;
    int contarNodos(const NodoDecision* n) const;
    int altura(const NodoDecision* n) const;
    void liberar(NodoDecision* n);   // recorrido postorden

public:
    ArbolDecision();
    ~ArbolDecision();

    void construir();   // arma el arbol con los criterios definidos por el grupo

    // Comprueba que los datos del paciente esten dentro de los rangos validos.
    static bool datosValidos(const Paciente& p);

    // Evalua al paciente y devuelve el servicio.
    // En "ruta" deja el camino recorrido, por ejemplo: "Edad >= 14 -> Trauma == 0 -> ..."
    Servicio evaluar(const Paciente& p, std::string& ruta) const;

    void mostrarPreorden() const;
    void mostrarInorden() const;
    void mostrarPostorden() const;
    void mostrarArbol() const;      // dibujo con sangria
    int getCantidadNodos() const;
    int getAltura() const;
};

#endif

#include "ArbolDecision.h"

#include <iostream>

using namespace std;

ArbolDecision::ArbolDecision() {
    raiz = NULL;
}

// Destructor: libera todos los nodos con un recorrido postorden.
ArbolDecision::~ArbolDecision() {
    liberar(raiz);
    raiz = NULL;
}

// Postorden: primero los hijos, al final el nodo (si no, perderiamos los punteros).
void ArbolDecision::liberar(NodoDecision* n) {
    if (n == NULL) {
        return;
    }
    liberar(n->si);
    liberar(n->no);
    delete n;
}

// ---------------------------------------------------------------
// Creacion de nodos
// ---------------------------------------------------------------

NodoDecision* ArbolDecision::crearHoja(Servicio s) {
    NodoDecision* n = new NodoDecision;
    n->esHoja = true;
    n->atributo = ATR_EDAD;      // sin uso en una hoja
    n->op = OP_IGUAL;            // sin uso en una hoja
    n->umbral = 0;
    n->servicio = s;
    n->si = NULL;
    n->no = NULL;
    return n;
}

NodoDecision* ArbolDecision::crearPregunta(Atributo a, Operador op, float umbral,
                                           const string& texto,
                                           const string& etiquetaSi,
                                           const string& etiquetaNo,
                                           NodoDecision* si, NodoDecision* no) {
    NodoDecision* n = new NodoDecision;
    n->esHoja = false;
    n->atributo = a;
    n->op = op;
    n->umbral = umbral;
    n->texto = texto;
    n->etiquetaSi = etiquetaSi;
    n->etiquetaNo = etiquetaNo;
    n->servicio = MEDICINA_GENERAL;   // sin uso en una pregunta
    n->si = si;
    n->no = no;
    return n;
}

// ---------------------------------------------------------------
// Construccion del arbol (criterio definido por el grupo)
//
// Orden de las preguntas (de mas especifica/urgente a mas general):
//   1. Edad < 14                      -> Pediatria
//   2. Trauma == 1                    -> Cirugia o Traumatologia
//   3. SintomasNeurologicos == 1      -> Neurologia
//   4. TipoDolor == pecho             -> Cardiologia
//   5. Temperatura >= 38.5            -> Urgencias
//   6. Dolor >= 8                     -> Urgencias
//   7. PresionArterial >= 180         -> Urgencias
//   8. DiasHospitalizacion >= 2       -> Hospitalizacion
//   9. Si nada de lo anterior         -> Medicina General
//
// Se arma "de abajo hacia arriba": primero las hojas y las preguntas
// finales, y despues las que las contienen.
// ---------------------------------------------------------------
void ArbolDecision::construir() {
    liberar(raiz);   // por si se llama dos veces, no se pierde memoria
    raiz = NULL;

    // Pregunta 8: hospitalizacion o medicina general
    NodoDecision* pDias = crearPregunta(ATR_DIAS, OP_MAYOR_IGUAL, 2, "",
                                        "DiasHospitalizacion >= 2",
                                        "DiasHospitalizacion < 2",
                                        crearHoja(HOSPITALIZACION),
                                        crearHoja(MEDICINA_GENERAL));

    // Pregunta 7: presion alta
    NodoDecision* pPresion = crearPregunta(ATR_PRESION, OP_MAYOR_IGUAL, 180, "",
                                           "PresionArterial >= 180",
                                           "PresionArterial < 180",
                                           crearHoja(URGENCIAS), pDias);

    // Pregunta 6: dolor intenso
    NodoDecision* pDolor = crearPregunta(ATR_DOLOR, OP_MAYOR_IGUAL, 8, "",
                                         "Dolor >= 8", "Dolor < 8",
                                         crearHoja(URGENCIAS), pPresion);

    // Pregunta 5: fiebre alta
    NodoDecision* pTemp = crearPregunta(ATR_TEMPERATURA, OP_MAYOR_IGUAL, 38.5f, "",
                                        "Temperatura >= 38.5", "Temperatura < 38.5",
                                        crearHoja(URGENCIAS), pDolor);

    // Pregunta 4: dolor de pecho
    NodoDecision* pPecho = crearPregunta(ATR_TIPO_DOLOR, OP_IGUAL, 0, "pecho",
                                         "TipoDolor == pecho", "TipoDolor != pecho",
                                         crearHoja(CARDIOLOGIA), pTemp);

    // Pregunta 3: sintomas neurologicos
    NodoDecision* pNeuro = crearPregunta(ATR_NEURO, OP_IGUAL, 1, "",
                                         "SintomasNeurologicos == 1",
                                         "SintomasNeurologicos == 0",
                                         crearHoja(NEUROLOGIA), pPecho);

    // Pregunta 2b: si hubo trauma, se decide si necesita cirugia
    NodoDecision* pCirugia = crearPregunta(ATR_CIRUGIA, OP_IGUAL, 1, "",
                                           "RequiereCirugia == 1",
                                           "RequiereCirugia == 0",
                                           crearHoja(CIRUGIA),
                                           crearHoja(TRAUMATOLOGIA));

    // Pregunta 2: trauma
    NodoDecision* pTrauma = crearPregunta(ATR_TRAUMA, OP_IGUAL, 1, "",
                                          "Trauma == 1", "Trauma == 0",
                                          pCirugia, pNeuro);

    // Pregunta 1 (raiz): edad
    raiz = crearPregunta(ATR_EDAD, OP_MENOR, 14, "",
                         "Edad < 14", "Edad >= 14",
                         crearHoja(PEDIATRIA), pTrauma);
}

// ---------------------------------------------------------------
// Evaluacion
// ---------------------------------------------------------------

// Datos fuera de rango = datos invalidos.
bool ArbolDecision::datosValidos(const Paciente& p) {
    if (p.edad < 0 || p.edad > 120) return false;
    if (p.temperatura < 30.0f || p.temperatura > 45.0f) return false;
    if (p.presionArterial < 40 || p.presionArterial > 250) return false;
    if (p.dolor < 0 || p.dolor > 10) return false;
    if (p.trauma != 0 && p.trauma != 1) return false;
    if (p.sintomasNeurologicos != 0 && p.sintomasNeurologicos != 1) return false;
    if (p.requiereCirugia != 0 && p.requiereCirugia != 1) return false;
    if (p.diasHospitalizacion < 0) return false;
    if (p.tipoDolor != "pecho" && p.tipoDolor != "cabeza" && p.tipoDolor != "abdomen" &&
        p.tipoDolor != "extremidad" && p.tipoDolor != "otro") return false;
    return true;
}

// Responde la pregunta de un nodo con los datos del paciente.
bool ArbolDecision::responder(const NodoDecision* n, const Paciente& p) const {
    // El tipo de dolor es texto, se compara aparte.
    if (n->atributo == ATR_TIPO_DOLOR) {
        return p.tipoDolor == n->texto;
    }

    // Para el resto se obtiene el valor numerico del atributo.
    float valor = 0;
    switch (n->atributo) {
        case ATR_EDAD:        valor = (float)p.edad; break;
        case ATR_TEMPERATURA: valor = p.temperatura; break;
        case ATR_PRESION:     valor = (float)p.presionArterial; break;
        case ATR_DOLOR:       valor = (float)p.dolor; break;
        case ATR_TRAUMA:      valor = (float)p.trauma; break;
        case ATR_NEURO:       valor = (float)p.sintomasNeurologicos; break;
        case ATR_CIRUGIA:     valor = (float)p.requiereCirugia; break;
        case ATR_DIAS:        valor = (float)p.diasHospitalizacion; break;
        default: break;
    }

    switch (n->op) {
        case OP_MENOR:       return valor < n->umbral;
        case OP_MAYOR_IGUAL: return valor >= n->umbral;
        case OP_IGUAL:       return valor == n->umbral;
    }
    return false;
}

// Baja por el arbol desde la raiz: en cada pregunta va al hijo "si" o "no",
// hasta llegar a una hoja, que contiene el servicio.
Servicio ArbolDecision::evaluar(const Paciente& p, string& ruta) const {
    ruta = "";

    // Datos invalidos: por seguridad se envian a Urgencias (ver README).
    if (!datosValidos(p) || raiz == NULL) {
        ruta = "Datos invalidos o insuficientes";
        return URGENCIAS;
    }

    const NodoDecision* actual = raiz;
    while (!actual->esHoja) {
        bool respuesta = responder(actual, p);
        if (!ruta.empty()) {
            ruta += " -> ";
        }
        ruta += respuesta ? actual->etiquetaSi : actual->etiquetaNo;
        actual = respuesta ? actual->si : actual->no;
    }
    return actual->servicio;
}

// ---------------------------------------------------------------
// Recorridos
// ---------------------------------------------------------------

// Texto corto de un nodo, para imprimirlo en los recorridos.
string ArbolDecision::descripcion(const NodoDecision* n) const {
    if (n->esHoja) {
        return string("Servicio: ") + nombreServicio(n->servicio);
    }
    return "Pregunta: " + n->etiquetaSi;
}

// Preorden: nodo, hijo si, hijo no.
void ArbolDecision::preorden(const NodoDecision* n) const {
    if (n == NULL) return;
    cout << "  - " << descripcion(n) << endl;
    preorden(n->si);
    preorden(n->no);
}

// Inorden: hijo si, nodo, hijo no.
void ArbolDecision::inorden(const NodoDecision* n) const {
    if (n == NULL) return;
    inorden(n->si);
    cout << "  - " << descripcion(n) << endl;
    inorden(n->no);
}

// Postorden: hijo si, hijo no, nodo.
void ArbolDecision::postorden(const NodoDecision* n) const {
    if (n == NULL) return;
    postorden(n->si);
    postorden(n->no);
    cout << "  - " << descripcion(n) << endl;
}

void ArbolDecision::imprimirIndentado(const NodoDecision* n, int nivel, const string& rama) const {
    if (n == NULL) return;
    for (int i = 0; i < nivel; i++) {
        cout << "    ";
    }
    cout << rama << descripcion(n) << endl;
    imprimirIndentado(n->si, nivel + 1, "[Si] ");
    imprimirIndentado(n->no, nivel + 1, "[No] ");
}

int ArbolDecision::contarNodos(const NodoDecision* n) const {
    if (n == NULL) return 0;
    return 1 + contarNodos(n->si) + contarNodos(n->no);
}

// Altura medida en niveles: una hoja sola tiene altura 1, un arbol vacio 0.
int ArbolDecision::altura(const NodoDecision* n) const {
    if (n == NULL) return 0;
    int hs = altura(n->si);
    int hn = altura(n->no);
    return 1 + (hs > hn ? hs : hn);
}

void ArbolDecision::mostrarPreorden() const {
    cout << "Recorrido preorden del arbol de decision:" << endl;
    preorden(raiz);
}

void ArbolDecision::mostrarInorden() const {
    cout << "Recorrido inorden del arbol de decision:" << endl;
    inorden(raiz);
}

void ArbolDecision::mostrarPostorden() const {
    cout << "Recorrido postorden del arbol de decision:" << endl;
    postorden(raiz);
}

void ArbolDecision::mostrarArbol() const {
    cout << "Arbol de decision (indentado):" << endl;
    imprimirIndentado(raiz, 0, "");
}

int ArbolDecision::getCantidadNodos() const {
    return contarNodos(raiz);
}

int ArbolDecision::getAltura() const {
    return altura(raiz);
}

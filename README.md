# Hospital Marmaja - Taller 02 (Estructura de Datos)

Clasificacion automatica de pacientes con un arbol de decision y almacenamiento
en arboles AVL segun el servicio asignado.

Flujo principal: `Archivo -> Cola -> Arbol de decision -> Servicio -> AVL`

## Integrantes

| Nombre | RUT | Usuario GitHub | Carrera | Paralelo |
|--------|-----|----------------|---------|----------|
| Nicolas _(completar apellido)_ | _(completar)_ | _(completar)_ | ICCI | _(completar)_ |

## Compilacion y ejecucion (Codespace / Linux)

```bash
make
./hospital                 # usa pacientes.txt
./hospital otro_archivo.txt
```

Para limpiar: `make clean`

Tambien se puede compilar a mano:

```bash
g++ -std=c++11 -Wall -Wextra -o hospital main.cpp Cola.cpp Lector.cpp
```

## Formato del archivo de pacientes

Los campos se separan con el caracter `;`. Una linea por paciente. Las lineas
vacias y las que empiezan con `#` se ignoran.

```
ID;Nombre;Edad;Temperatura;PresionArterial;Dolor;TipoDolor;Trauma;SintomasNeurologicos;RequiereCirugia;DiasHospitalizacion
```

Validaciones al cargar (la linea se omite y se informa el motivo):

- Debe tener exactamente 11 campos.
- ID entero positivo y no repetido.
- Edad entre 0 y 120, temperatura entre 30 y 45, presion arterial (sistolica) entre 40 y 250.
- Dolor entre 0 y 10; TipoDolor: pecho, cabeza, abdomen, extremidad u otro.
- Trauma, SintomasNeurologicos y RequiereCirugia solo pueden ser 0 o 1.
- DiasHospitalizacion mayor o igual a 0.
- Si el archivo no existe, se informa el error y el programa termina.

## Estructura del proyecto

| Archivo | Contenido |
|---------|-----------|
| `Paciente.h` | struct con los datos del paciente |
| `Cola.h / Cola.cpp` | Cola FIFO hecha a mano con nodos y punteros |
| `Lector.h / Lector.cpp` | Lectura y validacion del archivo |
| `Servicio.h` | enum con los 8 servicios y su nombre para mostrar |
| `ArbolDecision.h / ArbolDecision.cpp` | Arbol de decision hecho con nodos y punteros |
| `main.cpp` | Programa principal (menu en commits posteriores) |
| `pacientes.txt` | Datos de ejemplo (cubre los 8 servicios) |
| `pacientes_invalidos.txt` | Datos de prueba para errores y duplicados |
| `casos_prueba.txt` | Casos limite para el arbol de decision |

## Arbol de decision

Cada nodo es una pregunta (con un hijo "si" y uno "no") o una hoja con el servicio.
La evaluacion parte en la raiz y baja hasta una hoja. Operaciones implementadas:
construccion, evaluacion con ruta, recorridos preorden / inorden / postorden,
dibujo indentado y destructor con recorrido postorden.

```
Edad < 14?
|-- Si: Pediatria
|-- No: Trauma == 1?
    |-- Si: RequiereCirugia == 1?
    |   |-- Si: Cirugia
    |   |-- No: Traumatologia
    |-- No: SintomasNeurologicos == 1?
        |-- Si: Neurologia
        |-- No: TipoDolor == pecho?
            |-- Si: Cardiologia
            |-- No: Temperatura >= 38.5?
                |-- Si: Urgencias
                |-- No: Dolor >= 8?
                    |-- Si: Urgencias
                    |-- No: PresionArterial >= 180?
                        |-- Si: Urgencias
                        |-- No: DiasHospitalizacion >= 2?
                            |-- Si: Hospitalizacion
                            |-- No: Medicina General
```

Atributos usados: Edad, Trauma, RequiereCirugia, SintomasNeurologicos, TipoDolor,
Temperatura, Dolor, PresionArterial y DiasHospitalizacion (9 en total).
Hay al menos una ruta hacia cada uno de los 8 servicios.

### Justificacion de cada decision

| Pregunta | Justificacion |
|----------|---------------|
| Edad < 14 | Los menores se atienden en un servicio especializado, por eso va primero. |
| Trauma == 1 | Un golpe o lesion se deriva a un servicio quirurgico, antes de mirar otros sintomas. |
| RequiereCirugia == 1 | Con trauma, si el paciente necesita operarse va a Cirugia; si no, a Traumatologia. |
| SintomasNeurologicos == 1 | Sin trauma, los sintomas neurologicos necesitan un especialista. |
| TipoDolor == pecho | El dolor de pecho se deriva a Cardiologia. |
| Temperatura >= 38.5 | Fiebre alta: atencion inmediata en Urgencias. |
| Dolor >= 8 | Dolor muy intenso (escala 0-10): Urgencias. |
| PresionArterial >= 180 | Se definio la presion sistolica en mmHg; 180 o mas se considera crisis y va a Urgencias. |
| DiasHospitalizacion >= 2 | Si se estima internacion de 2 dias o mas, va a Hospitalizacion. |
| Ninguna anterior | Caso general: Medicina General. |

### Empates y valores limite

- Si un paciente cumple varias condiciones, **gana la primera pregunta del arbol** (el orden
  de arriba hacia abajo es la prioridad). Ejemplo: un nino con trauma va a Pediatria,
  y un paciente con trauma y sintomas neurologicos va a Traumatologia.
- Los limites estan definidos: edad 13 es Pediatria y 14 sigue el arbol; temperatura 38.5,
  dolor 8, presion 180 y 2 dias cuentan como "si" (usan `>=`).

### Datos invalidos o insuficientes

- En la carga (`Lector.cpp`) las lineas con campos de menos, valores fuera de rango o
  ID repetido se omiten y se informa el motivo.
- Como segunda barrera, `ArbolDecision::evaluar` vuelve a validar al paciente. Si llegara uno
  con datos invalidos, se asigna a **Urgencias** por seguridad (que lo revise una persona) y la
  ruta indica "Datos invalidos o insuficientes".

### Casos de prueba

`casos_prueba.txt` incluye valores limite y prioridades. Para ejecutarlos:

```bash
./hospital casos_prueba.txt
```

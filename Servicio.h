#ifndef SERVICIO_H
#define SERVICIO_H

// Los 8 servicios del hospital (se mantienen respecto al Taller 1).
// NUM_SERVICIOS sirve para dimensionar arreglos (por ejemplo, un AVL por servicio).
enum Servicio {
    URGENCIAS = 0,
    MEDICINA_GENERAL,
    CARDIOLOGIA,
    NEUROLOGIA,
    TRAUMATOLOGIA,
    CIRUGIA,
    PEDIATRIA,
    HOSPITALIZACION,
    NUM_SERVICIOS
};

// Devuelve el nombre del servicio para mostrarlo en consola.
inline const char* nombreServicio(int s) {
    switch (s) {
        case URGENCIAS:        return "Urgencias";
        case MEDICINA_GENERAL: return "Medicina General";
        case CARDIOLOGIA:      return "Cardiologia";
        case NEUROLOGIA:       return "Neurologia";
        case TRAUMATOLOGIA:    return "Traumatologia";
        case CIRUGIA:          return "Cirugia";
        case PEDIATRIA:        return "Pediatria";
        case HOSPITALIZACION:  return "Hospitalizacion";
        default:               return "Desconocido";
    }
}

#endif

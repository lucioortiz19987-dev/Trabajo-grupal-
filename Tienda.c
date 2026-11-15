#include <stdio.h>

// Variables globales y constantes
#define MAX_PRODUCTOS 5
#define MAX_NOMBRE 50

//PROTOTIPOS DE FUNCIONES
void limpiarBuffer();
void leerCadena(char *cadena, int tamano);
int compararCadenas(const char *cad1, const char *cad2);
void copiarCadena(char *destino, const char *origen);

int buscarProducto(char nombres[][MAX_NOMBRE], int total, const char *nombreBuscado);
void ingresarProducto(char nombres[][MAX_NOMBRE], float *tiempos, float *recursos, int *demandas, int *total);
void mostrarProductos(char nombres[][MAX_NOMBRE], float *tiempos, float *recursos, int *demandas, int total);
void editarProducto(char nombres[][MAX_NOMBRE], float *tiempos, float *recursos, int *demandas, int total);
void eliminarProducto(char nombres[][MAX_NOMBRE], float *tiempos, float *recursos, int *demandas, int *total);
void calcularProduccion(char nombres[][MAX_NOMBRE], float *tiempos, float *recursos, int *demandas, int total);

int main() {
    // Arreglos paralelos para almacenar los datos de los 5 productos
    char nombres[MAX_PRODUCTOS][MAX_NOMBRE];
    float tiempos_unitarios[MAX_PRODUCTOS];
    float recursos_unitarios[MAX_PRODUCTOS];
    int demandas[MAX_PRODUCTOS];
    
    int total_registrados = 0;
    int opcion;

    do {
        printf("\n======================================================\n");
        printf("   SISTEMA DE OPTIMIZACION DE PRODUCCION (FAB-ELEC)   \n");
        printf("======================================================\n");
        printf("Productos registrados: %d / %d\n", total_registrados, MAX_PRODUCTOS);
        printf("1. Ingresar nuevo producto\n");
        printf("2. Mostrar productos registrados\n");
        printf("3. Editar datos de un producto (Busqueda)\n");
        printf("4. Eliminar un producto (Busqueda)\n");
        printf("5. Calcular tiempo, recursos y viabilidad de demanda\n");
        printf("6. Salir\n");
        printf("------------------------------------------------------\n");
        printf("Seleccione una opcion: ");
        
        if (scanf("%d", &opcion) != 1) {
            printf("\n[Error] Entrada invalida. Ingrese un numero.\n");
            limpiarBuffer();
            continue;
        }
        limpiarBuffer();

        switch (opcion) {
            case 1:
                ingresarProducto(nombres, tiempos_unitarios, recursos_unitarios, demandas, &total_registrados);
                break;
            case 2:
                mostrarProductos(nombres, tiempos_unitarios, recursos_unitarios, demandas, total_registrados);
                break;
            case 3:
                editarProducto(nombres, tiempos_unitarios, recursos_unitarios, demandas, total_registrados);
                break;
            case 4:
                eliminarProducto(nombres, tiempos_unitarios, recursos_unitarios, demandas, &total_registrados);
                break;
            case 5:
                calcularProduccion(nombres, tiempos_unitarios, recursos_unitarios, demandas, total_registrados);
                break;
            case 6:
                printf("\nSaliendo del sistema. Hasta luego!\n");
                break;
            default:
                printf("\n[Aviso] Opcion no valida. Intente de nuevo.\n");
        }
    } while (opcion != 6);

    return 0;
}

//FUNCIONES AUXILIARES 

void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void leerCadena(char *cadena, int tamano) {
    int i = 0;
    char c;
    while (i < tamano - 1) {
        c = getchar();
        if (c == '\n' || c == EOF) {
            break;
        }
        *(cadena + i) = c; // Uso de aritmética de punteros
        i++;
    }
    *(cadena + i) = '\0';
}

// Retorna 1 si son iguales, 0 si son diferentes 
int compararCadenas(const char *cad1, const char *cad2) {
    int i = 0;
    while (*(cad1 + i) != '\0' && *(cad2 + i) != '\0') {
        if (*(cad1 + i) != *(cad2 + i)) {
            return 0;
        }
        i++;
    }
    return (*(cad1 + i) == '\0' && *(cad2 + i) == '\0');
}

void copiarCadena(char *destino, const char *origen) {
    int i = 0;
    while (*(origen + i) != '\0') {
        *(destino + i) = *(origen + i);
        i++;
    }
    *(destino + i) = '\0';
}

// FUNCIONES PRINCIPALES DEL SISTEMA 

// Búsqueda lineal propia: retorna el índice si lo encuentra, o -1 si no existe
int buscarProducto(char nombres[][MAX_NOMBRE], int total, const char *nombreBuscado) {
    for (int i = 0; i < total; i++) {
        if (compararCadenas(nombres[i], nombreBuscado) == 1) {
            return i;
        }
    }
    return -1;
}

void ingresarProducto(char nombres[][MAX_NOMBRE], float *tiempos, float *recursos, int *demandas, int *total) {
    if (*total >= MAX_PRODUCTOS) {
        printf("\n[Aviso] Limite alcanzado. Ya se ingresaron los %d productos permitidos.\n", MAX_PRODUCTOS);
        return;
    }

    char tempNombre[MAX_NOMBRE];
    printf("\n--- INGRESO DE PRODUCTO #%d ---\n", *total + 1);
    printf("Nombre del componente/producto: ");
    leerCadena(tempNombre, MAX_NOMBRE);

    // Validar que no esté duplicado usando nuestra búsqueda propia
    if (buscarProducto(nombres, *total, tempNombre) != -1) {
        printf("[Error] Ya existe un producto con ese nombre.\n");
        return;
    }

    copiarCadena(nombres[*total], tempNombre);

    do {
        printf("Tiempo de fabricacion unitario (horas): ");
        scanf("%f", (tiempos + *total));
        if (*(tiempos + *total) <= 0) printf("[Error] El tiempo debe ser mayor a 0.\n");
    } while (*(tiempos + *total) <= 0);

    do {
        printf("Cantidad de recursos por unidad: ");
        scanf("%f", (recursos + *total));
        if (*(recursos + *total) <= 0) printf("[Error] Los recursos deben ser mayores a 0.\n");
    } while (*(recursos + *total) <= 0);

    do {
        printf("Cantidad demandada (unidades): ");
        scanf("%d", (demandas + *total));
        if (*(demandas + *total) < 0) printf("[Error] La demanda no puede ser negativa.\n");
    } while (*(demandas + *total) < 0);

    limpiarBuffer();
    (*total)++;
    printf("[Exito] Producto registrado correctamente.\n");
}

void mostrarProductos(char nombres[][MAX_NOMBRE], float *tiempos, float *recursos, int *demandas, int total) {
    if (total == 0) {
        printf("\n[Aviso] No hay productos registrados en el sistema.\n");
        return;
    }

    printf("\n--------------------------------------------------------------------------\n");
    printf("%-4s | %-22s | %-12s | %-12s | %-10s\n", "ID", "Producto", "Tiempo/U (h)", "Recursos/U", "Demanda");
    printf("--------------------------------------------------------------------------\n");
    for (int i = 0; i < total; i++) {
        printf("%-4d | %-22s | %-12.2f | %-12.2f | %-10d\n", 
               i + 1, nombres[i], *(tiempos + i), *(recursos + i), *(demandas + i));
    }
    printf("--------------------------------------------------------------------------\n");
}

void editarProducto(char nombres[][MAX_NOMBRE], float *tiempos, float *recursos, int *demandas, int total) {
    if (total == 0) {
        printf("\n[Aviso] No hay productos registrados para editar.\n");
        return;
    }

    char nombreBuscado[MAX_NOMBRE];
    printf("\n--- EDITAR PRODUCTO ---\n");
    printf("Ingrese el nombre exacto del producto a editar: ");
    leerCadena(nombreBuscado, MAX_NOMBRE);

    int idx = buscarProducto(nombres, total, nombreBuscado);
    if (idx == -1) {
        printf("[Error] Producto '%s' no encontrado.\n", nombreBuscado);
        return;
    }

    printf("\nProducto encontrado: %s (Tiempo: %.2f h | Recursos: %.2f | Demanda: %d)\n",
           nombres[idx], *(tiempos + idx), *(recursos + idx), *(demandas + idx));

    char nuevoNombre[MAX_NOMBRE];
    printf("Nuevo nombre (escriba el mismo si no desea cambiarlo): ");
    leerCadena(nuevoNombre, MAX_NOMBRE);
    
    // Verificar si el nuevo nombre choca con otro producto distinto
    int idxCheck = buscarProducto(nombres, total, nuevoNombre);
    if (idxCheck != -1 && idxCheck != idx) {
        printf("[Error] Ya existe otro producto con ese nombre. Edicion cancelada.\n");
        return;
    }
    copiarCadena(nombres[idx], nuevoNombre);

    do {
        printf("Nuevo tiempo de fabricacion unitario (horas): ");
        scanf("%f", (tiempos + idx));
    } while (*(tiempos + idx) <= 0);

    do {
        printf("Nueva cantidad de recursos por unidad: ");
        scanf("%f", (recursos + idx));
    } while (*(recursos + idx) <= 0);

    do {
        printf("Nueva cantidad demandada: ");
        scanf("%d", (demandas + idx));
    } while (*(demandas + idx) < 0);

    limpiarBuffer();
    printf("[Exito] Datos del producto actualizados correctamente.\n");
}

void eliminarProducto(char nombres[][MAX_NOMBRE], float *tiempos, float *recursos, int *demandas, int *total) {
    if (*total == 0) {
        printf("\n[Aviso] No hay productos registrados para eliminar.\n");
        return;
    }

    char nombreBuscado[MAX_NOMBRE];
    printf("\n--- ELIMINAR PRODUCTO ---\n");
    printf("Ingrese el nombre exacto del producto a eliminar: ");
    leerCadena(nombreBuscado, MAX_NOMBRE);

    int idx = buscarProducto(nombres, *total, nombreBuscado);
    if (idx == -1) {
        printf("[Error] Producto '%s' no encontrado.\n", nombreBuscado);
        return;
    }

    // Desplazamiento hacia la izquierda en todos los arreglos paralelos
    for (int i = idx; i < (*total) - 1; i++) {
        copiarCadena(nombres[i], nombres[i + 1]);
        *(tiempos + i) = *(tiempos + i + 1);
        *(recursos + i) = *(recursos + i + 1);
        *(demandas + i) = *(demandas + i + 1);
    }

    (*total)--;
    printf("[Exito] Producto '%s' eliminado del sistema.\n", nombreBuscado);
}

void calcularProduccion(char nombres[][MAX_NOMBRE], float *tiempos, float *recursos, int *demandas, int total) {
    if (total == 0) {
        printf("\n[Aviso] Registre al menos un producto antes de realizar el calculo.\n");
        return;
    }

    float tiempo_disponible, recursos_disponibles;
    printf("\n--- ANALISIS DE CAPACIDAD Y OPTIMIZACION ---\n");
    
    do {
        printf("Ingrese el tiempo total disponible en la linea de produccion (horas): ");
        scanf("%f", &tiempo_disponible);
    } while (tiempo_disponible <= 0);

    do {
        printf("Ingrese la cantidad total de recursos disponibles en fabrica: ");
        scanf("%f", &recursos_disponibles);
    } while (recursos_disponibles <= 0);
    limpiarBuffer();

    float tiempo_total_req = 0.0;
    float recursos_totales_req = 0.0;

    printf("\nDesglose por producto:\n");
    for (int i = 0; i < total; i++) {
        float t_prod = (*(tiempos + i)) * (*(demandas + i));
        float r_prod = (*(recursos + i)) * (*(demandas + i));
        tiempo_total_req += t_prod;
        recursos_totales_req += r_prod;
        printf("- %-20s | Tiempo req: %8.2f h | Recursos req: %8.2f\n", nombres[i], t_prod, r_prod);
    }

    printf("\n======================================================\n");
    printf("RESULTADOS TOTALES DE PRODUCCION:\n");
    printf("Tiempo Total Requerido   : %.2f h (Disponible: %.2f h)\n", tiempo_total_req, tiempo_disponible);
    printf("Recursos Totales Req.    : %.2f   (Disponible: %.2f)\n", recursos_totales_req, recursos_disponibles);
    printf("------------------------------------------------------\n");

    if (tiempo_total_req <= tiempo_disponible && recursos_totales_req <= recursos_disponibles) {
        printf("DIAGNOSTICO: [VIABLE] La fabrica SI puede cumplir con toda la demanda.\n");
        printf("Sobrante de tiempo  : %.2f h\n", tiempo_disponible - tiempo_total_req);
        printf("Sobrante de recursos: %.2f unidades\n", recursos_disponibles - recursos_totales_req);
    } else {
        printf("DIAGNOSTICO: [NO VIABLE] La fabrica NO puede cumplir con la demanda.\n");
        if (tiempo_total_req > tiempo_disponible) {
            printf("-> Deficit de tiempo  : Faltan %.2f horas de produccion.\n", tiempo_total_req - tiempo_disponible);
        }
        if (recursos_totales_req > recursos_disponibles) {
            printf("-> Deficit de recursos: Faltan %.2f unidades de recursos.\n", recursos_totales_req - recursos_disponibles);
        }
    }
    printf("======================================================\n");
}
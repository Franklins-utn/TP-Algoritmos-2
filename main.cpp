// Copyright 2021 Los pibes SA
// Author: Luciano
// Author: Franco
// Author: Juan

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <locale.h>

/*** LIMPIAR CONSOLA ***/
#ifdef _WIN32
#define limpiarConsola system("cls");
#endif // WINDOWS

#ifdef __linux__
#define limpiarConsola system("clear");
#endif // LINUX

#ifdef __APPLE__
#define limpiarConsola system("clear");
#endif // APPLE
/*** FIN LIMPIAR CONSOLA ***/

using namespace std;

/**** DECLARACIONES DE CONSTANTES ****/

struct Conductor
{
    int conductorId;
    long fechaVencimiento;
    bool activo;
    int totalInfracciones;
    char email[100];
};

struct Infraccion
{
    int infraccionId;
    char fechaHora[13]; //AAAAMMDDHH:MM
    float monto;
    int conductorId;
    int provincia;
};

struct NodoConductor
{
    Conductor conductor;
    NodoConductor *next;
};

struct NodoInfraccion
{
    Infraccion infraccion;
    NodoInfraccion *next;
    NodoInfraccion *previous;
};

/**** FIN DECLARACIONES DE CONSTANTES ****/

/**** DECLARACIONES DE FUNCIONES ****/
//Generales
void mostrarMenu(int &opcionMenu);
void procesar_lote(NodoInfraccion *listaInfracciones, NodoConductor *listaconductores);

/** Conductores **/
//Mostrar y llenar
void agregarConductorAlArchivo(Conductor conductor);
void mostrarConductor(Conductor conductor);
void mostrar_informe(NodoInfraccion *listainfracciones, NodoConductor *listaconductores);

//Listas conductores
void cargarConductoresEnMemoria(NodoConductor *&listaConductores); //Esta función genera una lista SE de conductores

//CRUD Listas conductores
void insertarConductorAlFinal(NodoConductor *&listaConductores, Conductor conductor);
void listarTodosLosConductores(NodoConductor *listaConductores);
void desactivarConductor(NodoConductor *listaConductores, int conductorId);
void actualizarTotalinfraccionesArchivo(NodoConductor *listaConductores, int conductorId, int totalInfracciones);
void finalizar_jornada(NodoConductor *listaconductores);
void borrar_lista_conductor(NodoConductor *&lista);
void inicializar_archivo_conductores();
void llenar_struct_conductor(Conductor &conduc,int conductorId,long fechaVencimiento,char email[]);
Conductor crearNuevoConductor(NodoConductor *listaConductores);
NodoConductor *obtenerUltimoConductor(NodoConductor *listaConductores);

/** Infracciones **/
//Mostrar y llenar
void mostrar_infractores_de_una_provincia(NodoInfraccion *listainfracciones);
void mostar_infra_de_conductor(NodoInfraccion *listainfracciones);
void llenar_struct_infraccion(Infraccion &infra, char fecha[], int infra_id, float monto, int provincia, int id_conductor);
void mostrar_infra(Infraccion infra);

//Generar infracciones
void generarInfraccionesRandom(NodoConductor *conductores, NodoInfraccion *&infracciones, int cantidadInfracciones, bool mostrar_infracciones);

//Listas Infracciones
void cargar_infracciones_en_memoria_ordena_conducID(NodoInfraccion *&listainfracciones);
void ingresar_ordenadamente_por_conducID(NodoInfraccion *&lista, Infraccion ticket);
void ingresar_ordenadamente_por_infraccionID(NodoInfraccion *&lista, Infraccion ticket);
void borrar_lista_infracciones(NodoInfraccion *&lista);
void actualizar_archivo_procesados(NodoInfraccion *listaInfracciones, NodoConductor *listaConductores);

//Archivos de texto
void exportarHTML(NodoConductor *listaConductores);
void exportarCSV(NodoConductor *listaConductores);

//Ayudines
int generarNumeroEnteroRandom(int min, int max);

/**** FIN DECLARACIONES DE FUNCIONES ****/

int main()
{
    setlocale(LC_ALL, "Spanish");

    int opcionMenu;
    int conductorId;
    int cantidadRandomInfracciones = 0;
    NodoConductor *listaConductores = NULL;
    NodoInfraccion *listaInfracciones = NULL;

    inicializar_archivo_conductores();

    cargarConductoresEnMemoria(listaConductores);

    cargar_infracciones_en_memoria_ordena_conducID(listaInfracciones);

    mostrarMenu(opcionMenu);

    while (opcionMenu != 0)
    {
        switch (opcionMenu)
        {
        case 1:
            insertarConductorAlFinal(listaConductores, crearNuevoConductor(listaConductores));
            break;
        case 2:
            listarTodosLosConductores(listaConductores);
            break;
        case 3:
            listarTodosLosConductores(listaConductores);
            cout << "Ingrese un ID de conductor para desactivar: " << endl;
            cin >> conductorId;
            desactivarConductor(listaConductores, conductorId);
            break;
        case 4:
            mostrar_infractores_de_una_provincia(listaInfracciones);
            break;
        case 5:
            while (cantidadRandomInfracciones <= 0)
            {
                cout << "Ingrese una cantidad de infracciones aleatorias a generar: " << endl;
                cin >> cantidadRandomInfracciones;
            }
            generarInfraccionesRandom(listaConductores, listaInfracciones, cantidadRandomInfracciones,true);
            procesar_lote(listaInfracciones, listaConductores);
            cantidadRandomInfracciones = 0;
            break;
        case 6:
            mostrar_informe(listaInfracciones, listaConductores);
            break;
        case 7:
            exportarHTML(listaConductores);
            break;
        case 8:
            exportarCSV(listaConductores);
            break;
        case 9:
            finalizar_jornada(listaConductores);
            borrar_lista_conductor(listaConductores);
            borrar_lista_infracciones(listaInfracciones);
            opcionMenu = 0;
            break;
        default:
            break;
        }
        mostrarMenu(opcionMenu);
    }

    cout << "\nEl programa ha finalizado." << endl;
    return 0;
}

/** SUBPROGRAMAS GENERALES **/

void mostrarMenu(int &opcionMenu)
{
    opcionMenu = -1;

    cout << "**************************************************" << endl;
    cout << "Bienvenido al sistema de infracciones de Gobierno Nacional" << endl;
    cout << "Por favor seleccione una opción: " << endl;
    cout << "1 - Cargar un nuevo conductor" << endl;
    cout << "2 - Listar todos los conductores" << endl;
    cout << "3 - Desactivar un conductor" << endl;
    cout << "4 - Listar detalle de conductores por provincia" << endl;
    cout << "5 - Procesar lote de infracciones" << endl;
    cout << "6 - Mostrar informe de un conductor" << endl;
    cout << "7 - Exportar informe HTML por conductor entre fechas de registro vencido" << endl;
    cout << "8 - Exportar informe CSV  por conductor entre fechas de registro vencido" << endl;
    cout << "9 - Finalizar la jornada" << endl;
    cout << "0 - Salir del programa" << endl;
    cout << "**************************************************" << endl;

    cin >> opcionMenu;
}

void procesar_lote(NodoInfraccion *listaInfracciones, NodoConductor *listaconductores)
{
    // Recibe todas las listas y actualiza los archivos
    actualizar_archivo_procesados(listaInfracciones, listaconductores);

    return;
}

/**** FIN SUBPROGRAMAS GENERALES ****/

/** SUBPROGRAMAS DE CONDUCTORES **/

void insertarConductorAlFinal(NodoConductor *&listaConductores, Conductor conductor)
{
    NodoConductor *aux;

    if (listaConductores == NULL)
    {
        listaConductores = new NodoConductor();
        listaConductores->conductor = conductor;
        listaConductores->next = NULL;
    }
    else
    {
        aux = obtenerUltimoConductor(listaConductores);
        aux->next = new NodoConductor();
        aux = aux->next;
        aux->conductor = conductor;
        aux->next = NULL;
    }
}

void mostrarConductor(Conductor conductor)
{
    cout << "-----------------------------------------------------------------" << endl;

    cout << "Información acerca del conductor con ID: " << conductor.conductorId << endl;

    cout << "Email: " << conductor.email << endl;

    cout << "Fecha de vencimiento: " << conductor.fechaVencimiento << endl;

    cout << "Activo: " << conductor.activo << endl;

    cout << "Total infracciones: " << conductor.totalInfracciones << endl;

    cout << "-----------------------------------------------------------------" << endl;
}

NodoConductor *obtenerUltimoConductor(NodoConductor *listaConductores)
{
    while (listaConductores && listaConductores->next)
    {
        listaConductores = listaConductores->next;
    }

    return listaConductores;
}

void actualizarTotalinfraccionesArchivo(NodoConductor *listaConductores, int conductorId, int totalInfracciones)
{
    NodoConductor *aux = listaConductores;

    if (!aux)
    {
        limpiarConsola;
        cout << "Error: No existe el archivo Conductores.bin \n"
             << endl;
        return;
    }

    FILE *f;
    Conductor conductor;
    int posicionConductor = 0;

    f = fopen("conductores.bin", "rb+");

    fseek(f , 0 , SEEK_SET);

    while (fread(&conductor, sizeof(Conductor), 1, f) == 1)
    {
        if (conductor.conductorId == conductorId)
        {
            fseek(f,  sizeof(Conductor) * (-1), SEEK_CUR);
            conductor.totalInfracciones = totalInfracciones;
            fwrite(&conductor, sizeof(Conductor),1, f);
            fseek(f,  0, SEEK_CUR);//patch
        }
    }

    fclose(f);
}

void desactivarConductor(NodoConductor *listaConductores, int conductorId)
{
    /*
    Para desactivar un conductor:
    1 - Debemos mostrar una lista de los conductores existentes
    2 - Debemos guardar el ingreso del conductorId
    3 - Abrimos el archivo de conductores en modo rb+
    4 - Buscamos el conductor por ID
    5 - Actualizamos activo = false;
    6 - Cerramos el archivo
    7 - Devolvemos el conductor para que se actualice en la lista
    */
    //Guardamos el inicio de la lista
    NodoConductor *aux = listaConductores;
    if (!aux)
    {
        limpiarConsola;
        cout << "Error: No existe el archivo Conductores.bin \n"
             << endl;
        return;
    }

    FILE *f;
    Conductor conductor;
    int posicionConductor = 0;

    while (aux)
    {
        if (aux->conductor.conductorId == conductorId)
        {
            aux->conductor.activo = false;
        }

        aux = aux->next;
    }

    f = fopen("conductores.bin", "rb+");

    while (fread(&conductor, sizeof(Conductor), 1, f) == 1)
    {
        if (conductor.conductorId == conductorId)
        {
            fseek(f,  sizeof(Conductor) * (-1), SEEK_CUR);
            conductor.activo = false;
            fwrite(&conductor, sizeof(Conductor),1, f);
            fseek(f,  0, SEEK_CUR);//patch
            posicionConductor = ftell(f);
        }
    }

    if (posicionConductor == 0)
    {
        cout << "No existe el conductor en nuestra base de datos" << endl;
        return;
    }

    fclose(f);
}

void listarTodosLosConductores(NodoConductor *listaConductores)
{
    NodoConductor *paux = listaConductores;

    //Validacion para verificar que exista la lista de Conductores.
    if (!paux)
    {
        limpiarConsola;
        cout << "Error: No existe el archivo Conductores.bin \n"
             << endl;
        return;
    }

    while (paux)
    {
        mostrarConductor(paux->conductor);
        paux = paux->next;
    }
}

Conductor crearNuevoConductor(NodoConductor *listaConductores)
{
    Conductor conductor;
    NodoConductor *ultimo_conductor = obtenerUltimoConductor(listaConductores);

    cout << "Ingrese fecha de vencimiento en formato AAAAMMDD " << endl;
    cin >> conductor.fechaVencimiento;

    cout << "Ingrese por favor un email del conductor" << endl;
    cin >> conductor.email;

    //Por default estas opciones despues lo podemos discutir si deben ser ingresadas por el usuario
    conductor.totalInfracciones = 0;
    conductor.activo = true;
    if (ultimo_conductor)
    {
        conductor.conductorId = (ultimo_conductor->conductor.conductorId + 1);
    }
    else
    {
        conductor.conductorId = 1;
    }

    agregarConductorAlArchivo(conductor);

    return conductor;
}

void agregarConductorAlArchivo(Conductor conductor)
{
    FILE *f;

    f = fopen("conductores.bin", "a");

    fwrite(&conductor, sizeof(Conductor), 1, f);

    fclose(f);

    limpiarConsola;

    cout << "El conductor con ID: " << conductor.conductorId << ". Fue guardado exitosamente en el archivo" << endl;
}

void cargarConductoresEnMemoria(NodoConductor *&listaConductores)
{
    FILE *f;
    NodoConductor *aux;
    Conductor conductor_aux;

    f = fopen("conductores.bin", "rb");

    if (f == NULL)
    {
        cout << "No existe el archivo conductores.bin" << endl;
        return;
    }

    fseek(f , 0 , SEEK_SET);

    while (fread(&conductor_aux, sizeof(Conductor), 1, f) == 1)
    {
        insertarConductorAlFinal(listaConductores, conductor_aux);
    }

    fclose(f);
}

void finalizar_jornada(NodoConductor *listaconductores)
{
    FILE *f;
    f = fopen("conductores.bin", "wb");
    int fechaActual;

    while (fechaActual <= 0)
    {
        cout << "Ingresa una fecha:  " << endl;
        cin >> fechaActual;
    }

    while (listaconductores)
    {
        if (listaconductores->conductor.fechaVencimiento > fechaActual && listaconductores->conductor.activo)
        {
            fwrite(&listaconductores->conductor, sizeof(Conductor), 1, f);
        }

        listaconductores = listaconductores->next;
    }
    fclose(f);
    return;
}

void borrar_lista_conductor(NodoConductor *&lista)
{
    NodoConductor *anterior;
    NodoConductor *paux;
    while (lista!=NULL)
    {
        anterior=NULL;
        paux= lista;
        while (paux && paux->next)
        {
            anterior = paux;
            paux= paux->next;
        }
        if (paux)
        {
            delete paux;
            if (anterior!=NULL)
                anterior->next=NULL;
            else
                lista =NULL;
        }
    }
    return;
}

void inicializar_archivo_conductores()
{
    Conductor conduc;
    FILE *f;
    f= fopen("conductores.bin","rb");
    if (f==NULL)
    {
        f= fopen("conductores.bin","wb");
        llenar_struct_conductor(conduc,1,20212810,"luchofoglia@gmail.com");
        fwrite(&conduc,sizeof(conduc),1,f);

        llenar_struct_conductor(conduc,2,20212824,"francofourmantin@gmail.com");
        fwrite(&conduc,sizeof(conduc),1,f);

        llenar_struct_conductor(conduc,3,20212828,"juanbolatti@gmail.com");
        fwrite(&conduc,sizeof(conduc),1,f);
    }
    fclose(f);
    return;
}

void llenar_struct_conductor(Conductor &conduc,int conductorId,long fechaVencimiento,char email[])
{
    conduc.activo= 1;
    conduc.conductorId= conductorId;
    strcpy(conduc.email,email);
    conduc.fechaVencimiento= fechaVencimiento;
    conduc.totalInfracciones= 0;
    return;
}

/**** FIN SUBPROGRAMAS DE CONDUCTORES ****/

/** SUBPROGRAMAS DE INFRACCIONES **/

void inicializar_archivo_infracciones(NodoConductor *listaconductores,NodoInfraccion *&listainfracciones)
{
    // supone que ya hay conductores cargados por lo que les
    // agrega infracciones
    FILE *f;
    f= fopen("procesados.bin","rb");
    if (f==NULL)
    {
        f= fopen("procesados.bin","wb");
        generarInfraccionesRandom(listaconductores,listainfracciones,10,false);

    }
    fclose(f);
    return;
}

// MOSTRAR Y LLENAR INFRACCIONES

void mostrar_infra(Infraccion infra)
{
    cout << "\n*ID de la infraccion: " << infra.infraccionId << endl;
    cout << "*Fecha y hora de la infraccion: " << infra.fechaHora << endl;
    cout << "*Monto a abonar: " << infra.monto << endl;
    cout << "*Provincia en la que se cometio: " << infra.provincia << endl;
    cout << "*Conductor ID: " << infra.conductorId << endl;
    return;
}

void llenar_struct_infraccion(Infraccion &infra, char fecha[], int infra_id, float monto, int provincia, int id_conductor)
{
    strcpy(infra.fechaHora, fecha);
    infra.infraccionId = infra_id;
    infra.monto = monto;
    infra.provincia = provincia;
    infra.conductorId = id_conductor;
    return;
}

void generarInfraccionesRandom(NodoConductor *conductores, NodoInfraccion *&infracciones, int cantidadInfracciones,bool mostrar_infracciones)
{
    NodoConductor *paux_conductor = conductores;
    NodoInfraccion *paux_infraccion = infracciones;
    Infraccion nueva_infraccion;
    int cantidadConductores = 0;
    int id_infraccion = 0;
    int maxInfraccionId = 0;
    int numRandom, diasRandom, mesesRandom, anoRandom, provincia, conductorId;
    char hora[] = "23:23";
    char fecha[8];
    char fechaHora[13];
    float monto;

    FILE *f;

    char nombre[50] = "";

    while (strlen(nombre) == 0)
    {
        cout << "Ingrese nombre del archivo:" << endl;
        cin >> nombre;
    }

    f = fopen(nombre, "a");

    while (paux_conductor)
    {
        cantidadConductores++;
        paux_conductor = paux_conductor->next;
    }

    while (paux_infraccion)
    {
        if (paux_infraccion->infraccion.infraccionId > maxInfraccionId)
        {
            maxInfraccionId = paux_infraccion->infraccion.infraccionId;
        }

        paux_infraccion = paux_infraccion->next;
    }

    id_infraccion = maxInfraccionId + 1;

    for (int i = 0; i < cantidadInfracciones; i++)
    {
        numRandom = generarNumeroEnteroRandom(1, cantidadConductores);
        diasRandom = generarNumeroEnteroRandom(1, 28); //28 Para que no exista un 30 de febrero o un 31 de septiembre
        mesesRandom = generarNumeroEnteroRandom(1, 12);
        conductorId = generarNumeroEnteroRandom(1, cantidadConductores);
        anoRandom = generarNumeroEnteroRandom(1998, 2021);
        monto = generarNumeroEnteroRandom(10000, 100000);
        provincia = generarNumeroEnteroRandom(1, 24);
        strcpy(fechaHora, "");

        nueva_infraccion.infraccionId = id_infraccion;
        nueva_infraccion.conductorId = conductorId;
        nueva_infraccion.monto = monto;
        nueva_infraccion.provincia = provincia;

        sprintf(fecha, "%d%d%d", anoRandom, mesesRandom, diasRandom);
        strcat(fechaHora, fecha);
        strcat(fechaHora, hora);

        strcpy(nueva_infraccion.fechaHora, fechaHora);

        fwrite(&nueva_infraccion, sizeof(Infraccion), 1, f);
        if (mostrar_infracciones)
        {
            cout << "---------------------------------------------------------------------------------" << endl;
            cout << "La infraccion con la siguiente informacion fue guardada correctamente: " << endl;
            cout << "id: " << nueva_infraccion.infraccionId << endl;
            cout << "conductorID: " << nueva_infraccion.conductorId << endl;
            cout << "monto: " << nueva_infraccion.monto << endl;
            cout << "Fecha y hora: " << nueva_infraccion.fechaHora << endl;
            cout << "provincia: " << nueva_infraccion.provincia << endl;
            cout << "---------------------------------------------------------------------------------" << endl;
        }
        ingresar_ordenadamente_por_conducID(infracciones, nueva_infraccion);

        id_infraccion++;
    }
}

// MOSTRAR INFRACCIONES CON UNA CONDICION

void mostar_infra_de_conductor(NodoInfraccion *listainfracciones)
{
    // RECIBE LA LISTA ORDENADA POR CONDUCTOR ID
    int id_a_buscar;
    cout << "Ingrese Id del conductor: " << endl;
    cin >> id_a_buscar;
    NodoInfraccion *paux = listainfracciones;
    while (paux)
    {
        if (paux->infraccion.conductorId == id_a_buscar)
        {
            mostrar_infra(paux->infraccion);
        }
        paux = paux->next;
    }
    return;
}

void mostrar_infractores_de_una_provincia(NodoInfraccion *listaInfracciones)
{
    // RECIBE LA LISTA ORDENADA POR CONDUCTOR ID
    int prov_a_buscar = -1;
    int id_anterior = -1;
    NodoInfraccion *paux = listaInfracciones;

    //Validacion para verificar que exista la lista de infracciones
    /*if(!paux)
    {
        limpiarConsola;
        cout << "Error: No hay infracciones cargadas. \n" << endl;
        return;
    }*/

    //Validación solo para verificar que ingrese un numero correcto
    while (prov_a_buscar < 1 || prov_a_buscar > 24)
    {
        cout << "Ingrese provincia de la que quiera ver sus infractores (1-24): " << endl;
        cin >> prov_a_buscar;
    }

    cout << "\nInfractores de la provincia: " << prov_a_buscar << endl;
    // Recorre la lista leyendo las infracciones
    while (paux)
    {
        if (paux->infraccion.conductorId != id_anterior && paux->infraccion.provincia == prov_a_buscar)
        {
            cout << "*ID del infractor: " << paux->infraccion.conductorId << endl;
            id_anterior = paux->infraccion.conductorId;
        }
        paux = paux->next;
    }
    return;
}

void mostrar_informe(NodoInfraccion *listainfracciones, NodoConductor *listaconductores)
{
    // RECIBE LA LISTA ORDENADA POR CONDUCTOR ID
    int id;
    cout << "Ingrese ID del conductor del que quiere ver su informe: " << endl;
    cin >> id;
    NodoConductor *paux = listaconductores;
    bool encontrado = false;
    while (paux && encontrado != true)
    {
        if (paux->conductor.conductorId == id)
        {
            mostrarConductor(listaconductores->conductor);
            encontrado = true;
        }
        paux = paux->next;
    }

    NodoInfraccion *paux2 = listainfracciones;
    while (paux2)
    {
        if (paux2->infraccion.conductorId == id)
        {
            mostrar_infra(paux2->infraccion);
        }
        paux2 = paux2->next;
    }
    return;
}

// CARGAR INFRACCIONES EN MEMORIA

void cargar_infracciones_en_memoria_ordena_conducID(NodoInfraccion *&listainfracciones)
{
    FILE *f;
    Infraccion infra;
    f = fopen("procesados.bin", "rb");
    if (f)
    {
        while (fread(&infra, sizeof(Infraccion), 1, f))
            ingresar_ordenadamente_por_conducID(listainfracciones, infra);
        fclose(f);
    }
    else
    { // Crea el archivo y se va porq entonces no hay
        // infracciones para cargar
        f = fopen("procesados.bin", "wb");
        fclose(f);
    }
    return;
}

// INGRESAR ORDENADAMENTE

void ingresar_ordenadamente_por_conducID(NodoInfraccion *&lista, Infraccion ticket)
{
    NodoInfraccion *paux = lista;
    NodoInfraccion *paux2 = NULL;
    if (lista == NULL)
    { /// crear el primer elemento
        lista = new NodoInfraccion();
        lista->previous = NULL;
        lista->next = NULL;
        // contenido del struct
        llenar_struct_infraccion(lista->infraccion, ticket.fechaHora, ticket.infraccionId, ticket.monto, ticket.provincia, ticket.conductorId);
    }
    else /// ya esta creada la lista
    {
        while (paux && paux->next && paux->next->infraccion.conductorId < ticket.conductorId)
            paux = paux->next;

        if (paux == lista && lista->infraccion.conductorId >= ticket.conductorId)
        { /// agrega un nodo al principio
            lista = new NodoInfraccion();
            lista->next = paux;
            paux->previous = lista;
            lista->previous = NULL;
            // contenido del struct
            llenar_struct_infraccion(lista->infraccion, ticket.fechaHora, ticket.infraccionId, ticket.monto, ticket.provincia, ticket.conductorId);
        }

        if (paux->next == NULL && paux->infraccion.conductorId <= ticket.conductorId)
        { /// si agrega al final
            paux->next = new NodoInfraccion();
            paux->next->previous = paux;
            paux = paux->next;
            paux->next = NULL;
            // contenido del struct
            llenar_struct_infraccion(paux->infraccion, ticket.fechaHora, ticket.infraccionId, ticket.monto, ticket.provincia, ticket.conductorId);
        }
        if (paux->infraccion.conductorId < ticket.conductorId)
        { /// agrega en el medio de otros dos
            paux2 = paux->next;
            paux->next = new NodoInfraccion();
            paux->next->previous = paux;
            paux = paux->next;
            paux2->previous = paux;
            paux->next = paux2;
            // contenido del struct
            llenar_struct_infraccion(paux->infraccion, ticket.fechaHora, ticket.infraccionId, ticket.monto, ticket.provincia, ticket.conductorId);
        }
    }

    return;
}

void ingresar_ordenadamente_por_infraccionID(NodoInfraccion *&lista, Infraccion ticket)
{
    NodoInfraccion *paux = lista;
    NodoInfraccion *paux2 = NULL;
    if (lista == NULL)
    { /// crear el primer elemento
        lista = new NodoInfraccion();
        lista->previous = NULL;
        lista->next = NULL;
        // contenido del struct
        llenar_struct_infraccion(lista->infraccion, ticket.fechaHora, ticket.infraccionId, ticket.monto, ticket.provincia, ticket.conductorId);
    }
    else /// ya esta creada la lista
    {
        while (paux && paux->next && paux->next->infraccion.infraccionId < ticket.infraccionId)
            paux = paux->next;

        if (paux == lista && lista->infraccion.infraccionId > ticket.infraccionId)
        { /// agrega un nodo al principio
            lista = new NodoInfraccion();
            lista->next = paux;
            paux->previous = lista;
            lista->previous = NULL;
            // contenido del struct
            llenar_struct_infraccion(lista->infraccion, ticket.fechaHora, ticket.infraccionId, ticket.monto, ticket.provincia, ticket.conductorId);
        }
        if (paux->next == NULL && paux->infraccion.infraccionId < ticket.infraccionId)
        { /// si agrega al final
            paux->next = new NodoInfraccion();
            paux->next->previous = paux;
            paux = paux->next;
            paux->next = NULL;
            // contenido del struct
            llenar_struct_infraccion(paux->infraccion, ticket.fechaHora, ticket.infraccionId, ticket.monto, ticket.provincia, ticket.conductorId);
        }
        if (paux->infraccion.infraccionId < ticket.infraccionId)
        { /// agrega en el medio de otros dos
            paux2 = paux->next;
            paux->next = new NodoInfraccion();
            paux->next->previous = paux;
            paux = paux->next;
            paux2->previous = paux;
            paux->next = paux2;
            // contenido del struct
            llenar_struct_infraccion(paux->infraccion, ticket.fechaHora, ticket.infraccionId, ticket.monto, ticket.provincia, ticket.conductorId);
        }
    }
    return;
}

// POP LISTA DE INFRACCIONES

void borrar_lista_infracciones(NodoInfraccion *&lista)
{

    NodoInfraccion *paux;
    while (lista != NULL) // mientras queden nodos
    {
        paux = lista;
        while (paux && paux->next)
            paux = paux->next; // obtiene el ultimo nodo
        if (paux)              // paux!=NULL
        {
            if (paux->previous != NULL) // si quedan nodos atras
            {
                paux->previous->next = NULL;
                delete paux;
            }
            else
            {
                delete paux;
                lista = NULL;
            }
        }
    }
    return;
}

// ARCHIVOS E INFRACCIONES

void actualizar_archivo_procesados(NodoInfraccion *listaInfracciones, NodoConductor *listaConductores)
{
    int cantidadInfracciones = 0;
    int id_conductor_anterior = -1;
    NodoConductor *paux_conductores = listaConductores;
    // Para actualizarlo, lo sobreescribe cargandolo con la lista.
    FILE *f;
    f = fopen("procesados.bin", "wb");

    while (listaInfracciones && listaConductores)
    {
        fwrite(&listaInfracciones->infraccion, sizeof(Infraccion), 1, f);

        if (listaInfracciones->infraccion.conductorId != id_conductor_anterior)
        {
            if (cantidadInfracciones != 0)
            {
                listaConductores->conductor.totalInfracciones = cantidadInfracciones;
                actualizarTotalinfraccionesArchivo(paux_conductores, listaConductores->conductor.conductorId, cantidadInfracciones);
                listaConductores = listaConductores->next;
            }

            id_conductor_anterior = listaInfracciones->infraccion.conductorId;
            cantidadInfracciones = 1;
        }
        else
        {
            cantidadInfracciones++;
        }

        if (!listaInfracciones->next)
        {
            listaConductores->conductor.totalInfracciones = cantidadInfracciones;
            actualizarTotalinfraccionesArchivo(paux_conductores, listaConductores->conductor.conductorId, cantidadInfracciones);
            listaConductores = listaConductores->next;
        }

        listaInfracciones = listaInfracciones->next;
    }

    fclose(f);
    return;
}

/**** FIN SUBPROGRAMAS DE INFRACCIONES ****/

/** SUBPROGRAMAS PARA EXPORTAR **/

void exportarHTML(NodoConductor *listaConductores)
{
    NodoConductor *paux = listaConductores;

    //Validacion para verificar que exista la lista de Conductores.
    if (!paux)
    {
        limpiarConsola;
        cout << "Error: No hay conductores cargados. \n"
             << endl;
        return;
    }

    int fecha1, fecha2;
    cout << "Ingrese la primera fecha entre las que quiere exportar: " << endl;
    cin >> fecha1;
    cout << "Ingrese la segunda fecha: " << endl;
    cin >> fecha2;

    FILE *f;
    f = fopen("vencidos.html", "wt");
    fprintf(f, "<!DOCTYPE html>\n"); //Le declara al navegador que tipo de archivo es (para evitar problemas)
    fprintf(f, "<html>\n");          //Abre etiqueta html

    fprintf(f, "<head>\n");                              //Abre etiqueta Head, no visible para el usuario
    fprintf(f, "<title>Conductores Vencidos</title>\n"); //Es el titulo que aparece en el navegador
    fprintf(f, "<meta charset='UTF-8'>\n");              //Para que hayan tildes y otros simbolos
    fprintf(f, "</head>\n");                             //cierra etiqueta head

    fprintf(f, "<body>\n"); //abre etiqueta body, es visible para el usuario
    //Los atributos style son para que se vea mejor, y si el programa no las puede leer no salta ningun error, solo las ignora.
    fprintf(f, "<h1 style='text-align: center;'>Lista de Conductores Vencidos</h1>\n"); //h1 es el titulo

    fprintf(f, "<table border=1 style='margin: auto;'>\n");                                      //La tabla donde van a estar las cosas
    fprintf(f, "<th style='padding: 10px; background-color: #94CBFF;'>ID del conductor</th>\n"); //Titulos de las columnas
    fprintf(f, "<th style='padding: 10px; background-color: #94CBFF;'>Fecha del Vencimiento</th>\n");
    fprintf(f, "<th style='padding: 10px; background-color: #94CBFF;'>Total de infracciones</th>\n");
    fprintf(f, "<th style='padding: 10px; background-color: #94CBFF;'>Email</th>\n");

    if (fecha1 != fecha2)
    {
        while (paux)
        {
            if (fecha1 > fecha2)
            {
                if ((paux->conductor.fechaVencimiento < fecha1) && (paux->conductor.fechaVencimiento > fecha2))
                {
                    fprintf(f, "<tr>\n");
                    fprintf(f, "<td>%d</td> <td>%ld</td> <td>%d</td> <td>%s</td>\n", paux->conductor.conductorId, paux->conductor.fechaVencimiento, paux->conductor.totalInfracciones, paux->conductor.email);
                    fprintf(f, "</tr>\n");
                }
            }
            else
            {
                if ((paux->conductor.fechaVencimiento < fecha2) && (paux->conductor.fechaVencimiento > fecha1))
                {
                    fprintf(f, "<tr>\n");
                    fprintf(f, "<td>%d</td> <td>%ld</td> <td>%d</td> <td>%s</td>\n", paux->conductor.conductorId, paux->conductor.fechaVencimiento, paux->conductor.totalInfracciones, paux->conductor.email);
                    fprintf(f, "</tr>\n");
                }
            }
            paux = paux->next;
        }
    }
    else
    {
        cout << "Error: Ingreso dos fechas iguales" << endl;
        fclose(f);
        return;
    }

    fprintf(f, "</table>\n"); //cierra etiqueta title
    fprintf(f, "</body>\n");  //cierra etiqueta body
    fprintf(f, "</html>\n");  //cierra etiqueta Html

    fclose(f);

    cout << "" << endl;
    cout << "El archivo HTML fue exportado correctamente" << endl;
    cout << "" << endl;

    return;
}

void exportarCSV(NodoConductor *listaConductores)
{
    NodoConductor *paux = listaConductores;

    //Validacion para verificar que exista la lista de Conductores.
    if (!paux)
    {
        limpiarConsola;
        cout << "Error: No hay conductores cargados \n"
             << endl;
        return;
    }

    int fecha1, fecha2;
    cout << "Ingrese la primera fecha entre las que quiere exportar: " << endl;
    cin >> fecha1;
    cout << "Ingrese la segunda fecha: " << endl;
    cin >> fecha2;

    FILE *f;
    f = fopen("vencidos.csv", "wt");
    fprintf(f, "Id del conductor;Fecha de vencimiento;Total de Infracciones;Email del conductor\n");

    if (fecha1 != fecha2)
    {
        while (paux)
        {
            if (fecha1 > fecha2)
            {
                if ((paux->conductor.fechaVencimiento < fecha1) && (paux->conductor.fechaVencimiento > fecha2))
                {
                    fprintf(f, "%d;%d;%d;%s\n", paux->conductor.conductorId, paux->conductor.fechaVencimiento, paux->conductor.totalInfracciones, paux->conductor.email);
                }
            }
            else
            {
                if ((paux->conductor.fechaVencimiento < fecha2) && (paux->conductor.fechaVencimiento > fecha1))
                {
                    fprintf(f, "%d;%d;%d;%s\n", paux->conductor.conductorId, paux->conductor.fechaVencimiento, paux->conductor.totalInfracciones, paux->conductor.email);
                }
            }
            paux = paux->next;
        }
    }
    else
    {
        cout << "Error: Ingreso dos fechas iguales" << endl;
        fclose(f);
        return;
    }

    fclose(f);

    cout << "" << endl;
    cout << "El archivo CSV fue exportado correctamente" << endl;
    cout << "" << endl;
    return;
}

/**** FIN SUBPROGRAMAS PARA EXPORTAR ****/

/** AYUDINES **/
int generarNumeroEnteroRandom(int min, int max)
{
    return min + rand() % (max + 1 - min);
}

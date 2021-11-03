// Copyright 2021 Los pibes SA
// Author: Luciano Foglia
// Author: Franco Fourmantin
// Author: Juan Bolatti

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
void procesar_lote(NodoInfraccion *&listaInfracciones, NodoConductor *listaconductores);
void inicializar_archivo_infracciones(NodoInfraccion *listainfracciones);
void inicializar_archivo_conductores();
void finalizar_jornada(NodoConductor *&listaconductores,NodoInfraccion *&listaInfracciones, long fechaActual);

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
void borrar_lista_conductor(NodoConductor *&lista);
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
void generarInfraccionesRandom(NodoConductor *conductores, NodoInfraccion *&infracciones, int cantidadInfracciones);

//Listas Infracciones
void cargar_infracciones_en_memoria_ordena_conducID(NodoInfraccion *&listainfracciones);
void ingresar_ordenadamente_por_conducID(NodoInfraccion *&lista, Infraccion ticket);
void borrar_lista_infracciones(NodoInfraccion *&lista);

//Archivos de texto
void exportarHTML(NodoConductor *listaConductores);
void exportarCSV(NodoConductor *listaConductores);

//Archivos
void actualizar_archivo_conductores(NodoConductor *listaconductores,long fechaActual);
void actualizar_archivo_procesados(NodoInfraccion *listaInfracciones);

//Ayudines
int generarNumeroEnteroRandom(int min, int max);
void actualizarTotalinfracciones(NodoInfraccion *listaInfracciones, NodoConductor *listaConductores);
void actualizarProcesados(char nombre[]);

/// VERSION 2
void generarInfraccionesRandom_V2(NodoConductor *conductores, NodoInfraccion *&infracciones, int cantidadInfracciones, char nombre[]);
void procesar_lote_V2(NodoConductor *listaconductores,NodoInfraccion *&listainfracciones);
void actualizarTotalinfracciones_V2(char nombre[], NodoConductor *listaConductores);

void leer_lote();

/**** FIN DECLARACIONES DE FUNCIONES ****/

int main()
{
    setlocale(LC_ALL, "Spanish");

    int opcionMenu;
    int conductorId;
    NodoConductor *listaConductores = NULL;
    NodoInfraccion *listaInfracciones = NULL;

    inicializar_archivo_conductores();
    inicializar_archivo_infracciones(listaInfracciones);


    cargarConductoresEnMemoria(listaConductores);
    cargar_infracciones_en_memoria_ordena_conducID(listaInfracciones);

    actualizarTotalinfracciones(listaInfracciones,listaConductores);


    long fechaActual;

    cout << "Ingrese fecha actual (AAAAMMDD):" << endl;
    cin >> fechaActual;



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
            procesar_lote_V2(listaConductores,listaInfracciones);
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
            actualizar_archivo_conductores(listaConductores,fechaActual);
            fechaActual++;
            opcionMenu= 0;
            break;
        default:
            break;
        }
        mostrarMenu(opcionMenu);
    }
    borrar_lista_conductor(listaConductores);
    borrar_lista_infracciones(listaInfracciones);
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
    cout << "9 - Finalizar la jornada y  salir del programa" << endl;
    cout << "0 - Salir del programa" << endl;
    cout << "**************************************************" << endl;

    cin >> opcionMenu;
}

void procesar_lote(NodoInfraccion *&listaInfracciones, NodoConductor *listaconductores)
{
    generarInfraccionesRandom(listaconductores,listaInfracciones,generarNumeroEnteroRandom(15,25));
    actualizarTotalinfracciones(listaInfracciones,listaconductores);
    return;
}

/// VERSION 2 DE TODO LO NUEVO

void procesar_lote_V2(NodoConductor *listaconductores,NodoInfraccion *&listainfracciones)
{
    char nombre[50]="";
    FILE *f;
    do
    {
        if(f==NULL)
        {
            cout << "Error: el nombre del archivo no existe.\n" << endl;
        }
        cout << "Ingrese nombre del lote a procesar:" << endl;
        cin >> nombre;
        f =fopen(nombre,"rb");
    } while (f==NULL);

    fseek(f, 0, SEEK_END);
    if(ftell(f) == 0)
    {
        // El archivo existe y esta vacio => lo carga con infracciones random
        generarInfraccionesRandom_V2(listaconductores,listainfracciones,5,nombre);
    }

    fclose(f);

    actualizarTotalinfracciones_V2(nombre,listaconductores);
    actualizarProcesados(nombre);

    return;
}

void actualizarTotalinfracciones_V2(char nombre[], NodoConductor *listaConductores)
{
    int cantidadInfracciones = 0;
    int id_conductor_anterior = -1;
    FILE *f;
    f = fopen(nombre,"rb");
    Infraccion infra;
    NodoConductor *paux;

    // MIENTRAS QUEDEN INFRACCIONES EN EL ARCHIVO Y CONDUCTORES EN LA LISTA.
    while (fread(&infra,sizeof(Infraccion),1,f))
        {
            // Apunta paux al principio de la lista para volver a buscar conductores
            paux= listaConductores;

            // Busca al conductor con mismo ID que el de la multa
            while (paux && infra.conductorId!=paux->conductor.conductorId)
                paux= paux->next;

            paux->conductor.totalInfracciones++;
        }
    fclose(f);

    return;
}

void actualizarProcesados(char nombre[])
{
    FILE *f, *f2;
    f = fopen(nombre,"rb");
    Infraccion infra;
    f2 = fopen("procesados.bin", "ab");
    while (fread(&infra,sizeof(Infraccion),1,f))
    {
        fwrite(&infra,sizeof(Infraccion),1,f2);
    }
    fclose(f);
    fclose(f2);
    return;
}

void generarInfraccionesRandom_V2(NodoConductor *conductores, NodoInfraccion *&infracciones, int cantidadInfracciones, char nombre[])
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

    f = fopen(nombre, "wb");

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

        cout << "---------------------------------------------------------------------------------" << endl;
        cout << "La infraccion con la siguiente informacion fue guardada correctamente: " << endl;
        cout << "id: " << nueva_infraccion.infraccionId << endl;
        cout << "conductorID: " << nueva_infraccion.conductorId << endl;
        cout << "monto: " << nueva_infraccion.monto << endl;
        cout << "Fecha y hora: " << nueva_infraccion.fechaHora << endl;
        cout << "provincia: " << nueva_infraccion.provincia << endl;
        cout << "---------------------------------------------------------------------------------" << endl;

        id_infraccion++;
    }
    fclose(f);
    return;
}

/// FIN SEGUNDAS VERSIONES

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
    return;
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
    return;
}

NodoConductor *obtenerUltimoConductor(NodoConductor *listaConductores)
{
    while (listaConductores && listaConductores->next)
    {
        listaConductores = listaConductores->next;
    }

    return listaConductores;
}

void desactivarConductor(NodoConductor *listaConductores, int conductorId)
{
    NodoConductor *aux = listaConductores;
    if (!aux)
    {
        limpiarConsola;
        cout << "Error: No existe el archivo Conductores.bin \n"
             << endl;
        return;
    }

    while (aux)
    {
        if (aux->conductor.conductorId == conductorId)
        {
            aux->conductor.activo = false;
        }

        aux = aux->next;
    }
    return;
}

void listarTodosLosConductores(NodoConductor *listaConductores)
{
    NodoConductor *paux = listaConductores;

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
    return;
}

void actualizarTotalinfracciones(NodoInfraccion *listaInfracciones, NodoConductor *listaConductores)
{
    int cantidadInfracciones = 0;
    int id_conductor_anterior = -1;

    while (listaInfracciones && listaConductores)
        {
            if (listaInfracciones->infraccion.conductorId != id_conductor_anterior)
            {
                if (cantidadInfracciones != 0)
                {
                    listaConductores->conductor.totalInfracciones = cantidadInfracciones;
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
                listaConductores = listaConductores->next;
            }

            listaInfracciones = listaInfracciones->next;
        }
    return;
}

Conductor crearNuevoConductor(NodoConductor *listaConductores)
{
    Conductor conductor;
    NodoConductor *ultimo_conductor = obtenerUltimoConductor(listaConductores);

    cout << "Ingrese fecha de vencimiento en formato AAAAMMDD " << endl;
    cin >> conductor.fechaVencimiento;

    cout << "Ingrese por favor un email del conductor" << endl;
    cin >> conductor.email;

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

    return;
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
    return;
}

void finalizar_jornada(NodoConductor *&listaConductores,NodoInfraccion *&listaInfracciones,long fechaActual)
{
    actualizarTotalinfracciones(listaInfracciones,listaConductores);
    actualizar_archivo_conductores(listaConductores,fechaActual);
    actualizar_archivo_procesados(listaInfracciones);
    borrar_lista_conductor(listaConductores);
    borrar_lista_infracciones(listaInfracciones);
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
        llenar_struct_conductor(conduc,1,20210510,"luchofoglia@gmail.com");
        fwrite(&conduc,sizeof(conduc),1,f);

        llenar_struct_conductor(conduc,2,20210921,"francofourmantin@gmail.com");
        fwrite(&conduc,sizeof(conduc),1,f);

        llenar_struct_conductor(conduc,3,20210728,"juanbolatti@gmail.com");
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

void inicializar_archivo_infracciones(NodoInfraccion *listainfraccion)
{
    FILE *f,*f2;
    Infraccion infra;
    f= fopen("procesados.bin","rb");
    if (f==NULL)
    {
        f= fopen("procesados.bin","wb");

        f2= fopen("Lote1","wb");
        fclose(f2);

        f2= fopen("Lote2","wb");
        fclose(f2);

        f2= fopen("Lote3","wb");
        fclose(f2);

        llenar_struct_infraccion(infra,"20210817",1,generarNumeroEnteroRandom(10000,100000),generarNumeroEnteroRandom(1,24),1);
        fwrite(&infra,sizeof(Infraccion),1,f);

        llenar_struct_infraccion(infra,"20191021",2,generarNumeroEnteroRandom(10000,100000),generarNumeroEnteroRandom(1,24),1);
        fwrite(&infra,sizeof(Infraccion),1,f);

        llenar_struct_infraccion(infra,"20050515",3,generarNumeroEnteroRandom(10000,100000),generarNumeroEnteroRandom(1,24),2);
        fwrite(&infra,sizeof(Infraccion),1,f);

        llenar_struct_infraccion(infra,"20071205",4,generarNumeroEnteroRandom(10000,100000),generarNumeroEnteroRandom(1,24),2);
        fwrite(&infra,sizeof(Infraccion),1,f);

        llenar_struct_infraccion(infra,"20100108",5,generarNumeroEnteroRandom(10000,100000),generarNumeroEnteroRandom(1,24),3);
        fwrite(&infra,sizeof(Infraccion),1,f);

        llenar_struct_infraccion(infra,"20040427",6,generarNumeroEnteroRandom(10000,100000),generarNumeroEnteroRandom(1,24),2);
        fwrite(&infra,sizeof(Infraccion),1,f);

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

void generarInfraccionesRandom(NodoConductor *conductores, NodoInfraccion *&infracciones, int cantidadInfracciones)
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

        cout << "---------------------------------------------------------------------------------" << endl;
        cout << "La infraccion con la siguiente informacion fue guardada correctamente: " << endl;
        cout << "id: " << nueva_infraccion.infraccionId << endl;
        cout << "conductorID: " << nueva_infraccion.conductorId << endl;
        cout << "monto: " << nueva_infraccion.monto << endl;
        cout << "Fecha y hora: " << nueva_infraccion.fechaHora << endl;
        cout << "provincia: " << nueva_infraccion.provincia << endl;
        cout << "---------------------------------------------------------------------------------" << endl;

        ingresar_ordenadamente_por_conducID(infracciones, nueva_infraccion);

        id_infraccion++;
    }
    return;
}

// MOSTRAR INFRACCIONES CON UNA CONDICION

void mostar_infra_de_conductor(NodoInfraccion *listainfracciones)
{
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

    int prov_a_buscar = -1;
    int id_anterior = -1;
    NodoInfraccion *paux = listaInfracciones;

    while (prov_a_buscar < 1 || prov_a_buscar > 24)
    {
        cout << "Ingrese provincia de la que quiera ver sus infractores (1-24): " << endl;
        cin >> prov_a_buscar;
    }

    cout << "\nInfractores de la provincia: " << prov_a_buscar << endl;

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
    int id;
    cout << "Ingrese ID del conductor del que quiere ver su informe: " << endl;
    cin >> id;
    NodoConductor *paux = listaconductores;
    bool encontrado = false;
    while (listaconductores && encontrado==false)
    {
        if (listaconductores->conductor.conductorId == id)
        {
            mostrarConductor(listaconductores->conductor);
            encontrado = true;
        }
        listaconductores = listaconductores->next;
    }

    while (listainfracciones)
    {
        if (listainfracciones->infraccion.conductorId == id)
        {
            mostrar_infra(listainfracciones->infraccion);
        }
        listainfracciones = listainfracciones->next;
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
        {
            ingresar_ordenadamente_por_conducID(listainfracciones, infra);
        }
        fclose(f);
    }
    else
    {
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
    {
        lista = new NodoInfraccion();
        lista->previous = NULL;
        lista->next = NULL;
        llenar_struct_infraccion(lista->infraccion, ticket.fechaHora, ticket.infraccionId, ticket.monto, ticket.provincia, ticket.conductorId);
    }
    else
    {
        while (paux && paux->next && paux->next->infraccion.conductorId < ticket.conductorId)
            paux = paux->next;

        if (paux == lista && lista->infraccion.conductorId >= ticket.conductorId)
        {
            lista = new NodoInfraccion();
            lista->next = paux;
            paux->previous = lista;
            lista->previous = NULL;
            llenar_struct_infraccion(lista->infraccion, ticket.fechaHora, ticket.infraccionId, ticket.monto, ticket.provincia, ticket.conductorId);
        }

        if (paux->next == NULL && paux->infraccion.conductorId < ticket.conductorId)
        {
            paux->next = new NodoInfraccion();
            paux->next->previous = paux;
            paux = paux->next;
            paux->next = NULL;
            llenar_struct_infraccion(paux->infraccion, ticket.fechaHora, ticket.infraccionId, ticket.monto, ticket.provincia, ticket.conductorId);
        }
        if (paux->infraccion.conductorId < ticket.conductorId)
        {
            paux2 = paux->next;
            paux->next = new NodoInfraccion();
            paux->next->previous = paux;
            paux = paux->next;
            paux2->previous = paux;
            paux->next = paux2;
            llenar_struct_infraccion(paux->infraccion, ticket.fechaHora, ticket.infraccionId, ticket.monto, ticket.provincia, ticket.conductorId);
        }
    }
    return;
}

// BORRAR LISTA DE INFRACCIONES

void borrar_lista_infracciones(NodoInfraccion *&lista)
{

    NodoInfraccion *paux;
    while (lista != NULL)
    {
        paux = lista;
        while (paux && paux->next)
            paux = paux->next;
        if (paux)
        {
            if (paux->previous != NULL)
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

/// ACTUALIZACION DE ARCHIVOS

void actualizar_archivo_conductores(NodoConductor *listaconductores,long fechaActual)
{
    FILE *f;
    f = fopen("conductores.bin", "wb");
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

void actualizar_archivo_procesados(NodoInfraccion *listaInfracciones)
{
    FILE *f;
    f = fopen("procesados.bin", "wb");
    while (listaInfracciones)
    {
        fwrite(&listaInfracciones->infraccion, sizeof(Infraccion), 1, f);
        listaInfracciones= listaInfracciones->next;
    }
    fclose(f);
    return;
}

/**** FIN SUBPROGRAMAS DE INFRACCIONES ****/

/** SUBPROGRAMAS PARA EXPORTAR **/

void exportarHTML(NodoConductor *listaConductores)
{
    NodoConductor *paux = listaConductores;

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
    fprintf(f, "<!DOCTYPE html>\n");
    fprintf(f, "<html>\n");

    fprintf(f, "<head>\n");
    fprintf(f, "<title>Conductores Vencidos</title>\n");
    fprintf(f, "<meta charset='UTF-8'>\n");
    fprintf(f, "</head>\n");

    fprintf(f, "<body>\n");
    fprintf(f, "<h1 style='text-align: center;'>Lista de Conductores Vencidos</h1>\n");

    fprintf(f, "<table border=1 style='margin: auto;'>\n");
    fprintf(f, "<th style='padding: 10px; background-color: #94CBFF;'>ID del conductor</th>\n");
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

    //system("start chrome %cd%/vencidos.html");

    return;
}

void exportarCSV(NodoConductor *listaConductores)
{
    NodoConductor *paux = listaConductores;


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

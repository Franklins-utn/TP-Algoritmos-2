#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

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
};

/**** FIN DECLARACIONES DE CONSTANTES ****/

/**** DECLARACIONES DE FUNCIONES ****/
void mostrarMenu(int &opcionMenu);

//Conductores
void agregarConductorAlArchivo(Conductor conductor);
void mostrarConductor(Conductor conductor);
void mostrar_informe();

//Listas conductores
void cargarConductoresEnMemoria(NodoConductor *&listaConductores); //Esta función genera una lista SE de conductores

//CRUD Listas conductores
void insertarConductorAlFinal(NodoConductor *&listaConductores, Conductor conductor);
void listarTodosLosConductores(NodoConductor *listaConductores);
void desactivarConductor(NodoConductor *listaConductores ,  int conductorId);
Conductor crearNuevoConductor(NodoConductor *listaConductores);
NodoConductor *obtenerUltimoConductor(NodoConductor *listaConductores);


//Infracciones
void cargar_infraccion_1(Conductor conductor);
void cargar_infraccion_2(Infraccion &infra,char fecha[],int infra_id,float monto,int provincia,int id_conductor);
void mostrar_infra(Infraccion infra);
void mostar_infra_de_conductor();
void mostrar_infractores_de_una_provincia();


void generarInfraccionesRandom( int cantidadInfracciones);

//Ayudines
void limpiarConsola();
int generarNumeroEnteroRandom(int min , int max);
float generarNumeroDecimalRandom(float min , float max);


/**** FIN DECLARACIONES DE FUNCIONES ****/
int main()
{
    int opcionMenu;
    int conductorId;
    int totalConductores = 0;
    int cantidadRandomInfracciones;
    NodoConductor *listaConductores = NULL;
    
    cargarConductoresEnMemoria(listaConductores);
    mostrarMenu(opcionMenu);

    while (opcionMenu != 0)
    {
        switch (opcionMenu)
        {
        case 1:
            insertarConductorAlFinal(listaConductores , crearNuevoConductor(listaConductores));
            break;
        case 2:
            listarTodosLosConductores(listaConductores);
            break;
        case 3:
            listarTodosLosConductores(listaConductores);
            cout << "Ingrese un ID de conductor para desactivar: "  << endl;
            cin >> conductorId;
            desactivarConductor(listaConductores ,  conductorId);
            break;
        case 4:
            //mostrar_infractores_de_una_provincia();
            break;
        case 5:
        case 6:
            //listarTodosLosConductores(conductores , totalConductores);
            //mostrar_informe();
            break;
        case 7:
        case 8:
            system("cls");
            cout << "Esta funcion todavia no se encuentra desarrollada" << endl;
            break;
        case 9: 
            while (cantidadRandomInfracciones < 5 || cantidadRandomInfracciones > 500)
            {
                cout << "Ingrese una cantidad de infracciones a ser generadas de manera random (5min - 500max): " << endl;
                cin >> cantidadRandomInfracciones;
            }
            generarInfraccionesRandom(cantidadRandomInfracciones);
        default:
            break;
        }
        mostrarMenu(opcionMenu);
    }

    cout << "\nEl programa ha finalizado." << endl;
    return 0;
}

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


/** Subprogramas **/

void insertarConductorAlFinal(NodoConductor *&listaConductores , Conductor conductor)
{
    NodoConductor *aux;

    if(listaConductores == NULL)
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


void generarInfraccionesRandom(int cantidadInfracciones)
{
    /*
    Okey, para generar infracciones random tenemos un tema, basicamente
    necesitamos todos los ID de conductores que ya existan en el archivo
    ya que en caso contrario se podrian generar infracciones para 
    conductores que no existen.
    Por ende la secuencia de pasos a seguir para generar infracciones random va a ser:
    1- Obtener un array con todos los ID de conductores que tengamos, dado que nunca vamos
    a saber cuantos conductores vamos a tener vamos a utilizar una LSE. 
    2- Para la fecha hora vamos a randomear solo los meses,dias y año, ya que es el dato por el 
    cual vamos a filtrar en un futuro. Hora y minutos lo vamos a dejar estatico.
    3- Para el monto simple, vamos a generar un random entre 1000,00 y 100000,99
    4- Para la provincia vamos a tirar un random entre 1 y 24 que son las provincias que tenemos
    5- Para obtener IDs correctos de las infracciones debemos acceder al archivo "procesados" y obtener
    el último id y empezar a contar a partir de ahi ( cuidado aca tiene que estar ordenado por los ID )
    5- Todo esto lo vamos a hacer en memoria y luego pasarlos a un archivo.
    */
}

void mostrarConductor(Conductor conductor)
{
    cout << "-----------------------------------------------------------------" << endl;

    cout << "Información acerca del conductor con ID: " << conductor.conductorId << endl;

    cout << "Email: " << conductor.email << endl;

    cout << "Fecha de vencimiento: " << conductor.fechaVencimiento << endl;

    cout << "Activo: " << conductor.activo << endl;;

    cout << "Total infracciones: " << conductor.totalInfracciones << endl;

    cout << "-----------------------------------------------------------------" << endl;

}

NodoConductor *obtenerUltimoConductor(NodoConductor *listaConductores)
{
    while(listaConductores && listaConductores->next)
    {
        listaConductores = listaConductores->next;
    }

    return listaConductores;
}


void desactivarConductor(NodoConductor *listaConductores ,  int conductorId)
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
    FILE *f;
    Conductor conductor;
    int posicionConductor = 0;

    //Guardamos el inicio de la lista
    NodoConductor *aux = listaConductores;
    while(aux)
    {
        if(aux->conductor.conductorId == conductorId){
            aux->conductor.activo = false; 
        }

        aux = aux->next;
    }


    f = fopen("conductores.bin" , "rb+");

    while( fread(&conductor , sizeof(Conductor) , 1 , f) == 1  ){
        if(conductor.conductorId == conductorId){
            posicionConductor = ftell(f);
        }
    }

    if(posicionConductor == 0){
        cout << "No existe el conductor en nuestra base de datos" << endl;
        return;
    }

    fseek( f , posicionConductor , SEEK_SET );

    fread( &conductor , sizeof(Conductor) , 1 , f );

    conductor.activo = false;

    /*
    esto es tremendo, basicamente lo que pasa aca es que para escribir en la posicion deseada
    hay que restarle a la posicion donde esta el struct guardado el tama�o del propio struct
    para que edite justo esa linea.
    */
    fseek( f , posicionConductor - sizeof(Conductor) , SEEK_SET );

    fwrite(&conductor , sizeof(Conductor) , 1 , f);

    fclose(f);

}

void listarTodosLosConductores(NodoConductor *listaConductores)
{
    NodoConductor *paux = listaConductores;

    while(paux)
    {
        mostrarConductor(paux->conductor);
        paux = paux->next;
    }
}

Conductor crearNuevoConductor(NodoConductor * listaConductores)
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
    if(ultimo_conductor)
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

    limpiarConsola();

    cout << "El conductor con ID: " << conductor.conductorId << ". Fue guardado exitosamente en el archivo" << endl;
}

void cargarConductoresEnMemoria(NodoConductor *&listaConductores)
{
    FILE *f;
    NodoConductor *aux;
    Conductor conductor_aux;

    f = fopen("conductores.bin", "rb");

    if(f == NULL)
    {
        cout << "No existe el archivo conductores.bin" << endl;
        return;
    }

    while ( fread(&conductor_aux, sizeof(Conductor), 1, f) == 1 )
    {
        insertarConductorAlFinal(listaConductores , conductor_aux);
    }


    fclose(f);
}

void limpiarConsola()
{
    system("clear"); //Si estan usando windows creo que aca hay que poner "cls"
}



void cargar_infraccion_1(Conductor conductor)
{
    FILE *f;
    Infraccion infra;
    int infra_id_ultimo=0;
    f=fopen("infracciones","rb");
    if (f == NULL)
    {   // para comprobar si existe el archivo de infracciones
        // o crearlo si fuera necesario.
        f=fopen("infracciones","wb");
    }
    else
    {
        fseek(f,(-1)*sizeof(Infraccion),SEEK_END);
        fread(&infra,sizeof(Infraccion),1,f);
        infra_id_ultimo = infra.infraccionId;
    }
    fclose(f);
    // si es la primera vez q se abre el archivo, no ejecuta el else
    // entonces,infra_id_ultimo=0.
    fopen("infracciones","ab");
    for (int l=0;l<conductor.totalInfracciones;l++)
    {
        cargar_infraccion_2(infra,"2022062213:30",infra_id_ultimo+l+1,l+150.83,2,conductor.conductorId);
        fwrite(&infra,sizeof(Infraccion),1,f);
        // escribe las infracciones en el archivo de infracciones
    }
    fclose(f);
    return;
}

void cargar_infraccion_2(Infraccion &infra,char fecha[],int infra_id,float monto,int provincia,int id_conductor)
{
    strcpy(infra.fechaHora,fecha);
    infra.infraccionId=infra_id;
    infra.monto=monto;
    infra.provincia=provincia;
    infra.conductorId=id_conductor;
    return;
}

void mostrar_infra(Infraccion infra)
{
    cout << "\n*ID de la infraccion: " << infra.infraccionId << endl;
    cout << "*Fecha y hora de la infraccion: " << infra.fechaHora << endl;
    cout << "*Monto a abonar: " << infra.monto << endl;
    cout << "*Provincia en la que se cometio: " << infra.provincia << endl;
    return;
}

void mostar_infra_de_conductor()
{
    Infraccion infra;
    FILE *f;
    int id_a_buscar;
    f=fopen("infracciones","rb");
    cout << "Ingrese Id del conductor: " << endl;
    cin >> id_a_buscar;
    while (fread(&infra,sizeof(Infraccion),1,f))
    {
        if (infra.conductorId==id_a_buscar)
        {
            mostrar_infra(infra);
        }
    }
    fclose(f);
    return;
}

void mostrar_infractores_de_una_provincia()
{
    // Listar todos los conductores que tengan al menos
    // una infracción en una provincia determinada.
    int prov_a_buscar = -1;
    FILE *f;
    Infraccion infra;
    int id_anterior;

    //Validación solo para verificar que ingrese un número correcto
    while (prov_a_buscar < 1 || prov_a_buscar > 24)
    {
        cout << "Ingrese provincia de la que quiera ver sus infractores (1-24): " << endl;
        cin >> prov_a_buscar;
    }

    cout << "\nInfractores de la provincia: " << prov_a_buscar << endl;

    f=fopen("infracciones","rb");
    while (fread(&infra,sizeof(Infraccion),1,f))
    {
        if(infra.conductorId!=id_anterior && infra.provincia==prov_a_buscar)
            {
                cout << "*ID del infractor: " << infra.conductorId << endl;
            }
        id_anterior=infra.conductorId;
    }
    fclose(f);
    return;
}

void mostrar_informe()
{
    int id;
    cout << "Ingrese ID del conductor del que quiere ver su informe: " << endl;
    cin >> id;
    Conductor conductor;
    FILE *f;
    f=fopen("conductores.bin","r");
    while (fread(&conductor,sizeof(Conductor),1,f))
    {
        if (conductor.conductorId==id)
        {
            mostrarConductor(conductor);
        }
    }
    fclose(f);
    Infraccion infra;
    f=fopen("infracciones","rb");
    cout << "Informacion sobre sus infracciones: " << endl;
    while (fread(&infra,sizeof(Infraccion),1,f))
    {
        if(infra.conductorId==id)
        {
            mostrar_infra(infra);
        }
    }
    fclose(f);
    return;
}

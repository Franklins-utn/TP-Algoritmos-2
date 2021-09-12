#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

/**** DECLARACIONES DE CONSTANTES ****/
struct Conductor
{
    int conductorId;
    double fechaVencimiento;
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
/**** FIN DECLARACIONES DE CONSTANTES ****/

/**** DECLARACIONES DE FUNCIONES ****/
void mostrarMenu(int &opcionMenu);

//Conductores
void cargarConductoresEnMemoria(Conductor *conductores , int &totalConductores);
void crearNuevoConductor();
void agregarConductorAlArchivo(Conductor conductor);
void listarTodosLosConductores(Conductor *conductores, int totalConductores);
void mostrarConductor(Conductor conductor);
void desactivarConductor(int conductorId);

//Infracciones

//Ayudines
void limpiarConsola();
/**** FIN DECLARACIONES DE FUNCIONES ****/

int main()
{
    int opcionMenu;
    int conductorId;
    int totalConductores = 0;
    struct Conductor conductores[100];

    cargarConductoresEnMemoria(conductores , totalConductores);
    mostrarMenu(opcionMenu);

    while (opcionMenu != 0)
    {
        switch (opcionMenu)
        {
        case 1:
            crearNuevoConductor();
            cargarConductoresEnMemoria(conductores ,  totalConductores);
            break;
        case 2:
            listarTodosLosConductores(conductores , totalConductores);
            break;
        case 3:
            listarTodosLosConductores(conductores , totalConductores);
            cout << "Ingrese un ID de conductor para desactivar: "  << endl;
            cin >> conductorId;
            desactivarConductor(conductorId);
            cargarConductoresEnMemoria(conductores , totalConductores);
            break;
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
            system("clear");
            cout << "Esta funcion todavia no se encuentra desarrollada" << endl;
            break;
        default:
            break;
        }

        mostrarMenu(opcionMenu);
    }

    return 0;
}



void mostrarConductor(Conductor conductor)
{
    cout << "-----------------------------------------------------------------" << endl;

    cout << "Informacion acerca del conductor con ID: " << conductor.conductorId << endl;

    cout << "Email: " << conductor.email << endl;

    cout << "Fecha de vencimiento: " << conductor.fechaVencimiento << endl;

    cout << "Activo: " << conductor.activo << endl;;

    cout << "Total infracciones: " << conductor.totalInfracciones << endl;

    cout << "-----------------------------------------------------------------" << endl;

}

void desactivarConductor(int conductorId)
{
    /*
    Para desactivar un conductor:
    1 - Debemos mostrar una lista de los conductores existentes
    2 - Debemos guardar el ingreso del conductorId 
    3 - Abrimos el archivo de conductores en modo rb+
    4 - Buscamos el conductor por ID
    5 - Actualizamos activo = false;
    6 - Cerramos el archivo
    */
    FILE *f;
    Conductor conductor;
    int posicionConductor = 0;

    f = fopen("conductores.bin" , "rb+");

    while( fread(&conductor , sizeof(Conductor) , 1 , f) == 1  ){
        if(conductor.conductorId = conductorId){
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
    hay que restarle a la posicion donde esta el struct guardado el tamaño del propio struct
    para que edite justo esa linea.
    */
    fseek( f , posicionConductor - sizeof(Conductor) , SEEK_SET );

    fwrite(&conductor , sizeof(Conductor) , 1 , f);

    fclose(f);
}

void listarTodosLosConductores(Conductor *conductores , int totalConductores)
{
    for(int i = 0; i < totalConductores ; i++)
    {
        mostrarConductor(conductores[i]);
    }
}

void crearNuevoConductor()
{
    Conductor conductor;

    cout << "Ingrese el ID del del conductor" << endl;
    cin >> conductor.conductorId;

    cout << "Ingrese fecha de vencimiento en formato AAAAMMDD " << endl;
    cin >> conductor.fechaVencimiento;

    cout << "Ingrese por favor un email del conductor" << endl;
    cin >> conductor.email;

    //Por default estas opciones despues lo podemos discutir si deben ser ingresadas por el usuario
    conductor.totalInfracciones = 0;
    conductor.activo = true;

    agregarConductorAlArchivo(conductor);
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


void cargarConductoresEnMemoria(Conductor *conductores , int &totalConductores)
{
    /*
    Para cargar todos los conductores:
    
    1 - Debemos abrir el archivo conductores
    2 - Vamos a cargar en memoria un array de conductores

    TODO: Primero se va a hacer con un array fijo de posiciones
    luego voy a utilizar listas SE(simplemente enlazada) para 
    no estar limitado por el tamaño del array
    */
    totalConductores = 0;

    FILE *f;

    f = fopen("conductores.bin", "rb");

    if(f == NULL)
    {
        cout << "No existe el archivo conductores.bin" << endl;
        return;
    }

    while (fread(&conductores[totalConductores], sizeof(Conductor), 1, f) == 1)
    {
        totalConductores++;
    }

    fclose(f);
}

void limpiarConsola()
{
    system("clear"); //Si estan usando windows creo que aca hay que poner "cls"
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
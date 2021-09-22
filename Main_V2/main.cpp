#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

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
void mostrar_informe();

//Infracciones
void cargar_infraccion_1(Conductor conductor);
void cargar_infraccion_2(Infraccion &infra,char fecha[],int infra_id,float monto,int provincia,int id_conductor);
void mostrar_infra(Infraccion infra);
void mostar_infra_de_conductor();
void mostrar_infractores_de_una_provincia();

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
            mostrar_infractores_de_una_provincia();
            break;
        case 5:
        case 6:
            mostrar_informe();
            break;
        case 7:
        case 8:
            system("cls");
            cout << "Esta funcion todavia no se encuentra desarrollada" << endl;
            break;
        default:
            break;
        }
        mostrarMenu(opcionMenu);
    }

    cout << "\nEl programa ha finalizado." << endl;
    return 0;
}

/** Subprogramas **/

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

    cout << "Ingrese cantidad de infracciones del conductor" << endl;
    cin >> conductor.totalInfracciones;
    //Por default estas opciones despues lo podemos discutir si deben ser ingresadas por el usuario

    conductor.activo = true;
    cargar_infraccion_1(conductor);
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
    system("cls"); //Si estan usando windows creo que aca hay que poner "cls"
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
    int prov_a_buscar;
    FILE *f;
    Infraccion infra;
    int id_anterior;
    cout << "Ingrese provincia de la que quiera ver sus infractores (1-24): " << endl;
    cin >> prov_a_buscar;
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

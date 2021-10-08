#include <iostream>
#include <cstring>

struct Infraccion
{
    int infraccionId;
    char fechaHora[13]; //AAAAMMDDHH:MM
    float monto;
    int conductorId;
    int provincia;
};

struct NodoInfraccion
{
    Infraccion infraccion;
    NodoInfraccion *next;
    NodoInfraccion *previous;
};

using namespace std;

void ingresar_ordenadamente_por_infraccionID(NodoInfraccion *&lista,Infraccion ticket);
void llenar_struct_infraccion(Infraccion &infra,char fecha[],int infra_id,float monto,int provincia,int id_conductor);
void mostrar_infra(Infraccion infra);
void cargar_infracciones_en_memoria(NodoInfraccion *&listainfracciones);
void mostrar_lista(NodoInfraccion *listainfracciones);

int main()
{
    NodoInfraccion *listainfracciones=NULL;
    Infraccion ticket;
    FILE *f;

    {
    f=fopen("procesados.bin","w");
    int i=1;
    llenar_struct_infraccion(ticket,"2021080118:04",10,i+101.25,i,i);
    fwrite(&ticket,sizeof(Infraccion),1,f);
    i++;

    llenar_struct_infraccion(ticket,"2021080118:10",11,i+105.25,i,i);
    fwrite(&ticket,sizeof(Infraccion),1,f);
    i++;

    llenar_struct_infraccion(ticket,"2021080118:15",4,i+11.25,i,i);
    fwrite(&ticket,sizeof(Infraccion),1,f);
    i++;

    llenar_struct_infraccion(ticket,"2021080118:31",15,i+13.25,i,i);
    fwrite(&ticket,sizeof(Infraccion),1,f);
    i++;

    llenar_struct_infraccion(ticket,"2021080118:25",8,i+16.25,i,i);
    fwrite(&ticket,sizeof(Infraccion),1,f);
    i++;

    llenar_struct_infraccion(ticket,"2021080118:51",1,i+132.25,i,i);
    fwrite(&ticket,sizeof(Infraccion),1,f);
    i++;

    llenar_struct_infraccion(ticket,"2021080118:26",3,i+15.25,i,i);
    fwrite(&ticket,sizeof(Infraccion),1,f);
    i++;

    llenar_struct_infraccion(ticket,"2021080118:00",6,i+101.25,i,i);
    fwrite(&ticket,sizeof(Infraccion),1,f);
    i++;

    llenar_struct_infraccion(ticket,"2021080118:59",12,i+101.25,i,i);
    fwrite(&ticket,sizeof(Infraccion),1,f);
    i++;

    llenar_struct_infraccion(ticket,"2021080118:59",21,i+50.25,i,i);
    fwrite(&ticket,sizeof(Infraccion),1,f);
    i++;

    fclose(f);
    }

    f=fopen("procesados.bin","r");
    // muestra el archivo sin ordenar
    while(fread(&ticket,sizeof(Infraccion),1,f))
        mostrar_infra(ticket);
    fseek(f,0,SEEK_SET);
    fclose(f);
    cout << "\n**** Infracciones en la lista *****\n" << endl;
    cargar_infracciones_en_memoria(listainfracciones);
    mostrar_lista(listainfracciones);
    return 0;
}

void mostrar_lista(NodoInfraccion *listainfracciones)
{
    while (listainfracciones && listainfracciones->next)
    {
        mostrar_infra(listainfracciones->infraccion);
        listainfracciones=listainfracciones->next;
    }
    if (listainfracciones)
        mostrar_infra(listainfracciones->infraccion);
    return;
}

void cargar_infracciones_en_memoria(NodoInfraccion *&listainfracciones)
{
    FILE *f;
    Infraccion infra;
    f=fopen("procesados.bin","r");
    while (fread(&infra,sizeof(Infraccion),1,f))
        ingresar_ordenadamente_por_infraccionID(listainfracciones,infra);
    fclose(f);
    return;
}

void llenar_struct_infraccion(Infraccion &infra,char fecha[],int infra_id,float monto,int provincia,int id_conductor)
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

void ingresar_ordenadamente_por_infraccionID(NodoInfraccion *&lista,Infraccion ticket)
{
    /**
    Cada vez que se crea una infraccion, se llama a este subprograma y se
    van agregando las infracciones nuevas en la lista de infracciones.
    **/
    NodoInfraccion *paux=lista;
    NodoInfraccion *paux2=NULL;
    if (lista==NULL)
    {   /// crear el primer elemento
        lista= new NodoInfraccion();
        lista->previous=NULL;
        lista->next=NULL;
        // contenido del struct
        strcpy(lista->infraccion.fechaHora,ticket.fechaHora);
        lista->infraccion.infraccionId = ticket.infraccionId;
        lista->infraccion.monto = ticket.monto;
        lista->infraccion.conductorId = ticket.conductorId;
        lista->infraccion.provincia = ticket.provincia;
    }
    else /// ya esta creada la lista
    {
        while (paux && paux->next && paux->next->infraccion.infraccionId < ticket.infraccionId)
            paux=paux->next;

        if (paux==lista && lista->infraccion.infraccionId > ticket.infraccionId)
        {   /// agrega un nodo al principio
            lista= new NodoInfraccion();
            lista->next = paux;
            paux->previous = lista;
            lista->previous = NULL;
            // contenido del struct
            strcpy(lista->infraccion.fechaHora,ticket.fechaHora);
            lista->infraccion.infraccionId = ticket.infraccionId;
            lista->infraccion.monto = ticket.monto;
            lista->infraccion.conductorId = ticket.conductorId;
            lista->infraccion.provincia = ticket.provincia;
        }
        if (paux->next==NULL)
        {   /// si agrega al final
            paux->next=new NodoInfraccion();
            paux->next->previous=paux;
            paux=paux->next;
            paux->next=NULL;
            // contenido del struct
            strcpy(paux->infraccion.fechaHora,ticket.fechaHora);
            paux->infraccion.infraccionId = ticket.infraccionId;
            paux->infraccion.monto = ticket.monto;
            paux->infraccion.conductorId = ticket.conductorId;
            paux->infraccion.provincia = ticket.provincia;
        }
        if (paux->infraccion.infraccionId < ticket.infraccionId)
        {   /// agrega en el medio de otros dos
            paux2=paux->next;
            paux->next= new NodoInfraccion();
            paux->next->previous= paux;
            paux=paux->next;
            paux2->previous=paux;
            paux->next= paux2;
            // contenido del struct
            strcpy(paux->infraccion.fechaHora,ticket.fechaHora);
            paux->infraccion.infraccionId = ticket.infraccionId;
            paux->infraccion.monto = ticket.monto;
            paux->infraccion.conductorId = ticket.conductorId;
            paux->infraccion.provincia = ticket.provincia;
        }
    }
    return;
}

#include <iostream>
#include <winsock2.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <list>
#include "Credencial.h"
#include <algorithm> // Necesario para std::transform
#include <thread>
#include <ctime>

using namespace std;
std::string horaActual() {
    // Obtiene la hora actual
    std::time_t tiempoActual = std::time(nullptr);

    // Convierte la hora actual en una estructura tm
    std::tm* tiempoInfo = std::localtime(&tiempoActual);

    // Formatea la hora actual como una cadena
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tiempoInfo);

    return std::string(buffer);
}
class Servidor{
public:
    WSADATA WSAData;
    SOCKET server,client;
    SOCKADDR_IN serverAddr,clientAddr;
    char buffer[1024];

    //string buffer;
    Servidor(){

    WSAStartup(MAKEWORD(2,0),&WSAData);
    server=socket(AF_INET,SOCK_STREAM,0);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_family=AF_INET;
    serverAddr.sin_port=htons(5005);
//: ryc@UNLa.2023

    bind(server,(SOCKADDR *)&serverAddr, sizeof(serverAddr));
    /*La función bind() utilizada asocia una dirección local con el socket.
        En este caso estamos especificando que el socket deberá recibir llamadas entrantes en cuyo port destino se especifice 5005.*/
    listen(server,0);
    cout <<"ESCUCHANDO CONEXIONES ENTRANTES."<<endl;
     ofstream archivo;//se crea el archivo de server.log con los datos de horaActual y el puerto
        archivo.open("server.log.txt"); //out para crear
        //-----------------------log archivo---------------------------------
        if(archivo.fail()){
            cout<<"No se pudo abrir el archivo";
            exit(1);
        }

        archivo<<horaActual() << ": =============================\n";
        archivo<<horaActual() << ": =======Inicia Servidor=======\n";
        archivo<<horaActual() << ": =============================\n";
        archivo<<horaActual() << ":====== Socket creado. Puerto de escucha: 5005=======" << "\n";

        int clientAddrSize = sizeof(clientAddr);
        if((client = accept(server, (SOCKADDR *)&clientAddr, &clientAddrSize)) != INVALID_SOCKET){

            cout << "\n----------El cliente se ha conectado!----------\n\n" << endl;
            archivo<<horaActual() << ": Conexion Aceptada\n";

        }else{
         cout << "\n----------El cliente no se ha conectado!----------\n\n" << endl;
            archivo<<horaActual() << ": Conexion Interrumpida\n";
             send(client,"conexion interupida,cliente desconectado",sizeof(buffer),0);
   memset(buffer,0,sizeof(buffer));
 closesocket(client);

        }

        archivo.close();
    }



    void Recibir(){
    recv(client,buffer,sizeof(buffer),0);
    char buf[sizeof(buffer)];
    strcpy(buf,buffer);
    cout<< "EL CLIENTE DICE : "<< buffer <<endl;
    memset(buffer,0,sizeof(buffer));

    }
    void Enviar(){
    cout<<"Escribe el mensaje a enviar";
    cin>>this->buffer;
    send(client,buffer,sizeof(buffer),0);
   memset(buffer,0,sizeof(buffer));
    //return buffer;
    cout<< "Mensaje enviado!"<<endl;
    }


    void CerrarSocket(){
    closesocket(client);
     send(client,"Socket cerrado,cliente desconectado.",sizeof(buffer),0);
   memset(buffer,0,sizeof(buffer));
    cout<<"Socket cerrado,cliente desconectado."<<endl;
    }
//[----------------------------------CREDENCIALE VALIDACIONES DE USUARIOS------------------------------------------------------]--
int Credenciales(Servidor* servidor) {
      ofstream archivo;
        archivo.open("server.log.txt",ios::app);
    list<Credencial> credencial = FuncionArchivoEnEstructura();
    bool valor = false;
    int num;
    char usuario[sizeof(buffer)]; // variable para guardar el usuario
    int intentos = 0; // Inicializamos el contador de intentos


        // Envio
        send(client, "Nombre Usuario?", sizeof(buffer), 0);
        memset(buffer, 0, sizeof(buffer));
        cout << "Mensaje enviado!" << endl;

        // Recibo
        int bytesRecibidos = recv(client, buffer, sizeof(buffer) - 1, 0);
        buffer[bytesRecibidos] = '\0';
        cout << "EL CLIENTE DICE : " << servidor->buffer << endl;
        strcpy(usuario, buffer);

        intentos = ObtenerNumeroUsuario(servidor->buffer);
       // cout << "ESTOS SON LOS INTENTOS: " << intentos << endl;

        num = verificarRol(usuario);
        valor = buscadorDeUsuario(credencial, buffer); // Busca dentro de la lista el usuario y devuelve un booleano
         if(num==1){
                string archi= "INICIO DE SESION:usuario CONSULTA:\n";
            archivo<<horaActual() << archi;
         }else{

          archivo<<horaActual() << "INICIO DE SESION:usuario Admin\n";
         }


        memset(servidor->buffer, 0, sizeof(servidor->buffer)); // Borra el buffer

        // Verificamos si el usuario existe y no est� bloqueado
        if (valor == true && intentos < 3) {
            bool valor2 = false;

            // Bucle para solicitar la contrase�a hasta que sea correcta o se alcancen los intentos m�ximos
            while (intentos < 3 && !valor2) {
                // Env�o
                send(client, "Usuario Encontrado!\nEscriba la Contrasenia:", sizeof(buffer), 0);

                //-----Recibo  REVISAR-------------------
int bytesRecibidosD = recv(client, buffer, sizeof(buffer) - 1, 0);
     buffer[bytesRecibidosD] ='\0';
       memset(buffer,0,sizeof(buffer));
     recv(client, buffer, sizeof(buffer) - 1, 0);
                cout << "EL CLIENTE DICE: " << buffer << endl;

                valor2 = verificarContrasenia(credencial, usuario, buffer);

                if (!valor2) {
                    intentos++;
                    CambiarNumeroUsuario(usuario, intentos);
                }
            }

            if (valor2) {
                return num; // Contrase�a correcta, retornamos el n�mero
            }
        } else {
            send(client, "Usuario no Encontrado o Bloqueado! Habla con algun administrador.", sizeof(buffer), 0);
            CerrarSocket();
            return -1; // Usuario no encontrado o bloqueado, retornamos un valor especial (por ejemplo, -1)
        }


    return -1; // Si se exceden los intentos, retornamos un valor especial
}



//[-----------------BUCADOR DE PALABRA PARA LA TRADUCCION--------------------------]
bool buscarPalabra(const std::string& palabra){
    //REUTILIZACION DE CODIGO EN FUNCION TRADUCTOR()
 std::ifstream file("traductor.txt");

    if (!file) {
        std::cerr << "Error al abrir el archivo de traductor." << std::endl;

    }

    std::string linea;
    bool encontrada = false;
    while (std::getline(file, linea)) {
        std::istringstream iss(linea);
        std::string palaEspa;
        std::string palaIngles;

        if (std::getline(iss, palaIngles, ':') && std::getline(iss, palaEspa)) {
            if (palaEspa == palabra||palabra==palaIngles) {
                encontrada = true;

            }
        }
    }
    return encontrada;
}


//[-----------------BUCADOR DE PALABRA PARA LA ALTA DE USUARIO--------------------------]
bool buscarUsuario(const std::string& palabra) {
    std::ifstream file("credenciales.txt");
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo de credenciales." << std::endl;
        return false; // O manejar el error de alguna otra manera
    }

    std::string linea;
    bool encontrada = false;
    while (std::getline(file, linea)) {
        std::istringstream iss(linea);

        std::string usuario;

        if (std::getline(iss, usuario, '|')) {
            // Eliminar espacios en blanco del principio y final de usuario
            usuario.erase(std::remove_if(usuario.begin(), usuario.end(), ::isspace), usuario.end());

            if (usuario == palabra) {
                encontrada = true;
                break; // Puedes salir del bucle tan pronto como encuentres el usuario
            }
        }
    }
    return encontrada;
}



 //-[----------------------------ALTA DE USUARIO----------------------------------]

   void AltaDeUsuario() {

        FILE *puntero;
        puntero = fopen ("credenciales.txt", "a");


        const char* mensaje;


        string pala;

        //[----------------------NOMBRE DE USUARIOOOO-----------------------]
        send(client,"COLOCA EL NOMBRE DEL USUARIO NUEVO:",sizeof(buffer),0);
 // recibo  REVISAR
int bytesRecibidosD = recv(client, buffer, sizeof(buffer) - 1, 0);
     buffer[bytesRecibidosD] ='\0';
   recv(client, buffer, sizeof(buffer) - 1, 0);

if (strcmp(buffer, "") == 0){

       mensaje= "\nError: Las palabras deben tener contenido.";
        send(client, mensaje, strlen(mensaje), 0);
        return;
    }
       char nombre[sizeof(buffer)];
//printf("NOMBRE USUARIO ALTA RECIBIDO: %s",buffer);
       strcpy(nombre,buffer);
    // memset(buffer,0,sizeof(buffer));
     pala=ConvertirAMinusculas(buffer);//transformo el buffer a string y valido que no este escrito
      //VERIFICA SI ESTA LA PALABRA YA ESTA ESCRITA EN EL TRADUCTOR
    // Verificar que las palabras no estén vacías

    if (buscarUsuario(pala)==true) {
            //creo un string para convertir en char
       string mensajeAux = "\nError al dar de alta el nuevo usuario: " + pala + " YA EXISTE";
mensaje = mensajeAux.c_str();
send(client, mensaje, strlen(mensaje), 0);
       return;
    }else{
 //[---------------------------------CONTASE�A DE USUARIO--------------------------------------------------]

 send(client,"\nNOMBRE DE USUARIO GUARDADO!\n COLOCA LA CONTRASENIA DEL USUARIO NUEVO:",sizeof(buffer),0);
 // recibo  REVISAR
int bytesRecibidosD = recv(client, buffer, sizeof(buffer) - 1, 0);
     buffer[bytesRecibidosD] ='\0';
     recv(client, buffer, sizeof(buffer) - 1, 0);

std::string bufferString = buffer;
      //string datoRecibido(buffer, bytesRecibidosD);
      // string contrasena = datoRecibido;

       if(bufferString.length()==1){

            cout << "NO se puede ingrear un usuario con la contrasena vacia" << endl;
             recv(client, buffer, sizeof(buffer) - 1, 0);
            send(client,"NO se puede ingrear un usuario con la contrasena vacia",sizeof(buffer),0);
            //closesocket(client);
       }

if(bufferString.length()!=1){
            //escribe
            mensaje= "\nMENSAJE AGREGADO CORRECTAMEN AL ARCHIVO DE CREDENCIALES!!\n";
        send(client, mensaje, strlen(mensaje), 0);
           // Agrega un car�cter de nueva l�nea al final del arreglo aux
        fprintf(puntero,"\n");
        //---------STRING A COLOCAR-------
        string usuarioTerminado = string(nombre) + "|" + string(buffer) + "|CONSULTA|0";
memset(buffer,0,sizeof(buffer));

     // Convertir el std::string a un arreglo de caracteres (const char*)
    const char* auxiliar = usuarioTerminado.c_str();


            fprintf(puntero, auxiliar);
}
    }

            fclose(puntero);
    }

    //[----------------------------------------------TRADUCCION-----------------------------------------------------------]
void Traductor(const std::string& palabra) {
    std::ifstream file("traductor.txt");

    if (!file) {
        std::cerr << "Error al abrir el archivo de traductor." << std::endl;
        return;
    }

    std::string linea;
    bool encontrada = false;
    while (std::getline(file, linea)) {
        std::istringstream iss(linea);
        std::string palaEspa;
        std::string palaIngles;

        if (std::getline(iss, palaIngles, ':') && std::getline(iss, palaEspa)) {
            if (palaEspa == palabra||palabra==palaIngles) {
                encontrada = true;
               string mensaje="\nPalabra recibida en ingles: "+palaIngles+"\nPalabra traducida al espaniol: "+palaEspa;
                send(client, mensaje.c_str(), mensaje.size(), 0);
            }
        }
    }

    if (!encontrada) {
        std::cout << "No fue posible encontrar la traduccion para: " << palabra << std::endl;
    }

}


void TraductorCliente(){

 // ---- Interaccion ----
    const char* mensaje = "Escribe el mensaje a enviar a traducir\n";
    send(client, mensaje, strlen(mensaje), 0);

    // --------- Recepcion -----------
   // char buffer[1024];
    int bytesRecibidos = recv(client, buffer, sizeof(buffer) - 1, 0);
     memset(buffer,0,sizeof(buffer));
     recv(client, buffer, sizeof(buffer) - 1, 0);

    if (bytesRecibidos == -1) {
        std::cerr << "Error al recibir el mensaje del cliente." << std::endl;
        return;
    }

    buffer[bytesRecibidos] = '\0'; // A�ade el car�cter nulo al final del mensaje recibido

  //  std::cout << "Palabra a Traducir: " << buffer << std::endl;
   // std::cout << "Traduzco entonces..." << std::endl;

//  ----------  Traductor(buffer);
      Traductor(ConvertirAMinusculas(buffer));//se convierte el texto antes de buscar
}


std::string ConvertirAMinusculas(const std::string& texto) {
    std::string resultado = texto;
    std::transform(resultado.begin(), resultado.end(), resultado.begin(), ::tolower);
    return resultado;
}




void VerRegistroActividades() {
    std::ifstream registroFile("server.log.txt");

    if (!registroFile.is_open()) {
        send(client, "No se pudo abrir el archivo de registro de actividades.", sizeof(buffer), 0);
        return;
    }

    std::string registroContent((std::istreambuf_iterator<char>(registroFile)),
                                 std::istreambuf_iterator<char>());

    registroFile.close();

    if (!registroContent.empty()) {
        send(client, registroContent.c_str(), registroContent.size(), 0);
    } else {
        send(client, "El registro de actividades está vacío.", sizeof(buffer), 0);
    }
}


  //--------------------------INSERTAR TRADUCCIONNN--------------------------------------

   void InsertarTraduccion() {
    FILE* puntero;
    puntero = fopen("traductor.txt", "a");


   const char* mensaje = "Escribe la nueva traduccion, separado las palabras con un ( : )\n";
   send(client, mensaje, strlen(mensaje),0);
  //printf("PALABRA QUE RECIBO::::::::%s",aux2);
    int bytesRecibidos = recv(client, buffer, sizeof(buffer) - 1, 0);

    buffer[bytesRecibidos] = '\0';
   printf("PALABRA QUE RECIBO::::::::%s",buffer);
     recv(client, buffer, sizeof(buffer) - 1, 0);


    string pala = buffer;


       // Verificar que las palabras no estén vacías
    if (pala.empty() ) {
       mensaje= "\nError: Las palabras deben tener contenido.";
        send(client, mensaje, strlen(mensaje), 0);
        return;
    }
    //VERIFICA SI ESTA LA PALABRA YA ESTA ESCRITA EN EL TRADUCTOR
    if (buscarPalabra(pala)==true) {
            //creo un string para convertir en char
       string mensajeAux="\nYa existe una traducción para LA PALABRA:"+ pala;
    mensaje=mensajeAux.c_str();
        send(client, mensaje, strlen(mensaje), 0);
        return;
    }

    // Verificar que las palabras no contengan ':' para cumplir con el formato
    if (pala.find(':')==std::string::npos ) {

        mensaje= "\nError: Las palabras deben tener el caracter ':'.\n";
        send(client, mensaje, strlen(mensaje), 0);
        return;
    }else{

        if (bytesRecibidos <= 0) {
            std::cout << "ERROR" << std::endl; //me da este error revisar!
            } else {
            buffer[bytesRecibidos] = '\0'; // Añadir terminador nulo para convertirlo en una cadena de C válida
            // Ahora 'aux' contiene los datos recibidos desde el socket
            }

            std::cout << buffer << std::endl;

            //escribe
            mensaje= "\nMENSAJE AGREGADO CORRECTAMEN AL ARCHIVO DE TRADUCCIONES!!\n";
        send(client, mensaje, strlen(mensaje), 0);
           // Agrega un carácter de nueva línea al final del arreglo aux
        fprintf(puntero,"\n");
       string auxi=ConvertirAMinusculas(buffer);
     // Convertir el std::string a un arreglo de caracteres (const char*)
    const char* auxiChar = auxi.c_str();


            fprintf(puntero, auxiChar);
    }
            fclose(puntero);


    }





   void ListadoBloqueados(){
const char* aux;
       bool p=false;
string men;
   list<Credencial> credenciales = FuncionArchivoEnEstructura();

   for(Credencial crede: credenciales){
    if(crede.bloqueos==3){


            men=crede.usuario.c_str()+men;
            aux=men.c_str();
            printf("\nlistado: %s",aux);


       p=true;
    }


   }
   if(p==false){
     send(client,"\nNO HAY USUARIOS BLOQUEADOS :(\n" , sizeof(buffer), 0);
   }
   send(client, men.c_str(), sizeof(buffer), 0);
   }
//-----------------DESBLOQUEO DE USUARIOS-------------------------------------
void DesbloqueoUsuarios(){
     ListadoBloqueados();
     //char aux[40];
 const char* mensaje = "ESCRIBA EL NOMBRE DEL USUARIO A DESBLOQUEAR";
  send(client, mensaje, strlen(mensaje), 0);

    int bytesRecibidos = recv(client, buffer, sizeof(buffer) - 1, 0);
    buffer[bytesRecibidos] = '\0'; // A�ade el car�cter nulo al final de la cadena recibida
      // memset(aux,0,sizeof(aux));
     recv(client, buffer, sizeof(buffer) - 1, 0);
     printf("usuario a desbloquear: %s",buffer);
     CambiarNumeroUsuario(buffer,0);
     mensaje="USUARIO DESBLOQUEADO CON EXITO";
     send(client, mensaje, strlen(mensaje), 0);
CerrarSocket();
}



void MenuUsuarios(Servidor* servidorr){
     char aux[10];
 const char* mensaje = "\nMENU USUARIOS!!!\n\n-----------MENU------------\n\nOPCION 0: DAR DE ALTA USUARIOS\n\nOPCION 1: DESBLOQUEAR USUARIOS,\nLISTADO:\nOPCION 2:SALIR\nCOLOQUE UNA OPCION ACONTINUACION:";
    send(client, mensaje, strlen(mensaje), 0);
    int bytesRecibidos = recv(client, buffer, sizeof(buffer) - 1, 0);
    buffer[bytesRecibidos] = '\0';
      // memset(aux,0,sizeof(aux));
     recv(client, aux, sizeof(aux) - 1, 0);
                cout << "EL CLIENTE DICE: " << buffer << endl;
    int opcion;

    try {
        opcion = std::stoi(aux);
    } catch (const std::invalid_argument& e) {
        // Maneja el error aqui si la conversi�n falla
        opcion = -1; // Otra forma de indicar un error
    }

  switch(opcion){
            case 0:
               // cout << "Alta: \n";
               servidorr->AltaDeUsuario();
                break;
            case 1:
               // cout << "Desbloqueo: \n";
                servidorr->DesbloqueoUsuarios();

                break;
            case 2:
                //cout << "escriba /salir para ir al menu anterior: ";
                send(client, "escriba (salir) para ir al menu anterior: ", strlen(mensaje), 0);
              int bytesRecibidos = recv(client, buffer, sizeof(buffer) - 1, 0);
    buffer[bytesRecibidos] = '\0'; // A�ade el car�cter nulo al final de la cadena recibida
       //memset(aux,0,sizeof(aux));
     recv(client, buffer, sizeof(buffer) - 1, 0);
printf("BUFER:%s",buffer);
                if(strcmp(buffer, "salir") == 0){
                    servidorr->MenuAdmin(servidorr);

                }
                break;

            }
}

void MenuAdmin(Servidor*servidorr){
    char aux[10];
 const char* mensaje = "\nBIENVENIDO ADMINISTRADO,QUE DESEA HACER?!!!\n\n-----------MENU------------\n\nOPCION 0: NUEVA TRADUCCION\n\nOPCION 1: ENTRAR AL MENU USUARIOS\n OPCION 2:VER REGISTRO DE ACTIVIDADES\n OPCION 3: CERRAR SESION\n\n------COLOQUE UNA OPCION ACONTINUACION:";
     send(client, mensaje, strlen(mensaje), 0);
    int bytesRecibidos = recv(client, aux, sizeof(aux) - 1, 0);
    aux[bytesRecibidos] = '\0'; // A�ade el car�cter nulo al final de la cadena recibida
       memset(aux,0,sizeof(aux));
     recv(client, aux, sizeof(aux) - 1, 0);
                cout << "EL CLIENTE DICE OPCION: " << aux << endl;
    int opcion;

    try {
        opcion = std::stoi(aux);
    } catch (const std::invalid_argument& e) {
        // Maneja el error aqu� si la conversi�n falla
        opcion = -1; // Otra forma de indicar un error
    }

  switch(opcion){
            case 0:
               // cout << "NUEVA TRADUCCION: \n";
              servidorr->InsertarTraduccion();
                break;
            case 1:
               // cout << "MENU USUARIOS \n";
              servidorr->MenuUsuarios(servidorr);
                break;
            case 2://REGISTRO ACTIVIDADES
                servidorr->VerRegistroActividades();
                break;
            case 3://CERRAR SESION
                CerrarSocket();
                break;

}
}
void MenuCliente(Servidor* servidorr) {
    char aux[10];
    const char* mensaje = "\nBIENVENIDO AL TRADUCTOR!!!\n\n-----------MENU------------\n\nOPCION 0: TRADUCTOR\n\nOPCION 1: CERRAR SESION\nCOLOQUE UNA OPCION ACONTINUACION:";
    send(client, mensaje, strlen(mensaje), 0);
    int bytesRecibidos = recv(client, aux, sizeof(aux) - 1, 0);
    aux[bytesRecibidos] = '\0'; // Añade el car�cter nulo al final de la cadena recibida
       memset(aux,0,sizeof(aux));
     recv(client, aux, sizeof(aux) - 1, 0);
                cout << "EL CLIENTE DICE: " << buffer << endl;
    int opcion;

    try {
        opcion = std::stoi(aux);
    } catch (const std::invalid_argument& e) {
        // Maneja el error aqui si la conversion falla
        opcion = -1; // Otra forma de indicar un error
    }

    switch (opcion) {
        case 0:
            servidorr->TraductorCliente();
            break;
        case 1:
            servidorr->CerrarSocket();
            break;
        default:
                 send(client," No es una opcion valida.\n", strlen(mensaje), 0);
            //printf("No es una opci�n v�lida.\n");
            break;
    }
}

};


    int main(){

 Servidor *servidorr =new Servidor();
  // Inicia un hilo para aceptar conexiones entrantes

 string usuario;
 int num;
 num=servidorr->Credenciales(servidorr);
int opcion;
//num=1;
//printf("EL boleano que me retorna credenciales : %d",num);

if(num==1){
   servidorr->MenuCliente(servidorr);

}
if(num==2){
    servidorr->MenuAdmin(servidorr);
}
servidorr->CerrarSocket();

return 0;
}

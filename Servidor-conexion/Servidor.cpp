#include <iostream>
#include <winsock2.h>
#include <vector>
#include <fstream>   // Agrega esta línea
#include <sstream>
#include <list>
#include "Credencial.h"
#include <algorithm> // Necesario para std::transform
//BUENA PRACTICA FUNCIONES EN MINUSCULA

using namespace std;

class Servidor{
public:
    WSADATA WSAData;
    SOCKET server,client;
    SOCKADDR_IN serverAddr,clientAddr;
    char buffer[1024];
    char buffer2[1024];
    //string buffer;
    Servidor(){

    WSAStartup(MAKEWORD(2,0),&WSAData);
    server=socket(AF_INET,SOCK_STREAM,0);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_family=AF_INET;
    serverAddr.sin_port=htons(5555);

    bind(server,(SOCKADDR *)&serverAddr, sizeof(serverAddr));
    listen(server,0);
    cout <<"ESCUCHANDO CONEXIONES ENTRANTES."<<endl;
    int clientAddrSize=sizeof(clientAddr);
    if((client=accept(server,(SOCKADDR *)&clientAddr,&clientAddrSize)) != INVALID_SOCKET){
        cout<< "Cliente conectado!" << endl;
    }
    }
/*


    void Recibir(){
    recv(client,buffer,sizeof(buffer),0);
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

*/
    void CerrarSocket(){
    closesocket(client);
    cout<<"Socket cerrado,cliente desconectado."<<endl;
    }

  void Credenciales(Servidor* servidor) {
     list<Credencial> credencial=FuncionArchivoEnEstructura();
      bool valor=false;
  bool valor2=false;
    int intentos=0;
    while(valor!=true){
            //envio
   send(client,"Nombre Usuario?",sizeof(buffer),0);
    memset(buffer,0,sizeof(buffer));
 cout << "Mensaje enviado!" << endl;
    // -----------recibo-----------
     int bytesRecibidos = recv(client, buffer, sizeof(buffer) - 1, 0);
     buffer[bytesRecibidos] = '\0';
     cout << "EL CLIENTE DICE : " << servidor->buffer << endl;
     char usuario[sizeof(buffer)];// variable para guardar el usuario
     strcpy(usuario,buffer);//copio el usuario del buffer a la variable y poder buscarla en funciones
 valor=buscadorDeUsuario(credencial,buffer);//busca dentro de la lista string  al usuario devuelve booleano

   memset(servidor->buffer, 0, sizeof(servidor->buffer)); // borra el buffer
 //--------contraseña--------------------
    if(valor==true){//verifica y avisa

//mensaje
 while(valor2==false){
    send(client,"Usuario Encontrado!\n Escriba la Contrasenia:",sizeof(buffer),0);
 // recibo  REVISAR
int bytesRecibidosD = recv(client, buffer, sizeof(buffer) - 1, 0);
     buffer[bytesRecibidosD] ='\0';
       memset(buffer,0,sizeof(buffer));
     recv(client, buffer, sizeof(buffer) - 1, 0);

      cout << "EL CLIENTE DICE : " << buffer << endl;
      valor2= verificarContrasenia(credencial,usuario,servidor->buffer);
    }
      printf("QUE BUSCO USUARIO: %s",usuario);
       printf("QUE BUSCO BUFFER: %s",buffer);
      printf("VALOR del booleano contra : %d",valor2);
   memset(servidor->buffer, 0, sizeof(servidor->buffer)); // borra el buffer

 }else{
        send(client,"Usuario  no Encontrado! :(,vuelve a intentar",sizeof(buffer),0);

 }

}
    }
//-----------------BUCADOR DE PALABRA PARA LA TRADUCCION--------------------------
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


//-----------------BUCADOR DE PALABRA PARA LA ALTA DE USUARIO--------------------------
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



 //-----------------------------ALTA DE USUARIO----------------------------------

   void AltaDeUsuario() {

        FILE *puntero;
        puntero = fopen ("credenciales.txt", "a");


        const char* mensaje;


        string pala;

        //----------------------NOMBRE DE USUARIOOOO-----------------------
        send(client,"COLOCA EL NOMBRE DEL USUARIO NUEVO:",sizeof(buffer),0);
 // recibo  REVISAR
int bytesRecibidosD = recv(client, buffer, sizeof(buffer) - 1, 0);
     buffer[bytesRecibidosD] ='\0';
       char nombre[sizeof(buffer)];

       strcpy(nombre,buffer);
    // memset(buffer,0,sizeof(buffer));
     pala=ConvertirAMinusculas(buffer);//transformo el buffer a string y valido que no este escrito
      //VERIFICA SI ESTA LA PALABRA YA ESTA ESCRITA EN EL TRADUCTOR
      printf("BUCAR USUARIO RESULTADO: %d",buscarUsuario(pala));
      printf("que hay guardado en pala: %s",pala);
       printf("que hay guardado en buffer: %s",buffer);
    if (buscarUsuario(buffer)==true) {
            //creo un string para convertir en char
       string mensajeAux = "\nError al dar de alta el nuevo usuario: " + pala + " YA EXISTE";
mensaje = mensajeAux.c_str();
send(client, mensaje, strlen(mensaje), 0);
       return;
    }else{
 //----------------------------------CONTASEÑA DE USUARIO--------------------------------------------------

 send(client,"\nNOMBRE DE USUARIO GUARDADO!\n COLOCA LA CONTRASENIA DEL USUARIO NUEVO:",sizeof(buffer),0);
 // recibo  REVISAR
int bytesRecibidosD = recv(client, buffer, sizeof(buffer) - 1, 0);
     buffer[bytesRecibidosD] ='\0';
      recv(client, buffer, sizeof(buffer) - 1, 0);
     printf("CONTRASENIA QUE RECIBO: %s",buffer);
     //  char contra[sizeof(buffer)];
    //   strcpy(contra,buffer);
    // memset(buffer,0,sizeof(buffer));

            //escribe
            mensaje= "\nMENSAJE AGREGADO CORRECTAMEN AL ARCHIVO DE CREDENCIALES!!\n";
        send(client, mensaje, strlen(mensaje), 0);
           // Agrega un carácter de nueva línea al final del arreglo aux
        fprintf(puntero,"\n");
        //---------STRING A COLOCAR-------
        string usuarioTerminado = string(nombre) + "|" + string(buffer) + "|CONSULTA|0";
memset(buffer,0,sizeof(buffer));

     // Convertir el std::string a un arreglo de caracteres (const char*)
    const char* auxiliar = usuarioTerminado.c_str();


            fprintf(puntero, auxiliar);
    }

            fclose(puntero);
    }

    //----------------------------------------------TRADUCCION-----------------------------------------------------------
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
        std::cout << "No fue posible encontrar la traducción para: " << palabra << std::endl;
    }

}


void TraductorCliente(){

 // ---- Interacción ----
    const char* mensaje = "Escribe el mensaje a enviar a traducir\n";
    send(client, mensaje, strlen(mensaje), 0);

    // --------- Recepción -----------
   // char buffer[1024];
    int bytesRecibidos = recv(client, buffer, sizeof(buffer) - 1, 0);

    if (bytesRecibidos == -1) {
        std::cerr << "Error al recibir el mensaje del cliente." << std::endl;
        return;
    }

    buffer[bytesRecibidos] = '\0'; // Añade el carácter nulo al final del mensaje recibido

    std::cout << "Palabra a Traducir: " << buffer << std::endl;
    std::cout << "Traduzco entonces..." << std::endl;

//    Traductor(buffer);
      Traductor(ConvertirAMinusculas(buffer));//se convierte el texto antes de buscar
}





std::string ConvertirAMinusculas(const std::string& texto) {
    std::string resultado = texto;
    std::transform(resultado.begin(), resultado.end(), resultado.begin(), ::tolower);
    return resultado;
}





  //INSERTAR TRADUCCIONNN

    void InsertarTraduccion() {

        FILE *puntero;
        puntero = fopen ("traductor.txt", "a");

        string palabraTraduccion;

        char aux[50];
        string pala;
        const char* mensaje = "Escribe la nueva traduccion, separado las palabras con un ( : )\n";
    send(client, mensaje, strlen(mensaje), 0);
        int bytesRecibidos = recv(client, aux, sizeof(aux), 0);
pala=aux;

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
            aux[bytesRecibidos] = '\0'; // Añadir terminador nulo para convertirlo en una cadena de C válida
            // Ahora 'aux' contiene los datos recibidos desde el socket
            }

            std::cout << aux << std::endl;

            //escribe
            mensaje= "\nMENSAJE AGREGADO CORRECTAMEN AL ARCHIVO DE TRADUCCIONES!!\n";
        send(client, mensaje, strlen(mensaje), 0);
           // Agrega un carácter de nueva línea al final del arreglo aux
        fprintf(puntero,"\n");
       string auxi=ConvertirAMinusculas(aux);
     // Convertir el std::string a un arreglo de caracteres (const char*)
    const char* auxiChar = auxi.c_str();


            fprintf(puntero, auxiChar);
    }
            fclose(puntero);


    }

    };

void MenuCliente(){
printf("\n-----------MENU------------\n");
printf("\nOPCION 0: TRADUCIR PALABRA\n");
printf("\nOPCION 1: AGREGAR PALABAR\n");
printf("\n-----------------------\n");
printf("\n-----------------------\n");
printf("\n-----------------------\n");
printf("\n-----------------------\n");
printf("\n-----------------------\n");


}








    int main(){

 Servidor *Servidorr =new Servidor();
//Servidorr->InsertarTraduccion();

//printf(" BUSCAR USUARIO LO QUE DEVUELVE: %d",Servidorr->buscarUsuario("persona1"));
Servidorr->AltaDeUsuario();
/*
list<Credencial> credencial = FuncionArchivoEnEstructura();
Servidorr->Credenciales(Servidorr);
*/
 //Servidorr->TraductorCliente();
    // Cargar las credenciales desde el archivo


    // Imprimir las credenciales cargadas (solo como ejemplo)

    //return 0;
    }


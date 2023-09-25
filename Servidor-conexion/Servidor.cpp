#include <iostream>
#include <winsock2.h>
#include <vector>
#include <fstream>   // Agrega esta línea
#include <sstream>
#include <list>
#include "Credencial.h"

using namespace std;

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
    serverAddr.sin_port=htons(5555);

    bind(server,(SOCKADDR *)&serverAddr, sizeof(serverAddr));
    listen(server,0);
    cout <<"ESCUCHANDO CONEXIONES ENTRANTES."<<endl;
    int clientAddrSize=sizeof(clientAddr);
    if((client=accept(server,(SOCKADDR *)&clientAddr,&clientAddrSize)) != INVALID_SOCKET){
        cout<< "Cliente conectado!" << endl;
    }

    }

    void Recibir(){
    recv(client,buffer,sizeof(buffer),0);
    cout<< "EL CLIENTE DICE : "<< buffer <<endl;
    memset(buffer,0,sizeof(buffer));

    }
    void Enviar(){
    cout<<"Escribe el mensaje a enviar ";
    cin>>this->buffer;
    send(client,buffer,sizeof(buffer),0);
    memset(buffer,0,sizeof(buffer));
    cout<< "Mensaje enviado!"<<endl;
    }


    void CerrarSocket(){
    closesocket(client);
    cout<<"Socket cerrado,cliente desconectado."<<endl;
    }

  void Credenciales(Servidor* servidor) {
     list<Credencial> credencial=FuncionArchivoEnEstructura();

    // ------------USUARIO-----------------
    // envío
    //cout << "Nombre Usuario?";
    //cin>> this->buffer;
    bool valor=false;
    bool valor2=false;
    int intentos=0;
    while(valor!=true){
   send(client,"Nombre Usuario?",sizeof(buffer),0);
    memset(buffer,0,sizeof(buffer));
 cout << "Mensaje enviado!" << endl;
    // -----------recibo-----------
    recv(servidor->client, servidor->buffer, sizeof(servidor->buffer), 0);
    cout << "EL CLIENTE DICE : " << servidor->buffer << endl;
    valor=buscadorDeUsuario(credencial,servidor->buffer);//busca dentro de la lista string parecido al usuario

    memset(servidor->buffer, 0, sizeof(servidor->buffer)); // borra el buffer

    if(valor==true){//verifica y avisa
    send(client,"Usuario Encontrado!",sizeof(buffer),0);
    }else{
     send(client,"Usuario  no Encontrado! :( ",sizeof(buffer),0);
    }
    }

    // ---------contraseña--------------------
    //agregar condicion de intentos &&
    /*
    if (valor == true) {
            while(valor2!=true ){
        cout << "CUAL ES LA CONTRASEÑA?";
        cin >> buscar;
        send(servidor->client, buscar.c_str(), buscar.length(), 0);
        memset(servidor->buffer, 0, sizeof(servidor->buffer));
        cout << "Mensaje enviado!" << endl;
        // -----------recibo-----------
        recv(servidor->client, servidor->buffer, sizeof(servidor->buffer), 0);
        cout << "EL CLIENTE DICE : " << servidor->buffer << endl;
      valor2= verificarContrasenia(credencial,servidor->buffer);
        memset(servidor->buffer, 0, sizeof(servidor->buffer)); // borra el buffer
    }

}*/
  }





    };

    int main(){

   Servidor *Servidorr =new Servidor();

/*
    // Cargar las credenciales desde el archivo


    // Imprimir las credenciales cargadas (solo como ejemplo)
list<Credencial> credencial = FuncionArchivoEnEstructura();

    for (Credencial credencial:credencial) {
        cout << "Usuario: " << credencial.usuario << endl;
        cout << "Contrasenia: " << credencial.contra << endl;
        cout << "Rol: " << credencial.rol << endl;
        cout << "Bloqueos: " << credencial.bloqueos << endl;
        cout << "-----------------------" << endl;
    }/**/
    Servidorr->Credenciales(Servidorr);
        Servidorr->Recibir();
        Servidorr->Enviar();

    return 0;
    }

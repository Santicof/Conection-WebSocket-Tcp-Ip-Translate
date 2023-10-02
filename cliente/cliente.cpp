#include <iostream>
#include <winsock2.h>

using namespace std;

class Cliente {
public:
    WSADATA WSAData;
    SOCKET server;
    SOCKADDR_IN addr;
    char buffer[1024];

    Cliente() {
        cout << "Conectando al servidor..." << endl << endl;
        WSAStartup(MAKEWORD(2, 0), &WSAData);
        server = socket(AF_INET, SOCK_STREAM, 0);
        addr.sin_addr.s_addr = inet_addr("192.168.0.174"); // Asegúrate de que esta dirección IP sea correcta
        addr.sin_family = AF_INET;
        addr.sin_port = htons(5005); // Asegúrate de que este puerto sea correcto

        if (connect(server, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
            cerr << "No se pudo conectar al servidor." << endl;
            closesocket(server);
            WSACleanup();
            exit(1);
        }
        cout << "Conectado al servidor!" << endl;
    }

    void Enviar() {
/*
        cout << "Escribe el mensaje a enviar:(termina con una linea vacia)\n ";
        string mensaje;
        int num=0;
        while(true){
            string linea;
            getline(cin,linea);
            if(num==0){//para que obtenga lo primero como mensaje y no el espacio vacio
                mensaje = linea;
                num=1;
            }

            if(linea==""){
                break;
            }

        }
*/
       // strcpy_s(buffer,mensaje.c_str());
       cout << "Escribe :";
       cin>> this->buffer;
        send(server, buffer, sizeof(buffer), 0);
        memset(buffer, 0, sizeof(buffer));
        cout << "Mensaje enviado!" << endl;
    }


    void Recibir() {
        recv(server, buffer, sizeof(buffer), 0);
        cout << "El servidor dice: " << buffer << endl;
        memset(buffer, 0, sizeof(buffer));
    }

    void CerrarSocket() {
        closesocket(server);
        WSACleanup();
        cout << "Socket cerrado" << endl << endl;
    }
};

int main() {

    Cliente* Clientee = new Cliente();
    while(true){
Clientee->Recibir();
Clientee->Enviar();


    }


}

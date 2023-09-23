#include <iostream>
#include <winsock2.h>
#include <vector>
#include <fstream>   // Agrega esta línea
#include <sstream>
using namespace std;
// Estructura para almacenar credenciales
struct Credencial {
    string usuario;
    string contrasenia;
    string rol;
    int intentos;
};
class Servidor{
public:
    WSADATA WSAData;
    SOCKET server,client;
    SOCKADDR_IN serverAddr,clientAddr;
    char buffer[1024];
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
    string buscar;
    // ------------USUARIO-----------------
    // envío
    //cout << "Nombre Usuario?";
    //cin>> this->buffer;
    bool valor = false;
   send(client,"Nombre Usuario?",sizeof(buffer),0);
    memset(buffer,0,sizeof(buffer));

    cout << "Mensaje enviado!" << endl;
    // -----------recibo-----------
    recv(servidor->client, servidor->buffer, sizeof(servidor->buffer), 0);
    cout << "EL CLIENTE DICE : " << servidor->buffer << endl;
    valor = BusquedaDentroDeFile(servidor->buffer); // busca el buffer e imprime si lo encontró
    memset(servidor->buffer, 0, sizeof(servidor->buffer)); // borra el buffer
    if(valor==true){
    send(client,"Usuario Encontrado!",sizeof(buffer),0);
    }else{
     send(client,"Usuario  no Encontrado! :( ",sizeof(buffer),0);
    }
    memset(servidor->buffer, 0, sizeof(servidor->buffer)); // borra el buffer
    // ---------contraseña--------------------
    if (valor == true) {
        cout << "CUAL ES LA CONTRASEÑA?";
        cin >> buscar;
        send(servidor->client, buscar.c_str(), buscar.length(), 0);
        memset(servidor->buffer, 0, sizeof(servidor->buffer));
        cout << "Mensaje enviado!" << endl;
        // -----------recibo-----------
        recv(servidor->client, servidor->buffer, sizeof(servidor->buffer), 0);
        cout << "EL CLIENTE DICE : " << servidor->buffer << endl;
        BusquedaDentroDeFile(servidor->buffer); // busca el buffer e imprime si lo encontró
        memset(servidor->buffer, 0, sizeof(servidor->buffer)); // borra el buffer
    }

}





//Función para cargar credenciales desde un archivo
vector <Credencial> CargarCredencialesDesdeArchivo(const string& archivo) {
    vector<Credencial> credenciales;
    ifstream file(archivo);

    if (!file) {
        cerr << "Error al abrir el archivo de credenciales." << endl;
        return credenciales;
    }


    string linea;
    while (getline(file, linea)) {
        istringstream iss(linea);
        string usuario, contrasenia, rol, intentosStr;

        if (getline(iss, usuario, '|') &&
            getline(iss, contrasenia, '|') &&
            getline(iss, rol, '|') &&
            getline(iss, intentosStr, '|')) {
            Credencial credencial;
            credencial.usuario = usuario;
            credencial.contrasenia = contrasenia;
            credencial.rol = rol;
            credencial.intentos = stoi(intentosStr);
            credenciales.push_back(credencial);
        } else {
            cerr << "Error al analizar la línea: " << linea << endl;
        }
    }

    file.close();
    return credenciales;
}
  bool BusquedaDentroDeFile(string buscar) {
        FILE* puntero;
        puntero = fopen("credenciales.txt", "r");
        int encontre = 0;
        bool valor=false;
        char linea[1024]; // Cambié "string linea" a "char linea[1024]" para usar fgets
        // Bucle
        while (fgets(linea, sizeof(linea), puntero) != NULL && encontre == 0) {
            if (strstr(linea, buscar.c_str()) != NULL) {
                encontre = 1;
            }
        }
        fclose(puntero); // Cierra el archivo aquí
        if (encontre == 1) {
           // printf("Usuario encontrado!");
            valor=true;
        }
         return valor;
    }

    };

    int main(){
    Servidor *Servidorr =new Servidor();
    //while(true){
               // Nombre del archivo de credenciales
    string archivoCredenciales = "credenciales.txt";

    // Cargar las credenciales desde el archivo
   // vector<Credencial> credenciales = CargarCredencialesDesdeArchivo(archivoCredenciales);
    vector<Credencial> credenciales = Servidorr->CargarCredencialesDesdeArchivo(archivoCredenciales);

    // Imprimir las credenciales cargadas (solo como ejemplo)
    for (const Credencial& credencial : credenciales) {
        cout << "Usuario: " << credencial.usuario << endl;
        cout << "Contraseña: " << credencial.contrasenia << endl;
        cout << "Rol: " << credencial.rol << endl;
        cout << "Intentos: " << credencial.intentos << endl;
        cout << "-----------------------" << endl;
    }/*
    Servidorr->Credenciales(Servidorr);
        Servidorr->Recibir();
        Servidorr->Enviar();
    }*/
    return 0;
    }

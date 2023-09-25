#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <list>
#include "Credencial.h"


using namespace std;


class Credencial {
private:
    string usuario;
    string contra;
    string rol;
    int bloqueos;

public:
    Credencial(string usuarioo, string contraa, string roll, int bloqueoo) {
        usuario = usuarioo;
        contra = contraa;
        rol = roll;
        bloqueos = bloqueoo;
    }

  string getUsuario() const {
    return usuario;
}
string getRol() const {
    return rol;
}

string getContra() const {
    return contra;
}

int getBloqueos() const {
    return bloqueos;
}
};

list<Credencial> FuncionArchivoEnEstructura() {
    list<Credencial> credenciales;
    ifstream file("credenciales.txt");

    if (!file) {
        cerr << "Error al abrir el archivo de credenciales." << endl;
        return credenciales;
    }

    string linea;
    while (getline(file, linea)) {
        istringstream iss(linea);
        string usuario, contrasenia, rol, bloqueosStr;

        if (getline(iss, usuario, '|') &&
            getline(iss, contrasenia, '|') &&
            getline(iss, rol, '|') &&
            getline(iss, bloqueosStr, '|')) {
                 // Intenta convertir bloqueosStr a entero, con manejo de errores
            int bloqueos;
              bloqueos = stoi(bloqueosStr);

            Credencial credencial(usuario, contrasenia, rol,bloqueos); // No es necesario convertir bloqueosStr
            credenciales.push_back(credencial);//crea instancia una credencial al final de la lista
            // Para agregar al final tambien puedo usar
                               //credenciales.insert(credenciales.end(), Credencial(usuario, contrasenia, rol, stoi(bloqueosStr)));

        } else {
            cerr << "Error al analizar la línea: " << linea << endl;
        }
    }

    file.close();
    return credenciales;
}



bool buscadorDeUsuario(list<Credencial> credencial,string usuario){
    bool valor=false;
for (Credencial credencial:credencial) {
if(credencial.getUsuario()==usuario){
    valor=true;
}
}
return valor;
}


bool verificarContrasenia(list<Credencial> credencial,string usuario,string contra){
    bool valor=false;
for (Credencial credencial:credencial) {
if(credencial.getUsuario()==usuario){
    if(credencial.getContra()==contra){
        valor=true;
    }
}
}
return valor;
}

/*

int main() {
    list<Credencial> credencial = FuncionArchivoEnEstructura();

    for (Credencial credencial:credencial) {
        cout << "Usuario: " << credencial.getUsuario() << endl;
        cout << "Contrasenia: " << credencial.getContra() << endl;
        cout << "Rol: " << credencial.getRol() << endl;
        cout << "Bloqueos: " << credencial.getBloqueos() << endl;
        cout << "-----------------------" << endl;
    }

    return 0;
}*/


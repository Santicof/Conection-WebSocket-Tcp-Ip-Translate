#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <list>
#include <string>
#include <cstring> // Incluye la biblioteca para usar strcmp


using namespace std;

struct Credencial {
    string usuario;
    string contra;
    string rol;
    int bloqueos;
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

            Credencial credencial;
            credencial.usuario = usuario;
            credencial.contra = contrasenia;
            credencial.rol = rol;
            credencial.bloqueos = bloqueos;

            credenciales.push_back(credencial);
        } else {
            cerr << "Error al analizar la línea: " << linea << endl;
        }
    }

    file.close();
    return credenciales;
}

bool buscadorDeUsuario(list<Credencial> credenciales, string usuario) {
    bool valor = false;
    for (Credencial credencial : credenciales) {
        if (credencial.usuario == usuario) {
                printf("SE ENCONTROOO");
            valor=true;
        }
    }
    return valor;
}

bool verificarContrasenia(list<Credencial> credenciales, string usuario, string contra) {
    bool valor = false;
    for (Credencial credencial : credenciales) {
        if (credencial.usuario == usuario) {
            if (credencial.contra == contra) {
                valor = true;
            }
        }
    }
    return valor;
}

int verificarRol(std::string nombre) {
    // Si devuelve 1 es CONSULTA, si devuelve 2 es ADMIN
    list<Credencial> credenciales = FuncionArchivoEnEstructura();
    int num = 2;

    for (const Credencial& credencial : credenciales) {
        if (credencial.usuario == nombre) {
            if (credencial.rol == "CONSULTA") {
                    printf("es consultaa");
                num = 1;
            }
        }
    }
 printf("nombre recibido: %s",nombre);
    return num;
}

int ObtenerNumeroUsuario(const string& nombreUsuario) {
    ifstream archivoEntrada("credenciales.txt");
    int blo=-1;

    if (!archivoEntrada) {
        cerr << "Error al abrir el archivo." << endl;
        return -1; // Valor de error, si no se puede abrir el archivo
    }

   list<Credencial> credenciales=FuncionArchivoEnEstructura();
   for(Credencial creden:credenciales){
    if(creden.usuario==nombreUsuario){
        blo=creden.bloqueos;
    }

   }

    // Si el usuario no se encuentra, devuelve un valor predeterminado
    return blo; // Valor predeterminado si el usuario no se encuentra
}

void CambiarNumeroUsuario(const string& nombreUsuario, int nuevoNumero) {
    ifstream archivoEntrada("credenciales.txt");
    ofstream archivoSalida("credenciales_temp.txt");

    if (!archivoEntrada || !archivoSalida) {
        cerr << "Error al abrir los archivos." << endl;
        return;
    }

    string linea;
    while (getline(archivoEntrada, linea)) {
        // Divide la línea en campos usando el delimitador '|'
        vector<string> campos;
        size_t inicio = 0;
        size_t fin = linea.find('|');
        while (fin != string::npos) {
            campos.push_back(linea.substr(inicio, fin - inicio));
            inicio = fin + 1;
            fin = linea.find('|', inicio);
        }
        campos.push_back(linea.substr(inicio, fin));

        if (campos.size() >= 4 && campos[0] == nombreUsuario) {
            // Cambia el último número en la línea
            campos[3] = to_string(nuevoNumero);
        }

        // Reconstruye la línea con los campos actualizados
        string nuevaLinea = campos[0];
        for (size_t i = 1; i < campos.size(); ++i) {
            nuevaLinea += "|" + campos[i];
        }

        archivoSalida << nuevaLinea << '\n';
    }

    archivoEntrada.close();
    archivoSalida.close();

    // Reemplaza el archivo original con el archivo temporal
    remove("credenciales.txt");
    rename("credenciales_temp.txt", "credenciales.txt");
}

/*
int main() {
    //CambiarNumeroUsuario("juan", 0);
  int a=ObtenerNumeroUsuario("juan");
   printf("%d",a);
   // MenuCliente();
    /*
    list<Credencial> credenciales = FuncionArchivoEnEstructura();
bool valor;
    for (Credencial credencial : credenciales) {
        cout << "Usuario: " << credencial.usuario << endl;
        cout << "Contrasenia: " << credencial.contra << endl;
        cout << "Rol: " << credencial.rol << endl;
        cout << "Bloqueos: " << credencial.bloqueos << endl;
        cout << "-----------------------" << endl;
    }
   int num=verificarRol("juan");
   printf("JUAN ES : %d",num);

    return 0;
}
*/

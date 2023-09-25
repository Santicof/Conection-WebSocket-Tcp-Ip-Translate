#ifndef CREDENCIAL_H
#define CREDENCIAL_H

#include <string>
#include <list>
#include <iostream>



struct Credencial {

    std::string usuario;
    std::string contra;
    std::string rol;
    int bloqueos;};

std::list<Credencial> FuncionArchivoEnEstructura();
bool buscadorDeUsuario(std::list<Credencial> credencial, std::string usuario);
bool verificarContrasenia(std::list<Credencial> credencial, std::string usuario, std::string contra);

#endif // CREDENCIAL_H


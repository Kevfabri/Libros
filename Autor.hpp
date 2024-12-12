#pragma once

#include "ListaDoble.hpp"
#include "Libro.hpp"
#include <string>

class Autor
{
private:
    std::string cedula;
    std::string nombre;
    std::string nombre2;
    std::string apellido;
public:
    ListaDoble<Libro*> libros;

public:
    Autor(std::string cedula, std::string nombre, std::string nombre2, std::string apellido);
    std::string getId();
    std::string getNombre();
    std::string getNombre2();
    std::string getApellido();
    void setId(std::string nuevoId);
    void setNombre(std::string nuevoNombre);
    void setNombre2(std::string nuevoNombre2);
    void setApellido(std::string nuevoApellido);


    friend std::ostream& operator<<(std::ostream& os, const Autor& autor);

};
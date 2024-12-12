#include "Autor.hpp"

Autor::Autor(std::string cedula, std::string nombre, std::string nombre2, std::string apellido)
    : cedula(cedula), nombre(nombre), nombre2(nombre2), apellido(apellido)
{
}

std::string Autor::getId()
{
    return cedula;
}

std::string Autor::getNombre()
{
    return nombre;
}

std::string Autor::getNombre2()
{
    return nombre2;
}

std::string Autor::getApellido()
{
    return apellido;
}

void Autor::setId(std::string nuevoId)
{
    cedula = nuevoId;
}

void Autor::setNombre(std::string nuevoNombre)
{
    nombre = nuevoNombre;
}

void Autor::setNombre2(std::string nuevoNombre2)
{
    nombre2 = nuevoNombre2;
}

void Autor::setApellido(std::string nuevoApellido)
{
    apellido = nuevoApellido;
}

std::ostream& operator<<(std::ostream& os, const Autor& autor)
{
    os << "Id: " << autor.cedula << ", Nombre: " << autor.nombre << ", Nombre2: " << autor.nombre2 << ", Apellido: " << autor.apellido << ", Libros: (";

    Nodo<Libro*>* aux = autor.libros.getCabeza();
    while (aux != NULL)
    {
        os << aux->getDato()->getTitulo() << ",";
        aux = aux->getSiguiente();
    }

    os << "\b)";

    return os;
}

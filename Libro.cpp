#include "Libro.hpp"
#include "Autor.hpp"

// Constructor
Libro::Libro(const std::string& id, const std::string& titulo, Autor* autor)
    : id(id), titulo(titulo), autor(autor)
{
}

Libro::Libro(const Libro& otroLibro)
    : id(otroLibro.id), titulo(otroLibro.titulo)
{
    autor = otroLibro.autor;
}

Libro::~Libro()
{
    // autor->libros.eliminar(this); // YA ESTARÁ DESTRUIDA, DESENLAZAR MANUALMENTE
}

// Getters y Setters
std::string Libro::getId() const {
    return id;
}

void Libro::setId(const std::string& nuevoId) {
    id = nuevoId;
}

std::string Libro::getTitulo() const {
    return titulo;
}

void Libro::setTitulo(const std::string& titulo) {
    this->titulo = titulo;
}

Autor* Libro::getAutor() const {
    return autor;
}

void Libro::setAutor(Autor* autor) {
    this->autor = autor;
}

// Métodos
std::string Libro::toString() const {
    std::ostringstream oss;
    oss << "Id: " << id << ", Título: " << titulo << ", Autor: " << autor->getNombre() << " " << autor->getApellido();
    return oss.str();
}

// Sobrecarga de operadores
std::ostream& operator<<(std::ostream& os, const Libro& libro) {
    os << libro.toString();
    return os;
}

std::istream& operator>>(std::istream& is, Libro& libro) {
    std::cout << "Ingrese el Id: ";
    std::getline(is, libro.id);
    std::cout << "Ingrese Título: ";
    std::getline(is, libro.titulo);
    std::cout << "Ingrese Autor: ";
    return is;
}

bool Libro::operator==(const Libro& otro) const {
    return id == otro.id;
}

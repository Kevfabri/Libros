#pragma once

#include <string>
#include <iostream>
#include <sstream>

class Autor;

class Libro {
private:
    std::string id;
    std::string titulo;
    Autor* autor;

public:
    Libro(const std::string& id, const std::string& titulo, Autor* autor);
    Libro(const Libro& otroLibro);
    ~Libro();
    
    std::string getId() const;
    void setId(const std::string& nuevoId);

    std::string getTitulo() const;
    void setTitulo(const std::string& titulo);

    Autor* getAutor() const;
    void setAutor(Autor* autor);

    
    std::string toString() const;

    // Sobrecarga de operadores
    friend std::ostream& operator<<(std::ostream& os, const Libro& libro);
    friend std::istream& operator>>(std::istream& is, Libro& libro);

    bool operator==(const Libro& otro) const;
};
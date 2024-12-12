#pragma once

#include "Nodo.hpp"
#include <string>
#include <iostream>

template <typename T>
class ListaDoble
{
private:
    Nodo<T>* cabeza;
    Nodo<T>* cola;
public:
    ListaDoble();
    ListaDoble(const ListaDoble& otraLista);
    ~ListaDoble();
    Nodo<T>* getCabeza() const;
    Nodo<T>* getCola() const;

    void insertarACabeza(T dato);
    void insertarACola(T dato);
    bool buscar(T dato);
    bool eliminar(T dato);
    void mostrar();
    
    void executarEnLista(void(*funcion)(T));

    template <typename U>
    bool buscar(U id, U(*identifierGetter)(T));
    template <typename U>
    bool eliminar(U id, U(*identifierGetter)(T));
};

template<typename T>
inline ListaDoble<T>::ListaDoble()
    : cabeza(NULL),
    cola(NULL)
{
}

template<typename T>
inline ListaDoble<T>::ListaDoble(const ListaDoble& otraLista)
    : cabeza(NULL),
    cola(NULL)
{
    Nodo<T>* aux = otraLista.getCabeza();
    while (aux != NULL)
    {
        insertarACola(aux->getDato());
        aux = aux->getSiguiente();
    }
}

template<typename T>
inline ListaDoble<T>::~ListaDoble()
{
    Nodo<T>* aEliminar;
    Nodo<T>* aux = cabeza;

    while (aux != NULL)
    {
        aEliminar = aux;
        aux = aux->getSiguiente();
        delete aEliminar;
    }
}

template<typename T>
inline Nodo<T>* ListaDoble<T>::getCabeza() const
{
    return cabeza;
}

template<typename T>
inline Nodo<T>* ListaDoble<T>::getCola() const
{
    return cola;
}

template<typename T>
inline void ListaDoble<T>::insertarACabeza(T dato)
{
    Nodo<T>* nuevo = new Nodo<T>(dato);

    if (cabeza == NULL) {
        cabeza = nuevo;
        cola = nuevo;
    }
    else {
        cabeza->setAnterior(nuevo);
        nuevo->setSiguiente(cabeza);
        cabeza = nuevo;
    }
}

template<typename T>
inline void ListaDoble<T>::insertarACola(T dato)
{
    Nodo<T>* nuevo = new Nodo<T>(dato);

    if (cabeza == NULL) {
        cabeza = nuevo;
        cola = nuevo;
    }
    else {

        cola->setSiguiente(nuevo);
        nuevo->setAnterior(cola);
        cola = nuevo;
    }
}

template<typename T>
inline bool ListaDoble<T>::buscar(T dato)
{
    Nodo<T>* aux = cabeza;

    while (aux != NULL)
    {
        if (aux->dato == dato)
            return true;

        aux = aux->siguiente;
    }

    return false;
}

template<typename T>
inline bool ListaDoble<T>::eliminar(T dato)
{
    Nodo<T>* aux = cabeza;

    while (aux != NULL)
    {
        if (aux->dato == dato)
        {
            if (aux == cabeza)
                cabeza = aux->siguiente;
            else
                aux->anterior->siguiente = aux->siguiente;

            if (aux == cola)
                cola = aux->anterior;
            else
                aux->siguiente->anterior = aux->anterior;

            delete aux;
            return true;
        }

        aux = aux->siguiente;
    }

    return false;
}

template<typename T>
inline void ListaDoble<T>::mostrar()
{
    Nodo<T>* aux = cabeza;

    while (aux != NULL)
    {
        std::cout << aux->dato << "=>\n";
        aux = aux->siguiente;
    }

    std::cout << "NULL";
}

template<typename T>
inline void ListaDoble<T>::executarEnLista(void(*funcion)(T))
{
}

template<typename T>
template<typename U>
inline bool ListaDoble<T>::buscar(U id, U(*identifierGetter)(T))
{
    Nodo<T>* aux = cabeza;

    while (aux != NULL)
    {
        if (identifierGetter(aux->dato) == id)
            return true;

        aux = aux->siguiente;
    }

    return false;
}

template<typename T>
template<typename U>
inline bool ListaDoble<T>::eliminar(U id, U(*identifierGetter)(T))
{
    Nodo<T>* aux = cabeza;

    while (aux != NULL)
    {
        if (identifierGetter(aux->dato) == id)
        {
            if (aux == cabeza)
                cabeza = aux->siguiente;
            else
                aux->anterior->siguiente = aux->siguiente;

            if (aux == cola)
                cola = aux->anterior;
            else
                aux->siguiente->anterior = aux->anterior;

            delete aux;
            return true;
        }

        aux = aux->siguiente;
    }

    return false;
}

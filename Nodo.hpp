#pragma once

#include <cstddef>

template <typename T>
class ListaDoble;

template <typename T>
class Nodo
{
    friend class ListaDoble<T>;

private:
    Nodo<T>* anterior;
    Nodo<T>* siguiente;
public:
    T dato;
public:
    Nodo(T dato);
    T getDato();
    Nodo<T>* getAnterior();
    Nodo<T>* getSiguiente();
    void setDato(T dato);
    void setAnterior(Nodo<T>* nuevoAnterior);
    void setSiguiente(Nodo<T>* nuevoSiguiente);
};

template<typename T>
inline Nodo<T>::Nodo(T dato)
    : dato(dato), anterior(NULL), siguiente(NULL)
{
}

template<typename T>
inline void Nodo<T>::setDato(T nuevoDato)
{
    dato = nuevoDato;
}

template<typename T>
inline void Nodo<T>::setAnterior(Nodo<T>* nuevoAnterior)
{
    anterior = nuevoAnterior;
}

template<typename T>
inline void Nodo<T>::setSiguiente(Nodo<T>* nuevoSiguiente)
{
    siguiente = nuevoSiguiente;
}

template<typename T>
inline T Nodo<T>::getDato()
{
    return dato;
}

template<typename T>
inline Nodo<T>* Nodo<T>::getAnterior()
{
    return nullptr;
}

template<typename T>
inline Nodo<T>* Nodo<T>::getSiguiente()
{
    return siguiente;
}

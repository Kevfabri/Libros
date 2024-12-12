#include "ListaDoble.hpp"
#include "Libro.hpp"
#include "Autor.hpp"
#include "Validaciones.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <ctime>
#include <algorithm>
#include <sstream>
#include <stdio.h>

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_ENTER 10

void cargar(ListaDoble<Libro>* ListaLibros, ListaDoble<Autor>* listaAutores, std::string directorio = ".");
void guardar(ListaDoble<Libro>* listaLibros, ListaDoble<Autor>* listaAutores, std::string directorio = ".", bool backup = false);

// FUNCION PARA CONSEGUIR EL PUNTERO DE UN LIBRO EXISTENTE USANDO SU ID
Autor* punteroAutorEnLista(std::string cedula, ListaDoble<Autor>* listaAutores);
Libro* punteroLibroEnLista(std::string id, ListaDoble<Libro>* listaLibros);

int main()
{
    ListaDoble<Libro>* listaLibros = new ListaDoble<Libro>();
    ListaDoble<Autor>* listaAutores = new ListaDoble<Autor>();
    

    cargar(listaLibros, listaAutores, ".");

    /////////////////////// COMPROBACIÓN DE EXISTENCIA DE UN ARCHIVO DE AUTOGUARDADO ////////////////////////
    
    if (std::filesystem::is_directory("unsaved"))
    {
        ListaDoble<Libro> listaLibrosTemp;
        ListaDoble<Autor> listaAutoresTemp;
        cargar(&listaLibrosTemp, &listaAutoresTemp, "unsaved");

        std::cout << "Lista de Libros guardada: " << std::endl;
        listaLibros->mostrar();
        std::cout << std::endl << "\033[1;30mLista de Libros sin guardar recuperada:" << std::endl;
        listaLibrosTemp.mostrar();

        std::cout << "\033[0m" << std::endl << std::endl;
        
        std::cout << "Lista de Autores guardada: " << std::endl;
        listaAutores->mostrar();
        std::cout << std::endl << "\033[1;30mLista de Autores sin guardar recuperada:" << std::endl;
        listaAutoresTemp.mostrar();

        std::cout << std::endl << std::endl << "\033[1;34mLa aplicación no se ha cerrado apropiadamente en la última sesión, ¿Desea restaurar los cambios sin guardar? (Y/n): \033[0m";
        char opcion = tolower(getch());

        if (opcion == 'y' || opcion == '\n' || opcion == '\r')
        {
            delete listaLibros;
            delete listaAutores;
            listaLibros = new ListaDoble<Libro>();
            listaAutores = new ListaDoble<Autor>();

            cargar(listaLibros, listaAutores, "unsaved");
            std::cout << std::endl << "DATOS RESTAURADOS";
            getch();
        }
    }

    /////////////////////// DECLARACIÓN DE LOS MENÚS ////////////////////////

    const std::string menuListas =
        "1. Libros\n"
        "2. Autores\n"
        "3. Salida\n";

    const std::string menuOpLibros =
        "1. Insertar\n"
        "2. Buscar\n"
        "3. Eliminar\n"
        "4. Mostrar\n"
        "5. Guardar\n"
        "6. Volver\n";

    const std::string menuOpAutores =
        "1. Insertar\n"
        "2. Buscar\n"
        "3. Eliminar\n"
        "4. Mostrar\n"
        "5. Guardar\n"
        "6. Volver\n";
    
    /////////////////////// FUNCIÓN LAMBDA QUE IMPRIME UN MENÚ ////////////////////////
    // Resalta el elemento seleccionado
    ///////////////////////////////////////////////////////////////////////////////////
    auto imprimirMenu = [](std::string menu, int eleccion)
        {
            std::stringstream ss(menu);
            std::string s;
            int cont = 0;
            while (getline(ss, s))
            {
                cont++;
                std::cout << ((cont == eleccion) ? "\033[5m>\033[25m \033[7;1m" : "  ") << s << std::endl;
                if (cont == eleccion)
                    std::cout << "\033[0m";
            }
        };

    /////////////////////// FUNCIÓN LAMBDA QUE RESTRINGE LOS VALORES DE ELECCION ////////////////////////
    // la entrada se realiza mediante las flechas arriba (aumentar), abajo (disminuir) y el botón enter
    // TODO: decidir si es apropiado cambiar el nombre de la función
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    auto procesarEntrada = [](int& eleccion, int entrada, int eleccionMax) -> bool
        {
            //
        // PARA LINUX, se procesan 3 caracteres al presionar las flechas: '\033', '[' y una letra
            if (entrada == '\033')
            {
                getch(); // lee el '['
                entrada = getch(); // lee la letra
                switch (entrada)
                {
                case 'A':   // KEY_UP
                    entrada = KEY_UP;
                    break;
                case 'B':   // KEY_DOWN
                    entrada = KEY_DOWN;
                    break;
                case 'C':   // KEY_LEFT
                    break;
                case 'D':   // KEY_RIGHT
                    break;

                default:
                    break;
                }
            }
            // FIN PARA LINUX
            //

            switch (entrada)
            {
            case KEY_UP:
                if (eleccion == 1)
                    eleccion = eleccionMax;
                else
                    eleccion--;

                break;
            case KEY_DOWN:
                if (eleccion == eleccionMax)
                    eleccion = 1;
                else
                    eleccion++;

                break;
            case KEY_ENTER:
                return true;
                break;
            }
            return false;
        };

    /////////////////////// DECLARACIÓN DE ENUMERACIONES ////////////////////////
    // No son necesarias, pero hacen que el código sea más legible (no ha sido
    // suficiente), especialmente cuando existen varios submenús.
    /////////////////////////////////////////////////////////////////////////////
    enum eleccionListas { LIBROS = 1, AUTORES, SALIR };
    enum eleccionOpLibros { INSERTAR_LIBROS = 1, BUSCAR_LIBROS, ELIMINAR_LIBROS, MOSTRAR_LIBROS, GUARDAR_LIBROS, SALIR_LIBROS };
    enum eleccionOpAutores { INSERTAR_AUTORES = 1, BUSCAR_AUTORES, ELIMINAR_AUTORES, MOSTRAR_AUTORES, GUARDAR_AUTORES, SALIR_AUTORES };

    /////////////////////// FUNCIÓN LAMBDA QUE OPERA SOBRE LA LISTA DE AUTORES ////////////////////////
    // Es sólo un switch
    // Va primero porque las operaciones en libros necesita acceder a las de autores
    //////////////////////////////////////////////////////////////////////////////////////////////////
    auto operarAutores = [&menuOpLibros, listaLibros, listaAutores](int eleccion)
        {
            std::string cedula;

            std::string(*conseguirIdAutor)(Autor) = [](Autor a)
                {
                    return a.getId();
                };

            switch (eleccion)
            {
            case INSERTAR_AUTORES:
            {
                std::cout << "Ingrese el cedula del autor a insertar (0 para cancelar): ";
                cedula = Validaciones::leerCedula();
                std::cout << std::endl;

                if (listaAutores->buscar(cedula, conseguirIdAutor))
                {
                    std::cout << "El cedula ya existe en la lista";
                    getch();
                    break;
                }

                if (cedula == "0" || cedula == "")
                {
                    std::cout << "No se ha insertado";
                    getch();
                    break;
                }

                if (!Validaciones::validarCedula(cedula))
                {
                    std::cout << "CÉDULA NO VÁLIDA";
                    getch();
                    break;
                }

                std::cout << "Ingrese el nombre a insertar (vacío para cancelar): ";
                std::string nombre = Validaciones::leerPalabra();
                std::cout << std::endl;

                if (nombre.empty())
                {
                    std::cout << "No se ha insertado";
                    getch();
                    break;
                }

                std::cout << "Ingrese el segundo nombre a insertar: ";
                std::string nombre2 = Validaciones::leerPalabra();
                std::cout << std::endl;
                

                std::cout << "Ingrese el apellido a insertar: ";
                std::string apellido = Validaciones::leerPalabra();
                std::cout << std::endl;

                listaAutores->insertarACola(Autor(cedula, nombre, nombre2, apellido));
                break;
            }
            case BUSCAR_AUTORES:
                std::cout << "Ingrese el cedula del autor a buscar: ";
                cedula = Validaciones::leerCedula();
                std::cout << std::endl;

                if (!Validaciones::validarCedula(cedula))
                {
                    std::cout << "CÉDULA NO VÁLIDA";
                    getch();
                    break;
                }

                if (listaAutores->buscar(cedula, conseguirIdAutor))
                    std::cout << "El autor con numero " << cedula << " se encuentra en la lista";
                else
                    std::cout << "El autor con numero " << cedula << " NO se encuentra en la lista";
                getch();
                break;
            case ELIMINAR_AUTORES:
            {
                std::cout << "Ingrese el cedula del autor a eliminar: ";
                cedula = Validaciones::leerCedula();
                std::cout << std::endl;

                if (!Validaciones::validarCedula(cedula))
                {
                    std::cout << "CÉDULA NO VÁLIDA";
                    getch();
                    break;
                }

                Autor* punteroAutor = punteroAutorEnLista(cedula, listaAutores);
                if (punteroAutor)
                {
                    Nodo<Libro*>* aux = punteroAutor->libros.getCabeza();
                    while (aux != NULL)
                    {
                        Nodo<Libro>* aux2 = listaLibros->getCabeza();
                        while (aux2 != NULL)
                        {
                            if (aux->dato == &(aux2->dato))
                            {
                                listaLibros->eliminar(aux2->getDato());
                                break;
                            }

                            aux2 = aux2->getSiguiente();
                        }

                        aux = aux->getSiguiente();
                    }

                    listaAutores->eliminar(cedula, conseguirIdAutor);
                    std::cout << "El autor con numero " << cedula << " ha sido eliminado de la lista";
                }
                else
                    std::cout << "El autor con numero " << cedula << " NO se encuentra en la lista";

                getch();
                break;
            }
            case MOSTRAR_AUTORES:
                std::cout << std::endl;
                listaAutores->mostrar();
                std::cout << std::endl;
                getch();
                break;
            case GUARDAR_AUTORES:
            {
                guardar(listaLibros, listaAutores, ".", 1);
                break;
            }
            case SALIR_AUTORES:
                std::cout << "Saliendo..." << std::endl;
                break;
            default:
                std::cout << "NO ES UNA OPCIÓN VÁLIDA" << std::endl;
                getch();
                break;
            }
        };

    
    /////////////////////// FUNCIÓN LAMBDA QUE OPERA SOBRE LA LISTA DE LIBROS ////////////////////////
    // Es sólo un switch
    // La lista de autores también está capturada, para la asignación de autores
    //////////////////////////////////////////////////////////////////////////////////////////////////
    
    auto operarLibros = [&menuOpLibros, &operarAutores, listaLibros, listaAutores](int eleccion)
        {
            std::string id;

            std::string(*conseguirIdLibro)(Libro) = [](Libro l)
                {
                    return l.getId();
                };

            switch (eleccion)
            {
            case INSERTAR_LIBROS:
            {
                std::cout << "Ingrese el id del libro a insertar (0 para cancelar): ";
                id = Validaciones::leerNumero();
                std::cout << std::endl;

                if (listaLibros->buscar(id, conseguirIdLibro))
                {
                    std::cout << "El id ya existe en la lista";
                    getch();
                    break;
                }

                if (id == "0" || id == "")
                {
                    std::cout << "No se ha insertado";
                    getch();
                    break;
                }

                std::cout << "Ingrese el título del libro a insertar (vacío para cancelar): ";
                std::string titulo = Validaciones::leerPalabra();
                std::cout << std::endl;

                if (titulo.empty())
                {
                    std::cout << "No se ha insertado";
                    getch();
                    break;
                }

                std::string idAutor;
                // para comprobar que un autor se ha insertado (la cola ha cambiado)
                Autor* punteroAutorEnCola = &(listaAutores->getCola()->dato);

                std::cout << "Ingrese el cedula del autor del libro (0 para cancelar, vacío para nuevo autor): \033[s";
                
                do
                {
                    std::cout << "\033[u\033[J" << std::endl << std::endl;

                    int cont = 0;

                    Nodo<Autor>* aux = listaAutores->getCabeza();
                    while (aux != NULL)
                    {
                        std::cout << aux->getDato() << std::endl;
                        aux = aux->getSiguiente();
                    }
                    std::cout << "\033[u";
                    std::string idAutor = Validaciones::leerCedula();

                    if (!Validaciones::validarCedula(idAutor))
                    {
                        std::cout << "CÉDULA NO VÁLIDA";
                        getch();
                        break;
                    }
                    
                    std::cout << "\033[J" << std::endl;

                    if (idAutor == "0")
                    {
                        std::cout << "No se ha insertado";
                        getch();
                        break;
                    }

                    Autor* punteroAutor = NULL;

                    if (idAutor.empty())
                    {
                        operarAutores(INSERTAR_AUTORES);
                        
                        if (punteroAutorEnCola != &(listaAutores->getCola()->dato))
                            punteroAutor = &(listaAutores->getCola()->dato);
                    }
                    else
                    {
                        Nodo<Autor>* aux = listaAutores->getCabeza();
                        while (aux != NULL)
                        {
                            if (aux->getDato().getId() == idAutor)
                                punteroAutor = &(aux->dato);
                            aux = aux->getSiguiente();
                        }
                    }

                    if (punteroAutor)
                    {
                        listaLibros->insertarACola(Libro(id, titulo, punteroAutor));
                        punteroAutor->libros.insertarACola(&(listaLibros->getCola()->dato));
                        break;
                    }
                    else if (!idAutor.empty())
                    {
                        std::cout << "El id ingresado no se encuentra en la lista, desea añadir un autor? (Y/n): ";
                        char c = getch();
                        std::cout << std::endl;
                        if (c == '\n' || c == '\r' || tolower(c) == 'y')
                        {
                            operarAutores(INSERTAR_AUTORES);

                            if (punteroAutorEnCola != &(listaAutores->getCola()->dato))
                            {
                                punteroAutor = &(listaAutores->getCola()->dato);

                                listaLibros->insertarACola(Libro(id, titulo, punteroAutor));
                                punteroAutor->libros.insertarACola(&(listaLibros->getCola()->dato));
                            }
                        }
                        else
                        {
                            std::cout << "NO SE HA INSERTADO";
                            getch();
                            break;
                        }

                    }
                } while (idAutor.empty() && punteroAutorEnCola == &(listaAutores->getCola()->dato));
                
                                
                break;
            }
            case BUSCAR_LIBROS:
                std::cout << "Ingrese el numero del libro a buscar: ";
                id = Validaciones::leerNumero();
                std::cout << std::endl;

                if (listaLibros->buscar(id, conseguirIdLibro))
                    std::cout << "El libro con numero " << id << " se encuentra en la lista";
                else
                    std::cout << "El libro con numero " << id << " NO se encuentra en la lista";
                getch();
                break;
            case ELIMINAR_LIBROS:
            {
                std::cout << "Ingrese el numero del libro a eliminar: ";
                id = Validaciones::leerNumero();
                std::cout << std::endl;
                Libro* punteroLibro = punteroLibroEnLista(id, listaLibros);
                if (punteroLibro)
                {
                    punteroLibro->getAutor()->libros.eliminar(punteroLibro);
                    listaLibros->eliminar(id, conseguirIdLibro);
                    std::cout << "El libro con numero " << id << " ha sido eliminado de la lista";
                }
                else
                    std::cout << "El libro con numero " << id << " NO se encuentra en la lista";

                getch();
                break;
            }
            case MOSTRAR_LIBROS:
                std::cout << std::endl;
                listaLibros->mostrar();
                std::cout << std::endl;
                getch();
                break;
            case GUARDAR_LIBROS:
            {
                guardar(listaLibros, listaAutores, ".", 1);
                break;
            }
            case SALIR_LIBROS:
                std::cout << "Saliendo..." << std::endl;
                break;
            default:
                std::cout << "NO ES UNA OPCIÓN VÁLIDA" << std::endl;
                getch();
                break;
            }
        };

    /*****************************************************************/
    /*///////////////////////////////////////////////////////////////*/
    /*////////////////////// LÓGICA PRINCIPAL ///////////////////////*/
    /*///////////////////////////////////////////////////////////////*/
    /*v v v v v v v v v v v v v v v v v v v v v v v v v v v v v v v v*/

    int eleccion = 1;
    int entrada;
    bool salir = false;
    do
    {
        system("clear");
        std::cout << "============ LISTAS DOBLES ============" << std::endl;
        imprimirMenu(menuListas, eleccion);
        entrada = getch();

        if (procesarEntrada(eleccion, entrada, SALIR))
        {
            bool salir2 = false;
            int eleccion2 = 1;
                switch (eleccion)
                {
                case LIBROS:
                    do
                    {
                        system("clear");
                        std::cout << "============ LISTA LIBROS ============" << std::endl;
                        imprimirMenu(menuOpLibros, eleccion2);
                        entrada = getch();

                        if (procesarEntrada(eleccion2, entrada, SALIR_LIBROS))
                        {
                            operarLibros(eleccion2);

                            if (eleccion2 == GUARDAR_LIBROS)
                                std::filesystem::remove_all("unsaved");

                            if (eleccion2 == INSERTAR_LIBROS || eleccion2 == ELIMINAR_LIBROS)
                                guardar(listaLibros, listaAutores, "unsaved");

                            if (eleccion2 == SALIR_LIBROS)
                                salir2 = true;
                        }

                    } while (!salir2);
                    break;
                case AUTORES:
                    do
                    {
                        system("clear");
                        std::cout << "============ LISTA AUTOR ============" << std::endl;
                        imprimirMenu(menuOpAutores, eleccion2);
                        entrada = getch();

                        if (procesarEntrada(eleccion2, entrada, SALIR_AUTORES))
                        {
                            operarAutores(eleccion2);

                            if (eleccion2 == GUARDAR_AUTORES)
                                std::filesystem::remove_all("unsaved");

                            if (eleccion2 == INSERTAR_AUTORES || eleccion2 == ELIMINAR_AUTORES)
                                guardar(listaLibros, listaAutores, "unsaved");

                            if (eleccion2 == SALIR_AUTORES)
                                salir2 = true;
                        }

                    } while (!salir2);
                    break;
                case SALIR:
                    salir = true;
                    break;
                default:
                    break;
                }
        }

    } while (!salir);

    return 0;
}

/////////////////////// FUNCIÓN DE CARGADO ////////////////////////
// Toma por defecto el directorio actual (ver declaración)
// Puede elegirse cualquier carpeta en la que se haya usado la
// función guardar (tendrá un archivo autores.txt y libros.txt)
////////////////////////////////////////////////////////////////////
void cargar(ListaDoble<Libro>* ListaLibros, ListaDoble<Autor>* listaAutores, std::string directorio)
{
     // CARGA AUTORES, va primero porque en el archivo de autores no están listados sus libros
    std::ifstream archivoAutores(directorio + '/' + "autores.txt");

    std::string lineaArchivo;

    while (getline(archivoAutores, lineaArchivo))
    {
        std::istringstream ss(lineaArchivo);
        std::string cedula, nombre, nombre2, apellido;

        getline(ss, cedula, ',');
        getline(ss, nombre, ',');
        getline(ss, nombre2, ',');
        getline(ss, apellido);

        listaAutores->insertarACola(Autor(cedula, nombre, nombre2, apellido));
    }

    archivoAutores.close();

    //  CARGA LIBROS en el archivo de libros se encuentran los cedula de sus autores
    std::ifstream archivoLibros(directorio + '/' + "libros.txt");


    while (getline(archivoLibros, lineaArchivo))
    {
        std::istringstream ss(lineaArchivo);
        std::string id, titulo, idAutor;
        Autor* autor;

        getline(ss, id, ',');
        getline(ss, titulo, ',');

        getline(ss, idAutor);
        autor = punteroAutorEnLista(idAutor, listaAutores);

        // BUSCA EL PUNTERO DEL AUTOR Y LO ASIGNA AL LIBRO
        ListaLibros->insertarACola(Libro(id, titulo, autor));
        // TOMA EL PUNTERO DEL LIBRO INGRESADO MÁS RECIENTEMENTE Y LO AÑADE A LOS LIBROS DEL AUTOR
        autor->libros.insertarACola(&(ListaLibros->getCola()->dato));
    }

    archivoLibros.close();
    // ENLAZA LIBROS Y AUTORES
}

/////////////////////// FUNCIÓN DE GUARDADO ////////////////////////
// Se explica sólo
// Toma por defecto el directorio actual (ver declaración)
// Generará libros.txt y autores.txt en el directorio señalado
// Puede eligirse si crear un backup a partir del archivo guardado
// El backup se genera en una carpera llamada backup, donde existen
// subcarpetas con la fecha y hora de su creación
////////////////////////////////////////////////////////////////////
void guardar(ListaDoble<Libro>* listaLibros, ListaDoble<Autor>* listaAutores, std::string directorio, bool backup)
{
    if (!directorio.empty())
        std::filesystem::create_directories(directorio);

    // GUARDA LIBROS AL ARCHIVO
    std::ofstream archivoLibros(directorio + '/' + "libros.txt");

    Nodo<Libro>* aux = listaLibros->getCabeza();

    while (aux != NULL)
    {
        archivoLibros << aux->getDato().getId() << ","
            << aux->getDato().getTitulo() << ","
            << aux->getDato().getAutor()->getId()
            << std::endl;

        aux = aux->getSiguiente();
    }

    archivoLibros.close();

    // GUARDA AUTORES AL LIBRO
    std::ofstream archivoAutores(directorio + '/' + "autores.txt");

    Nodo<Autor>* aux2 = listaAutores->getCabeza();

    while (aux2 != NULL)
    {
        archivoAutores << aux2->getDato().getId() << ","
            << aux2->getDato().getNombre() << ","
            << aux2->getDato().getNombre2() << ","
            << aux2->getDato().getApellido()
            << std::endl;

        aux2 = aux2->getSiguiente();
    }

    archivoAutores.close();

    if (backup)
    {
        time_t tiempo = time(NULL);
        std::string nombreCarpeta(ctime(&tiempo));
        nombreCarpeta.pop_back();   // elimina el salto de línea
        std::replace(nombreCarpeta.begin(), nombreCarpeta.end(), ':', '.');

        std::string direccionBackup = "backup/" + nombreCarpeta;

        if (!std::filesystem::is_directory(direccionBackup))
        {
            std::filesystem::create_directories(direccionBackup);
            std::filesystem::copy_file(directorio + '/' + "libros.txt", direccionBackup + '/' + "libros.txt");
            std::filesystem::copy_file(directorio + '/' + "autores.txt", direccionBackup + '/' + "autores.txt");
        }
    }
}

Autor* punteroAutorEnLista(std::string id, ListaDoble<Autor>* listaAutores)
{
    Nodo<Autor>* aux = listaAutores->getCabeza();
    while (aux != NULL)
    {
        if (aux->getDato().getId() == id)
            return &(aux->dato);

        aux = aux->getSiguiente();
    }

    return (Autor*)nullptr;
};

Libro* punteroLibroEnLista(std::string id, ListaDoble<Libro>* listaLibros)
{
    Nodo<Libro>* aux = listaLibros->getCabeza();
    while (aux != NULL)
    {
        if (aux->getDato().getId() == id)
            return &(aux->dato);

        aux = aux->getSiguiente();
    }

    return (Libro*)nullptr;
};
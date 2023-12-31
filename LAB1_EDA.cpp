#include <iostream>
#include <math.h>
#define MAX_CHAR 3
#define TEXTO_MAX 22
using namespace std;

typedef string Cadena;

enum _retorno{
    OK, ERROR, NO_IMPLEMENTADA
};

typedef enum _retorno TipoRet;

struct archivo{
    Cadena nombre;
    Cadena extension;
    bool escritura = true;
    Cadena contenido;
    archivo * sig;
    Cadena creador;
};

typedef archivo * archivos;

struct directorio {
    Cadena nombre;
    archivos file;
    directorio * sH;
    directorio * pH;
    directorio * padre;
};

typedef directorio * dir;

struct user{
    Cadena nombre;
    user * sig;
};

typedef user * users;

struct _sistema{
    Cadena nombre = "SISTEMA";
    dir RAIZ;
    dir actual;
    users usuarios;
    users usuarioActual;
};

typedef _sistema * Sistema;

// FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES 

// CREATEFILE

/*
Recorre los archivos de un directorio y posiciona un puntero en el archivo anterior al que se va a colocar un nuevo archivo,
de forma que respete el orden alfabético. En caso de tener mismo nombre, compara las extensiones de cada uno.
*/

archivos buscarEspacioArchAlfabeticamente(archivos cadenaArchivos, Cadena nombreArchivo, Cadena extensionArchivo){
    int cont = 0;
    while(cadenaArchivos->sig != NULL){
        if(nombreArchivo == cadenaArchivos->sig->nombre){
            Cadena nombreArchivoTemp = cadenaArchivos->sig->nombre;
            while(cadenaArchivos->sig != NULL && nombreArchivoTemp == cadenaArchivos->sig->nombre && cont < extensionArchivo.length()){
                if(cadenaArchivos->sig->extension[cont] < extensionArchivo[cont]){
                    cadenaArchivos = cadenaArchivos->sig;
                    cont = 0;
                }
                else if(cadenaArchivos->sig->extension[cont] > extensionArchivo[cont]){
                    return cadenaArchivos;
                } else {
                    cont++;
                    if (cont >= extensionArchivo.length()){
                        return cadenaArchivos;
                    }
                }
            }
        } else {
            bool cambioArchivo = false;
            while(cadenaArchivos->sig != NULL && !cambioArchivo && cont < nombreArchivo.length()){
                if(cadenaArchivos->sig->nombre[cont] < nombreArchivo[cont]){
                    cadenaArchivos = cadenaArchivos->sig;
                    cambioArchivo = true;
                    cont = 0;
                }
                else if(cadenaArchivos->sig->nombre[cont] > nombreArchivo[cont]){
                    return cadenaArchivos;
                } else {
                    cont++;
                    if (cont >= nombreArchivo.length()){
                        return cadenaArchivos;
                    }
                }
            }
        }
    }
    return cadenaArchivos;
}

// Devuelve true si en un directorio especificado ya existe un archivo con el mismo nombre y extension, y false en caso contrario.

bool existeArch(archivos & aux, Sistema s, Cadena nombre, Cadena extension){
    aux = s->actual->file;
    while(aux != NULL){
        if(aux->nombre == nombre && aux->extension == extension){
            return true;
        }
        aux = aux->sig;
    }
    return false;
}

// Toma el nombre de un archivo (sin la extensión).

Cadena tomarNombre(Cadena nombreArchivo){
    Cadena nombre;
    for (int i = 0; i < nombreArchivo.length(); i++){
        if (nombreArchivo[i] != '.'){ 
            nombre += nombreArchivo[i];
        } else {
            return nombre;
        }
    }
    return nombre;
}

// Toma la extensión de un archivo (sin el nombre).

Cadena tomarExtension(Cadena nombreArchivo){
    Cadena extensionTemp;
    for(int i = 0; i < nombreArchivo.length(); i++){
        if(nombreArchivo[i] == '.'){
            i++;
            while(i < nombreArchivo.length()){
                extensionTemp += nombreArchivo[i];
                i++;
            }
            return extensionTemp;
        }
    }
    return extensionTemp;
}

// Permite crear un nuevo archivo en el directorio actual (con un nombre especificado), siempre que no haya uno con el mismo nombre y extensión.

TipoRet CREATEFILE (Sistema & s, Cadena nombreArchivo){
    archivos puntVerificar = NULL;
    if(!existeArch(puntVerificar, s, tomarNombre(nombreArchivo), tomarExtension(nombreArchivo)) && nombreArchivo != "RAIZ"){

        archivos puntFinal = buscarEspacioArchAlfabeticamente(s->actual->file, tomarNombre(nombreArchivo), tomarExtension(nombreArchivo));

        archivos nuevoArchivo = new archivo;
        nuevoArchivo->nombre = tomarNombre(nombreArchivo);
        nuevoArchivo->extension = tomarExtension(nombreArchivo);
        nuevoArchivo->creador = s->usuarioActual->nombre;

        if (puntFinal->sig == NULL){
            nuevoArchivo->sig = NULL;
        } else {
            nuevoArchivo->sig = puntFinal->sig;
        }

        puntFinal->sig = nuevoArchivo;
        return OK;
    } else {
        cout << "Ya existe un archivo con ese nombre completo en el directorio actual" << endl;
        return ERROR;
    }
}

// DELETE

// Elimina un archivo del directorio actual (según el nombre especificado), siempre y cuando no sea de solo lectura.

TipoRet DELETE (Sistema & s, Cadena nombreArchivo){
    archivos borrar = NULL;
    if(existeArch(borrar, s, tomarNombre(nombreArchivo), tomarExtension(nombreArchivo))){
        if(!borrar->escritura){
            cout << "El archivo existe, pero es de sólo lectura." << endl;
            return ERROR;
        }
        archivos aux = s->actual->file;
        while(aux->sig != borrar){
            aux = aux->sig;
        }
        aux->sig = borrar->sig;
        delete borrar;
        return OK;
    } else {
        cout << "No existe un archivo con ese nombre en el directorio actual" << endl;
        return ERROR;
    }
}

// ATTRIB

// Permite modificar los permisos que tiene un archivo (lectura/escritura y solo lectura).

TipoRet ATTRIB (Sistema & s, Cadena nombreArchivo, Cadena parametro){
    archivos aux = NULL;
    if(existeArch(aux, s, tomarNombre(nombreArchivo), tomarExtension(nombreArchivo))){
        if(parametro == "+W"){
            aux->escritura = true;
            return OK;
        } else if(parametro == "-W"){
            aux->escritura = false;
            return OK;
        } else {
            cout << "Ingrese un parámetro válido" << endl;
            return ERROR;
        }
    } else {
        cout << "No existe un archivo con ese nombre en el directorio actual" << endl;
        return ERROR;
    }
}

// IF

// Devuelve true en caso de que el usuario especificado se encuentre creado en el sistema, y false en caso contrario.

bool existeElUsuario(users listaUsuarios, Cadena creador){
    while(listaUsuarios != NULL){
        if(listaUsuarios->nombre == creador){
            return true;
        }
        listaUsuarios = listaUsuarios->sig;
    }
    return false;
}

// Agrega un texto al comienzo del contenido de un archivo, y verificando que no supere los 22 caracteres.

void agregarTexto(archivos aux, Sistema s, Cadena texto){
    if (texto.length() >= TEXTO_MAX){
        aux->contenido = "";
        for (int i = 0; i < TEXTO_MAX; i++){
            aux->contenido += texto[i];
        }
    } else {
        Cadena nuevoTexto = "";
        for (int i = 0; i < texto.length(); i++){
            nuevoTexto += texto[i];
        }
        for (int i = 0; i < (TEXTO_MAX - texto.length()); i++){
            nuevoTexto += aux->contenido[i];
        }
        aux->contenido = nuevoTexto;
    }
}

// Permite modificar el contenido de un archivo, agregando un texto especificado al comienzo de este (en caso de que sea de lectura/escritura).

TipoRet IF (Sistema & s, Cadena nombreArchivo, Cadena texto){
    archivos aux = NULL;
    if(existeArch(aux, s, tomarNombre(nombreArchivo), tomarExtension(nombreArchivo))){
        if (!aux->escritura){
            cout << "El archivo es de sólo lectura" << endl;
            return ERROR;
        }
        if(aux->creador != s->usuarioActual->nombre && existeElUsuario(s->usuarios, aux->creador)){
            cout << "No eres el creador de este archivo, por lo que no puedes modificarlo" << endl;
            return ERROR;
        }
        agregarTexto(aux, s, texto);
        return OK;
    } else {
        cout << "No existe un archivo con ese nombre en el directorio actual" << endl;
        return ERROR;
    }
}

// DF

// Permite modificar el contenido de un archivo, eliminando los k primeros caracteres de este (en caso de que sea de lectura/escritura).

TipoRet DF (Sistema & s, Cadena nombreArchivo, int k){
    archivos aux = NULL;
    if(existeArch(aux, s, tomarNombre(nombreArchivo), tomarExtension(nombreArchivo))){
        if (!aux->escritura){
            cout << "El archivo es de sólo lectura" << endl;
            return ERROR;
        }
        if(aux->creador != s->usuarioActual->nombre && existeElUsuario(s->usuarios, aux->creador)){
            cout << "No eres el creador de este archivo, por lo que no puedes modificarlo" << endl;
            return ERROR;
        }
        Cadena resultado = "";
        for(int i = k; i < aux->contenido.length(); i++){
            resultado += aux->contenido[i];
        }
        aux->contenido = resultado;
        return OK;
    } else {
        cout << "No existe un archivo con ese nombre en el directorio actual" << endl;
        return ERROR;
    }
}

// TYPE

// Devuelve el contenido de un archivo especificado del directorio actual.

TipoRet TYPE (Sistema & s, Cadena nombreArchivo){
    archivos aux = NULL;
    if(existeArch(aux, s, tomarNombre(nombreArchivo), tomarExtension(nombreArchivo))){
        if (aux->contenido == ""){
            cout << "El archivo parámetro no posee contenido" << endl;
            return OK;
        }
        cout << aux->contenido << endl;
        return OK;
    } else {
        cout << "No existe un archivo con ese nombre en el directorio actual" << endl;
        return ERROR;
    }
}

// CREARSISTEMA

// Crea un nodo dummy para los directorios.

dir crearDirectorioDummy(){
    dir dummy = new directorio;
    dummy->nombre = "NULL";
    dummy->file = NULL;
    dummy->pH = NULL;
    dummy->sH = NULL;
    return dummy;
}

// Crea un nodo dummy para los archivos.

archivos crearArchivoDummy(){
    archivos dummy = new archivo;
    dummy->nombre = "NULL";
    dummy->sig = NULL;
    return dummy;
}

// Crea un nodo dummy para los usuarios.

users crearUsuarioDummy(){
    users dummy = new user;
    dummy->nombre = "NULL";
    dummy->sig = NULL;
    return dummy;
}

// Crea el sistema, junto con el directorio RAIZ y los respectivos dummies.

TipoRet CREARSISTEMA(Sistema & s){
    s = new _sistema;

    // Creamos un primer usuario dummy

    s->usuarios = crearUsuarioDummy();
    s->usuarioActual = s->usuarios;
    
    s->RAIZ = new directorio;
    s->RAIZ->nombre = "RAIZ";
    s->RAIZ->sH = NULL;
    s->RAIZ->padre = NULL;
    s->actual = s->RAIZ;

    // Creamos un primer archivo dummy

    s->RAIZ->file = crearArchivoDummy();
    
    // Creamos un primer hijo dummy

    s->RAIZ->pH = crearDirectorioDummy();
    s->RAIZ->pH->padre = s->actual;

    return OK;
}

// DESTRUIRSISTEMA

// Borra todos los archivos de una lista.

void borrarArchivos(archivos archivosABorrar){
    if(archivosABorrar == NULL){
        return;
    }
    if(archivosABorrar->sig != NULL){
        borrarArchivos(archivosABorrar->sig);
    }
    delete archivosABorrar;
}

// Borra todos los hijos (directorios) de un directorio, junto con los archivos de cada uno, asi como el mismo directorio (y sus archivos).

void borrarHijos(dir borrar){
    if(borrar == NULL){
        return;
    }
    if(borrar->sH != NULL){
        borrarHijos(borrar->sH);
    }
    if(borrar->pH != NULL){
        borrarHijos(borrar->pH);
    }
    borrarArchivos(borrar->file);
    delete borrar;
}

// Borra todos los usuarios del sistema.

void borrarUsuarios(users usuariosABorrar){
    if (usuariosABorrar == NULL){
        return;
    }
    if (usuariosABorrar->sig != NULL){
        borrarUsuarios(usuariosABorrar->sig);
    }
    delete usuariosABorrar;
}

// Borra todos los archivos y directorios contenidos dentro de RAIZ, asi como el propio directorio y el sistema

TipoRet DESTRUIRSISTEMA(Sistema & s){
    borrarHijos(s->RAIZ);
    borrarUsuarios(s->usuarios);
    delete s;
    return OK;
}

// CD

// Devuelve true si ya existe un directorio con el nombre especificado en el directorio actual, y false en caso contrario.

bool nombreDirectorioExistente(dir cadenaDirectorios, Cadena nombreDirectorio){
    while(cadenaDirectorios->sH != NULL){
        cadenaDirectorios = cadenaDirectorios->sH;
        if (cadenaDirectorios->nombre == nombreDirectorio){
            return true;
        }
    }
    return false;
}

// Toma parte de la ruta (nombre de un directorio) ingresada por el usuario, para analizarla correctamente.

Cadena dividirNombreCD(Cadena ruta, int & cont){
    Cadena sigDir;
    cont++;
    while(ruta[cont] != '/' && cont < ruta.length()){
        sigDir += ruta[cont];
        cont++;
    }
    return sigDir;
}

/*
Permite mover el puntero actual a los distintos directorios del sistema (RAIZ lleva a la RAIZ, el un ruta lleva al directorio especificado
y .. permite retroceder al directorio padre).
*/

TipoRet CD (Sistema & s, Cadena nombreDirectorio){
    if(nombreDirectorio == ".."){
        if(s->actual->nombre == "RAIZ"){
            cout << "No existe el directorio al que se pretende ir" << endl;
            return ERROR;
        }
        s->actual = s->actual->padre;
        return OK;
    }
    if(nombreDirectorio[0] == '/'){
        dir aux = s->RAIZ;
        int cont = 0;
        bool casoRaiz = true;
        while(cont < nombreDirectorio.length()){
            Cadena sigDir = dividirNombreCD(nombreDirectorio, cont);
            if(!casoRaiz){
                if(nombreDirectorioExistente(aux->pH, sigDir)){
                    aux = aux->pH;
                    while(aux->nombre != sigDir){
                        aux = aux->sH;
                    }
                } else {
                    cout << "No existe el directorio al que se pretende ir" << endl;
                    return ERROR;
                }
            }
            if(sigDir != "RAIZ" && casoRaiz){
                cout << "No existe el directorio al que se pretende ir" << endl;
                return ERROR;
            }
            casoRaiz = false;
        }
        s->actual = aux;
        return OK;
    }
    if(nombreDirectorio == "RAIZ"){
        s->actual = s->RAIZ;
        return OK;
    }
    if(nombreDirectorioExistente(s->actual->pH, nombreDirectorio)){
        s->actual = s->actual->pH;
        while(s->actual->nombre != nombreDirectorio){
            s->actual = s->actual->sH;
        }
        return OK;
    }
    cout << "No existe el directorio al que se pretende ir" << endl;
    return ERROR;
}

// MKDIR

// Devuelve un puntero al espacio anterior del dir a colocar, en orden alfabético

dir buscarEspacioDirAlfabeticamente(dir cadenaDirectorios, Cadena nombreDirectorio){
    int cont = 0;
    while(cont < nombreDirectorio.length() && cadenaDirectorios->sH != NULL){
        if(cadenaDirectorios->sH->nombre[cont] < nombreDirectorio[cont]){
            cadenaDirectorios = cadenaDirectorios->sH;
            cont = 0;
        }
        else if(cadenaDirectorios->sH->nombre[cont] > nombreDirectorio[cont]){
            return cadenaDirectorios;
        } else {
            cont++;
        }
    }
    return cadenaDirectorios;
}

// Crea una carpeta con el nombre pasado por parámetro

TipoRet MKDIR (Sistema &s, Cadena nombreDirectorio){
    if(nombreDirectorio == "RAIZ"){
        cout << "El nombre del directorio no puede ser RAIZ" << endl;
        return ERROR;
    }
    if(nombreDirectorioExistente(s->actual->pH, nombreDirectorio)){
        cout << "El directorio actual ya contiene un subdirectorio con ese nombre" << endl;
        return ERROR;
    }
    if (nombreDirectorio == "NULL"){
        cout << "El nombre del directorio no puede ser NULL" << endl;
        return ERROR;
    }

    dir espacioParaColocar = buscarEspacioDirAlfabeticamente(s->actual->pH, nombreDirectorio);
    
    dir nuevo = new directorio;
    nuevo->nombre = nombreDirectorio;
    nuevo->file = crearArchivoDummy();
    nuevo->pH = crearDirectorioDummy();
    nuevo->padre = s->actual;
    nuevo->pH->padre = nuevo;

    if(espacioParaColocar->sH == NULL){
        nuevo->sH = NULL;
        espacioParaColocar->sH = nuevo;
    } else {
        nuevo->sH = espacioParaColocar->sH;
        espacioParaColocar->sH = nuevo;
    }

    return OK;
}

// RMDIR

// Elimina una carpeta

TipoRet RMDIR (Sistema &s, Cadena nombreDirectorio){
    dir aux = s->actual->pH;
    if(nombreDirectorioExistente(aux, nombreDirectorio)){
        while(aux->sH->nombre != nombreDirectorio){
            aux = aux->sH;
        }
    } else {
        cout << "No existe el directorio que se pretende eliminar" << endl;
        return ERROR;
    }
    dir borrar = aux->sH;
    aux->sH = borrar->sH;
    borrarHijos(borrar);
    return OK;
}

// MOVE

// Devuelve un puntero al directorio destino

dir buscarDirDestino(Sistema s, Cadena directorioDestino){
    if(directorioDestino[0] == '/'){
        dir aux = s->RAIZ;
        int cont = 0;
        bool casoRaiz = true;
        while(cont < directorioDestino.length()){
            Cadena sigDir = dividirNombreCD(directorioDestino, cont);
            if(!casoRaiz){
                if(nombreDirectorioExistente(aux->pH, sigDir)){
                    aux = aux->pH;
                    while(aux->nombre != sigDir){
                        aux = aux->sH;
                    }
                } else {
                    return NULL;
                }
            }
            if(sigDir != "RAIZ" && casoRaiz){
                return NULL;
            }
            casoRaiz = false;
        }
        return aux;
    } else {
        cout << "La ruta debe ser válida" << endl;
        return NULL;
    }
}

// Devuelve true si el subdirectorio es hijo/nieto/etc del directorio actual

bool esDirectorioPadre(dir actual, dir subdirectorio){
    if (subdirectorio == NULL){
        return false;
    }
    if (subdirectorio->padre == actual){
        return true;
    }
    return esDirectorioPadre(actual, subdirectorio->padre) || false;
}

// Devuelve un puntero a la posición anterior al directorio que se busca mover

dir buscarAnteriorMOVEDir(dir cadenaDirectorios, Cadena nombreDir){
    while(cadenaDirectorios->sH != NULL && cadenaDirectorios->sH->nombre != nombreDir){
        cadenaDirectorios = cadenaDirectorios->sH;
    }
    return cadenaDirectorios;
}

// Devuelve un puntero a la posición anterior al archivo que se busca mover

archivos buscarAnteriorMOVEFile(archivos cadenaArchivos, Cadena nombreArch, Cadena extension){
    while(cadenaArchivos->sig != NULL && (cadenaArchivos->sig->nombre != nombreArch && cadenaArchivos->sig->extension != extension)){
        cadenaArchivos = cadenaArchivos->sig;
    }
    return cadenaArchivos;
}

// Devuelve true si existe el directorio en la cadena especificada

bool existeDir(dir cadenaDirectorios, Cadena nombre){
    while(cadenaDirectorios->sH != NULL && cadenaDirectorios->nombre != nombre){
        cadenaDirectorios = cadenaDirectorios->sH;
    }
    if(cadenaDirectorios->nombre == nombre){
        return true;
    }
    return false;
}

// Devuelve true si existe el archivo en la cadena especificada

bool existeArchAnt(archivos & actual, Cadena nombre, Cadena extension){
    while(actual->sig != NULL && (actual->sig->nombre != nombre && actual->sig->extension != extension)){
        actual = actual->sig;
    }
    if(actual->sig->nombre == nombre && actual->sig->extension == extension){
        return true;
    }
    return false;
}

// Mueve un directorio o carpeta a una nueva ruta especificada

TipoRet MOVE (Sistema &s, Cadena nombre, Cadena directorioDestino){
    dir destino = buscarDirDestino(s, directorioDestino);
    if(destino == NULL){
        cout << "No existe el directorio al que se pretende ir" << endl;
        return ERROR;
    }
    if(destino == s->actual){
        cout << "El directorio destino es igual al directorio actual" << endl;
        return ERROR;
    }
    if(esDirectorioPadre(s->actual, destino)){
        cout << "El directorio destino es un subdirectorio del directorio padre" << endl;
        return ERROR;
    }
    if(tomarExtension(nombre) == ""){
        if(!existeDir(s->actual->pH, nombre)){
            cout << "No existe el directorio que se pretende mover" << endl;
            return ERROR;
        }
        dir anteriorMOVE = buscarAnteriorMOVEDir(s->actual->pH, nombre);
        dir espacioDir;
        if(existeDir(destino->pH, nombre)){
            dir aux = destino->pH;
            while(aux->sH->nombre != nombre){
                aux = aux->sH;
            }
            espacioDir = aux;
            dir borrar = espacioDir->sH;
            dir MOVE = anteriorMOVE->sH;
            anteriorMOVE->sH = MOVE->sH;
            MOVE->sH = borrar->sH;
            espacioDir->sH = MOVE;
            borrarHijos(borrar);
            return OK;
        } else {
            espacioDir = buscarEspacioDirAlfabeticamente(destino->pH, nombre);
        }
        if(espacioDir->sH == NULL){
            espacioDir->sH = anteriorMOVE->sH;
            anteriorMOVE->sH = anteriorMOVE->sH->sH;
            espacioDir->sH->sH = NULL;
        } else {
            dir MOVE = anteriorMOVE->sH;
            anteriorMOVE->sH = MOVE->sH;
            MOVE->sH = espacioDir->sH;
            espacioDir->sH = MOVE;
        }
        return OK;
    } else {
        archivos aux;
        if(!existeArch(aux, s, tomarNombre(nombre), tomarExtension(nombre))){
            cout << "No existe el archivo que se pretende mover" << endl;
            return ERROR;
        }
        archivos anteriorMOVE = buscarAnteriorMOVEFile(s->actual->file, tomarNombre(nombre), tomarExtension(nombre));
        archivos espacioArch;
        if (existeArchAnt(destino->file, tomarNombre(nombre), tomarExtension(nombre))){
            espacioArch = destino->file;
            archivos borrar = espacioArch->sig;
            archivos MOVE = anteriorMOVE->sig;
            anteriorMOVE->sig = MOVE->sig;
            MOVE->sig = borrar->sig;
            espacioArch->sig = MOVE;
            delete borrar;
            return OK;
        } else {
            espacioArch = buscarEspacioArchAlfabeticamente(destino->file, tomarNombre(nombre), tomarExtension(nombre));
        }
        if(espacioArch->sig == NULL){
            espacioArch->sig = anteriorMOVE->sig;
            anteriorMOVE->sig = anteriorMOVE->sig->sig;
            espacioArch->sig->sig = NULL;
        } else {
            archivos MOVE = anteriorMOVE->sig;
            anteriorMOVE->sig = MOVE->sig;
            MOVE->sig = espacioArch->sig;
            espacioArch->sig = MOVE;
        }
        return OK;
    }
}

// DIR

// Lista una lista de archivos

void leerArchivos(archivos archivosALeer, Cadena ruta){
    if(archivosALeer == NULL){
        return;
    }
    if(archivosALeer->nombre != "NULL"){
        cout << ruta << "/" << archivosALeer->nombre << "." << archivosALeer->extension << endl;
    }
    if(archivosALeer->sig != NULL){
        leerArchivos(archivosALeer->sig, ruta);
    }
}

// Lista una lista de directorios junto a sus archivos

void leerCarpeta(dir carpetaALeer, Cadena ruta){
    if(carpetaALeer == NULL){
        return;
    }
    if(carpetaALeer->nombre != "NULL"){
        Cadena rutaConHermano = ruta + "/" + carpetaALeer->nombre;
        cout << rutaConHermano << endl;
        leerArchivos(carpetaALeer->file, rutaConHermano);
        if(carpetaALeer->pH != NULL){
            leerCarpeta(carpetaALeer->pH, rutaConHermano);
        }
    }
    if(carpetaALeer->sH != NULL){
        leerCarpeta(carpetaALeer->sH, ruta);
    }
}

// Devuelve la ruta actual en el árbol de directorios

Cadena obtenerRutaActual(dir actual, Cadena ruta){
    if (actual == NULL){
        return ruta;
    }
    Cadena nombreActual = "/" + actual->nombre;
    ruta = nombreActual + ruta;
    ruta = obtenerRutaActual(actual->padre, ruta);
    return ruta;
}

// Lista los archios y directorios de la carpeta actual

void obtenerContenido(dir actual, Cadena ruta){
    cout << ruta << endl;
    if (actual->file->sig == NULL && actual->pH->sH == NULL){
        cout << "El directorio está vacío" << endl;
        return;
    }
    archivos aux = actual->file;
    while (aux->sig != NULL){
        aux = aux->sig;
        cout << aux->nombre << "." << aux->extension;
        if (aux->escritura){
            cout << "     " << "Lectura/Escritura" << endl;
        } else {
            cout << "     " << "Lectura" << endl;
        }
    }
    actual = actual->pH;
    while (actual->sH != NULL){
        actual = actual->sH;
        cout << actual->nombre << endl;
    }
}

// Lista archivos y directorios

TipoRet DIR (Sistema &s, Cadena parametro){
    Cadena ruta;
    if(parametro == "/S"){
        leerCarpeta(s->RAIZ, ruta);
        return OK;
    }
    ruta = obtenerRutaActual(s->actual, ruta);
    obtenerContenido(s->actual, ruta);
    return OK;
}

// MDFY

// Busca el texto pasado por parámetro en un string, y devuelve el lugar de su primera letra

int buscarTexto(Cadena contenido, Cadena textoAModificar){
    int mover = 0;
    int i = 0;
    for(i; i < contenido.length(); i++){
        if(contenido[i] == textoAModificar[0]){
            mover = i;
            for(int j = 0; j < textoAModificar.length(); j++){
                if(contenido[mover] != textoAModificar[j]){
                    break;
                }
                if(j == (textoAModificar.length()-1)){
                    return i;
                }
                mover++;
            }
        }
    }
    return i+1;
}

// Desplaza el texto "espacios" lugares hacia la izquierda

Cadena desplazarTexto(Cadena contenido, int posicion, int espacios){
    int i = posicion;
    for (i; i < contenido.length(); i++){
        if ((i + espacios) < contenido.length()){
            contenido[i] = contenido[i+espacios];
        } else {
            break;
        }
    }
    for (i; i < contenido.length(); i++){
        contenido[i] = ' ';
    }
    return contenido;
}

// Devuelve la cadena de texto introducida por parámetro por una modificada en el lugar posicion por el nuevo texto

Cadena obtenerTexto(Cadena contenido, int posicion, Cadena textoAModificar, Cadena nuevoTexto){
    Cadena sig;
    Cadena ant;
    for(int i = posicion; i < contenido.length(); i++){
        sig += contenido[i];
    }
    for(int j = 0; j < posicion-textoAModificar.length(); j++){
        ant += contenido[j];
    }
    contenido = ant + nuevoTexto + sig;
    Cadena contenidoFinal;
    for (int i = 0; i < TEXTO_MAX; i++){
        contenidoFinal += contenido[i];
    }
    return contenidoFinal;
}

// Cambia el texto a modificar por el nuevo texto

Cadena modificarContenido(Cadena contenido, Cadena textoAModificar, Cadena nuevoTexto){
    Cadena copia = contenido;
    int cont = 0;
    int i = buscarTexto(contenido, textoAModificar);
    if(i == 23){
        return copia;
    }
    if(textoAModificar.length() == nuevoTexto.length()){
        for (i; i < contenido.length(); i++){
            if (cont < nuevoTexto.length()){
                contenido[i] = nuevoTexto[cont];
                cont++;
            } else {
                return contenido;
            }
        }
    }
    if(textoAModificar.length() > nuevoTexto.length()){
        int posicion = i + nuevoTexto.length();
        for (i; i < contenido.length(); i++){
            if (cont < nuevoTexto.length()){
                contenido[i] = nuevoTexto[cont];
                cont++;
            }
        }
        contenido = desplazarTexto(contenido, posicion, (textoAModificar.length() - nuevoTexto.length()));
        return contenido;
    }
    if(textoAModificar.length() < nuevoTexto.length()){
        int posicion = i + textoAModificar.length();
        contenido = obtenerTexto(contenido, posicion, textoAModificar, nuevoTexto);
        return contenido;
    }
    return contenido;
}

// Modifica el texto de un archivo por otro nuevo pasado por parámetro

TipoRet MDFY(Sistema & s, Cadena archivo, Cadena textoAModificar, Cadena nuevoTexto){
    archivos buscar;
    if(existeArch(buscar, s, tomarNombre(archivo), tomarExtension(archivo))){
        Cadena nuevoContenido = modificarContenido(buscar->contenido, textoAModificar, nuevoTexto);
        if (nuevoContenido == buscar->contenido){
            cout << "No se encontró el texto especificado dentro del archivo o es idéntico al anterior" << endl;
            return ERROR;
        }
        buscar->contenido = nuevoContenido;
        return OK;
    } else {
        cout << "No existe el archivo especificado en el directorio actual" << endl;
        return ERROR;
    }
}

// REX

// Lee el nombre de todos los archivos que coincidan en el nombre de extensión pasado por parametro

void leerArchivosExtension(dir actual, archivos archivosALeer, Cadena extension){
    if(archivosALeer == NULL){
        return;
    }
    string ruta;
    if(archivosALeer->nombre != "NULL" && archivosALeer->extension == extension){
        cout << obtenerRutaActual(actual, ruta) << "/" << archivosALeer->nombre << "." << archivosALeer->extension << endl;
    }
    if(archivosALeer->sig != NULL){
        leerArchivosExtension(actual, archivosALeer->sig, extension);
    }
}

// Lista todos los archivos, en profundidad, con el nombre de extensión pasado por parámetro

TipoRet REX(Sistema s, dir actual, Cadena extension){
    if(actual == NULL){
        return OK;
    }
    leerArchivosExtension(actual, actual->file, extension);
    if(s->actual != actual){
        REX(s, actual->sH, extension);
    }
    REX(s, actual->pH->sH, extension);
    return OK;
}

// CREATEUSR

// Crea un usuario

TipoRet CREATEUSR(Sistema & s, string nombreUsuario){
    if(nombreUsuario == "NULL"){
        cout << "El nombre de usuario no puede ser NULL" << endl;
        return ERROR;
    }
    users aux = s->usuarios;
    while(aux->sig != NULL){
        aux = aux->sig;
        if (nombreUsuario == aux->nombre){
            cout << "Ya existe un usuario con ese nombre" << endl;
            return ERROR;
        }
    }
    users nuevoUsuario = new user;
    nuevoUsuario->nombre = nombreUsuario;
    nuevoUsuario->sig = NULL;
    aux->sig = nuevoUsuario;
    s->usuarioActual = nuevoUsuario;
    cout << "El usuario " << nombreUsuario << " ha sido creado con éxito" << endl;
    return OK;
}

// DELUSR

// Elimina un usuario

TipoRet DELUSR(Sistema & s, string nombreUsuario){
    users aux = s->usuarios;
    while(aux->sig != NULL && aux->sig->nombre != nombreUsuario){
        aux = aux->sig;
    }
    if(aux->sig == NULL){
        cout << "No se encontró el usuario a borrar" << endl;
        return ERROR;
    }
    if(s->usuarioActual->nombre == nombreUsuario){
        s->usuarioActual = s->usuarios;
        cout << "Se eliminó el usuario en uso, deberás cambiar a un nuevo usuario" << endl;
    }
    users temp = aux->sig;
    aux->sig = temp->sig;
    delete temp;
    cout << "El usuario " << nombreUsuario << " se eliminó exitosamente" << endl;
    return OK;
}

// CHUSR

// Permite cambiar a un usuario existente

TipoRet CHUSR(Sistema & s, string nombreUsuario){
    users aux = s->usuarios->sig;
    while(aux != NULL && aux->nombre != nombreUsuario){
        aux = aux->sig;
    }
    if(aux == NULL){
        cout << "No se encontró el usuario a cambiar" << endl;
        return ERROR;
    }
    s->usuarioActual = aux;
    cout << "Se cambió al usuario " << nombreUsuario << " exitosamente" << endl;
    return OK;
}

// ANÁLISIS DE COMANDO

// Didive el comando especificado

void dividirComando(Cadena comandoEntero, int & cont, Cadena & Cambio){
    Cambio = "";
    while(comandoEntero[cont] != ' ' && cont < comandoEntero.length()){
        if(comandoEntero[cont] == '"'){
            cont++;
            while(cont < (comandoEntero.length()-1)){
                Cambio += comandoEntero[cont];
                cont++;
            }
            return;
        }
        Cambio += comandoEntero[cont];
        cont++;
    }
    cont++;
}

// Transforma un string de números a un entero

int convertirAInt(Cadena parametro){
    int numero = 0;
    for(int i = 0; i < parametro.length(); i++){
        numero += (parametro[i]-48)*pow(10, (parametro.length()-(1+i)));
    }
    return numero;
}

// Analiza el comando introducido para hacer un correcto llamado a la función especificada

void analizarComando(Cadena comando, Cadena parametro1, Cadena parametro2, Cadena parametro3, Sistema & MAIN){
    int cont = 0;
    if (comando == "CREARSISTEMA") {
        if(parametro1 == "" && parametro2 == "" && parametro3 == ""){
            CREARSISTEMA(MAIN);
            return;
        } else {
            cout << "El comando CREARSISTEMA no toma parámetros" << endl;
            return;
        }
    }
    if(comando == "DESTRUIRSISTEMA"){
        if(parametro1 == "" && parametro2 == "" && parametro3 == ""){
            DESTRUIRSISTEMA(MAIN);
            return;    
        } else {
            cout << "El comando DESTRUIRSISTEM no toma parámetros" << endl;
            return;
        }  
    }
    if (comando == "CREATEFILE") {
        if (parametro1 != "" && parametro2 == "" && parametro3 == ""){
            if(MAIN->usuarioActual->nombre != "NULL"){
                CREATEFILE(MAIN, parametro1);
                return;
            } else {
                cout << "Debes crear un usuario" << endl;
            }
        } else {
            cout << "El comando CREATEFILE toma solo un parámetro" << endl;
        }
    }
    if (comando == "DELETE") {
        if(parametro1 != "" && parametro2 == "" && parametro3 == ""){
            DELETE(MAIN, parametro1);
            return;
        } else {
            cout << "El comando DELETE toma solo un parámetro" << endl;
            return;
        }
    }
    if (comando == "ATTRIB") {
        if (parametro1 != "" && parametro2 != ""  && parametro3 == ""){
            ATTRIB(MAIN, parametro1, parametro2);
            return;
        } else {
            cout << "El comando ATTRIB necesita dos parámetros" << endl;
            return;
        }
    }
    if (comando == "IF") {
        if (parametro1 != "" && parametro2 != ""  && parametro3 == ""){
            IF(MAIN, parametro1, parametro2);
            return;
        } else {
            cout << "El comando IF necesita dos parámetros" << endl;
            return;
        }
    }
    if (comando == "DF") {
        if (parametro1 != "" && parametro2 != "" && parametro3 == ""){
            DF(MAIN, parametro1, convertirAInt(parametro2));
            return;
        } else {
            cout << "El comando DF necesita dos parámetros" << endl;
            return;
        }
    }
    if (comando == "TYPE") {
        if (parametro1 != "" && parametro2 == "" && parametro3 == ""){
            TYPE(MAIN, parametro1);
            return;
        } else {
            cout << "El comando TYPE toma solo un parámetro" << endl;
            return;
        }
    }
    if(comando == "CD"){
        if (parametro1 != "" && parametro2 == "" && parametro3 == ""){
            CD(MAIN, parametro1);
            return;
        } else {
            cout << "El comando CD toma solo un parámetro" << endl;
            return;
        }
    }
    if(comando == "MKDIR"){
        if (parametro1 != "" && parametro2 == "" && parametro3 == ""){
            MKDIR(MAIN, parametro1);
            return;
        } else {
            cout << "El comando MKDIR toma solo un parámetro" << endl;
            return;
        }
    }
    if(comando == "RMDIR"){
        if (parametro1 != "" && parametro2 == "" && parametro3 == ""){
            RMDIR(MAIN, parametro1);
            return;
        } else {
            cout << "El comando RMDIR toma solo un parámetro" << endl;
            return;
        }
    }
    if(comando == "MOVE"){
        if(parametro1 != "" && parametro2 != "" && parametro3 == ""){
            MOVE(MAIN, parametro1, parametro2);
            return;
        } else {
            cout << "El comando MOVE necesita dos parámetros" << endl;
            return;
        }
    }
    if(comando == "DIR"){
        if (parametro2 == "" && parametro3 == ""){
            DIR(MAIN, parametro1);
            return;
        } else {
            cout << "El comando DIR puede tomar solo un parámetro" << endl;
            return;
        }
    }
    if(comando == "MDFY"){
        if (parametro1 != "" && parametro2 != "" && parametro3 != ""){
            MDFY(MAIN, parametro1, parametro2, parametro3);
            return;
        } else {
            cout << "El comando MDFY necesita tres parámetros" << endl;
            return;
        }
    }
    if(comando == "REX"){
        if (parametro1 != "" && parametro2 == "" && parametro3 == ""){
            REX(MAIN, MAIN->actual, parametro1);
            return;
        } else {
            cout << "El comando REX toma solo un parámetro" << endl;
            return;
        }
    }
    if(comando == "CREATEUSR"){
        if (parametro1 != "" && parametro2 == "" && parametro3 == "") {
            CREATEUSR(MAIN, parametro1);
            return;
        } else  {
            cout << "El comando CREATEUSR toma solo un parámetro" << endl;
            return;
        }
    }
    if(comando == "DELUSR"){
        if (parametro1 != "" && parametro2 == "" && parametro3 == "") {
            DELUSR(MAIN, parametro1);
            return;
        } else {
            cout << "El comando DELUSR toma solo un parámetro" << endl;
            return;
        }
    }
    if(comando == "CHUSR"){
        if (parametro1 != "" && parametro2 == "" && parametro3 == "") {
            CHUSR(MAIN, parametro1);
            return;
        } else {
            cout << "El comando CHUSR toma solo un parámetro" << endl;
            return;
        }
    }
}

// Es el bucle que permite ejecutar múltiples comandos uno detrás de otro

void manejarComando(Sistema & MAIN){
    Cadena comandoEntero;
    Cadena comando;
    Cadena parametro1;
    Cadena parametro2;
    Cadena parametro3;
    Cadena ruta;
    int cont;
    do {
    getline(cin, comandoEntero);
    if(comandoEntero != "EXIT"){
        cont = 0;
        dividirComando(comandoEntero, cont, comando);
        dividirComando(comandoEntero, cont, parametro1);
        dividirComando(comandoEntero, cont, parametro2);
        dividirComando(comandoEntero, cont, parametro3);
        analizarComando(comando, parametro1, parametro2, parametro3, MAIN);
        if(comando != "DESTRUIRSISTEMA"){
            cout << obtenerRutaActual(MAIN->actual, ruta) << " > ";
        }
    }
    } while (comandoEntero != "EXIT");
}

// MAIN

int main(){
    Sistema s = NULL;

    manejarComando(s);
}
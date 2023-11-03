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

struct _sistema{
    Cadena nombre = "SISTEMA";
    dir RAIZ;
    dir actual;
};

typedef _sistema * Sistema;

// FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES 

// CREATEFILE

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

Cadena tomarNombre(Cadena nombreArchivo){
    Cadena nombre;
    for (int i = 0; i < nombreArchivo.length(); i++){
        if (nombreArchivo[i] != '.'){ 
            nombre += nombreArchivo[i];
        } else {
            return nombre;
        }
    }
}

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

TipoRet CREATEFILE (Sistema & s, Cadena nombreArchivo){
    archivos puntVerificar = NULL;
    if(!existeArch(puntVerificar, s, tomarNombre(nombreArchivo), tomarExtension(nombreArchivo)) && nombreArchivo != "RAIZ"){

        archivos puntFinal = buscarEspacioArchAlfabeticamente(s->actual->file, tomarNombre(nombreArchivo), tomarExtension(nombreArchivo));

        archivos nuevoArchivo = new archivo;
        nuevoArchivo->nombre = tomarNombre(nombreArchivo);
        nuevoArchivo->extension = tomarExtension(nombreArchivo);

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

TipoRet IF (Sistema & s, Cadena nombreArchivo, Cadena texto){
    archivos aux = NULL;
    if(existeArch(aux, s, tomarNombre(nombreArchivo), tomarExtension(nombreArchivo))){
        if (!aux->escritura){
            cout << "El archivo es de sólo lectura" << endl;
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

TipoRet DF (Sistema & s, Cadena nombreArchivo, int k){
    archivos aux = NULL;
    if(existeArch(aux, s, tomarNombre(nombreArchivo), tomarExtension(nombreArchivo))){
        if (!aux->escritura){
            cout << "El archivo es de sólo lectura" << endl;
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

dir crearDirectorioDummy(){
    dir dummy = new directorio;
    dummy->nombre = "NULL";
    dummy->file = NULL;
    dummy->pH = NULL;
    dummy->sH = NULL;
    return dummy;
}

archivos crearArchivoDummy(){
    archivos dummy = new archivo;
    dummy->nombre = "NULL";
    dummy->sig = NULL;
    return dummy;
}

TipoRet CREARSISTEMA(Sistema & s){
    s = new _sistema;
    
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

void borrarArchivos(archivos archivosABorrar){
    if(archivosABorrar == NULL){
        return;
    }
    if(archivosABorrar->sig != NULL){
        borrarArchivos(archivosABorrar->sig);
    }
    delete archivosABorrar;
}

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

TipoRet DESTRUIRSISTEMA(Sistema & s){
    borrarHijos(s->RAIZ);
    delete s;
    return OK;
}

// CD

bool nombreDirectorioExistente(dir cadenaDirectorios, Cadena nombreDirectorio){
    while(cadenaDirectorios->sH != NULL){
        cadenaDirectorios = cadenaDirectorios->sH;
        if (cadenaDirectorios->nombre == nombreDirectorio){
            return true;
        }
    }
    return false;
}

Cadena dividirNombreCD(Cadena ruta, int & cont){
    Cadena sigDir;
    cont++;
    while(ruta[cont] != '/' && cont < ruta.length()){
        sigDir += ruta[cont];
        cont++;
    }
    return sigDir;
}

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

bool esDirectorioPadre(dir actual, dir subdirectorio){
    if (subdirectorio == NULL){
        return false;
    }
    if (subdirectorio->padre == actual){
        return true;
    }
    return esDirectorioPadre(actual, subdirectorio->padre) || false;
}

dir buscarAnteriorMOVEDir(dir cadenaDirectorios, Cadena nombreDir){
    while(cadenaDirectorios->sH != NULL && cadenaDirectorios->sH->nombre != nombreDir){
        cadenaDirectorios = cadenaDirectorios->sH;
    }
    return cadenaDirectorios;
}

archivos buscarAnteriorMOVEFile(archivos cadenaArchivos, Cadena nombreArch, Cadena extension){
    while(cadenaArchivos->sig != NULL && (cadenaArchivos->sig->nombre != nombreArch && cadenaArchivos->sig->extension != extension)){
        cadenaArchivos = cadenaArchivos->sig;
    }
    return cadenaArchivos;
}

bool existeDir(dir cadenaDirectorios, Cadena nombre){
    while(cadenaDirectorios->sH != NULL && cadenaDirectorios->nombre != nombre){
        cadenaDirectorios = cadenaDirectorios->sH;
    }
    if(cadenaDirectorios->nombre == nombre){
        return true;
    }
    return false;
}

bool existeArchAnt(archivos & actual, Cadena nombre, Cadena extension){
    while(actual->sig != NULL && (actual->sig->nombre != nombre && actual->sig->extension != extension)){
        actual = actual->sig;
    }
    if(actual->sig->nombre == nombre && actual->sig->extension == extension){
        return true;
    }
    return false;
}

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

Cadena obtenerRutaActual(dir actual, Cadena ruta){
    if (actual == NULL){
        return ruta;
    }
    Cadena nombreActual = "/" + actual->nombre;
    ruta = nombreActual + ruta;
    ruta = obtenerRutaActual(actual->padre, ruta);
    return ruta;
}

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
        contenido = desplazarTexto(contenido, posicion, ((textoAModificar.length()-nuevoTexto.length())-1));
        return contenido;
    }

    return contenido;
}

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

TipoRet REX(dir actual, Cadena ruta){
    if(actual->file == NULL){
        return;
    }
    while()
}

// CREATEUSR

TipoRet CREATEUSR(Sistema & s, string nombreUsuario){
    
}

// DELUSR

TipoRet DELUSR(Sistema & s, string nombreUsuario){
    
}

// ANÁLISIS DE COMANDO

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

int convertirAInt(Cadena parametro){
    int numero = 0;
    for(int i = 0; i < parametro.length(); i++){
        numero += (parametro[i]-48)*pow(10, (parametro.length()-(1+i)));
    }
    return numero;
}

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
            CREATEFILE(MAIN, parametro1);
            return;
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
}

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
    }
    } while (comandoEntero != "EXIT");
}

// MAIN

int main(){
    Sistema s = NULL;

    manejarComando(s);
}
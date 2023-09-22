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
    string nombre;
    Cadena extension;
    bool escritura = true;
    string contenido;
    archivo * sig;
    archivo * ant;
};

typedef archivo * archivos;

struct directorio {
    string nombre;
    archivos file;
    directorio * sH;
    directorio * pH;
};

typedef directorio * dir;

struct _sistema{
    string nombre = "SISTEMA";
    dir RAIZ;
};

typedef _sistema* Sistema;

// Definición del main

Sistema MAIN = NULL;

// FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES 

// CREATEFILE

void moverPunteroAlFinal(archivos & aux, Sistema s){
    aux = s->RAIZ->file;
    while(aux->sig != NULL){
        aux = aux->sig;
    }
}

bool existeArch(archivos & aux, Sistema s, Cadena nombre, Cadena extension){
    aux = s->RAIZ->file;
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
}

TipoRet CREATEFILE (Sistema & s, Cadena nombreArchivo){
    archivos puntVerificar = NULL;
    if(!existeArch(puntVerificar, s, tomarNombre(nombreArchivo), tomarExtension(nombreArchivo)) && nombreArchivo != "RAIZ"){

        archivos puntFinal = NULL;
        moverPunteroAlFinal(puntFinal, s);

        archivos nuevoArchivo = new archivo;
        nuevoArchivo->nombre = tomarNombre(nombreArchivo);
        nuevoArchivo->extension = tomarExtension(nombreArchivo);
        nuevoArchivo->sig = NULL;

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
        archivos aux = s->RAIZ->file;
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
        } else {
            aux->escritura = false;
            return OK;
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
        cout << resultado << endl;
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

TipoRet CREARSISTEMA(Sistema & s){
    s = new _sistema;
    s->RAIZ = new directorio;
    s->RAIZ->nombre = "RAIZ";
    s->RAIZ->file = new archivo;
    s->RAIZ->file->nombre = "NULL";
    s->RAIZ->file->sig = NULL;
    return OK;
}

// DESTRUIRSISTEMA

void borrarArchivos(archivos borrar){
    if (borrar->sig != NULL){
        borrarArchivos(borrar->sig);
    }
    delete borrar;
}

TipoRet DESTRUIRSISTEMA(Sistema & s){
    archivos borrar = s->RAIZ->file;
    borrarArchivos(borrar);
    delete s->RAIZ;
    delete s;
    return OK;
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

void analizarComando(Cadena comando, Cadena parametro1, Cadena parametro2){
    int cont = 0;
    if (comando == "CREARSISTEMA") {
        if(parametro1 == "" && parametro2 == ""){
            CREARSISTEMA(MAIN);
        } else {
            return;
        }
    }
    if(comando == "DESTRUIRSISTEMA"){
        DESTRUIRSISTEMA(MAIN);
        return;
    }
    if (comando == "CREATEFILE") {
        CREATEFILE(MAIN, parametro1);
        return;
    }
    if (comando == "DELETE") {
        DELETE(MAIN, parametro1);
        return;
    }
    if (comando == "ATTRIB") {
        ATTRIB(MAIN, parametro1, parametro2);
        return;
    }
    if (comando == "IF") {
        IF(MAIN, parametro1, parametro2);
        return;
    }
    if (comando == "DF") {
        DF(MAIN, parametro1, convertirAInt(parametro2));
        return;
    }
    if (comando == "TYPE") {
        TYPE(MAIN, parametro1);
        return;
    }
}

void manejarComando(){
    Cadena comandoEntero;
    Cadena comando;
    Cadena parametro1;
    Cadena parametro2;
    int cont;
    do {
    getline(cin, comandoEntero);
    if(comandoEntero != "EXIT"){
        cont = 0;
        dividirComando(comandoEntero, cont, comando);
        dividirComando(comandoEntero, cont, parametro1);
        dividirComando(comandoEntero, cont, parametro2);
        analizarComando(comando, parametro1, parametro2);
    }
    } while (comandoEntero != "EXIT");
}

// MAIN

// leerExistentes sirve para verificar que el código esté funcionando como es debido

void leerExistentes(Sistema s){
    cout << s->nombre << endl;
    cout << s->RAIZ->nombre << endl;
    while (s->RAIZ->file != NULL){
        cout << s->RAIZ->file->nombre << endl;
        cout << s->RAIZ->file->contenido << endl;
        s->RAIZ->file = s->RAIZ->file->sig;
    }
}

int main(){
    manejarComando();
    // leerExistentes(MAIN);
}
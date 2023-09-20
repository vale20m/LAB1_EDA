#include <iostream>
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
    archivo * sig = NULL;
    archivo * ant = NULL;
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
    string nombre;
    dir RAIZ;
    dir RAIZptr;
};

typedef _sistema* Sistema;

// Definición del main

Sistema MAIN = NULL;

// FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES 

// CREATEFILE

bool existeArch(Sistema s, Cadena nombre, Cadena extension){
    while(s->RAIZ->file->nombre != ""){
        if(s->RAIZ->file->nombre == nombre && s->RAIZ->file->extension == extension){
            return true;
        }
        s->RAIZ->file = s->RAIZ->file->sig;
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

archivos colocarPunteroAlFinal(Sistema s){
    archivos aux = NULL;
    while(s->RAIZ->file->nombre != ""){
        s->RAIZ->file = s->RAIZ->file->sig;
    }
    aux = s->RAIZ->file;
    return aux;
}

TipoRet CREATEFILE (Sistema & s, Cadena nombreArchivo){
    if(!existeArch(s, tomarNombre(nombreArchivo), tomarExtension(nombreArchivo)) && nombreArchivo != "RAIZ"){
        archivos aux = colocarPunteroAlFinal(s);

        aux->nombre = tomarNombre(nombreArchivo);
        cout << aux->nombre << endl;

        aux->extension = tomarExtension(nombreArchivo);
        cout << aux->extension << endl;

        aux->sig = new archivo;
        aux->sig->nombre = "";
        aux->sig->extension = "";
        
        cout << "Salí de createfile" << endl;
        return OK;
    } else {
        cout << "Ya existe un archivo con ese nombre completo en el directorio actual" << endl;
        return ERROR;
    }
}

// DELETE

TipoRet DELETE (Sistema & s, Cadena nombreArchivo){

}

// ATTRIB

TipoRet ATTRIB (Sistema & s, Cadena nombreArchivo, Cadena parametro){

}

// IF

TipoRet IF (Sistema & s, Cadena nombreArchivo, Cadena texto){

}

// DF

TipoRet DF (Sistema & s, Cadena nombreArchivo, int k){

}

// TYPE

TipoRet TYPE (Sistema & s, Cadena nombreArchivo){

}

// CREARSISTEMA

TipoRet CREARSISTEMA(Sistema & s){
    s = new _sistema;
    s->RAIZ = new directorio;
    s->RAIZ->nombre = "RAIZ";
    s->RAIZptr = s->RAIZ;
    s->RAIZ->file = new archivo;
    s->RAIZ->file->nombre = "";
    s->RAIZ->file->extension = "";
    cout << "Se ha creado exitosamente" << endl;
    return OK;
}

// DESTRUIRSISTEMA

TipoRet DESTRUIRSISTEMA(Sistema & s){

}

// ANÁLISIS DE COMANDO

void dividirComando(Cadena comandoEntero, int & cont, Cadena & Cambio){
    Cambio = "";
    while(comandoEntero[cont] != ' ' && cont < comandoEntero.length()){
        Cambio += comandoEntero[cont];
        cont++;
    }
    cont++;
}

void analizarComando(Cadena comando, Cadena parametro1, Cadena parametro2){
    int cont = 0;
    cout << comando << endl;
    cout << parametro1 << endl;
    if (comando == "CREARSISTEMA") {
        if(parametro1 == "" && parametro2 == ""){
            CREARSISTEMA(MAIN);
            cout << "Salí de crear sistema" << endl;
        } else {
            return;
        }
    }
    if(comando == "DESTRUIRSISTEMA"){
        return;
    }
    if (comando == "CREATEFILE") {
        CREATEFILE(MAIN, parametro1);
        return;
    }
    if (comando == "DELETE") {
        return;
    }
    if (comando == "ATTRIB") {
        return;
    }
    if (comando == "IF") {
        return;
    }
    if (comando == "DF") {
        return;
    }
    if (comando == "TYPE") {
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

int main(){
    manejarComando();
    cout << MAIN->RAIZ->file->nombre << endl;
    cout << MAIN->RAIZ->file->sig->nombre << endl;
}

/*
if (nombre == "RAIZ"){
    cout << "No se puede crear un directorio con el nombre 'RAIZ'";
    return ERROR;
}
*/

/*
if ()
*/

/* CREAR UN DIRECTORIO "RAIZ" EN SISTEMA
    _sistema sistema1;
    sistema1.RAIZ;
    sistema1.RAIZ.nombre = "JIJIJIJA";
    cout << sistema1.RAIZ.nombre;
    sistema1.RAIZ.sig = new root;
    sistema1.RAIZ.sig.nombre = "ELBANA";
    cout << sistema1.RAIZ->sig.nombre;
*/
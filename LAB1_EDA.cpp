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
    char extension[MAX_CHAR];
    bool escritura = true;
    string contenido = "";
};

struct root {
    string nombre = "RAIZ";
    archivo jamon;
    root * sig;
    root * origen;
};

struct _sistema{
    string nombre = "PEDRO";
    root RAIZ;
};

typedef _sistema* Sistema;

// Definición del main


// FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES FUNCIONES 

TipoRet CREATEFILE (Sistema & s, Cadena nombreArchivo){

}

TipoRet DELETE (Sistema & s, Cadena nombreArchivo){

}

TipoRet ATTRIB (Sistema & s, Cadena nombreArchivo, Cadena parametro){

}

TipoRet IF (Sistema & s, Cadena nombreArchivo, Cadena texto){

}

TipoRet DF (Sistema & s, Cadena nombreArchivo, int k){

}

TipoRet TYPE (Sistema & s, Cadena nombreArchivo){

}

void CREARSISTEMA(Sistema & s){
    cout << s->nombre;
    cout << s.nombre;
}

TipoRet DESTRUIRSISTEMA(Sistema & s){

}

/*
CREARSISTEMA = 1
DESTRUIRSISTEMA = 2
CREATEFILE = 3
DELETE = 4
ATTRIB = 5
IF = 6
DF = 7
TYPE = 8
*/

// && comandoEntero[cont] != '\0' && comandoEntero[cont] != '\n'

void dividirComando(Cadena comandoEntero, int & cont, Cadena & Cambio){
    while(comandoEntero[cont] != ' ' && cont < comandoEntero.length()){
        Cambio += comandoEntero[cont];
        cont++;
    }
    cont++;
}

void analizarComando(Cadena comando, Cadena parametro1, Cadena parametro2){
    int cont = 0;
    if (comando == "CREARSISTEMA") {
        if(parametro1 != "" && parametro2 != ""){
        } else {
            CREARSISTEMA(MAIN);
        }
    }
    if(comando == "DESTRUIRSISTEMA"){
        return;
    }
    if (comando == "CREATEFILE") {
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
    int cont = 0;
    getline(cin, comandoEntero);
    dividirComando(comandoEntero, cont, comando);
    dividirComando(comandoEntero, cont, parametro1);
    dividirComando(comandoEntero, cont, parametro2);
    analizarComando(comando, parametro1, parametro2);
}

int main(){
    Sistema MAIN = new _sistema;
    manejarComando();
    cout << "HoLA ESToY cAMbiANDO" << endl;
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
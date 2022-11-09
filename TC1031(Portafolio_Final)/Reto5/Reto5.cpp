/* 
Ximena Sánchez Padilla A01275072
Karla Valeria Mondragón Rosas A01025108
Reto 5
Una ip interna, que llamaremos A, la cual se comunica con algunas otras computadoras internas = mary.reto.com-- > 172.22.55.15
Algún sitio con nombre raro, denominaremos B = 4ftjzr4g5u8gawcsnjv1.com-- > 42.153.2.226
Un sitio web normal que tiene un volumen de trafico anómalo un día, el cual denominaremos C = reddit.com
*/
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
using namespace std;

class Record {
public:
    string fecha;
    string hora;
    string IPFuente;
    int puertoFuente;
    string nombreFuente;
    string IPDestino;
    int puertoDestino;
    string nombreDestino;

    Record(string Fecha, string Hora, string ipF, string pF, string nF, string ipD, string pD, string nD)
    {
        this->fecha = Fecha;
        this->hora = Hora;
        this->IPFuente = ipF;

        if (pF == "-") {
            this->puertoFuente = 0;
        }
        else {
            this->puertoFuente = stoi(pF);
        }

        this->nombreFuente = nF;
        this->IPDestino = ipD;

        if (pD == "-") {
            this->puertoDestino = 0;
        }
        else {
            this->puertoDestino = stoi(pD);
        }

        this->nombreDestino = nD;
    }

    void imprimirRecord() {
        cout << fecha << ": " << hora << ": " << IPFuente << ": " << puertoFuente << ": " << nombreFuente << ": " << IPDestino << ": " << puertoDestino << ": " << nombreDestino << endl;
    }
};

vector<Record> datos;
int cargarDatos(string path) {
    ifstream in;
    in.open(path);
    string line, parte;
    vector<string> partes;
    int r = 0;

    if (in) {
        r = 1;
        while (getline(in, line)) {
            if (line.find("\r") != line.npos) {
                line = line.substr(0, line.size() - 1);
            }
            istringstream inString(line);
            while (getline(inString, parte, ',')) {
                partes.push_back(parte);
            }
            Record r(partes[0], partes[1], partes[2], partes[3], partes[4], partes[5], partes[6], partes[7]);
            datos.push_back(r);
            partes.clear();
        }
        in.close();
    }
    else
        r = 0;
    return r;
}

template <class T>
class Graph {
    vector<vector<int>> matriz;
    int cantidad;

public:
    unordered_map<T, int> ids;
    Graph() {
        cantidad = 0;
    }

    void agregarNodo(T id) {
        if (ids.find(id) == ids.end()) {
            ids[id] = cantidad;
            vector<int> nuevaFila;
            for (int fila = 0; fila < cantidad; fila++) {
                matriz[fila].push_back(0);
                nuevaFila.push_back(0);
            }
            nuevaFila.push_back(0);
            matriz.push_back(nuevaFila);
            cantidad++;
        }
    }

    void agregarArcoDirigidoConPeso(T fuente, T destino, int peso) {
        if (ids.find(fuente) != ids.end() && ids.find(destino) != ids.end()) {
            matriz[ids[fuente]][ids[destino]] = peso;
        }
    }

    void agregarPeso(T fuente, T destino) {
        if (ids.find(fuente) != ids.end() && ids.find(destino) != ids.end()) {
            matriz[ids[fuente]][ids[destino]]++;
        }
        else {
            agregarArcoDirigidoConPeso(fuente, destino, 1);
        }
    }

    void agregarArcoConPeso(T fuente, T destino, int peso) {
        agregarArcoDirigidoConPeso(fuente, destino, peso);
        agregarArcoDirigidoConPeso(destino, fuente, peso);
    }

    void imprimir() {
        cout << "Ip : Id" << endl;
        for (auto nodo : ids) {
            cout << nodo.first << ": " << nodo.second << endl;
        }
        //int col=0;
        for (int col = 0; col < cantidad; col++) {
            for (int filas = 0; filas < cantidad; filas++) {
                cout << matriz[col][filas] << ", ";
            }
            cout << endl;
        }
    }
};

// 1 y 2 
Graph<string> crearGrafoIPs(string fecha) {
    Graph<string> conexiones;
    for (int i = 0; i < datos.size(); i++) { // Lectura de todos los registros del archivo
        // Contabiliza las conexiones de una ip hacia cualquier fuente
        if (datos[i].fecha == fecha && datos[i].nombreDestino.find("reto.com")!=string::npos) {
            conexiones.agregarNodo(datos[i].IPDestino);
            conexiones.agregarNodo(datos[i].IPFuente);
            conexiones.agregarPeso(datos[i].IPFuente, datos[i].IPDestino);
        }
    }
    return conexiones;
}

// 3 y 4
Graph<string> crearGrafoSW(string fecha, string ipd) {
    Graph<string> conexiones;
    int cuenta=0;
    for (int i = 0; i < datos.size(); i++) { // Lectura de todos los registros del archivo
        // Contabiliza las conexiones de ip hacia cualquier fuente
        if (datos[i].fecha == fecha && datos[i].IPDestino == ipd) {
            conexiones.agregarNodo(datos[i].nombreDestino);
            conexiones.agregarNodo(datos[i].nombreFuente);
            conexiones.agregarPeso(datos[i].nombreFuente, datos[i].nombreDestino);
            cuenta++;
        }
    }
    cout << "cantidad de conexiones: " << cuenta << endl;
    return conexiones;
}

int main() {
    if (cargarDatos("datosEquipo12.csv")) {
        vector<string> fechas;
        string ipda = "42.153.2.226";
        string ipdr = "169.167.12.139";
        cout << endl;

        for (int i = 1; i < datos.size(); i++) { // Lectura de todos los registros del archivo
            if (datos[i].fecha != datos[i - 1].fecha)
                fechas.push_back(datos[i].fecha);
        }

        // Punto 1 y 2
        cout << "------------------- Computadoras se han conectado a A por día mary.reto.com-- > 172.22.55.15 -------------------" << endl;
        for (auto c : fechas) {
            Graph<string> conexionesIps = crearGrafoIPs(c);
            cout << c << "-----------------------------------" << endl;
            conexionesIps.imprimir();
            cout << endl;
        }

        // Punto 3
        cout << "------------------- Computadoras se han conectado a B por día (4ftjzr4g5u8gawcsnjv1.com-- > 42.153.2.226) -------------------" << endl;
        for (auto c : fechas) {
            Graph<string> conexiones = crearGrafoSW(c, ipda);
            cout << c << "-----------------------------------" << endl;
            conexiones.imprimir();
            cout << endl;
        }
      
        // Punto 4
        cout << "------------------- Computadoras se han conectado a C por día (reddit.com) -------------------" << endl;
        for (auto c : fechas) {
            Graph<string> conexionesIps = crearGrafoSW(c, ipdr);
            cout << c << "-----------------------------------" << endl;
            conexionesIps.imprimir();
            cout << endl;
        }
    }
    else
        cout << "No se localizó el archivo" << endl;
    return 0;
}
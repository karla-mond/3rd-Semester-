//Ximena Sánchez Padilla A01275072
//Karla Valeria Mondragón Rosas A01025108
#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include <set>
#include <string>
#include <iostream>
#include <unordered_map>
#include <iostream>

using namespace std;


/// Árbol BST
template <class T>
class Nodo {
public:
	T value;
	string key;
	bool visitado;
	Nodo<T>* izq;
	Nodo<T>* der;

	Nodo() {
		value = NULL;
		visitado = false;
		izq = NULL;
		der = NULL;
		key = "";
	}

	Nodo(T val, string aux) {
		value = val;
		visitado = false;
		izq = NULL;
		der = NULL;
		key = aux;
	}
};

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
int cargarDatos(string path)
{
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

class ConexionesComputadora {
public:
    string ip;
    string domain;
    string fecha;
    vector<Record> conexionesSalientes;
    vector<Record> conexionesEntrantes;

    ConexionesComputadora() {
        ip = "";
        domain = "";
        fecha = "";
    }

    ConexionesComputadora(string Ip, string Fecha) {
        this->ip = Ip;
        this->fecha = Fecha;
        for (int i = 0; i < datos.size(); i++) {
            if (datos[i].fecha == Fecha) {
                // Solo agrega las conexiones de esa fecha
                if (datos[i].IPFuente == Ip) {
                    conexionesSalientes.push_back(datos[i]);
                    domain = datos[i].nombreFuente;
                }
                else if (datos[i].IPDestino == Ip && datos[i].nombreDestino != "-" && datos[i].nombreDestino.find("reto.com") == string::npos) {
                    conexionesEntrantes.push_back(datos[i]);
                    domain = datos[i].nombreDestino;
                }
            }
        }
    }

    ConexionesComputadora(string Ip) {
        this->ip = Ip;
        this->fecha = "";
        for (int i = 0; i < datos.size(); i++) {
            if (datos[i].IPFuente == Ip) {
                conexionesSalientes.push_back(datos[i]);
                domain = datos[i].nombreFuente;
            }
            else if (datos[i].IPDestino == Ip) {
                conexionesEntrantes.push_back(datos[i]);
                domain = datos[i].nombreDestino;
            }
        }
    }
};

/// PUNTO 1
unordered_map<string, int> conexionesPorDia(string fecha) {
    unordered_map<string, int> c;
    for (int i = 0; i < datos.size(); i++) { // Lectura de todos los registros del archivo
            if ((datos[i].nombreDestino != "-") && (datos[i].nombreDestino.find("reto.com") == string::npos)) { //  compara los dominios 
                if (c.find(datos[i].nombreDestino) == c.end()) {    // Verifica que aun no exista esa llave en el map
                    ConexionesComputadora conexiones(datos[i].IPDestino, fecha);
                    c[datos[i].nombreDestino] = conexiones.conexionesEntrantes.size(); // se añade el dominio y la cantidad de conexiones entrantes
                }
            }
    }
    return c; // se devuelve 
}

/// PUNTO 2
void top(int n, string fecha) {
	map<int, vector<string>> tree;
	unordered_map<string, int> cpd = conexionesPorDia(fecha);
	// Construye el árbol BSD
	for(pair<string, int> p: cpd) {
        tree[p.second].push_back(p.first);
	}
    cout << "Top " << n << " del número de conexiones en " << fecha << ": " << endl;
	for(auto it=tree.rbegin(); it!=tree.rend(); it++) {

        cout<< it->first << " : ";
        for(auto elem: it->second) {
            cout << elem << ", ";
            n--;
            if(n==0) return;
        }
        cout << endl;
    }
}


int main() {
    if (cargarDatos("datosEquipo12.csv")) {
        cout << "Datos en Registro: " << datos.size() << endl;
        cout << endl;

        // Ejemplo funcionamiento Parte 1
        cout << "Ejemplo funcionamiento PUNTO 1" << endl;
        cout << "Dominio : Cantidad de Conexiones entrantes" << endl;
        unordered_map<string, int> cpd = conexionesPorDia("14-8-2020");
        for (auto c : cpd) {
            if(c.second!=0)
                cout << c.first << " : " << c.second << endl;
        }
        cout << endl; 


		// Ejemplo Funcionamiento Parte 2
        cout << "Ejemplo funcionamiento PUNTO 2" << endl;
		top(5, "10-8-2020");
		cout << endl; 

		// Parte 3
        cout << "Punto 3" << endl;
        cout << "Cantidad de conexiones entrantes : Dominio" << endl;
        cout << endl;
		vector<string> fechas;
        // Obtiene fechas únicas y luego el top 5 de cada día
		fechas.push_back(datos[0].fecha);
		for (int i = 1; i < datos.size(); i++) { // Lectura de todos los registros del archivo
			if (datos[i].fecha != datos[i-1].fecha)
				fechas.push_back(datos[i].fecha);
		}
		for (auto c : fechas) {
			top(5, c);
            cout << endl;
		}
    }
    else {
        cout << "No se localizó el archivo" << endl;
    }
    return 0;
}
//Ximena Sánchez Padilla A01275072
//Karla Valeria Mondragón Rosas A01025108
#include <stack>
#include <queue>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
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
        string nombre;
        stack<Record> conexionesEntrantes;
        queue<Record> conexionesSalientes;

        ConexionesComputadora(){
            string Ip;
            cout << "-------- Generando un número entre 1 y 150 --------" << endl;
            //int num = 1+rand()%(151-1);
            cout << "-------- Generando IP interna --------" << endl;
            //Ip = "172.22.55." + to_string(num);
            Ip = "172.22.55.15"; // se utilizara esta IP que ya se sabe que existe para responder las preguntas
            ip = Ip;
        }
};

int main() {
     if (cargarDatos("datosEquipo12.csv")) {
        cout << "Datos en Registro: " << datos.size() << endl;

        // Inicializa la clase ConexionesComputadora
        // Usa la computadora con ip 172.22.55.numrandomgeneradoenelconstructor
        ConexionesComputadora c;

        for (int i = 0; i < datos.size(); i++) // Lectura de todos los registros del archivo
        {
            if (datos[i].IPDestino == c.ip) // Si la Ip Destino del archivo es igual a la Ip de la conexión 
            {
                if (c.nombre.length() <= 0) { // Si la conexión no tiene nombre entonces toma el nombre de la ip destino del archivo
                    c.nombre = datos[i].nombreDestino;
                }
                c.conexionesEntrantes.push(datos[i]); // Se añade la conexión a la pila 
            }
            if (datos[i].IPFuente == c.ip) // Si la Ip Fuente del archivo es igual a la Ip de la conexión 
            {
                if (c.nombre.length() <= 0) { // Si la conexión no tiene nombre entonces toma el nombre de la ip fuente del archivo
                    c.nombre = datos[i].nombreFuente; 
                }
                c.conexionesSalientes.push(datos[i]); // Se añade la conexión a la cola 
            }
        }

        cout << "IP: " << c.ip << "     Nombre: " << c.nombre << endl;
        cout << endl;

        // a) ¿Qué dirección ip estas usando?
        cout << "a) Dirección IP siendo utilizada " << c.ip << endl;
        cout << endl;

        // b) ¿Cuál fue la ip de la última conexión que recibió esta computadora ? ¿Es interna o externa ?
        Record r = c.conexionesEntrantes.top();
        cout << "b) La IP de la última conexión que recibió esta computadora es " << r.IPFuente << " la cual es interna" << endl;
        cout << "Registro completo: " << endl;
        r.imprimirRecord();
        cout << endl;

        // c) ¿Cuántas conexiones entrantes tiene esta computadora?
        cout << "c) Esta computadora tiene " << c.conexionesEntrantes.size() << " conexiones entrantes " << endl;
        cout << endl;

        // d) ¿Cuántas conexiones salientes tiene esta computadora?
        cout << "d) Esta computadora tiene " << c.conexionesSalientes.size() << " conexiones salientes " << endl;
        cout << endl;

        // Extra: ¿Tiene esta computadora 3 conexiones seguidas a un mismo sitio web?
        Record r1 = c.conexionesSalientes.front();
        string ip1;
        bool cseguidas;
        for(int i=0; i<c.conexionesSalientes.size(); i++) {
            if(r1.IPDestino[i]==r1.IPDestino[i+1] && r1.IPDestino[i+1]==r1.IPDestino[i+2]) { 
                cseguidas = true;
                ip1 = r1.nombreDestino;
            }
        }
        if(cseguidas==true) {
            cout << "Extra: esta computadora si tiene 3 conexiones seguidas al sitio web " << ip1 << endl;
        }
        else {
            cout << "Extra: esta computadora no tiene 3 conexiones seguidas" << endl;
        }
        cout << endl;
    }
    else {
        cout << "No se localizó el archivo" << endl;
    }
    return 0;
}
//Ximena Sánchez Padilla A01275072
//Karla Valeria Mondragón Rosas A01025108
#include <stack>
#include <queue>
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
    vector<string> conexionesSalientes;
    vector<string> conexionesEntrantes;
    ConexionesComputadora() {
        ip = "";
        domain = "";
    }

    ConexionesComputadora(string Ip, string dom) {
        this->ip = Ip;
        domain = dom;
    }

    void agregarConexionSaliente(string ipDestino)
    {
        conexionesSalientes.push_back(ipDestino);
    }

    void agregarConexionEntrante(string ipOrigen)
    {
        conexionesEntrantes.push_back(ipOrigen);
    }

    void imprimir() {
        cout << ip << ": Conexiones Salientes :" << conexionesSalientes.size() << endl;
        for (string ipDestino : conexionesSalientes) {
            cout << ipDestino << ",";
        }

        cout << ip << ": Conexiones Entrantes :" << conexionesEntrantes.size() << endl;
        for (string ipFuente : conexionesEntrantes) {
            cout << ipFuente << ",";
        }
    }
};

int main() {
    if (cargarDatos("datosEquipo12.csv")) {
        cout << "Datos en Registro: " << datos.size() << endl;

        unordered_map<string, ConexionesComputadora> di;
        vector<string> dominios;
        string dom;

        for (int i = 0; i < datos.size(); i++) // Lectura de todos los registros del archivo
        {

            // 1. Cree un conjunto de strings. En este debes agregar los nombres de los dominios/computadoras que no 
            // pertenezcan al dominio reto.com.
            if (datos[i].nombreDestino.find("reto.com") == -1 ) {
                dom = datos[i].nombreDestino;
                // Código adaptado de la página: https://www.cplusplus.com/reference/algorithm/find/ de la línea 21 el ejemplo
                if (find(dominios.begin(), dominios.end(), datos[i].nombreDestino) == dominios.end()) 
                    dominios.push_back(datos[i].nombreDestino);
            }
            else
            {
                dom = "reto.com";
            }

            /* 2. Crea una diccionario de pares <string,ConexionesComputadora> usando la clase creada en el reto pasado. 
                Este diccionario debe ser llenado con todas las ips en los datos. Por cada una, es necesario registrar todas 
                las conexiones que hayan salido desde esta ip o hayan sido recibidas por la misma. */
                
            // Revisa si esta IP ya se encuentra en el diccionario
                if (di.find(datos[i].IPFuente) == di.end()) {
                    // Aún no, se crea
                    ConexionesComputadora aux(datos[i].IPFuente,"reto.com");
                    di[datos[i].IPFuente] = aux;
                }
                di[datos[i].IPFuente].agregarConexionSaliente(datos[i].IPDestino);

                // Revisa si esta IP ya se encuentra en el diccionario
                if (di.find(datos[i].IPDestino) == di.end()) {
                    // Aún no, se crea
                    ConexionesComputadora aux(datos[i].IPDestino,dom);
                    di[datos[i].IPDestino] = aux;
                }
                di[datos[i].IPDestino].agregarConexionEntrante(datos[i].IPFuente);
        }
        cout << endl;

        // 1) Hay alguna nombre de dominio que - anómalo (Esto puede ser con inspección visual.
        cout << "LISTADO DE DOMINIOS DISTINTOS A RETO.COM ==================================" << endl;
        for (int i = 0; i < dominios.size(); i++) {
            cout << dominios[i] << endl;
        }
        cout << "===========================================================================" << endl;
        cout << "Los domnios anómalos son 2ngv8iamw6s87eo4z6c7.org y  4ftjzr4g5u8gawcsnjv1.com" << endl;  
        cout << endl;

        /* 2) Del los nombres de dominio encontrados en el paso anterior, ¿cuál es su ip? 
        Son 2ngv8iamw6s87eo4z6c7.org (88.10.73.146) y  4ftjzr4g5u8gawcsnjv1.com  (42.153.2.226) 
        ¿Cómo determinarías esta información de la manera más optima en complejidad temporal?
        RESPUESTA: Se agrega el atributo domain a ConexionesComputadora para realizar esto de forma más eficiente */

        cout << "La dirección IP de 2ngv8iamw6s87eo4z6c7.org es: ";
        for(auto c: di) {
            if(c.second.domain == "2ngv8iamw6s87eo4z6c7.org") {
                cout << c.second.ip << endl;
                break;
            }
        }

        cout << "La dirección IP de 4ftjzr4g5u8gawcsnjv1.com es: ";
        for(auto c:di) {
            if(c.second.domain == "4ftjzr4g5u8gawcsnjv1.com") {
                cout << c.second.ip << endl;
                break;
            }
        }
        cout << endl;

        /* 3) De las computadoras pertenecientes al dominio reto.com determina la cantidad de ips que tienen al menos una conexión entrante. 
            (Recuerda que ya tienes la dirección de la red y el último octeto puede tener computadoras del .1 al .254. 
            Imprime la cantidad de computadoras. */

        cout << "CANTIDAD DE IPs CON AL MENOS UNA CONEXION ENTRANTE =======================" << endl;
        int cuenta = 0;
        for (auto c : di) {
            if (c.second.domain == "reto.com" && c.second.conexionesEntrantes.size() > 0) 
            {
                cuenta++;
                cout << c.second.ip << endl;
            }
        }
        cout << "No. de equipos con al menos una conexión entrante del dominio reto.com: " << cuenta << endl;
        cout << "===========================================================================" << endl;

        /* 4) Toma algunas computadoras internas que no sean server.reto.com o el servidor dhcp. 
            Obten las ip unicas de las conexiones entrantes
            jack.reto.com --> 172.22.55.27
            mary.reto.com --> 172.22.55.15
            kevin.reto.com --> 172.22.55.92 */

            unordered_set<string> hashJack;
            for(int i=0; i<di["172.22.55.27"].conexionesEntrantes.size(); i++) {
                hashJack.insert(di["172.22.55.27"].conexionesEntrantes[i]);
            }
            cout << "Conexiones entrantes de jack.reto.com: " << endl;
            for(string j:hashJack) {
                cout << j << endl;
            }

            unordered_set<string> hashMary;
            for(int i=0; i<di["172.22.55.15"].conexionesEntrantes.size(); i++) {
                hashMary.insert(di["172.22.55.15"].conexionesEntrantes[i]);
            }
            cout << "Conexiones entrantes de mary.reto.com: " << endl;
            for(string h:hashMary) {
                cout << h << endl;
            }

            unordered_set<string> hashKevin;
            for(int i=0; i<di["172.22.55.92"].conexionesEntrantes.size(); i++) {
                hashKevin.insert(di["172.22.55.92"].conexionesEntrantes[i]);
            }
            cout << "Conexiones entrantes de rebecca.reto.com: " << endl;
            for(string h:hashKevin) {
                cout << h << endl;
            }
            cout << endl;

        /* 5)  Considerando el resultado de las preguntas 3 y 4, 
        ¿Qué crees que este ocurriendo en esta red? (Pregunta sin código)
        Que la ip 42.153.2.226 infecto de alguna forma a la computadora de mary.reto.com con ip 172.22.55.1 */
        

        /* 6) Para las ips encontradas en el paso anterior, determina si se han comunicado 
            con los datos encontrados en la pregunta 1
            comparar si las ip de los dominios anómalos estan en las ips de la pregunta 4 */

            int c1=0,c2=0;
            unordered_set<string> hashMary2;
            for(int i=0; i<di["172.22.55.15"].conexionesSalientes.size(); i++) {
                hashMary2.insert(di["172.22.55.15"].conexionesSalientes[i]);
            }
            cout << "Conexiones salientes de mary.reto.com: " << endl;
            for(string h:hashMary2) {
                if (h == "88.10.73.146") c1++;
                if (h == "42.153.2.226") c2++;
            }

            cout << "El equipo de mary.reto.com tuvo " << c1 << " conexion saliente a 2ngv8iamw6s87eo4z6c7.org" << endl;
            cout << "El equipo de mary.reto.com tuvo " << c2 << " conexion saliente a 4ftjzr4g5u8gawcsnjv1.com" << endl;
            cout << endl;

        /* 7) (Extra) : En caso de que hayas encontrado que las computadoras del paso 1 y 4 se comunican, determina 
            en qué fecha ocurre la primera comunicación entre estas 2 y qué protocolo se uso.
            RESPUESTA: Se realiza el 14 de agosto de 2020 (12:00:43 p. m.) utilizando la vulnerabilidad del puerto 443 */
        queue <string> f1, f2;
        for(int i=0; i<datos.size(); i++) {
            if(datos[i].IPFuente=="172.22.55.15" && datos[i].IPDestino=="88.10.73.146") 
                f1.push(datos[i].fecha);
        
            if(datos[i].IPFuente=="172.22.55.15" && datos[i].IPDestino=="42.153.2.226") 
                f2.push(datos[i].fecha);
        }
        cout << "La primera comunicación entre la ip de Mary y 88.10.73.146 fue: " << f1.front() << endl;
        cout << "La primera comunicación entre la ip de Mary y 42.153.2.2262 fue: " << f2.front() << endl;

        queue <int> p1, p2;
        for(int i=0; i<datos.size(); i++) {
            if(datos[i].IPFuente=="172.22.55.15" && datos[i].IPDestino=="88.10.73.146") 
                p1.push(datos[i].puertoDestino);
        
            if(datos[i].IPFuente=="172.22.55.15" && datos[i].IPDestino=="42.153.2.226") 
                p2.push(datos[i].puertoDestino);
        }
        cout << "La comunicación entre la ip de Mary y 88.10.73.146 se hizo utilizando la vulnerabilidad del puerto " << p1.front() << " el cual corresponde al protocolo HTTPS." << endl;
        cout << "La comunicación entre la ip de Mary y 42.153.2.2262 hizo utilizando la vulnerabilidad del puerto " << p2.front() << " el cual corresponde al protocolo HTTPS." << endl; 
    }
    else {
        cout << "No se localizó el archivo" << endl;
    }
    return 0;
}

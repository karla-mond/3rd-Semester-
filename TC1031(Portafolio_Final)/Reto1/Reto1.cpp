//Ximena Sánchez Padilla A01275072
//Karla Mondragón Rosas A01025108
/* 
Descripción: Programa que carga datos de un archivo el cual contiene registros de diferentes días
y diferentes computadoras. En dicho archivo se registra la IP Fuente al igual que su puerto y su nombre, 
el nombre del sitio visitado, su IP y puerto.
*/
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
using namespace std;

/***************************************************************************************
  CLASES Y METODOS DE DATOS
***************************************************************************************/
// Clase Group que se encaraga de consolidar los datos repetidos y sacar los totales  
class Group {
    public:
        string nombre;
        long cantidad;
        Group(string nom, long cant) {
            nombre = nom;
            cantidad = cant;
        }
        void imprimirgrupo() {
            cout << nombre << endl;
        }

};

// Clase principal necesaria para cargar los datos, ordenarlos y buscar 
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

/***************************************************************************************
  Cargando datos
***************************************************************************************/

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


/***************************************************************************************
  METODOS DE ORDENAMIENTO
***************************************************************************************/

template <class T>
class Sorter{
	public:
	virtual void Sort(vector<T> &data, int comparador(T a, T b))=0;
	
	void Imprimir(vector<T> &data){
		for(int i=0; i<data.size(); i++){
			cout<<data[i]<<",";
		}
		cout<<endl;
	}
	
	void Intercambiar(vector<T> &data, int posI, int posJ){
		T temp=data[posI];
		data[posI]=data[posJ];
		data[posJ]=temp;
	}
};


template <class T>
class MergeSort: public Sorter<T>{
	public:
        void Sort(vector<T> &data, int comparador(T a, T b)){
            SortAux(data, 0, data.size()-1, comparador);
        }
	private:
        void SortAux(vector<T> &data, int lo, int hi, int comparador(T a, T b)){
            if(lo>=hi){
                return;
            }
            int mid=(lo+hi)/2;
            SortAux(data, lo, mid, comparador);//sort de la izquierda
            SortAux(data, mid+1, hi, comparador);//sort de la derecha
            Merge(data, lo, mid, hi, comparador);
        }
	
        void Merge(vector<T> &data, int lo, int mid, int hi, int comparador(T a, T b)){
            //Determinar el tamaño de la copia izquierda
            //int sizeI=mid-lo+1;
            //Determinar el tamaño de la copia derecha
            //int sizeD=hi-mid;
            //Hacer la copia izquierda
            vector<T> copiaI;
            for(int i=lo; i<=mid; i++){
                copiaI.push_back(data[i]);
            }
            //Hacer la copia derecha
            vector<T> copiaD;
            for(int i=mid+1; i<=hi; i++){
                copiaD.push_back(data[i]);
            }
            //Poner los indicadores de posicion Izquierda, Derecha y Global en su lugar
            int posI=0;
            int posD=0;
            int posG=lo;
            //Copiar en su lugar
            //mientras posI<sizeI y posD<sizeD
            while(posI<copiaI.size() && posD<copiaD.size()){
                if(comparador(copiaI[posI],copiaD[posD])==-1){      // copiaI[posI]<copiaD[posD]
                    data[posG]=copiaI[posI];
                    posI++;
                    posG++;
                }
                else{
                    data[posG]=copiaD[posD];
                    posD++;
                    posG++;
                }
            }
			//revisar si el menor esta en copiaI o copiaD y poner en posicion Global
            //copiar elementos restantes
            while(posI<copiaI.size()){
                data[posG]=copiaI[posI];
                posI++;
                posG++;
            }
            while(posD<copiaD.size()){
                data[posG]=copiaD[posD];
                posD++;
                posG++;
            }
        }
};


/***************************************************************************************
 Comparar Dos Records - métodos auxiliares de ORDENAMIENTO
***************************************************************************************/
int compararDosRecordsNombreFuente(Record r1, Record r2)
{
    if (r1.nombreFuente < r2.nombreFuente)
        return -1;
    else if (r1.nombreFuente == r2.nombreFuente)
        return 0;
    else
        return 1;
}

int compararDosRecordsNombreDestino(Record r1, Record r2)
{
    if (r1.nombreDestino < r2.nombreDestino)
        return -1;
    else if (r1.nombreDestino == r2.nombreDestino)
        return 0;
    else
        return 1;
}

int compararDosRecordsIPFuente(Record r1, Record r2)
{
    if (r1.IPFuente < r2.IPFuente)
        return -1;
    else if (r1.IPFuente == r2.IPFuente)
        return 0;
    else
        return 1;
}

int compararDosRecordsIPDestino(Record r1, Record r2)
{
    if (r1.IPDestino < r2.IPDestino)
        return -1;
    else if (r1.IPDestino == r2.IPDestino)
        return 0;
    else
        return 1;
}

int compararDosRecordsPuertoFuente(Record r1, Record r2)
{
    if (r1.puertoFuente < r2.puertoFuente)
        return -1;
    else if (r1.puertoFuente == r2.puertoFuente)
        return 0;
    else
        return 1;
}

int compararDosRecordsPuertoDestino(Record r1, Record r2)
{
    if (r1.puertoDestino < r2.puertoDestino)
        return -1;
    else if (r1.puertoDestino == r2.puertoDestino)
        return 0;
    else
        return 1;
}

int compararDosRecordsFecha(Record r1, Record r2)
{
    if (r1.fecha < r2.fecha)
        return -1;
    else if (r1.fecha == r2.fecha)
        return 0;
    else
        return 1;
}


/***************************************************************************************
  MÉTODOS DE BÚSQUEDA
***************************************************************************************/
// Algoritmo de Busqueda Binaria
template <class T, class B>
int busquedaBinaria(vector<T> a, B buscado, int comparador(T a, B b)) { 
    int inicio = 0; 
    int fin = a.size() - 1; 
    while (inicio <= fin) {
        int medio = (fin + inicio) / 2; 
        if (comparador(a[medio],buscado)==0) {                // a[medio] == buscado
            return medio; // 1
        }
        else if (comparador(a[medio],buscado)==-1  ) {         // buscado > a[medio] ||  a[medio] < buscado
            inicio = medio + 1;
        }
        else {
            fin = medio - 1;  
        }
    }
    return -1;
}

// Algoritmo de Búsqueda Secuencial
template <class T, class B>
int busquedaSecuencial(vector<T> a,B buscado, int comparador(T a, B b)){
	for(int i=0; i<a.size(); i++){
		if (comparador(a[i],buscado)==0){
			return i;
		}
	}
	return -1;
}

// Comparadores Auxiliares para las pregunta Group  
int comparaGroupNombre(Group g, string nombre)
{
    if(g.nombre<nombre) {
        return -1;
    }
    else if(g.nombre==nombre) {
        return 0;
    }
    else return 1;
}

// Comparar Records - Búsqueda
int compararRecordNombreFuente(Record r, string nF)
{
    if(r.nombreFuente<nF) {
        return -1;
    }
    else if(r.nombreFuente==nF) {
        return 0;
    }
    else 
        return 1;
}

int compararRecordNombreDestino(Record r, string nD)
{
    if(r.nombreDestino<nD) {
        return -1;
    }
    else if(r.nombreDestino==nD) {
        return 0;
    }
    else return 1;
}

int compararRecordIPFuente(Record r, string IPFuente)
{
    if(r.IPFuente<IPFuente) {
        return -1;
    }
    else if(r.IPFuente==IPFuente) {
        return 0;
    }
    else return 1;
}

int compararRecordIPDestino(Record r, string IPDestino)
{
    if(r.IPDestino<IPDestino) {
        return -1;
    }
    else if(r.IPDestino==IPDestino) {
        return 0;
    }
    else return 1;
}

int compararRecordPuertoDestino(Record r, int puertoDestino)
{
    if(r.puertoDestino<puertoDestino) {
        return -1;
    }
    else if(r.puertoDestino==puertoDestino) {
        return 0;
    }
    else return 1;
}

int compararRecordPuertoFuente(Record r, int puertoFuente)
{
    if(r.puertoFuente<puertoFuente) {
        return -1;
    }
    else if(r.puertoFuente==puertoFuente) {
        return 0;
    }
    else return 1;
}

int compararRecordFecha(Record r, string fecha)
{
    if(r.fecha<fecha) {
        return -1;
    }
    else if(r.fecha==fecha) {
        return 0;
    }
    else return 1;
}

/***************************************************************************************
  MÉTODO que agrupa 
***************************************************************************************/

vector<Group> grupos;
template <class T>
void agrupar(vector<T> a, int c, int comparador(T a, T b)) {
    string valor;
        int grupo = 0;
        for (int i = 0; i < a.size(); i++) {
            if (grupo == 0) {
                grupo++;

                if (c==1)       valor = a[i].fecha;
                else if(c==2)   valor = a[i].hora;
                else if(c==3)   valor = a[i].IPFuente;
                else if(c==4)   valor = a[i].puertoFuente;
                else if(c==5)   valor = a[i].nombreFuente;
                else if(c==6)   valor = a[i].IPDestino;
                else if(c==7)   valor = a[i].puertoDestino;
                else if(c==8)   valor = a[i].nombreDestino;
            }
            else 
            {
                if (comparador(a[i], a[i - 1]) != 0) {
                    grupo++;
                    Group g(valor,grupo);
                    grupos.push_back(g);
                    grupo = 0;
                }
                else {
                    grupo++;
                }
            }
        }
}

/***************************************************************************************
  Preguntas del ejercicio 
***************************************************************************************/

void pregunta2(){

    grupos.clear();
    // Se ordena con el algoritmo de ordenamiento MergeSort
    cout << "\n a) Ordenando por Merge" << endl;
        MergeSort<Record> s;
        s.Sort(datos, compararDosRecordsFecha);

        cout << "\n b) Agrupando" << endl;
        agrupar(datos,1,compararDosRecordsFecha);

        for(int i=0;i<grupos.size();i++) {
            grupos[i].imprimirgrupo();
        }
        
        cout << "\n c) Búsqueda secuencial en los grupos" << endl;
        string valor = "11-8-2020";
        int r = busquedaSecuencial(grupos,valor,comparaGroupNombre);
        if(r!=-1) {
            cout << "Valor: " << valor << " encontrado con " << grupos[r].cantidad << " registros" << endl;
        }

}

// ¿Alguna de las computadoras pertenece a Jeffrey, Betty, Katherine, Scott, Benjamin, Samuel o Raymond?
void pregunta3(){

    cout << "\na) Ordenando por Merge" << endl;
        MergeSort<Record> s;
        s.Sort(datos, compararDosRecordsNombreFuente);   

        // Por el algoritmo de búsqueda binaria se inicializa un arreglo que contiene los datos que se quieren localizar
        cout << "b) Buscando por búsqueda binaria " << endl;
        string valor[] = {"jeffrey.reto.com", "betty.reto.com", "katherine.reto.com","scott.reto.com", "benjamin.reto.com","samuel.reto.com","raymond.reto.com"};
        // Se crea un ciclo con el tamaño del arreglo y se buscan todos los datos del arreglo hasta encontrar alguno
        for (int i=0;i<6;i++)
        {
            int r = busquedaBinaria(datos,valor[i],compararRecordNombreFuente);
            if (r != -1) {
                cout << " - "<< valor[i] << " encontrado en la posición " << r << endl;
            }
            else
                cout << " - " << valor[i] << " NO encontrado" << endl;
        }

}

// ¿Cuál es la dirección de la red interna de la compañía?
void pregunta4(){
    grupos.clear(); // Se incializa el vector de grupos 
    cout << "\n a) Ordenando por Merge" << endl;
        MergeSort<Record> s;
        s.Sort(datos, compararDosRecordsIPFuente);

        cout << "\n b) Agrupando" << endl;
         
        agrupar(datos,3,compararDosRecordsIPFuente); // Acumula los totales por IP Fuente 

        // Se utiliza un ciclo para imprimir las IP fuentes ya agrupadas
        for(int i=0;i<grupos.size();i++) {
            grupos[i].imprimirgrupo();
        }
   
}

//¿Alguna computadora se llama server.reto.com?
void pregunta5(){
    cout << "\na) Ordenando por Merge" << endl;
        MergeSort<Record> s;
        s.Sort(datos, compararDosRecordsNombreFuente);   
        // Utilizando búsqueda binaria (ya que el conjunto de datos ya esta ordenado), se quiere buscar
        // un valor específico en el registro del nombre de la fuente 
        cout << "b) Buscando por búsqueda binaria " << endl;
        string valor= "server.reto.com";
        int r = busquedaBinaria(datos,valor,compararRecordNombreFuente);
        if (r != -1) {
            cout << " - "<< valor << " encontrado en la posición " << r << endl;
        }
        else
            cout << " - " << valor << " NO encontrado" << endl;
}

// Pregunta 6: ¿Qué servicio de mail utilizan de todos estos: gmail.com, outlook.com, protonmail.com, freemailserver.com?
void pregunta6(){
    
    cout << "\n a) Ordenando por Merge... " << endl;
        MergeSort<Record> s;
        s.Sort(datos, compararDosRecordsNombreDestino);   
        // Se iniciliza un arreglo que contiene las opciones que se deben de buscar por búsqueda binaria
        cout << "b) Buscando por búsqueda binaria " << endl;
        string valor[] = {"gmail.com", "outlook.com","protonmail.com", "freemailserver.com"};
        // Se crea un ciclo con el tamaño del arreglo para poder buscar cada elemento y saber si se encuentra o no
        for (int i=0;i<4;i++)
        {
            int r = busquedaBinaria(datos,valor[i],compararRecordNombreDestino);
            if (r != -1) {
                cout << " - "<< valor[i] << " encontrado en la posición " << r << endl;
            }
            else
                cout << " - "<< valor[i] << " NO encontrado" << endl;
        }

}

// Considerando solamente los puertos destino ¿Qué puertos abajo del 1000 se están usando? Lista los puertos e investiga qué aplicación/servicio
// lo utiliza generalmente. 
void pregunta7(){
    /* Pregunta 7
    Considerando solamente los puertos destino ¿Qué puertos abajo del 1000 se están usando? 
    Lista los puertos e investiga que aplicación/servicio lo utiliza generalmente. */

    //Sección auxiliar para saber el puerto máximo del arreglo
    int mayor=0;
    for(int i=0; i<datos.size(); i++) {
        if(datos[i].puertoDestino>mayor) {
            mayor = datos[i].puertoDestino;
        }
    }
    cout << "El puerto mayor es: " << mayor << endl; 

    /* Contadores para verificar que el tamaño del vector sea igual a las veces que se repiten 
    los puertos en el archivo */
    int count993=0, count965=0, count465=0, count443=0, count80=0, count67=0, count53=0; 
    for(int i=0; i<datos.size(); i++) {
        //puertos[i].imprimirPuerto();
        if(datos[i].puertoDestino==993) {
            count993++;
        }
        else if(datos[i].puertoDestino==965) {
            count965++;
        }
        else if(datos[i].puertoDestino==465) {
            count465++;
        }
        else if(datos[i].puertoDestino==443) {
            count443++;
        }
            else if(datos[i].puertoDestino==80) {
            count80++;
        }
        else if(datos[i].puertoDestino==67) {
            count67++;
        }
        else if(datos[i].puertoDestino==53) {
            count53++;
        }
    }
    cout << "puerto 993, " << count993 << endl;
    cout << "puerto 965, " << count965 << endl;
    cout << "puerto 465, " << count465 << endl;
    cout << "puerto 443, " << count443 << endl;
    cout << "puerto 80, " << count80 << endl;
    cout << "puerto 67, " << count67 << endl;
    cout << "puerto 53, " << count53 << endl;
    int suma = count993 + count965 + count465 + count443 + count80 + count67 + count53;
    cout << suma << endl;
    
    int numPuertos[] = {53, 67, 80, 443, 465, 965, 993};
    // Búsqueda de el servicio de cada puerto, estos "servicios" pueden cambiar ya que las computadoras acceden a diferentes
    // sitios que pueden acceder por mismos puertos. 
    for(int i=0; i<8; i++) {
        int r = busquedaBinaria(datos,numPuertos[i],compararRecordPuertoDestino);
        if(r!=-1) {
            cout << "Puerto: " << numPuertos[i] << " / Servicio: " << datos[r].nombreDestino << endl;
        }
    }
}

int main()
{
    vector<Group> grupos;
    
    if (cargarDatos("datosEquipo12.csv")) { 
        /*cout << "PREGUNTA 1 -------------------------------------------" << endl;
        cout << "Cantidad total de registros: " << datos.size() << endl;

        cout << "PREGUNTA 2 -------------------------------------------" << endl;
        pregunta2();

        cout << "PREGUNTA 3 -------------------------------------------" << endl;
        pregunta3(); */

        cout << "PREGUNTA 4 -------------------------------------------" << endl;
        pregunta4();

        /*cout << "PREGUNTA 5 -------------------------------------------" << endl;
        pregunta5();

        cout << "PREGUNTA 6 -------------------------------------------" << endl;
        pregunta6();

        cout << "PREGUNTA 7 -------------------------------------------" << endl;
        pregunta7(); */
    }
    else {
        cout << "No se localizó el archivo" << endl;
    }
	return 0;
}
#include<bits/stdc++.h>
using namespace std;

double calcularLongitudRuta(const vector<int>& ruta, const vector<vector<int>>& matrizAdyacencia) {
    double longitud = 0.0;
    for (size_t i = 0; i < ruta.size() - 1; ++i) {
        longitud += matrizAdyacencia[ruta[i]][ruta[i + 1]];
    }
    longitud += matrizAdyacencia[ruta.back()][ruta.front()];
    return longitud;
}

vector<int> generarSolucionInicial(int numCiudades) {
    vector<int> rutaInicial(numCiudades);
    iota(rutaInicial.begin(), rutaInicial.end(), 0);  // Llena la ruta con 0, 1, 2, ..., numCiudades-1
    random_device rd;
    mt19937 g(rd());
    shuffle(rutaInicial.begin() + 1, rutaInicial.end(), g);  // Mezcla las ciudades excepto la primera
    return rutaInicial;
}

vector<int> generarVecino(const vector<int>& ruta) {
    vector<int> vecino = ruta;
    int ciudad1 = rand() % (ruta.size() - 1) + 1;
    int ciudad2 = rand() % (ruta.size() - 1) + 1;
    swap(vecino[ciudad1], vecino[ciudad2]);
    return vecino;
}

bool aceptarVecino(double delta, double temperatura) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dis(0.0, 1.0);
    return (delta < 0.0) || (dis(gen) < exp(-delta / temperatura));
}

vector<int> simulatedAnnealingTSP(const vector<vector<int>>& matrizAdyacencia, double temperaturaInicial, double enfriamiento, int iteraciones) {
    srand(time(0));

    int numCiudades = matrizAdyacencia.size();
    vector<int> mejorRuta = generarSolucionInicial(numCiudades);
    double mejorLongitud = calcularLongitudRuta(mejorRuta, matrizAdyacencia);

    vector<int> rutaActual = mejorRuta;
    double longitudActual = mejorLongitud;

    double temperatura = temperaturaInicial;

    for (int iter = 0; iter < iteraciones; ++iter) {
        vector<int> vecino = generarVecino(rutaActual);
        double longitudVecino = calcularLongitudRuta(vecino, matrizAdyacencia);

        double delta = longitudVecino - longitudActual;

        if (aceptarVecino(delta, temperatura)) {
            rutaActual = vecino;
            longitudActual = longitudVecino;
        }

        if (longitudActual < mejorLongitud) {
            mejorRuta = rutaActual;
            mejorLongitud = longitudActual;
        }

        temperatura *= enfriamiento;
    }

    return mejorRuta;
}

int main() {
    std::vector<std::vector<int>> algo;
    // ... (lectura del archivo CSV)
    const std::string archivo_csv = "grafo1\\g1_1.csv";

    // Abrir el archivo
    std::ifstream archivo(archivo_csv);

    // Verificar si se abrió correctamente
    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo CSV." << std::endl;
        return 1;  // Salir con código de error
    }

    
    // Leer el archivo línea por línea
    std::string linea;
    while (std::getline(archivo, linea)) {
        // Usar un stringstream para dividir la línea en campos
        std::istringstream ss(linea);
        std::vector<int> fila;

        // Leer cada campo y agregarlo al vector fila
        std::string campo;
        while (std::getline(ss, campo, ';')) {
            fila.push_back(std::stoi(campo));
        }

        // Agregar la fila al vector de datos
        algo.push_back(fila);
    }

    // Cerrar el archivo después de leer
    archivo.close();
    
    // Parámetros del algoritmo
    const double temperaturaInicial = 5000.0;
    const double enfriamiento = 0.97;
    const int iteraciones = 5000;

    vector<int> mejorRuta = simulatedAnnealingTSP(algo, temperaturaInicial, enfriamiento, iteraciones);

    // Imprimir la mejor ruta encontrada
    cout << "Mejor Ruta: ";
    for (int ciudad : mejorRuta) {
        cout << ciudad << " ";
    }
    cout << endl;

    // Imprimir la longitud de la mejor ruta
    cout << "Longitud de la Mejor Ruta: " << calcularLongitudRuta(mejorRuta, algo) << endl;

    return 0;
}

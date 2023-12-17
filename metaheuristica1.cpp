#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

// Función para calcular la longitud de una ruta
double calcularLongitudRuta(const vector<int>& ruta, const vector<vector<double>>& matrizAdyacencia) {
    double longitud = 0.0;
    for (size_t i = 0; i < ruta.size() - 1; ++i) {
        longitud += matrizAdyacencia[ruta[i]][ruta[i + 1]];
    }
    longitud += matrizAdyacencia[ruta.back()][ruta.front()]; // Regreso al inicio
    return longitud;
}

// Función para generar una solución inicial aleatoria
vector<int> generarSolucionInicial(int numCiudades) {
    vector<int> rutaInicial;
    for (int i = 0; i < numCiudades; ++i) {
        rutaInicial.push_back(i);
    }
    random_shuffle(rutaInicial.begin() + 1, rutaInicial.end()); // Barajar aleatoriamente, evitando cambiar el punto de inicio
    return rutaInicial;
}

// Función para generar un vecino intercambiando dos ciudades
vector<int> generarVecino(const vector<int>& ruta) {
    vector<int> vecino = ruta;
    int ciudad1 = rand() % (ruta.size() - 1) + 1; // Evitar cambiar el punto de inicio
    int ciudad2 = rand() % (ruta.size() - 1) + 1; // Evitar cambiar el punto de inicio
    swap(vecino[ciudad1], vecino[ciudad2]);
    return vecino;
}

// Función para determinar si se acepta un vecino según la probabilidad de Boltzmann
bool aceptarVecino(double delta, double temperatura) {
    return (delta < 0.0) || (rand() / (RAND_MAX + 1.0) < exp(-delta / temperatura));
}

// Algoritmo de Recocido Simulado para TSP
vector<int> simulatedAnnealingTSP(const vector<vector<double>>& matrizAdyacencia, double temperaturaInicial, double enfriamiento, int iteraciones) {
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
    // Ejemplo de matriz de adyacencia (distancias entre ciudades)
    vector<vector<double>> matrizAdyacencia = {
        {0, 10, 15, 20},
        {10, 0, 35, 25},
        {15, 35, 0, 30},
        {20, 25, 30, 0}
    };

    // Parámetros del algoritmo
    double temperaturaInicial = 1000.0;
    double enfriamiento = 0.98;
    int iteraciones = 1000;

    // Ejecutar el algoritmo de Recocido Simulado para TSP
    vector<int> mejorRuta = simulatedAnnealingTSP(matrizAdyacencia, temperaturaInicial, enfriamiento, iteraciones);

    // Imprimir la mejor ruta encontrada
    cout << "Mejor Ruta: ";
    for (int ciudad : mejorRuta) {
        cout << ciudad << " ";
    }
    cout << endl;

    // Imprimir la longitud de la mejor ruta
    cout << "Longitud de la Mejor Ruta: " << calcularLongitudRuta(mejorRuta, matrizAdyacencia) << endl;

    return 0;
}

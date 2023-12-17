#include <bits/stdc++.h>
using namespace std;

// Definición de la estructura de un individuo
struct Individual {
    vector<int> tour;
    double fitness;
};

// Función para calcular la distancia entre dos ciudades
double distance(int city1, int city2, const vector<vector<double>> &distances) {
    return distances[city1][city2];
}

// Función para calcular la aptitud (fitness) de un individuo
double calculateFitness(const vector<int> &tour, const vector<vector<double>> &distances) {
    double totalDistance = 0.0;
    for (int i = 0; i < tour.size() - 1; ++i) {
        totalDistance += distance(tour[i], tour[i + 1], distances);
    }
    // Agregar la distancia de regreso a la ciudad inicial
    totalDistance += distance(tour.back(), tour.front(), distances);
    return 1.0 / totalDistance; // Mayor distancia, menor fitness
}

// Función para inicializar una población aleatoria
vector<Individual> initializePopulation(int populationSize, int numCities, const vector<vector<double>> &distances) {
    vector<Individual> population;
    for (int i = 0; i < populationSize; ++i) {
        vector<int> tour(numCities);
        for (int j = 0; j < numCities; ++j) {
            tour[j] = j;
        }
        random_shuffle(tour.begin() + 1, tour.end()); // Barajar aleatoriamente, excepto la primera ciudad
        double fitness = calculateFitness(tour, distances);
        population.push_back({tour, fitness});
    }
    return population;
}

// Función para seleccionar padres utilizando torneo
Individual tournamentSelection(const vector<Individual> &population, int tournamentSize) {
    int populationSize = population.size();
    Individual bestParent = population[rand() % populationSize];
    for (int i = 1; i < tournamentSize; ++i) {
        Individual candidate = population[rand() % populationSize];
        if (candidate.fitness > bestParent.fitness) {
            bestParent = candidate;
        }
    }
    return bestParent;
}

// Función para realizar el cruce (crossover) entre dos padres
Individual crossover(const Individual &parent1, const Individual &parent2, const vector<vector<double>> &distances) {
    int numCities = parent1.tour.size();
    int crossoverPoint1 = rand() % numCities;
    int crossoverPoint2 = rand() % numCities;

    int startCrossover = min(crossoverPoint1, crossoverPoint2);
    int endCrossover = max(crossoverPoint1, crossoverPoint2);

    vector<int> childTour(numCities, -1);
    for (int i = startCrossover; i <= endCrossover; ++i) {
        childTour[i] = parent1.tour[i];
    }

    int currentIndex = endCrossover + 1;
    for (int i = 0; i < numCities; ++i) {
        if (currentIndex == numCities) {
            currentIndex = 0;
        }
        if (find(childTour.begin(), childTour.end(), parent2.tour[i]) == childTour.end()) {
            childTour[currentIndex++] = parent2.tour[i];
        }
    }

    double fitness = calculateFitness(childTour, distances);
    return {childTour, fitness};
}

// Función para realizar mutaciones en un individuo
void mutate(Individual &individual, double mutationRate, const vector<vector<double>> &distances) {
    if (rand() / (double)RAND_MAX < mutationRate) {
        int numCities = individual.tour.size();
        int mutationPoint1 = rand() % numCities;
        int mutationPoint2 = rand() % numCities;
        swap(individual.tour[mutationPoint1], individual.tour[mutationPoint2]);
        individual.fitness = calculateFitness(individual.tour, distances);
    }
}

// Función principal del algoritmo genético para TSP
vector<int> geneticAlgorithmTSP(int populationSize, int generations, int tournamentSize, double mutationRate, const vector<vector<double>> &distances) {
    int numCities = distances.size(); /* Número de ciudades */;

    srand(static_cast<unsigned>(time(nullptr)));

    vector<Individual> population = initializePopulation(populationSize, numCities, distances);

    for (int generation = 0; generation < generations; ++generation) {
        vector<Individual> newPopulation;
        for (int i = 0; i < populationSize; ++i) {
            Individual parent1 = tournamentSelection(population, tournamentSize);
            Individual parent2 = tournamentSelection(population, tournamentSize);
            Individual child = crossover(parent1, parent2, distances);
            mutate(child, mutationRate, distances);
            newPopulation.push_back(child);
        }
        population = newPopulation;
    }

    // Seleccionar el mejor individuo de la población final
    auto bestIndividual = max_element(population.begin(), population.end(),
                                      [](const Individual &ind1, const Individual &ind2) {
                                          return ind1.fitness < ind2.fitness;
                                      });

    return bestIndividual->tour;
}

int main() {
    // Lectura del archivo CSV
    vector<vector<int>> algo;
    const string archivo_csv = "grafo2\\g2_1.csv";

    // Abrir el archivo
    ifstream archivo(archivo_csv);

    // Verificar si se abrió correctamente
    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo CSV." << endl;
        return 1; // Salir con código de error
    }

    // Leer el archivo línea por línea
    string linea;
    while (getline(archivo, linea))
    {
        // Usar un stringstream para dividir la línea en campos
        istringstream ss(linea);
        vector<int> fila;

        // Leer cada campo y agregarlo al vector fila
        string campo;
        while (getline(ss, campo, ';'))
        {
            fila.push_back(stoi(campo));
        }

        // Agregar la fila al vector de datos
        algo.push_back(fila);
    }

    // Cerrar el archivo después de leer
    archivo.close();

    // Asignar los datos leídos a numCities y distances
    int numCities = algo.size();
    vector<vector<double>> distances(numCities, vector<double>(numCities, 0.0));
    for (int i = 0; i < numCities; ++i) {
        for (int j = 0; j < numCities; ++j) {
            distances[i][j] = static_cast<double>(algo[i][j]);
        }
    }

    // Parámetros del algoritmo genético
    int populationSize = 50;
    int generations = 100;
    int tournamentSize = 5;
    double mutationRate = 0.02;

    srand(static_cast<unsigned>(time(nullptr)));

    vector<Individual> population = initializePopulation(populationSize, numCities, distances);

    for (int generation = 0; generation < generations; ++generation){
   

        vector<Individual> newPopulation;
        for (int i = 0; i < populationSize; ++i) {
            Individual parent1 = tournamentSelection(population, tournamentSize);
            Individual parent2 = tournamentSelection(population, tournamentSize);
            Individual child = crossover(parent1, parent2, distances);
            mutate(child, mutationRate, distances);

            newPopulation.push_back(child);
        }
        population = newPopulation;
    }

    // Seleccionar el mejor individuo de la población final
    auto bestIndividual = max_element(population.begin(), population.end(),
        [](const Individual& ind1, const Individual& ind2) {
            return ind1.fitness < ind2.fitness;
        });

    // Imprimir la mejor ruta encontrada
    cout << "Mejor Ruta: ";
    for (int city : bestIndividual->tour) {
        cout << city << " ";
    }
    cout << endl;

    // Obtener la mejor ruta y su distancia total
    vector<int> bestTour = bestIndividual->tour;
    double bestDistance = 1.0 / bestIndividual->fitness;

    cout << "Mejor distancia: " << bestDistance << endl;
    return 0;
}

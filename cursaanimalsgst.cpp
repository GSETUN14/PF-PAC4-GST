#include <iostream>
#include <thread>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <mutex>
#include <atomic>

using namespace std;

const int DISTANCIA_META = 50;

mutex console_mutex;
atomic<bool> carrera_acabada(false);

struct Animal {
    string nom;
    int velocitat_base, posicio = 0, fila;
};

// Mou el cursor a la posició x, y
void moureCursor(int x, int y) {
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Fa que l'animal avanci fins a la meta
void corre(Animal& animal) {
    while (!carrera_acabada) {
        this_thread::sleep_for(chrono::milliseconds(100 + rand() % 200));
        animal.posicio += max(animal.velocitat_base + (rand() % 3 - 1), 0); 

        // Bloqueja la consola i mostra la posició
        lock_guard<mutex> lock(console_mutex);
        moureCursor(animal.posicio, animal.fila);
        cout << animal.nom[0];

        // Comprova si ha guanyat
        if (animal.posicio >= DISTANCIA_META) {
            carrera_acabada = true;
            moureCursor(0, animal.fila + 2);
            cout << "¡" << animal.nom << " ha guanyat!" << endl;
        }
    }
}

int main() {
    srand(time(NULL));

    // Defineix els animals
    vector<Animal> animals = {{"Conill", 3, 0, 2}, {"Tortuga", 1, 0, 3}, {"Cangur", 2, 0, 4}};
    vector<thread> fils;

    // Crea un fil per a cada animal
    for (auto& animal : animals) fils.push_back(thread(corre, ref(animal)));
    
    // Espera que acabin els fils
    for (auto& fil : fils) fil.join();

    return 0;
}

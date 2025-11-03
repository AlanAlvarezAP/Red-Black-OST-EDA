#include <iostream>
#include <windows.h>
#include "Preprocesamiento.h"
#include "RB_OST.h"

std::vector<std::string> leer_archivos_carpeta(const std::string& carpeta) {
    std::vector<std::string> archivos;
    WIN32_FIND_DATAA ffd;
    HANDLE hFind = FindFirstFileA((carpeta + "\\*").c_str(), &ffd);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                std::string filename = ffd.cFileName;
                if (filename.size() >= 4 && filename.substr(filename.size() - 4) == ".txt") {
                    archivos.push_back(carpeta + "\\" + filename);
                }   
            }
        } while (FindNextFileA(hFind, &ffd) != 0);
        FindClose(hFind);
    }
    return archivos;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    /*Preprocesador prepo("stop_words.txt");
    std::cout << " Bienvenido al proyecto de EDA RB-OST" << std::endl;
    auto archivos = leer_archivos_carpeta("EDA input");
    for (const auto& f : archivos) {
        std::cout << " NOTICIA " << f << std::endl;
        prepo.reset();
        prepo.Limpieza(f);
        prepo.Tokenizar();
        prepo.Stop_words();
        prepo.Porter_Stemming();
        prepo.Conteo();
        prepo.print();
    }*/
    RB_OST arbol;

    const char* noticias[][3] = {
        {"Noticia A1", "Noticia A2", "Noticia A3"},
        {"Noticia B1", "Noticia B2", "Noticia B3"},
        {"Noticia C1", "Noticia C2", "Noticia C3"},
        {"Noticia D1", "Noticia D2", "Noticia D3"},
        {"Noticia E1", "Noticia E2", "Noticia E3"},
        {"Noticia F1", "Noticia F2", "Noticia F3"},
        {"Noticia G1", "Noticia G2", "Noticia G3"},
        {"Noticia H1", "Noticia H2", "Noticia H3"},
        {"Noticia I1", "Noticia I2", "Noticia I3"},
        {"Noticia J1", "Noticia J2", "Noticia J3"},
        {"Noticia K1", "Noticia K2", "Noticia K3"},
        {"Noticia L1", "Noticia L2", "Noticia L3"},
        {"Noticia M1", "Noticia M2", "Noticia M3"},
        {"Noticia N1", "Noticia N2", "Noticia N3"},
        {"Noticia O1", "Noticia O2", "Noticia O3"},
        {"Noticia P1", "Noticia P2", "Noticia P3"},
        {"Noticia Q1", "Noticia Q2", "Noticia Q3"},
        {"Noticia R1", "Noticia R2", "Noticia R3"},
        {"Noticia S1", "Noticia S2", "Noticia S3"},
        {"Noticia T1", "Noticia T2", "Noticia T3"}
    };

    uint64_t momentos_sets[][10] = {
        {5, 10, 15, 20, 25, 30, 35, 40, 45, 50},
        {100, 90, 80, 70, 60, 50, 40, 30, 20, 10},
        {50, 20, 70, 10, 30, 60, 80, 25, 65, 85},
        {100, 105, 102, 110, 108, 107, 120, 125, 123, 122},
        {10, 1000, 500, 1500, 250, 750, 1250, 1750, 50, 2000}
    };

    int total_sets = 5;
    int noticias_usadas = 0;

    for (int s = 0; s < total_sets; ++s) {
        std::cout << "\n==============================\n";
        std::cout << " Conjunto " << s + 1 << " de inserciones\n";
        std::cout << "==============================\n";

        for (int i = 0; i < 10; ++i) {
            MiArray<const char*> arr;
            for (int j = 0; j < 3; ++j) {
                arr.push_back(noticias[noticias_usadas % 20][j]);
            }

            uint64_t momento = momentos_sets[s][i];
            if (s == 4) {
                std::cout << "Esperando" << std::endl;
            }
            arbol.Insert(arr, momento);

            std::cout << "\n Insertando momento " << momento << "...\n";
            arbol.preprinting();
            noticias_usadas++;
        }

        std::cout << "\n Árbol final del conjunto " << s + 1 << ":\n";
        arbol.preprinting();
        std::cout << "------------------------------------------\n";

        // Reiniciar el árbol para el siguiente conjunto
        arbol = RB_OST();
    }
}

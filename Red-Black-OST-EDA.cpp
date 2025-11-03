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

    uint64_t momentos[] = { 50, 120, 30, 180, 75, 90, 200, 60, 15, 250, 170, 110, 140, 220, 10, 5, 95, 135, 160, 300 };

    for (int i = 0; i < 20; ++i) {
        MiArray<const char*> arr;
        for (int j = 0; j < 3; ++j) {
            arr.push_back(noticias[i][j]);
        }
        arbol.Insert(arr, momentos[i]);
    }
    std::cout << "Arbol RB_OST después de inserciones:" << std::endl;
    arbol.preprinting();
}

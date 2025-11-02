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
    Preprocesador prepo("stop_words.txt");
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
    }
}

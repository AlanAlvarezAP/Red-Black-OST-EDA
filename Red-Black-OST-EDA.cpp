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

    const char* noticias[] = {
        "Noticia A", "Noticia B", "Noticia C", "Noticia D",
        "Noticia E", "Noticia F", "Noticia G", "Noticia H",
        "Noticia I", "Noticia J", "Noticia K", "Noticia L",
        "Noticia M", "Noticia N", "Noticia O", "Noticia P",
        "Noticia Q", "Noticia R", "Noticia S", "Noticia T",
        "Noticia U", "Noticia V", "Noticia W", "Noticia X",
        "Noticia Y"
    };
    uint64_t momentos[] = {
        50, 10, 70, 20, 30, 90, 60, 40, 80, 100,
        25, 35, 55, 65, 75, 85, 15, 95, 5, 45,
        105, 110, 115, 120, 125
    };

    int total = 25;

    for (int i = 0; i < total; ++i) {
        arbol.Insert(noticias[i], momentos[i]);

        std::cout << "Despues de insertar nodo " << i + 1 << " con noticia " << noticias[i] << " y momento " << momentos[i] << ":" << std::endl;
        arbol.preprinting();
        std::cout << "--------------------------------------" << std::endl;
    }

    std::cout << "Arbol final tras 25 inserciones" << std::endl;
    arbol.preprinting();

    std::cout << *((arbol.Select(arbol.root, 1))->momentos.last_elem());

    /*UnorderedHash map;

    Node* n1 = new Node("hola", 1, false, 1, nullptr, nullptr, nullptr);
    Node* n2 = new Node("mundo", 2, false, 2, nullptr, nullptr, nullptr);
    Node* n3 = new Node("adios", 3, true, 3, nullptr, nullptr, nullptr);
    Node* n4 = new Node("hola", 4, true, 4, nullptr, nullptr, nullptr);

    map.insert_hash(n1);
    map.insert_hash(n2);
    map.insert_hash(n3);

    if (map["hola"]) std::cout << "hola -> " << map["hola"]->value->topico << std::endl;
    if (map["mundo"]) std::cout << "mundo -> " << map["mundo"]->value->topico << std::endl;
    if (map["adios"]) std::cout << "adios -> " << map["adios"]->value->topico << std::endl;

    map.insert_hash(n4);
    if (map["hola"]) std::cout << "hola (despues de reemplazo) -> " << map["hola"]->value->topico << std::endl;
    if (!map["test"]) std::cout << "'test' no existe en el hash map\n";

    map.print();

    delete n1;
    delete n2;
    delete n3;
    delete n4;*/
    return 0;
}

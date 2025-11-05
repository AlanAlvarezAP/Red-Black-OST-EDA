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
    
    // TEST PARA PREPROCESADO
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

    // TEST PARA EL ARBOL
    const char* noticias[] = {
        "Noticia A", "Noticia B", "Noticia C", "Noticia D",
        "Noticia E", "Noticia F", "Noticia G", "Noticia H",
        "Noticia I", "Noticia J", "Noticia K", "Noticia L",
        "Noticia M", "Noticia N", "Noticia O", "Noticia P",
        "Noticia Q", "Noticia R", "Noticia S", "Noticia T",
        "Noticia U", "Noticia V", "Noticia W", "Noticia X",
        "Noticia Y"
    };

    uint64_t momentos[25];
    for (int i = 0; i < 25; ++i) {
        momentos[i] = i + 1; // 1, 2, 3, ...
    }

    int total = 25;

    for (int i = 0; i < total; ++i) {
        std::cout << "=== Insertando nodo " << i + 1 << " | Noticia: " << noticias[i] << " | Momento: " << momentos[i] << " ===" << std::endl;
        arbol.Insert(noticias[i], momentos[i]);
        arbol.preprinting();
        std::cout << "--------------------------------------" << std::endl;
    }

    // Reinserciones, continuando la secuencia de momentos
    uint64_t nuevos_momentos[10];
    for (int i = 0; i < 10; ++i) {
        nuevos_momentos[i] = total + i + 1; // 26, 27, 28, ...
    }

    for (int i = 0; i < 10; ++i) {
        std::cout << "=== Reinsercion nodo " << i + 1 << " | Noticia: " << noticias[i] << " | Nuevo Momento: " << nuevos_momentos[i] << " ===" << std::endl;
        arbol.Insert(noticias[i], nuevos_momentos[i]);
        arbol.preprinting();
        std::cout << "--------------------------------------" << std::endl;
    }

    Node* primero = arbol.Select(arbol.root, 1);
    if (primero) {
        std::cout << "Primer nodo en orden (Select 1): " << primero->topico << " | Ultimo momento: " << *(primero->momentos.last_elem()) << std::endl;
    }

    std::cout << "=== Arbol final tras inserciones y reinserciones ===" << std::endl;
    arbol.preprinting();


    // TEST PARA LA HASH MAP
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

#include <iostream>
#include <windows.h>
#include <limits>
#include <fstream>
#include <cstdlib>
#include <chrono>
#include "Preprocesamiento.h"
#include "RB_OST.h"

void escribir_top_csv(MiArray<Topk> window) {
    std::ofstream escritor("topk.csv");

    for (int i = 0; i < window.get_size(); i++) {
        escritor << window[i].nodo->topico << "," << window[i].freq << std::endl;
    }

    escritor.close();
}
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

void reset_top() {
    std::ofstream archivo("topk.csv", std::ios::out); 
    if (!archivo) {
        std::cerr << "No se pudo abrir el archivo" << std::endl;
        return;
    }
    archivo.close();

    std::cout << "Archivo limpiado correctamente" << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    Preprocesador prepo("stop_words.txt");
    RB_OST arbol;
    char tipo_query;
    char continuo;
    bool se_mantenga=false;
    std::cout << " Bienvenido al proyecto de EDA ||| RB-OST para Streaming continuo |||" << std::endl;
    uint64_t momento_global = 0;
    auto archivos = leer_archivos_carpeta("EDA input");
    int k_factor=0;
    int m_factor=0;
    int end_moment=0;
    int start_moment=0;
    bool ultimos_k=false;
    int total_topics=0;
    double duracion=0;
    for (const auto& f : archivos) {
        prepo.reset();
        prepo.Limpieza(f);
        prepo.Tokenizar();
        prepo.Stop_words();
        prepo.Porter_Stemming();
        prepo.Conteo();

        if (!se_mantenga) {
            reset_top();
            do {
                std::cout << "¿Desea query de los ultimos k (S) o rango personalizado (N)? " << std::endl;
                std::cin >> tipo_query;
                tipo_query = (char)std::tolower(tipo_query);
            } while (tipo_query != 's' && tipo_query != 'n');

            if (tipo_query == 's') {
                ultimos_k = true;
                std::cout << "Dame los ultimos cuantos deseas" << std::endl;
                std::cin >> k_factor;
                start_moment = momento_global - k_factor + 1;
                if (start_moment < 0) {
                    start_moment = 0;
                }
                end_moment = momento_global;
            }
            else {
                ultimos_k = false;
                do {
                    std::cout << "Dame el inicio de tu rango" << std::endl;
                    std::cin >> start_moment;
                    std::cout << "Dame el fin de tu rango" << std::endl;
                    std::cin >> end_moment;
                } while (start_moment > end_moment);
                k_factor = end_moment - start_moment + 1;

            }
            do {
                std::cout << "Ingrese top m a mostrar en la wordcloud (m > 0): " << std::endl;
                std::cin >> m_factor;
            } while (m_factor <= 0);

            do {
                std::cout << "¿Desea que esta configuracion continue o se modifique? (S/N)" << std::endl;
                std::cin >> continuo;
                continuo = (char)std::tolower(continuo);
            } while (continuo != 's' && continuo != 'n');

            se_mantenga = continuo == 's' ? true : false;
        }
        start_moment = momento_global - k_factor + 1;
        if (start_moment < 0) {
            start_moment = 0;
        }
        end_moment = momento_global;
        auto inicio = std::chrono::high_resolution_clock::now();
        for (int i = 0;i < prepo.topics.size();i++) {
            arbol.Insert(prepo.topics[i].c_str(), momento_global, k_factor, m_factor, start_moment, end_moment, ultimos_k);
            
            total_topics += prepo.topics.size();
        }
        if (!se_mantenga) {
            MiArray<Topk> window;
            if (ultimos_k) {
                if (k_factor > arbol.root->size) {
                    k_factor = arbol.root->size;
                }
                int rank = arbol.root->size - k_factor + 1;
                if (rank <= 0) {
                    rank = 1;
                }
                Node* raiz_ultimos = arbol.Select(arbol.root, rank);
                arbol.GetWindowLastK(raiz_ultimos, start_moment, end_moment, window, k_factor, m_factor);
            }
            else {
                arbol.GetWindow(arbol.root, start_moment, end_moment, window, m_factor);
            }
            escribir_top_csv(window);
            system("python script_wordcloud.py topk.csv");
        }
        momento_global++;
        auto fin = std::chrono::high_resolution_clock::now();
        duracion += std::chrono::duration_cast<std::chrono::milliseconds>(fin - inicio).count();
        
    }
    MiArray<Topk> window;
    if (ultimos_k) {
        if (k_factor > arbol.root->size) {
            k_factor = arbol.root->size;
        }
        int rank = arbol.root->size - k_factor + 1;
        if (rank <= 0) {
            rank = 1;
        }
        Node* raiz_ultimos = arbol.Select(arbol.root, rank);
        arbol.GetWindowLastK(raiz_ultimos, start_moment, end_moment, window, k_factor, m_factor);
    }
    else {
        if (k_factor > arbol.root->size) {
            k_factor = arbol.root->size;
        }
        arbol.GetWindow(arbol.root, start_moment, end_moment, window, m_factor);
    }
    escribir_top_csv(window);
    std::cout << "Tiempo que tardo " << std::fixed << duracion << "ns" << std::endl;
    std::cout << "--- Wordcloud generada para esta noticia ---" << std::endl;
    system("python script_wordcloud.py topk.csv");
    arbol.preprinting();
    return 0;
}

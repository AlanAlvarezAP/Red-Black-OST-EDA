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
    char al_final;
    bool se_mantenga=false;
    std::cout << " Bienvenido al proyecto de EDA ||| RB-OST para Streaming continuo |||" << std::endl;
    uint64_t momento_global = 1;
    auto archivos = leer_archivos_carpeta("EDA input");
    int k_factor=0;
    int m_factor=0;
    int end_moment=0;
    int start_moment=0;
    bool ultimos_k=false;
    int total_topics=0;
    double duracion=0;
    double tiempo_limpieza_ns = 0;
    double tiempo_tokenizar_ns = 0;
    double tiempo_stopwords_ns = 0;
    double tiempo_stemming_ns = 0;
    double tiempo_conteo_ns = 0;
    double insert = 0;
    double total_general = 0;
    auto inicio_general = std::chrono::high_resolution_clock::now();
    for (const auto f : archivos) {
        auto inicio = std::chrono::high_resolution_clock::now();
        prepo.reset();
        auto t1 = std::chrono::high_resolution_clock::now();
        prepo.Limpieza(f);
        auto t2 = std::chrono::high_resolution_clock::now();
        tiempo_limpieza_ns += std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
        auto t11 = std::chrono::high_resolution_clock::now();
        prepo.Tokenizar();
        auto t21 = std::chrono::high_resolution_clock::now();
        tiempo_tokenizar_ns += std::chrono::duration_cast<std::chrono::nanoseconds>(t21 - t11).count();
        auto t12 = std::chrono::high_resolution_clock::now();
        prepo.Stop_words();
        auto t22 = std::chrono::high_resolution_clock::now();
        tiempo_stopwords_ns += std::chrono::duration_cast<std::chrono::nanoseconds>(t22 - t12).count();
        auto t13 = std::chrono::high_resolution_clock::now();
        prepo.Porter_Stemming();
        auto t23 = std::chrono::high_resolution_clock::now();
        tiempo_stemming_ns += std::chrono::duration_cast<std::chrono::nanoseconds>(t23 - t13).count();
        auto t14 = std::chrono::high_resolution_clock::now();
        prepo.Conteo();
        auto t24 = std::chrono::high_resolution_clock::now();
        tiempo_conteo_ns += std::chrono::duration_cast<std::chrono::nanoseconds>(t24 - t14).count();
        auto fin = std::chrono::high_resolution_clock::now();
        duracion += std::chrono::duration_cast<std::chrono::nanoseconds>(fin - inicio).count();
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


            if (se_mantenga) {
                do {
                    std::cout << "¿Desea que se realice su consulta al final del procesado (S/N)?" << std::endl;
                    std::cin >> al_final;
                    al_final = std::tolower(al_final);
                } while (al_final != 's' && al_final != 'n');
            }
            else {
                al_final = 'n';
            }
        }
        for (int i = 0;i < prepo.topics.size();i++) {
            auto t3 = std::chrono::high_resolution_clock::now();
            arbol.Insert(prepo.topics[i].c_str(), momento_global, k_factor, m_factor, ultimos_k);
            auto t4 = std::chrono::high_resolution_clock::now();
            insert += std::chrono::duration_cast<std::chrono::nanoseconds>(t4 - t3).count();
        }
        total_topics += prepo.topics.size();
        if (al_final == 'n') {
            MiArray<Topk> window;
            if (ultimos_k) {
                int k_factor_cambiable = k_factor;
                start_moment = momento_global - k_factor_cambiable + 1;
                if (start_moment < 0) {
                    start_moment = 0;
                }
                end_moment = momento_global;
                if (k_factor_cambiable > arbol.root->size) {
                    k_factor_cambiable = arbol.root->size;
                }
                int rank = arbol.root->size - k_factor_cambiable + 1;
                if (rank <= 0) {
                    rank = 1;
                }
                Node* raiz_ultimos = arbol.Select(arbol.root, rank);
                arbol.GetWindowLastK(raiz_ultimos, start_moment, end_moment, window, k_factor_cambiable, m_factor);
            }
            else {
                arbol.GetWindow(arbol.root, start_moment, end_moment, window, m_factor);
            }
            escribir_top_csv(window);
            system("python script_wordcloud.py topk.csv");
        }
        momento_global++;

    }
    
    MiArray<Topk> window;
    double duracion_query = 0;
    if (ultimos_k) {
        int k_factor_cambiable = k_factor;
        start_moment = momento_global - k_factor_cambiable + 1;
        if (start_moment < 0) {
            start_moment = 0;
        }
        end_moment = momento_global;
        if (k_factor_cambiable > arbol.root->size) {
            k_factor_cambiable = arbol.root->size;
        }
        int rank = arbol.root->size - k_factor_cambiable + 1;
        if (rank <= 0) {
            rank = 1;
        }
        Node* raiz_ultimos = arbol.Select(arbol.root, rank);
        auto t5 = std::chrono::high_resolution_clock::now();
        arbol.GetWindowLastK(raiz_ultimos, start_moment, end_moment, window, k_factor_cambiable, m_factor);
        auto t6 = std::chrono::high_resolution_clock::now();
        duracion_query = std::chrono::duration_cast<std::chrono::nanoseconds>(t6 - t5).count();
    }
    else {
        arbol.GetWindow(arbol.root, start_moment, end_moment, window, m_factor);
    }
    auto fin_general = std::chrono::high_resolution_clock::now();
    auto duracion_general = std::chrono::duration_cast<std::chrono::nanoseconds>(fin_general - inicio_general).count();;
    escribir_top_csv(window);
    std::cout << "--- Wordcloud generada para esta noticia ---" << std::endl;
    system("python script_wordcloud.py topk.csv");
    arbol.preprinting();
    std::cout << "TIEMPO DE TODO " << std::fixed << duracion_general << "ns" << std::endl;
    std::cout << "Tiempo de query final " << std::fixed << duracion_query << "ns" << std::endl;
    std::cout << "Tiempo que tardo insert " << std::fixed << insert << "ns" << std::endl;
    std::cout << "Tiempo que tardo toda la prepo " << std::fixed << duracion << "ns" << std::endl;
    std::cout << "--------------------------------------------\n";
    std::cout << "Tiempo total LIMPIEZA:        " << std::fixed << tiempo_limpieza_ns << " ns\n";
    std::cout << "Tiempo total TOKENIZACION:    " << std::fixed << tiempo_tokenizar_ns << " ns\n";
    std::cout << "Tiempo total STOP WORDS:      " << std::fixed << tiempo_stopwords_ns << " ns\n";
    std::cout << "Tiempo total STEMMING:        " << std::fixed << tiempo_stemming_ns << " ns\n";
    std::cout << "Tiempo total CONTEO:          " << std::fixed << tiempo_conteo_ns << " ns\n";
    std::cout << "--------------------------------------------\n";
    return 0;
}

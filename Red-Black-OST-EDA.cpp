#include <iostream>
#include "Preprocesamiento.h"
#include "RB_OST.h"


int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    Preprocesador prepo("stop_words.txt");
    std::string dir;
    std::cout << " Bienvenido al proyecto de EDA RB-OST dame el nombre del primer archivo" << "\n";
    std::cin >> dir;
    prepo.Limpieza(dir);
    prepo.Tokenizar();
    prepo.Porter_Stemming();
}

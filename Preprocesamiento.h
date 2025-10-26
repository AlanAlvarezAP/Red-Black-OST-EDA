#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <string>

class Preprocesador {
	std::vector<std::string> stop_words;
public:
	Preprocesador();
	void Limpieza(std::string& texto);


};
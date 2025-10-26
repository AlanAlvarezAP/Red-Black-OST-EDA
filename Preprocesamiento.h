#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <unordered_set>
#include <fstream>
#include <regex>

class Preprocesador {
	std::unordered_set<std::string> stop_words;
	std::string final_text;
	std::vector<std::string> tokens;
	std::ifstream lector;
	std::regex regex;
public:
	Preprocesador(const std::string& stop);
	void Limpieza(const std::string& texto);
	void Tokenizar();

};
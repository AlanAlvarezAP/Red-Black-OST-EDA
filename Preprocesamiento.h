#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <unordered_set>
#include <fstream>
#include <regex>
#include <unordered_map>


class Preprocesador {
	std::unordered_set<std::string> stop_words;
	std::string final_text;
	std::vector<std::string> tokens;
	std::ifstream lector;
public:
	std::vector<std::string> topics;
	Preprocesador(const std::string& stop);
	void Limpieza(const std::string& texto);
	void Tokenizar();
	void Stop_words();
	void Porter_Stemming();
	std::vector<std::string> generar_ngrams(const std::vector<std::string>& tokens, int n);
	void Conteo();
	void print();
	void reset();
};
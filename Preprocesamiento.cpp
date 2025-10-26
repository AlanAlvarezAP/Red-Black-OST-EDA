#include "Preprocesamiento.h"


Preprocesador::Preprocesador(const std::string& stop) {
	std::string read;
	lector.open(stop);
	while (lector >> read) {
		stop_words.insert(read);
	}
	lector.close();
}


// Posible mejora toda función con to... puede dar error si hay tildes seria static_cast<unsigned char> en ese caso preguntar
void Preprocesador::Limpieza(const std::string& texto) {
	std::string lineas;
	final_text.clear();
	lector.open(texto);
	while (std::getline(lector,lineas)) {
		for (unsigned char c : lineas) {
			if (c == '\t' || c == '\n') {
				final_text.push_back(' ');
			}
			else if (c == '\'' || c == ' ') {
				final_text.push_back(c);
			}
			else if (std::isalpha(c)) {
				final_text.push_back(std::tolower(c));
			}
		}
		final_text.push_back(' ');
	}
	final_text = std::regex_replace(final_text, std::regex("^\\s+|\\s+$"), "");
	final_text = std::regex_replace(final_text, std::regex("\\s+"), " ");
}

void Preprocesador::Tokenizar() {
	std::string token;
	for (int i = 0;i < final_text.size();i++) {
		if (final_text[i] != ' ') {
			token.push_back(final_text[i]);
		}
		else {
			tokens.push_back(token);
			token.clear();
		}
	}
	if (!token.empty()) {
		tokens.push_back(token);
	}
}
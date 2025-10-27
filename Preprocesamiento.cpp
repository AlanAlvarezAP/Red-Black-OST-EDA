#include "Preprocesamiento.h"
#include "Porter.h"

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

void Preprocesador::Porter_Stemming() {
	for (int i = 0; i < tokens.size(); i++) {
		if (tokens[i] == "crunchy") {
			std::cout << "Lo encontre " << std::endl;
		}
		Porter port(tokens[i]);
		port.paso_1_a_b();
		port.paso_1_c();
		port.paso_2();
		port.paso_3();
		port.paso_4();
		port.paso_5_a_b();
		tokens[i] = port.copy_of_original;
		std::cout << tokens[i] << std::endl;
	}
}

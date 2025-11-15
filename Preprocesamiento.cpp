#include "Preprocesamiento.h"
#include "Porter.h"

Preprocesador::Preprocesador(const std::string& stop) {
	std::string read;
	lector.open(stop);
	if (!lector.is_open()) {
		std::cout << " No se pudo abrir el archivo en el constructor " << stop << std::endl;
		return;
	}
	while (lector >> read) {
		stop_words.insert(read);
	}
	lector.close();
}


void Preprocesador::Limpieza(const std::string& texto) {
	std::string lineas;
	lector.open(texto);
	if (!lector.is_open()) {
		std::cout << " No se puedo leer el archivo " << std::endl;
		return;
	}
	final_text.reserve(100000);
	bool space = true;

	while (std::getline(lector,lineas)) {
		for (unsigned char c : lineas) {
			c = std::tolower(c);
			if (c >= 'a' && c <= 'z') {
				final_text.push_back(c);
				space = false;
			}
			else {
				if (!space) {
					final_text.push_back(' ');
					space = true;
				}
			}
		}
		if (!space) {
			final_text.push_back(' ');
			space = true;
		}
	}
	if (!final_text.empty() && final_text.back() == ' ') {
		final_text.pop_back();
	}
	lector.close();
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

void Preprocesador::Stop_words() {
	std::vector<std::string> tokens_noStop;
	for (const auto& tok : tokens) {
		if (stop_words.find(tok) == stop_words.end()) {
			tokens_noStop.push_back(tok);
		}
	}
	tokens = std::move(tokens_noStop);
	
}

void Preprocesador::Porter_Stemming() {
	for (int i = 0; i < tokens.size(); i++) {
		Porter port(tokens[i]);
		port.paso_1_a_b();
		port.paso_1_c();
		port.paso_2();
		port.paso_3();
		port.paso_4();
		port.paso_5_a_b();
		tokens[i] = port.copy_of_original;

	}
	
}

std::vector<std::string> Preprocesador::generar_ngrams(const std::vector<std::string>& tokens, int n) {
	std::vector<std::string> ngrams;
	if (tokens.size() < n) {
		return ngrams;
	}

	for (int i = 0; i <= tokens.size() - n; i++) {
		std::string ngram = tokens[i];
		for (int j = 1; j < n; j++) {
			ngram += " " + tokens[i + j];
		}
		ngrams.push_back(ngram);
	}
	return ngrams;
}


void Preprocesador::Conteo() {
	std::unordered_map<std::string, int> conteo_unigram;
	std::unordered_map<std::string, int> conteo_bigrams;
	std::unordered_map<std::string, int> conteo_trigrams;
	for (const auto& p : tokens) {
		conteo_unigram[p]++;
	}

	int max_freq = INT_MIN;
	for (auto& p : conteo_unigram) {
		if (p.second > max_freq) {
			max_freq = p.second;
		}
	}

	int max_empate=0;
	for (auto& p : conteo_unigram) {
		if (max_freq == p.second) {
			topics.push_back(p.first);
			max_empate++;
		}
	}
}



void Preprocesador::print() {
	std::cout << "Topicos de la notica " << std::endl;
	for (const auto& p : topics) {
		std::cout << p << " | ";
	}
	std::cout << std::endl;
}

void Preprocesador::reset() {
	final_text.clear();
	tokens.clear();
	topics.clear();
}
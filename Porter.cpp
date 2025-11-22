#include "Porter.h"


Porter::Porter(std::string& str) {
	copy_of_original = str;
	auxiliar_index=inicio_index = measure = 0;
	end_index = str.size() - 1;
}

bool Porter::verificar_consonante(int index) {
	char ch = copy_of_original[index];
	switch (ch) {
		case 'a':
		case 'e':
		case 'i':
		case 'o':
		case 'u':
			return false;
		case 'y':
			if (index == inicio_index) {
				return true;
			}
			else {
				return !(verificar_consonante(index - 1));
			}
		default:
			break;
	}
	return true;
}

int Porter::m() {
	int contar_VC = 0;
	int inicio = inicio_index;

	while (true) {
		if (inicio > auxiliar_index) {
			return contar_VC;
		}
		if (!(verificar_consonante(inicio))) {
			break;
		}
		inicio++;
	}
	inicio++;

	while (true) {
		while (true) {
			if (inicio > auxiliar_index) {
				return contar_VC;
			}
			if (verificar_consonante(inicio)) {
				break;
			}
			inicio++;
		}
		inicio++;
		contar_VC++;
		while (true) {
			if (inicio > auxiliar_index) {
				return contar_VC;
			}
			if (!(verificar_consonante(inicio))) {
				break;
			}
			inicio++;
		}
		inicio++;
	}
	return contar_VC;
}

bool Porter::vocal_en_rango() {
	for (int i = inicio_index;i <= auxiliar_index;i++) {
		if (!verificar_consonante(i)) {
			return true;
		}
	}
	return false;
}

bool Porter::doble_consonante(int j) {
	if (j == inicio_index) {
		return false;
	}
	if (copy_of_original[j] != copy_of_original[j - 1]) {
		return false;
	}
	return verificar_consonante(j);
}

bool Porter::ver_cvc(int j) {
	if (j <= inicio_index + 1) {
		return false;
	}
	if (!verificar_consonante(j - 2) || verificar_consonante(j - 1) || !verificar_consonante(j)) {
		return false;
	}
	char ch = copy_of_original[j];
	if (ch == 'w' || ch == 'x' || ch == 'y') {
		return false;
	}
	return true;
}

bool Porter::check_fin(std::string str) {
	int size_word = copy_of_original.size();
	int size_other = str.size();
	if (size_other > size_word) {
		return false;
	}
	for (int i = 0;i < size_other;i++) {
		if(str[i] != copy_of_original[size_word-size_other + i]) {
			return false;
		}
	}
	auxiliar_index = size_word-size_other-1;
	return true;
}

void Porter::cambiar_rango(std::string str) {
	int start = auxiliar_index + 1;
	int longitud = end_index - auxiliar_index;
	copy_of_original.replace(start, longitud, str);
	end_index = start + (int)str.size() - 1;
	
}

void Porter::paso_1_a_b() {
	if (copy_of_original[end_index] == 's') {
		if (check_fin("sses")) {
			cambiar_rango("ss");
		}
		else if (check_fin("ies")) {
			cambiar_rango("i");
		}
		else if (copy_of_original[end_index - 1] != 's') {
			end_index--;
			copy_of_original.resize(end_index + 1);
			auxiliar_index = end_index;
		}
	}
	if (check_fin("eed")) {
		if (m() > 0) {
			end_index--;
			copy_of_original.resize(end_index + 1);
			auxiliar_index = end_index;
		}
	}
	else if ((check_fin("ed") || check_fin("ing")) && vocal_en_rango()) {
		int old_aux = auxiliar_index;
		end_index = auxiliar_index;
		copy_of_original.resize(end_index + 1);
		auxiliar_index = old_aux;
		if (check_fin("at")) {
			cambiar_rango("ate");
		}
		else if (check_fin("bl")) {
			cambiar_rango("ble");
		}
		else if (check_fin("iz")) {
			cambiar_rango("ize");
		}
		else if (doble_consonante(end_index)) {
			char last = copy_of_original[end_index];
			if (last != 'l' && last != 's' && last != 'z') {
				end_index--;
				copy_of_original.resize(end_index + 1);
				auxiliar_index = end_index;
			}
		}
		else if (m() == 1 && ver_cvc(end_index)) {
			cambiar_rango("e");
		}
	}
}

void Porter::paso_1_c() {
	if (check_fin("y") && vocal_en_rango()){
		cambiar_rango("i");
	}
}

void Porter::paso_2() {
	char ch = (end_index >= 1) ? copy_of_original[end_index - 1] : '\0';
	switch (ch) {
		case 'a':
			if (check_fin("ational") && m() > 0) {
				cambiar_rango("ate");
			}
			else if (check_fin("tional") && m() > 0) {
				cambiar_rango("tion");
			}
			break;
		case 'c':
			if (check_fin("enci") && m() > 0) {
				cambiar_rango("ence");
			}
			else if (check_fin("anci") && m() > 0) {
				cambiar_rango("ance");
			}
			break;
		case 'e':
			if (check_fin("izer") && m() > 0) {
				cambiar_rango("ize");
			}
			break;
		case 'l':
			if (check_fin("abli") && m() > 0) {
				cambiar_rango("able");
			}
			else if (check_fin("alli") && m() > 0) {
				cambiar_rango("al");
			}
			else if (check_fin("entli") && m() > 0) {
				cambiar_rango("ent");
			}
			else if (check_fin("eli") && m() > 0) {
				cambiar_rango("e");
			}
			else if (check_fin("ousli") && m() > 0) {
				cambiar_rango("ous");
			}
			break;
		case 'o':
			if (check_fin("ization") && m() > 0) {
				cambiar_rango("ize");
			}
			else if (check_fin("ation") && m() > 0) {
				cambiar_rango("ate");
			}
			else if (check_fin("ator") && m() > 0) {
				cambiar_rango("ate");
			}
			break;
		case 's':
			if (check_fin("alism") && m() > 0) {
				cambiar_rango("al");
			}
			else if (check_fin("iveness") && m() > 0) {
				cambiar_rango("ive");
			}
			else if (check_fin("fulness") && m() > 0) {
				cambiar_rango("ful");
			}
			else if (check_fin("ousness") && m() > 0) {
				cambiar_rango("ous");
			}
			break;
		case 't':
			if (check_fin("aliti") && m() > 0) {
				cambiar_rango("al");
			}
			else if (check_fin("iviti") && m() > 0) {
				cambiar_rango("ive");
			}
			else if (check_fin("biliti") && m() > 0) {
				cambiar_rango("ble");
			}
			break;
		case 'g':
			if (check_fin("logi") && m() > 0) {
				cambiar_rango("log");
			}
			break;
		default:
			break;
	}
}

void Porter::paso_3() {
	switch (copy_of_original[end_index]) {
		case 'e': {
			if (check_fin("icate") && m() > 0) {
				cambiar_rango("ic");
			}
			else if (check_fin("ative") && m() > 0) {
				cambiar_rango("");
			}
			else if (check_fin("alize") && m() > 0) {
				cambiar_rango("al");
			}
			break;
		}
		case 'i': {
			if (check_fin("iciti") && m() > 0) {
				cambiar_rango("ic");
			}
			break;
		}
		case 'l': {
			if (check_fin("ical") && m() > 0) {
				cambiar_rango("ic");
			}
			else if (check_fin("ful") && m() > 0) {
				cambiar_rango("");
			}
			break;
		}
		case 's': {
			if (check_fin("ness") && m() > 0) {
				cambiar_rango("");
			}
			break;
		}
		default: {
			break;
		}
	}
}

void Porter::paso_4() {
	char ch = (end_index >= 1) ? copy_of_original[end_index - 1] : '\0';
	switch (ch) {
		case 'a': {
			if (check_fin("al") && m() > 1) {
				cambiar_rango("");
			}
			break;
		}
		case 'c': {
			if (check_fin("ance") && m() > 1) {
				cambiar_rango("");
			}
			else if (check_fin("ence") && m() > 1) {
				cambiar_rango("");
			}
			break;
		}
		case 'e': {
			if (check_fin("er") && m() > 1) {
				cambiar_rango("");
			}
			break;
		}
		case 'i': {
			if (check_fin("ic") && m() > 1) {
				cambiar_rango("");
			}
			break;
		}
		case 'l': {
			if (check_fin("able") && m() > 1) {
				cambiar_rango("");
			}
			else if (check_fin("ible") && m() > 1) {
				cambiar_rango("");
			}
			break;
		}
		case 'n': {
			if (check_fin("ant") && m() > 1) {
				cambiar_rango("");
			}
			else if (check_fin("ement") && m() > 1) {
				cambiar_rango("");
			}
			else if (check_fin("ment") && m() > 1) {
				cambiar_rango("");
			}
			else if (check_fin("ent") && m() > 1) {
				cambiar_rango("");
			}
			break;
		}
		case 'o': {
			if (check_fin("ion") && auxiliar_index >= inicio_index && (copy_of_original[auxiliar_index] == 's' || copy_of_original[auxiliar_index] == 't') && m() > 1) {
				cambiar_rango("");
			}
			else if (check_fin("ou") && m() > 1) {
				cambiar_rango("");
			}
			break;
		}
		case 's': {
			if (check_fin("ism") && m() > 1) {
				cambiar_rango("");
			}
			break;
		}
		case 't': {
			if (check_fin("ate") && m() > 1) {
				cambiar_rango("");
			}
			else if (check_fin("iti") && m() > 1) {
				cambiar_rango("");
			}
			break;
		}
		case 'u': {
			if (check_fin("ous") && m() > 1) {
				cambiar_rango("");
			}
			break;
		}
		case 'v': {
			if (check_fin("ive") && m() > 1) {
				cambiar_rango("");
			}
			break;
		}
		case 'z': {
			if (check_fin("ize") && m() > 1) {
				cambiar_rango("");
			}
			break;
		}
		default: {
			break;
		}
	}
}

void Porter::paso_5_a_b() {
	if (check_fin("e")) {
		int m_val = m();
		if (m_val > 1) {
			cambiar_rango("");
		}
		else if (m_val == 1 && !ver_cvc(end_index - 1)) {
			cambiar_rango("");
		}
	}
	if (check_fin("l") && m() > 1 && doble_consonante(end_index)) {
		end_index--;
		copy_of_original.resize(end_index + 1);
		auxiliar_index = end_index;
	}
}
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
	return false;
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
	end_index = start + (int)str.size();
}

void Porter::paso_1_a_b() {
	if (copy_of_original[end_index] == 's') {
		if (check_fin("sses")) {
			cambiar_rango("ss");
		}
		else if (check_fin("ies")) {
			cambiar_rango("i");
		}
		else if (check_fin("s")) {
			cambiar_rango("");
		}
	}
	if (m() > 0 && check_fin("eed")) {
		cambiar_rango("ee");
	}
	else if (vocal_en_rango() && (check_fin("ing")|| check_fin("ed"))) {
		cambiar_rango("");
		if (check_fin("at")) {
			cambiar_rango("ate");
		}
		else if (check_fin("bl")) {
			cambiar_rango("ble");
		}
		else if (check_fin("iz")) {
			cambiar_rango("ize");
		}
		else if (doble_consonante(end_index) && (copy_of_original[end_index] != 'l' && copy_of_original[end_index] != 's' && copy_of_original[end_index] != 'z')) {
			cambiar_rango(std::string{ copy_of_original[end_index] });
		}
		else if (m() == 1 && ver_cvc(end_index)) {
			cambiar_rango("e");
		}
	}
}

void Porter::paso_1_c() {
	if (vocal_en_rango() && check_fin("y")) {
		cambiar_rango("i");
	}
}

void Porter::paso_2() {
	if (m() > 0) {
		char ch = (end_index >= 1) ? copy_of_original[end_index - 1] : '\0';
		switch (ch) {
			case 'a': {
				if (check_fin("ational")) {
					cambiar_rango("ate");
				}
				else if (check_fin("tional")) {
					cambiar_rango("tion");
				}
				break;
			}
			case 'c': {
				if (check_fin("enci")) {
					cambiar_rango("ence");
				}
				else if (check_fin("anci")) {
					cambiar_rango("ance");
				}
				break;
			}
			case 'e': {
				if (check_fin("izer")) {
					cambiar_rango("ize");
				}
				break;
			}
			case 'l': {
				if (check_fin("abli")) {
					cambiar_rango("able");
				}
				else if (check_fin("alli")) {
					cambiar_rango("al");
				}
				else if (check_fin("entli")) {
					cambiar_rango("ent");
				}
				else if (check_fin("eli")) {
					cambiar_rango("e");
				}
				else if (check_fin("ousli")) {
					cambiar_rango("ous");
				}
				break;
			}
			case 'o':{
				if (check_fin("ization")) {
					cambiar_rango("ize");
				}
				else if (check_fin("ation")) {
					cambiar_rango("ate");
				}
				else if (check_fin("ator")) {
					cambiar_rango("ate");
				}
				break;
			}
			case 's': {
				if (check_fin("alism")) {
					cambiar_rango("al");
				}
				else if (check_fin("iveness")) {
					cambiar_rango("ive");
				}
				else if (check_fin("fulness")) {
					cambiar_rango("ful");
				}
				else if (check_fin("ousness")) {
					cambiar_rango("ous");
				}
				break;
			}
			case 't': {
				if (check_fin("aliti")) {
					cambiar_rango("al");
				}
				else if (check_fin("iviti")) {
					cambiar_rango("ive");
				}
				else if (check_fin("biliti")) {
					cambiar_rango("ble");
				}
				break;
			}
			case 'g': {
				if (check_fin("logi")) {
					cambiar_rango("log");
				}
				break;
			}
			default: {
				break;
			}
		}
	}
}

void Porter::paso_3() {
	if (m() > 0) {
		switch (copy_of_original[end_index]) {
			case 'e': {
				if (check_fin("icate")) {
					cambiar_rango("ic");
				}
				else if (check_fin("ative")) {
					cambiar_rango("");
				}
				else if (check_fin("alize")) {
					cambiar_rango("al");
				}
				break;
			}
			case 'i': {
				if (check_fin("iciti")) {
					cambiar_rango("ic");
				}
				break;
			}
			case 'l': {
				if (check_fin("ical")) {
					cambiar_rango("ic");
				}
				else if (check_fin("ful")) {
					cambiar_rango("");
				}
				break;
			}
			case 's': {
				if (check_fin("ness")) {
					cambiar_rango("");
				}
				break;
			}
			default: {
				break;
			}
		}
	}
}

void Porter::paso_4() {
	if (m() > 1) {
		char ch = (end_index >= 1) ? copy_of_original[end_index - 1] : '\0';
		switch (ch) {
			case 'a': {
				if (check_fin("al")) {
					cambiar_rango("");
				}
				break;
			}
			case 'c': {
				if (check_fin("ance")) {
					cambiar_rango("");
				}
				else if (check_fin("ence")) {
					cambiar_rango("");
				}
				break;
			}
			case 'e': {
				if (check_fin("er")) {
					cambiar_rango("");
				}
				break;
			}
			case 'i': {
				if (check_fin("ic")) {
					cambiar_rango("");
				}
				break;
			}
			case 'l': {
				if (check_fin("able")) {
					cambiar_rango("");
				}
				else if (check_fin("ible")) {
					cambiar_rango("");
				}
				break;
			}
			case 'n': {
				if (check_fin("ant")) {
					cambiar_rango("");
				}
				else if (check_fin("ement")) {
					cambiar_rango("");
				}
				else if (check_fin("ment")) {
					cambiar_rango("");
				}
				else if (check_fin("ent")) {
					cambiar_rango("");
				}
				break;
			}
			case 'o': {
				if (check_fin("ion") && auxiliar_index >= inicio_index && (copy_of_original[auxiliar_index] == 's' || copy_of_original[auxiliar_index] == 't')) {
					cambiar_rango("");
				}
				else if (check_fin("ou")) {
					cambiar_rango("");
				}
				break;
			}
			case 's': {
				if (check_fin("ism")) {
					cambiar_rango("");
				}
				break;
			}
			case 't': {
				if (check_fin("ate")) {
					cambiar_rango("");
				}
				else if (check_fin("iti")) {
					cambiar_rango("");
				}
				break;
			}
			case 'u': {
				if (check_fin("ous")) {
					cambiar_rango("");
				}
				break;
			}
			case 'v': {
				if (check_fin("ive")) {
					cambiar_rango("");
				}
				break;
			}
			case 'z': {
				if (check_fin("ize")) {
					cambiar_rango("");
				}
				break;
			}
			default: {
				break;
			}
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
	if (m() > 1 && doble_consonante(end_index) && check_fin("ll")) {
		cambiar_rango("l");
	}
}
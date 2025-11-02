#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

class Porter {
public:
	int measure,inicio_index,end_index,auxiliar_index;
	std::string copy_of_original;

	Porter(std::string& str);
	bool verificar_consonante(int index);
	int m();

	bool vocal_en_rango();
	bool doble_consonante(int j);
	bool ver_cvc(int j);
	bool check_fin(std::string str);
	void cambiar_rango(std::string str);

	void paso_1_a_b();
	void paso_1_c();
	void paso_2();
	void paso_3();
	void paso_4();
	void paso_5_a_b();
};
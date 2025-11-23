#include "EstructurasAuxiliares.h"
#include "RB_OST.h"

HashNode::HashNode(char* ke, Node* val, HashNode* ne) {
	value = val;
	next = ne;
	int len = 0;
	const char* tmp = ke;
	while (*tmp != '\0') {
		len++;
		tmp++;
	}
	key = new char[len + 1];
	tmp = ke;
	for (int i = 0; i < len; i++) {
		key[i] = tmp[i];
	}
	key[len] = '\0';
}

UnorderedHash::UnorderedHash() {
	size = 1000;
	tabla = new HashNode * [size];
	for (int i = 0;i < size;i++) {
		tabla[i] = nullptr;
	}
	hash_prime = 1099511628211ULL;
	load_factor = 0.7;
	num_elements = 0;
}
UnorderedHash::~UnorderedHash() {
	for (int i = 0; i < size; i++) {
		HashNode* curr = tabla[i];
		while (curr) {
			HashNode* tmp = curr;
			curr = curr->next;
			delete tmp;
		}
	}
	delete[] tabla;
}
int UnorderedHash::Hash_Func(const char* str, int sizee) {
	unsigned long long hash_offset = 14695981039346656037ULL;
	for (int i = 0; str[i]; i++) {
		hash_offset ^= static_cast<unsigned long long>(str[i]);
		hash_offset *= hash_prime;
	}
	return static_cast<int>(hash_offset % sizee);
}

bool UnorderedHash::char_cmp(const char* first, const char* second) {
	while (*first && *second) {
		if (*first != *second) {
			return false;
		}
		first++;
		second++;
	}

	return *first == '\0' && *second == '\0';
}

void UnorderedHash::insert_hash(Node* nodo) {
	int index = Hash_Func(nodo->topico, size);
	HashNode* curr = tabla[index];
	while (curr) {
		if (char_cmp(nodo->topico, curr->key)) {
			curr->value = nodo;
			return;
		}
		curr = curr->next;
	}
	HashNode* new_node = new HashNode(nodo->topico, nodo, tabla[index]);
	tabla[index] = new_node;
	num_elements++;
	if (num_elements > size * load_factor) {
		rehash();
	}
}

HashNode* UnorderedHash::operator[](const char* str) {
	int index = Hash_Func(str, size);
	HashNode* curr = tabla[index];
	while (curr) {
		if (char_cmp(str, curr->key)) {
			return curr;
		}
		curr = curr->next;
	}
	return nullptr;
}

void UnorderedHash::rehash() {
	int new_size = size * 2;
	HashNode** new_tab = new HashNode * [new_size];
	for (int i = 0;i < new_size;i++) {
		new_tab[i] = nullptr;
	}
	for (int i = 0;i < size;i++) {
		HashNode* curr = tabla[i];
		while (curr) {
			HashNode* next = curr->next;
			int new_index = Hash_Func(curr->key, new_size);
			curr->next = new_tab[new_index];
			new_tab[new_index] = curr;
			curr = next;
		}
	}
	delete[] tabla;
	tabla = new_tab;
	size = new_size;
}

void UnorderedHash::print() {
	std::cout << " Key  |  Value" << std::endl;
	for (int i = 0;i < size;i++) {
		HashNode* curr = tabla[i];
		while (curr) {
			std::cout << " Para i " << i << std::endl;
			std::cout << curr->key << " | " << *(curr->value->momentos.last_elem()) << std::endl;
			curr = curr->next;
			if (!curr) {
				std::cout << std::endl;
			}
		}
	}
}
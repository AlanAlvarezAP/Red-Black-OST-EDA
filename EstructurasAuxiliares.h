#pragma once
#include <iostream>
class Node;

class HashNode {
public:
	char* key;
	Node* value;
	HashNode* next;
	HashNode(char* ke, Node* val, HashNode* ne = nullptr);
};

class UnorderedHash {
public:
	HashNode** tabla;
	uint64_t hash_prime;
	int size;
	float load_factor;
	int num_elements;
	UnorderedHash();
	~UnorderedHash();
	int Hash_Func(const char* str, int sizee);
	bool char_cmp(const char* first, const char* second);
	void insert_hash(Node* nodo);
	HashNode* operator[](const char* str);
	void rehash();
	void print();
};
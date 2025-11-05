#pragma once
#include <iostream>
#include <vector>
#include "EstructurasAuxiliares.h"
#include "MiArray.h"

class Node {
public:
	char* topico;
	bool color;
	int size;
	MiArray<uint64_t> momentos;
	Node* left;
	Node* right;
	Node* parent;
	uint64_t max_moment_subtree,frecuencia_ventana,frecuencia;

	Node(const char* noti, uint64_t mome, bool col, uint64_t max_moment, Node* l, Node* r, Node* p , int siz = 1);

};

class RB_OST {
public:
	Node* root;
	Node* nil;
	UnorderedHash hash;


	RB_OST();
	void Insert(const char* noticia, uint64_t moment);
	void AjustarForma(Node* raiz);
	void RotacionIzq(Node* raiz);
	void RotacionDer(Node* raiz);
	void ActualizarMetadata(Node* raiz);
	Node* Select(Node* raiz,int smallest_key);
	void printing(Node* raiz,int nivel = 0);
	void preprinting();
};
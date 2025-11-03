#pragma once
#include <iostream>
#include <vector>
#include "EstructurasAuxiliares.h"

class Node {
public:
	MiArray<const char*> topicos;
	bool color;
	int size;
	uint64_t momentos;
	Node* left;
	Node* right;
	Node* parent;
	uint64_t max_moment_subtree;

	Node(uint64_t mome, bool col, uint64_t max_moment, Node* l, Node* r, Node* p , int siz = 1);

};

class RB_OST {
	Node* root;
	Node* nil;
public:
	RB_OST();
	void Insert(MiArray<const char*> noticia, uint64_t moment);
	void AjustarForma(Node* raiz);
	void RotacionIzq(Node* raiz);
	void RotacionDer(Node* raiz);
	void ActualizarMetadata(Node* raiz);
	void printing(Node* raiz,int nivel = 0);
	void preprinting();
};
#pragma once
#include <iostream>
#include "EstructurasAuxiliares.h"
#include "MiArray.h"

struct Topk {
	Node* nodo;
	int freq;
};

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
	void Insert(const char* noticia, uint64_t moment,int k_factor, int m_factor, int start_moment, int end_moment, bool ultimos_k);
	void AjustarForma(Node* raiz);
	void RotacionIzq(Node* raiz);
	void RotacionDer(Node* raiz);
	void ActualizarMetadata(Node* raiz);
	void AumentarTop(MiArray<Topk>& top, Node* nodo, int freq, int m_factor);
	void GetWindow(Node* raiz, int start_moment, int end_moment, MiArray<Topk>& arr, int m_factor);
	void GetWindowLastK(Node* raiz, int start_moment, int end_moment, MiArray<Topk>& arr, int k_factor, int m_factor);
	Node* Select(Node* raiz,int smallest_key);
	void Transplant(Node* node_1, Node* node_2);
	void DeleteFix(Node* raiz);
	Node* Delete(const char* str);
	void printing(Node* raiz,int nivel = 0);
	void preprinting();
};
#include "RB_OST.h"

Node::Node(uint64_t mome,bool col,uint64_t max_moment, Node* l, Node* r, Node* p, int siz) {
	color = col;
	left = l;
	right = r;
	parent = p;
	max_moment_subtree = max_moment;
	momentos = mome;
	size = siz;
}

RB_OST::RB_OST() {
	nil = new Node(0,0,0,nullptr,nullptr,nullptr);
    nil->left = nil;
    nil->right = nil;
    nil->parent = nil;
	root = nil;
	root->parent=root->right = root->left = nil;
}

void RB_OST::ActualizarMetadata(Node* raiz) {
    if (raiz == nil) {
        return;
    }
    
    raiz->size = 1 + raiz->left->size + raiz->right->size;
    raiz->max_moment_subtree = raiz->momentos;
    if (raiz->max_moment_subtree < raiz->left->max_moment_subtree) {
        raiz->max_moment_subtree = raiz->left->max_moment_subtree;
    }
    if (raiz->max_moment_subtree < raiz->right->max_moment_subtree) {
        raiz->max_moment_subtree = raiz->right->max_moment_subtree;
    }
}

void RB_OST::RotacionIzq(Node* raiz) {
    Node* hijo = raiz->right;
    raiz->right = hijo->left;
    if (hijo->left != nil) {
        hijo->left->parent = raiz;
    }
    hijo->parent = raiz->parent;
    if (raiz->parent == nil) {
        root = hijo;
    }
    else if (raiz == raiz->parent->left) {
        raiz->parent->left = hijo;
    }
    else {
        raiz->parent->right = hijo;
    }
    hijo->left = raiz;
    raiz->parent = hijo;

    ActualizarMetadata(raiz);
    ActualizarMetadata(hijo);

    for (Node* tmp = raiz->parent;tmp != nil;tmp = tmp->parent) {
        ActualizarMetadata(tmp);
    }

}

void RB_OST::RotacionDer(Node* raiz) {
    Node* hijo = raiz->left;
    raiz->left = hijo->right;
    if (hijo->right != nil) {
        hijo->right->parent = raiz;
    }
    hijo->parent = raiz->parent;
    if (raiz->parent == nil) {
        root = hijo;
    }
    else if (raiz == raiz->parent->right) {
        raiz->parent->right = hijo;
    }
    else {
        raiz->parent->left = hijo;
    }
    hijo->right = raiz;
    raiz->parent = hijo;

    ActualizarMetadata(raiz);
    ActualizarMetadata(hijo);
    for (Node* tmp = raiz->parent;tmp != nil;tmp = tmp->parent) {
        ActualizarMetadata(tmp);
    }
}

void RB_OST::AjustarForma(Node* raiz) {
    if (raiz == root) {
        root->color = 0;
        return;
    }
    Node* parent = raiz->parent;
    Node* grandparent = parent->parent;
    if (!grandparent) {
        return;
    }
    Node* uncle = (grandparent->left == parent) ? grandparent->right : grandparent->left;

    if (parent->color && uncle->color) {
        parent->color = 0;
        uncle->color = 0;
        grandparent->color = 1;
        AjustarForma(grandparent);
        root->color = 0;
        return;
    }
    else if (parent->color && !uncle->color) {
        if (parent==grandparent->left) {
            if (parent->right == raiz) {
                RotacionIzq(parent);
            }
            parent->color = 0;
            grandparent->color = 1;
            RotacionDer(grandparent);
        }
        else {
            if (parent->left == raiz) {
                RotacionDer(parent);
            }
            parent->color = 0;
            grandparent->color = 1;
            RotacionIzq(grandparent);
        }
    }
    
    root->color = 0;
}

void RB_OST::Insert(MiArray<const char*> noticia, uint64_t moment) {
    Node** raiz = &root;
    Node* prev_parent = nil;
    preprinting();
    while ((*raiz) != nil) {
        (*raiz)->size++;
        if (moment > (*raiz)->max_moment_subtree) {
            (*raiz)->max_moment_subtree = moment;
        }
        prev_parent = *raiz;
        raiz = (moment < (*raiz)->momentos) ? &((*raiz)->left) : &((*raiz)->right);
    }
    *raiz = new Node(moment, 1, moment, nil, nil, prev_parent);
    (*raiz)->topicos = noticia;
    AjustarForma(*raiz);
    root->color = 0;
}

void RB_OST::printing(Node* raiz, int nivel) {
    if (raiz == nil) {
        return;
    }
    for (int i = 0; i < nivel; ++i) {
        std::cout << "    ";
    }
    std::cout << (raiz->color ? "R " : "B ") << raiz->size << " " << raiz->momentos << " " << raiz->max_moment_subtree << std::endl;

    printing(raiz->left, nivel + 1);
    printing(raiz->right, nivel + 1);
}

void RB_OST::preprinting() {
    std::cout << "Datos desde la raiz";
    std::cout << "color | size | momentos | max_moment " << std::endl;
    printing(root);
}
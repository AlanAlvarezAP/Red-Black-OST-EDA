#include "RB_OST.h"

Node::Node(const char* noti,uint64_t mome, bool col, uint64_t max_moment, Node* l, Node* r, Node* p, int siz) {
	color = col;
	left = l;
	right = r;
	parent = p;
	max_moment_subtree = max_moment;
	momentos.push_back(mome);
    frecuencia = frecuencia_ventana = 1;
	size = siz;
    int len = 0;
    const char* tmp = noti;
    while (*tmp != '\0') {
        len++;
        tmp++;
    }
    topico = new char[len + 1];
    tmp = noti;
    for (int i = 0; i < len; i++) {
        topico[i] = tmp[i];
    }
    topico[len] = '\0';
}

RB_OST::RB_OST() {
	nil = new Node("",0, 0, 0, nullptr, nullptr, nullptr, 0);
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
    raiz->max_moment_subtree = *(raiz->momentos.last_elem());
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
        ActualizarMetadata(raiz);
        AjustarForma(grandparent);
        root->color = 0;
        return;
    }
    else if (parent->color && !uncle->color) {
        if (parent==grandparent->left) {
            if (parent->right == raiz) {
                RotacionIzq(parent);
                raiz = parent;
                parent = raiz->parent;
            }
            parent->color = 0;
            grandparent->color = 1;
            RotacionDer(grandparent);
        }
        else {
            if (parent->left == raiz) {
                RotacionDer(parent);
                raiz = parent;
                parent = raiz->parent;
            }
            parent->color = 0;
            grandparent->color = 1;
            RotacionIzq(grandparent);
        }
    }
    root->color = 0;
}

void RB_OST::Insert(const char* noticia, uint64_t moment) {
    Node* encontro = Delete(noticia);
    Node** raiz = &root;
    Node* prev_parent = nil;
    if (!encontro) {
        
        while ((*raiz) != nil) {
            prev_parent = *raiz;
            raiz = (moment < *((*raiz)->momentos.last_elem())) ? &((*raiz)->left) : &((*raiz)->right);
        }
        *raiz = new Node(noticia, moment, 1, moment, nil, nil, prev_parent);
        hash.insert_hash(*raiz);
        AjustarForma(*raiz);
        for (Node* tmp = (*raiz)->parent;tmp != nil;tmp = tmp->parent) {
            ActualizarMetadata(tmp);
        }
        root->color = 0;
    }
    else {
        encontro->momentos.push_back(moment);
        encontro->max_moment_subtree = moment;
        encontro->size = 1;
        encontro->color = 1;
        encontro->left = encontro->right = nil;
        // Falta actualizar la frecuencia para la ventana
        encontro->frecuencia++;
        encontro->frecuencia_ventana++;
        while ((*raiz) != nil) {
            prev_parent = *raiz;
            raiz = (moment < *((*raiz)->momentos.last_elem())) ? &((*raiz)->left) : &((*raiz)->right);
        }
        encontro->parent = prev_parent;
        *raiz = encontro;
        AjustarForma(*raiz);
        for (Node* tmp = (*raiz)->parent;tmp != nil;tmp = tmp->parent) {
            ActualizarMetadata(tmp);
        }
        root->color = 0;
    }
}

Node* RB_OST::Select(Node* raiz,int smallest_key) {
    int r = raiz->left->size + 1;
    if (smallest_key == r) {
        return raiz;
    }
    else if (smallest_key < r) {
        return Select(raiz->left, smallest_key);
    }
    else {
        return Select(raiz->right, smallest_key-r);
    }
}

void RB_OST::Transplant(Node* node_1, Node* node_2) {
    if (node_1->parent == nil) {
        root = node_2;
    }
    else if (node_1 == node_1->parent->left) {
        node_1->parent->left = node_2;
    }
    else {
        node_1->parent->right = node_2;
    }
    node_2->parent = node_1->parent;
}

void RB_OST::DeleteFix(Node* raiz) {
    while (raiz != root && !raiz->color) {
        if (raiz == raiz->parent->left) {
            Node* sibling = raiz->parent->right;
            if (sibling->color) {
                sibling->color = 0;
                raiz->parent->color = 1;
                RotacionIzq(raiz->parent);
                sibling = raiz->parent->right;
            }
            if (!sibling->left->color && !sibling->right->color) {
                sibling->color = 1;
                raiz = raiz->parent;
            }
            else {
                if (!sibling->right->color) {
                    sibling->left->color = 0;
                    sibling->color = 1;
                    RotacionDer(sibling);
                    sibling = raiz->parent->right;
                }
                sibling->color = raiz->parent->color;
                raiz->parent->color = 0;
                sibling->right->color = 0;
                RotacionIzq(raiz->parent);
                raiz = root;
            }
        }
        else {
            Node* sibling = raiz->parent->left;
            if (sibling->color) {
                sibling->color = 0;
                raiz->parent->color = 1;
                RotacionDer(raiz->parent);
                sibling = raiz->parent->left;
            }
            if (!sibling->right->color && !sibling->left->color) {
                sibling->color = 1;
                raiz = raiz->parent;
            }
            else {
                if (!sibling->left->color) {
                    sibling->right->color = 0;
                    sibling->color = 1;
                    RotacionIzq(sibling);
                    sibling = raiz->parent->left;
                }
                sibling->color = raiz->parent->color;
                raiz->parent->color = 0;
                sibling->left->color = 0;
                RotacionDer(raiz->parent);
                raiz = root;
            }
        }
        
    }
    raiz->color = 0;
}

Node* RB_OST::Delete(const char* str) {
    HashNode* find = hash[str];
    if (!find) {
        std::cout << " No se encontro el nodo :( en la hash" << std::endl;
        return nullptr;
    }
    Node* replacing_node_z = find->value;
    Node* replacing_node_y = replacing_node_z;
    Node* replacing_node_x;
    bool replacing_node_color_y = replacing_node_y->color;
    if (replacing_node_z->left == nil) {
        replacing_node_x = replacing_node_z->right;
        Transplant(replacing_node_z, replacing_node_x);
    }
    else if (replacing_node_z->right == nil) {
        replacing_node_x = replacing_node_z->left;
        Transplant(replacing_node_z, replacing_node_x);
    }
    else {
        replacing_node_y = Select(replacing_node_z->right, 1);
        replacing_node_color_y = replacing_node_y->color;
        replacing_node_x = replacing_node_y->right;
        if (replacing_node_y->parent == replacing_node_z) {
            replacing_node_x->parent = replacing_node_y;
        }
        else {
            Transplant(replacing_node_y, replacing_node_x);
            replacing_node_y->right = replacing_node_z->right;
            replacing_node_y->right->parent = replacing_node_y;
        }
        Transplant(replacing_node_z, replacing_node_y);
        replacing_node_y->left = replacing_node_z->left;
        replacing_node_y->left->parent = replacing_node_y;
        replacing_node_y->color = replacing_node_z->color;

        ActualizarMetadata(replacing_node_y);
    }
    if (!replacing_node_color_y) {
        DeleteFix(replacing_node_x);
    }
    Node* tmp = replacing_node_x->parent;
    while (tmp != nil) {
        ActualizarMetadata(tmp);
        tmp = tmp->parent;
    }
    return replacing_node_z;
}

void RB_OST::printing(Node* raiz, int nivel) {
    if (raiz == nil) {
        return;
    }
    for (int i = 0; i < nivel; ++i) {
        std::cout << "    ";
    }
    std::cout << (raiz->color ? "R " : "B ") << raiz->size << " " << *(raiz->momentos.last_elem()) << " " << raiz->max_moment_subtree << " "  << raiz->topico << std::endl;

    printing(raiz->left, nivel + 1);
    printing(raiz->right, nivel + 1);
}

void RB_OST::preprinting() {
    std::cout << "Datos desde la raiz" << std::endl;
    std::cout << "color | size | momentos | max_moment | noticia " << std::endl;
    printing(root);
}
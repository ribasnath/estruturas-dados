#ifndef MINHA_ARVORE_AVL_HPP
#define MINHA_ARVORE_AVL_HPP

#include "MinhaArvoreDeBuscaBinaria.h"
//#define nodoRaiz MInha
/**
 * @brief Representa uma árvore AVL.
 *
 * @tparam T O tipo de dado guardado na árvore.
 */
template <typename T>
class MinhaArvoreAVL final : public MinhaArvoreDeBuscaBinaria<T>
{
private:
    Nodo<T>* inseNew(T chave) {
        Nodo<T>* newNode = new Nodo<T>();
        newNode->chave = chave;
        newNode->altura = attAlt(newNode);
        this->nodoRaiz->altura = attAlt(this->nodoRaiz);

        return newNode;
    };

    void inserirAux(Nodo<T>* node, T chave) {
        if (node == nullptr) {
            inseNew(chave);
        }
        else if (chave > node->chave) {
            if (node->filhoDireita == nullptr)
                node->filhoDireita = inseNew(chave);
            else {
                inserirAux(node->filhoDireita, chave);
            }
        }
        else if (chave < node->chave) {
            if (node->filhoEsquerda == nullptr)
                node->filhoEsquerda = inseNew(chave);
            else {
                inserirAux(node->filhoEsquerda, chave);
            }
        }
        Nodo<T>* nodePai = this->retornaPai(node->chave);
        node = verificaRot(node, nodePai);
    };

    void removerAux(Nodo<T>* nodePai, T chave) {
        Nodo<T>* nodeRem = nullptr;
        if (nodePai->filhoDireita != nullptr && nodePai->filhoDireita->chave == chave)
            nodeRem = nodePai->filhoDireita;
        else if (nodePai->filhoEsquerda != nullptr && nodePai->filhoEsquerda->chave == chave)
            nodeRem = nodePai->filhoEsquerda;
        else nodeRem = nodePai;

        // se for folha direita
        if (nodePai->filhoDireita == nodeRem && nodeRem->filhoDireita == nullptr && nodeRem->filhoEsquerda == nullptr) {
            delete nodeRem;
            nodePai->filhoDireita = nullptr;
        }
        // se for folha esquerda
        else if (nodePai->filhoEsquerda == nodeRem && nodeRem->filhoDireita == nullptr && nodeRem->filhoEsquerda == nullptr) {
            delete nodeRem;
            nodePai->filhoEsquerda = nullptr;
        }

        // se tiver apenas filho à esquerda
        else if (nodeRem->filhoDireita == nullptr && nodeRem->filhoEsquerda != nullptr) {
            Nodo<T>* nodeAux = nodeRem->filhoEsquerda;
            nodePai->filhoEsquerda = nodeAux;
            delete nodeRem;
        }

        // se tiver filho à direita
        else if (nodeRem->filhoDireita != nullptr) {
            Nodo<T>* filhoDirMin = nodeRem->filhoDireita;

            while (filhoDirMin->filhoEsquerda != nullptr)
                filhoDirMin = filhoDirMin->filhoEsquerda;

            Nodo<T>* paiDoDirMin = this->retornaPai(filhoDirMin->chave);

            nodeRem->chave = filhoDirMin->chave;
            filhoDirMin->chave = chave;

            removerAux(paiDoDirMin, filhoDirMin->chave);
            Nodo<T>* nodePaidoPai = this->retornaPai(nodePai->chave);
            nodePai = verificaRot(nodePai, nodePaidoPai);
        }
    };

    int attAlt(Nodo<T>* node) const {
        if (node == nullptr) return -1;
        else return std::max(attAlt(node->filhoEsquerda), attAlt(node->filhoDireita)) + 1;
    };

    Nodo<T>* dirEsq(Nodo<T>* A, Nodo<T>* nodePai) {
        Nodo<T>* C = A->filhoDireita;
        Nodo<T>* D = C->filhoEsquerda;
        Nodo<T>* G = D->filhoDireita;

        Nodo<T>* B = A->filhoEsquerda;
        Nodo<T>* E = C->filhoDireita;
        Nodo<T>* F = D->filhoEsquerda;

        C->filhoEsquerda = G;
        D->filhoDireita = C;
        A->filhoDireita = D;

        A->filhoDireita = F;
        D->filhoEsquerda = A;

        if (A == this->nodoRaiz)
            this->nodoRaiz = D;
        else {
            A = D;
            nodePai->filhoDireita = D;
        }

        return D;
    };

    Nodo<T>* esqDir(Nodo<T>* A, Nodo<T>* nodePai) {
        Nodo<T>* B = A->filhoEsquerda;
        Nodo<T>* E = B->filhoDireita;
        Nodo<T>* F = E->filhoEsquerda;
        Nodo<T>* G = E->filhoDireita;

        B->filhoDireita = F;
        E->filhoEsquerda = B;
        A->filhoEsquerda = E;

        A->filhoEsquerda = G;
        E->filhoDireita = A;

        if (A == this->nodoRaiz)
            this->nodoRaiz = E;
        else {
            A = E;
            nodePai->filhoEsquerda = E;
        }

        return E;
    };

    Nodo<T>* simplesEsq(Nodo<T>* node, Nodo<T>* nodePai) {
        Nodo<T>* nodeAux = node->filhoDireita;
        Nodo<T>* nodeAuxDir = nodeAux->filhoDireita;

        node->filhoDireita = nodeAux->filhoEsquerda;
        nodeAux->filhoEsquerda = node;
        nodeAux->filhoDireita = nodeAuxDir;

        if (node == this->nodoRaiz)
            this->nodoRaiz = nodeAux;
        else {
            node = nodeAux;
            nodePai->filhoDireita = nodeAux;
        }

        node->altura = attAlt(node);
        nodeAux->filhoEsquerda->altura = attAlt(nodeAux->filhoEsquerda);

        return nodeAux;
    };

    Nodo<T>* simplesDir(Nodo<T>* node, Nodo<T>* nodePai) {
        Nodo<T>* nodeAux = node->filhoEsquerda;
        Nodo<T>* nodeAuxEsq = nodeAux->filhoEsquerda;

        node->filhoEsquerda = nodeAux->filhoDireita;
        nodeAux->filhoDireita = node;
        nodeAux->filhoEsquerda = nodeAuxEsq;

        if (node == this->nodoRaiz)
            this->nodoRaiz = nodeAux;
        else {
            node = nodeAux;
            nodePai->filhoEsquerda = nodeAux;
        }

        node->altura = attAlt(node);
        nodeAux->filhoDireita->altura = attAlt(nodeAux->filhoDireita);

        return nodeAux;
    };

    int fatorB(Nodo<T>* node) {
        int fatorB = 0;
        int hE = 0, hD = 0;
        hD = attAlt(node->filhoDireita);
        hE = attAlt(node->filhoEsquerda);

        fatorB = hE - hD;

        return fatorB;
    };

public:
    ~MinhaArvoreAVL() {};
    MinhaArvoreAVL() {};

    void inserir(T chave) {
        if (this->nodoRaiz == nullptr) {
            Nodo<T>* newNode = new Nodo<T>{ chave, 0 };
            this->nodoRaiz = newNode;
        }
        else {
            inserirAux(this->nodoRaiz, chave);
            this->nodoRaiz->altura = attAlt(this->nodoRaiz);
        }
    };

    void remover(T chave) {
        if (this->nodoRaiz->chave == chave && this->nodoRaiz->filhoDireita == nullptr && this->nodoRaiz->filhoEsquerda == nullptr) {
            delete this->nodoRaiz;
            this->nodoRaiz = nullptr;
            return;
        }
        if (!this->contem(chave)) return;
        Nodo<T>* nodePai = this->retornaPai(chave);
        removerAux(nodePai, chave);
        this->nodoRaiz->altura = attAlt(this->nodoRaiz);
        verificaRot(this->nodoRaiz, nullptr);
    };

    /*
    * Quando o fatorB for positivo, terá uma inclinação para a esquerda - rotação para direita
    * Negativo, para a direita - rotação para esquerda
    */

    Nodo<T>* verificaRot(Nodo<T>* node, Nodo<T>* nodePai) {
        if (node == nullptr)
            return nullptr;

        int fatorBalan = fatorB(node);

        // direita-esquerda
        if (fatorBalan < -1 && (fatorB(node->filhoDireita) > 0)) {
            return dirEsq(node, nodePai);
        }

        // esquerda-direita
        else if (fatorBalan > 1 && (fatorB(node->filhoEsquerda) < 0)) {
            return esqDir(node, nodePai);
        }
        // simples esquerda
        else if ((fatorBalan < -1) && (fatorB(node->filhoDireita) <= 0)) {
            return simplesEsq(node, nodePai);
        }

        // simples direita
        else if ((fatorBalan > 1) && (fatorB(node->filhoEsquerda) >= 0)) {
            return simplesDir(node, nodePai);
        }
        return node;
    };
};

#endif
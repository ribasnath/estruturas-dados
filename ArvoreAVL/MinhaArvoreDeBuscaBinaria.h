#ifndef MINHAARVOREDEBUSCABINARIA_HPP
#define MINHAARVOREDEBUSCABINARIA_HPP

#include "ArvoreDeBuscaBinaria.h"
#include <cassert>
#include <utility>

/**
 * @brief Representa uma árvore binária de busca.
 *
 * @tparam T O tipo de dado guardado na árvore.
 *
 * chave = dado do nodo
 */
template<typename T>
class MinhaArvoreDeBuscaBinaria : public ArvoreDeBuscaBinaria<T>
{
protected:
    Nodo<T>* nodoRaiz = this->_raiz;

    Nodo<T>* buscarChave(Nodo<T>* node, T chave) const {
        if (node == nullptr) return nullptr;
        if (chave == node->chave) return node;

        if (chave > node->chave)
            buscarChave(node->filhoDireita, chave);
        else if (chave < node->chave)
            buscarChave(node->filhoEsquerda, chave);
    };

    Nodo<T>* inseNew(T chave) {
        Nodo<T>* newNode = new Nodo<T>();
        newNode->chave = chave;
        newNode->altura = recAlt(newNode);
        nodoRaiz->altura = recAlt(nodoRaiz);
        //newNode = verificaRot(newNode);

        return newNode;
    };

    void inserirAux(Nodo<T>* node, T chave) {
        if (node == nullptr) {
            inseNew(chave);
        }
        else if (chave > node->chave) {
            if (node->filhoDireita == nullptr)
                node->filhoDireita = inseNew(chave);
            else
                inserirAux(node->filhoDireita, chave);
        }
        else if (chave < node->chave) {
            if (node->filhoEsquerda == nullptr)
                node->filhoEsquerda = inseNew(chave);
            else
                inserirAux(node->filhoEsquerda, chave);
        }
    };

    int quantDir(Nodo<T>* node) const {
        Nodo<T>* nodeDir = node->filhoDireita;
        int quant = 0;

        while (nodeDir != nullptr)
        {
            quant++;

            if (nodeDir->filhoEsquerda != nullptr)
            {
                quant += quantEsq(nodeDir);
            }

            nodeDir = nodeDir->filhoDireita;
        }

        return quant;
    }

    int quantEsq(Nodo<T>* node) const {
        Nodo<T>* nodeEsq = node->filhoEsquerda;
        int quant = 0;

        while (nodeEsq != nullptr)
        {
            quant++;

            if (nodeEsq->filhoDireita != nullptr)
            {
                quant += quantDir(nodeEsq);
            }

            nodeEsq = nodeEsq->filhoEsquerda;
        }

        return quant;
    }

    int recAlt(Nodo<T>* node) const {
        if (node == nullptr)
            return -1;
        else {
            return std::max(recAlt(node->filhoEsquerda), recAlt(node->filhoDireita)) + 1;
        }
    };

    void removerAux(Nodo<T>* nodePai, T chave) const {
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

            Nodo<T>* paiDoDirMin = retornaPai(filhoDirMin->chave);

            nodeRem->chave = filhoDirMin->chave;
            filhoDirMin->chave = chave;

            removerAux(paiDoDirMin, filhoDirMin->chave);
        }
    };

    Nodo<T>* retornaPai(T chave) const {
        Nodo<T>* node = nodoRaiz;

        while (node != nullptr) {
            if (chave > node->chave) {
                if (node->filhoDireita != nullptr && chave == node->filhoDireita->chave) return node;
                node = node->filhoDireita;
            }
            else if (chave < node->chave) {
                if (node->filhoEsquerda != nullptr && chave == node->filhoEsquerda->chave) return node;
                node = node->filhoEsquerda;
            }
            else if (chave == node->chave) return node;
        }
        return nullptr;
    };

    void emAux(Nodo<T>* node, ListaEncadeadaAbstrata<T>* lista) const {
        if (node != nullptr)
        {
            emAux(node->filhoEsquerda, lista);
            lista->inserirNoFim(node->chave);
            emAux(node->filhoDireita, lista);
        }
        else return;
    }

    void preAux(Nodo<T>* node, ListaEncadeadaAbstrata<T>* lista) const {
        if (node != nullptr)
        {
            lista->inserirNoFim(node->chave);
            preAux(node->filhoEsquerda, lista);
            preAux(node->filhoDireita, lista);
        }
        else return;
    }

    void posAux(Nodo<T>* node, ListaEncadeadaAbstrata<T>* lista) const {
        if (node != nullptr)
        {
            posAux(node->filhoEsquerda, lista);
            posAux(node->filhoDireita, lista);
            lista->inserirNoFim(node->chave);
        }
        else return;
    }

    void destrutor(Nodo<T>* node) {
        if (node != nullptr) {
            destrutor(node->filhoEsquerda);
            destrutor(node->filhoDireita);
            delete node;
        }
    };

public:
    ~MinhaArvoreDeBuscaBinaria() {
        destrutor(nodoRaiz);
    }

    /**
     * @brief Verifica se a arvore esta vazia
     * @return Verdade se a arvore esta vazia.
     */
    bool vazia() const override
    {
        if (nodoRaiz == nullptr) return true;
        else return false;
    };

    /**
     * @brief Retornar quantidade de chaves na arvore
     * @return Numero natural que representa a quantidade de chaves na arvore
     */
    int quantidade() const override
    {
        //Nodo<T>* esq = esq->filhoEsquerda;
        //Nodo<T>* dir = dir->filhoDireita;
        /*
          return 1+max(getAltura(raiz->filhoEsq, raiz->filhoDir))
          return -1 se vazio
        */

        if (!vazia())
        {
            int quant = 0;
            
            quant = quantDir(nodoRaiz) + quantEsq(nodoRaiz) + 1;
            return quant;
        }
        else return 0;
    };

    /**
     * @brief Verifica se a arvore contem uma chave
     * @param chave chave a ser procurada na arvore
     * @return Verdade se a arvore contem a chave
     */
    bool contem(T chave) const override
    {
        //Nodo<T>* node = nodoRaiz;
        //bool dir = false, esq = false;

        if (!vazia())
        {
            if (buscarChave(nodoRaiz, chave) == nullptr) return false;
            else return true;
            //return contemAux(node, chave);
            //if (chave >= node->chave) dir = contemDir(node, chave);
            //else if (chave < node->chave) esq = contemEsq(node, chave);

            //if (dir == true || esq == true) return true;
        }
        return false;
    };

    /**
     * @brief Retorna a altura da (sub)arvore
     * @param chave chave que é raiz da (sub)arvore cuja altura queremos. Se chave é nula, retorna a altura da arvore.
     * @return Numero inteiro representando a altura da (subarvore). Se chave nao esta na arvore, retorna std::nullopt
     */
    std::optional<int> altura(T chave) const override
    {
        //usar recursividade
        // 1+max(filhoEs->raiz, filhoD->raiz)

        if (vazia()) return std::nullopt;
        if (!contem(chave)) return std::nullopt;

        Nodo<T>* node = nodoRaiz;

        node = buscarChave(node, chave);

        if (node == nullptr) return std::nullopt;

        return recAlt(node);
    };

    /**
     * @brief Insere uma chave na arvore
     * @param chave chave a ser inserida
     */
    void inserir(T chave) override
    {
        if (nodoRaiz == nullptr) {
            Nodo<T>* newNode = new Nodo<T>{ chave, 0 };
            nodoRaiz = newNode;
        }
        else {
            inserirAux(nodoRaiz, chave);
            nodoRaiz->altura = recAlt(nodoRaiz);
        }
    };

    /**
     * @brief Remove uma chave da arvore
     * @param chave chave a removida
     * @return Retorna a chave removida ou nullptr se a chave nao esta na arvore
     */
    void remover(T chave) override
    {
        if (nodoRaiz->chave == chave && nodoRaiz->filhoDireita == nullptr && nodoRaiz->filhoEsquerda == nullptr) {
            delete nodoRaiz;
            nodoRaiz = nullptr;
            return;
        }
        if (!contem(chave)) return;
        Nodo<T>* nodePai = retornaPai(chave);
        removerAux(nodePai, chave);
    };

    /**
     * @brief Busca a chave do filho a esquerda de uma (sub)arvore
     * @param chave chave da arvore que eh pai do filho a esquerda
     * @return Chave do filho a esquerda. Se chave nao esta na arvore, retorna std::nullopt
     */
    std::optional<T> filhoEsquerdaDe(T chave) const override
    {
        Nodo<T>* node = nodoRaiz;

        if (!contem(chave)) return std::nullopt;
        else {
            node = buscarChave(node, chave);
            if (node->filhoEsquerda != nullptr)
            {
                node = node->filhoEsquerda;
                return node->chave;
            }
            else return std::nullopt;
        }
    };

    /**
     * @brief Busca a chave do filho a direita de uma (sub)arvore
     * @param chave chave da arvore que eh pai do filho a direita
     * @return Chave do filho a direita. Se chave nao esta na arvore, retorna nullptr
     */
    std::optional<T> filhoDireitaDe(T chave) const override
    {
        Nodo<T>* node = nodoRaiz;

        if (!contem(chave)) return std::nullopt;
        else {
            node = buscarChave(node, chave);
            if (node->filhoDireita != nullptr)
            {
                node = node->filhoDireita;
                return node->chave;
            }
            else return std::nullopt;
        }
    };

    /**
     * @brief Lista chaves visitando a arvore em ordem
     * @return Lista encadeada contendo as chaves em ordem.
     */
    ListaEncadeadaAbstrata<T>* emOrdem() const override
    {
        ListaEncadeadaAbstrata<T>* lista = new MinhaListaEncadeada<T>();
        Nodo<T>* node = nodoRaiz;

        if (nodoRaiz != nullptr) emAux(node, lista);

        return lista;
    };

    /**
     * @brief Lista chaves visitando a arvore em pre-ordem
     * @return Lista encadeada contendo as chaves em pre-ordem.
     */
    ListaEncadeadaAbstrata<T>* preOrdem() const override
    {
        ListaEncadeadaAbstrata<T>* lista = new MinhaListaEncadeada<T>();
        Nodo<T>* node = nodoRaiz;

        if (nodoRaiz != nullptr) preAux(node, lista);

        return lista;
    };

    /**
     * @brief Lista chaves visitando a arvore em pos-ordem
     * @return Lista encadeada contendo as chaves em pos ordem.
     */
    ListaEncadeadaAbstrata<T>* posOrdem() const override
    {
        ListaEncadeadaAbstrata<T>* lista = new MinhaListaEncadeada<T>();
        Nodo<T>* node = nodoRaiz;

        if (nodoRaiz != nullptr) posAux(node, lista);

        return lista;
    };
};

#endif
#ifndef DEC0006_MINHA_LISTA_ENCADEADA_H
#define DEC0006_MINHA_LISTA_ENCADEADA_H

#include <cstddef>
// std::size_t

#include "Elemento.h"
// Elemento
#include "ListaEncadeadaAbstrata.h"
#include "excecoes.h"
// ExcecaoDadoInexistente
// ExcecaoListaEncadeadaVazia
// ExcecaoNaoImplementado
// ExcecaoPosicaoInvalida

/**
 * @brief Uma lista encadeada de dados.
 * 
 * @tparam T O tipo dos dados armazenados na lista.
 */
template<typename T>
class MinhaListaEncadeada: public ListaEncadeadaAbstrata<T>
{
    //Implemente aqui as funcÃµes marcadas com virtual na ListaEncadeadaAbstrata
    //Lembre-se de implementar o construtor e destrutor da classe

public:

    MinhaListaEncadeada(){
        this->_tamanho = 0;
        this->_primeiro = nullptr;
    };
    ~MinhaListaEncadeada(){
        Elemento<T>* elemento = this->_primeiro;

        size_t i=0;

        while(i < this->_tamanho)
        {
            i++;
            Elemento<T>* aux = elemento;
            elemento = elemento->proximo;
            delete aux;
        }
    };

    std::size_t tamanho() const override
    {
        return this->_tamanho;
    };

    bool vazia() const override
    {
        if(tamanho() == 0 || this->_primeiro == nullptr)
            return true;
        return false;
    };

    std::size_t posicao(T dado) const override
    {
        Elemento<T>* elemento = this->_primeiro;

        if(vazia() == true)
        {
            throw ExcecaoListaEncadeadaVazia{};
        }

        size_t pos = 0;

        while(elemento != nullptr)
        {
            if(dado == elemento->dado)
                return pos;
            else
            {
                pos++;
            }
            elemento = elemento->proximo;
        }

        throw ExcecaoDadoInexistente{};
    };

    bool contem(T dado) const override
    {
        if(vazia())
            return false;

        Elemento<T>* elemento = this->_primeiro;

        while(elemento != nullptr)
        {
            if(elemento->dado == dado)
                return true;
            elemento = elemento->proximo;
        }

        return false;
    };

    void inserirNoInicio(T dado) override
    {
        Elemento<T>* novoElemento = new Elemento<T>(dado, this->_primeiro);
        this->_primeiro = novoElemento;

        this->_tamanho++;
    };

    void inserir(std::size_t posicao, T dado) override
    {
        if(posicao == 0)
            return inserirNoInicio(dado);

        if(posicao > tamanho() || posicao < 0 || vazia())
            throw ExcecaoPosicaoInvalida{};
        
        if(posicao == (tamanho()-1))
            return inserirNoFim(dado);

        Elemento<T>* aux = this->_primeiro;
        Elemento<T>* aux2 = aux;

        for(size_t i=0; i<posicao; i++)
        {
            aux2 = aux;
            aux = aux->proximo;
        }

        Elemento<T>* novoElemento = new Elemento<T>(dado, aux);
        aux2->proximo = novoElemento;

        this->_tamanho++;
    };

    void inserirNoFim(T dado) override
    {
        if(vazia() == true)
        {
            inserirNoInicio(dado);
            return;
        }

        Elemento<T>* aux = this->_primeiro;

        while(aux->proximo != nullptr)
        {
            aux = aux->proximo;
        }

        Elemento<T>* novoElemento = new Elemento<T>(dado, nullptr);
        aux->proximo = novoElemento;

        this->_tamanho++;
    };

    T removerDoInicio() override
    {
        if(vazia())
            throw ExcecaoListaEncadeadaVazia{};

        Elemento<T>* elemento = this->_primeiro;
        Elemento<T>* aux = elemento->proximo;
        T audado = elemento->dado;

        delete elemento;

        this->_primeiro = aux;
        this->_tamanho--;

        return audado;
    };

    T removerDe(std::size_t posicao) override
    {
        if(this->_primeiro != nullptr && posicao == 0)
            return removerDoInicio();

        if(posicao >= tamanho() || posicao < 0 || vazia())
            throw ExcecaoPosicaoInvalida{};
        
        if(posicao == (tamanho()-1))
            return removerDoFim();

        Elemento<T>* elemento = this->_primeiro;
        Elemento<T>* aux = elemento;
        T audado = elemento->dado;
        
        for(size_t i=0; i<posicao; i++)
        {
            aux = elemento;
            elemento = elemento->proximo;
        }

        aux->proximo = elemento->proximo;
        audado = elemento->dado;
        delete elemento;
        this->_tamanho--;

        return audado;
    };

    T removerDoFim() override
    {
        if(vazia() == true)
            throw ExcecaoListaEncadeadaVazia{};
        if(tamanho() == 1)
            return removerDoInicio();

        Elemento<T>* elemento = this->_primeiro;
        Elemento<T>* aux;
        T audado;

        while(elemento->proximo != nullptr)
        {
            aux = elemento;
            elemento = elemento->proximo;
        }

        audado = elemento->dado;
        
        aux->proximo = nullptr;
        delete elemento;
        this->_tamanho--;
        return audado;
    };

    void remover(T dado) override
    {
        if(vazia() == true)
            throw ExcecaoListaEncadeadaVazia{};

        size_t pos = posicao(dado);

        removerDe(pos);
    };
};

#endif
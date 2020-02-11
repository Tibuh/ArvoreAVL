#include <stdio.h>
#include <stdlib.h>
#define true 1
#define false 0

typedef int bool;
typedef int TIPOCHAVE;
//comentar o codigo

//Um noh de uma arvore AVL
typedef struct aux{
    TIPOCHAVE chave;
    struct aux *esq;
    struct aux *dir;
    //altura
    int h;
}noh, *pont;

int maiorNum(int a, int b); 

pont incializArvore(){
    return NULL;
}

//Funcao para descobrir altura 
int altura(pont raiz){
    if(raiz == NULL) return 0;
    return raiz->h;
}

//Funcao para descobrir o maior valor entre dois numeros
int maiorNum(int a, int b) {
    return (a > b)? a : b;
}

pont criarNoh(TIPOCHAVE ch){
    pont novoNoh = (pont)malloc(sizeof(noh));
    novoNoh->esq = NULL;
    novoNoh->dir = NULL;
    novoNoh->chave = ch;
    novoNoh->h = 0;
    return novoNoh;
}

pont direita(pont r){
    pont aux = r->esq;
    pont aux2 = aux ->dir;

    //Executando a rotacao
    aux->dir = r;
    r->esq = aux2;    

    //Atutalizando alturas
    r->h = maiorNum(altura(r->dir), altura(r->esq)) + 1;
    aux->h = maiorNum(altura(aux->esq), altura(aux->dir)) + 1;

    //retorna a nova rotacao
    return aux;
}

pont esquerda(pont r){
    pont aux = r->dir;
    pont aux2 = aux->esq;

    //Executando a rotacao
    aux->esq = r;
    r->dir = aux2;
    
    //Atutalizando alturas
    r->h = maiorNum(altura(r->esq), altura(r->dir)) + 1;
    aux->h = maiorNum(altura(aux->esq), altura(aux->dir)) + 1;

    //retorna a nova rotacao
    return aux;
}

pont esquerdaDireita(pont r){
    r->esq = esquerda(r->esq);
    return direita(r);
}

pont direitaEsquerda(pont r){
    r->dir = direita(r->dir);
    return esquerda(r);
}

int balanco(pont r){
    if(r == NULL) return 0;
    return (altura(r->esq) - altura(r->dir));
}

pont insere(pont raiz, pont p){ 

    if(raiz == NULL) return(p);
    
    if(p->chave < raiz->chave) raiz->esq = insere(raiz->esq, p);
    else if(p->chave > raiz->chave) raiz->dir = insere(raiz->dir, p);
    else return raiz;    

    //atualizando a altura do noh ancestral    
    raiz->h = maiorNum(altura(raiz->esq), altura(raiz->dir)) + 1;

    //analizando o balanco da arvore
    int balancoArvore = balanco(raiz);

    //agora testando nos 4 casos existentes;
    //Realizando a rotacao a direita
    if(balancoArvore > 1 && p->chave < raiz->esq->chave) return direita(raiz);
    
    //realizando rotacao a esquerda
    if(balancoArvore < -1 && p->chave > raiz->dir->chave) return  esquerda(raiz);

    //realizando rotacao esquerda direita
    if(balancoArvore > 1 && p->chave > raiz->esq->chave) return esquerdaDireita(raiz);

    //realizando rotacao direita esquerda
    if(balancoArvore < -1 && p->chave < raiz->dir->chave) return direitaEsquerda(raiz);
    
    return raiz; 

}


pont buscaNoh(pont raiz, TIPOCHAVE ch, pont *pai){
    pont atual = raiz;

    *pai = NULL;

    while (atual){

        if(atual->chave == ch) return atual;

        *pai = atual;

        if(ch < atual->chave) atual = atual->esq;
        else atual = atual->dir;

    }

    return NULL;

}

pont removeNoh(pont raiz, TIPOCHAVE ch) {
    pont pai, no, paiDaDireita, maisADireita;
    //busca o noh a ser removido e marca o pai dele
    no = buscaNoh(raiz, ch, &pai);
    //verifica se o no e null
    if(no == NULL) return raiz;

    //verifica se ele possui apenas um descendente
    if(!no->esq || !no->dir) {
        if(!no->esq) maisADireita = no->dir;
        else maisADireita = no->esq;
    //caso tenha dois descendentes
    }else {
        //marca pai da direita no noh
        paiDaDireita = no;
        //marca o mais a direita do esquerdo 
        maisADireita = no->esq;

        //repeticao para marcar o pai do mais a direita do elemento mais a direita da esquerda do no a ser removido
        while(maisADireita->dir){
            paiDaDireita = maisADireita;
            maisADireita = maisADireita->dir;
        }

        //verifica se o pai mais a direita e diferente do no a ser removido
        if(paiDaDireita != no){
            //faz o pai dda direita apontas para o filho mais a direita
            paiDaDireita->dir = maisADireita->esq;
            //aponta para o filho esquerdo do no a ser removido
            maisADireita->esq = no->esq;
        }

        //aponta para o noh a direita do que sera removido
        maisADireita->dir = no->dir;
    }

    //verifica se o que sera removido e a raiz
    if(!pai){
        //remove o noh desejado
        free(no);
        //retorna o mais a direita
        return maisADireita;
    }

    if(ch < pai->chave) pai->esq = maisADireita;
    else pai->dir = maisADireita;
    free(no);

    //pega o balanco
    int balancoArvore = balanco(raiz);

    //rotacao Direita
    if(balancoArvore > 1 && balanco(raiz->esq) >= 0) return direita(raiz);
    //rotacao esquerda direita
    if(balancoArvore > 1 && balanco(raiz->esq) < 0) return esquerdaDireita(raiz);
    //rotacao Esquerda
    if(balancoArvore < -1 && balanco(raiz->dir) <= 0) return esquerda(raiz);
    //rotacao DireitaEsquerda
    if(balancoArvore < -1 && balanco(raiz->dir) > 0) return direitaEsquerda(raiz);

    return raiz;
}

//imprime raiz, esquesda, direita
void imprimePreOrdem(pont raiz){ 
    printf("%d ",raiz->chave);
    if(raiz->esq != NULL) imprimePreOrdem(raiz->esq);
    if(raiz->dir != NULL) imprimePreOrdem(raiz->dir);
}

//imprime esquerda, raiz, direita
void imprimeOrdem(pont raiz){ 
    if(raiz->esq != NULL) imprimeOrdem(raiz->esq);
    printf("%d ",raiz->chave);
    if(raiz->dir != NULL) imprimeOrdem(raiz->dir);
}

//imprime esquerda, direita, raiz
void imprimePosOrdem(pont raiz){ 
    if(raiz->esq != NULL) imprimePosOrdem(raiz->esq);
    if(raiz->dir != NULL) imprimePosOrdem(raiz->dir);
    printf("%d ",raiz->chave);
}



void main(){
    pont r = incializArvore();
    pont noh = criarNoh(1);
    r = insere(r, noh);

    noh = criarNoh(2);
    r = insere(r, noh);
    noh = criarNoh(3);
    r = insere(r, noh);
    noh = criarNoh(4);
    r = insere(r, noh);
    noh = criarNoh(5);
    r = insere(r, noh);
    noh = criarNoh(6);
    r = insere(r, noh);
    noh = criarNoh(7);
    r = insere(r, noh);
    noh = criarNoh(8);
    r = insere(r, noh);
    noh = criarNoh(9);
    r = insere(r, noh);


    imprimePreOrdem(r);

    r = removeNoh(r, 6);
    printf("\n");
    imprimePreOrdem(r);
}

/* 
int Balanceamento(pont *ppRaiz) {

    int fb = FB(*ppRaiz);

    if ( fb > 1) return BalancaEsquerda(ppRaiz);

    else if (fb < -1 ) return BalancaDireita(ppRaiz);

    else return 0;

} */


/* int EhArvoreArvl(pont pRaiz) {
    int fb;

    if (pRaiz == NULL) return 1;

    if (!EhArvoreArvl(pRaiz->esq)) return 0;

    if (!EhArvoreArvl(pRaiz->dir)) return 0;

    fb = FB (pRaiz);

    if ( ( fb > 1 ) || ( fb < -1) ) return 0;

    else return 1;
} */

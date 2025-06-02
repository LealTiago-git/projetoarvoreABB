#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_NOME 100
#define MAX_LINHA 150

typedef struct Aluno
{
    int rgm;
    char nome[MAX_NOME];
} Aluno;

typedef struct NoArvore
{
    Aluno dados;
    struct NoArvore *esquerda;
    struct NoArvore *direita;
} NoArvore;

typedef NoArvore *Arvore;

NoArvore *criar_no_arvore(int rgm, const char *nome)
{
    NoArvore *novo_no = (NoArvore *)malloc(sizeof(NoArvore));
    if (novo_no != NULL)
    {
        novo_no->dados.rgm = rgm;
        strcpy(novo_no->dados.nome, nome);
        novo_no->esquerda = NULL;
        novo_no->direita = NULL;
    }
    return novo_no;
}

NoArvore *inserir_aluno(Arvore raiz, int rgm, const char *nome)
{
    if (raiz == NULL)
    {
        return criar_no_arvore(rgm, nome);
    }
    if (strcmp(nome, raiz->dados.nome) < 0)
    {
        raiz->esquerda = inserir_aluno(raiz->esquerda, rgm, nome);
    }
    else if (strcmp(nome, raiz->dados.nome) > 0)
    {
        raiz->direita = inserir_aluno(raiz->direita, rgm, nome);
    }
    return raiz;
}

void exibir_alunos_em_ordem(const Arvore raiz)
{
    if (raiz != NULL)
    {
        exibir_alunos_em_ordem(raiz->esquerda);
        printf("RGM: %d\tNome: %s\n", raiz->dados.rgm, raiz->dados.nome);
        exibir_alunos_em_ordem(raiz->direita);
    }
}

void liberar_arvore(Arvore raiz)
{
    if (raiz != NULL)
    {
        liberar_arvore(raiz->esquerda);
        liberar_arvore(raiz->direita);
        free(raiz);
    }
}

Arvore ler_arquivo_alunos(const char *nome_arquivo)
{
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo '%s'.\n", nome_arquivo);
        return NULL;
    }

    char linha[MAX_LINHA];
    Arvore raiz = NULL;

    while (fgets(linha, sizeof(linha), arquivo) != NULL)
    {
        int rgm;
        char nome[MAX_NOME];
        if (sscanf(linha, "%d %[^\n]", &rgm, nome) == 2 && strlen(nome) > 0)
        {
            raiz = inserir_aluno(raiz, rgm, nome);
        }
    }

    fclose(arquivo);
    return raiz;
}

void inserir_na_arvore(NoArvore **raiz, int rgm, char nome[]) {
    if (*raiz == NULL) {
        *raiz = (NoArvore *)malloc(sizeof(NoArvore));
        (*raiz)->dados.rgm = rgm;
        strcpy((*raiz)->dados.nome, nome);
        (*raiz)->esquerda = NULL;
        (*raiz)->direita = NULL;
    } else if (rgm < (*raiz)->dados.rgm) {
        inserir_na_arvore(&(*raiz)->esquerda, rgm, nome);
    } else if (rgm > (*raiz)->dados.rgm) {
        inserir_na_arvore(&(*raiz)->direita, rgm, nome);
    } else {
        printf("RGM já cadastrado!\n");
    }
}


NoArvore* pesquisar_na_arvore(NoArvore *raiz, int rgm) {
    if (raiz == NULL) {
        return NULL;
    } else if (rgm == raiz->dados.rgm) {
        return raiz;
    } else if (rgm < raiz->dados.rgm) {
        return pesquisar_na_arvore(raiz->esquerda, rgm);
    } else {
        return pesquisar_na_arvore(raiz->direita, rgm);
    }
}

NoArvore* encontrar_min(NoArvore* no) {
    if (no == NULL) return NULL;
    while (no->esquerda != NULL) {
        no = no->esquerda;
    }
    return no;
}

NoArvore* remover_no(NoArvore* raiz, int rgm, int mostrar_conteudo) {
    if (raiz == NULL) return NULL;

    if (rgm < raiz->dados.rgm) {
        raiz->esquerda = remover_no(raiz->esquerda, rgm, mostrar_conteudo);
    } else if (rgm > raiz->dados.rgm) {
        raiz->direita = remover_no(raiz->direita, rgm, mostrar_conteudo);
    } else {
        if (mostrar_conteudo) {
            printf("Nó removido - RGM: %d, Nome: %s\n", raiz->dados.rgm, raiz->dados.nome);
        }

        NoArvore* temp;

        if (raiz->esquerda == NULL) {
            temp = raiz->direita;
            free(raiz);
            return temp;
        } else if (raiz->direita == NULL) {
            temp = raiz->esquerda;
            free(raiz);
            return temp;
        }

        temp = encontrar_min(raiz->direita);
        raiz->dados = temp->dados;
        raiz->direita = remover_no(raiz->direita, temp->dados.rgm, 0);
    }
    return raiz;
}

void remover_aluno(Arvore* raiz, int rgm) {
    if (*raiz == NULL) {
        printf("Árvore vazia!\n");
        return;
    }

    NoArvore* no = pesquisar_na_arvore(*raiz, rgm);
    if (no == NULL) {
        printf("Aluno com RGM %d não encontrado!\n", rgm);
        return;
    }

    *raiz = remover_no(*raiz, rgm, 1);
    printf("Aluno removido com sucesso!\n");
}
int main()
{
    setlocale(LC_ALL, "pt_BR.UTF-8");
    const char *nome_arquivo = "bancodedados.txt";

    Arvore arvore_alunos = ler_arquivo_alunos(nome_arquivo);

    if (arvore_alunos == NULL)
    {
        printf("Árvore está vazia. Nenhum aluno foi inserido.\n");
    }
    else
    {
        printf("Alunos em ordem alfabética:\n");
        exibir_alunos_em_ordem(arvore_alunos);

        remover_aluno(&arvore_alunos, 12345678);
    }

    liberar_arvore(arvore_alunos);
    return 0;
}

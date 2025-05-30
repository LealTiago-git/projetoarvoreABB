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
    }

    liberar_arvore(arvore_alunos);
    return 0;
}

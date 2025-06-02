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
    if (rgm < raiz->dados.rgm)
    {
        raiz->esquerda = inserir_aluno(raiz->esquerda, rgm, nome);
    }
    else if (rgm > raiz->dados.rgm)
    {
        raiz->direita = inserir_aluno(raiz->direita, rgm, nome);
    }
    return raiz;
}



//MODOS DE EXIBIÃ‡ÃƒO DA ÃRVORE
void exibir_alunos_in_ordem(const Arvore raiz)
{
    if (raiz != NULL)
    {
        exibir_alunos_in_ordem(raiz->esquerda);
        printf("RGM: %d\tNome: %s\n", raiz->dados.rgm, raiz->dados.nome);
        exibir_alunos_in_ordem(raiz->direita);
    }
}

void exibir_alunos_pre_ordem(const Arvore raiz){
    if(raiz != NULL){
        printf("RGM: %d\tNome: %s\n", raiz->dados.rgm, raiz->dados.nome);
        exibir_alunos_pre_ordem(raiz->esquerda);
        exibir_alunos_pre_ordem(raiz->direita);
    }
}

void exibir_alunos_pos_ordem(const Arvore raiz){
    if(raiz != NULL){
        exibir_alunos_pos_ordem(raiz->esquerda);
        exibir_alunos_pos_ordem(raiz->direita);
        printf("RGM: %d\tNome: %s\n", raiz->dados.rgm, raiz->dados.nome);
    }
}



Arvore ler_arquivo_alunos(const char *nome_arquivo)
{
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo '%s'.\nVerifique se o arquivo se encontra junto ao executavel do programa.", nome_arquivo);
        getchar();
        exit(1);
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


//OPERAÃ‡Ã•ES NA ÃRVORE
void liberar_arvore(Arvore raiz)
{
    if (raiz != NULL)
    {
        liberar_arvore(raiz->esquerda);
        liberar_arvore(raiz->direita);
        free(raiz);
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
            printf("NÃ³ removido - RGM: %d, Nome: %s\n", raiz->dados.rgm, raiz->dados.nome);
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
        printf("Ãrvore vazia!\n");
        return;
    }

    NoArvore* no = pesquisar_na_arvore(*raiz, rgm);
    if (no == NULL) {
        printf("Aluno com RGM %d nÃ£o encontrado!\n", rgm);
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

    int opcao;
    do {
        printf("\n======= MENU =======\n\n");
        printf("ALUNOS: Alik, Tiago, Jonatha, Lucas, Marcos e Fernando.\n");
        printf("DISCIPLINA: ESTRUTURA DE DADOS I\n");
        printf("PROFESSOR: WALACE BONFIM\n\n");
        printf("1 - INSERIR - fornecer RGM e NOME\n");
        printf("2 - EXIBIR ÁRVORE (In-Ordem)\n");
        printf("3 - EXIBIR ÁRVORE (Pré-Ordem)\n");
        printf("4 - EXIBIR ÁRVORE (Pós-Ordem)\n");
        printf("5 - PESQUISAR - fornecer o RGM a pesquisar\n");
        printf("6 - REMOVER UM NÓ - fornecer o RGM remover\n");
        printf("7 - ESVAZIAR ÁRVORE\n");
        printf("0 - SAIR\n");
        printf("\n====================\n\n");
        printf("DIGITE SUA OPÇÃO: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer de entrada

        switch (opcao) {
            case 1: {
                int rgm;
                char nome[MAX_NOME];

                printf("Digite o RGM do aluno: ");
                scanf("%d", &rgm);
                getchar(); // Limpa o buffer

                printf("Digite o nome do aluno: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = '\0'; // Remove o \n final

                arvore_alunos = inserir_aluno(arvore_alunos, rgm, nome);
                printf("Aluno inserido com sucesso!\n");
                break;
            }

            case 2:
                printf("\nAlunos (In-Ordem):\n");
                exibir_alunos_in_ordem(arvore_alunos);
                break;

            case 3:
                printf("\nAlunos (Pré-Ordem):\n");
                exibir_alunos_pre_ordem(arvore_alunos);
                break;

            case 4:
                printf("\nAlunos (Pós-Ordem):\n");
                exibir_alunos_pos_ordem(arvore_alunos);
                break;

            case 5: {
                int rgm;
                printf("Digite o RGM do aluno a ser buscado: ");
                scanf("%d", &rgm);

                NoArvore* resultado = pesquisar_na_arvore(arvore_alunos, rgm);
                if (resultado != NULL) {
                    printf("Aluno encontrado - RGM: %d, Nome: %s\n", resultado->dados.rgm, resultado->dados.nome);
                } else {
                    printf("Aluno com RGM %d não encontrado!\n", rgm);
                }
                break;
            }

            case 6: {
                int rgm;
                printf("Digite o RGM do aluno a ser removido: ");
                scanf("%d", &rgm);
                remover_aluno(&arvore_alunos, rgm);
                break;
            }

            case 7: {
                liberar_arvore(arvore_alunos);
                arvore_alunos = NULL;
                printf("Árvore esvaziada com sucesso!\n");
                break;
            }

            case 0:
                printf("Encerrando o programa...\n");
                liberar_arvore(arvore_alunos);
                exit(0);

            default:
                printf("Opção inválida. Tente novamente.\n");
        }

    } while (1); // Laço infinito, pois a opção 0 agora usa exit()

    return 0; // Nunca será alcançado, mas mantido por boas práticas
}


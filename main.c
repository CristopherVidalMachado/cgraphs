#include <stdio.h>
#include <stdlib.h>
#include "Grafo.c"

int main()
{
    char option;
    int num_vertices, grau_max, ponderado, origem, destino, digrafo;
    float peso;

    clearScreen();
    printf("Insira o numero de vertices:\n");
    scanf("%d", &num_vertices);
    printf("Insira o grau maximo do grafo:\n");
    scanf("%d", &grau_max);
    printf("Grafo ponderado? (0: nao | 1: sim)\n");
    scanf("%d", &ponderado);
    printf("Grafo digrafo? (0: nao | 1: sim)\n");
    scanf("%d", &digrafo);

    Grafo *gr = criaGrafo(num_vertices, grau_max, ponderado, digrafo);
    int vis[num_vertices], anterior[num_vertices];
    float distancia[num_vertices];

    do
    {
        clearScreen();
        printf("1: Inserir aresta.\n");
        printf("2: Remover aresta.\n");
        printf("3: Imprimir grafo.\n");
        printf("4: Busca em profundidade.\n");
        printf("5. Busca em largura.\n");
        printf("6. Dijkstra.\n");
        printf("7. Arvore expandida minima.\n");
        printf("8. Kruskal.\n");
        printf("9. Prim.\n");
        printf("0: Liberar grafo.\n");
        clean_stdin();
        option = getchar();
        clearScreen();

        if (option == '1')
        {
            printf("Qual o vertice de origem?\n");
            scanf("%d", &origem);
            printf("Qual o vertice de destino?\n");
            scanf("%d", &destino);
            if (ponderado == 1)
            {
                printf("Qual o peso da aresta?\n");
                scanf("%f", &peso);
            }

            insereAresta(gr, origem, destino, digrafo, peso);
        }
        else if (option == '2')
        {
            printf("Qual o vertice de origem?\n");
            scanf("%d", &origem);
            printf("Qual o vertice de destino?\n");
            scanf("%d", &destino);

            removeAresta(gr, origem, destino, digrafo);
        }
        else if (option == '3')
        {
            printf("Estrutura do grafo:\n\n");
            imprimeGrafo(gr);
            clean_stdin();
            getchar();
        }
        else if (option == '4')
        {
            printf("Busca em profundidade:\n\n");
            buscaProfundidade(gr, 0, vis);
            clean_stdin();
            getchar();
        }
        else if (option == '5')
        {
            printf("Busca em largura:\n\n");
            buscaLargura(gr, 0, vis);
            clean_stdin();
            getchar();
        }
        else if (option == '6')
        {
            printf("Busca pelo menor caminho (algoritmo de Dijkstra):\n\n");
            menorCaminho(gr, 0, anterior, distancia);
            clean_stdin();
            getchar();
        }
        else if (option == '0')
        {
            liberaGrafo(gr);
            printf("Grafo liberado");
            clean_stdin();
            getchar();
        }

    } while (option != '0');
    
    return 0;
}
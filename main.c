#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "Grafo.c"


int main()
{
    char option;
    int num_vertices, grau_max, ponderado, origem, destino,  digrafo;
    float peso;

    // clearScreen();
    initscr();
   
	
	cbreak();	/* Line buffering disabled. pass on everything */
    printw("Insira as informações do mapa para começar:\n");
    
    printw("Insira o numero de localidades: (Onde o tamanho  real será N-1) : ");
    refresh();
    scanw("%d", &num_vertices);
    printw("Insira o numero máximo de entradas e saidas das localidades: ");
    scanw("%d", &grau_max);
    printw("O mapa possui distancia entre os locais? (0: nao | 1: sim) (Qualquer valor diferente será considerado = 0 ): ");
    scanw("%d", &ponderado);
    if(ponderado != 1  && ponderado!= 0 ){
        ponderado = 0;
    }
    printw("Os caminhos são unidirecionais? (0: nao | 1: sim) (Qualquer valor diferente será considerado = 0): ");
    scanw("%d", &digrafo);
    if(digrafo != 1  && digrafo!= 0 ){
        digrafo = 0;
    }


    Grafo *gr = criaGrafo(num_vertices, grau_max, ponderado, digrafo);
    int vis[num_vertices], anterior[num_vertices];
    float distancia[num_vertices];
    clear();
    do
    {
        // clearScreen();
        printw("1: Inserir localidade.\n");
        printw("2: Remover localidade.\n");
        printw("3: Imprimir mapa.\n");
        printw("4: Busca em profundidade.\n");
        printw("5. Busca em largura.\n");
        printw("6. Dijkstra.\n");
        printw("8. Kruskal.\n");
        printw("9. Prim.\n");
        printw("0: Liberar mapa.\n");
        refresh();
        option = getch();
        // clearScreen();

        if (option == '1')
        {
            clear();
            printw("Qual a localidade de origem?\n");
            scanw("%d", &origem);
            printw("Qual o localidade de destino?\n");
            scanw("%d", &destino);
            if (ponderado == 1)
            {
                printw("Qual a distancia para o destino?\n");
                scanw("%f", &peso);
            }

            insereAresta(gr, origem, destino, digrafo, peso);
            clear();
        }
        else if (option == '2')
        {
            clear();
            printw("Qual a localidade de origem?\n");
            scanw("%d", &origem);
            printw("Qual o localidade de destino?\n");
            scanw("%d", &destino);

            removeAresta(gr, origem, destino, digrafo);
            clear();
        }
        else if (option == '3')
        {
            clear();
            printw("Estrutura do mapa:\n\n");
            imprimeGrafo(gr);
            refresh();
            getch();
            clear();

        }
        else if (option == '4')
        {
            clear();
            printw("Busca em profundidade:\n\n");
            buscaProfundidade(gr, 0, vis);
            refresh();
            getch();
            clear();

        }
        else if (option == '5')
        {
            clear();
            printw("Busca em largura:\n\n");
            buscaLargura(gr, 0, vis);
            refresh();
            getch();
            clear();

        }
        else if (option == '6')
        {
            clear();
            printw("Busca pelo menor caminho (algoritmo de Dijkstra):\n\n");
            int i,ant[5];
            float dist[5];
            

            menorCaminho_Grafo(gr, 0, ant, dist);
            for(i=0; i<num_vertices; i++)
                printw("%d: %d -> %.2f\n",i,ant[i],dist[i]);
            refresh();
            getch();
            clear();

        }
            else if (option == '8')
        {
            clear();
            printw("\n\nAlgoritmo de Kruskal:\n");
             int pai1[num_vertices], pai2[num_vertices];
             int i;
            algKruskal(gr, 0, pai2);
            for(i = 0; i < num_vertices; i++)
                printw("%d: %d\n", pai2[i], i);

             getch();
             clear();
        }
        else if (option == '9')
        {
            clear();
            int i;
   
            int pai1[num_vertices], pai2[num_vertices];
            printf("\n\nAlgoritmo de PRIM:\n");
            algPRIM(gr, 0, pai1);
            for(i = 0; i <num_vertices ; i++)
                printw("%d: %d\n", pai1[i], i);

             getch();
             clear();
        }
        else if (option == '0')
        {
            clear();
            liberaGrafo(gr);
            printw("Mapa liberado");
            refresh();
            getch();
            clear();

        }

    } while (option != '0');
    refresh();
	endwin();
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include "Grafo.h"

struct grafo
{
    int ponderado;
    int digrafo;
    int num_vertices;
    int grau_max;
    int **arestas;
    float **pesos;
    int *grau;
};

Grafo *criaGrafo(int num_vertices, int grau_max, int ponderado, int digrafo)
{
    Grafo *gr;
    gr = (Grafo *)malloc(sizeof(struct grafo));
    if (gr != NULL)
    {
        int i;
        gr->num_vertices = num_vertices;
        gr->grau_max = grau_max;
        gr->ponderado = ponderado;
        gr->digrafo = digrafo;
        gr->grau = (int *)calloc(num_vertices, sizeof(int));
        gr->arestas = (int **)malloc(num_vertices * sizeof(int *));

        for (i = 0; i < num_vertices; i++)
            gr->arestas[i] = (int *)malloc(grau_max * sizeof(int));

        if (gr->ponderado)
        {
            gr->pesos = (float **)malloc(num_vertices * sizeof(float *));

            for (i = 0; i < num_vertices; i++)
                gr->pesos[i] = (float *)malloc(grau_max * sizeof(float));
        }
    }

    return gr;
}

void liberaGrafo(Grafo *gr)
{
    if (gr != NULL)
    {
        int i;
        for (i = 0; i < gr->num_vertices; i++)
            free(gr->arestas[i]);
        free(gr->arestas);

        if (gr->ponderado)
        {
            for (i = 0; i < gr->num_vertices; i++)
                free(gr->pesos[i]);
            free(gr->pesos);
        }
        free(gr->grau);
        free(gr);
    }
}

int insereAresta(Grafo *gr, int origem, int destino, int digrafo, float peso)
{
    if (gr == NULL)
        return 0;
    if (origem < 0 || origem >= gr->num_vertices)
        return 0;
    if (destino < 0 || destino >= gr->num_vertices)
        return 0;

    gr->arestas[origem][gr->grau[origem]] = destino;
    if (gr->ponderado)
        gr->pesos[origem][gr->grau[origem]] = peso;
    gr->grau[origem]++;

    if (digrafo == 0)
        insereAresta(gr, destino, origem, 1, peso);
    return 1;
}

int removeAresta(Grafo *gr, int origem, int destino, int digrafo)
{
    if (gr == NULL)
        return 0;
    if (origem < 0 || origem >= gr->num_vertices)
        return 0;
    if (destino < 0 || destino >= gr->num_vertices)
        return 0;

    int i = 0;
    while (i < gr->grau[origem] && gr->arestas[origem][i] != destino)
        i++;
    if (i == gr->grau[origem]) //elemento nao encontrado
        return 0;
    gr->grau[origem]--;
    gr->arestas[origem][i] = gr->arestas[origem][gr->grau[origem]];
    if (gr->ponderado)
        gr->pesos[origem][i] = gr->pesos[origem][gr->grau[origem]];
    if (digrafo == 0)
        removeAresta(gr, destino, origem, 1);
    return 1;
}

void imprimeGrafo(Grafo *gr)
{
    if (gr == NULL)
        return;

    int i, j;
    for (i = 0; i < gr->num_vertices; i++)
    {
        printw("%d: ", i);
        for (j = 0; j < gr->grau[i]; j++)
        {
            if (gr->ponderado)
                printw("%d(%.2f), ", gr->arestas[i][j], gr->pesos[i][j]);
            else
                printw("%d, ", gr->arestas[i][j]);
        }
        printw("\n");
    }
}

void buscaProfundidadeAux(Grafo *gr, int ini, int *visitado, int cont)
{
    int i;
    visitado[ini] = cont;
    for (i = 0; i < gr->grau[ini]; i++)
    {
        if (!visitado[gr->arestas[ini][i]])
            buscaProfundidadeAux(gr, gr->arestas[ini][i], visitado, cont + 1);
    }
}

void buscaProfundidade(Grafo *gr, int ini, int *visitado)
{
    int i, cont = 1;
    for (i = 0; i < gr->num_vertices; i++)
        visitado[i] = 0;
    buscaProfundidadeAux(gr, ini, visitado, cont);

    for (i = 0; i < gr->num_vertices; i++)
        printw("Vertice [%d] -> %d nível\n", i, visitado[i]);
}

void buscaLargura(Grafo *gr, int ini, int *visitado)
{
    int i, vert, NV, cont = 1;
    int *fila, IF = 0, FF = 0;
    for (i = 0; i < gr->num_vertices; i++)
        visitado[i] = 0;

    NV = gr->num_vertices;
    fila = (int *)malloc(NV * sizeof(int));
    FF++;
    fila[FF] = ini;
    visitado[ini] = cont;
    while (IF != FF)
    {
        IF = (IF + 1) % NV;
        vert = fila[IF];
        cont++;
        for (i = 0; i < gr->grau[vert]; i++)
        {
            if (!visitado[gr->arestas[vert][i]])
            {
                FF = (FF + 1) % NV;
                fila[FF] = gr->arestas[vert][i];
                visitado[gr->arestas[vert][i]] = cont;
            }
        }
    }
    free(fila);
    for (i = 0; i < gr->num_vertices; i++)
        printw("%d -> %d\n", i, visitado[i]);
}

int menorCaminhoAux(float *distancia, int *visitado, int NV)
{
    int i, menor = -1, primeiro = 1;
    for (i = 0; i < NV; i++)
    {
        if (distancia[i] >= 0 && visitado[i] == 0)
        {
            if (primeiro)
            {
                menor = i;
                primeiro = 0;
            }
            else
            {
                if (distancia[menor] > distancia[i])
                    menor = i;
            }
        }
    }
    printw("menor: %d\n", menor);
    return menor;
}

// Retorna o vértice de menor distância e que não foi visitado ainda.
int procuraMenorDistancia(float *dist, int *visitado, int NV){
    int i, menor = -1, primeiro = 1;
    for(i=0; i < NV; i++){
        //Procura vértice com menor distância e que não tenha sido visitado.
		// Inicialmente todos os vértices não foram visitados e tem a distância -1;
		if(dist[i] >= 0 && visitado[i] == 0){
            if(primeiro){
                menor = i;
                primeiro = 0;
            }else{
                if(dist[menor] > dist[i])
                    menor = i;
            }
        }
    }
    return menor;
}

void menorCaminho_Grafo(Grafo *gr, int ini, int *ant, float *dist){
    int i, cont, NV, ind, *visitado, vert;
	
	//Esse contador é utilizado para controlar que visitou todos os nós.
    cont = NV = gr->num_vertices;
    
    refresh();
	// Cria vetor auxiliar. Inicializa distâncias e anteriores.
    visitado = (int*) malloc(NV * sizeof(int));
    for(i=0; i < NV; i++){
        ant[i] = -1;
        dist[i] = -1;
        visitado[i] = 0;
    }
	

        refresh();
	
    dist[ini] = 0;
    while(cont > 0){
		//Procura vértice com menor distância e marca como visitado.
        vert = procuraMenorDistancia(dist, visitado, NV);
        //printf("u = %d\n",u);
        if(vert == -1)
            break;

        visitado[vert] = 1;
        cont--;
		
		//Para cada vértice vizinho, atualize as distÂncias dos vizinhos.
        for(i=0; i<gr->grau[vert]; i++){
            ind = gr->arestas[vert][i];
            if(dist[ind] < 0){
			   //Ou peso da aresta
               dist[ind] = dist[vert] + gr->pesos[vert][i];
            //    dist[ind] = dist[vert] + 1;
               ant[ind] = vert;
            }else{
				//Se ele já tiver um valor de distÂncia válida...
                if(dist[ind] > dist[vert] + 1){
					//Significa que existe um caminho para vert com a distância menor
                    // dist[ind] = dist[vert] + 1;
					//ou peso da aresta
                    dist[ind] = dist[vert] + gr->pesos[vert][i];
					ant[ind] = vert;
                }
            }
        }
    }

    free(visitado);
}

void clearScreen()
{
    system("clear");
    //printw("\e[H\e[2J");
}

void clean_stdin(void)
{
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}
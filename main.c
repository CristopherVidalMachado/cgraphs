#include <stdio.h>
#include <stdlib.h>
#include <curses.h>

typedef struct grafo Grafo;

Grafo* cria_Grafo(int nro_vertices, int grau_max, int eh_ponderado);
void libera_Grafo(Grafo* gr);
int insereAresta(Grafo* gr, int orig, int dest, int eh_digrafo, float peso);
int removeAresta(Grafo* gr, int orig, int dest, int eh_digrafo);

//void buscaProfundidade(Grafo *gr, int ini, int *visitado, int cont);
void buscaProfundidade_Grafo(Grafo *gr, int ini, int *visitado);
void buscaLargura_Grafo(Grafo *gr, int ini, int *visitado);
void menorCaminho_Grafo(Grafo *gr, int ini, int *antecessor, float *distancia);

void imprime_Grafo(Grafo *gr);

//Definição do tipo Grafo
struct grafo{
    int eh_ponderado;
    int nro_vertices;
    int grau_max;
    int** arestas;
    float** pesos;
    int* grau;
};

Grafo* cria_Grafo(int nro_vertices, int grau_max, int eh_ponderado){
    Grafo *gr;
    gr = (Grafo*) malloc(sizeof(struct grafo));
    if(gr != NULL){
        int i;
        gr->nro_vertices = nro_vertices;
        gr->grau_max = grau_max;
        gr->eh_ponderado = (eh_ponderado != 0)?1:0;
        gr->grau = (int*) calloc(nro_vertices,sizeof(int));

        gr->arestas = (int**) malloc(nro_vertices * sizeof(int*));
        for(i=0; i<nro_vertices; i++)
            gr->arestas[i] = (int*) malloc(grau_max * sizeof(int));

        if(gr->eh_ponderado){
            gr->pesos = (float**) malloc(nro_vertices * sizeof(float*));
            for(i=0; i<nro_vertices; i++)
                gr->pesos[i] = (float*) malloc(grau_max * sizeof(float));
        }

    }
    return gr;
}

void libera_Grafo(Grafo* gr){
    if(gr != NULL){
        int i;
        for(i=0; i<gr->nro_vertices; i++)
            free(gr->arestas[i]);
        free(gr->arestas);

        if(gr->eh_ponderado){
            for(i=0; i<gr->nro_vertices; i++)
                free(gr->pesos[i]);
            free(gr->pesos);
        }
        free(gr->grau);
        free(gr);
    }
}

int insereAresta(Grafo* gr, int orig, int dest, int eh_digrafo, float peso){
    if(gr == NULL)
        return 0;

        //grafo origem e destino existem
    if(orig < 0 || orig >= gr->nro_vertices)
        return 0;
    if(dest < 0 || dest >= gr->nro_vertices)
        return 0;

    gr->arestas[orig][gr->grau[orig]] = dest;
    //aumenta um no grau
    if(gr->eh_ponderado)
        gr->pesos[orig][gr->grau[orig]] = peso;
    gr->grau[orig]++;

    if(eh_digrafo == 0)// chamada recursiva
        insereAresta(gr,dest,orig,1,peso);
    return 1;
}

int removeAresta(Grafo* gr, int orig, int dest, int eh_digrafo){
    if(gr == NULL)
        return 0;
    if(orig < 0 || orig >= gr->nro_vertices)
        return 0;
    if(dest < 0 || dest >= gr->nro_vertices)
        return 0;

    int i = 0;
    while(i<gr->grau[orig] && gr->arestas[orig][i] != dest)
        i++;
    if(i == gr->grau[orig])//elemento nao encontrado
        return 0;
    gr->grau[orig]--;
    gr->arestas[orig][i] = gr->arestas[orig][gr->grau[orig]];
    if(gr->eh_ponderado)
        gr->pesos[orig][i] = gr->pesos[orig][gr->grau[orig]];
    if(eh_digrafo == 0)
        removeAresta(gr,dest,orig,1);
    return 1;
}

void imprime_Grafo(Grafo *gr){
    if(gr == NULL)
        return;

    int i, j;
    for(i=0; i < gr->nro_vertices; i++){
        printf("%d: ", i);
        for(j=0; j < gr->grau[i]; j++){
            if(gr->eh_ponderado)
                printf("%d(%.2f), ", gr->arestas[i][j], gr->pesos[i][j]);
            else
                printf("%d, ", gr->arestas[i][j]);
        }
        printf("\n");
    }
}

/*
Algoritmos para Grafos em C
 via Sedgewick
http://www.ime.usp.br/~pf/algoritmos_para_grafos/
*/
// https://www.cs.auckland.ac.nz/software/AlgAnim/dijkstra.html
// http://www.cprogramming.com/tutorial/computersciencetheory/dijkstra.html
int procuraMenorDistancia(float *dist, int *visitado, int NV){
    int i, menor = -1, primeiro = 1;
    for(i=0; i < NV; i++){
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
    cont = NV = gr->nro_vertices;
    visitado = (int*) malloc(NV * sizeof(int));
    for(i=0; i < NV; i++){
        ant[i] = -1;
        dist[i] = -1;
        visitado[i] = 0;
    }
    dist[ini] = 0;
    while(cont > 0){
        vert = procuraMenorDistancia(dist, visitado, NV);
        //printf("u = %d\n",u);
        if(vert == -1)
            break;

        visitado[vert] = 1;
        cont--;
        for(i=0; i<gr->grau[vert]; i++){
            ind = gr->arestas[vert][i];
            if(dist[ind] < 0){
               dist[ind] = dist[vert] + 1;//ou peso da aresta
               ant[ind] = vert;
            }else{
                if(dist[ind] > dist[vert] + 1){
                    dist[ind] = dist[vert] + 1;//ou peso da aresta
                    ant[ind] = vert;
                }
            }
        }
    }

    free(visitado);
}


// http://www.ime.usp.br/~pf/algoritmos_para_grafos/aulas/dfs1.html
void buscaProfundidade(Grafo *gr, int ini, int *visitado, int cont){
    int i;
    visitado[ini] = cont;
    for(i=0; i<gr->grau[ini]; i++){
        if(!visitado[gr->arestas[ini][i]])
            buscaProfundidade(gr,gr->arestas[ini][i],visitado,cont+1);
    }
}
void buscaProfundidade_Grafo(Grafo *gr, int ini, int *visitado){
    int i, cont = 1;
    for(i=0; i<gr->nro_vertices; i++)
        visitado[i] = 0;
    buscaProfundidade(gr,ini,visitado,cont);

    for(i=0; i < gr->nro_vertices; i++)
        printf("%d -> %d\n",i,visitado[i]);
}
// http://en.wikipedia.org/wiki/Breadth-first_search
// http://www.princeton.edu/~achaney/tmve/wiki100k/docs/Breadth-first_search.html

void buscaLargura_Grafo(Grafo *gr, int ini, int *visitado){
    int i, vert, NV, cont = 1;
    int *fila, IF = 0, FF = 0;
    for(i=0; i<gr->nro_vertices; i++)
        visitado[i] = 0;

    NV = gr->nro_vertices;
    fila = (int*) malloc(NV * sizeof(int));
    FF++;
    fila[FF] = ini;
    visitado[ini] = cont;
    while(IF != FF){
        IF = (IF + 1) % NV;
        vert = fila[IF];
        cont++;
        for(i=0; i<gr->grau[vert]; i++){
            if(!visitado[gr->arestas[vert][i]]){
                FF = (FF + 1) % NV;
                fila[FF] = gr->arestas[vert][i];
                visitado[gr->arestas[vert][i]] = cont;
            }
        }
    }
    free(fila);
    for(i=0; i < gr->nro_vertices; i++)
        printf("%d -> %d\n",i,visitado[i]);
}

/* OUTRA VERSÃO
void buscaLargura_Grafo(Grafo *gr, int ini, int *visitado){
    int i, vert, NV, cont = 1;
    int *pilha, IP = 0, FP = 0;
    for(i=0; i<gr->nro_vertices; i++)
        visitado[i] = 0;
    NV = gr->nro_vertices;
    pilha = (int*) malloc(NV * sizeof(int));
    FP++;
    pilha[FP] = ini;
    while(IP != FP){
        IP = (IP + 1) % NV;
        vert = pilha[IP];
        if(!visitado[vert]){
            visitado[vert] = cont;
            cont++;
            for(i=0; i<gr->grau[vert]; i++){
                if(!visitado[gr->arestas[vert][i]]){
                    FP = (FP + 1) % NV;
                    pilha[FP] = gr->arestas[vert][i];
                }
            }
        }
    }
    free(pilha);
    for(i=0; i < gr->nro_vertices; i++)
        printf("%d -> %d\n",i,visitado[i]);
}
*/
// int main(){
    

//     // int eh_digrafo = 1;
//     // Grafo* gr = cria_Grafo(5, 5, 0);

//     // insereAresta(gr, 0, 1, eh_digrafo, 0);
//     // insereAresta(gr, 1, 3, eh_digrafo, 0);
//     // insereAresta(gr, 1, 2, eh_digrafo, 0);
//     // insereAresta(gr, 2, 4, eh_digrafo, 0);
//     // insereAresta(gr, 3, 0, eh_digrafo, 0);
//     // insereAresta(gr, 3, 4, eh_digrafo, 0);
//     // insereAresta(gr, 4, 1, eh_digrafo, 0);

//     // imprime_Grafo(gr);
//     // printf("\nBusca \n");
//     // int vis[5];
//     // // http://www.thelearningpoint.net/computer-science/algorithms-graph-traversal---breadth-first-search-with-c-program-source-code
//     // //buscaProfundidade_Grafo(gr, 0, vis);
//     // //buscaLargura_Grafo(gr, 0, vis);

//     // int i,ant[5];
//     // float dist[5];
//     // menorCaminho_Grafo(gr, 0, ant, dist);
//     // for(i=0; i<5; i++)
//     //     printf("%d: %d -> %f\n",i,ant[i],dist[i]);

//     // libera_Grafo(gr);



//     return 0;
// }
    

#include <menu.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 	4

char *choices[] = {
    "Criar Mapa",
    "Inserir um destino",
    "Inserir uma origem",
    "Apagar mapa",
    "Achar melhor caminho",
    "Achar melhor para visitar todos",
    "Mostrar mapa inteiro",
    "Sair"
};

char *choicesData[] = {
    "Crie um mapa interativo",
    "Para onde voce quer ir",
    "Insira os pontos que vc vai pasar ",
    "Apagar todos os dados",
    "Melhor caminho dos pontos que vc inseriu",
    "PRIM",
    "Mostrar mapa inteiro",
    "Sair"
};

void drawScreenDividers(){
        for (size_t i = 0; i <60 ; i++)
    {
           mvprintw(i, 80, "|");    
    }
    
    for (size_t i = 0; i <80 ; i++)
    {
           mvprintw(10, i, "-");    
    }

}
int main()
{	ITEM **my_items;
	int c;				
	MENU *my_menu;
        int n_choices, i;
	ITEM *cur_item;
	
	/* Initialize curses */	
	initscr();
    drawScreenDividers();
	start_color();
        cbreak();
        noecho();
	keypad(stdscr, TRUE);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_MAGENTA, COLOR_BLACK);

	/* Initialize items */
        n_choices = ARRAY_SIZE(choices);
        my_items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));
        for(i = 0; i < n_choices; ++i)
                my_items[i] = new_item(choices[i], choicesData[i]);
	my_items[n_choices] = (ITEM *)NULL;
	item_opts_off(my_items[3], O_SELECTABLE);



	/* Create menu */
	my_menu = new_menu((ITEM **)my_items);

	/* Set fore ground and back ground of the menu */
	set_menu_fore(my_menu, COLOR_PAIR(1) | A_REVERSE);
	set_menu_back(my_menu, COLOR_PAIR(2));
	set_menu_grey(my_menu, COLOR_PAIR(3));

	/* Post the menu */
	mvprintw(LINES - 3, 0, "Aperte enter para selecionar uma opcao");
	mvprintw(LINES - 2, 0, "Use as setas para navegar, FN+F1 para sair");
	post_menu(my_menu);
	refresh();

	while((c = getch()) != KEY_F(1))
	{       switch(c)
	        {	case KEY_DOWN:
				menu_driver(my_menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(my_menu, REQ_UP_ITEM);
				break;
			case 10: /* Enter */
				move(20, 0);
				clrtoeol();
				mvprintw(20, 0, "Selecionado: %s", 
						item_name(current_item(my_menu)));
         
				pos_menu_cursor(my_menu);
				if(item_index(current_item(my_menu)) == 0 ){
                    int eh_digrafo = 1;
                    Grafo* gr = cria_Grafo(5, 5, 0);
                    mvprintw(26, 0, "Mapa criado com sucesso");
                }
                
                break;
		}
	}	
	unpost_menu(my_menu);
	for(i = 0; i < n_choices; ++i)
		free_item(my_items[i]);
	free_menu(my_menu);
	endwin();
}
	
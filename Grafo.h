typedef struct grafo Grafo;

Grafo *criaGrafo(int num_vertices, int grau_max, int ponderado, int digrafo);
void liberaGrafo(Grafo *gr);
int insereAresta(Grafo *gr, int origem, int destino, int digrafo, float peso);
int removeAresta(Grafo *gr, int origem, int destino, int digrafo);

void buscaProfundidadeAux(Grafo *gr, int ini, int *visitado, int cont);
void buscaProfundidade(Grafo *gr, int ini, int *visitado);
void buscaLargura(Grafo *gr, int ini, int *visitado);
int menorCaminhoAux(float *distancia, int *visitado, int NV);
void menorCaminho(Grafo *gr, int ini, int *anterior, float *distancia);

void imprimeGrafo(Grafo *gr);

void clearScreen();

void clean_stdin(void);
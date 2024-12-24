#include <stdio.h>
#include <stdlib.h>

#define MP_BLOCKS 64
#define BLOCK_SIZE 8
#define CACHE_LINES 8

//Variáveis globais. Prática não recomendada, porém solução mais simples que encontrei
int cacheHit  = 0;
int cacheMiss = 0;

//Estrutura que representa uma linha da cache
typedef struct {
	int bloco[BLOCK_SIZE]; //Armazena um bloco da MP
	int tag;               //Armazena o número da tag
	int valido;            //Indica se a linha está presente no bloco
} CacheLine;

//Função para verificar o índice da cache e verificar se o bloco está presente
int buscarNaCache(CacheLine cache[], int memoriaPrincipal[MP_BLOCKS][BLOCK_SIZE], int endereco) {

        int bloco        = endereco / BLOCK_SIZE;        //Calcula o número do bloco na MP        
	int deslocamento = endereco % BLOCK_SIZE;        //Deslocamento dentro do bloco
        int indiceCache  = bloco    % CACHE_LINES;       //Índice da linha cache

        printf("\nProcurando endereço %d...\n", endereco);

        if (cache[indiceCache].valido && cache[indiceCache].tag == bloco) {

                printf("\nCache HIT na linha %d!\n", indiceCache); //Cache HIT
                cacheHit++;
		
		return cache[indiceCache].bloco[deslocamento];

        } else {
                
		printf("\nCache MISS! Carregando bloco %d na linha %d.\n", bloco, indiceCache); //Cache MISS
		cacheMiss++;
                
		for (int i=0; i<BLOCK_SIZE; i++) { //Carrega o bloco na linha da cache
                        cache[indiceCache].bloco[i] = memoriaPrincipal[bloco][i];
                }
        }

        //Atualiza metadados da linha da cache
        cache[indiceCache].tag    = bloco;
        cache[indiceCache].valido = 1;

        return cache[indiceCache].bloco[deslocamento];
}




int main() {
	int memoriaPrincipal[MP_BLOCKS][BLOCK_SIZE];
	int endereco, finalizar, dado;
	CacheLine  cache[CACHE_LINES];

	for (int i=0; i<MP_BLOCKS; i++) {
		for (int j=0; j<BLOCK_SIZE; j++) {
			memoriaPrincipal[i][j] = i * 100 + j; //Preenche a MP
		}
	}

	for (int i=0; i<CACHE_LINES; i++) {
		cache[i].valido = 0; //Marca todas as linhas como inválidas no ínicio
	}

	printf("\nMP e Cache inicializadas com sucesso!\n");

	//Realiza a busca
	while (1) {
		printf("\nDigite um endereço (de 0 a %d): ", MP_BLOCKS*BLOCK_SIZE - 1);
	       	scanf("%d", &endereco);
    		
		dado = buscarNaCache(cache, memoriaPrincipal, endereco);
    		printf("\nDado obtido: %d\n", dado);

		printf("\nDeseja continuar buscando ? (1/0): ");
		scanf("%d", &finalizar);

		printf("\nTotal de %d HITs e %d MISSes\n", cacheHit, cacheMiss);

		if (finalizar == 0) { break; }
	}
	
	return 0;

}

/*
Descrição: Algoritmo genético para seleção de culturas para plantio
Autores: Elton Silva e Robert Gebhardt
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "culturasLucrativas.h"

int decToBin(unsigned n, char* strBin, unsigned len){
	int i, aux;

  // Checa falha de alocação
	if ( strBin == NULL )
		exit(EXIT_FAILURE);

	// Limpando string
	memset(strBin, 0, len); // ACHAR ESSE PROBLEMA FOI UM PESADELO

	for ( i = len-1 ; i >= 0 ; i-- )
	{
    // Usando operador de bits para pegar a quantidade de bits referente ao indice do for
		aux = n >> i;

    // Usa o AND com os bits para checar se o ultimo bit é 1
		if ( aux & 1 )
			strcat(strBin, "1");  // Adiciona caracter na string de bits
		else
			strcat(strBin, "0");

	}
	return  0;
}

int tamanhoEmBinario(unsigned n){

	if (n < 2) {
		return 1;
	}

	int aux, bitsMaximo;

	aux = 0;
	bitsMaximo = 10;

	while (aux == 0 && bitsMaximo > 1) {
		bitsMaximo--;
		// Aux recebe o n deslocado "bitsMaximo" vezes para a direita
    aux = n >> bitsMaximo;

  }
  bitsMaximo += 1;

	return bitsMaximo;
}

char** gerarPopulacaoInicial(unsigned tamanhoPopulacao, unsigned limiteValor, int* individuosExcluidos){

  int i, j;
  unsigned individuo, tamanhoStrBin;
  char **populacao;


  // Calcula a quantidade de bits para representar o valor maximo do inidividuo em binario
	tamanhoStrBin = tamanhoEmBinario(limiteValor);

  // Alocando espaço para a array de ponteiros para string
  populacao = malloc(sizeof (char*)*tamanhoPopulacao);

  // Gerando cada individuo randomicamente
  for ( i = 0; i < tamanhoPopulacao; i++) {
		populacao[i] = malloc(sizeof(char*)*tamanhoStrBin);

		// Culturas descansando
		if (individuosExcluidos[i]){
			decToBin(0, populacao[i], tamanhoStrBin);
			continue;
		}
		// Criando um individuo aleatorio
    individuo = 1 + rand() % limiteValor; // Individuos maiores que 0
    // Passando para binário
    decToBin(individuo, populacao[i], tamanhoStrBin);
  }

  return populacao;
}

double classificarIndividuo(char* individuo, char* individuoAnterior){

  double valorIndividuo;
	unsigned individuoDec, individuoAnteriorDec;

  // Converte pra decimal
  individuoDec = strtol(individuo, NULL, 2);
  individuoAnteriorDec = strtol(individuoAnterior, NULL, 2);

	// Se for um talhao descansando
	if (individuoDec == 0) {
		return individuoAnteriorDec*-0.5*10;
	}

  valorIndividuo = individuoDec*10;

  // Se não houve individuo anterior
  if (individuoAnteriorDec == 0) {
    return valorIndividuo;
  }

  // AND com os bits do valor verifica se os dois são pares ou os dois são impares
  if (((individuoDec & 1) && (individuoAnteriorDec & 1)) || (!(individuoDec & 1) && !(individuoAnteriorDec & 1)))
		valorIndividuo = valorIndividuo*0.5;
  else
		valorIndividuo += valorIndividuo*0.2;

  return valorIndividuo;
}

char** roletarIndividuos(char** populacao, char** populacaoAnterior, unsigned tamanhoPopulacao, int numSelecionados){
	unsigned r;
  double valorTotal, valor;
  char** selecionados;
  int individuo, i;

	valor = 0;
	valorTotal = 0;

  selecionados = malloc(sizeof (char*)*numSelecionados);

	// Calcula valor total da geracao
  for (individuo = 0; individuo < tamanhoPopulacao; individuo++) {
    valorTotal += classificarIndividuo(populacao[individuo], populacaoAnterior[individuo]);
  }

	// Seleciona individuos
  for (i = 0; i < numSelecionados; i++) {

		if (valorTotal < 0 ) {
			r = 0;
		}
		else	// Sorteia um valor entre 0 e o valorTotal
			r = rand() % ((int)valorTotal + 1);

		// Percorre toda a população
    for ( individuo = 0; individuo < tamanhoPopulacao; individuo++) {
      valor += classificarIndividuo(populacao[individuo], populacaoAnterior[individuo]);

			// if (valor < 0) {
			// 	valor *= -1;
			// }
      if (valor >= r) {
        selecionados[i] = populacao[individuo];
        // break;
				valor = 0;
      }
    }
  }

  return selecionados;
}

int crossoverUmPonto(char* pai, char* mae, char* filho, unsigned limiteValor){

	unsigned filhoDec, paiDec, maeDec;
  unsigned correcao;
  unsigned quantidadeBits = strlen(pai);
  unsigned pontoCruzamento = 1 + rand()%(quantidadeBits-1);

	if (filho == NULL) {
		printf("DEU MERDINHA\n");
		exit(0);
	}
	paiDec = strtol(pai, NULL, 2);
	maeDec = strtol(mae, NULL, 2);

	// Pega parte da mãe até o ponto e a outra parte do pai
	filhoDec = (maeDec & (63 << pontoCruzamento)) | (paiDec & ~(63 << pontoCruzamento));


	// Se der zero pega a outra parte do mae com a outra parte do pai
	if (filhoDec == 0){
		filhoDec = (maeDec & ~(63 << pontoCruzamento)) | (paiDec & (63 << pontoCruzamento));
	}

	// Se for maior que o limite
  if (filhoDec > limiteValor){
		filhoDec = filhoDec-limiteValor;
	}
	decToBin(filhoDec, filho, quantidadeBits);
  return 0;
}

int mutarIndividuo(char* individuo, unsigned limiteValor){
	unsigned qtdBits = strlen(individuo);
	unsigned individuoDec = strtol(individuo, NULL, 2);
	// Inverte um unico gene do cromossomo
	unsigned mutante = individuoDec ^ (1 << (rand()%(qtdBits-1)));
	// Corrige se for criado um indíviduo inválido
	if (mutante > limiteValor) {
		mutante = mutante - limiteValor;
	}
	decToBin(mutante, individuo, qtdBits);
	// Refaz mutação se der 0
	if (mutante == 0)
		return mutarIndividuo(individuo, limiteValor);
	return 0;
}

double valorTotalPopulacao(char** populacao, char** populacaoAnterior, unsigned tamanhoPopulacao){
  unsigned individuo;
  double total = 0;
  for ( individuo = 0; individuo < tamanhoPopulacao; individuo++) {
    total += classificarIndividuo(populacao[individuo], populacaoAnterior[individuo]);
		// printf("%f\n", total);
  }
  return total;
}

int* descansarTalhoes(char** populacao, char** populacaoAnterior, unsigned tamanhoPopulacao){
	int* individuos;
	int individuo;

	individuos = malloc(sizeof(int)*tamanhoPopulacao);

	for ( individuo = 0; individuo < tamanhoPopulacao; individuo++) {
		if (strtol(populacao[individuo], NULL, 2) == 0){
			individuos[individuo] = 0;
			continue;
		}
		if (strtol(populacao[individuo], NULL, 2) == strtol(populacaoAnterior[individuo], NULL, 2)){
			individuos[individuo] = 1;
			continue;
		}
		individuos[individuo] = 0;
	}

	return individuos;
}

void freeArrayTalhoes(char** array){
	if (array == NULL)
		return;

	char **a;

  for ( a = array; *a; a++ )
    if (*a != NULL)
	    free( *a );

  free( array );
	array = NULL;
}

int main(int argc, char const *argv[]) {
  srand(time(NULL));

  unsigned plantio, talhao, valorIndividuo, geracao, qtdBits;
	double valorGeracao;
	double valorGeracaoEstagnado = 0;
	double lucroTotal = 0;
	unsigned intervaloGeracao, contador, selecionado, individuoAleatorio;
  char** selecionados;
	char** selecionados2;
  unsigned plantios[NUMERO_ESTACOES][NUMERO_TALHOES];
	double lucrosPlatios[NUMERO_ESTACOES];
	unsigned qtdGeracoes[NUMERO_ESTACOES];
	int* talhoesDescansando;
  char** plantioAtual;
  char** plantioAnterior;
  char** plantioNovo;
	char** plantioDescartado = NULL;

	// Quantidade de bits necessario para representar o numero em binario
	qtdBits = tamanhoEmBinario(NUMERO_CULTURAS);
	// Parte da população que fara crossover
	intervaloGeracao = (unsigned) NUMERO_TALHOES*INVERVALO_GERACAO;

	geracao = 1;
	valorGeracao = 0;

	// Gerando populações inicias
	talhoesDescansando = malloc(sizeof(int)*NUMERO_TALHOES);
	for (talhao = 0; talhao < NUMERO_TALHOES; talhao++) {
		talhoesDescansando[talhao] = 1; // Define todos os talhoes como descansando
	}
	// Alocando população anterior como todos talhoes descansando
	plantioAnterior = gerarPopulacaoInicial(NUMERO_TALHOES, NUMERO_CULTURAS, talhoesDescansando);
	// Gerando primeira população a ser avaliada
	for (talhao = 0; talhao < NUMERO_TALHOES; talhao++) {
		talhoesDescansando[talhao] = 0;	// Define todos os talhoes como ativos
	}
  plantioAtual = gerarPopulacaoInicial(NUMERO_TALHOES, NUMERO_CULTURAS, talhoesDescansando);

	free(talhoesDescansando);

	// Iniciando o algoritmo
	for (plantio = 0; plantio < NUMERO_ESTACOES; plantio++) {

		while (1) {	// Loop das gerações

			// Alocando ponteiro para geração nova
			plantioNovo = malloc(sizeof (char*)*NUMERO_TALHOES);

			// Efetuando crossover em parte da população
			for ( talhao = 0; talhao < intervaloGeracao; talhao++) {

				plantioNovo[talhao] = malloc(sizeof (char)*qtdBits);

				// Checa se o talhao está descansando
				if (strtol(plantioAtual[talhao], NULL, 2) == 0) {
					decToBin(0, plantioNovo[talhao], qtdBits);
					continue;
				}

				// Seleciona 2 indivíduos por roleta
				selecionados2 = roletarIndividuos(plantioAtual, plantioAnterior, NUMERO_TALHOES, 2);

				// Efetua o cruzamento
				crossoverUmPonto(selecionados2[0], selecionados2[1], plantioNovo[talhao], NUMERO_CULTURAS);
				free(selecionados2);
			}

			// Copiando indivíduos na outra parte restante da população
			selecionados = roletarIndividuos(plantioAtual, plantioAnterior, NUMERO_TALHOES, NUMERO_TALHOES - intervaloGeracao);
			for (size_t i = 0; i < NUMERO_TALHOES - intervaloGeracao; i++) {
				plantioNovo[talhao] = malloc(sizeof (char)*qtdBits);

				// Checa se o talhao está descansando
				if (strtol(plantioAtual[talhao], NULL, 2) == 0) {
					decToBin(0, plantioNovo[talhao], qtdBits);
					talhao++;
					continue;

				}
				// Individuo copiado da geracao anterior
				decToBin(
					strtol(selecionados[i], NULL, 2),
					plantioNovo[talhao],
					qtdBits
				);
				talhao++;
			}

			// Aplica mutação com a chance de acontecer determinada pelo INTERVALO MUTAÇÂO
			if ((1 + rand() % 100) < (INVERVALO_MUTACAO * 100)) {
				// Seleciona um individuo aleatorio
				individuoAleatorio = rand() % NUMERO_TALHOES;
				// Se NÃO for um talhao descansando
				if (strtol(plantioAtual[individuoAleatorio], NULL, 2) != 0)
					mutarIndividuo(plantioNovo[individuoAleatorio], NUMERO_CULTURAS);
			}

			// Liberando plantioAtual
			free(selecionados);
			freeArrayTalhoes(plantioAtual);
			plantioAtual = &(plantioNovo[0]);

			// Valor total da população
			valorGeracao = valorTotalPopulacao(plantioAtual, plantioAnterior, NUMERO_TALHOES);
			printf("%d - %f\n", geracao, valorGeracao);

			//Condições de parada
			if (geracao > LIMITE_GERACAO || valorGeracao > LIMITE_LUCRO )
				break;

			geracao++;
		}	// FIM LOOP GERAÇAO

		// Guarda a quantidade de gerações que o plantio atual teve
		qtdGeracoes[plantio] = geracao;
		lucrosPlatios[plantio] = valorGeracao;

		geracao = 1;
		valorGeracao = 0;
		// Verificando talhoes descansando da próxima geracao
		talhoesDescansando = descansarTalhoes(plantioAtual, plantioAnterior, NUMERO_TALHOES);
		// Liberando plantio anterior
		freeArrayTalhoes(plantioAnterior);

		plantioAnterior = plantioAtual;

		for ( talhao = 0; talhao < NUMERO_TALHOES; talhao++) {
			plantios[plantio][talhao] = strtol(plantioAtual[talhao], NULL, 2);
		}

		// Gerando população Inicial para próxima estação
		plantioAtual = gerarPopulacaoInicial(NUMERO_TALHOES, NUMERO_CULTURAS, talhoesDescansando);

		free(talhoesDescansando);

	}

	// Imprimi resposta
	for ( plantio = 0; plantio < NUMERO_ESTACOES; plantio++) {
		printf("[");
		for (talhao = 0; talhao < NUMERO_TALHOES-1; talhao++) {
			printf("%d, ", plantios[plantio][talhao]);
		}
		printf("%d", plantios[plantio][talhao]);
		printf("] - ");
		printf("%f - ", lucrosPlatios[plantio] );
		printf("%d\n", qtdGeracoes[plantio] );
		lucroTotal += lucrosPlatios[plantio];
	}
	printf("Lucro total: %f\n", lucroTotal);
  return 0;
}

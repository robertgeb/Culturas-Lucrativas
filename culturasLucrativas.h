/*
Descrição: Algoritmo genético para seleção de culturas para plantio
Autores: Elton Silva e Robert Gebhardt
*/

// Constantes
#define NUMERO_ESTACOES         10
#define NUMERO_TALHOES          10
#define NUMERO_CULTURAS         5
#define INVERVALO_GERACAO       0.7   // Porcentagem da população que sera resultante de crossover
#define INVERVALO_MUTACAO       0.02  // Chance de uma mutação acontecer
#define LIMITE_GERACAO          400   // Limite de gerações para finalizar o Algoritmo
#define LIMITE_LUCRO            550   // Limite de lucro que finaliza o algoritmo
#define LIMITE_ESTAGNACAO       0.2   // Não implementado

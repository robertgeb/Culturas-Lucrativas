### Instruções

    $ git clone // Clone
    // Modifique as constantes do arquivo culturasLucrativas.h se quiser
    $ make // Compile
    $ ./culturasLucrativas // Execute

### Componentes do Algoritmo Genético

  * Representação dos cromossomos
  * Gerador de população inicial
  * Classificador de aptidão dos indivíduos
  * Operadores genéticos(crossover ou mutação)


### Passos do Algoritmo Genético

  1. Criar população inicial
  2. Enquanto um **critério de parada** não for satisfeito:
    1. Classifica a aptidão de cada indivíduo da população
    2. Gera nova população através da aplicação das **operaçõs genéticas** nos melhores indivíduos classificados
  3. A população é selecionada após o critério ou **critérios de parada** serem satisfeitos

### Operações genéticas

  * Copía de um indivíduo classificado para uma nova geração
  * Geração de dois indivíduos pela conbinação crossover de 2 indivíduos
  * Mutação aleatória de um individuo criando um novo indivíduo

### Critérios de parada

  * Quantidade limite de gerações
  * Tempo limite de processamento
  * Estagnação de evolução da aptidão das populações

# Algoritmos Construtivos para o Problema do Conjunto Dominante de Peso Mínimo (MWDS)

Implementação e avaliação experimental de três algoritmos construtivos para o **Minimum Weight Dominating Set (MWDS)**, um problema NP-difícil de dominância em grafos com aplicações em redes de sensores, alocação de recursos e otimização de cobertura.

Dado um grafo `G = (V, E)` com peso `w(v)` associado a cada vértice, o objetivo é encontrar um subconjunto `D ⊆ V` tal que todo vértice do grafo esteja em `D` ou seja adjacente a algum vértice de `D`, minimizando a soma dos pesos dos vértices selecionados.

## Algoritmos Implementados

### 1. Guloso
Constrói a solução iterativamente, selecionando a cada passo o vértice com melhor razão entre número de vértices ainda não dominados que ele cobre e seu peso. Versão adaptativa: a razão de cada candidato é recalculada a cada iteração, e vértices já cobertos (mas fora do conjunto dominante) permanecem elegíveis, o que melhorou a qualidade das soluções em relação à remoção total de candidatos dominados.

### 2. Guloso Randomizado Adaptativo (GRASP)
Randomiza a escolha do próximo vértice dentro de uma lista restrita dos `α × n` melhores candidatos (RCL), permitindo gerar soluções diferentes a cada execução. A lista de candidatos é reordenada a cada iteração (insertion sort) com base na razão atualizada.

### 3. Guloso Randomizado Adaptativo Reativo
Evolução do GRASP: em vez de um único valor de α, utiliza um conjunto de valores e ajusta dinamicamente, por roleta probabilística, a chance de cada α ser escolhido, com base no desempenho histórico observado em blocos de iterações. Resolve a dificuldade de calibrar manualmente o parâmetro α.

## Experimentos e Resultados

Os três algoritmos foram avaliados sobre **18 instâncias** (25, 40 e 60 vértices, fornecidas para a disciplina), comparando:

- **Qualidade da solução**, medida pelo desvio percentual em relação à melhor solução encontrada entre os algoritmos;
- **Tempo de execução**, também analisado por desvio percentual;
- Guloso: execução única (determinístico); Randomizado: média de 60 execuções (20 por integrante); Reativo: média de 30 execuções (10 por integrante).

**Principal resultado:** o algoritmo Reativo obteve consistentemente as melhores soluções (menor desvio percentual médio em relação ao best), confirmando que o ajuste dinâmico de parâmetros compensa o maior custo computacional da abordagem. O Guloso puro, por sua vez, mostrou-se mais suscetível a ótimos locais, especialmente em instâncias mais densas.

Detalhes completos de metodologia, parâmetros (valores de α, número de iterações, ambiente computacional) e tabelas de resultados estão no [relatório técnico do projeto](#) *(adicione aqui o link, se for compartilhar o PDF)*.

## Compilando e Executando

O nome do arquivo de entrada é informado via linha de comando (`argv`).

**Compilação** (Linux ou macOS):
```bash
g++ *.cpp -o execGrupoX
```

**Execução:**
```bash
./execGrupoX <arquivo_entrada>
```

Onde `<arquivo_entrada>` é o nome do arquivo contendo as informações do grafo. Se o arquivo estiver em uma subpasta de `instancias`, informe o caminho relativo:

```bash
./execGrupoX instancias_t2/<arquivo_entrada>
```

## Autoria

Trabalho desenvolvido em grupo para a disciplina de Teoria dos Grafos (UFJF):
- Emerson Caneschi Coelho de Souza
- Emmanuel Gomes Nassif
- Quezia Emanuelly

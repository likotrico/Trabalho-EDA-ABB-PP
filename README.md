# Estrutura de Dados Avançadas - Trabalho 01

Igor Torquato Maia Sales - 603155
Murillo Inácio da Costa Silva - 603284

# Árvore Binária de Busca com Persistência Parcial

Trabalho da disciplina de Estruturas de Dados Avançadas (EDA) que implementa, em C++, uma **Árvore Binária de Busca (ABB) com persistência parcial**.

Na persistência parcial, toda operação de atualização (inserção ou remoção) gera uma **nova versão** da árvore, e todas as versões anteriores continuam acessíveis para **consulta**. Apenas a versão mais recente pode ser modificada.

A técnica usada é a dos **nós gordos com cópia limitada** (*node copying*): cada nó guarda um número fixo de modificações. Quando esse espaço acaba, o nó é **clonado** e a alteração se propaga para o pai através de **ponteiros de retorno**.

Este README.md, assim como o makefile, foi feito utilizando a inteligência artifical ClaudeAI. Todos os comentários presentes no código foram feitos manualmente pelos membros da equipe do trabalho. A inteligência artificial foi utilizada para organizar os comentários das funções presentes no código no README.md, exceto o makefile que foi completamente gerado e explicado utilizando IA. O link público para acessar o chat utilizado é: https://claude.ai/share/a580e0c0-d423-40e7-af9e-636c2e532579.

---

## Sumário

- [Estrutura do projeto](#estrutura-do-projeto)
- [Compilação e execução](#compilação-e-execução)
- [Formato da entrada](#formato-da-entrada)
- [Estruturas de dados](#estruturas-de-dados)
- [Funções](#funções)
  - [structures.cpp](#structurescpp)
  - [functions.cpp](#functionscpp)
  - [insert.cpp](#insertcpp)
  - [remove.cpp](#removecpp)
  - [sucessor.cpp](#sucessorcpp)

---

## Estrutura do projeto

```
Trabalho-EDA-ABB-PP/
├── main.cpp               # Lê os comandos da entrada e chama as operações
├── makefile               # Comandos de compilação e execução
├── headers/
│   ├── structures.h       # Constantes globais, enum e structs (node, tree, internal_info)
│   ├── functions.h        # Leitura de nós, modificações, impressão e busca
│   ├── insert.h           # Inserção
│   ├── remove.h           # Remoção
│   └── sucessor.h         # Sucessor
└── functions/
    ├── structures.cpp     # Criação da árvore, de nós e de modificações
    ├── functions.cpp      # Funções auxiliares e de persistência
    ├── insert.cpp         # Implementação da inserção
    ├── remove.cpp         # Implementação da remoção
    └── sucessor.cpp       # Implementação da busca pelo sucessor
```

O `main.cpp` inclui diretamente os arquivos `.cpp` da pasta `functions/`, por isso basta compilar apenas ele.

---

## Compilação e execução

O `makefile` oferece os seguintes comandos:

| Comando      | O que faz |
|--------------|-----------|
| `make build` | Compila o `main.cpp` com o `g++` e gera o executável `programa`. Só recompila se algum arquivo de código (`main.cpp`, `functions/*.cpp` ou `headers/*.h`) tiver mudado. |
| `make run`   | Compila, se necessário, e executa `programa` usando como entrada o arquivo da variável `INPUT`. |
| `make test`  | Compila, se necessário, roda o programa para todos os casos de teste, compara as saídas com as esperadas e mostra o resultado de cada teste. |
| `make clean` | Apaga o executável `programa` e os arquivos `.resp` e `.diff` gerados pelos testes. |

### Compilar e executar com uma entrada

```bash
make build
make run
# ou, com outro arquivo de entrada:
make run INPUT=teste/entrada/nome_arquivo.in
```

### Rodar os testes

Os testes usam a seguinte organização de pastas:

```
teste/
├── entrada/           # [nome_arquivo].in   → entradas (criada por você)
├── saida_esperada/    # [nome_arquivo].out  → saídas esperadas (criada por você)
├── saida/             # [nome_arquivo].resp → saídas geradas pelo programa (criada pelo make)
└── diferenca/         # [nome_arquivo].diff → diferenças encontradas pelo diff (criada pelo make)
```

Para usar:

1. Coloque cada entrada em `teste/entrada/[nome_arquivo].in`.
2. Coloque a saída esperada correspondente, com o **mesmo nome**, em `teste/saida_esperada/[nome_arquivo].out`.
3. Rode:

   ```bash
   make test
   ```

Para cada arquivo `.in`, o `make test`:

1. executa `./programa < teste/entrada/[nome_arquivo].in` e salva a saída em `teste/saida/[nome_arquivo].resp`;
2. compara esse arquivo com `teste/saida_esperada/[nome_arquivo].out` usando o `diff` do Linux;
3. salva a diferença em `teste/diferenca/[nome_arquivo].diff`.

Ao final, é mostrado o resultado de cada teste:

```
OK:     teste/diferenca/teste01.diff
FALHOU: teste/diferenca/teste02.diff
```

- **OK:** o `.diff` está vazio, ou seja, a saída do programa é igual à esperada.
- **FALHOU:** o `.diff` tem conteúdo. Abra esse arquivo para ver as diferenças. Linhas com `<` vêm da saída do programa (`.resp`) e linhas com `>` vêm da saída esperada (`.out`).

Observações:

- As pastas `teste/saida` e `teste/diferenca` são criadas automaticamente se não existirem.
- Se um `.in` não tiver o `.out` correspondente, o `make` para com o erro `No rule to make target 'teste/diferenca/[nome_arquivo].diff'`. Nesse caso, confira se o `.out` existe e tem o mesmo nome do `.in`.
- Ao rodar `make test` de novo, só são refeitos os testes cujos arquivos `.in` ou `.out` mudaram, ou todos eles se o código for alterado.

### Limpar os arquivos gerados

```bash
make clean
```

Use antes de rodar os testes do zero.

---

## Formato da entrada

O programa lê comandos da entrada padrão, um por linha:

| Comando             | Descrição | Saída |
|---------------------|-----------|-------|
| `INC x`             | Insere o valor `x` na árvore, gerando uma nova versão. | — |
| `REM x`             | Remove o valor `x` da árvore, gerando uma nova versão. | — |
| `SUC x v`           | Busca o sucessor de `x` na versão `v`. | Ecoa o comando e imprime o sucessor, ou `inf` se não existir. |
| `IMP v`             | Imprime os elementos da versão `v` em ordem crescente. | Ecoa o comando e imprime os valores em ordem crescente separados por vírgula. |

Exemplo:

```
INC 10
INC 5
INC 15
SUC 10 3
IMP 3
REM 10
IMP 4
```

Se a versão pedida em `IMP` for maior que a última existente, é impressa a versão mais recente.

---

## Estruturas de dados

Definidas em `headers/structures.h`.

### Constantes e variáveis globais

| Nome               | Valor | Descrição |
|--------------------|-------|-----------|
| `number_p`         | `1`   | Valor de *p*: quantidade máxima de ponteiros de retorno em cada nó. |
| `version`          | `0`   | Número da última versão da árvore. |
| `maximum_versions` | `1001`| Número máximo de versões suportadas. |
| `queue`            | `std::vector<int>` | Estrutura auxiliar usada para imprimir os elementos em ordem crescente numa versão requerida. |

### `enum type_field`

Campos que podem ser alterados por uma modificação:

- `EMPTY` — posição de modificação vazia;
- `L_SON` — filho esquerdo;
- `R_SON` — filho direito.

### `struct internal_info`

Armazena **uma modificação** feita em um nó.

| Campo     | Tipo     | Descrição |
|-----------|----------|-----------|
| `version` | `int`    | Versão em que a modificação passa a valer. |
| `value`   | `node*`  | Novo valor do campo (o novo filho). |
| `field`   | `int`    | Campo alterado (`L_SON`, `R_SON` ou `EMPTY`). |

### `struct node`

O nó da árvore.

| Campo             | Tipo                         | Descrição |
|-------------------|------------------------------|-----------|
| `value`           | `int`                        | Chave armazenada. |
| `version`         | `int`                        | Versão em que o nó foi criado. |
| `l_son`, `r_son`  | `node*`                      | Filhos esquerdo e direito originais do nó. |
| `return_pointers` | `node*[number_p]`            | Ponteiros de retorno (para o pai). |
| `mods`            | `internal_info[2*number_p]`  | Lista de modificações do nó (capacidade `2p`). |

### `struct tree`

Contém o vetor `vector_of_roots[maximum_versions]`, com a **raiz de cada versão** da árvore. A versão `i` é acessada a partir de `vector_of_roots[i]`.

---

## Funções

### `structures.cpp`

| Função | Descrição |
|--------|-----------|
| `tree* create_tree()` | Cria a estrutura da árvore (todas as raízes como `NULL`) e a retorna. |
| `internal_info create_mod()` | Cria a estrutura interna de uma modificação, válida a partir da versão `version + 1`, e a retorna. |
| `node* create_node()` | Cria um novo nó na versão `version + 1`, com filhos, ponteiros de retorno e modificações vazios, e o retorna. |

### `functions.cpp`

Funções auxiliares e o núcleo da persistência.

| Função | Descrição |
|--------|-----------|
| `node* read_node()` | Faz a leitura do nó na versão requerida. Aplica sobre os filhos originais todas as modificações com versão ≤ `version_req` e retorna uma **cópia** do nó com os filhos atualizados (a cópia deve ser liberada com `clear_node`). |
| `void clear_node()` | Limpa a memória de um nó. |
| `node* clone_node()` | Clona um nó cujas modificações estão cheias, criando-o na versão `version + 1`. Retorna o nó clonado. |
| `void mod_overflow_DOWN()` | Atualiza os ponteiros de retorno dos filhos de um nó clonado, trocando o pai antigo pelo novo. |
| `node* mod_overflow_UP()` | Adiciona uma modificação no pai, substituindo o filho antigo pelo novo no campo em que ele for encontrado (esquerdo ou direito). Retorna o nó que recebeu a modificação. |
| `node* mod_overflow_UP_ROOT()` | Substitui a raiz na versão mais atualizada. Retorna a nova raiz. |
| `node* add_mod()` | Adiciona uma modificação no campo requerido. Se houver espaço em `mods`, grava a modificação; caso contrário, clona o nó, atualiza os filhos (`mod_overflow_DOWN`) e propaga a mudança para o pai (`mod_overflow_UP`) ou para a raiz (`mod_overflow_UP_ROOT`). |
| `void print_order()` | Função recursiva que insere na `queue` os elementos em ordem crescente (percurso em ordem). |
| `void print_order_tree()` | Imprime os elementos da árvore na versão requerida, separados por vírgula. Se a versão pedida for maior que a atual, imprime a mais recente. |
| `int node_is_leaf()` | Verifica se um nó é folha na versão requerida. |
| `node* search_node()` | Procura um nó com o valor dado na versão requerida. Retorna o nó encontrado ou `NULL` caso não exista. |

### `insert.cpp`

| Função | Descrição |
|--------|-----------|
| `node* insert()` | Função auxiliar recursiva que recebe o nó a ser adicionado e um possível pai. Desce pela árvore (valores menores ou iguais à esquerda, maiores à direita) até achar uma posição livre, define o ponteiro de retorno do novo nó e registra a modificação no pai com `add_mod`. Retorna o nó inserido. |
| `void insert_node()` | Recebe um valor e o adiciona na ABB com persistência parcial. Copia a raiz da versão atual para a próxima, insere o nó como raiz (se a árvore estiver vazia) ou via `insert`, e incrementa `version`. Rejeita valores não positivos e o excesso do número máximo de versões. |

### `remove.cpp`

A remoção trata os três casos clássicos da ABB: nó folha, nó com apenas um filho e nó com dois filhos (substituído pelo sucessor).

| Função | Descrição |
|--------|-----------|
| `void remove()` | Remove da árvore o nó com o valor informado, gerando uma nova versão. Busca o nó com `search_node` e trata o caso adequado. |
| `int has_only_son()` | Verifica se um nó possui apenas um filho. |
| `node* add_unknown_mod()` | Modifica os nós de baixo para cima, partindo do nó a ser removido (casos folha ou um filho). Substitui `old_node` por `new_node` no campo correto do pai. Se o pai for `NULL`, o novo nó vira raiz. Retorna o pai mais atualizado, ou `NULL` se o nó atual for `NULL`. |
| `node* add_unknown_mod_to_remove()` | Modifica os nós de baixo para cima, partindo do sucessor, até chegar ao nó a ser removido. Retorna o último nó modificado/clonado. |
| `node* add_mod_to_remove()` | Adiciona uma modificação em um campo dos nós entre o sucessor e o nó a ser removido, clonando o nó se não houver espaço. Retorna o último nó modificado/clonado. |
| `node* add_mode_to_remove_overflowUP()` | Atualiza o pai de um nó que foi clonado durante a remoção. Retorna o último nó modificado/clonado ou para ao chegar no nó a ser removido. |

**Casos da remoção:**

1. **Folha** — o pai passa a apontar para `NULL` no lugar do nó (`add_unknown_mod`).
2. **Apenas um filho** — o pai passa a apontar diretamente para o único filho.
3. **Dois filhos** — encontra o sucessor; o filho direito do sucessor assume o lugar dele (propagando com `add_unknown_mod_to_remove`); o sucessor recebe os filhos do nó removido e ocupa sua posição no pai.

### `sucessor.cpp`

| Função | Descrição |
|--------|-----------|
| `node* find_strictly_greater()` | Função auxiliar que encontra o sucessor descendo sempre à esquerda até o elemento mais à esquerda da subárvore. Retorna o sucessor ou `NULL`. |
| `node* successor()` | Retorna o nó sucessor de um valor numa dada versão. Se o elemento tiver filho direito, o sucessor é o menor elemento dessa subárvore; caso contrário (ou se o valor não existir), é o último ancestral em que a busca desceu para a esquerda. Retorna `NULL` se não houver sucessor. |
| `void successor_tree()` | Imprime o valor do sucessor numa dada versão, ou `inf` se ele não existir. |

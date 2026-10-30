INPUT = 'testes/teste01.in'

# Pastas usadas nos testes.
DIR_ENTRADA   = teste/entrada
DIR_SAIDA     = teste/saida
DIR_ESPERADA  = teste/saida_esperada
DIR_DIFERENCA = teste/diferenca

# Lista de arquivos [nome_arquivo].in na pasta de entrada e os arquivos .resp e .diff correspondentes.
ENTRADAS = $(wildcard $(DIR_ENTRADA)/*.in)
RESPOSTAS = $(patsubst $(DIR_ENTRADA)/%.in,$(DIR_SAIDA)/%.resp,$(ENTRADAS))
DIFERENCAS = $(patsubst $(DIR_ENTRADA)/%.in,$(DIR_DIFERENCA)/%.diff,$(ENTRADAS))

# Arquivos de código usados para gerar o programa.
FONTES = main.cpp $(wildcard functions/*.cpp) $(wildcard headers/*.h)

# O comando `make build` compila o conteúdo do arquivo `main.cpp` usando o compilador `g++` e cria o programa executável `programa` como saída.
.PHONY: build
build: programa

programa: $(FONTES)
	g++ -o programa main.cpp

# O comando `make run` coloca `programa` para rodar passando como entrada o arquivo fornecido pela variável `INPUT`.
.PHONY: run
run: programa
	@./programa < $(INPUT)

# O comando `make test` roda o programa para cada arquivo [nome_arquivo].in da pasta teste/entrada,
# gera teste/saida/[nome_arquivo].resp e compara com teste/saida_esperada/[nome_arquivo].out usando o `diff`,
# salvando a diferença em teste/diferenca/[nome_arquivo].diff.
.PHONY: test
test: $(RESPOSTAS) $(DIFERENCAS)
	@for f in $(DIFERENCAS); do \
		if [ -s $$f ]; then echo "FALHOU: $$f"; else echo "OK:     $$f"; fi; \
	done

# Gera a saída [nome_arquivo].resp a partir da entrada [nome_arquivo].in.
$(DIR_SAIDA)/%.resp: $(DIR_ENTRADA)/%.in programa | $(DIR_SAIDA)
	./programa < $< > $@

# Compara [nome_arquivo].resp com [nome_arquivo].out e salva a diferença em [nome_arquivo].diff.
# O `diff` termina com erro quando os arquivos são diferentes, por isso o `|| true` evita que o make pare.
$(DIR_DIFERENCA)/%.diff: $(DIR_SAIDA)/%.resp $(DIR_ESPERADA)/%.out | $(DIR_DIFERENCA)
	diff $(DIR_SAIDA)/$*.resp $(DIR_ESPERADA)/$*.out > $@ || true

# Cria as pastas de saída e de diferença caso não existam.
$(DIR_SAIDA) $(DIR_DIFERENCA):
	mkdir -p $@

# O comando `make clean` apaga o executável e os arquivos gerados pelos testes.
.PHONY: clean
clean:
	rm -f programa $(DIR_SAIDA)/*.resp $(DIR_DIFERENCA)/*.diff
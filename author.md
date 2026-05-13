# Introduction

O "passgen" é um gerador de senhas de linha de comando (CLI) desenvolvido em C++.
O programa permite ao usuário configurar o tamanho da senha e os grupos de caracteres
que devem compô-la, garantindo que ao menos um caractere de cada grupo selecionado
esteja presente na senha final. A qualidade da senha gerada é avaliada com base na
entropia e classificada em 6 categorias.

# Author(s)

- Nome: Heitor Gomes de Sena Melo Mariz | Guilherme Soares Fialho Ribeiro Dantas
- Email: marizheitor@gmail.com | guilhermefialho@gmail.com 

# Problems found or limitations

- Distribuição de Probabilidade: A lógica de geração atual prioriza a inclusão de ao menos um caractere de cada grupo selecionado, o que é um requisito do projeto. No entanto, para o preenchimento do restante da senha, o programa utiliza um "pool" único. Isso significa que grupos com mais caracteres (como letras) têm uma probabilidade estatística maior de aparecer do que grupos menores (como aspas), não havendo um controle de proporcionalidade entre os grupos ativos.

# Observations

As seguintes ferramentas e recursos externos foram utilizados como auxílio durante
o desenvolvimento do projeto:

- **IA generativa (Claude — Anthropic):** utilizada para entender o gerenciamento
  de múltiplos arquivos em C++ (separação em ".h" e ".cpp", processo de compilação
  e linkagem), para aprender a escrever comentários no estilo Doxygen, e para
  entender a configuração do CMake com múltiplos arquivos fonte (CMakeLists.txt).

# Grading

| Item                                             | Valor máximo | Valor esperado |
| ------------------------------------------------ | :----------: | :------------: |
| Interface de acordo com o especificado           |      20      |      20        |
| Seleciona caracteres dos grupos adequadamente    |      15      |      15        |
| Apresenta a tela de ajuda quando solicitado      |      5       |       5        |
| Trata os erros de entrada corretamente           |      12      |      12        |
| Avalia a qualidade da senha com 5 categorias     |      10      |      10        |
| Implementa os 9 grupos de caracteres solicitados |      18      |      18        |
| Utiliza corretamente a aleatoriedade             |      10      |      10        |

# Compiling and Running

## Compilando com CMake ou Diretamente(Clang ou g++)

```bash
Compilar usando Cmake

cmake -S . -B build
cmake --build build
```
```bash
Compilar usando g++ 

g++ -std=c++17 -Wall -o passgen src/main.cpp src/password.cpp
```

## Executando

```bash
# Senha padrão (7 chars, letras minúsculas)
./build/passgen

# Senha de 10 chars com letras maiúsculas e minúsculas, modo verbose e flag curtas combinada -lu (lower e upper)
./build/passgen --len 10 -lu --verbose

# Senha de 15 chars com todos os grupos
./build/passgen --len 15 --all-groups --verbose

# Forçar tamanho 3 com todos os grupos ativos
./build/passgen --len 3 --all-groups --force

# Exibir tela de ajuda
./build/passgen --help
```
```
Para facilitar a compreensão, aqui estão as opções de grupos de caracteres e configurações disponíveis:

Uso: ./passgen [<opções>]

--len n: Define o tamanho da senha (o padrão é 7).
-l, --lower: Adiciona caracteres aleatórios de [a-z].
-u, --upper: Adiciona caracteres aleatórios de [A-Z].
-d, --digits: Adiciona caracteres aleatórios de [0-9].
-o, --logograms: Adiciona caracteres aleatórios de [#$%&@^`~].
-p, --punctuation: Adiciona caracteres aleatórios de [.,;:].
-q, --quotes: Adiciona caracteres aleatórios de [" '].
-s, --slashes: Adiciona caracteres aleatórios de [/|_-].
-m, --math: Adiciona caracteres aleatórios de [*+!?=].
-b, --braces: Adiciona caracteres aleatórios de [{}].
-a, --all-groups: Ativa todos os grupos de caracteres listados acima.
-v, --verbose: Exibe informações extras de segurança (Variabilidade e Entropia).
-h, --help: Exibe esta tela de ajuda com as instruções de uso.
--force: Força a geração da senha mesmo que o comprimento seja menor que a quantidade de grupos ativos.
```

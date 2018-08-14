(PTBR) - Projeto de compilador feito em C.
Estrutura: 
- analisador_lexico.c -> recebe como entrada um arquivo txt contendo a linguagem requerida (Visualg PT) e gera um arquivo de saida contendo as estruturas token e atributos.
- analisador_semantico.c -> le o arquivo que geour de saida e analisa se as estruturas da linguagem e declaração semânticas estão corretas.

(EN) - Compiler Project made in C.
Structure:
- analisador_lexico.c (lexic analysis) - read input txt file that contains the language requirement (Visualg PT) and do an output with the meanings of tokens and attributes.
- analisador_semantico.c (semantic analysis) - read the output file before and read to verify if the structures and variable declarations are correct.


<b>Language requirement<b>
Below are the structures that contains on the code.
  
LEXIC SPECIFICATION
▪ Type:
inteiro: intenger that not contains float numbers. Example: 0,1,2,3,4..
logico: boolean value TRUE or FALSE.
▪ Declaration of variables
<array-of-vars> : <type>
In <array-of-vars>, the vars are separated by comas (,).


▪ Operations
+ - * \ mod 
> >= < <= exp 
= <> e ou

▪ IO
leia (<array-of-vars>)
escreva (<array-of-vars>)

▪ IF structure
se <expressão-lógica> entao
<sequência-de-comandos>
senao
<sequência-de-comandos>
fimse

▪ LOOP strucutre
para <variável> de <valor-inicial> ate <valor-limite> [passo <incremento>] faca
<sequência-de-comandos>
fimpara

enquanto <expressão-lógica> faca
<sequência-de-comandos>
fimenquanto

▪ Attribution
variável <- valor
variável <- variável
variável <- expressão

▪ Basic structure that requires to follow to works
algoritmo "semnome"
// Função :
// Autor :
// Data :
// Seção de Declarações
inicio
// Seção de Comandos
fimalgoritmo



<code>
﻿algoritmo "salario"
// Seção de Declarações
var sal:inteiro
i,contador:inteiro

inicio
// Seção de Comandos
i<-0;
contador<-0;
para i de 1 ate 5 passo 1 faca
   escreva("Digite o salario do funcionário ",i, ": ")
   leia (sal)
   se sal>500 entao
      contador<-contador+1
   fimse
fimpara
escreval(contador, " Funcionários recebem salários superiores a R$ 300" )
fimalgoritmo  
  
</code>

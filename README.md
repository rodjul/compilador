### (PTBR) - Projeto de compilador feito em C.<br>
Estrutura: <br>
- analisador_lexico.c -&gt; recebe como entrada um arquivo txt contendo a linguagem requerida (Visualg PT) e gera um arquivo de saida contendo as estruturas token e atributos.
- analisador_semantico.c -&gt; le o arquivo que geour de saida e analisa se as estruturas da linguagem e declara&ccedil;&atilde;o sem&acirc;nticas est&atilde;o corretas.

### (EN) - Compiler Project made in C.<br>
Structure:<br>
- analisador_lexico.c (lexic analysis) - read input txt file that contains the language requirement (Visualg PT) and do an output with the meanings of tokens and attributes.
- analisador_semantico.c (semantic analysis) - read the output file before and read to verify if the structures and variable declarations are correct.


### Language requirement <br>
Below are the structures that contains on the code.<br>


#### LEXIC SPECIFICATION<br>

Type:<br>
```
inteiro: intenger that not contains float numbers. Example: 0,1,2,3,4..
logico: boolean value TRUE or FALSE.
```

Declaration of variables<br>
```
<array-of-vars> : <type>
```
In &lt;array-of-vars&gt;, the vars are separated by comas (,). <br>


### Operations<br>
```
+ - * \ mod 
> >= < <= exp 
= <> e ou
```

### IO<br>
```
leia (<array-of-vars>)
escreva (<array-of-vars>)
```

### IF structure <br>
```
se <expressão-lógica> entao
<sequência-de-comandos>
senao
<sequência-de-comandos>
fimse
```

### LOOP strucutre <br>
```
para <variável> de <valor-inicial> ate <valor-limite> [passo <incremento>] faca
<sequência-de-comandos>
fimpara
```
```
enquanto <expressão-lógica> faca
<sequência-de-comandos>
fimenquanto
```

### Attribution <br> 
```
variável <- valor
variável <- variável
variável <- expressão
 ```
 
### Basic structure that requires to follow to works <br>
```
algoritmo "semnome"
// Função :
// Autor :
// Data :
// Seção de Declarações
inicio
// Seção de Comandos
fimalgoritmo
```

### Example of input.txt
```
algoritmo "salario"
// Seção de Declarações
var sal:inteiro
i,contador:inteiro

inicio
// Seção de Comando
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
```

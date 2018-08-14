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
inteiro: intenger that not contains float numbers. Example: 0,1,2,3,4..<br>
logico: boolean value TRUE or FALSE.<br>


Declaration of variables<br>
&lt;array-of-vars&gt; : &lt;type&gt; <br>
In &lt;array-of-vars&gt;, the vars are separated by comas (,). <br>


### Operations<br>
```
+ - * \ mod  <br>
&gt; &gt;= &lt; &lt;= exp  <br>
= &lt;&gt; e ou <br>
```

### IO<br>
```
leia (&lt;array-of-vars&gt;) <br>
escreva (&lt;array-of-vars&gt;) <br>
```

###IF structure <br>
```
se &lt;express&atilde;o-l&oacute;gica&gt; entao <br>
&lt;sequ&ecirc;ncia-de-comandos&gt; <br>
senao <br>
&lt;sequ&ecirc;ncia-de-comandos&gt; <br>
fimse <br>
```

### LOOP strucutre <br>
```
para &lt;vari&aacute;vel&gt; de &lt;valor-inicial&gt; ate &lt;valor-limite&gt; [passo &lt;incremento&gt;] faca <br>
&lt;sequ&ecirc;ncia-de-comandos&gt; <br>
fimpara <br>
```
```
enquanto &lt;express&atilde;o-l&oacute;gica&gt; faca <br>
&lt;sequ&ecirc;ncia-de-comandos&gt; <br> 
fimenquanto <br>
```

### Attribution <br> 
```
vari&aacute;vel &lt;- valor <br> 
vari&aacute;vel &lt;- vari&aacute;vel <br>
vari&aacute;vel &lt;- express&atilde;o <br>
 ```
 
### Basic structure that requires to follow to works <br>
```
algoritmo &quot;semnome&quot; <br>
// Fun&ccedil;&atilde;o : <br>
// Autor : <br>
// Data : <br> 
// Se&ccedil;&atilde;o de Declara&ccedil;&otilde;es <br>
inicio <br> 
// Se&ccedil;&atilde;o de Comandos <br>
fimalgoritmo <br>
```


```
algoritmo &quot;salario&quot; <br>
// Se&ccedil;&atilde;o de Declara&ccedil;&otilde;es <br>
var sal:inteiro <br>
i,contador:inteiro <br>

inicio <br>
// Se&ccedil;&atilde;o de Comando <br>
i&lt;-0; <br>
contador&lt;-0; <br>
para i de 1 ate 5 passo 1 faca <br>
   escreva(&quot;Digite o salario do funcion&aacute;rio &quot;,i, &quot;: &quot;) <br>
   leia (sal) <br>
   se sal&gt;500 entao <br>
      contador&lt;-contador+1 <br>
   fimse <br>
fimpara <br>
escreval(contador, &quot; Funcion&aacute;rios recebem sal&aacute;rios superiores a R$ 300&quot; ) <br>
fimalgoritmo  <br>
```

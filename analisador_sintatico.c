/*Leonardo Toniolo
Julio Brazil
Raphael Bertachine
Rodrigo Juliano M

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int errosSintaticos=0;
int errosSemanticos=0;
int position = 0;
int lines=0;

char file_saida[21] = "saida_sintatica.txt";
char file_entrada[21] = "sintatico_entrada.txt";



struct Tabela{
	char * cadeia; // exemplo id,2
	char * token; // id
	char * categoria; //var, for, etc
	char * tipo; //logico inteiro
	char * valor; //0-9, verdadeiro, falso	
};

int qtdeSimbolosTabela = 15;
int qtdeSimbolosTabelaInteiro=0;
int qtdeSimbolosTabelaLogico=0;
struct Tabela *tabelaInteiro[15];
struct Tabela *tabelaLogico[15];



struct arrayString{
	char *string;
};


void insereTabelaVirgulas(int posicoesAteEncontrar, struct arrayString *buffer);
void insereTabelaSimbolo(char *cadeia, char *token, char *categoria, char *tipo, char *valor);

int isReservada(char *buffer, char *charWanted);
int isTipoReservada(char *buffer);
int isBooleana(char *string);
int isVariable(char *string);
int isAtribuicao(char *string);
int isNumber(char *string);
int isPontuacao(char *string, char *pontuacao);
int isOperador(char *string);
int sintaxeFuncaoIO(struct arrayString *buffer);
int sintaxeFor(struct arrayString *buffer);
void startParser(struct arrayString *buffer);
void verificaSecaoDeclaracao(struct arrayString *buffer);
int verificaSecaoComandos(struct arrayString *buffer, int mode);
int verificaExpLogica(struct arrayString *buffer);
int sintaxeSe(struct arrayString *buffer);
int isRelOp(char *string);
int verifyType(char *buffer, char *typeExpected);

/*funcoes de auxilio */
char * returnSecondWord(char *string);
void stxRemoveCharsFromFile(struct arrayString buffer);
void removeChar(char *str, char garbage);
void writeTokenFile(char *string,int erro, char *stringMotivo);

int main(){


	int c;
	FILE *fp;
	
	fp = fopen(file_entrada,"r");
	while( (c = fgetc(fp))!= EOF){
		if(c == '\n')	
			lines++;
	} 	
	fclose(fp);
	
	//alocando array de acordo com a qtde de linhas obtidas do arquivo
	struct arrayString *buffer;
	buffer = (struct arrayString *) calloc(lines,sizeof(struct arrayString)) ;
	int line=0;
	for(; line<lines;line++)
		buffer[line].string = (char*) calloc(70,sizeof(char));
	

	
	fp = fopen(file_entrada,"r");
	int i=0;
	//le a linha e coloca no buffer
	char row[255];
	while( fgets( row, sizeof( row ), fp )) {
		strcpy(buffer[i++].string, row);
		memset(row,0, 255 );
	}	
	fclose(fp);

	
	line=0;
	for(; line<lines; line++)
		stxRemoveCharsFromFile(buffer[line]); //remove < > \n do arquivo para facilitar o processo
	
	int qtdTabela=0;
	for(; qtdTabela<qtdeSimbolosTabela; qtdTabela++){
		tabelaInteiro[qtdTabela] = (struct Tabela *) calloc(15, sizeof(struct Tabela));
		tabelaLogico[qtdTabela] = (struct Tabela *) calloc(15, sizeof(struct Tabela));	
	}
	

	startParser(buffer);
	
	if(errosSintaticos==0)
		writeTokenFile("Analise sintatica sem erros",3,"NULL");
	if(errosSemanticos==0)
		writeTokenFile("Analise semantica sem erros",3,"NULL");
	
	for(qtdTabela=0; qtdTabela<qtdeSimbolosTabela; qtdTabela++){
		free(tabelaInteiro[qtdTabela]);
		free(tabelaLogico[qtdTabela]);
	}		
	
	for(line=0; line<lines;line++)
		free(buffer[line].string);
	free(buffer);
		
	return 0;
}

/*
posicoesAteEncontrar -> posicao do buffer ate encontrar um logico/inteiro, usado para encontrar em impares o tipo (ultimo valor) e os IDs
*/
void insereTabelaVirgulas(int posicoesAteEncontrar, struct arrayString *buffer){
	int i=1;
	char *tipo = buffer[position-i].string;
	i++;
	while(i<=posicoesAteEncontrar){
		if( (i % 2) == 1){
			insereTabelaSimbolo(buffer[position-i].string, "id" , "var", tipo, "-");
		}
		i++;
	}
	
}
void insereTabelaSimbolo(char *cadeia, char *token, char *categoria, char *tipo, char *valor){
	if(strcmp(tipo,"inteiro")==0){
		
		tabelaInteiro[qtdeSimbolosTabelaInteiro]->cadeia = (char*) calloc(20, sizeof(char));
		tabelaInteiro[qtdeSimbolosTabelaInteiro]->token = (char*) calloc(20, sizeof(char));
		tabelaInteiro[qtdeSimbolosTabelaInteiro]->categoria = (char*) calloc(20, sizeof(char));
		tabelaInteiro[qtdeSimbolosTabelaInteiro]->tipo = (char*) calloc(20, sizeof(char));
		tabelaInteiro[qtdeSimbolosTabelaInteiro]->valor = (char*) calloc(20, sizeof(char));
		tabelaInteiro[qtdeSimbolosTabelaInteiro]->cadeia = cadeia;
		tabelaInteiro[qtdeSimbolosTabelaInteiro]->token = token;
		tabelaInteiro[qtdeSimbolosTabelaInteiro]->categoria = categoria;
		tabelaInteiro[qtdeSimbolosTabelaInteiro]->tipo = tipo;
		tabelaInteiro[qtdeSimbolosTabelaInteiro]->valor = valor;
		qtdeSimbolosTabelaInteiro++;
		
	}else{
		
		tabelaLogico[qtdeSimbolosTabelaLogico]->cadeia = (char*) calloc(20, sizeof(char));
		tabelaLogico[qtdeSimbolosTabelaLogico]->token = (char*) calloc(20, sizeof(char));
		tabelaLogico[qtdeSimbolosTabelaLogico]->categoria = (char*) calloc(20, sizeof(char));
		tabelaLogico[qtdeSimbolosTabelaLogico]->tipo = (char*) calloc(20, sizeof(char));
		tabelaLogico[qtdeSimbolosTabelaLogico]->valor = (char*) calloc(20, sizeof(char));
		tabelaLogico[qtdeSimbolosTabelaLogico]->cadeia = cadeia;
		tabelaLogico[qtdeSimbolosTabelaLogico]->token = token;
		tabelaLogico[qtdeSimbolosTabelaLogico]->categoria = categoria;
		tabelaLogico[qtdeSimbolosTabelaLogico]->tipo = tipo;
		tabelaLogico[qtdeSimbolosTabelaLogico]->valor = valor;
		qtdeSimbolosTabelaLogico++;
		
	}
}


void updateValor(char *id, char *valor){
	int i=0;
	for(;i < qtdeSimbolosTabelaInteiro; i++){
		if(strcmp(tabelaInteiro[i]->cadeia,id) == 0){
			tabelaInteiro[i]->valor = valor;
			return;
		}
	}
	i=0;
	for(;i < qtdeSimbolosTabelaLogico; i++){
		if(strcmp(tabelaLogico[i]->cadeia,id) == 0){
			tabelaLogico[i]->valor = valor;
			return;
		}
	}
}


void startParser(struct arrayString *buffer){
	
	//starts with algoritmo
	if( isReservada(buffer[position].string, "algoritmo")){
		notAlgoritmo:
		if(isVariable(buffer[position].string) ){
			verifyAgain:
			verificaSecaoDeclaracao(buffer);
			verificaSecaoComandos(buffer, 0);
		}else{
			writeTokenFile(buffer[position].string,1,"MOTIVO: falta nome do algoritmo");
			errosSintaticos++;
			goto verifyAgain;
		}
		
	}else{
		//writeTokenFile(buffer[position].string,1,"MOTIVO: falta algoritmo");

		errosSintaticos++;
		goto notAlgoritmo;
	}
	
}

void verificaSecaoDeclaracao(struct arrayString *buffer){
	int startWithVar =0;
	
	loop:
	if( strcmp( buffer[position].string , "inicio") != 0){
		//verifica se inicia com var
		if(isReservada(buffer[position].string,"var")){
			startWithVar =1;
			//verifica se possui a estrutura correta ": inteiro"
			if(isVariable(buffer[position].string )){
				char *tempID = buffer[position-1].string;
				
				notVariable:
				
				if(isPontuacao( buffer[position].string, "DOISPT")){
					
					missedDOISPT:
					if (! (isTipoReservada( buffer[position].string ))) {
						writeTokenFile(buffer[position].string,1,"MOTIVO: declaracao de Tipo nao reservada");
						errosSintaticos++;			
					}else{
						insereTabelaSimbolo(tempID, "id","var", buffer[position-1].string, "-");
					}
					
					
				}else{
					writeTokenFile(buffer[position].string,1,"MOTIVO: falta \":\"");
					errosSintaticos++;
					goto missedDOISPT;
				}	
			
			}else{
				writeTokenFile(buffer[position].string,1,"MOTIVO: palavra nao eh variavel");
				errosSintaticos++;
				goto notVariable;
			} 
			
			
		}
		
		if( strcmp( buffer[position].string , "inicio") != 0 && startWithVar ) {
			int isVir,isPontuacaoVerified;
		
		//notStartedWithVar é chamado quando a secao de declaracao nao é inicializado primeiro com VAR
		notStartedWithVar:	
		
			isVir =0; isPontuacaoVerified=0;
			//verifica i,asdsa,dasdsa,dasdas : inteiro
			int posicoesAteEncontrar=0;
			while(1){
				if(isVariable(buffer[position].string )){ //verifica se eh variavel
					posicoesAteEncontrar++;
					if(isPontuacao(buffer[position].string, "VIR")){ //virgula
						posicoesAteEncontrar++;
						isVir=1;
					}else if( isVir && isPontuacao( buffer[position].string, "DOISPT")){ //se n encontrou virgula, verifica se eh :
						posicoesAteEncontrar++;
						isPontuacaoVerified=1;
						//se apos o : nao for logico ou inteiro, erro lexico
						if(isTipoReservada( buffer[position].string ) ){ //se encontrou : , verifica se eh logico/inteiro
							posicoesAteEncontrar++;
							insereTabelaVirgulas(posicoesAteEncontrar, buffer);
							break; //para o while
							
						}else{ //se n encontrou, erro sintatico
							errosSintaticos++; 
						}
						
					}else if( !isVir && isPontuacao( buffer[position].string, "DOISPT")){ //se n encontrou virgula, verifica se eh :
						posicoesAteEncontrar++;
						isPontuacaoVerified=1;
						//se apos o : nao for logico ou inteiro, erro lexico
						if(isTipoReservada( buffer[position].string ) ){ //se encontrou : , verifica se eh logico/inteiro
							posicoesAteEncontrar++;
							insereTabelaVirgulas(posicoesAteEncontrar, buffer);
							break; //para o while
							
						}else{ //se n encontrou, erro sintatico
							errosSintaticos++; 
						}
						
					}
					else if(strcmp(buffer[position].string,"cmdatribuicao,-") == 0){
						//writeTokenFile(buffer[position].string,1,"MOTIVO: falta inicio");
						//errosSintaticos++;
						position--;
						return;
					}
					else if(!isVir){ //se n encontrou vir e :, erro sintatico
						writeTokenFile(buffer[position].string,1,"MOTIVO: falta virgula ou dois pontos");
						errosSintaticos++;
					}
						
				}else if(isTipoReservada( buffer[position].string ) ){ //se encontrou erro sintatico, entra nessa condicao
					break;
				}
			}
				
			if(!startWithVar){ //se n iniciou com var, verifica se n eh inicio && volta pro while
				if( strcmp( buffer[position].string , "inicio") ==0)
					goto loop;
				goto notStartedWithVar;
			}else{ //se iniciou com var, seta para valor 0 para n acusar de erro sintatico
				if( strcmp( buffer[position].string , "inicio") ==0)
					goto loop;
				startWithVar = 0;
				goto notStartedWithVar;
			}
			
			//goto loop;
		}
		//se nao for inicializado primeiro ocm VAR, chama a variavle de goto notStartedWithVar para verificar o restante da declaracao
		if(!startWithVar)
			goto notStartedWithVar;
		
		
		
	}
}

int verificaSecaoComandos(struct arrayString *buffer, int mode){
	if(mode==0){ //inicio fimalgoritmo
		if(isReservada(buffer[position].string,"inicio")){
			notInicio:
			if(verificaSecaoComandos(buffer,1)){
				
				if(isReservada(buffer[position].string,"fimalgoritmo")){
					return 1;
				}else{
					writeTokenFile(buffer[position].string,1,"MOTIVO: falta algoritmo");
					errosSintaticos++;
					return 1;
				}
			}
		}else{
			writeTokenFile(buffer[position].string,1,"MOTIVO: falta palavra inicio");
			errosSintaticos++;
			goto notInicio;
		}
				
				
	}else if (mode ==1){
		loop:
		if(lines == position){
			position--;
			return 1;
		}
		if( strcmp( buffer[position].string, "se" ) == 0){
			sintaxeSe(buffer);
		}else if( strcmp( buffer[position].string, "para" ) == 0 ){
			sintaxeFor(buffer);
		}else if( strcmp( buffer[position].string, "enquanto" ) == 0 ){
			sintaxeEnquanto(buffer);
		}else if( (strcmp( returnSecondWord(buffer[position].string), "escreva" ) == 0)
				||(strcmp( returnSecondWord(buffer[position].string), "escreval" ) == 0)
				||(strcmp( returnSecondWord(buffer[position].string), "exp" ) == 0)
				||(strcmp( returnSecondWord(buffer[position].string), "mod" ) == 0)
				||(strcmp( returnSecondWord(buffer[position].string), "leia" ) == 0) ){
					sintaxeFuncaoIO(buffer);
		
		}else if(isVariable(buffer[position].string)){
			char *temp = buffer[position-1].string;
			if(isAtribuicao(buffer[position].string)){
				
				if(verifyType(temp,"inteiro")){
				
					if(isBooleana(buffer[position].string)){
						errosSemanticos++;
						writeTokenFile(buffer[position-1].string,1,"MOTIVO: erro semantico");
						writeTokenFile(temp,2,"MOTIVO: variavel do tipo inteiro, mas declarada como Booleana");
						goto foundBool;
					}
					
					if(isNumber(buffer[position].string )  ||  (verifyType(buffer[position].string,"inteiro") && isVariable(buffer[position].string) ))   {
						if(verifyType(buffer[position-1].string,"logico")){
							errosSemanticos++;
							writeTokenFile(buffer[position-1].string,1,"MOTIVO: erro semantico");
							writeTokenFile(temp,2,"MOTIVO: variavel eh do tipo logico");
						}
						foundBool:
						if(isOperador(buffer[position].string)){
							if( isNumber(buffer[position].string) || (verifyType(buffer[position].string, "inteiro" )  && isVariable(buffer[position].string) ) ){
								if(verifyType(buffer[position-1].string,"logico")){
									errosSemanticos++;
									writeTokenFile(buffer[position-1].string,1,"MOTIVO: erro semantico");
									writeTokenFile(temp,2,"MOTIVO: variavel eh do tipo logico");
								}
								//updateValor()
								goto loop;
							}
						}else if(isPontuacao(buffer[position].string,"PTVIR")){
							goto loop;
						}else
							goto loop;
					}else{
						writeTokenFile(buffer[position].string,1,"MOTIVO: falta Numero ou Variavel");
						errosSintaticos++;
					}
				
				}else if(verifyType(temp,"logico")){
					
					if(  isNumber(buffer[position].string )){
						errosSemanticos++;
						writeTokenFile(buffer[position-1].string,1,"MOTIVO: erro semantico");
						writeTokenFile(temp,2,"MOTIVO: variavel eh do tipo logico");
						goto achouNumero;
					}
					if( isVariable(buffer[position].string) )   {
						achouNumero:
						if(isOperador(buffer[position].string)){
							errosSemanticos++;
							writeTokenFile(buffer[position-1].string,1,"MOTIVO: erro semantico");
							writeTokenFile(temp,2,"MOTIVO: variavel eh do tipo logico");					
							
							if( isNumber(buffer[position].string) || (verifyType(buffer[position].string, "inteiro" )  && isVariable(buffer[position].string) ) ){
								//updateValor()
								goto loop;
							}
						}else if(isPontuacao(buffer[position].string,"PTVIR")){
							goto loop;
						}else
							goto loop;
							
					}else if(isBooleana(buffer[position].string)){
						
						if(isOperador(buffer[position].string)){
							errosSemanticos++;
							writeTokenFile(buffer[position-1].string,1,"MOTIVO: erro semantico");
							writeTokenFile(temp,2,"MOTIVO: variavel eh do tipo logico");					
							
							if( isNumber(buffer[position].string) || (verifyType(buffer[position].string, "inteiro" )  && isVariable(buffer[position].string) ) ){
								errosSemanticos++;
								writeTokenFile(buffer[position-1].string,1,"MOTIVO: erro semantico");
								writeTokenFile(temp,2,"MOTIVO: variavel eh do tipo logico");
								goto loop;
							}
						}else if(isPontuacao(buffer[position].string,"PTVIR")){
							goto loop;
						}else
							goto loop;
					}else{
						writeTokenFile(buffer[position].string,1,"MOTIVO: falta Numero ou Variavel");
						errosSintaticos++;
					}
				
				}else{
					errosSemanticos++;
					writeTokenFile(buffer[position].string,1,"MOTIVO: erro semantico");
					writeTokenFile(temp,2,"MOTIVO: variavel nao instanciada");
					
					if(isNumber(buffer[position].string )  ||  (isVariable(buffer[position].string) ))   {
						if(isOperador(buffer[position].string)){
							if( isNumber(buffer[position].string) || (isVariable(buffer[position].string) ) ){
								//updateValor()
								goto loop;
							}
						}else if(isPontuacao(buffer[position].string,"PTVIR")){
							goto loop;
						}else
							goto loop;
					}else{
						writeTokenFile(buffer[position].string,1,"MOTIVO: falta Numero ou Variavel");
						errosSintaticos++;
					}
				}
					
			}
			
		}else if((strcmp( buffer[position].string, "fimse" ) == 0)
				||(strcmp( buffer[position].string, "fimenquanto" ) == 0)
				||(strcmp( buffer[position].string, "fimpara" ) == 0)
				||(strcmp( buffer[position].string, "fimalgoritmo" ) == 0)){
					return 1;
		}else{
			errosSintaticos++;
			writeTokenFile(buffer[position].string,1,"MOTIVO: caracter desconhecido");
			position++;
		}
		
		
	
		goto loop;
	}
	
	
	
	return 0;
}


int verifyType(char *buffer, char *typeExpected){
	if(strcmp(typeExpected,"inteiro") == 0){
		int i=0;
		for(; i < qtdeSimbolosTabelaInteiro; i++){
			if(strcmp(tabelaInteiro[i]->cadeia, buffer) == 0){
				if(strcmp(tabelaInteiro[i]->tipo, "inteiro") == 0){
					return 1;
				}
			}
		}
	}else if(strcmp(typeExpected,"logico")==0 ){
		int i=0;
		for(; i < qtdeSimbolosTabelaLogico; i++){
			if(strcmp(tabelaLogico[i]->cadeia, buffer) == 0){
				if(strcmp(tabelaLogico[i]->tipo, "logico") == 0){
					return 1;
				}
			}
		}		
	}


	return 0;
}

int verificaExpLogica(struct arrayString *buffer){
	int boolean=0;
	loop:
	if(isNumber(buffer[position].string) || (isVariable(buffer[position].string)  ) ){
		if(!isNumber(buffer[position-1].string)){
			if(!verifyType(buffer[position-1].string,"inteiro")){
				errosSemanticos++;
				writeTokenFile(buffer[position-1].string,1,"MOTIVO: erro semantico");
				writeTokenFile(buffer[position-1].string,2,"MOTIVO: variavel eh do tipo logico");
			}
		}else{
			position--;
		}
		boolean = 1;
		
		if(isRelOp(buffer[position].string)){
			boolean = 0;
			goto loop;
		}else if(strcmp(buffer[position].string,"entao") == 0){
			return boolean;
		}
		else{
			writeTokenFile(buffer[position].string,1,"MOTIVO: falta variavel em ExpLogica");
			errosSintaticos++;
			goto loop;
		}
	}
	if(!boolean){
		writeTokenFile(buffer[position].string,1,"MOTIVO: falta variavel em ExpLogica");
		errosSintaticos++;
	}
	return boolean;
}

int isReservada(char *string, char *charWanted){
	int boolean=0;
	
	if(strcmp( string, "algoritmo" ) == 0) boolean = 1;
	else if(strcmp( string, "ate" ) == 0) boolean = 1;
	else if(strcmp( string, "de" ) == 0) boolean = 1;
	else if(strcmp( string, "enquanto" ) == 0) boolean = 1;
	else if(strcmp( string, "entao" ) == 0) boolean = 1;
	else if(strcmp( string, "faca" ) == 0) boolean = 1;
	else if(strcmp( string, "fimpara" ) == 0) boolean = 1;
	else if(strcmp( string, "fimalgoritmo" ) == 0) boolean = 1;
	else if(strcmp( string, "fimse" ) == 0) boolean = 1;
	else if(strcmp( string, "fimenquanto" ) == 0) boolean = 1;
	else if(strcmp( string, "inicio" ) == 0) boolean = 1;
	else if(strcmp( string, "var" ) == 0) boolean = 1;
	else if(strcmp( string, "para" ) == 0) boolean = 1;
	else if(strcmp( string, "passo" ) == 0) boolean = 1;
	else if(strcmp( string, "se" ) == 0) boolean = 1;
	else if(strcmp( string, "senao" ) == 0) boolean = 1;
	
	if(boolean && (strcmp(string,charWanted) ==0)){
		position++;
		//writeTokenFile(string,0);
	}
	else if(boolean){
		char *stringMotivo = (char*) calloc(60,sizeof(char));
		strcpy(stringMotivo,"MOTIVO: palavra reservada requerida:  ");
		strcat(stringMotivo,charWanted);
		writeTokenFile(string,1,stringMotivo);
		free(stringMotivo);
		errosSintaticos++;
	}else{
		char *stringMotivo = (char*) calloc(60,sizeof(char));
		strcpy(stringMotivo,"MOTIVO: palavra reservada nao encontrada -> ");
		strcat(stringMotivo,charWanted);
		writeTokenFile(string,1,stringMotivo);
		free(stringMotivo);
		errosSintaticos++;
	}
	
	return boolean;
}

int isBooleana(char *string){
	int boolean=0;
	
	if(strcmp( string, "VERDADEIRO" ) == 0) boolean = 1;
	else if(strcmp( string, "FALSO" ) == 0) boolean = 1;

	if(boolean){
		position++;
		//writeTokenFile(string,0);
	} 
	
	return boolean;
}

int isTipoReservada(char *string){
	int boolean=0;
	
	if(strcmp( string, "inteiro" ) == 0) boolean = 1;
	else if(strcmp( string, "logico" ) == 0) boolean = 1;

	if(boolean){
		position++;
		//writeTokenFile(string,0);
	}else{
		errosSintaticos++;
		writeTokenFile(string,1,"MOTIVO: falta declaracao de tipo Inteiro ou Logico");
	} 
	
	return boolean;
}

//verifica se a string possui ID ou STRING
int isVariable(char *string){
	int boolean = 0;
	
	int length = strlen(string); int l=0;
	char *tmp = (char*) calloc(length, sizeof(char));
	for(; l<length; l++){
		tmp[l] = string[l];
		if( (strcmp(tmp,"id") == 0) || (strcmp(tmp,"string") == 0) )  {
			free(tmp);
			boolean = 1;
			break;
		}
	}
	if(boolean){
		position++;
	}
	return boolean;
}

int isAtribuicao(char *string){
	int boolean = 0;
	
	int length = strlen(string); int l=0;
	char *tmp = (char*) calloc(length, sizeof(char));
	for(; l<length; l++){
		tmp[l] = string[l];
		if( (strcmp(tmp,"cmdatribuicao") == 0))  {
			free(tmp);
			boolean = 1;
			break;
		}
	}
	if(boolean){
		position++;
	}else{
		boolean=1;
		errosSintaticos++;
		writeTokenFile(string,1,"MOTIVO: falta <-");
	}
	return boolean;
}

int isRelOp(char *string){
	int boolean=0;
	int length = strlen(string); int l=0, isRelop=0,i=0;
	char *tmp = (char*) calloc(length, sizeof(char));
	for(; l<length; l++){
		if(!isRelop)
			tmp[l] = string[l];
		else if(isRelop)
			tmp[i++] = string[l];
		if(!isRelop && (strcmp(tmp,"relop") == 0))  {
			memset(tmp,0,length);
			isRelop=1;
			l++;
		}else if(isRelop && ( (strcmp(tmp, "GT") == 0) 
							||(strcmp(tmp, "GE") == 0)
							||(strcmp(tmp, "LT") == 0)
							||(strcmp(tmp, "LE") == 0)
							||(strcmp(tmp, "EQ") == 0)
							||(strcmp(tmp, "NE") == 0)
							||(strcmp(tmp, "E") == 0)
							||(strcmp(tmp, "OU") == 0) ) ){
								boolean = 1;
								break;
			
		}
	}
	if(boolean){
		position++;
		
	}
	return boolean;
}

int isNumber(char *string){
	int boolean = 0;
	
	int length = strlen(string); int l=0;
	char *tmp = (char*) calloc(length, sizeof(char));
	for(; l<length; l++){
		tmp[l] = string[l];
		if( (strcmp(tmp,"numero") == 0))  {
			free(tmp);
			boolean = 1;
			break;
		}
	}
	if(boolean){
		position++;
	}
	return boolean;
}

int isPontuacao(char *string, char *pontuacao){
	int boolean=0;
	
	int length = strlen(string); int l=0, i=0;
	char *tmp = (char*) calloc(length, sizeof(char));
	for(; l<length; l++){
		tmp[i++] = string[l];
		if( strcmp(tmp, "pontuacao") == 0 ){
			l++; i=0;
			memset(tmp, 0, length);
			continue;
		}if( strcmp(tmp, pontuacao) == 0 ){
			boolean=1;
			free(tmp);
			break;
		}
	}
	if(boolean){
		position++;
	}
	
	return boolean;
	
}

int isOperador(char *string){
	int boolean=0;
	
	int length = strlen(string); int l=0, i=0;
	char *tmp = (char*) calloc(length, sizeof(char));
	for(; l<length; l++){
		tmp[i++] = string[l];
		if( strcmp(tmp, "operador") == 0 ){
			l++; i=0;
			memset(tmp, 0, length);
			continue;
		}if((strcmp(tmp, "SOMA") == 0) 
			||(strcmp(tmp, "SUBT") == 0)
			||(strcmp(tmp, "MULT") == 0)
			||(strcmp(tmp, "DIV") == 0)){
			boolean=1;
			free(tmp);
			break;
		}
	}
	if(boolean){
		position++;
		
	}
	
	return boolean;
}

int sintaxeFuncaoIO(struct arrayString *buffer){
	int boolean=0;
	char *strFuncao = returnSecondWord(buffer[position].string);
	
	if( (strcmp(strFuncao, "escreva") == 0) || (strcmp(strFuncao, "escreval") == 0) || (strcmp(strFuncao, "leia") == 0)
		|| (strcmp(strFuncao, "exp") == 0)  || (strcmp(strFuncao, "mod") == 0) ) {
		//writeTokenFile(buffer[position].string,0);
		
		position++;
		
		verifyAgain:
		if(isPontuacao(buffer[position].string, "PA" )){
			int isVir =1;
			//verifica i,asdsa,dasdsa,dasdas,"asdsda"
			while(isVir){
				
				verifyAgainNumber:
				
				if(isPontuacao(buffer[position].string, "PA" )){
					position--;
					writeTokenFile(buffer[position].string,1,"MOTIVO: \"(\" repetido");
					position++;
					errosSintaticos++;
					goto verifyAgainNumber;
				}
				
				
				if(isVariable(buffer[position].string )){
					if( !(isPontuacao( buffer[position].string, "VIR") ))
						isVir=0;
				}
				if(isVir && isPontuacao(buffer[position].string, "PF" )){
					writeTokenFile(buffer[position].string,1,"MOTIVO: Falta \")\"");
					errosSintaticos++;
				}else if(isPontuacao(buffer[position].string, "PF" )){
					boolean=1;
					break;	

				}else{
					goto verifyAgainNumber;
				}
			}
		}else{
			writeTokenFile(buffer[position].string,1,"MOTIVO: Falta \"(\"");
			errosSintaticos++;
			position++;
			goto verifyAgain;
		}
		
	}
	
	return boolean;
}


int sintaxeSe(struct arrayString *buffer){
	int boolean=0;
	if(isReservada(buffer[position].string,"se")){
		
		if(verificaExpLogica(buffer)){
			
			if(isReservada(buffer[position].string,"entao")){
				notSe:
				if(verificaSecaoComandos(buffer,1))	{
				
					if(isReservada(buffer[position].string,"fimse")){
						boolean =1;
					}else{
						writeTokenFile(buffer[position].string,1,"MOTIVO: Falta fimse");
						errosSintaticos++;
						boolean=1;
					}
					
				}
			}else{
				writeTokenFile(buffer[position].string,1,"MOTIVO: Falta entao no SE");
				errosSintaticos++;
				goto notSe;
			}
		}		
		
	}
	 
}

int sintaxeFor(struct arrayString *buffer){
	int lineFor=0;
	if(isReservada(buffer[position].string,"para")){
		
		if(isVariable(buffer[position].string)){
			notVar:
			
			if(!verifyType(buffer[position-1].string, "inteiro") ){
				writeTokenFile(buffer[position-1].string,1,"MOTIVO: erro semantico");
				writeTokenFile(buffer[position-1].string,2,"MOTIVO: variavel precisa ser do tipo inteiro");
			}	
				
			if(isReservada(buffer[position].string,"de")){
				notDe:
				if(isNumber(buffer[position].string) ){
					notNumber:
					if(isReservada(buffer[position].string,"ate")){
						notAte:
						if(isNumber(buffer[position].string)){
							lineFor=1;
						}else{
							writeTokenFile(buffer[position].string,1,"MOTIVO: Falta numero no Para");
							errosSintaticos++;
							lineFor=1;
						}
						
					}else{
						writeTokenFile(buffer[position].string,1,"MOTIVO: Falta palavra ate no Para");
						errosSintaticos++;
						goto notAte;
					} 
				}else{
					writeTokenFile(buffer[position].string,1,"MOTIVO: Falta numero no Para");
					errosSintaticos++;
					goto notNumber;
				} 
			}else{
				writeTokenFile(buffer[position].string,1,"MOTIVO: Falta de no Para");
				errosSintaticos++;
				goto notDe;
			}
		}else{
			writeTokenFile(buffer[position].string,1,"MOTIVO: Falta variavel no Para");
			errosSintaticos++;
			goto notVar;
		}
	}

	if(lineFor){
		if(isReservada(buffer[position].string, "passo") && isNumber(buffer[position].string)){
			if(isReservada(buffer[position].string,"faca"))
				verificaSecaoComandos(buffer,1);
				return isReservada(buffer[position].string,"fimpara");
		}else if(isReservada(buffer[position].string,"faca")){
				verificaSecaoComandos(buffer,1);
				return isReservada(buffer[position].string,"fimpara");
		}
	}
			
							
}



int sintaxeEnquanto(struct arrayString *buffer){
	if(isReservada(buffer[position].string,"enquanto")){
		
		if(verificaExpLogica(buffer)){
			expErro:
			if(isReservada(buffer[position].string,"faca")){
				notFaca:
				if(verificaSecaoComandos(buffer,1)){
					if(isReservada(buffer[position].string,"fimenquanto")){
						return 1;
					}else{
						writeTokenFile(buffer[position].string,1,"MOTIVO: Falta fimenquanto no Enquanto");
						errosSintaticos++;
						return 1;
					}
				}
			}else{
				writeTokenFile(buffer[position].string,1,"MOTIVO: Falta faca no Enquanto");
				errosSintaticos++;
				goto notFaca;
			}
		}else{
			goto expErro;
		}
	}
	
	return 0;
}



/* funcoes de auxilio */
char * returnSecondWord(char *string){	
	int length = strlen(string); int l=0,i=0, finded=0;
	char *tmp = (char*) calloc(length, sizeof(char));
	for(; l<length; l++){
		if(!finded)
			tmp[i] = string[l];
		if( !finded && (strcmp(tmp,",") == 0)) {
			memset(tmp,0,length);
			finded=1;
		}else if(finded){
			tmp[i++] = string[l];
		}else
			memset(tmp,0,length);
		
	}
	
	return tmp;
}

void stxRemoveCharsFromFile(struct arrayString buffer){
	removeChar(buffer.string, '<');
	removeChar(buffer.string, '>');
	removeChar(buffer.string, '\n');
}

void removeChar(char *str, char garbage) {
    char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != garbage) dst++;
    }
    *dst = '\0';
}

int firstTime=0;
void writeTokenFile(char *string, int erro, char *stringMotivo){
	FILE *file;

	char *output = (char*)calloc(strlen(string)+strlen(stringMotivo)+60, sizeof(char) );
	if(erro == 1){
		strcpy(output,"ERRO SINTATICO: ");
		strcat(output, "<");
		strcat(output,string);
		strcat(output, ">");
		
		char *linha = (char*) calloc(2,sizeof(char));
		sprintf(linha,"%d",position+1);
		strcat(output," LINHA: ");
		strcat(output,linha );
		//free(linha);
		strcat(output,"  ");
		strcat(output,stringMotivo);
		strcat(output, "\n");
	}else if(erro==0){
		strcat(output, "<");
		strcat(output,string);
		strcat(output, ">");
		strcat(output, "\n");
	}else if(erro==2){
		strcpy(output,"ERRO SEMANTICO: <");
		strcat(output,string);
		strcat(output,"> ");
		strcat(output,stringMotivo);
		strcat(output,"\n");
	}else if(erro==3){ //print messsage only
		strcpy(output,string);
		strcat(output,"\n");
	}
	printf("%s",output);
	if(!firstTime){
		file = fopen(file_saida,"w"); //"limpa" o arquivo se existir 
		firstTime++;	
	}else{
		file = fopen(file_saida,"a"); //append
	}
	fprintf(file,output);
	free(output);
	fclose(file);
}

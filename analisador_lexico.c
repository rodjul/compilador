#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char file_saida[21] = "analisador_lexico.txt";
char file_entrada[21] = "entrada.txt";

//falta mod, senao, logico, validar expressões?

struct TOKEN{
	char *token;
	char *attribute;
};

struct VARS{
	char string[50];
};


void writeTokenFile(struct TOKEN *token){
	FILE *file;

	char *output = (char*)calloc(strlen(token->attribute) + strlen(token->token) + 5, sizeof(char) ); //< + , + > + \n
	if(strcmp(token->attribute,"PRIV") == 0){
		strcpy(output,"<");
		strcat(output,token->token);
		strcat(output,">");
		strcat(output,"\n");
	}else{
		strcpy(output,"<");
		strcat(output,token->token);
		strcat(output,",");
		strcat(output,token->attribute);
		strcat(output,">");
		strcat(output,"\n");
	}
	printf("%s",output);
	file = fopen(file_saida,"a"); //a = se n existe, cria o arquivo e append os strings
	fprintf(file,output);
	free(output);
	fclose(file);
}

int isPontuacao(char c){
	if(c == ',' || c==':' || c=='(' || c==')' || c==';' || c=='<' || c=='>' || c=='-' || c=='+' || c=='/'||c=='*')
		return 1;
	return 0;
}

void limpaBuffer(char *buffer, int len){
	memset(buffer,0,len);
}


//array de variaveis
int qtde_vars = 20;
int id = 0;

int main(){
	
	int c;
	FILE *fp;
	fp = fopen(file_entrada,"r");

	struct TOKEN *token;
	token = (struct TOKEN *) calloc(1, sizeof(struct TOKEN));
	token->token = (char *) calloc(20,sizeof(char));
	token->attribute = (char *) calloc(15,sizeof(char));

	struct VARS *vars[qtde_vars];
	int i;
	for(i=0; i<qtde_vars; i++)
		vars[i] = (struct VARS*) calloc(1, sizeof(struct VARS));
	
	long len = 150;
	char *buffer = (char*) calloc(len, sizeof(char));
	
	
	i=0;
	int index=0;
	while (1){
		//limpa o buffer
		limpaBuffer(buffer,len);
		
		//coloca em um buffer a linha atual da entrada
		while( (c = fgetc(fp)) != '\n' && c != EOF) 
			buffer[i++] = c;
		//se nao leu linha, verifica se ha linhas em brancos e le novamente
		if(i == 0){
			while((c = fgetc(fp)) == '\n');
			//se o caracter atual for valido, le o restante
			if(c>0){
				buffer[i++] = c;
				while( (c = fgetc(fp)) != '\n' && c != EOF) 
					buffer[i++] = c;
			}
		}
		
		//se a variavel i que incrementa ao colocar char no buffer for 0, entao nao ha mais char para ler
		if(i == 0 && index == 0)
			break;
		
		char *string_buffer;
		int len_string_buffer = 100;
		//variavel usado para armazenar em um buffer temporariamente a string ou id
		string_buffer = (char*) calloc(len_string_buffer, sizeof(char));
		int string_buffer_i=0;
		
		int state=0;
		int lexemaBegin=0; //variavel usado para ter controle do buffer ao manusear os indices de uma string
		while(1){
			switch(state){
				case 0:
					c = buffer[index];
					//verifica se o token nao eh NULL para poder imprimir a saida
					if(strcmp(token->attribute,"") != 0){
						writeTokenFile(token);
						token->attribute = "";
						token->token = "";
						limpaBuffer(string_buffer,len_string_buffer);
						c = buffer[++index]; //forward
						if(c == 32)
							while( (c = buffer[++index]) == ' ' ); //pula espaço
						if(c < 0 || c == '\0') //condicao de parada se nao encontrar mais caracteres validos
							break;
					}else if(c==32){ //se entrou nessa condicao, a linha possui indentacao
						while( (c = buffer[++index]) == ' ' ); //pula espaço
					}
						
					//lexemaBegin recebe o INDEX que ta sendo iniciado a verificacao da string
					lexemaBegin = index;
					
					if(c == 'a') state=2; //algoritmo
					else if(c == '0' | c == '1' | c == '2' | c == '3' | c == '4' | c == '5' | c == '6' | c == '7' | c == '8' | c == '9') state = 12;
					else if(c == 'd') state= 147; //de
					else if(c == 'e') state = 106; //E, entao, enquanto, exp, escreva
					else if(c == 'f'){
						c = buffer[++index];
						if(c=='a') state=83; //faca 
						else{ 
							c = buffer[--index];
							state=55; //fimpara fimse fimalgoritmo fimenquanto
						}
					}
					else if(c == 'l') state=94; //leia
					else if(c == 'i') state=32; //inicio
					else if(c == 'p') state=143; //para
					else if(c == 's') state=45; //se
					else if(c == 'v') state = 29; //var
					//relop	
					else if(c == '<') state=52;
					else if(c == '>') state=54;
					else if(c == '=') state=53;
					 //string
					else if(c == '"') state=11;
					//comentario OU div
					else if(c == '/'){
						c = buffer[++index];
						if(c == '/'){
							token->token = "COMENT";
							state = 71;
						}
						else{
							token->token = "operacao";
							token->attribute = "div";
							state = 71;
						}
					}
					//operadores
					else if(c == '+' || c == '-' || c == '*'){
						if(c=='+') state=68;
						else if(c=='-') state=69;
						else if(c=='*') state=70;
					}
					//pontuacao ( ) : ; ,
					else if(c == '(' || c ==')'){
						if(c==')') state=114;
						else state=115;
					}else if(c == ':') state=140;
					else if(c == ';') state=141;
					else if(c == ',') state=142;
					//se chegou aqui, é uma variavel
					else state=1;
					 
					break;
				
				//variavel
				case 1:
					//faz um forward na string ate encontrar um \n || EOF || vazio || pontuacao
					while( !(isPontuacao(c)) && c!=32 && (c = buffer[++index]) !=' ' && c!= '\n' && c!=EOF && c!='\0');

					//utiliza o lexemaBegin que eh o inicio da verificacao da string e armazena em um buffer
					while(lexemaBegin != index)
						string_buffer[string_buffer_i++] = buffer[lexemaBegin++];
					
					int var=0, num_id;
					//procura na variavel VARS se possui alguma variavel ja inserida
					for(; var<qtde_vars; var++){
						if(strcmp(vars[var]->string, "") == 0){
							strcpy(vars[var]->string, string_buffer);
							num_id = var +1;
							break;
						}else if( strcmp(vars[var]->string, string_buffer) == 0 ){
							num_id = var +1;
							break;
						}
					}
					
					token->token = "id";
					char *num;
					asprintf(&num,"%d",num_id); //convertendo int para char*, essa funcao faz auto-gerenciamento de memoria onde gerencia o alocamento que sera necessario apos a conversao
					token->attribute= num;
					
					string_buffer_i=0;
					state = 0;
					index--;
					break;
				
				//algoritmo 
				case 2:
					c = buffer[++index];
					if(c == 'l') state = 3;
					else if( c =='t') state = 80; //ate 79-81
					else state = 1; //string
					break;
				case 3:
					c = buffer[++index];
					if(c == 'g') state = 4;
					else state = 1; //variavel
					break;
				case 4:
					c = buffer[++index];
					if(c == 'o') state = 5;
					else state = 1; //variavel
					break;
				case 5:
					c = buffer[++index];
					if(c == 'r') state = 6;
					else state = 1; //variavel
					break;
				case 6:
					c = buffer[++index];
					if(c == 'i') state = 7;
					else state = 1; //variavel
					break;
				case 7:
					c = buffer[++index];
				 	if(c == 't') state = 8;
				 	else state = 1; //variavel
					break;
				case 8:
					c = buffer[++index];
					if(c == 'm') state = 9;
					else state = 1; //variavel
					break;
				case 9:
					c = buffer[++index];
					if(c == 'o') state = 10;
					else state = 1; //variavel
					break;
				case 10:
					c = buffer[++index];
					if(c != ' ' ) state = 1; //se tiver mais alguma letra, é variavel
					else{
						index--;//rollback
						token->attribute = "PRIV";
						token->token = "algoritmo";
						state = 0;
					}
					break;
				//ate
				case 80:
					c = buffer[++index];
					if(c == 'e') state=81;
					else state=1;
					break;
				case 81:
					c = buffer[++index];
					if(c == ' '){
						token->token = "ate";
						token->attribute = "PRIV";
						index--;
						state=0;
					}else state=1;
					break;
				
				//string "string"
				case 11:
					c = buffer[index]; //pega o " para armazenar no string_buffer
					string_buffer[string_buffer_i++] = c;
					c = buffer[++index];
					if( !(c == '"') )
						state=13; //DUVIDA E SE FOR "" ?
					break;
				
				case 13:
					string_buffer[string_buffer_i++] = c;
					//se nao for ", continua verificando [A-Za-z][0-9]
					while( !( (c = buffer[++index]) == '"') ) 
						string_buffer[string_buffer_i++] = c;
					
					if(c == '"')
						state=14;
					
					break;
					
				case 14:
					c = buffer[index];
					string_buffer[string_buffer_i++] = c;
					
					token->attribute = string_buffer;
					token->token = "string";
					
					string_buffer_i = 0; //reseta buffer do string_buffer
					state=0;
					break;
				
				//numeros
				case 12: ; //https://stackoverflow.com/questions/18496282/why-do-i-get-a-label-can-only-be-part-of-a-statement-and-a-declaration-is-not-a
					char *numero = (char*) calloc(10,sizeof(char)); 
					int n=0;
					numero[n++] = c;
					while( isdigit( (c = buffer[++index]) ) ){
						//if(c == '0' | c == '1' | c == '2' | c == '3' | c == '4' | c == '5' | c == '6' | c == '7' | c == '8' | c == '9' )
						numero[n++] = c;
					}
					token->token = "numero";
					token->attribute = numero;
					state = 0;
					index--;
					break;
					
				//var
				case 29:
					c = buffer[++index];
					if(c == 'a') state = 30;
					else state = 1;
					break;
				case 30:
					c = buffer[++index];
					if(c == 'r') state=31;
					else state = 1;
					break;
				case 31:
					c = buffer[++index];
					if(c == 32){ //espaço/vazio
						token->token = "var";
						token->attribute = "PRIV";
						index--;
						state=0;
					}else state = 1;
					break;
				//inicio
				case 32:
					c = buffer[++index];
					if(c == 'n') state=33;
					else state=1;
					break;
				case 33:
					c = buffer[++index];
					if(c=='i') state=34;
					else if(c == 't') state=17; //inteiro
					else state=1;
					break;
				case 34:
					c = buffer[++index];
					if(c=='c') state=35;
					else state=1;
					break;
				case 35:
					c = buffer[++index];
					if(c=='i') state=36;
					else state=1;
					break;
				case 36:
					c = buffer[++index];
					if(c=='o') state=37;
					else state=1;
					break;
				case 37:
					c = buffer[++index];
					if(c==32 || c == ' '){
						token->token = "inicio";
						token->attribute = "PRIV";
						index--;
						state=0;
					}else state=1;
					break;
				//inteiro
				case 17:
					c = buffer[++index];
					if(c=='e') state=18;
					else state=1;
					break;
				case 18:
					c = buffer[++index];
					if(c=='i') state=19;
					else state=1;
					break;
				case 19:
					c = buffer[++index];
					if(c=='r') state=20;
					else state=1;
					break;
				case 20:
					c = buffer[++index];
					if(c=='o') state=21;
					else state=1;
					break;
				case 21:
					c = buffer[++index];
					if(c==32 || c == ' '){
						token->token = "inteiro";
						token->attribute = "PRIV";
						index--;
						state=0;
					}else state=1;
					break;
				
				
				//fimpara faca fimse fimalgoritmo fimenquanto
				case 83://faca
					c = buffer[++index];
					if(c=='c') state=84;
					else state=1;
					break;
				case 84:
					c = buffer[++index];
					if(c=='a') state=85;
					else state=1;
					break;
				case 85:
					c = buffer[++index];
					if(c==32 || c==' '){
						token->token = "faca";
						token->attribute = "PRIV";
						index--;
						state=0;
					}else state=1;
					break;
				
				case 55://fim
					c = buffer[++index];
					if(c == 'i') state=56;
					else state=1;
					break;
				case 56:
					c = buffer[++index];
					if(c == 'm') state=57;
					else state=1;
					break;
				case 57:
					c = buffer[++index];
					if(c == 'p'){
						 state=41; //fimpara 
					}
					else if(c == 'a'){
						state=58;//fimalgoritmo
					}
					else if(c == 's') state=118;//fimse
					else if(c == 'e') state=123;//fimenquanto
					else state=1;
					break;
				case 41: //fimpara
					c = buffer[++index];
					if(c=='a') state=42;
					else state=1;
					break;
				case 42:
					c = buffer[++index];
					if(c=='r') state=43;
					else state=1;
					break;
				case 43:
					c = buffer[++index];
					if(c=='a') state=44;
					else state=1;
					break;
				case 44:
					c = buffer[++index];
					if(c ==32 || c==' ' || c=='\0'){
						token->token = "fimpara";
						token->attribute = "PRIV";
						index--;
						state=0;
					}else state=1;
					break;
				case 58: //fimalgoritmo
					c = buffer[++index];
					if(c=='l') state=59;
					else state=1;
					break;
				case 59:
					c = buffer[++index];
					if(c=='g') state=60;
					else state=1;
					break;
				case 60:
					c = buffer[++index];
					if(c=='o') state=61;
					else state=1;
					break;
				case 61:
					c = buffer[++index];
					if(c=='r') state=62;
					else state=1;
					break;
				case 62:
					c = buffer[++index];
					if(c=='i') state=63;
					else state=1;
					break;
				case 63:
					c = buffer[++index];
					if(c=='t') state=64;
					else state=1;
					break;
				case 64:
					c = buffer[++index];
					if(c=='m') state=65;
					else state=1;
					break;
				case 65:
					c = buffer[++index];
					if(c=='o') state=66;
					else state=1;
					break;
				case 66:
					c = buffer[++index];
					if(c==32 || c != ' ' || c=='\0'){
						token->token = "fimalgoritmo";
						token->attribute = "PRIV";
						index--;
						state=0;
					}else state=1;
					break;
				case 118: //fimse
					c = buffer[++index];
					if(c=='e') state=119;
					else state=1;
					break;
				case 119:
					c = buffer[++index];
					if(c != ' '){
						token->token = "fimse";
						token->attribute = "PRIV";
						index--;
						state=0;
					}else state=1;
					break;
				case 123: //fimenquanto
					c = buffer[++index];
					if(c=='n') state=124;
					else state=1;
					break;
				case 124:
					c = buffer[++index];
					if(c=='q') state=125;
					else state=1;
					break;
				case 125:
					c = buffer[++index];
					if(c=='u') state=126;
					else state=1;
					break;
				case 127:
					c = buffer[++index];
					if(c=='a') state=128;
					else state=1;
					break;
				case 128:
					c = buffer[++index];
					if(c=='n') state=129;
					else state=1;
					break;
				case 129:
					c = buffer[++index];
					if(c=='t') state=130;
					else state=1;
					break;
				case 130:
					c = buffer[++index];
					if(c=='o') state=131;
					else state=1;
					break;
				case 131:
					c = buffer[++index];
					if(c!=' '){
						token->token = "fimenquanto";
						token->attribute = "PRIV";
						index--;
						state=0;
					}else state=1;
					break;
				
				//para //passo
				case 143:
					c = buffer[++index];
					if(c=='a') state=144;
					else state=1;
					break;
				case 144:
					c = buffer[++index];
					if(c=='r') state=145;
					else if(c=='s') state=149;
					else state=1;
					break;
				case 145:
					c = buffer[++index];
					if(c=='a') state=146;
					else state=1;
					break;
				case 146:
					c = buffer[++index];
					if(c == 32 || c == ' '){
						token->token = "para";
						token->attribute = "PRIV";
						index--;
						state=0;
					}else state=1;
					break;
				case 149: //passo
					c = buffer[++index];
					if(c=='s') state=150;
					else state=1;
					break;
				case 150:
					c = buffer[++index];
					if(c=='o') state=151;
					else state=1;
					break;
				case 151:
					c = buffer[++index];
					if(c==32 || c==' '){
						token->token = "passo";
						token->attribute = "PRIV";
						index--;
						state=0;
					}else state=1;
					break;
				
				//de
				case 147:
					c = buffer[++index];
					if(c=='e') state=148;
					else state=1;
					break;
				case 148:
					c = buffer[++index];
					if(c==32 || c==' '){
						token->token = "de";
						token->attribute = "PRIV";
						index--;
						state=0;
					}else state=1;
					break;
				
				//se
				case 45:
					c = buffer[++index];
					if(c == 'e') state=46;
					else state=1;
					break;
				case 46:
					c = buffer[++index];
					if(c==32 || c==' '){
						token->token = "se";
						token->attribute = "PRIV";
						index--;
						state=0;
					}else state=1;
					break;
				
				
				//relop 	
				case 52: //<
					c = buffer[++index];
					if(c == '=') state = 53;
					else if(c == '>') state = 54;
					else if(c == '-') state=132;
					else{
						token->token = "relop";
						token->attribute = "LT";
						index--; //rollback
						state=0;
					} 
					break;
				case 132: //<- atribuicao
					token->token = "comando";
					token->attribute = "atribuicao";
					state=0;
					break;
					
				case 53: //=
					if(strcmp(token->attribute, "LT") == 0){
						token->attribute = "LE";
						state =0;
					}else if(strcmp(token->attribute, "GT") == 0){
						token->attribute = "GE";
						state = 0;
					}else{
						token->token = "relop";
						token->attribute = "EQ";
						state=0;
					}
					break;
				case 54: //>
					c = buffer[++index];
					if(c == '=') state = 53;
					else if(strcmp(token->attribute,"LT") == 0){
						token->attribute = "NE";
						state = 0;
					}
					else{
						token->token = "relop";
						token->attribute = "GT";
						index--; //rollback
						state=0;
					} 
					break;
				case 107: //ou
					c = buffer[++index];
					if(c == 'u') state = 38;
					else state=1;
					break;
				case 38:
					c = buffer[++index];
					if(c==32 || c==' '){
						token->token = "relop";
						token->attribute = "OU";
						index--;
						state =0;
					}else state=1;
					break;
				
				//verificando E, entao, enquanto, exp, escreva
				case 106: //e no AF, os estados sao 86,99,111,47, removendo esses valores
					c = buffer[++index];
					if(c==32 || c==' '){
						token->token = "relop";
						token->attribute = "E";
						index--;
						state=0;
					}
					else if(c == 'n') state=48; //entao 47 e 86
					else if(c == 's') state=100; //escreva 99
					else if(c == 'x') state=112; //exp 111
					else state=1;
					break;
				case 48: //n - entao || enquanto
					c = buffer[++index];
					if(c == 't') state=49;
					else if(c == 'q') state=88;
					else state=1;
					break;
				case 49: //entao
					c = buffer[++index];
					if(c == 'a') state=50;
					else state=1;
					break;
				case 50: //entao
					c = buffer[++index];
					if(c == 'o') state=51;
					else state=1;
					break;
				case 51: //entao
					c = buffer[++index];
					if(c==32 || c==' ' || c=='\0'){
						token->token = "entao";
						token->attribute = "PRIV";
						index--;
						state=0;
					}else state=1;
					break;
				case 88: //enquanto
					c = buffer[++index];
					if(c == 'u') state=89;
					else state=1;
					break;
				case 89: //enquanto
					c = buffer[++index];
					if(c == 'a') state=90;
					else state=1;
					break;
				case 90: //enquanto
					c = buffer[++index];
					if(c == 'n') state=91;
					else state=1;
					break;
				case 91: //enquanto
					c = buffer[++index];
					if(c == 't') state=92;
					else state=1;
					break;
				case 92: //enquanto
					c = buffer[++index];
					if(c == 'o') state=93;
					else state=1;
					break;
				case 93: //enquanto
					c = buffer[++index];
					if(c==32 || c==' '){
						token->token = "enquanto";
						token->attribute = "PRIV";
						index--;
						state=0;
					}else state=1;
					break;
				case 100: //escreva
					c = buffer[++index];
					if(c == 'c') state=101;
					else state=1;
					break;
				case 101:
					c = buffer[++index];
					if(c == 'r') state=102;
					else state=1;
					break;
				case 102:
					c = buffer[++index];
					if(c == 'e') state=103;
					else state=1;
					break;
				case 103:
					c = buffer[++index];
					if(c == 'v') state=104;
					else state=1;
					break;
				case 104:
					c = buffer[++index];
					if(c == 'a') state=105;
					else state=1;
					break;
				case 105: //escreva
					c = buffer[++index];
					if(c == '(' || c == ')' || c == ' ' || c==32){
						token->token = "funcaoio";
						token->attribute = "escreva";
						index--;
						state=0;
					}else if(c=='l') state=152;
					else state=1;
					break;
				case 152: //escreval
					c = buffer[++index];
					if(c == '(' || c==')' || c== ' ' || c==32){
						token->token = "funcaoio";
						token->attribute = "escreval";
						index--;
						state=0;
					}else state=1;
					break;
					
				case 112: //exp
					c = buffer[++index];
					if(c == 'p') state=113;
					else state=1;
					break;
				case 113:
					c = buffer[++index];
					if(c==32 || c==' '){
						token->token = "relop";
						token->attribute = "exp";
						index--;
						state=0;
					}else state=1;
					break;
				
				case 94: //leia
					c = buffer[++index];
					if(c == 'e') state=95;
					else state=1;
					break;
				case 95:
					c = buffer[++index];
					if(c=='i') state=96;
					else state=1;
					break;
				case 96:
					c = buffer[++index];
					if(c=='a') state=97;
					else state=1;
					break;
				case 97:
					c = buffer[++index];
					if(c =='(' || c==')' || c!=' ' || c == 32){
						token->token = "funcaoio";
						token->attribute = "leia";
						index--;
						state=0;
					}else state=1;
					break;
					
				
				//SOMA
				case 68:
					token->token = "operador";
					token->attribute = "SOMA";
					state=0;
					break;
				//SUBTRACAO
				case 69:
					token->token = "operador";
					token->attribute = "SUBT";
					state=0;
					break;
				//MULTI
				case 70:
					token->token = "operador";
					token->attribute = "MULT";
					state=0;
					break;
				//comentario e DIV
				case 71:
					if(strcmp(token->token, "COMENT") == 0){
						//se for um comentario, ira ler a linha restante
						while( (c = buffer[++index]) != '\n' && c!='\0' && c!=EOF );
						//token->token = ""; //reseta para NULL para nao imprimir no arquivo ao voltar pro estado 0
					}else{
						state = 0;
						index--;
					}
					break;
				
				//pontuacao ( )
				case 114: //)
					token->token = "pontuacao";
					token->attribute = "PF";
					state=0;
					break;
				case 115://(
					token->token = "pontuacao";
					token->attribute = "PA";
					state=0;
					break;
				case 140: //: dois pontos
					token->token = "pontuacao";
					token->attribute = "DOISPT";
					state=0;
					break;
				case 141: //;
					token->token = "pontuacao";
					token->attribute = "PTVIR";
					state=0;
					break;
				case 142: //,
					token->token = "pontuacao";
					token->attribute = "VIR";
					state=0;
					break;

			}
			
			
			
			//condicao de parada se index atingir valor i (buffer inserido)
			if(i <= index){
				i=0;
				index = 0;
				break;
			}
		}
		
		free(string_buffer);
		
	}



	fclose(fp);
	
	for(i=0; i<qtde_vars; i++)
		free(vars[i]);
	
	free(token);
	


}

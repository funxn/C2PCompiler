/*************************************************************************
    > File Name: scanner.c
    > Author: xiaofeng
    > Mail: xxf263@gmail.com 
    > Created Time: Thursday, May 05, 2016 AM09:15:00 CST
 ************************************************************************/

#include"compiler.h"

#define TOKEN_SIZE 32			// 限定标识符长度

int LexAnalysis();
int isLetter(char ch);
int isDigit(char ch);
int reserve(char* str);
int getVar(char* str);
void addVar(char* str, int varw);
int FGS(char* buf, int num, FILE* fp);

int main(int argc, char* argv[]){
	if(argc<1){
		printf("Usage: ./a.out [sourcefile]\n");
		exit(0);
	}
	LexAnalysis(argv[1]);
}

// 词法分析主程序
int LexAnalysis(char* str){
	FILE *source;				// source: the C source file
	if((source = fopen(str, "r")) == NULL){
		printf("can't open file %s!", str);
		exit(0);
	}
	FILE *target;				// source: the C source file
	if((target = fopen("target.c", "w")) == NULL){
		printf("can't open file target.c!");
		exit(0);
	}

	char buf[2][BUF_SIZE];			// input buffer
	int buf_k = 0;					// tag for choosing which buf[]
	int i, j;
	int flag = 0;					// 用于对缓冲区半区切换进行标记。flag!=0时，记录的是token的长度
	char token[TOKEN_SIZE] = "";
	int token_i = 0;				// token数组游标
	int varw = 0;
	char ch;
	
	while(FGS(buf[buf_k] + flag, 1023-flag, source)){
	// get_char();
	i = 0;
	flag = 0;
	while((ch=buf[buf_k][i]) != '\0'){
		memset(token, 0, sizeof(token));
		token_i = 0;				// *********
		// get_nbc();
		while((ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t') && ch != '\0')
			ch = buf[buf_k][++i];
		/*
		 * 修正：当ch=='\0'时，不便于修改flag的值，所以在此处将j回退，显示为正常水平
		 */
		if(ch == '\0')
			j = i - 1;
		else
			j = i;
		switch(ch){
			case '\0':
				break;
			case '<':
				ch = buf[buf_k][++j];
				if(ch == '='){
					strcpy(token, "relop LE\n");
					fwrite(token, sizeof(char), 9, target);
				}
				else if(ch == '>'){
					strcpy(token, "relop NE\n");
					fwrite(token, sizeof(char), 9, target);
				}
				else if(ch == '\0'){
					token[0] = '<';
					flag = sizeof(token);
				}
				else{
					j--;
					strcpy(token, "relop LT\n");
					fwrite(token, sizeof(char), 9, target);
				}
				break;
			case '>':
				ch = buf[buf_k][++j];
				if(ch == '='){
					strcpy(token, "relop GE\n");
					fwrite(token, sizeof(char), 9, target);
				}
				else if(ch == '\0'){
					token[0] = '>';
					flag = sizeof(token);
				}
				else{
					j--;
					strcpy(token, "relop GT\n");
					fwrite(token, sizeof(char), 9, target);
				}
				break;
			case '=':
				strcpy(token, "relop EQ\n");
				fwrite(token, sizeof(char), 9, target);
				break;
			case '/':
				ch = buf[buf_k][++j];
				if(ch == '*'){
					while(ch != '/'){
						while((ch = buf[buf_k][++j]) != '*' && ch != '\0');
						if(ch == '\0'){
							token[0] = '/';
							token[1] = '*';
							flag = sizeof(token);
						}
						if((ch = buf[buf_k][++j]) == '*')
							j--;
						else if(ch == '\0'){
							token[0] = '/';
							token[1] = '*';
							token[3] = '*';
							flag = sizeof(token);
						}
					}
				}
				else if(ch == '/'){
					while((ch = buf[buf_k][++j]) != '\n' && ch != '\0');
					if(ch == '\0'){
						token[0] = '/';
						token[1] = '/';
						flag = sizeof(token);
					}
				}
				else if(ch == '\0'){
					token[0] = '/';
					flag = sizeof(token);
				}
				else{
					strcpy(token, "/ null\n");
					fwrite(token, sizeof(char), 7, target);
				}
				break;
			case ':':
				ch = buf[buf_k][++j];
				if(ch == '='){
					strcpy(token, "assign-op null\n");
					fwrite(token, sizeof(char), 15, target);
				}
				else if(ch == '\0'){
					token[0] = ':';
					flag = sizeof(token);
				}
				else{
					j--;
					strcpy(token, ": null\n");
					fwrite(token, sizeof(char), 7, target);
				}
				break;
			case '+':
				strcpy(token, "+ null\n");
				fwrite(token, sizeof(char), 7, target);
				break;
			case '-':
				strcpy(token, "- null\n");
				fwrite(token, sizeof(char), 7, target);
				break;
			case '(':
				strcpy(token, "( null\n");
				fwrite(token, sizeof(char), 7, target);
				break;
			case ')':
				strcpy(token, ") null\n");
				fwrite(token, sizeof(char), 7, target);
				break;
	/*		case '{':
				strcpy(token, "{ null\n");
				fwrite(token, sizeof(char), 7, target);
				break;
			case '}':
				strcpy(token, "} null\n");
				fwrite(token, sizeof(char), 7, target);
				break;
	*/
			case ';':
				strcpy(token, "; null\n");
				fwrite(token, sizeof(char), 7, target);
				break;
			default:
					// case 'a'...'z''A'...'Z':
					if((ch <= 122 && ch >= 97) || (ch <= 90 && ch >= 65) || ch == '_'){
						do{
							token[token_i++] = ch;
							ch = buf[buf_k][++j];
							if(ch == '\0'){
								flag = sizeof(token);
								break;
							}
						}while(isLetter(ch) || isDigit(ch) || ch == '_');
						if(flag) break;
						j--;
						token[token_i] = '\0';
						if(reserve(token)){
							fprintf(target, "%s null\n", token);
						}else if((varw = getVar(token)) != 0){
							fprintf(target, "ID %d\n", varw);
						}else{
							varw = malloc(sizeof(int));
							printf("%d\n", varw);
							addVar(token, varw);
							fprintf(target, "ID %d\n", varw);
						}
					}
					// case '0'...'9':
					else if(isDigit(ch)){
						do{
							token[token_i++] = ch;
							ch = buf[buf_k][++j];
							if(ch == '\0'){
								flag = sizeof(token);
								break;
							}
						}while(isDigit(ch));
						if(flag) break;
						j--;
						fprintf(target, "num %s\n", token);
					}
					else{
						printf("error char is: %c, code is %d", ch, ch);
						exit(0);
					}
		}
		if(flag == 0)				// 正常情况
			i = j + 1;
		else						// 遇到\0情况，指针停在j处
			i = j;
	}
	buf_k = (buf_k + 1)%2;
	strcpy(buf[buf_k], token);				// 将token被截断的部分移到后半区
	}
	if(!feof(source)){
		printf("Error in FGS()");
		exit(0);
	}
	fclose(source);
	fclose(target);
}

int isLetter(char ch){
	return (ch <= 122 && ch >= 97) || (ch <= 90 && ch >= 65);
}
int isDigit(char ch){
	return ch <= 57 && ch >= 48;
}
// 判断是否为保留字， 是则返回1
int reserve(char* str){
	FILE *res_w;				// res_w: reserve word(in C, there are 32 words reserved!)
	if((res_w = fopen("reserve_words.txt", "r")) == NULL){
		printf("can't open file reserve_words.txt!");
		exit(0);
	}
	// rewind(res_w);
	char pair[TOKEN_SIZE];
	int flag = 0;
	while(!feof(res_w)){
		fscanf(res_w, "%s", pair);
		if(strcmp(pair, str) == 0){
			flag = 1;
			break;
		}
	}
	fclose(res_w);
	return flag;
}
// 判断是否已存在该标识符, 存在就获取地址
int getVar(char *str){
	FILE *v;				// res_w: reserve word(in C, there are 32 words reserved!)
	if((v = fopen("var_words.txt", "r")) == NULL){
		printf("can't open file var_words.txt!");
		exit(0);
	}
	char pair[TOKEN_SIZE];
	int flag = 0;
	int varw = 0;
	while(!feof(v)){
		fscanf(v, "%s %d\n", pair, &varw);
		if(strcmp(pair, str) == 0){
			flag = 1;
			break;
		}
	}
	fclose(v);
	if(flag == 1) return varw;
	else return 0;
}
void addVar(char* str, int varw){
	FILE *v;				// res_w: reserve word(in C, there are 32 words reserved!)
	if((v = fopen("var_words.txt", "a")) == NULL){
		printf("can't open file var_words.txt!");
		exit(0);
	}
	fprintf(v, "%s %d\n", str, varw);
	fclose(v);
}
// 重写函数fgets()为FGS();
int FGS(char* buf, int num, FILE* fp){
	int i;
	char ch;
	for(i=0; i < num; i++){
		if((ch = fgetc(fp)) == EOF){
			buf[i] = '\0';
			break;
		}else{
			buf[i] = ch;
		}
	}
	return i;
}

// 局限记录：
// 1. 不支持浮点数，只能整数
// 2. 还未写 ++， --， 等
// 3. 最大问题：输出的文档全是字符串，难以读入判断等等（字符串匹配算法效率会比较低的！！！）

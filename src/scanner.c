/*************************************************************************
    > File Name: scanner.c
    > Author: xiaofeng
    > Mail: xxf263@gmail.com 
    > Created Time: Thursday, May 05, 2016 AM09:15:00 CST
 ************************************************************************/

#include"compiler.h"

#define TOKEN_SIZE 32			// 限定标识符长度

int LexAnalysis(char* str);
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
	int thisline;

	FILE *source;				// source: the C source file
	if((source = fopen(str, "r")) == NULL){
		printf("can't open file %s!", str);
		exit(0);
	}
	FILE *target;				// source: the C source file
	if((target = fopen("target.pas", "w")) == NULL){
		printf("can't open file target.c!");
		exit(0);
	}

	char buf[2][BUF_SIZE];			// input buffer
	int buf_k = 0;					// tag for choosing which buf[]
	int i, j;
	int flag = 0;					// 用于对缓冲区半区切换进行标记。flag!=0时，记录的是token的长度
	char token[TOKEN_SIZE] = "";
	int token_i = 0;				// token数组游标

	// 2016.05.12添加
	int reserve_i;
	int point_flag;					// 识别浮点数使用，只能出现一次


	int varw = 0;
	char ch;
	
	while(FGS(buf[buf_k] + flag, 1023-flag, source)){
	// get_char();
	i = 0;
	flag = 0;
	while((ch=buf[buf_k][i]) != '\0'){
		memset(token, 0, sizeof(token));
		token_i = 0;
		point_flag = 1;				// *********
		// get_nbc();
		while((ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t') && ch != '\0'){
			ch = buf[buf_k][++i];
			if(ch == '\n')
				thisline++;
		}
			
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
					strcpy(token, "42 relop 12\n");
					fwrite(token, sizeof(char), 12, target);
				}
				else if(ch == '>'){
					strcpy(token, "42 relop 10\n");
					fwrite(token, sizeof(char), 12, target);
				}
				else if(ch == '\0'){
					token[0] = '<';
					flag = sizeof(token);
				}
				else{
					j--;
					strcpy(token, "42 relop 11\n");
					fwrite(token, sizeof(char), 12, target);
				}
				break;
			case '>':
				ch = buf[buf_k][++j];
				if(ch == '='){
					strcpy(token, "42 relop 14\n");
					fwrite(token, sizeof(char), 12, target);
				}
				else if(ch == '\0'){
					token[0] = '>';
					flag = sizeof(token);
				}
				else{
					j--;
					strcpy(token, "42 relop 13\n");
					fwrite(token, sizeof(char), 12, target);
				}
				break;
			case '=':
				strcpy(token, "42 relop 9\n");
				fwrite(token, sizeof(char), 11, target);
				break;
			case '{':
				while((ch = buf[buf_k][++j]) != '}' && ch != '\0'){
					if(ch == '\n')
						thisline++;
					};
				if(ch == '\0'){
					token[0] = '}';
					flag = sizeof(token);
				}
				else if(ch == EOF){
					printf("error: unclosed comment! Line: %d\n", thisline);
					exit(1);
				}
				break;
			case ':':
				ch = buf[buf_k][++j];
				if(ch == '='){
					strcpy(token, "41 assignop 8\n");
					fwrite(token, sizeof(char), 14, target);
				}
				else if(ch == '\0'){
					token[0] = ':';
					flag = sizeof(token);
				}
				else{
					j--;
					strcpy(token, "45 : 17\n");
					fwrite(token, sizeof(char), 8, target);
				}
				break;
			case '+':
				strcpy(token, "39 addop 2\n");
				fwrite(token, sizeof(char), 11, target);
				break;
			case '-':
				strcpy(token, "39 addop 3\n");
				fwrite(token, sizeof(char), 11, target);
				break;
			case '*':
				strcpy(token, "40 mulop 4\n");
				fwrite(token, sizeof(char), 11, target);
				break;
			case '/':
				strcpy(token, "40 mulop 5\n");
				fwrite(token, sizeof(char), 11, target);
				break;
			case '(':
				strcpy(token, "43 ( 15\n");
				fwrite(token, sizeof(char), 8, target);
				break;
			case ')':
				strcpy(token, "44 ) 16\n");
				fwrite(token, sizeof(char), 8, target);
				break;
			case '[':
				strcpy(token, "48 [ 20\n");
				fwrite(token, sizeof(char), 8, target);
				break;
			case ']':
				strcpy(token, "49 ] 21\n");
				fwrite(token, sizeof(char), 8, target);
				break;
			case ';':
				strcpy(token, "46 ; 18\n");
				fwrite(token, sizeof(char), 8, target);
				break;
			case ',':
				strcpy(token, "47 , 19\n");
				fwrite(token, sizeof(char), 8, target);
				break;
			case '.':
				strcpy(token, "72 . 49\n");
				fwrite(token, sizeof(char), 8, target);
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
						if((reserve_i=reserve(token)) != -1){
							fprintf(target, "%d %s %d\n", reserve_i, token, reserve_i);
						}else if(strcmp(token, "mod")==0){
							fprintf(target, "40 mulop 85\n");
						}else if(strcmp(token, "div")==0){
							fprintf(target, "40 mulop 86\n");
						}else if(strcmp(token, "and")==0){
							fprintf(target, "40 mulop 87\n");
						}else{
							fprintf(target, "35 id %d\n", getVar(token));
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
							if(ch == '.' && point_flag){
								point_flag = 0;
								token[token_i++] = ch;
								ch = buf[buf_k][++j];
							}
						}while(isDigit(ch));
						if(flag) break;
						j--;
						// 添加到NUMLIST
						strcpy(numlist[NUMLIST_CUR_NUM].value, token);
						numlist[NUMLIST_CUR_NUM].numID = NUMLIST_CUR_NUM;
						NUMLIST_CUR_NUM++;
						fprintf(target, "36 num %s\n", token);
					}
					else{
						printf("error char is: %c, code is %d, line is: %d\n", ch, ch, thisline);
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
	int flag = -1;
	int i;
	for(i=1; i<=VAR_NUM; i++){
		if(strcmp(var_list[i], str) == 0){
			flag = i;
			break;
		}
	}
	return flag;
}
// 判断是否已存在该标识符, 存在就获取地址,否则就创建
int getVar(char *str){
	int flag = 0;
	int i;
	for(i=0; i<IDLIST_CUR_NUM; i++){
		if(strcmp(idlist[i].name, str) == 0){
			flag = 1;
			break;
		}
	}
	if(flag == 1) 
		return idlist[i].entryID;
	else{
		idlist[IDLIST_CUR_NUM].entryID = IDLIST_CUR_NUM;
		strcpy(idlist[IDLIST_CUR_NUM].name, str);
		IDLIST_CUR_NUM++;
		return IDLIST_CUR_NUM-1;
	}
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
// 3. 最大问题：输出的文档全是字符串，难以读入判断等等（字符串匹配算法效率会比较低的！！！）

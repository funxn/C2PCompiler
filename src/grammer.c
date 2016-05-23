/*************************************************************************
    > File Name: grammer.c
    > Author: xiaofeng
    > Mail: xxf263@gmail.com 
    > Created Time: Tuesday, May 10, 2016 PM05:19:26 CST
 ************************************************************************/

#include "compiler.h"

TOKEN token[1024];
int len = 0;
SqStack *s;


	// 表达式存储, 其中f_vn[]存储产生式前面的非终结符, flen[]存储产生式右部的长度信息


// 从文件中接收输入并对输入进行处理（保留第一列）
// token[] 即是 由词法分析器产生的token流
void lexi_input(){
	FILE *source;				// source: the C source file
	if((source = fopen("example/target.pas", "r")) == NULL){
		printf("can't open file example/target!");
		exit(0);
	}
	while(fscanf(source, "%d %s %d\n", token[len].sign, token[len].value, token[len].id) != EOF){
		len++;
	}
}

// flen: 找出产生式右部的长度信息
int flen(int* arr){
	int i;
	for(i=0; arr[i]!=0&&arr[i]!=E_NUM; i++);
	return i-1;
}

void LR(){
	int temp;
	// 将输入栈最后添加$
	token[len].sign = END_NUM;
	len++;
	// 清空分析栈
	s = InitStack(s);
	// 从1号项目开始
	Push(s, 1);
	int pointer = 0;

	int cur_state, cur_token, cur_pointer;
	int i, j, k=1, it=0;

	int meaning;					// 移进时标识语义动作的标号
	int index_m;					// 记录要在“带有语义动作的产生式数组”中查找的表达式编号（即对应非终结符）
	int pos_m;						// 记录该移进动作对应的移进位置，以唯一匹配“带有语义动作的产生式数组”中的产生式
									// 因为可能有-1分割多个
	
	FILE* fp;
	int finpos=0;						// 记录当前插入位置距行首距离, 遇\n则置零
	if((fp=fopen("target.c", "a+")) == NULL){
		printf("can't open target.c\n");
		exit(1);
	}

	printf("栈                                       输入     输出\n");
	do{
		//开始处理
		cur_state = *(s->top-1);
		cur_token = token[pointer].sign;
		cur_pointer = pointer;
		//移进项目
		if(act[cur_state][cur_token].rs == S){					
			Push(s, cur_token);
			Push(s, act[cur_state][cur_token].no[0]);
			pointer++;
			printf("S_%d\n", act[cur_state][cur_token].no[0]);

			// 记录移进时的标号：
			meaning = 0;
			for(i=0; i<PS[cur_state].len; i++){
				if(cur_token == PS[cur_state].proj[i].thisOp[PS[cur_state].proj[i].pos]){
					index_m = PS[cur_state].proj[i].thisOp[0];
					pos_m = PS[cur_state].proj[i].pos;
					break;											// 默认文法是无移进-移进冲突的！ 此处直接break
				}
			}
			for(i=2; genOp_more[index_m][i]!=0; i++){
				j=0;
				while(genOp_more[index_m][i]!=0 && genOp_more[index_m][i]!=-1 && j != pos_m){
					if(genOp_more[index_m][i] < 100)
						j++;
					i++;
				}
				if(j == pos_m && cur_token == genOp_more[index_m][i-1]){
					meaning = genOp_more[index_m][i];								// 此处比较危险！如果一直找不到应该有警告
					break;
				}
				while(genOp_more[index_m][i]!=0 && genOp_more[index_m][i]!=-1)		// 读完后面的，继续搜索下一个
					i++;															// 不必平衡i，因为0后面就都是0
			}
		//归约项目
		}else if(act[cur_state][cur_token].rs == R){			
			j = act[cur_state][cur_token].no[0];
			i = 2*flen(act[cur_state][cur_token].no);
			while(i--){
				Pop(s, &temp);
			}
			printf("S_%d", act[cur_state][cur_token].no[0]);

			cur_state = *(s->top-1);
			/* 可能出错 */
			// 要使用的回退表达式的首非终结符入栈
			Push(s, j);
			Push(s, gto[cur_state][j]);
			printf(" | R_%d", j);


			// 记录归约时，移进的标号
			meaning = 0;
			for(i=0; i<PS[cur_state].len; i++){
				if(act[cur_state][cur_token].no[0] == PS[cur_state].proj[i].thisOp[PS[cur_state].proj[i].pos]){
					index_m = PS[cur_state].proj[i].thisOp[0];
					pos_m = PS[cur_state].proj[i].pos;
					break;											// 默认文法是无移进-移进冲突的！ 此处直接break
				}
			}
			for(i=2; genOp_more[index_m][i]!=0; i++){
				j=0;
				while(genOp_more[index_m][i]!=0 && genOp_more[index_m][i]!=-1 && j != pos_m){
					if(genOp_more[index_m][i] < 100)
						j++;
					i++;
				}
				if(j == pos_m && cur_token == genOp_more[index_m][i-1]){
					meaning = genOp_more[index_m][i];								// 此处比较危险！如果一直找不到应该有警告
					break;
				}
				while(genOp_more[index_m][i]!=0 && genOp_more[index_m][i]!=-1)		// 读完后面的，继续搜索下一个
					i++;															// 不必平衡i，因为0后面就都是0
			}
		}else if(act[cur_state][cur_token].rs == ACC){
			printf("\n\n===============Accept!!!=================\n");
			break;
		}else{
			printf("\n\n===============Error !!!=================\n");
			break;
		}

		k++;
		it=0;

		// 执行具体的语义动作：
		switch(meaning){
			case 100:
				fprintf(fp, "int main");
				finpos += 8;
				break;
			case 101:
				fprintf(fp, "(");
				finpos += 1;
				break;
			case 102:
				fprintf(fp, ")\n{");
				finpos = 0;
				break;
			case 103:
				fprintf(fp, "%s", token[cur_pointer].id);
				finpos += 4;				// 后面替换为id长度
				break;
			case 104:
				fseek(fp, -finpos, SEEK_CUR);
				break;
			case 105:
				fseek(fp, 0, SEEK_END);
				fprintf(fp, "['Digits']");
				finpos += 10;
				break;
			case 106:
				fprintf(fp, "int");
				finpos += 3;
				break;
			case 107:
				fprintf(fp, "float");
				finpos += 5;
				break;
			case 108:
				fprintf(fp, "Boolean");
				finpos += 7;
				break;
			case 109:
				fprintf(fp, "{");
				finpos += 1;
				break;
			case 110:
				fprintf(fp, "}");
				finpos += 1;
				break;
			case 111:
				fprintf(fp, "'ID'");
				finpos += 4;
				break;
			case 112:
				fseek(fp,-finpos, SEEK_CUR);
				fprintf(fp, "'STANDARD_TYPE'");
				finpos += 15;
				break;
			case 113:
				fprintf(fp, "void");
				finpos += 4;
				break;
			case 114:
				fprintf(fp, "'ID'");
				finpos += 4;
				break;
			case 115:
				fprintf(fp, "(");
				finpos += 1;
				break;
			case 116:
				fprintf(fp, ")");
				finpos += 1;
				break;
			case 117:
				fseek(fp, -finpos, SEEK_CUR);
				break;
			case 118:
				fseek(fp, 0, SEEK_END);
				break;
			case 119:
				printf("'ASSIGNOP'");

			default:
				printf("ERROR: no such meaning: %d\n", meaning);
				// exit(1);
		}
	}while(1);
}

int main(){
	// 处理输入
	create_p_set();
	goto_action();
	lexi_input();
	LR();

	return 0;
}
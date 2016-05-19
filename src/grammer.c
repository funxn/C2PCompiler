/*************************************************************************
    > File Name: grammer.c
    > Author: xiaofeng
    > Mail: xxf263@gmail.com 
    > Created Time: Tuesday, May 10, 2016 PM05:19:26 CST
 ************************************************************************/

#include "compiler.h"

int token[1024];
int len = 0;
SqStack *s;


	// 表达式存储, 其中f_vn[]存储产生式前面的非终结符, flen[]存储产生式右部的长度信息


// 从文件中接收输入并对输入进行处理（保留第一列）
// token[] 即是 由词法分析器产生的token流
void lexi_input(){
	char buf[32];
	int temp;
	FILE *source;				// source: the C source file
	if((source = fopen("example/target.pas", "r")) == NULL){
		printf("can't open file example/target!");
		exit(0);
	}
	while(fscanf(source, "%d %s %d\n", token+len, buf, &temp) != EOF){
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
	token[len] = END_NUM;
	len++;
	// 清空分析栈
	s = InitStack(s);
	// 从1号项目开始
	Push(s, 1);
	int pointer = 0;

	int cur_state, cur_token;
	int i, j, k=1, it=0;

	printf("栈                                       输入     输出");
	do{
		//开始处理
		cur_state = *(s->top-1);
		cur_token = token[pointer];
		//移进项目
		if(act[cur_state][cur_token].rs == S){					
			Push(s, cur_token);
			Push(s, act[cur_state][cur_token].no[0]);
			pointer++;
			printf("S_%d\n", act[cur_state][cur_token].no[0]);
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
		}else if(act[cur_state][cur_token].rs == ACC){
			printf("\n\n===============Accept!!!=================\n");
			break;
		}else{
			printf("\n\n===============Error !!!=================\n");
			break;
		}

		k++;
		it=0;
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
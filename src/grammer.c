/*************************************************************************
    > File Name: grammer.c
    > Author: xiaofeng
    > Mail: xxf263@gmail.com 
    > Created Time: Tuesday, May 10, 2016 PM05:19:26 CST
 ************************************************************************/

#include<stdio.h>



#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <stack>
#include <string>
#include <iostream>

using namespace std;

const int S = 1;
const int R = 2;
const int ACC = 3;
const int Vt_num = 9;
const int Vn_num = 3;
const int State_num = 17;
const int formula_num = 10;
const int Max_input = 1024;

string istr;
int iErrChar = 0; 					// 错误字符标志

int token[Max_input];
int len;
int pointer;
string fm[formula_num];
int f_vn[formula_num];
int flen[formula_num];

stack<int> ST; 

typedef struct{
	int rs;
	int no;
}block;

block act[State_num][Vt_num];
int gto[State_num][Vn_num];

char VN[5]={'E','A','T','B','F'};						//存储非终结符
	// 使用整数0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 ,10, 11 分别代替
char VTN[12]={'+','-','*','/','i','n','(',')','$','E','T','F'};		//存储终结符


void init_table(){
	memset(act, 0, sizeof(act));
	memset(gto, -1, sizeof(gto));

	// action表格构造
	act[0][4] = {S, 4}; act[0][5] = {S, 6}; act[0][6] = {S, 5};
	act[1][0] = {S, 7}; act[1][1] = {S, 8}; act[1][8].rs = ACC;
	act[2][0] = {R, 3}; act[2][1] = {R, 3}; act[2][2] = {S, 9}; act[2][3] = {S, 10}; act[2][7] = {R, 3}; act[2][8] = {R, 3};
	act[3][0] = act[3][1] = act[3][2] = act[3][3] = act[3][7] = act[3][8] = {R, 6};
	act[4][0] = act[4][1] = act[4][2] = act[4][3] = act[4][7] = act[4][8] = {R, 7};
	act[5][4] = {S, 4}; act[5][5] = {S, 6}; act[5][6] = {S, 5};
	act[6][0] = act[6][1] = act[6][2] = act[6][3] = act[6][7] = act[6][8] = {R, 7};
	act[7][4] = {S, 4}; act[7][5] = {S, 6}; act[7][6] = {S, 5};
	act[8][4] = {S, 4}; act[8][5] = {S, 6}; act[8][6] = {S, 5};
	act[9][4] = {S, 4}; act[9][5] = {S, 6}; act[9][6] = {S, 5};
	act[10][4] = {S, 4}; act[10][5] = {S, 6}; act[10][6] = {S, 5};
	act[11][0] = {S, 7}; act[11][1] = {S, 8}; act[11][7] = {S, 16};
	act[12][0] = {R, 1}; act[12][1] = {R, 1}; act[12][2] = {S, 9}; act[12][3] = {S, 10}; act[12][7] = {R, 1}; act[12][8] = {R, 1};
	act[13][0] = {R, 2}; act[13][1] = {R, 2}; act[13][2] = {S, 9}; act[13][3] = {S, 10}; act[13][7] = {R, 2}; act[13][8] = {R, 2};
	act[14][0] = act[14][1] = act[14][2] = act[14][3] = act[14][7] = act[14][8] = {R, 4};
	act[15][0] = act[15][1] = act[15][2] = act[15][3] = act[15][7] = act[15][8] = {R, 5};
	act[16][0] = act[16][1] = act[16][2] = act[16][3] = act[16][7] = act[16][8] = {R, 8};

	// goto表格构造
	gto[0][0] = 1; gto[0][1] = 2; gto[0][2] = 3;
	gto[5][0] = 11; gto[5][1] = 2; gto[5][2] = 3;
	gto[7][1] = 12; gto[7][2] = 3;
	gto[8][1] = 13; gto[8][2] = 3;
	gto[9][2] = 14;
	gto[10][2] = 15;
	gto[11][2] = 16;

	// 表达式存储, 其中f_vn[]存储产生式前面的非终结符, flen[]存储产生式右部的长度信息
	fm[1] = "E -> E+T\n"; 	f_vn[1] = 0; flen[1] = 3;
	fm[2] = "E -> E-T\n"; 	f_vn[2] = 0; flen[2] = 3;
	fm[3] = "E -> T\n";		f_vn[3] = 0; flen[3] = 1;
	fm[4] = "T -> T*F\n"; 	f_vn[4] = 1; flen[4] = 3;
	fm[5] = "T -> T/F\n"; 	f_vn[5] = 1; flen[5] = 3;
	fm[6] = "T -> F\n"; 	f_vn[6] = 1; flen[6] = 1;
	fm[7] = "F -> id\n"; 	f_vn[7] = 2; flen[7] = 1;
	fm[8] = "F -> (E)\n"; 	f_vn[8] = 2; flen[8] = 3;
	fm[9] = "F -> num\n"; 	f_vn[9] = 2; flen[9] = 1;
}


// 接收输入并对输入的算式进行处理
// token[] 即是 由词法分析器产生的token流
// void lexi_input(){
// 	int i;
// 	len = 0;
// 	cout << "输入要分析的字符串: ";
// 	cin >> istr;

// 	for(i=0; i<istr.size(); i++){
// 		switch(istr[i]){
// 			case '+':
// 				token[len++] = 0;
// 				break;
// 			case '-':
// 				token[len++] = 1;
// 				break;
// 			case '*':
// 				token[len++] = 2;
// 				break;
// 			case '/':
// 				token[len++] = 3;
// 				break;
// 			case '(':
// 				token[len++] = 6;
// 				break;
// 			case ')':
// 				token[len++] = 7;
// 				break;
// 			default:
// 				if((istr[i]<='z' && istr[i]>='a') || (istr[i]<='Z' && istr[i]>='A')){
// 					while((istr[i]<='z' && istr[i]>='a') || (istr[i]<='Z' && istr[i]>='A'))
// 						i++;
// 					token[len++] = 4;
// 					i--;						// 指针回退
// 				}else if(istr[i]<='9' && istr[i]>='0'){
// 					while(istr[i]<='9' && istr[i]>='0')
// 						i++;
// 					token[len++] = 5;
// 					i--;	
// 				}else{
// 					iErrChar = 1;
// 				}
// 		}

// 		if(iErrChar){
// 			cout << "输入中有不符合要求的字符， 分析失败！ 退出...";
// 			break;
// 		}
// 	}
// }

void LR(){
	// 将输入栈最后添加$
	token[len] = Vt_num - 1;
	len++;
	// 清空分析栈
	while(!ST.empty()){
		ST.pop();
	}
	ST.push(0);
	pointer = 0;

	int cur_state, cur_token;
	int i, j, k=1, it=0;;

	cout<<"栈                      "<<"                 输入"<<"     输出"<<endl;
	do{
		cout<<k;
		if(k>99)
			cout<<":";
		else
			if(k>9)
				cout<<": ";
			else
				cout<<":  ";
		//输出
		stack<int>t_ST;//栈	
		int t_char;
		int c_i = 0;	// 判断应该输出标号还是相应的字符
		int i2=ST.size();
		for(int i1=0; i1<i2; i1++)
		{
			t_char=ST.top();
			t_ST.push(t_char);
			ST.pop();
		}
		for(int i1=0; i1<i2; i1++)
		{
			t_char=t_ST.top();
			t_ST.pop();
			if(c_i == 0){
				cout << t_char;
				if(t_char>=10)
					it++;					// 记录空格数量
			}else{
				cout<<VTN[t_char];
			}
			c_i = (c_i + 1)%2;
			ST.push(t_char);
		}
		for(int i1=0; i1<20-i2-it; i1++)
		{
			cout<<" ";
		}
		cout<<"|";
		for(int i1=0; i1<20-len+pointer; i1++)
		{
			cout<<" ";
		}
		for(int i1=pointer; i1<len; i1++)
			cout<<VTN[token[i1]];
		cout<<"     ";

		//开始处理
		cur_state = ST.top();
		cur_token = token[pointer];
		if(act[cur_state][cur_token].rs == S){					//移进项目
			ST.push(cur_token);
			ST.push(act[cur_state][cur_token].no);
			pointer++;
			printf("S_%d\n", act[cur_state][cur_token].no);
		}else if(act[cur_state][cur_token].rs == R){			//归约项目
			j = act[cur_state][cur_token].no;
			i = 2*flen[j];
			while(i--){
				ST.pop();
			}
			printf("S_%d", act[cur_state][cur_token].no);

			cur_state = ST.top();
			ST.push(f_vn[j] + Vt_num);
			ST.push(gto[cur_state][f_vn[j]]);
			cout << " | R_" <<fm[j];
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
	// 初始化分析表
	init_table();
	cout<<endl<<"实现算法4.3，LR分析字符串";
	// 接收并处理输入
	lexi_input();
	// 出错: 直接退出
	if(iErrChar){
		return 0;
	}

	LR();

	return 0;
}
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

char ftemp[100];//存储put_ptr_first操作之前的字符
char type[100];
char fun_name[100];
char  FileTemp[100];//存储id.val
char judege[100];//判断addop等是不是等于:=
char ju_temp=0;
int judege_if=0,fi,fj;//fi表示scanf后跟的符号个数
					//fj表示printf后跟的符号个数，二者也可用来作为循环变量
FILE *fileptr;

void put_ptr_first(FILE *fp,char *ftemp)
{

	char type[100];
	int fi = 0;//用于记录文件行数     
	if((fileptr=fopen("temp.c", "w+")) == NULL)
	{
	printf("can't open temp.c\n");
	exit(1);
	}
	fseek(fp,0, SEEK_SET);//回到fp文件头
	while(fgets(type,100,fp) != NULL )//查询行数
		fi++;
	fseek(fp,0, SEEK_SET);//回到fp文件头
	for(;fi>0;fi--)//向temp.c中写，最后一行存到temp中
	{
		if(fi == 1)
		{
			fgets(ftemp,100,fp);
		}
		else
		{
			fgets(type,100,fp);
			fputs(type,fileptr);
		}		
	}
	fclose(fp);
	if((fp=fopen("target.c", "w")) == NULL)//关闭，再以w方式打开，清空文件
	{
	printf("can't open temp.c\n");
	exit(1);
	}
	fseek(fileptr,0,SEEK_SET);
	while(fgets(type,100,fileptr) != NULL)
		fputs(type,fp);//写回文件
	fclose(fileptr);
	fprintf(fp, "\n");
	fclose(fp);
	if((fp=fopen("target.c", "a+")) == NULL)//最后以a+方式打开，允许读写
	{
	printf("can't open temp.c\n");
	exit(1);
	}
}


// 从文件中接收输入并对输入进行处理（保留第一列）
// token[] 即是 由词法分析器产生的token流
void lexi_input(){
	FILE *source;				// source: the C source file
	if((source = fopen("target.pas", "r")) == NULL){
		printf("can't open file target.pas!");
		exit(0);
	}
	while(fscanf(source, "%d %s %d %d\n", &(token[len].sign), token[len].value, &(token[len].id), &(token[len].linenum)) != EOF){
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
	if((fp=fopen("target.c", "w+")) == NULL){
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
				if(genOp_more[index_m][i-1] > 100){
					if(j == pos_m && cur_token == genOp_more[index_m][i-2]){
						meaning = genOp_more[index_m][i-1];								// 此处比较危险！如果一直找不到应该有警告
						break;
					}
				}else{
					if(j == pos_m && cur_token == genOp_more[index_m][i-1]){
						meaning = genOp_more[index_m][i];								// 此处比较危险！如果一直找不到应该有警告
						break;
					}
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
			// 归约时当前的token应该为j
			meaning = 0;
			for(i=0; i<PS[cur_state].len; i++){
				if(j == PS[cur_state].proj[i].thisOp[PS[cur_state].proj[i].pos]){
					index_m = PS[cur_state].proj[i].thisOp[0];
					pos_m = PS[cur_state].proj[i].pos;
					break;											// 默认文法是无移进-移进冲突的！ 此处直接break
				}
			}
			if(i<PS[cur_state].len){
				cur_token = j;
				for(i=2; genOp_more[index_m][i]!=0; i++){
					j=0;
					while(genOp_more[index_m][i]!=0 && genOp_more[index_m][i]!=-1 && j != pos_m){
						if(genOp_more[index_m][i] < 100)
							j++;
						i++;
					}
					if(genOp_more[index_m][i-1] > 100){
						if(j == pos_m && cur_token == genOp_more[index_m][i-2]){
							meaning = genOp_more[index_m][i-1];								// 此处比较危险！如果一直找不到应该有警告
							break;
						}
					}else{
						if(j == pos_m && cur_token == genOp_more[index_m][i-1]){
							meaning = genOp_more[index_m][i];								// 此处比较危险！如果一直找不到应该有警告
							break;
						}
					}
					while(genOp_more[index_m][i]!=0 && genOp_more[index_m][i]!=-1)		// 读完后面的，继续搜索下一个
						i++;
				}									// 不必平衡i，因为0后面就都是0
			}
		}else if(act[cur_state][cur_token].rs == ACC){
			printf("\n\n===============Accept!!!=================\n");
			break;
		}else{
			printf("\n\n===============Error !!!=================\n");
			printf("ERROR: Grammer wrong at line %d\n", token[cur_pointer].linenum);
			exit(1);
			break;
		}

		k++;
		it=0;

		// 进行简单的类型检查：
		if(cur_token == 41){
			if(strcmp(token[cur_pointer-1].value, token[cur_pointer+1].value) != 0){
				printf("ERROR: line %d ::: type not match! %s := %s\n", token[cur_pointer].linenum, token[cur_pointer-1].value, token[cur_pointer+1].value);
			}
		}

		// 执行具体的语义动作：
				// fseek(fp,-finpos, SEEK_CUR);
				// finpos = 0;
				// break;
				// fseek(fp, 0, SEEK_END);
				// break;
		switch(meaning){
			case 100:
				fprintf(fp, "\nreturn 0;\n}");
				break;
			case 101:
				fprintf(fp, "#include<stdio.h>\n" );
				fprintf(fp, "int main(int argc,char** argv)\n{\n");
				break;
			case 102:
				fprintf(fp, "//");
				break;
			case 103:
				fprintf(fp, "\n");			
				break;
			case 104:
				fprintf(fp, "%s", idlist[token[cur_pointer].id].name);//多两个空格
				fi++;
				break;
			case 105:
				fprintf(fp, ",");				
				break;
			case 106:
				fprintf(fp, ";\n");
				break;
			case 107:
				fprintf(fp, "int ");
				break;
			case 108:
				fprintf(fp, "float ");
				break;
			case 109:
				fprintf(fp, "Boolean ");
				break;
			case 112:
				fprintf(fp, "\n{\n");
				break;
			case 113:
				fprintf(fp, "\n}\n");
				break;
			case 114:
			//put_ptr_first
				put_ptr_first(fp,ftemp);
			//print temp.val
				fprintf(fp, "%s", FileTemp);
			//put_ptr_end
				fseek(fp, 0, SEEK_END);
				fprintf(fp, "%s",ftemp);
			//put_ptr_first
				put_ptr_first(fp,ftemp);
				break;
			case 115:
			//put_ptr_first
				put_ptr_first(fp,ftemp);
			//print temp.val
				fprintf(fp, "%s", FileTemp);
			//put_ptr_end
				fseek(fp, 0, SEEK_END);
				fprintf(fp, "%s",ftemp);
			//put_ptr_first
				put_ptr_first(fp,ftemp);
			//print void
				fprintf(fp, "void");
			//put_ptr_end
				fseek(fp, 0, SEEK_END);
				fprintf(fp, "%s",ftemp);
				break;
			case 116:
			//put_ptr_first
				put_ptr_first(fp,ftemp);
			//print (
				fprintf(fp, "(");
			//put_ptr_end
				fseek(fp, 0, SEEK_END);
				fputs(ftemp,fp);
			//print )
				fprintf(fp, ")");
				break;
			case 117:  			//print assignop.val
				if(strcmp(judege,reserve_words[token[cur_pointer].id])==0)
				{
					fprintf(fp, "=");
				}
				else
				{
					fprintf(fp, " %s ",reserve_words[token[cur_pointer].id]);
				}	
				
				break;
			case 118:
				fprintf(fp, "if(");
				judege_if = 1;//证明前方有If出现
				break;
			case 119:
			    fprintf(fp, "else\n{\n");
				break;
			case 120:
				fi=0;//fi存储符号个数
				break;
			case 121:
				fprintf(fp, ")");
				break;
			case 122:
				fj=0;//fj存储符号个数
				break;
			case 123:
				fprintf(fp, "while(");
				break;
			case 124:
				fprintf(fp, ")\n{\n");
			case 125:
			    fprintf(fp, "\n}");
				break;
			case 126:
			    fprintf(fp, "%s[",idlist[token[cur_pointer].id].name);
				finpos += strlen(idlist[token[cur_pointer].id].name)+1;
				break;
			case 127:
				fprintf(fp, "]");
				break;
			case 128:
				fprintf(fp, "(");
				break;
			case 129:
				fprintf(fp, "%s(",idlist[token[cur_pointer].id].name);
				break;
			case 130:
			    fprintf(fp, "%s",numlist[token[cur_pointer].id].value);
				break;
			case 131:
				//put_ptr_first
				put_ptr_first(fp,ftemp);
				//print !(
				fprintf(fp, "!(");
				//put_ptr_end
				fseek(fp, 0, SEEK_END);
				fputs(ftemp,fp);
				//print )
				fprintf(fp, ")");
				break;
			case 132:
				fprintf(fp, "ture");
				break;
			case 133:
				fprintf(fp, "false");
				break;
			case 134:
				fprintf(fp, "+");
				break;
			case 135:
				fprintf(fp, "-");
				break;
			case 136:
				fprintf(fp, ")\n");
				break;
			case 137:
				if(strcmp("=",reserve_words[token[cur_pointer].id])==0)
				{
					if(judege_if == 1)
					{
						fprintf(fp, "==(");
						judege_if = 0;
					}
					else
					{
						fprintf(fp, " %s (",reserve_words[token[cur_pointer].id]);
					}

				}
				else if(strcmp(":=",reserve_words[token[cur_pointer].id])==0)//如果符号是:=
				{
					put_ptr_first(fp,type);
			    	if(strcmp(type,fun_name)==0)//如果：＝前是本函数的函数名，则打印return
			    	{
			    		fprintf(fp, "return (");
			    	}
			    	else
			    	{
<<<<<<< HEAD
			    		fputs(type,fp);///
=======
>>>>>>> 8bacc4d3d8db95744841722753f2acb369722d9e
			    		fprintf(fp, "=(");
			    	}	
				}
			    else if(strcmp("mod",reserve_words[token[cur_pointer].id])==0)
				{
					fprintf(fp, "%%(");
				}
				else
				{
					fprintf(fp, " %s (",reserve_words[token[cur_pointer].id]);
				}	
				break;
			case 138:
				fprintf(fp, ";\n}");
				break;
			case 139:
				fprintf(fp, ");\n");
				break;
			case 140:
				put_ptr_first(fp,type);
				fputs(type,fp);
				fputs(ftemp,fp);
			    fseek(fp,-finpos, SEEK_CUR);//回到行首
			    put_ptr_first(fp,ftemp);
			   // fputs(ftemp,fp);
			    for(fi=0;fi<strlen(ftemp);fi++)//处理此行数据，加int
			    {
			    	if(ftemp[fi]==',')
			    	{
			    		ju_temp = ++fi;
			    		for(fj=strlen(ftemp)-1;fj>=ju_temp;fj--)
			    		{
			    			ftemp[fj+strlen(type)]=ftemp[fj];//逗号后，数组下标统一向后移
			    		}
			    		for(fj=0;fj<strlen(type);fj++)
			    		{
			    			ftemp[ju_temp]=type[fj];
			    			ju_temp++;
			    		}
			    	}
			    	ju_temp=0;
			    }
			    fputs(ftemp,fp);
				break;
			case 141:
				put_ptr_first(fp,type);
			    fprintf(fp, "scanf(\"");
			    for(;fi>0;fi--)//fi表示scanf后跟的符号个数
			    {
			    	fprintf(fp, "%%d" );
			    }
			    for(fi=0;fi<strlen(type);fi++)
			    {
			    	if(type[fi] == ',' || fi== 0 )
			    	{
			    		ju_temp = ++fi;
			    		if(fi == 1)
			    			ju_temp--;
			    		for(fj=strlen(type)-1;fj>=ju_temp;fj--)
			    		{
			    			type[fj+1]=type[fj];//数组下标后移
			    		}
			    		type[ju_temp] = '&';
			    	}
			    }
			    fprintf(fp, "\",%s);\n",type);
				break;
			case 142:
				put_ptr_first(fp,type);
			    fprintf(fp, "printf(\"");
			    for(;fj>0;fj--)//fj表示printf后跟的表达式个数
			    {
			    	fprintf(fp, "%%d" );
			    }
			    fprintf(fp, "\",%s);\n",type);
				break;
			case 143:
				fj++;
				break;
			case 144:
				fprintf(fp, ")");
				fj--;
				break;
			case 145:
				strcpy(fun_name,FileTemp);
				fprintf(fp, "\n{\n" );
				break;

			case 200:
				 put_ptr_first(fp,ftemp);

				break;
			case 201:
				fseek(fp, 0, SEEK_END);//put_ptr_end
				fputs(ftemp,fp);
				break;
			case 202:
				strcpy(FileTemp,idlist[token[cur_pointer].id].name);//temp.val=id.val
				break;

			default:
				// printf("ERROR: no such meaning: %d\n", meaning);
				// exit(1);
				break;
		}
	}while(1);
}

// 由于顺序打印出来的文件有些错乱，此处要分几个文件输出，最后合并文件
void MergeFile()
{
	FILE * fp1,*fp2,*fp3, *fp;
<<<<<<< HEAD
	int i,j,k,l,ju,ju_temp=0;//j,k存储子函数开始和结束的行数，l记录花括号
	char temp[100];

	if((fp1=fopen("target.c","r+"))==NULL)
		printf("open target.c error\n");                  //计算函数中有多少个子函数
	while(fgets(temp,100,fp1) != NULL)
	{
		if( ( strstr(temp,"int") || strstr(temp,"void") || strstr(temp,"float") ) && strstr(temp,"(") && strstr(temp,")") && !strstr(temp,"main") )
			ju_temp++;
	}
	fclose(fp1);

	while(ju_temp>0)
	{
		ju_temp--;
	if((fp1=fopen("target.c","r+"))==NULL)
		printf("open target.c error\n");
	for(i=0,k=0,l=0,j=0;fgets(temp,100,fp1) != NULL;)//
	{
		i++;
		if( ( strstr(temp,"int") || strstr(temp,"void") || strstr(temp,"float") ) && strstr(temp,"(") && strstr(temp,")") && !strstr(temp,"main") )//如此行有子函数声明
		{
			j=i;//记录声明行数
			while(fgets(temp,100,fp1) != NULL)
			{
				i++;
				if(strstr(temp,"{"))
				{
					l++;
					ju=1;
				}
					
				if(strstr(temp,"}"))
					l--;
				if(l == 0 && ju==1)
				{
					k=i;
					break;
				}
			}	
		}
	}

	fclose(fp1);
=======
	int i=0;
	char temp[100];
>>>>>>> 8bacc4d3d8db95744841722753f2acb369722d9e
	if((fp1=fopen("target.c","r+"))==NULL)
		printf("open target.c error\n");
	if((fp2=fopen("temp.c","w+"))==NULL)
		printf("open temp.c error\n");
	if((fp3=fopen("temp2.c","w+"))==NULL)
		printf("open temp2.c error\n");
	fseek(fp1,0,SEEK_SET);
<<<<<<< HEAD
	i=0;
	while(fgets(temp,100,fp1) != NULL)//获取文件行数
	{
		i++;
		if(i == 1 || ( (i>=j) && (i<=k) ) )
=======
	while(fgets(temp,100,fp1) != NULL)//获取文件行数
	{
		i++;
		if(i == 1 || ( (i>=13) && (i<=29) ) )
>>>>>>> 8bacc4d3d8db95744841722753f2acb369722d9e
			fputs(temp,fp2);
		else
			fputs(temp,fp3);
	}
	fclose(fp1);
	if((fp1=fopen("target_temp.c","w+"))==NULL)//关闭再打开 清空
		printf("open target_temp.c error\n");
	fseek(fp2,0,SEEK_SET);
	fseek(fp3,0,SEEK_SET);
	while(fgets(temp,100,fp2) != NULL)
	{

		fputs(temp,fp1);
	}
	while(fgets(temp,100,fp3) != NULL)
	{

		fputs(temp,fp1);
	}

	// 删除多余的空行：@xiaofeng:
	if((fp=fopen("target.c","w+"))==NULL)//关闭再打开 清空
		printf("open target.c error\n");
	fseek(fp1, 0, SEEK_SET);
	while(fgets(temp, 100, fp1) != NULL)
		if(strcmp(temp, "\n") != 0){
			fputs(temp, fp);
		}

	fclose(fp);
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	system("rm -rf temp.c temp2.c target_temp.c");
<<<<<<< HEAD

	}

=======
>>>>>>> 8bacc4d3d8db95744841722753f2acb369722d9e
}

int main(int argc, char* argv[]){
	// 词法分析
	if(argc<1){
		printf("Usage: ./a.out [sourcefile]\n");
		exit(0);
	}
	LexAnalysis(argv[1]);

	// 处理输入
	create_p_set();
	goto_action();
	lexi_input();
	LR();
	MergeFile();

	return 0;
}
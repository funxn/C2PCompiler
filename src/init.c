/*************************************************************************
    > File Name: init.c
    > Author: xiaofeng
    > Mail: xxf263@gmail.com 
    > Created Time: Wednesday, May 04, 2016 PM02:43:05 CST
	> 任务：完成第一遍扫描，1.去掉注释
							2.将出现的大写字母变成小写
 ************************************************************************/

#include"compiler.h"

int init(char* str){
	FILE *source;				// source: the C source file
	if((source = fopen(str, "r")) == NULL){
		printf("can't open file %s!", str);
		exit(0);
	}
	FILE *target;				// source: the C source file
	if((target = fopen("example/target.pas", "w")) == NULL){
		printf("can't open file target.c!");
		exit(0);
	}
	
	char ch;
	// 统计行号
	int thisline = 1;
	while((ch=fgetc(source)) != EOF){
		if(ch == '{'){
			while((ch=fgetc(source)) != '}'){
				if(ch == '\n')
					thisline++;
				else if(ch == EOF){
					printf("error: unclosed comment! Line: %d\n", thisline);
					exit(1);
				}
			}
			continue;
		}
		// 将所有大写字母转为小写字母
		if(ch>=65 && ch<=90){
			ch += 32;
		}
		if(ch == '\n')
			thisline++;
		fputc(ch, target);
	}
}

int main(int argc, char* argv[]){
	if(argc<=1){
		printf("usage: ./a.out [sourcefile]\n");
		exit(0);
	}

	init(argv[1]);
}




 /*************************************************************************
    > File Name: yufa_func.c
    > Author: xiaofeng
    > Mail: xxf263@gmail.com 
    > Created Time: Thursday, May 05, 2016 AM09:49:09 CST
 ************************************************************************/

#include"compiler.h"

// 为LR1编写的closure函数
void closure_func(PROJECT_SET* p_set, int next_m){
	int i;
	PROJECT new_proj;

	for(i=0; i<p_set->len; i++){
		p_set->proj[i].pos++;
	}
}

// void go_func(){

// }

void action_func(){

}

void goto_func(){

}

// 判断两个项目是否相等
int proj_equal(PROJECT* proj1, PROJECT* proj2){
	int i;
	if(proj2->pos != proj1->pos)
		return 0;

	for(i=0; i<TAIL_LEN && proj2->tail[i]!=0 && proj1->tail[i]!=0; i++){
		if(proj1->tail[i] != proj2->tail[i])
			return 0;
	}


	for(i=0; i<OP_LEN && proj2->thisOp[i]!=0 && proj1->thisOp[i]!=0; i++){
		if(proj1->thisOp[i] != proj2->thisOp[i])
			return 0;
	}

	return 1;
}

// 将项目插入项目集
void projset_add(PROJECT_SET* p_set, PROJECT* p){
	int i;

	p_set->proj[len].pos = p->pos;
	for(i=0; i<OP_LEN && p->thisOp[i]!=0; i++){
		p_set->proj[len].thisOp[i] = p->thisOp[i];
	}
	for(i=0; i<TAIL_LEN && p->tail[i]!=0; i++){
		p_set->proj[len].tail[i] = p->tail[i];
	}
}
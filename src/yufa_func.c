 /*************************************************************************
    > File Name: yufa_func.c
    > Author: xiaofeng
    > Mail: xxf263@gmail.com 
    > Created Time: Thursday, May 05, 2016 AM09:49:09 CST
 ************************************************************************/

#include "compiler.h"


// 为LR1编写的closure函数
int closure_func(PROJECT_SET* p_set, int *flag_new_add){
	int i, j, k;
	PROJECT new_proj;
	PROJECT this_proj;
	int thisOp[OP_LEN];
	int singleOp[OP_LEN];						// 当前待分析的单个的Op，不含-1分割符
	int singleTail[OP_LEN];						// 对应的tail[]
	int index;									// 调用First()函数要用的指标
	int fiSet[OP_LEN];							// First集

	PROJECT_SET* p_set_new = (PROJECT_SET *)malloc(sizeof(PROJECT_SET));
	*p_set_new = *p_set; 						// p_set 赋值

	// 对p_set中每个项目求闭包
	int item_new_add;							// 控制当有未加入closure集中的项目时，继续扫描
	do{
	item_new_add = 0;
	for(i=0; i<p_set_new->len; i++){
		this_proj = p_set_new->proj[i];
		// thisOp指向B->n形式的所有产生式
		memcpy(thisOp, genOp[this_proj.thisOp[this_proj.pos]], sizeof(genOp[this_proj.thisOp[this_proj.pos]]));	
		for(j=0; j<OP_LEN && thisOp[j]!=0; j++){
			k = 0;
			memset(singleOp, 0, OP_LEN*sizeof(int));
			memset(singleTail, 0, TAIL_LEN*sizeof(int));

			if(thisOp[j] == -1){
				// 构建一个新的项目new_proj;
				// 获取singleOp[]
				singleOp[k++] = thisOp[0];		// 获取的singleOp无需-1分割
				for(j=j+1; thisOp[j]!=-1 && thisOp[j]!=0; j++)
					singleOp[k++] = thisOp[j];
				memcpy(new_proj.thisOp, singleOp, sizeof(singleOp));
				// 获取singleTail[]
				if(this_proj.thisOp[this_proj.pos+1]==0){
					memcpy(singleTail, this_proj.tail, sizeof(this_proj.tail));
				}else{
					int flag = 1;							// 含E_NUM的标识
					int add_e_flag = 0;						// 含空标识，要把小尾巴加入下一个项目
					int h, g=0;
					index = 0;
					int new_pos = this_proj.pos+1;					// 找出小尾巴first集

					while(flag == 1 && this_proj.thisOp[new_pos]!=0){
						flag = 0;
						memset(fiSet, 0, OP_LEN*sizeof(int));
						First(this_proj.thisOp[new_pos], &index, fiSet);
						for(h=0; h<OP_LEN && fiSet[h]!=0; h++){
							if(fiSet[h] == E_NUM){
								add_e_flag = 1;
								flag = 1;
								continue;						// 所以此处用了continue
							}
							singleTail[g++] = fiSet[h];
						}
						new_pos++;								// 当beta中第一个的first集含空时，要看下一个
					}
					if(add_e_flag)
						memcpy(singleTail+g, this_proj.tail, sizeof(this_proj.tail));
					// print singleTail!! to check!!!
				}
				memcpy(new_proj.tail, singleTail, sizeof(singleTail));
				// 获取pos
				new_proj.pos = 1;

				// 添加项目
				if(!proj_include(p_set_new, &new_proj)){
					projset_add(p_set_new, &new_proj);
					item_new_add = 1;
					p_set_new->len++;
				}

				j--;							// 抵消两个j++的效果

			}
		}
	}
	}while(item_new_add);

	// 是否将项目集加入项目集规范族？
	for(i=0; i<PS_CUR_NUM; i++){
		if(projset_equal(&PS[i], p_set_new))
			return i;
	}

	// 若该项目是新项目， 置flag_new_add为1;
	*flag_new_add = 1;
	PS_CUR_NUM++;								// 当前项目集个数加一
	PS[i] = *p_set_new;
	PS[i].index = i;
	return i;
}

// 构建一个新的项目集存储go一步之后的待closure分析的项目集合
int go_func(int ps_index, int m, int* flag_new_add){
	int i;
	PROJECT_SET p_set_new = {0};

	// 若pos指向结尾，直接结束本循环
	if(m == 0)
		return -1;

	for(i=0; i<PS[ps_index].len; i++){
		if(PS[ps_index].proj[i].thisOp[PS[ps_index].proj[i].pos] == m){
			PS[ps_index].proj[i].pos++;
			p_set_new.proj[p_set_new.len++] = PS[ps_index].proj[i];
			PS[ps_index].proj[i].pos--;
		}
	}

	return closure_func(&p_set_new, flag_new_add);
}

// 构造LR(1)项目集规范族
void create_p_set(){
	int flag_new_add; 
	int i, j, k;
	int m_array[30]; 			// 保存当前分析项目中的所有待分析文法符号
	int m_array_len;

	int temp_go;

	// 初始项目设置
	// {S' -> S, $}
	PS[0].len = 1;
	PS[0].index = 0;
	PS[0].proj[0].thisOp[0] = STA_NUM;
	PS[0].proj[0].thisOp[1] = 1;
	PS[0].proj[0].tail[0] = END_NUM;
	PS[0].proj[0].pos = 1;
	PS_CUR_NUM++;

	closure_func(&(PS[0]), &flag_new_add);							// I0项目集标记为PS[i]

	// 当有新项目集加入规范族时，循环继续
	do{
		flag_new_add = 0;
		for(i=0; i<PS_CUR_NUM; i++){
			memset(m_array, 0, 30*sizeof(int));
			m_array_len = 0;
			for(j=0; j<PS[i].len; j++){
				int flag = 0;
				for(k=0; k<m_array_len; k++)
					if(PS[i].proj[j].thisOp[PS[i].proj[j].pos] == m_array[k])
						flag = 1;
				if(!flag){
					m_array[m_array_len] = PS[i].proj[j].thisOp[PS[i].proj[j].pos];
					m_array_len++;
				}
			}
			for(k=0; k<m_array_len; k++)
				go_func(i, m_array[k], &flag_new_add);
		}
	}while(flag_new_add);

	printf("PS_NUM: %d\n\n", PS_CUR_NUM);
}

// goto, action的生成：
void goto_action(){
	int i, j, k, h;					
	int searched_m[VAR_NUM];					// 记录每一次对项目集处理时已经处理过的符号
	int searched_flag;
	int this_m;									// 待传入go_func()的符号
	int flag_new_add;
	// 对每一个项目集
	for(i=0; i<PS_CUR_NUM; i++){
		k=0;
		memset(searched_m, 0, VAR_NUM*sizeof(int));
		//对每一个项目集中的项目
		for(j=0; j<PS[i].len; j++){
			this_m = PS[i].proj[j].thisOp[PS[i].proj[j].pos];

			// ACC完成
			if(this_m == 0 && PS[i].proj[j].tail[0] == END_NUM && PS[i].proj[j].thisOp[0] == STA_NUM){
				act[i][PS[i].proj[j].tail[0]].rs = ACC;
				act[i][PS[i].proj[j].tail[0]].no[0] = 0;
			}
			// R回退
			//else if((this_m == 0 && PS[i].proj[j].tail[0] == END_NUM) || this_m == E_NUM){
			else if(this_m == 0 || this_m == E_NUM){
				int tail_i = 0;
				for(tail_i=0; PS[i].proj[j].tail[tail_i]!=0; tail_i++){
				//for(tail_i=VN_NUM+1; tail_i<=VAR_NUM; tail_i++){
					act[i][PS[i].proj[j].tail[tail_i]].rs = R;
					// 若是回退项目，直接把产生式赋值给no
					memcpy(act[i][PS[i].proj[j].tail[tail_i]].no, PS[i].proj[j].thisOp, sizeof(act[i][PS[i].proj[j].tail[tail_i]].no));
				}
			}
			else{
				searched_flag = 0;
				for(h=0; h<k; h++){
					if(this_m == searched_m[h]){
						searched_flag = 1;
						break;
					}
				}

				// 若PS[i]项目集中pos位置中有符号未处理
				if(searched_flag != 1){
					searched_m[k++] = this_m;
					// 添加到GOTO表
					if(this_m <= VN_NUM)
						gto[i][this_m] = go_func(i, this_m, &flag_new_add);
					else{
						act[i][this_m].rs = S;
						act[i][this_m].no[0] = go_func(i, this_m, &flag_new_add);
					}
				}
			}
		}
	}
}

// 判断两个项目是否相等
int proj_equal(PROJECT* proj1, PROJECT* proj2){
	int i;
	if(proj2->pos != proj1->pos)
		return 0;

//	for(i=0; i<TAIL_LEN && proj2->tail[i]!=0 && proj1->tail[i]!=0; i++){
	for(i=0; i<TAIL_LEN; i++){
		if(proj2->tail[i]!=0 && proj1->tail[i]!=0){
			if(proj1->tail[i] != proj2->tail[i])
				return 0;
		}else if(proj2->tail[i]==0 && proj1->tail[i]==0){
			break;
		}else{
			return 0;
		}
			
	}


//	for(i=0; i<OP_LEN && proj2->thisOp[i]!=0 && proj1->thisOp[i]!=0; i++){
	for(i=0; i<OP_LEN; i++){
		if(proj2->thisOp[i]!=0 && proj1->thisOp[i]!=0){
			if(proj1->thisOp[i] != proj2->thisOp[i])
				return 0;
		}else if(proj2->thisOp[i]==0 && proj1->thisOp[i]==0){
			break;
		}else{
			return 0;
		}
	}

	return 1;
}

// 判断项目是否包含在项目集里面
int proj_include(PROJECT_SET* p_set, PROJECT* p){
	int i;
	for(i=0; i<p_set->len; i++){
		if(proj_equal(&(p_set->proj[i]), p))
			return 1;
	}
	return 0;
}


// 将项目插入项目集
void projset_add(PROJECT_SET* p_set, PROJECT* p){
	int i;

	p_set->proj[p_set->len].pos = p->pos;
	for(i=0; i<OP_LEN && p->thisOp[i]!=0; i++){
		p_set->proj[p_set->len].thisOp[i] = p->thisOp[i];
	}
	for(i=0; i<TAIL_LEN && p->tail[i]!=0; i++){
		p_set->proj[p_set->len].tail[i] = p->tail[i];
	}
}

// 项目集相等的判断
int projset_equal(PROJECT_SET* p_set1, PROJECT_SET* p_set2){
	int i;
	if(p_set1->len != p_set2->len)
		return 0;
	for(i=0; i<p_set1->len; i++){
		if(!proj_equal(&(p_set1->proj[i]), &(p_set2->proj[i])))
			return 0;
	}

	return 1;
}


// int main(){
// 	int i, j;
// 	create_p_set();
// 	goto_action();

// 	// 打印
// 	for(i=0; i<10; i++)
// 		for(j=0; j<VN_NUM; j++)
// 			printf("goto[%d][%d] = %d\n", i, j, gto[i][j]);
// 	for(i=0; i<10; i++)
// 		for(j=0; j<VT_NUM; j++)
// 			printf("action[%d][%d].no = %d\n", i, j, act[i][j].no);
// 	return 0;
// }
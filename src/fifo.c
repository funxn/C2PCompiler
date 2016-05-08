/*************************************************************************
    > File Name: fifl.c --> First/Follow set generator
    > Author: xiaofeng
    > Mail: xxf263@gmail.com 
    > Created Time: Wednesday, May 04, 2016 PM02:44:45 CST
 ************************************************************************/

#include"compiler.h"

// 递归, m>0由调用者进行判断
void First(int m, int* i, int* set){					// m: mark 符号, i: 将要添加的元素位置, set: 要保存First集的数组
	// 遍历VN的所有产生式（递归）
	int j;
	for(j=1; genOp[m][j]!=0; j++){
		if(genOp[m][j]==-1){
			if(genOp[m][j+1] > VN_NUM)
				set[(*i)++] = genOp[m][j+1];
			else
				First(genOp[m][j+1], i, set);
		}
	}
}

void Follow(int m, int* set){
	int fiSet[OP_LEN];
	int thisOp[OP_LEN];
	int i = 0;								// i永远指向要返回的Follow集的带插入位置
	int j, k, h, g;
	int fg = 0; 							// 用于判定set[]集合里面是否已存在待插入元素, 或是否为E_NUM
	
	int index;

	if(m == 1){
		set[i++] = -2;
	}
	for(j=1; j<=VN_NUM; j++){
		memcpy(thisOp, genOp[j], sizeof(thisOp));
		for(k=2; thisOp[k]!=0; k++){
			if(thisOp[k] == m){
				memset(fiSet, 0, OP_LEN*sizeof(int));
				index = 0;
				if(thisOp[k+1] == 0 || thisOp[k+1] == -1){
					First(thisOp[0], &index, fiSet);
					for(h=0; h<OP_LEN&&fiSet[h]!=0; h++){
						fg = 0;
						for(g=0; g<i; g++){									// 测试是否能插入set[]集合的功能块
							if(set[g] == fiSet[h] || fiSet[h] == E_NUM){
								fg = 1;
								break;
							}
						}
						if(fg == 0)
							set[i++] = fiSet[h];
					}
				}else{
					int flag = 0;											// 测试是否为E_NUM
					First(thisOp[k+1], &index, fiSet);
					// 还要把First集中所有非‘&’的加入Follow集里面
					for(h=0; h<OP_LEN && fiSet[h]!=0; h++){
						fg = 0;
						if(fiSet[h] == E_NUM){
							flag = 1;
							continue;						// 所以此处用了continue
						}
						for(g=0; g<i; g++){
							if(set[g] == fiSet[h]){
								fg = 1;
								break;
							}
						}
						if(fg == 0)
							set[i++] = fiSet[h];
					}
					index = 0;
					memset(fiSet, 0, OP_LEN*sizeof(int));

					if(flag == 1){
						First(thisOp[0], &index, fiSet);
						for(h=0; h<OP_LEN&&fiSet[h]!=0; h++){
							fg = 0;
							for(g=0; g<i; g++){
								if(set[g] == fiSet[h] || fiSet[h] == E_NUM){
									fg = 1;
									break;
								}
							}
							if(fg == 0)
								set[i++] = fiSet[h];
						}
					}
				}

			}
		}
	}
}


// 测试：
int main(int argc, char* argv[]){
	int i,j;
	int set[OP_LEN];
	int index = 0;								// 存放First集的指标
	for(i=1; i<=VN_NUM; i++){
		memset(set, 0, OP_LEN*sizeof(int));
		index = 0;
		First(genOp[i][0], &index, set);
		printf("%d First: ", genOp[i][0]);
		for(j=0; j<OP_LEN&&set[j]!=0; j++){
			printf("%d, ", set[j]);
		}
		printf("\n");
	}
	for(i=1; i<=VN_NUM; i++){
		memset(set, 0, OP_LEN*sizeof(int));
		index = 0;
		Follow(genOp[i][0], set);
		printf("%d Follow: ", genOp[i][0]);
		for(j=0; j<OP_LEN&&set[j]!=0; j++){
			printf("%d, ", set[j]);
		}
		printf("\n");
	}

	return 0;
}
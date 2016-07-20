/*************************************************************************
    > File Name: fifl.c --> First/Follow set generator
    > Author: xiaofeng
    > Mail: xxf263@gmail.com 
    > Created Time: Wednesday, May 04, 2016 PM02:44:45 CST
 ************************************************************************/

#include "compiler.h"


//!!!
//!!!一定注意初始化set所有元素为0


void AddSet(int *set , int ele)//将元素加入First
{
    int i,judge=0;
    for(i=0;i<OP_LEN;i++)
    {
        if(set[i]==ele)//如其中已有此元素
        {
            judge=1;
            break;
        }
        if(set[i]==0)//递归到了数组尾
            break;
    }
    if(judge==0)
        set[i]=ele;
}

void First(int m , int *x ,int* set)// m: mark 符号, i: 将要添加的元素位置, set: 要保存First集的数组
{
    int i,j,*n,judge=0;
    int temp[OP_LEN];
    for(i=0;i<OP_LEN;i++)
        temp[i]=0;
    if(m>VN_NUM)
    {
        set[0]=m;
        return;
    }
    else
    {
        for(i=1;i<=GENOP_NUM;i++)
        {
            if(genOp[m][i]==0)
                break;
            if(genOp[m][i]==-1)//如遇到产生式
            {
                i++;
                if(genOp[m][i]>VN_NUM)//如产生式首是终结符，加入First集
                    AddSet(set, genOp[m][i]);
                if(genOp[m][i]<VN_NUM)//如产生式首是非终结符
                {
                    First(genOp[m][i],n,temp);//计算首个非终结符的First
                    for(j=0;j<OP_LEN;j++)//FIRST(X)=FIRST(X)+FIRST（Y）
                    {
                        if(temp[j]==E_NUM)//除去空元素
                        {
                            j++;
                            judge=1;//表明存在空元素
                        }
                        
                        else if(temp[j]==0)
                            break;
                        else
                            AddSet(set, temp[j]);
                    }
                    while(judge==1)//若首个非终结符的FIRST集中有空
                    {
                        i++;
                        if(genOp[m][i]==-1)
                        {
                            AddSet(set,E_NUM);//将空元素加入First集
                            break;
                        }
                        else
                        {
                            First(genOp[m][i],n,temp);
                            for(j=0;j<OP_LEN;j++)
                                if(temp[j]==E_NUM)//如有空元素
                                    break;
                            if(j==OP_LEN)//无空元素
                            {
                                judge=0;
                                for(j=0;j<OP_LEN;j++)//FIRST(X)=FIRST(X)+FIRST（Y）
                                {
                                    if(temp[j]==0)
                                        break;
                                    else
                                        AddSet(set, temp[j]);
                                }

                            }
                        }
                         
                    }
                }
            }
        }
    }
    
}



// 发现E_NUM对应的表达式头
/*void GET_E_SET(int m, int* i, int* set){					// m: mark 符号, i: 将要添加的元素位置, set: 要保存First集的数组
	// 遍历VN的所有产生式（递归）
	int j;
	int k;
	int flag = 0;

	if(m>VN_NUM){
		set[*i] = m;
		return ;
	}

	for(j=1; genOp[m][j]!=0; j++){
		if(genOp[m][j]==-1){
			if(genOp[m][j+1] == E_NUM){
				set[(*i)++] = genOp[m][0];
			}
				
			else
				GET_E_SET(genOp[m][j+1], i, set);
		}
	}
}
 */


void Follow(int m, int* set)
{
    int i,j,k,l,n,temp[OP_LEN],*x,judge=0;//i,j,k,l,n循环变量，temp临时存储数组,*x为满足函数参量要求无意义，judge判断是否有空
    if(m==1)
        set[0]=END_NUM;
    for(i=1;i<=VN_NUM;i++)
        for(j=1;j<=OP_LEN;j++)
        {
            if(genOp[i][j]==0)
                break;
            if(genOp[i][j]==m)//若找到此符号
            {
                j++;
                if(genOp[i][j]!=-1 && genOp[i][j]!=0)//若此符号后仍有符号   A->aBp
                {
                    for(l=0;l<OP_LEN;l++)//temp清零
                        temp[l]=0;
                    First(genOp[i][j], x, temp);//计算此后符号的FIRST集
                    for(k=0;k<OP_LEN && temp[k]!=0;k++)//将此后符号的FIRST集的非空元素加入FOLLOW集
                    {
                        if(temp[k]==E_NUM)
                        {
                            judge=1;
                            k++;
                        }
                        else
                            AddSet(set, temp[k]);
                    }
                    if(judge==1)//若p的FIRST集中有空FOLLOW(M)=FOLLOW(P)
                    {
                        for(l=j;l<OP_LEN && genOp[i][l]!=0 && genOp[i][l]!=-1;l++)//判断是否后续所有元素FIRST集都有空
                        {
                          for(k=0;k<OP_LEN;k++)//temp清零
                                temp[l]=0;
                            First(genOp[i][l], x, temp);
                          for(n=0;n<OP_LEN && temp[n]!=0;n++)
                              if(temp[n]==E_NUM)
                                  judge=0;
                        }
                        if(judge==0)
                        {
                            for(l=0;l<OP_LEN;l++)//temp清零
                                temp[l]=0;
                            Follow(genOp[i][0], temp);
                            for(k=0;k<OP_LEN && temp[k]!=0;k++)
                                AddSet(set, temp[k]);
                        }
                    }
                }
                else if (genOp[i][j]==-1 || genOp[i][j]==0)//若此后没有符号 A->aB
                {
                    if(genOp[i][0] != m)
                    {
                        for(l=0;l<OP_LEN;l++)//temp清零
                            temp[l]=0;
                        Follow(genOp[i][0], temp);
                        for(k=0;k<OP_LEN && temp[k]!=0;k++)
                            AddSet(set, temp[k]);
                    }
                }
            }
        }
}



 // int main(int argc, char* argv[]){

	
 //    int i,j;
 // 	int set[OP_LEN];						// 存放First集的指标
 //     for(i=0;i<OP_LEN;i++)
 //         set[i]=0;
 // 	int index = 0;
 // 	for(i=1; i<=VN_NUM; i++){
	// 	memset(set, 0, OP_LEN*sizeof(int));
 // 		index = 0;
 // 		First(genOp[i][0], &index, set);
 // 		printf("%d First: ", genOp[i][0]);
 // 		for(j=0; j<OP_LEN&&set[j]!=0; j++){
 // 			printf("%d, ", set[j]);
 // 		}
 // 		printf("\n");
 // 	}
     
 //     for(i=0;i<OP_LEN;i++)
 //        set[i]=0;
     

 // 	for(i=1; i<=VN_NUM; i++){
 // 		memset(set, 0, OP_LEN*sizeof(int));
 // 		index = 0;
 // 		Follow(genOp[i][0], set);
 // 		printf("%d Follow: ", genOp[i][0]);
 // 		for(j=0; j<OP_LEN&&set[j]!=0; j++){
 // 			printf("%d, ", set[j]);
 // 		}
 // 		printf("\n");
 // 	}

 // 	return 0;
 // }
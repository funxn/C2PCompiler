/*
 * 维护重要的表结构，供词法分析器/语法分析器调用
 */


#include"const.h"



// #ifndef	DS_STRUCT
// #define	DS_STRUCT

// 关键字结构定义
typedef struct{
	char name[NAME_LEN];	// 关键字名
	int keyID;				// 要返回的ID值
} KEYWORD;

// id表
typedef struct{
	int entryID;					// id表入口
	char name[NAME_LEN];			// 标识符名
} IDLIST;							// 保留为结构体以便后续修改

// num表
typedef struct{
	int numID;						// num表入口
	char value[VALUE_LEN];			// num的值
} NUMLIST;




// 符号表结构
typedef struct SymbolNode{
	int type;		// 类型
	int addr;		// 地址
	struct SymbolNode* next;
}* SN;

// token[]
typedef struct{
	int sign;					// the value in 产生式符号对照表.conf
	char value[NAME_LEN];		
	int id;						// id表 或 num表入口
} TOKEN;


// 项目结构体
typedef struct PROJ{
	int thisOp[OP_LEN];
	int tail[TAIL_LEN];
	int pos;		// 指向将要进行分析的那一个符号
} PROJECT;

// 项目集规范族结构
typedef struct PROJ_SET{
	PROJECT proj[PROJ_NUM];			// 具体的项目
	int len;						// 项目个数
	int index;						// 项目集规范族的编号
} PROJECT_SET;



// action数组
typedef struct{
	int rs;
	int no[OP_LEN];					// 为了R回退时能找到特定的产生式，这里将no修改！
}act_block;


typedef struct TNODE{
	int no;
	int is_act;						// 记录是否为要执行的动作
	int act_no;						// 要执行动作的编号
	int childList[CHILD_LIST_LEN];
} TREENODE;

typedef int ElemType;
typedef int Status;

typedef struct SqStack{
    ElemType *base;
    ElemType *top;
    int stackSize;
}SqStack;

// #endif


extern KEYWORD keyTable[];
extern IDLIST idlist[IDLIST_LEN];
extern int IDLIST_CUR_NUM;
extern NUMLIST numlist[NUMLIST_LEN];
extern int NUMLIST_CUR_NUM;
extern char* errList[ERR_TYPES];
extern int genOp[GENOP_NUM][OP_LEN];
extern int genOp_more[GENOP_NUM][OP_LEN];
extern char* reserve_words[RESERVE_WORD_NUM];
extern char* var_list[];
extern SN symbolTable;
extern TOKEN tokenlist[TOKENLIST_LEN];
extern int TOKEN_CUR_LEN;
extern PROJECT_SET PS[PROJECT_SET_NUM];
extern int PS_CUR_NUM;
extern int gto[PROJECT_SET_NUM][VN_NUM];
extern act_block act[PROJECT_SET_NUM][VT_NUM];
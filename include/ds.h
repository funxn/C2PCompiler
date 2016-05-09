/*
 * 维护重要的表结构，供词法分析器/语法分析器调用
 */


#include"const.h"

// 关键字结构定义
typedef struct{
	char name[NAME_LEN];	// 关键字名
	int keyID;				// 要返回的ID值
} KEYWORD;
KEYWORD keyTable[] = {
	{"integer", 1},		{"real", 2}, 		{"boolean", 3},
	{"if", 4}, 			{"then", 5}, 		{"else", 6},
	{"begin", 7}, 		{"end", 8}, 		{"while", 9},
	{"do", 10}, 		{"read", 11}, 		{"write", 12},
	{"array", 13}, 		{"of", 14}, 		{"record", 15},
	{"var", 16}, 		{"function", 17}, 	{"procedure", 18},
	{"program", 19}, 	{"true", 20}, 		{"false", 21}
};

// id表
typedef struct idlist{
	int entryID;					// id表入口
	char name[NAME_LEN];			// 标识符名
	struct idlist* next;
}* IDLIST;

// num表
typedef struct numlist{
	int numID;						// num表入口
	char value[VALUE_LEN];			// num的值
	struct numlist* next;
}* NUMLIST;

// 错误类型表：
char* errList[ERR_TYPES] = {
	"No num",
	"No name"
};

// 产生式定义
/*
int genOp[GENOP_NUM][OP_LEN] = {
	{0},
	{1,-1,4,74},
	{2,-1,1,27,42,3,43,45},
	{3,-1,3,46,27,-1,27},
	{4,-1,5,9,15},
	{5,-1,64,6,45,-1,75},
	{6,-1,6,45,3,44,7,-1,3,44,7},
	{7,-1,8,-1,61,47,76,74,74,76,48,62,8,-1,63,6,56,},
	{8,-1,49,-1,50,-1,51,-1,28,74,74,28},
	{9,-1,0,10,45,-1,75},
	{10,-1,11,5,15,},
	{11,-1,65,27,12,44,8,45,-1,66,27,12,45,},
	{12,-1,42,13,43,-1,75},
	{13,-1,13,45,14,-1,14},
	{14,-1,64,3,44,7,-1,3,44,7},
	{15,-1,55,16,56},
	{16,-1,17,-1,75},
	{17,-1,17,45,18,-1,18},
	{18,-1,19,35,22,-1,20,-1,15,-1,52,22,53,18,54,18,-1,57,22,58,18,-1,59,42,3,43,-1,60,21,43},
	{19,-1,27,-1,27,47,22,28},
	{20,-1,27,-1,27,42,21,43},
	{21,-1,21,46,22,-1,22},
	{22,-1,23,36,23,-1,23},
	{23,-1,23,41,24,-1,24,-1,26,24},
	{24,-1,24,39,25,-1,25},
	{25,-1,27,-1,27,42,21,43,-1,27,47,22,48,-1,28,-1,42,22,43,-1,70,25,-1,68,-1,69},
	{26,-1,29,-1,30}
};
*/
int genOp[4][OP_LEN] = {
	{0},
	{1,-1,2},
	{2,-1,3,2,-1,6},
	{3,-1,4,3,-1,5}
};

// 符号表结构
typedef struct SymbolNode{
	int type;		// 类型
	int addr;		// 地址
	struct SymbolNode* next;
}* SN;
SN symbolTable;

// token[]
typedef struct{
	char sign[NAME_LEN];
	int value;		// the value in op_define.conf
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

PROJECT_SET PS[PROJECT_SET_NUM];
int PS_CUR_NUM;						// 记录当前的项目集的个数

// goto数组
int gto[PROJECT_SET_NUM][VN_NUM];

// action数组
typedef struct{
	int rs;
	int no;
}act_block;
act_block act[PROJECT_SET_NUM][VT_NUM];
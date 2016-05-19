/*
 * const.h常量定义：
 */

#define		NAME_LEN	20
#define		VALUE_LEN	100
#define		ERR_TYPES	10

// 记录保留字数量
#define		RESERVE_WORD_NUM	50

// IDLIST, NUMLIST的长度
#define 	IDLIST_LEN 			200
#define 	NUMLIST_LEN			200

// TOKEN表的长度
#define		TOKENLIST_LEN		1024

// 记录产生是非终结符数量
#define		VAR_NUM		74
#define		VN_NUM		34
#define		VT_NUM		VAR_NUM - VN_NUM
#define		E_NUM		73						// &的编号
#define		GENOP_NUM	VN_NUM+1
#define		STA_NUM		75 						// 记录开始符号 S’ 的编号
#define		END_NUM		76 						// 记录 $ 的编号

// 供action表使用的几个常量
#define		S 			1
#define		R 			2
#define		ACC			3



// 产生式最长长度
#define		OP_LEN		50

// 项目集中最多的项目个数
#define 	PROJ_NUM	100
#define 	TAIL_LEN	50

// 项目集规范族的个数
#define		PROJECT_SET_NUM		500

// buffer size
#define 	BUF_SIZE 	1024


// 构造的语法树的孩子节点长度信息
#define		CHILD_LIST_LEN		20



/*implement a stack in c*/
#define STACK_INIT_SIZE 100             // 栈初始的大小
#define STACK_INCREMENT 10              // 每次当栈已满的时候，要增加的个数
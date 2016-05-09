/*
 * const.h常量定义：
 */

#define		NAME_LEN	20
#define		VALUE_LEN	100
#define		ERR_TYPES	10

// 记录产生是非终结符数量
#define		VAR_NUM		6
#define		VN_NUM		3
#define		VT_NUM		3
#define		GENOP_NUM	VN_NUM+1
#define		STA_NUM		x 						// 记录开始符号 S’ 的编号
#define		EMP_NUM		x 						// 记录 & 的编号
#define		END_NUM		x 						// 记录 $ 的编号

// &的编号
#define		E_NUM		6

// 产生式最长长度
#define		OP_LEN		50

// 项目集中最多的项目个数
#define 	PROJ_NUM	100
#define 	TAIL_LEN	50

// 项目集规范族的个数
#define		PROJECT_SET_NUM		500

// buffer size
#define 	BUF_SIZE 	1024


// 供action表使用的几个常量
#define		S 			0;
#define		R 			1;
#define		ACC			2;

/*
 * 可供调用的函数声明
 */

// 词法分析程序
// 返回TOKEN序列
TOKEN Scanner();

/* 语法语义分析程序, 结合翻译方案，产生目标语言
 * 返回void
 */
void Grammer();

/*
 * log() 日志处理
 */
void logRecord();

/*
 * 错误处理
 * 参数： row-行号，volumn-列号， errType-错误号（查错误表）
 */
void errPrint(int row, int volumn, int errType);


//================================================================
//				Tools_functions
//================================================================

// 语法分析下属函数
int closure_func(PROJECT_SET* p_set);
int go_func(int ps_index, int m);
void goto_action();
int proj_equal(PROJECT* proj1, PROJECT* proj2);
int proj_include(PROJECT_SET* p_set, PROJECT* p);
void projset_add(PROJECT_SET* p_set, PROJECT* p);
int projset_equal(PROJECT_SET* p_set1, PROJECT_SET* p_set2);
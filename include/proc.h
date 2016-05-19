/*
 * 可供调用的函数声明
 */

// 词法分析程序
// 返回TOKEN序列
extern TOKEN Scanner();

/* 语法语义分析程序, 结合翻译方案，产生目标语言
 * 返回void
 */
extern void Grammer();

extern void Semantic();

/*
 * log() 日志处理
 */
extern void logRecord();

/*
 * 错误处理
 * 参数： row-行号，volumn-列号， errType-错误号（查错误表）
 */
extern void errPrint(int row, int volumn, int errType);


//================================================================
//				Tools_functions
//================================================================

// 语法分析下属函数
//extern int closure_func(PROJECT_SET* p_set);
//extern int go_func(int ps_index, int m);
extern void goto_action();
extern int proj_equal(PROJECT* proj1, PROJECT* proj2);
extern int proj_include(PROJECT_SET* p_set, PROJECT* p);
extern void projset_add(PROJECT_SET* p_set, PROJECT* p);
extern int projset_equal(PROJECT_SET* p_set1, PROJECT_SET* p_set2);

extern void First(int m, int* i, int* set);


// 分析栈使用的函数
extern void print(SqStack *s);
extern SqStack* InitStack(SqStack *s);
extern Status FreeStack(SqStack *s);
extern Status Push(SqStack *s, ElemType e);
extern Status Pop(SqStack *s, ElemType *e);
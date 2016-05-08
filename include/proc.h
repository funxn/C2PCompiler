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
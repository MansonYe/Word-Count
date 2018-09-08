#include<stdio.h>
#include<string.h>
#include<assert.h>
#include<malloc.h>
#include<io.h>


int CodeCount(char *Path) {	//计算字符个数
	
	FILE *file = fopen(Path, "r");
	assert(file != NULL);	//若文件不存在则报错

	char code;
	int count = 0;

	while ((code = fgetc(file)) != EOF) {	//读取字符直到结束
		if ( (code!=' ') && (code!='\n') && (code!='\t') )	//判断是否是字符
			count++;
	}

	fclose(file);

	return count;
}

int WordCount(char *Path) {	//计算单词个数

	FILE *file = fopen(Path, "r");
	assert(file != NULL);

	char word;
	int is_word = 0;	//用于记录字符是否处于单词中
	int count = 0;

	while ((word = fgetc(file)) != EOF) {
		if ((word >= 'a' && word <= 'z') || (word >= 'A' && word <= 'Z')) {	//判断是否是字母
			if (is_word == 0)	//若该字母处在单词首
				count++;
			is_word = 1;	//记录单词状态
		}
		else 
			is_word = 0;	//记录不处于单词状态
	}

	fclose(file);

	return count;
}

int LineCount(char *Path) {	//计算行数

	FILE *file = fopen(Path, "r");
	assert(file != NULL);

	char *s = (char*)malloc(100 * sizeof(char));//申请空间
	int count = 0;

	while (fgets(s, 100, file) != NULL)	//逐次取文件中的行
		count++;

	free(s);
	fclose(file);

	return count;
}

void AllDetail(char *Path) {
	
}

void Scan(char *Path, long Temp, struct _finddata_t FileData, char Type) {//通过递归实现目录下文件遍历

	char *Address = (char*)malloc(100 * sizeof(char));//申请空间储存文件地址
	char *p = Address;
	int i;

	for (i = 0; *(Path + i) != '*'; i++)//把遇到'*'前的路径保存
		*(p++) = *(Path + i);

	for (i = 0; *(FileData.name + i) != '\0'; i++)//把文件名插入到路径后
		*(p++) = *(FileData.name + i);

	*(p++) = '\0';//字符串结束标志

	printf("%s ", FileData.name);
	switch (Type) {//根据功能选择输出
	case 'c': printf("code count: %d\n", CodeCount(Address)); break;
	case 'w': printf("word count: %d\n", WordCount(Address)); break;
	case 'l': printf("line count: %d\n", LineCount(Address)); break;
	default: break;
	}

	free(Address);

	if (_findnext(Temp, &FileData) == 0)//以下一个文件能被读取为递归条件
		Scan(Path, Temp, FileData, Type);
	else
		_findclose(Temp);//否则结束递归
}

int main() {
	
	char *Path = "*.txt";
	char *Type = "-s";
	char *Type2 = "-l";
	long Temp = 0;
	struct _finddata_t FileData;

	switch (*(Type + 1)) {
	case 'c': printf("code count: %d\n", CodeCount(Path)); break;
	case 'w': printf("word count: %d\n", WordCount(Path)); break;
	case 'l': printf("line count: %d\n", LineCount(Path)); break;
	case 's': Temp = _findfirst(Path, &FileData); 
			  Scan(Path, Temp, FileData, *(Type2+1));
			  break;
	default: break;

	}

	getchar();

	return 0;
}
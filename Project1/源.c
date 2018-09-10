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

	char *s = (char*)malloc(200 * sizeof(char));//申请空间
	int count = 0;

	while (fgets(s, 200, file) != NULL)	//逐次取文件中的行
		count++;

	free(s);
	fclose(file);

	return count;
}

void AllDetail(char *Path) {
	
	FILE *file = fopen(Path, "r");
	assert(file != NULL);

	char *s = (char*)malloc(200 * sizeof(char));//申请空间
	int i;
	
	int is_codeline = 0;
	int is_annoline = 0;
	int codecount = 0;
	int annocount = 0;
	int blankcount = 0;
	int lock = 0;
	int filelock = 0;
	
	while (fgets(s, 200, file) != NULL) {	//逐次取文件中的行
		for (i = 0; *(s+i) != '\0'; i++) {
			
			if ( ( ( *(s+i) >= 'a' && *(s+i) <= 'z') || ( *(s+i) >= 'A' && *(s+i) <= 'Z') ) && filelock == 0) {//判断是否是代码行
				if (is_codeline == 0 && lock == 0) 
					codecount++;
				is_codeline = 1;
			}

			if ( *(s+i) == '/' && *(s+i+1) == '/' && is_codeline == 0 && filelock == 0){	//判断是否为注释行
					annocount++;
					lock = 1;
			}

			if (*(s + i) == '/' && *(s + i + 1) == '*'){//判断文档注释开始
				filelock = 1;
				annocount -= is_codeline;//注释在代码后的情况
			}

			if (*(s + i) == '*' && *(s + i + 1) == '/') {//判断文档注释结束
				filelock = 0;
				annocount += (*(s + i + 2) == '\n');//注释后换行情况

			}

		}

		if (filelock) 
			annocount++;//注释行结束
		blankcount++;//每一行结束计数加一，并清空状态
		is_codeline = 0;
		is_annoline = 0;
		lock = 0;		
	}

	free(s);
	fclose(file);

	blankcount = blankcount - codecount - annocount;
	printf("codeline:%d, annoline:%d, blankline:%d", codecount, annocount, blankcount);
}

void Scan(char *Path, char Type) {
	
	char *FileName = NULL;
	char *FileType = NULL;
	char Temp[30];
	char NextPath[100];
	long Head;
	struct _finddata_t FileData;
	int i = 0;

	while (*(Path + i) != '\0') {
		if (*(Path + i) == '\\')
			FileName = Path + i + 1;
		if (*(Path + i) == '.')
			FileType = Path + i + 1;
		i++;
	}
	
	strcpy(Temp, FileType);//调整字符串
	*FileType = '*';
	*(FileType + 1) = '\0';
	
	Head = _findfirst(Path, &FileData);
	
	strcpy(FileType, Temp);//恢复字符串

	do {
		if ( !strcmp(FileData.name, "..") || !strcmp(FileData.name, "."))
			continue;
		
		if (_A_SUBDIR == FileData.attrib)	//是文件夹
		{	
			strcpy(Temp, FileName);
			
			for (i = 0; *(FileData.name + i) != '\0'; i++) {
				*(FileName + i) = *(FileData.name + i);
			}
			*(FileName + i) = '\\';
			strcat(Path, Temp);

			Scan(Path, Type);

			strcpy(FileName, Temp);
			
		}
		else//是文件 
		{	
			for (i = 0; *(FileData.name + i) != '.'; i++);

			if (!strcmp(FileData.name + i + 1, FileType)) {	//是指定类型的文件
				
				strcpy(Temp, FileName);
				strcpy(FileName, FileData.name); //调整字符串
				
				printf("%s:  ", FileData.name);
				AllDetail(Path);
				printf("\n");

				strcpy(FileName, Temp);//恢复字符串
			}			
		}
	} while (_findnext(Head, &FileData) == 0);

	_findclose(Head);
	
}

int main() {
	
	char Path[100] = "C:\\Users\\user\\source\\repos\\Project1\\Project1\\*.c";
	char *Type = "-s";
	char *Type2 = "-a";


	switch (*(Type + 1)) {
	case 'c': printf("code count: %d\n", CodeCount(Path)); break;
	case 'w': printf("word count: %d\n", WordCount(Path)); break;
	case 'l': printf("line count: %d\n", LineCount(Path)); break;
	case 'a': AllDetail(Path); break;
	case 's': Scan(Path, *(Type2+1)); break;
	default: break;

	}

	printf("\nPress any key to continue");
	getchar();

	return 0;
}
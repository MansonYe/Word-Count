#include<stdio.h>
#include<string.h>
#include<assert.h>
#include<malloc.h>
#include<io.h>


int CodeCount(char *Path) {	//�����ַ�����
	
	FILE *file = fopen(Path, "r");
	assert(file != NULL);	//���ļ��������򱨴�

	char code;
	int count = 0;

	while ((code = fgetc(file)) != EOF) {	//��ȡ�ַ�ֱ������
		if ( (code!=' ') && (code!='\n') && (code!='\t') )	//�ж��Ƿ����ַ�
			count++;
	}

	fclose(file);

	return count;
}

int WordCount(char *Path) {	//���㵥�ʸ���

	FILE *file = fopen(Path, "r");
	assert(file != NULL);

	char word;
	int is_word = 0;	//���ڼ�¼�ַ��Ƿ��ڵ�����
	int count = 0;

	while ((word = fgetc(file)) != EOF) {
		if ((word >= 'a' && word <= 'z') || (word >= 'A' && word <= 'Z')) {	//�ж��Ƿ�����ĸ
			if (is_word == 0)	//������ĸ���ڵ�����
				count++;
			is_word = 1;	//��¼����״̬
		}
		else 
			is_word = 0;	//��¼�����ڵ���״̬
	}

	fclose(file);

	return count;
}

int LineCount(char *Path) {	//��������

	FILE *file = fopen(Path, "r");
	assert(file != NULL);

	char *s = (char*)malloc(200 * sizeof(char));//����ռ�
	int count = 0;

	while (fgets(s, 200, file) != NULL)	//���ȡ�ļ��е���
		count++;

	free(s);
	fclose(file);

	return count;
}

void AllDetail(char *Path) {
	
	FILE *file = fopen(Path, "r");
	assert(file != NULL);

	char *s = (char*)malloc(200 * sizeof(char));//����ռ�
	int i;
	
	int is_codeline = 0;
	int is_annoline = 0;
	int codecount = 0;
	int annocount = 0;
	int blankcount = 0;
	int lock = 0;
	int filelock = 0;
	
	while (fgets(s, 200, file) != NULL) {	//���ȡ�ļ��е���
		for (i = 0; *(s+i) != '\0'; i++) {
			
			if ( ( ( *(s+i) >= 'a' && *(s+i) <= 'z') || ( *(s+i) >= 'A' && *(s+i) <= 'Z') ) && filelock == 0) {//�ж��Ƿ��Ǵ�����
				if (is_codeline == 0 && lock == 0) 
					codecount++;
				is_codeline = 1;
			}

			if ( *(s+i) == '/' && *(s+i+1) == '/' && is_codeline == 0 && filelock == 0){	//�ж��Ƿ�Ϊע����
					annocount++;
					lock = 1;
			}

			if (*(s + i) == '/' && *(s + i + 1) == '*'){//�ж��ĵ�ע�Ϳ�ʼ
				filelock = 1;
				annocount -= is_codeline;//ע���ڴ��������
			}

			if (*(s + i) == '*' && *(s + i + 1) == '/') {//�ж��ĵ�ע�ͽ���
				filelock = 0;
				annocount += (*(s + i + 2) == '\n');//ע�ͺ������

			}

		}

		if (filelock) 
			annocount++;//ע���н���
		blankcount++;//ÿһ�н���������һ�������״̬
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
	
	strcpy(Temp, FileType);//�����ַ���
	*FileType = '*';
	*(FileType + 1) = '\0';
	
	Head = _findfirst(Path, &FileData);
	
	strcpy(FileType, Temp);//�ָ��ַ���

	do {
		if ( !strcmp(FileData.name, "..") || !strcmp(FileData.name, "."))
			continue;
		
		if (_A_SUBDIR == FileData.attrib)	//���ļ���
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
		else//���ļ� 
		{	
			for (i = 0; *(FileData.name + i) != '.'; i++);

			if (!strcmp(FileData.name + i + 1, FileType)) {	//��ָ�����͵��ļ�
				
				strcpy(Temp, FileName);
				strcpy(FileName, FileData.name); //�����ַ���
				
				printf("%s:  ", FileData.name);
				AllDetail(Path);
				printf("\n");

				strcpy(FileName, Temp);//�ָ��ַ���
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
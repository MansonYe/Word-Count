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

	char *s = (char*)malloc(100 * sizeof(char));//����ռ�
	int count = 0;

	while (fgets(s, 100, file) != NULL)	//���ȡ�ļ��е���
		count++;

	free(s);
	fclose(file);

	return count;
}

void AllDetail(char *Path) {
	
}

void Scan(char *Path, long Temp, struct _finddata_t FileData, char Type) {//ͨ���ݹ�ʵ��Ŀ¼���ļ�����

	char *Address = (char*)malloc(100 * sizeof(char));//����ռ䴢���ļ���ַ
	char *p = Address;
	int i;

	for (i = 0; *(Path + i) != '*'; i++)//������'*'ǰ��·������
		*(p++) = *(Path + i);

	for (i = 0; *(FileData.name + i) != '\0'; i++)//���ļ������뵽·����
		*(p++) = *(FileData.name + i);

	*(p++) = '\0';//�ַ���������־

	printf("%s ", FileData.name);
	switch (Type) {//���ݹ���ѡ�����
	case 'c': printf("code count: %d\n", CodeCount(Address)); break;
	case 'w': printf("word count: %d\n", WordCount(Address)); break;
	case 'l': printf("line count: %d\n", LineCount(Address)); break;
	default: break;
	}

	free(Address);

	if (_findnext(Temp, &FileData) == 0)//����һ���ļ��ܱ���ȡΪ�ݹ�����
		Scan(Path, Temp, FileData, Type);
	else
		_findclose(Temp);//��������ݹ�
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
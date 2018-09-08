#include<stdio.h>
#include<string.h>
#include<assert.h>


void CodeCount(char *Address) {
	
	FILE *file = fopen(Address, "r");
	assert(file != NULL);

	char code;
	int count = 0;

	while ((code = fgetc(file)) != EOF) {
		if ( (code!=' ') && (code!='\n') && (code!='\t') )
			count++;
	}

	fclose(file);

	printf(" code = %d", count);

}

void WordCount(char *Address) {

	FILE *file = fopen(Address, "r");
	assert(file != NULL);

	char word;
	int is_word = 0;
	int count = 0;

	while ((word = fgetc(file)) != EOF) {
		if ((word >= 'a' && word <= 'z') || (word >= 'A' && word <= 'Z')) {
			if (is_word == 0) 
				count++;
			is_word = 1;
		}
		else 
			is_word = 0;
	}

	fclose(file);

	printf(" word = %d", count);

}

void LineCount(char *Address) {

	FILE *file = fopen(Address, "r");
	assert(file != NULL);

	char code;
	int is_line = 0;
	int count = 0;

	while ((code = fgetc(file)) != EOF) {
		if ((code != ' ') && (code != '\n') && (code != '\t')) {
			if (is_line == 0)
				count++;
			is_line = 1;
		}
		if (code == '\n')
			is_line = 0;
	}

	fclose(file);

	printf(" line = %d", count);

}

int main() {
	
	char *Address = "test.txt";
	char *Type = "-l";
	
	switch (*(Type + 1)) {
	case 'c': CodeCount(Address); break;
	case 'w': WordCount(Address); break;
	case 'l': LineCount(Address); break;

	}

	

	return 0;
}
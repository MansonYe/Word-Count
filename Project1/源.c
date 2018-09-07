#include<stdio.h>
#include<string.h>
#include<assert.h>


void CodeCalu(char *Address) {
	
	FILE *file = fopen(Address, "r");
	assert(file != NULL);

	char code;
	int cal = 0;

	while ((code = fgetc(file)) != EOF) {
		if ( (code!=' ') && (code!='\n') && (code!='\t') )
			cal++;
	}

	fclose(file);

	printf("%d", cal);

}



int main() {
	
	char *Address = "test.txt";
	char *Type = "-c";
	
	switch (*(Type + 1)) {
	case 'c': CodeCalu(Address); break;

	}

	

	return 0;
}
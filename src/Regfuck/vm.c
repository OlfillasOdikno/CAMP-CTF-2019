#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <time.h>
int parse(int * regs, char * code, short *codePtr, short* ptr,int num_regs){
	if((code[(*codePtr)/8]>>(7-(*codePtr)%8))&1){
		regs[0]++;
	}else{
		switch(regs[0]){
			case 1:
				if(*ptr == num_regs-1){
					return 1;
				}
				*ptr=*ptr+1;
				break;
			case 2:
				if(*ptr==-1){
					return 1;
				}
				*ptr=*ptr-1;
				break;
			case 3:
				regs[*ptr+2]++;
				break;
			case 4:
				regs[*ptr+2]--;
				break;
			case 5:
				if(regs[*ptr+2]){
					*codePtr = (int16_t)((regs[1]>>16)&0xFFFF);
				}
				break;
			case 6:				
				regs[1] = regs[*ptr+2];
				break;
			case 7:
				putchar(regs[*ptr+2]);
				break;
			case 8:
				regs[*ptr+2]=(*codePtr<<16)|*ptr&0xFFFF;
				break;
			case 9:
				if(regs[*ptr+2]){
					*ptr = (int16_t)(regs[1]&0xFFFF);
				}
				break;
		}
		regs[0]= 0;
	}
	return 0;
}

int vm(int * regs,int num_regs,char * code, int length){
	short ptr = 0;
	ushort codePtr = 0;
	time_t start;
	time(&start);
	time_t now;
	while(codePtr < length){
		time(&now);
		if(now-start>2){
			return 1;
		}
		if(parse(regs, code,&codePtr,&ptr,num_regs)){
			return 1;
		}
		codePtr++;
	}
	return 0;
}

int main(int argc, char *argv[]){
	puts(
" ______ _______ _______ _______ _______ ______ __  __ \n"
"|   __ \\    ___|     __|    ___|   |   |      |  |/  |\n"
"|      <    ___|    |  |    ___|   |   |   ---|     < \n"
"|___|__|_______|_______|___|   |_______|______|__|\\__|\n"
"by localo\n"
"\nusage: just push your payload to stdin..\n"
		);
	int *regs;
	int n_regs;
	int length;
	read(STDIN_FILENO, &n_regs, 4);
	if(n_regs<=0){
		return 1;
	}
	read(STDIN_FILENO, &length, 4);
	if(length<=0 || length%8 != 0){
		return 1;
	}
	regs = mmap((void *)0x405000, (n_regs+2)*sizeof(int)+length/8, PROT_READ | PROT_WRITE, MAP_PRIVATE| MAP_ANONYMOUS, -1, 0); 
    if (regs == MAP_FAILED)
        exit(EXIT_FAILURE);
	char * code = (char *)(regs+(n_regs+2)*sizeof(int));
	if(code==0){
		return 1;
	}
	read(STDIN_FILENO, code, length/8);
	return vm(regs,n_regs,code,length);
}
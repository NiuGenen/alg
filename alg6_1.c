#include<stdio.h>
#include<stdlib.h>

#include <getopt.h>

#include <time.h>

#define BLOCK_DARK  1
#define BLOCK_SPACE 0
#define BLOCK_WHITE 2

#define STATE_START -1
#define STATE_0 0
#define STATE_1 1
#define STATE_2 2
#define STATE_3 3
#define STATE_4 4
#define STATE_5 5
#define STATE_6 6

int result[256] = {0};
int retLen = 0;

int DFA(int *block,int length)
{
    int state = STATE_START;
    int pos_space = -1;
    for(int i=0; i<length; ++i){
        if(block[i] == BLOCK_SPACE){
            pos_space = i;
            state = STATE_START;
            continue;
        }
        switch(state){
        case STATE_START:
            switch(block[i]){
            case BLOCK_DARK:
                state = STATE_0;
                break;
            case BLOCK_WHITE:
                state = STATE_1;
                break;
            }
            break;
        case STATE_0:
            switch(block[i]){
            case BLOCK_DARK:
                //state = STATE_0;
                break;
            case BLOCK_WHITE:
                state = STATE_1;
                break;
            }
            break;
        case STATE_1:
            switch(block[i]){
            case BLOCK_DARK:
                state = STATE_3;
                break;
            case BLOCK_WHITE:
                state = STATE_2;
                break;
            }
            break;
        case STATE_2:
            switch(block[i]){
            case BLOCK_DARK:
                state = STATE_4;
                break;
            case BLOCK_WHITE:
                //state = STATE_2;
                break;
            }
            break;
        case STATE_3:
            switch(block[i]){
            case BLOCK_DARK:
                state = STATE_6;
                break;
            case BLOCK_WHITE:
                state = STATE_1;
                break;
            }
            break;
        case STATE_4:
            if(pos_space != -1) {
                return 0;
            }
            switch(block[i]){
            case BLOCK_DARK:
                state = STATE_5;
                break;
            case BLOCK_WHITE:
                state = STATE_1;
                break;
            }
            break;
        case STATE_5:
            return 0;
            break;
        case STATE_6:
            if(pos_space == -1) {
                return 0;
            }
            switch(block[i]){
            case BLOCK_DARK:
                //state = STATE_6;
                break;
            case BLOCK_WHITE:
                state = STATE_1;
                break;
            }
            break;
        }
    }
    return 1;
}

int check(int*block,int length){
    for(int i=0;i<length/2;++i){
        if( block[i]==BLOCK_DARK )
            continue;
        else
            return 0;
    }
    return 1;
}

int print(int *block,int length){
    for(int i=0;i<length;++i){
        switch(block[i]){
        case BLOCK_DARK:
            printf("1");
            break;
        case BLOCK_WHITE:
            printf("2");
            break;
        case BLOCK_SPACE:
            printf("_");
            break;
        }
    }
    printf("\n");
}

int movinng(int *block,int pos_space,int length,int level){
    printf("[%3d] ",level);
    print(block,length);

    if(check(block,length)) return 1;

    if( pos_space+1 < length && block[pos_space+1]==BLOCK_DARK){
        result[retLen++] = pos_space+1;
        block[pos_space] = BLOCK_DARK;
        block[pos_space+1] = BLOCK_SPACE;
        if(DFA(block,length)){
            if( movinng(block,pos_space+1,length,level+1)){
                return 1;
            }else{
                block[pos_space] =BLOCK_SPACE;
                block[pos_space+1] = BLOCK_DARK;
            }
        }else{
            block[pos_space] =BLOCK_SPACE;
            block[pos_space+1] = BLOCK_DARK;
        }
        --retLen;
    }
    if( pos_space+2<length && block[pos_space+2]==BLOCK_DARK ){
        result[retLen++] = pos_space+2;
        block[pos_space] = BLOCK_DARK;
        block[pos_space+2] = BLOCK_SPACE;
        if(DFA(block,length)){
            if( movinng(block,pos_space+2,length,level+1)){
                return 1;
            }else{
                block[pos_space] =BLOCK_SPACE;
                block[pos_space+2] = BLOCK_DARK;
            }
        }else{
            block[pos_space] =BLOCK_SPACE;
            block[pos_space+2] = BLOCK_DARK;
        }
        --retLen;
    }

    if( pos_space-1>=0 && block[pos_space-1]==BLOCK_WHITE ){
        result[retLen++] = pos_space-1;
        block[pos_space] = BLOCK_WHITE;
        block[pos_space-1] = BLOCK_SPACE;
        if(DFA(block,length)){
            if( movinng(block,pos_space-1,length,level+1)){
                return 1;
            }else{
                block[pos_space] = BLOCK_SPACE;
                block[pos_space-1] = BLOCK_WHITE;
            }
        }else{
            block[pos_space] = BLOCK_SPACE;
            block[pos_space-1] = BLOCK_WHITE;
        }
        --retLen;
    }
    if( pos_space-2>=0 && block[pos_space-2]==BLOCK_WHITE ){
        result[retLen++] = pos_space-2;
        block[pos_space] = BLOCK_WHITE;
        block[pos_space-2] = BLOCK_SPACE;
        if(DFA(block,length)){
            if( movinng(block,pos_space-2,length,level+1)){
                return 1;
            }else{
                block[pos_space] = BLOCK_SPACE;
                block[pos_space-2] = BLOCK_WHITE;
            }
        }else{
            block[pos_space] = BLOCK_SPACE;
            block[pos_space-2] = BLOCK_WHITE;
        }
        --retLen;
    }

    return 0;
}

int main(int argc,char*argv[])
{
    int ret = 0;

	int n=3;

    struct option longopts[]=
	{
		{"number",1,0,'n'},
		{0,0,0,0}
	};

    char c;
	while((c=getopt_long(argc,argv,"n:",longopts,NULL))!=EOF){
		switch(c)
		{
		case 'n':
			n = atoi(optarg);
			break;
		default:
			printf("Wrong Option!\n");
            ret = 1;
			goto _MAIN_OVER;
			break;
		}
	}

    int *block = (int*)malloc(sizeof(int)*(2*n+1));
    block[n] = BLOCK_SPACE;
    for(int i=0;i<n;++i){
        block[i] = BLOCK_WHITE;
        block[2*n-i] = BLOCK_DARK;
    }

    clock_t start = clock();
    printf("Start: %ld\n",start);

    movinng(block, n, 2*n+1, 0);

    for(int i=0;i<retLen;++i){
        printf("%d ",result[i]);
    }
    printf("\n");

    clock_t end = clock();
    printf("Start: %ld\n",end);
    printf("Total Time: %lf s\n",((double)end-start)/CLOCKS_PER_SEC );

_MAIN_OVER:
    return ret;
}

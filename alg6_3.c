#include<stdio.h>
#include<stdlib.h>

#include <time.h>

#include <getopt.h>

#define STAGE_SIZE  8

int **stage;

int stepVegas = 0;

int solution_count = 0;

int count_search = 0;
int count_random = 0;

void init()
{
    stage = (int **)malloc(sizeof(int*)*STAGE_SIZE);
    for(int i=0; i<STAGE_SIZE; ++i){
        stage[i] = (int *)malloc(sizeof(int)*STAGE_SIZE);
    }
}

int validPos(int x,int y){
    if( x<0 || x>=STAGE_SIZE || y<0 || y>=STAGE_SIZE ) return 0;
    return 1;
}

int validStep(int x,int y)
{

    if( !validPos(x,y) ) return 0;

    for(int i=1;i<STAGE_SIZE;++i){
        //if( validPos(x+i,y+i) && stage[x+i][y+i] == 1) return 0;
        //if( validPos(x+i,y)    && stage[x+i][y]   == 1) return 0;
        //if( validPos(x+i,y-i)  && stage[x+i][y-i] == 1) return 0;
        if( validPos(x,y-i)    && stage[x][y-i]   == 1) return 0;
        if( validPos(x,y+i)   && stage[x][y+i]   == 1) return 0;
        if( validPos(x-i,y+i) && stage[x-i][y+i] == 1) return 0;
        if( validPos(x-i,y)    && stage[x-i][y]   == 1) return 0;
        if( validPos(x-i,y-i)  && stage[x-i][y-i] == 1) return 0;
    }

    return 1;
}

int NQueen(int row,int col){

    //printf("[%d][%d]\n",row,col);
    stage[row][col] = 1;

    if(row == STAGE_SIZE - 1){
        return 1;
    }

    for(int coll=0; coll<STAGE_SIZE; ++coll){
        ++count_search;
        if( validStep(row+1, coll) ) 
            if( NQueen(row+1, coll) )
                return 1;
    }

    stage[row][col] = 0;

    return 0;
}

void clearStage(){
    for(int i=0;i<STAGE_SIZE;++i)
        for(int j=0;j<STAGE_SIZE;++j)
            stage[i][j]=0;
}

int sequence[STAGE_SIZE] = {0,1,2,3,4,5,6,7};
int randomSize = STAGE_SIZE;
void resetRanSeq(){
    randomSize = STAGE_SIZE;
    for(int i=0;i<randomSize;++i){
        sequence[i] = i;
    }
}

int randomPos(int row,int col,int maxRow){

    if(maxRow < 0) return 1;

    stage[row][col] = 1;

    if(row == maxRow) return 1;

    int randomSequence[STAGE_SIZE];

    resetRanSeq();
    int randomRange = randomSize;
    for(int i=0; i<STAGE_SIZE; ++i){
        int ran_col = rand() % randomRange;
        //printf("[%d] ",ran_col);
        --randomRange;
        randomSequence[i] = sequence[ran_col];
        sequence[ran_col] = sequence[randomRange];
    }
    //for(int i=0;i<STAGE_SIZE;++i) printf("%d ",randomSequence[i]);
    //printf("\n");

    for(int i=0;i<STAGE_SIZE;++i){
        ++count_random;
        if( validStep(row+1,randomSequence[i]) ) 
            if( randomPos(row+1, randomSequence[i], maxRow) )
                return 1;
    }

    stage[row][col] = 0;

    return 0;
}

int LasVegasNQueen()
{
    clearStage();

    int row = 0;

    if(stepVegas>=1){
        int randomSequence[STAGE_SIZE];
        resetRanSeq();
        int randomRange = randomSize;
        for(int i=0; i<STAGE_SIZE; ++i){
            int ran_col = rand() % randomRange;
            //printf("[%d] ",ran_col);
            --randomRange;
            randomSequence[i] = sequence[ran_col];
            sequence[ran_col] = sequence[randomRange];
        }
        //for(int i=0;i<STAGE_SIZE;++i) printf("%d ",randomSequence[i]);
        //printf("\n");

        for(int i=0;i<STAGE_SIZE;++i){
            //printf("%d ",randomSequence[i]);
            ++count_random;
            if( validStep(row, randomSequence[i]) ) 
                if( randomPos(row, randomSequence[i], stepVegas-1) )
                    break;
        }
    }
    //printf("\n");
    row = stepVegas;

    /*for(; row<stepVegas; ++row){
        int col_random = rand() % STAGE_SIZE;
        while( !validStep(row, col_random) ){
            col_random = rand() % STAGE_SIZE;
        }
        stage[row][col_random] = 1;
    }*/

    if(row == STAGE_SIZE) return 1;

    if(stepVegas>=1){
        printf("Get One Random Solution ");
        for(int i=0; i<stepVegas; ++i){
            for(int j=0; j<STAGE_SIZE; ++j){
                if( stage[i][j] == 1 ){
                    printf("%d ",j);
                    break;
                }
            }
        }
        printf("\n");
    }

    for(int col=0; col<STAGE_SIZE; ++col){
        ++count_search;
        if( validStep(row, col) ) 
            if( NQueen(row,col) )
                return 1;
    }

    return 0;
}

void NQueenAll(int row,int col){

    stage[row][col] = 1;

    if(row == STAGE_SIZE - 1){
        printf("Found One Solution [%3d] ",++solution_count);
        for(int i=0; i<STAGE_SIZE; ++i){
            for(int j=0; j<STAGE_SIZE; ++j){
                if(stage[i][j]==1){
                    printf("%d ",j);
                    break;
                }
            }
        }
        printf("\n");
    }

    for(int coll=0; coll<STAGE_SIZE; ++coll){
        if( validStep(row+1,coll) ) NQueenAll(row+1,coll);
    }

    stage[row][col] = 0;
}

void allSolution(){
    clearStage();
    for(int col=0; col<STAGE_SIZE; ++col){
        if( validStep(0,col) ) NQueenAll(0,col);
    }
}

int main(int argc,char*argv[])
{
    int ret = 0;

    init();

    srand( (unsigned int)time(NULL) );

    struct option longopts[]=
    {
		{"n",1,0,'n'},
        {"all",0,0,'a'},
		{0,0,0,0}
    };

    char c;
	while((c=getopt_long(argc,argv,"n:a",longopts,NULL))!=EOF){
		switch(c)
		{
		case 'n':
			stepVegas = atoi(optarg);
			break;
        case 'a':
            allSolution();
            goto _MAIN_OVER;
            break;
		default:
			printf("Wrong Option!\n");
            ret = 1;
			goto _MAIN_OVER;
			break;
		}
	}

    clock_t start = clock();
    printf("Start at %ld\n",start);

    while(! LasVegasNQueen() );

    printf("Find Solution: ");
    for(int i=0; i<STAGE_SIZE; ++i){
        for(int j=0; j<STAGE_SIZE; ++j){
            if(stage[i][j]==1){
                printf("%d ",j);
                break;
            }
        }
    }
    printf("\n");

    clock_t end = clock();
    printf("Ended at %ld\n",end);

    printf("Total Time %lf s\n",(double)(end-start)/CLOCKS_PER_SEC);

    printf("Random Nodes Count %d\n",count_random);
    printf("Search Nodes Count %d\n",count_search);

_MAIN_OVER:
    return ret;
}

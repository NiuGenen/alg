#include<stdio.h>
#include<stdlib.h>

#include <time.h>

#include <getopt.h>

#define STAGE_SIZE  8

int testWay = 1;

clock_t startT;
clock_t endT;

static int dir[8][2] = { {1,2},{2,1},{2,-1},{1,-2},{-1,-2},{-2,-1},{-2,1},{-1,2} };

int **stage;
int **next;

typedef struct _POS{
    int x;
    int y;
}Pos;

int validPos(Pos p){
    if(p.x<0 || p.x>=STAGE_SIZE || p.y<0 || p.y>=STAGE_SIZE){
        return 0;
    }
    else{
        return 1;
    }
}

int getNext(Pos p){
    int nextN = 0;
    for(int i=0;i<8;++i){
        Pos nextP;
        nextP.x = p.x + dir[i][0];
        nextP.y = p.y + dir[i][1];
        if( validPos(nextP) && stage[nextP.x][nextP.y]==0 ){
            ++nextN;
        }
    }
    return nextN;
}


Pos tourWay[STAGE_SIZE*STAGE_SIZE];
int tourWayLength = 0;

int step(Pos p){
    if( stage[p.x][p.y] == 0 ){

        stage[ p.x ][ p.y ] = 1;
        tourWay[ tourWayLength++ ] = p;

        //printf("STEP: %3d %3d\n",p.x,p.y);

        for(int i=0; i<8; ++i){
            Pos nextP = { p.x+dir[i][0], p.y+dir[i][1] };
            if( validPos(nextP) /*&& next[nextP.x][nextP.y] > 0*/ ){
                next[nextP.x][nextP.y] -= 1;
            }
        }

        return 0;
    }else{
        printf("Unvalid pos to step %3d %3d\n",p.x,p.y);
        return 1;
    }

}

int unstep(Pos p){
    if( stage[p.x][p.y] == 1 ){ 

        stage[p.x][p.y] = 0;
        --tourWayLength;

        //printf("STEP: %3d %3d\n",p.x,p.y);

        for(int i=0; i<8; ++i){
            Pos nextP = {p.x+dir[i][0],p.y+dir[i][1]};
            if( validPos(nextP) ){
                next[nextP.x][nextP.y] += 1;
            }
        }

        return 0;
    }else{
        printf("Unvalid pos to unstep %3d %3d\n",p.x,p.y);
        return 1;
    }
}

void init(){
    stage = (int**)malloc(sizeof(int*)*STAGE_SIZE);
    next = (int**)malloc(sizeof(int*)*STAGE_SIZE);
    for(int i=0;i<STAGE_SIZE;++i){
        stage[i] = (int*)malloc(sizeof(int)*STAGE_SIZE);
        next[i] = (int*)malloc(sizeof(int)*STAGE_SIZE);
    }

    for(int x=0; x<STAGE_SIZE; ++x){
        for(int y=0; y<STAGE_SIZE; ++y){
            Pos p = {x,y};
            next[x][y] = getNext(p);
        }
    }
}

Pos start = {1,1};
int count = 0;

void sortNextList_1_to_2(Pos*list,int len){
    /*for(int i=0;i<len;++i){
        printf("[%1d][%1d] ",list[i].x,list[i].y);
    }
    printf("\n");*/
    for(int i=0;i<len-1;++i){
        int index = i;
        for(int j=i+1;j<len;++j){
            if( next[ list[j].x ][ list[j].y ] < next[ list[index].x ][ list[index].y ] ){
                index = j;
            }
        }
        Pos temp = list[i];
        list[i] = list[index];
        list[index] = temp;
    }
    /*for(int i=0;i<len;++i){
        printf("[%1d][%1d] ",list[i].x,list[i].y);
    }
    printf("\n");
    exit(4);*/
}

void sortNextList_2_to_1(Pos*list,int len){
    /*for(int i=0;i<len;++i){
        printf("[%1d][%1d] ",list[i].x,list[i].y);
    }
    printf("\n");*/
    for(int i=0;i<len-1;++i){
        int index = i;
        for(int j=i+1;j<len;++j){
            if( next[ list[j].x ][ list[j].y ] > next[ list[index].x ][ list[index].y ] ) {
                index = j;
            }
        }
        Pos temp = list[i];
        list[i] = list[index];
        list[index] = temp;
    }
    /*for(int i=0;i<len;++i){
        printf("[%1d][%1d] ",list[i].x,list[i].y);
    }
    printf("\n");
    exit(4);*/
}

int canreturn(){
    Pos last = tourWay[STAGE_SIZE*STAGE_SIZE-1];
    for(int i=0; i<8; ++i){
        if( last.x+dir[i][0]==start.x && last.y+dir[i][1]==start.y ){
            return 1;
        }
    }
    return 0;
}

int Count = 0;

void tour(Pos p){
    ++count;
    if( step(p) ) exit(3);

    if( count == STAGE_SIZE*STAGE_SIZE ){
        ++Count;

        if( canreturn() ){   //needs return
            printf("OneWayGot %d\n",Count);
            printf("This way returns \n");

            for(int i=0; i<STAGE_SIZE; ++i){
                for(int j=0; j<STAGE_SIZE; ++j){
                    printf("[%2d][%2d]  ", tourWay[STAGE_SIZE*i+j].x, tourWay[STAGE_SIZE*i+j].y );
                }
                printf("\n");
            }
    
            endT = clock();
            printf("End at %ld\n",endT);
            printf("Total time = %lf s\n",(double)(endT-startT)/CLOCKS_PER_SEC);
    
            exit(0);
        }
        
        if(testWay == 1 || testWay == 2){   //no need return
            printf("OneWayGot %d\n",Count);

            for(int i=0; i<STAGE_SIZE; ++i){
                for(int j=0; j<STAGE_SIZE; ++j){
                    printf("[%2d][%2d]  ", tourWay[STAGE_SIZE*i+j].x, tourWay[STAGE_SIZE*i+j].y );
                }
                printf("\n");
            }

            endT = clock();
            printf("Start at %ld\n",endT);
            printf("Total time = %lf s\n",(double)(endT-startT)/CLOCKS_PER_SEC);
            
            exit(0);
        }

        goto _TOUR_OVER;
    }

    if( testWay != 1 && testWay != 2 ){     // if needs return 
        if( next[start.x][start.y] <= 0 ){  // then we can go back when START POSITION has no way to reach
            //printf("Go Back %d\n",count);
            goto _TOUR_OVER;
        }
    }

    Pos nextList[8];
    int nextListLength = 0;

    for(int i=0; i<8; ++i){
        Pos nextP = { p.x+dir[i][0], p.y+dir[i][1] };
        if( validPos(nextP) && stage[nextP.x][nextP.y] == 0 ){
            nextList[nextListLength++] = nextP;
        }
    }

    if(testWay == 1){//1->2
        sortNextList_1_to_2(nextList,nextListLength);

    }else if(testWay == 2){//2->1
        sortNextList_2_to_1(nextList,nextListLength);

    }else if(testWay == 3){//first 2->1 then 1->2   //needs return 
        sortNextList_1_to_2(nextList,nextListLength);

    }else if(testWay == 4){//first 1->2 then 2->1   //needs return
        sortNextList_2_to_1(nextList,nextListLength);

    }else if(testWay == 5){//first 1->2 then 2->1   //needs return
        if( count <= STAGE_SIZE*STAGE_SIZE/2 )
            sortNextList_1_to_2(nextList,nextListLength);
        else
            sortNextList_2_to_1(nextList,nextListLength);

    }else if(testWay == 6){//first 2->1 then 1->2   //needs return
        if( count <= STAGE_SIZE*STAGE_SIZE/2 )
            sortNextList_2_to_1(nextList,nextListLength);
        else
            sortNextList_1_to_2(nextList,nextListLength);
    }

    for(int i=0; i<nextListLength; ++i){
        tour( nextList[i] );
    }

_TOUR_OVER:
    --count;
    if( unstep(p) ) exit(3);
    return ;
}

int main(int argc,char*argv[])
{
    int ret = 0;

    init();

    //for(int i=0; i<8; ++i){
    //    printf("DIR: %2d %2d\n",dir[i][0],dir[i][1]);
    //}

    struct option longopts[]=
    {
		{"x",1,0,'x'},
		{"y",1,0,'y'},
		{"test",1,0,'t'},
		{0,0,0,0}
    };

    char c;
	while((c=getopt_long(argc,argv,"x:y:t:",longopts,NULL))!=EOF){
		switch(c)
		{
		case 'x':
			start.x = atoi(optarg);
			break;
        case 'y':
            start.y = atoi(optarg);
            break;
		case 't':
			testWay = atoi(optarg);
			break;
		default:
			printf("Wrong Option!\n");
            ret = 1;
			goto _MAIN_OVER;
			break;
		}
	}

    switch(testWay){
        case 1:printf("[[[ 1->2 no return]]]\n");
        break;
        case 2:printf("[[[ 2->1 no return]]]\n");
        break;
        case 3:printf("[[[ 1->2 need return]]]\n");
        break;
        case 4:printf("[[[ 2->1 need return]]]\n");
        break;
        case 5:printf("[[[ 1->2 & 2->1 need return]]]\n");
        break;
        case 6:printf("[[[ 2->1 & 1->2 need return]]]\n");
        break;
    }

    if( validPos(start) ){
        printf("Start pos: %3d %3d\n",start.x,start.y);

        startT = clock();
        printf("Start at %ld\n",startT);

        tour(start);

        endT = clock();
        printf("End at %ld\n",endT);
        printf("Total time = %lf s\n",(double)(endT-startT)/CLOCKS_PER_SEC);

    }
    else{
        printf("Unvalid start pos: %3d %3d\n",start.x,start.y);
        printf("Game Over!\n");
        ret = 2;
        goto _MAIN_OVER;
    }
    

_MAIN_OVER:
    return ret;
}

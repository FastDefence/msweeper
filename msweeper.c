#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#define SIZE 100

typedef struct GAMECONF{
  int m;
  int mine_num;
}gameconf;
typedef struct MSGRID{
  char grid_info;
  int mine_around;
  int grid_opened;
}msgrid;

void conf_init(gameconf *conf);
void grid_init(int x,msgrid grid[SIZE][SIZE]);
void mine_plot(int x,int mine_num,msgrid grid[SIZE][SIZE]);
void mine_calc(int x,msgrid grid[SIZE][SIZE]);
void grid_open(int x,int y,msgrid grid[SIZE][SIZE]);
int  check_gameclear(int x,msgrid grid[SIZE][SIZE]);
int  check_gameover(int x,int y,msgrid grid[SIZE][SIZE]);
void grid_allopen(msgrid grid[SIZE][SIZE]);
void board_whole_disp(int x,msgrid grid[SIZE][SIZE]);
void board_disp(int x,msgrid grid[SIZE][SIZE]);

int main(){
  int x,y,num; 
  gameconf conf;

  conf_init(&conf);
  msgrid grid[SIZE][SIZE];
  x=conf.m;
  num=conf.mine_num;
  grid_init(x,grid);
  mine_plot(x,num,grid);
  mine_calc(x,grid);
  board_whole_disp(x,grid);
  while(1){
    int punchx,punchy;
    char dummy;
    printf("$どこを開けますか(x y)>>");
    scanf("%d%c%d",&punchx,&dummy,&punchy);
    grid_open(punchx,punchy,grid);
    if(check_gameclear(x,grid)==(x*x)-num){
      printf("====ゲームクリア！！！====\n");
      board_whole_disp(x,grid);
      return 0;
    }
    if(check_gameover(punchx,punchy,grid)==1){
      printf("====ゲームオーバー！！====\n");
      grid_allopen(grid);
      board_whole_disp(x,grid);
      return 0;
    }
    board_whole_disp(x,grid);
  }
  return 0;
}

void conf_init(gameconf *conf){
  int mine_num;
  char dummy;

  for(int flag=0;flag==0;){
    printf("$ボードの大きさを入力(x<%d):",SIZE);
    scanf("%d%c",&conf->m,&dummy);

    if((conf->m)>SIZE){
      printf("$ボードのサイズが大きすぎます！\n");
    }
    else if((conf->m)<1){
      printf("$ボードのサイズが小さすぎます！\n");
    }
    else{
      flag=1;
    }
  }
  for(int flag=0;flag==0;){
    printf("$地雷の数を入力(<%d):",conf->m*conf->m);
    scanf("%d",&conf->mine_num);
    if(conf->mine_num>=conf->m*conf->m){
      printf("$地雷の個数が多すぎます！！\n");
    }
    else if(conf->mine_num<0){
      printf("$地雷の個数が少なすぎます！！\n");
    }
    else if(conf->mine_num<conf->m*conf->m){
      flag=1;
    }
  }
}

void grid_init(int x,msgrid grid[SIZE][SIZE]){
  for(int i=0;i<x;i++){
    for(int j=0;j<x;j++){
      grid[i][j].grid_info=' ';
      grid[i][j].mine_around=0;
      grid[i][j].grid_opened=0;
    }
  }
}

void mine_plot(int x,int mine_num,msgrid grid[SIZE][SIZE]){
  for(int i=0;i<mine_num;i++){
    int flag=0,plotx,ploty;
    while(flag==0){
      plotx=rand()%x;
      ploty=rand()%x;
      if(grid[plotx][ploty].grid_info!='*'){
	      grid[plotx][ploty].grid_info='*';
	      flag=1;
      }
    }
  }
}

void mine_calc(int x,msgrid grid[SIZE][SIZE]){
  for(int i=0;i<x;i++){
    for(int j=0;j<x;j++){
      if(grid[i-1][j-1].grid_info=='*'){/*左上*/
        grid[i][j].mine_around++;
      }
      if(grid[i-1][j].grid_info=='*'){/*上*/
        grid[i][j].mine_around++;
      }
      if(grid[i-1][j+1].grid_info=='*'){/*右上*/
        grid[i][j].mine_around++;
      }
      if(grid[i][j+1].grid_info=='*'){/*右*/
        grid[i][j].mine_around++;
      }
      if(grid[i+1][j+1].grid_info=='*'){/*右下*/
        grid[i][j].mine_around++;
      }
      if(grid[i+1][j].grid_info=='*'){/*下*/
        grid[i][j].mine_around++;
      }
      if(grid[i+1][j-1].grid_info=='*'){/*左下*/
        grid[i][j].mine_around++;
      }
      if(grid[i][j-1].grid_info=='*'){/*左*/
        grid[i][j].mine_around++;
      }
      if(grid[i][j].mine_around>0&&grid[i][j].grid_info!='*'){
        grid[i][j].grid_info='0'+grid[i][j].mine_around;
      }
    }
  }
}

void grid_open(int x,int y,msgrid grid[SIZE][SIZE]){
  if(grid[x][y].grid_opened==0){//if文おかしい
    grid[x][y].grid_opened=1;
  }
  else{
    printf("$すでにそのマスは空いています！\n");
  }
}

int check_gameclear(int x,msgrid grid[SIZE][SIZE]){
  int flag=0;
  for(int i=0;i<x;i++){
    for(int j=0;j<x;j++){
      if(grid[i][j].grid_opened==1){
        flag++;
      }
    }
  }
  return flag;
}

int check_gameover(int x,int y,msgrid grid[SIZE][SIZE]){
  int flag=0;
  if(grid[x][y].grid_info=='*'){
    flag=1;
  }
  return flag;
}

void grid_allopen(msgrid grid[SIZE][SIZE]){
  for(int i=0;i<SIZE;i++){
    for(int j=0;j<SIZE;j++){
      grid[i][j].grid_opened=1;
      if(grid[i][j].grid_info-'0'<10&&grid[i][j].grid_info-'0'>=0){
        grid[i][j].grid_info=' ';
      }
    }
  }
}

void board_whole_disp(int x,msgrid grid[SIZE][SIZE]){
  /*上枠*/
  printf(" ");
  for(int i=0;i<x;i++){
    printf("--");
  }
  printf(" \n");
  /*中段*/
  board_disp(x,grid);
  /*下枠*/
  printf(" ");
  for(int i=0;i<x;i++){
    printf("--");
  }
  printf(" \n");
}

void board_disp(int x,msgrid grid[SIZE][SIZE]){
  for(int i=0;i<x;i++){
    printf("|");
    for(int j=0;j<x;j++){
      if(grid[i][j].grid_opened==0){
        printf(". ");
      }
      else{
        printf("%c ",grid[i][j].grid_info);
      }
    }
    printf("|\n");
  }
}
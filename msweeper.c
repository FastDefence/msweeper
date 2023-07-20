#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<ctype.h>

#define SIZE 100

typedef struct GAMECONF{
  int m;
  int mine_num;
}gameconf;
typedef struct MSGRID{
  char grid_info;
  int mine_around;
  int grid_opened;
  int grid_pinned;
}msgrid;

void game_init(int x,int num,msgrid grid[SIZE][SIZE]);
void msweeper(int x,msgrid grid[SIZE][SIZE],gameconf conf);

void conf_init(gameconf *conf);
void grid_init(int x,msgrid grid[SIZE][SIZE]);
void mine_plot(int x,int mine_num,msgrid grid[SIZE][SIZE]);
void mine_calc(int x,msgrid grid[SIZE][SIZE]);
void grid_open(int x,int y,msgrid grid[SIZE][SIZE],gameconf conf);
void grid_pin(int x,int y,msgrid grid[SIZE][SIZE],gameconf conf);
int  check_gameclear(int x,msgrid grid[SIZE][SIZE]);
int  check_gameover(int x,int y,msgrid grid[SIZE][SIZE]);
void grid_allopen(msgrid grid[SIZE][SIZE]);
void board_whole_disp(int x,msgrid grid[SIZE][SIZE]);
void board_disp(int x,msgrid grid[SIZE][SIZE]);
void time_write(time_t time);
void best_time_disp();

int main(){
  int x,y,num; 
  time_t game_start,game_end;
  gameconf conf;
  conf_init(&conf);
  msgrid grid[SIZE][SIZE];
  x=conf.m;
  num=conf.mine_num;

  game_init(x,num,grid);
  msweeper(x,grid,conf);
  best_time_disp();

  return 0;
}

void game_init(int x,int num,msgrid grid[SIZE][SIZE]){//構造体等のゲーム設定を初期化する関数のグループ
  grid_init(x,grid);
  mine_plot(x,num,grid);
  mine_calc(x,grid);
  board_whole_disp(x,grid);
}

void msweeper(int x,msgrid grid[SIZE][SIZE],gameconf conf){//ゲームを実行する関数のグループ
  time_t end;
  time_t start=time(NULL);
  while(1){
    int punchx,punchy;
    char opt,dummy;
    printf("$コマンド入力(x y op)>>");
    scanf("%d%c%d%c%c",&punchx,&dummy,&punchy,&dummy,&opt);
    if(grid[punchx][punchy].grid_opened==0){
      if(opt=='o'||opt=='O'){
        grid_open(punchx,punchy,grid,conf);
      }
      if(opt=='m'||opt=='M'){
        grid_pin(punchx,punchy,grid,conf);
      }
    }
    else{
      printf("$そのますは既に開かれています\n");
    }
    if(check_gameclear(x,grid)==(x*x)-conf.mine_num){
      end=time(NULL);
      printf("====ゲームクリア！！！====\n");
      grid_allopen(grid);
      board_whole_disp(x,grid);
      time_write(end-start);
      return;
    }
    if(check_gameover(punchx,punchy,grid)==1){
      printf("====ゲームオーバー！！====\n");
      grid_allopen(grid);
      board_whole_disp(x,grid);
      return;
    }
    board_whole_disp(x,grid);
  }
}

void conf_init(gameconf *conf){//コマンドラインからの入力によるデータの初期化(ボードサイズ、地雷個数)
  int mine_num;
  char dummy;
  for(int flag=0;flag==0;){
    printf("$ボードの大きさを入力(x<%d)>>:",SIZE);
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
    printf("$地雷の数を入力(<%d)>>:",conf->m*conf->m);
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

void grid_init(int x,msgrid grid[SIZE][SIZE]){//ますの状態の初期化
  for(int i=0;i<x;i++){
    for(int j=0;j<x;j++){
      grid[i][j].grid_info=' ';
      grid[i][j].mine_around=0;
      grid[i][j].grid_opened=0;
      grid[i][j].grid_pinned=0;
    }
  }
}

void mine_plot(int x,int mine_num,msgrid grid[SIZE][SIZE]){//地雷設置位置の設定
  srand((unsigned)time(NULL));
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

void mine_calc(int x,msgrid grid[SIZE][SIZE]){//各ますの地雷数計算
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

void grid_open(int x,int y,msgrid grid[SIZE][SIZE],gameconf conf){//ますを開く
  int flag=grid[x][y].mine_around;
  grid[x][y].grid_opened=1;
  if(flag>0){
    return;
  }
  if(grid[x-1][y].grid_opened==0&&grid[x-1][y].mine_around==0&&x>=0&&x<conf.m&&y>=0&&y<conf.m){
    grid_open(x-1,y,grid,conf);
  }
  if(grid[x+1][y].grid_opened==0&&grid[x+1][y].mine_around==0&&x>=0&&x<conf.m&&y>=0&&y<conf.m){
    grid_open(x+1,y,grid,conf);
  }
  if(grid[x][y-1].grid_opened==0&&grid[x][y-1].mine_around==0&&x>=0&&x<conf.m&&y>=0&&y<conf.m){
    grid_open(x,y-1,grid,conf);
  }
  if(grid[x][y+1].grid_opened==0&&grid[x][y+1].mine_around==0&&x>=0&&x<conf.m&&y>=0&&y<conf.m){
    grid_open(x,y+1,grid,conf);
  }
  
  else{
    return;
  }
}

void grid_pin(int x,int y,msgrid grid[SIZE][SIZE],gameconf conf){//ピンを設置関数
//地雷の存在する可能性のあるマスにマークを付ける
  if(grid[x][y].grid_pinned==0){
    grid[x][y].grid_pinned=1;
    return;
  }
  if(grid[x][y].grid_pinned==1){
    grid[x][y].grid_pinned=0;
    return;
  }
}

int check_gameclear(int x,msgrid grid[SIZE][SIZE]){//終了条件(クリア)判定
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

int check_gameover(int x,int y,msgrid grid[SIZE][SIZE]){//終了条件(ゲームオーバー)判定
  int flag=0;
  if(grid[x][y].grid_info=='*'){
    flag=1;
  }
  return flag;
}

void grid_allopen(msgrid grid[SIZE][SIZE]){//全てのますを開く
  for(int i=0;i<SIZE;i++){
    for(int j=0;j<SIZE;j++){
      grid[i][j].grid_opened=1;
      if(grid[i][j].grid_info-'0'<10&&grid[i][j].grid_info-'0'>=0){
        grid[i][j].grid_info=' ';
      }
    }
  }
}

void board_whole_disp(int x,msgrid grid[SIZE][SIZE]){//装飾含めたすべてのユーザインタフェース部分を表示j
  /*上枠*/
  printf("   ");
  for(int i=0;i<x;i++){
    printf("%d ",i%10);
  }
  printf(" \n");
  /*中段*/
  board_disp(x,grid);
  /*下枠*/
  printf("  ");
  for(int i=0;i<x;i++){
    printf("--");
  }
  printf(" \n");
}

void board_disp(int x,msgrid grid[SIZE][SIZE]){//ユーザインタフェース中のボードの盤面部分の表示
  for(int i=0;i<x;i++){
    printf("%2d|",i);
    for(int j=0;j<x;j++){
      if(grid[i][j].grid_pinned==1){
        printf("? ");
      }
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

void time_write(time_t time){//経過時間計算、表示、ファイル書き込み
  time_t h=time/(60*60);
  time_t m=(time-h*(60*60))/60;
  time_t s=time%60;
  printf("経過時間:%ld時間%ld分%ld秒\n",h,m,s);
  FILE *fp;
  fp=fopen("score.txt","a");
  fprintf(fp,"%ld\n",time);
}

void best_time_disp(){//ベストタイム表示
  int time[1000],num,count=0;
  int c;
  FILE *fp;
  memset(time,0,sizeof(time));
  fp=fopen("score.txt","rb"); 
  while ((c=fgetc(fp)) != EOF) {
    if(isdigit(c)) {
      ungetc(c,fp);
      if (fscanf(fp, "%d", &num) == 1) {
        time[count] = num;
        count++;
      }
    }
  }
  int min=time[0];
  for(int i=1;i<count;i++){
    if(min>time[i]){
      min=time[i];
    }
  }
  time_t h=min/(60*60);
  time_t m=(min-h*(60*60))/60;
  time_t s=min%60;
  printf("ハイスコアは%d時間%d分%d秒です\n",h,m,s);
}
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>

int min(int x, int y){
  if(x >= y) return y;
  else return x;
}

int max(int x, int y){
  if(x >= y) return x;
  else return y;
}

typedef struct state_board{
  int board[5][5];
  int outbo[12];
}STATE_BOARD;

typedef struct state_bit{
  unsigned long int huhi;
  unsigned long int kakugin;
  unsigned long int kinou;
}STATE_BIT;

typedef struct node{
  STATE_BIT state_bit;
  struct node *parent;
  struct node *left;
  struct node *right;
  struct node *child;
}NODE;

int ban[5][5] = {{1, 9, 0, 0, 17}, {2, 0, 0, 0, 15}, {3, 0, 0, 0, 13}, {5, 0, 0, 0, 12}, {7, 0, 0, 19, 11}};
int motigoma[12] = {};
STATE_BIT history[200];
int co = 0; //手数のカウンター,150を超えたら終わり。
int co2 = 0; //プレイヤーの連続王手の千日手のカウンター
int co3 = 0; //AIの連続王手の千日手のカウンター


int getmove(int board[][5], int outbo[], int x, int y, int xx, int yy, int h, int k){

  if(h == 0){ // 自番手

    if(board[x][y] != k){ //動かす駒が現在地にいない場合なので不正な手.
      return 0; //　返り値 0で不正を表す.
    }

    if(board[xx][yy] == 0){
      board[x][y] = 0;
      board[xx][yy] = k;
      return 1; // 返り値 1は成功
    }

    if(board[xx][yy] <= 10){ //自駒とブッキングしている場合は不正.
      return 0;
    }

    if(board[xx][yy] > 10){ //敵駒が取れる場合.いい書き方を考えたくないので場合分けします。

      if(board[xx][yy] == 11){
        if(outbo[0] == 0) outbo[0] = 1;
          else outbo[1] = 1;
      }
      if(board[xx][yy] == 12){
          if(outbo[2] == 0) outbo[2] = 2;
          else outbo[3] = 2;
      }
      if(board[xx][yy] == 13 || board[xx][yy] == 14){
          if(outbo[4] == 0) outbo[4] = 3;
          else outbo[5] = 3;
      }
      if(board[xx][yy] == 15 || board[xx][yy] == 16){
          if(outbo[6] == 0) outbo[6] = 5;
          else outbo[7] = 5;
      }
      if(board[xx][yy] == 17 || board[xx][yy] == 18){
          if(outbo[8] == 0) outbo[8] = 7;
          else outbo[9] = 7;
      }
      if(board[xx][yy] == 19 || board[xx][yy] == 20){
          if(outbo[10] == 0) outbo[10] = 9;
          else outbo[11] = 9;
      }

      board[xx][yy] = k;
      board[x][y] = 0;
      return 1;
    }
  }

  else{ //敵番手

    if(board[x][y] != k){
      return 0;
    }

    if(board[xx][yy] == 0){
      board[x][y] = 0;
      board[xx][yy] = k;
      return 1;
    }

    if(board[xx][yy] > 10){
      return 0;
    }

    if(board[xx][yy] <= 10){

      if(board[xx][yy] == 1){
        if(outbo[0] == 0) outbo[0] = 11;
          else outbo[1] = 11;
      }
      if(board[xx][yy] == 2){
          if(outbo[2] == 0) outbo[2] = 12;
          else outbo[3] = 12;
      }
      if(board[xx][yy] == 3 || board[xx][yy] == 4){
          if(outbo[4] == 0) outbo[4] = 13;
          else outbo[5] = 13;
      }
      if(board[xx][yy] == 5 || board[xx][yy] == 6){
          if(outbo[6] == 0) outbo[6] = 15;
          else outbo[7] = 15;
      }
      if(board[xx][yy] == 7 || board[xx][yy] == 8){
          if(outbo[8] == 0) outbo[8] = 17;
          else outbo[9] = 17;
      }
      if(board[xx][yy] == 9 || board[xx][yy] == 10){
          if(outbo[10] == 0) outbo[10] = 19;
          else outbo[11] = 19;
      }

      board[xx][yy] = k;
      board[x][y] = 0;
      return 1;
    }
  }
  exit(1);
}

int move(int board[][5], int outbo[], int x, int y, int xx, int yy, int h, int k){

  if(h == 0){ // 自番手

    if(board[x][y] != k){ //動かす駒が現在地にいない場合なので不正な手.
      return 0; //　返り値 0で不正を表す.
    }

    if(board[xx][yy] == 0){
      board[x][y] = 0;
      board[xx][yy] = k;
      return 1; // 返り値 1は成功
    }

    if(board[xx][yy] <= 10){ //自駒とブッキングしている場合は不正.
      return 0;
    }

    if(board[xx][yy] >= 10){ //敵駒が取れる場合.いい書き方を考えたくないので場合分けします。

      if(board[xx][yy] == 11){
        if(outbo[0] == 0) outbo[0] = 1;
	      else outbo[1] = 1;
      }
      if(board[xx][yy] == 12){
	      if(outbo[2] == 0) outbo[2] = 2;
	      else outbo[3] = 2;
      }
      if(board[xx][yy] == 13 || board[xx][yy] == 14){
	      if(outbo[4] == 0) outbo[4] = 3;
	      else outbo[5] = 3;
      }
      if(board[xx][yy] == 15 || board[xx][yy] == 16){
	      if(outbo[6] == 0) outbo[6] = 5;
	      else outbo[7] = 5;
      }
      if(board[xx][yy] == 17 || board[xx][yy] == 18){
	      if(outbo[8] == 0) outbo[8] = 7;
	      else outbo[9] = 7;
      }
      if(board[xx][yy] == 19 || board[xx][yy] == 20){
	      if(outbo[10] == 0) outbo[10] = 9;
	      else outbo[11] = 9;
      }

      board[xx][yy] = k;
      board[x][y] = 0;
      return 1;
    }
  }

  else{ //敵番手

    if(board[x][y] != k){ 
      return 0; 
    }

    if(board[xx][yy] == 0){
      board[x][y] = 0;
      board[xx][yy] = k;
      return 1; 
    }

    if(board[xx][yy] >= 10){ 
      return 0;
    }

    if(board[xx][yy] <= 10){ 

      if(board[xx][yy] == 1){
        if(outbo[0] == 0) outbo[0] = 11;
	      else outbo[1] = 11;
      }
      if(board[xx][yy] == 2){
	      if(outbo[2] == 0) outbo[2] = 12;
	      else outbo[3] = 12;
      }
      if(board[xx][yy] == 3 || board[xx][yy] == 4){
	      if(outbo[4] == 0) outbo[4] = 13;
	      else outbo[5] = 13;
      }
      if(board[xx][yy] == 5 || board[xx][yy] == 6){
	      if(outbo[6] == 0) outbo[6] = 15;
	      else outbo[7] = 15;
      }
      if(board[xx][yy] == 7 || board[xx][yy] == 8){
	      if(outbo[8] == 0) outbo[8] = 17;
	      else outbo[9] = 17;
      }
      if(board[xx][yy] == 9 || board[xx][yy] == 10){
	      if(outbo[10] == 0) outbo[10] = 19;
	      else outbo[11] = 19;
      }

      board[xx][yy] = k;
      board[x][y] = 0;
      return 1;
    }
  }
  exit(1);
}

int nari(int y, int yy, int h){
  if (y == 4-4*h || yy == 4-4*h) return 1;
  else return 0;
}

int sennitite(STATE_BIT history[], STATE_BIT state_bit, int co){
  int cnt = 0;
  for(int i = 0; i < co; i++){
    if (history[i].huhi == state_bit.huhi && history[i].kakugin == state_bit.kakugin && history[i].kinou == state_bit.kinou){
      cnt++;
    } 
  }
  if (cnt>=3) return 1;
  return 0;
}

void init_board(int source_board[][5], int source_outbo[], STATE_BOARD *copy){

  int j, k;

  for(j = 0; j < 5; j++){
    for(k = 0; k < 5; k++){
      copy->board[j][k] = source_board[j][k];
    }
  }
  for(j = 0; j < 12; j++){
    copy->outbo[j] = source_outbo[j];
  }
}

STATE_BOARD bit_to_board(STATE_BIT state_bit){
  STATE_BOARD state_board;//これをreturnする
  int i, j;

  for(i = 0; i < 12; i++){
    state_board.outbo[i] = 0;
  }
  for(i = 0; i < 5; i++){
    for(j = 0; j < 5; j++){
      state_board.board[i][j] = 0;
    }
  }
  //初期化

  for(i = 0; i < 12; i++){
    unsigned int place;
    unsigned long int bit;
    if(i < 4){
      bit = state_bit.kinou;
      place = (bit >> (7 * i)) & 0b1111111;
    }else if(i < 8){
      bit = state_bit.kakugin;
      place = (bit >> (7 * (i - 4))) & 0b1111111;
    }else{
      bit = state_bit.huhi;
      place = (bit >> (7 * (i - 8))) & 0b1111111;
    }

    if((place >> 6) == 0){//味方だったら
      if((place & 0b11111) == 25){//盤上になかったら
    state_board.outbo[i] = i / 2 + 1 + (i > 5) * ((i - 6) / 2 + 1);
      }else if(((place >> 5) % 2) == 1){//成っていたら
    state_board.board[(place & 0b11111) % 5][(place & 0b11111) / 5] = i / 2 + 2 + (i > 5) * ((i - 6) / 2 + 1);
      }else{
    state_board.board[(place & 0b11111) % 5][(place & 0b11111) / 5] = i / 2 + 1 + (i > 5) * ((i - 6) / 2 + 1);
      }
    }else{//敵だったら
      if((place & 0b11111) == 25){
    state_board.outbo[i] = i / 2 + 11 + (i > 5) * ((i - 6) / 2 + 1);
      }else if(((place >> 5) % 2) == 1){
    state_board.board[(place  & 0b11111) % 5][(place & 0b11111) / 5] = i / 2 + 12 + (i > 5) * ((i - 6) / 2 + 1);
      }else{
    state_board.board[(place & 0b11111) % 5][(place & 0b11111) / 5] = i / 2 + 11 + (i > 5) * ((i - 6) / 2 + 1);
      }
    }
  }
  return state_board;
}

STATE_BIT board_to_bit(STATE_BOARD state_board){
    STATE_BIT state_bit;
    state_bit.huhi = 0;
    state_bit.kakugin = 0;
    state_bit.kinou = 0;

    int count[6] = {0, 2, 0, 2, 0, 2};
    int i, j;

    for(i = 0; i < 5; i++){
        for(j = 0; j < 5; j++){
            if(state_board.board[i][j] == 0){
                continue;
            }else if(state_board.board[i][j] % 10 == 1 || state_board.board[i][j] % 10 == 2){
                unsigned int place = 0;
                place = (state_board.board[i][j] > 10) << 6;//敵or味方
                place |= 5 * j + i;
                if(state_board.board[i][j] % 10 == 1){//王
                    state_bit.kinou |= place << (7 * count[0]);
                    count[0] ++;
                }else{//金
                    state_bit.kinou |= place << (7 * count[1]);
                    count[1] ++;
                }
            }else if(state_board.board[i][j] % 10 >= 3 && state_board.board[i][j] % 10 <= 6){
                unsigned int place = 0;
                place = (state_board.board[i][j] > 10) << 6; //敵or味方
                place |= 5 * j + i;//場所
                place |= ((state_board.board[i][j] != 2) * ((state_board.board[i][j]) % 2 == 0)) << 5;//成っているか
                if(state_board.board[i][j] % 10 == 3 || state_board.board[i][j] % 10 == 4){//銀
                    state_bit.kakugin |= place << (7 * count[2]);
                    count[2] ++;
                }else{//角
                    state_bit.kakugin |= place << (7 * count[3]);
                    count[3] ++;
                }
            }else{
                unsigned int place = 0;
                place = (state_board.board[i][j] > 10) << 6;
                place |= 5 * j + i;
                place |= ((state_board.board[i][j] != 2) * ((state_board.board[i][j]) % 2 == 0)) << 5;
                if(state_board.board[i][j] % 10 == 7 || state_board.board[i][j] % 10 == 8){//飛
                    state_bit.huhi |= place << (7 * count[4]);
                    count[4] ++;
                }else{//歩
                    state_bit.huhi |= place << (7 * count[5]);
                    count[5] ++;
                }
            }
        }
    }
    for(i = 0; i < 12; i++){
        if(i < 4){
            if(state_board.outbo[i] > 10){
                state_bit.kinou |= 0b1011001 << (7 * count[i / 2]);
                count[i / 2] ++;
            }else if(state_board.outbo[i] > 0){
                state_bit.kinou |= 0b0011001 << (7 * count[i / 2]);
                count[i / 2] ++;
            }
        }else if(i < 8){
            if(state_board.outbo[i] > 10){
                state_bit.kakugin |= 0b1011001 << (7 * count[i / 2]);
                count[i / 2] ++;
            }else if(state_board.outbo[i] > 0){
                state_bit.kakugin |= 0b0011001 << (7 * count[i / 2]);
                count[i / 2] ++;
            }
        }else{
            if(state_board.outbo[i] > 10){
                state_bit.huhi |= 0b1011001 << (7 * count[i / 2]);
                count[i / 2] ++;
            }else if(state_board.outbo[i] > 0){
                state_bit.huhi |= 0b0011001 << (7 * count[i / 2]);
                count[i / 2] ++;
            }
        }
    }
    return state_bit;
}

int oute(int board[][5], int outbo[], int h){
    
    //h = 0なら1(自分の王)が取られるそうか否か、取られそうなら1を返す
    
    int x,y;
    for(int xx = 0; xx < 5; xx++){
        for(int yy = 0; yy < 5; yy++){
            if(h == 0){
                if(board[xx][yy] == 1){
                    x = xx, y = yy;
                }
            }
            else{
                if(board[xx][yy] == 11){
                    x = xx, y = yy;
                }
            }
        }
    }
    if(h == 0){
        for(int i=0; i<5; i++){
            for(int j=0; j<5; j++){
                int k = board[i][j];

                if(k == 11){ //王
                    int dx = abs(i-x);
                    int dy = abs(j-y);
                    if (dx <= 1 && dy <= 1) return 1;
                }

                if(k == 12 || k == 14 || k == 20){ //金・成銀・と金
                    int dx = abs(i-x);
                    int dy = abs(j-y);
                    if (dx <= 1 && dy <= 1){
                        if (!(x != i && y-j == 1)) return 1;
                    }
                }

                if(k == 13){ //銀
                    int dx = abs(i-x);
                    int dy = abs(j-y);
                    if (dx <= 1 && dy <= 1){
                        if (!(x == i && y-j == 1)){
                            if (y != j) return 1;
                        }
                    }
                }

                if(k == 15){ //角
                    int dx = i-x;
                    int dy = j-y;
                    int f2 = 1;
                    if (abs(dx) != abs(dy)) continue;
                    if (dx < 0){
                        if (dy < 0){
                            for (int l = -1; l > dx; l--){
                                if (board[x+l][y+l] != 0){
                                    f2 = 0;
                                    break;
                                }
                            }
                        }
                        else{
                            for (int l = -1; l > dx; l--){
                                if (board[x+l][y-l] != 0){
                                    f2 = 0;
                                    break;
                                }
                            }
                        }
                    }
                    else{
                        if (dy < 0){
                            for (int l = 1; l < dx; l++){
                                if (board[x+l][y-l] != 0){
                                    f2 = 0;
                                    break;
                                }
                            }
                        }
                        else{
                            for (int l = 1; l < dx; l++){
                                if (board[x+l][y+l] != 0){
                                    f2 = 0;
                                    break;
                                }
                            }
                        }
                    }
                    if(f2 == 1) return 1;
                }

                if(k == 16){ //馬
                    int dx = i-x;
                    int dy = j-y;
                    int f2 = 1;
                    if (abs(dx) != abs(dy)){
                        if (abs(abs(dx) - abs(dy)) == 1 && abs(dx) * abs(dy) == 0) return 1;
                        else continue;
                    }
                    if (dx < 0){
                        if (dy < 0){
                            for (int l = -1; l > dx; l--){
                                if (board[x+l][y+l] != 0){
                                    f2 = 0;
                                    break;
                                }
                            }
                        }
                        else{
                            for (int l = -1; l > dx; l--){
                                if (board[x+l][y-l] != 0){
                                    f2 = 0;
                                    break;
                                }
                            }
                        }
                    }
                    else{
                        if (dy < 0){
                            for (int l = 1; l < dx; l++){
                                if (board[x+l][y-l] != 0){
                                    f2 = 0;
                                    break;
                                }
                            }
                        }
                        else{
                            for (int l = 1; l < dx; l++){
                                if (board[x+l][y+l] != 0){
                                    f2 = 0;
                                    break;
                                }
                            }
                        }
                    }
                    if(f2 == 1) return 1;
                }

                if(k == 17){ //飛車
                    int f2 = 1;
                    if(x != i && y != j){
                        continue;
                    }

                    if(x != i){
                        int s = min(x, i);
                        int g = max(x, i);

                        for(int l = s + 1; l < g; l++){
                            if(board[l][y] != 0){
                                f2 = 0;
                                break;
                            }
                        }
                    }

                    if(y != j){
                        int s = min(y, j);
                        int g = max(y, j);

                        for(int l = s + 1; l < g; l++){
                            if(board[x][l] != 0){
                                f2 = 0;
                                break;
                            }
                        }
                    }
                    if(f2 == 1) return 1;
                }

                if(k == 18){ //龍
                    int f2 = 1;
                    if(x != i && y != j){
                        if(abs(x - i) * abs(y - j) == 1) return 1;
                        else continue;
                    }

                    if(x != i){
                        int s = min(x, i);
                        int g = max(x, i);

                        for(int l = s + 1; l < g; l++){
                            if(board[l][y] != 0){
                                f2 = 0;
                                break;
                        
                            }
                        }
                    }
                    
                    if(y != j){
                        int s = min(y, j);
                        int g = max(y, j);

                        for(int l = s + 1; l < g; l++){
                            if(board[x][l] != 0){
                                f2 = 0;
                            }
                        }
                    }
                    if(f2 == 1) return 1;
                    
                }

                if(k == 19){ //歩
                    if(x == i && y+1 == j) return 1;
                }
            }
        }
    }
    else{
        for(int i=0; i < 5; i++){
            for(int j=0; j < 5; j++){
                int k = board[i][j];

                if(k == 1){ //王
                    int dx = abs(i-x);
                    int dy = abs(j-y);
                    if (dx <= 1 && dy <= 1) return 1;
                }

                if(k == 2 || k == 4 || k == 10){ //金・成銀・と金
                    int dx = abs(i-x);
                    int dy = abs(j-y);
                    if (dx <= 1 && dy <= 1){
                        if (!(x != i && j-y == 1)) return 1;
                    }
                }

                if(k == 3){ //銀
                    int dx = abs(i-x);
                    int dy = abs(j-y);
                    if (dx <= 1 && dy <= 1){
                        if (!(x == i && j-y == 1)){
                            if (y != j) return 1;
                        }
                    }
                }

                if(k == 5){ //角
                    int dx = i-x;
                    int dy = j-y;
                    int f2 = 1;
                    if (abs(dx) != abs(dy)) f2 = 0;
                    if (dx < 0){
                        if (dy < 0){
                            for (int l = -1; l > dx; l--){
                                if (board[x+l][y+l] != 0){
                                    f2 = 0;
                                    break;
                                }
                            }
                        }
                        else{
                            for (int l = -1; l > dx; l--){
                                if (board[x+l][y-l] != 0){
                                    f2 = 0;
                                    break;
                                }
                            }
                        }
                    }
                    else{
                        if (dy < 0){
                            for (int l = 1; l < dx; l++){
                                if (board[x+l][y-l] != 0){
                                    f2 = 0;
                                    break;
                                }
                            }
                        }
                        else{
                            for (int l = 1; l < dx; l++){
                                if (board[x+l][y+l] != 0){
                                    f2 = 0;
                                    break;
                                }
                            }
                        }
                    }
                    if(f2 == 1) return 1;
                }

                if(k == 6){ //馬
                    int dx = i-x;
                    int dy = j-y;
                    int f2 = 1;
                    if (abs(dx) != abs(dy)){
                        if (abs(dx) <= 1 && abs(dy) <= 1) return 1;
                        else continue;
                    }
                    if (dx < 0){
                        if (dy < 0){
                            for (int l = -1; l > dx; l--){
                                if (board[x+l][y+l] != 0){
                                    f2 = 0;
                                }
                            }
                        }
                        else{
                            for (int l = -1; l > dx; l--){
                                if (board[x+l][y-l] != 0){
                                    f2 = 0;
                                }
                            }
                        }
                    }
                    else{
                        if (dy < 0){
                            for (int l = 1; l < dx; l++){
                                if (board[x+l][y-l] != 0){
                                    f2 = 0;
                                }
                            }
                        }
                        else{
                            for (int l = 1; l < dx; l++){
                                if (board[x+l][y+l] != 0){
                                    f2 = 0;
                                }
                            }
                        }
                    }
                    if(f2 == 1) return 1;
                }

                if(k == 7){ //飛車
                    int f2 = 1;
                    if(x != i && y != j){
                        continue;
                    }

                    if(x != i){
                        int s = min(x, i);
                        int g = max(x, i);

                        for(int l = s + 1; l < g; l++){
                            if(board[l][y] != 0) f2 = 0;
                        }
                    }

                    if(y != j){
                        int s = min(y, j);
                        int g = max(y, j);

                        for(int l = s + 1; l < g; l++){
                            if(board[x][l] != 0) f2 = 0;
                        }
                    }
                    if(f2 == 1) return 1;
                }

                if(k == 8){ //龍
                    int f2 = 1;
                    if(x != i && y != j){
                        if(abs(x - i) * abs(y - j) == 1) return 1;
                        else continue;
                    }

                    if(x != i){
                        int s = min(x, i);
                        int g = max(x, i);

                        for(int l = s + 1; l < g; l++){
                            if(board[l][y] != 0){
                                f2 = 0;
                                break;
                            }
                        }
                    }

                    if(y != j){
                        int s = min(y, j);
                        int g = max(y, j);

                        for(int l = s + 1; l < g; l++){
                            if(board[x][l] != 0){
                                f2 = 0;
                                break;
                            }
                        }
                    }
                    if(f2 == 1) return 1;
                }

                if(k == 9){ //歩
                    if(x == i && y-1 == j) return 1;
                }
            }
        }
    }
    return 0;
}

int oushou(int board[][5], int outbo[], int x, int y, int xx, int yy, int h, int n){

  int dx = abs(xx - x);
  int dy = abs(yy - y);
  if(!(dx + dy == 1 || dx * dy == 1)){ //不正な移動を検出.
    return 0;
  }

  if(n != 0){ //王は成ることが出来ないので不正手.
    return 0;
  }

  int f = 0;
  if(h == 0) f = getmove(board, outbo, x, y, xx, yy, h, 1);// 自駒でも敵駒でも動くように作りました.
  else f = getmove(board, outbo, x, y, xx, yy, h, 11);

  return f; //moveの返り値を引き継ぐ.
}

int hisha(int board[][5], int outbo[], int x, int y, int xx, int yy, int h, int n){
  
  if(x != xx && y != yy){
    return 0;
  }

  if(x == xx && y == yy){
    return 0;
  }

  if(x != xx){ // x側の移動,移動経路に駒がないかを見ます. oso角gみたいなケースを弾く.
    int s = min(x, xx);
    int g = max(x, xx);

    for(int i = s + 1; i < g; i++){

      if(board[i][y] != 0){ //駒があるケース.不正な手.
    return 0;
      }
    }
  }


  if(y != yy){
    int s = min(y, yy);
    int g = max(y, yy);

    for(int i = s + 1; i < g; i++){

      if(board[x][i] != 0){ //駒があるケース.不正な手.
          return 0;
      }
    }
  }

  if(n == 0){

    int f = 0;
    if(h == 0){
      f = getmove(board, outbo, x, y, xx, yy, h, 7);
    }
    else{
      f = getmove(board, outbo, x, y, xx, yy, h, 17);
    }
    return f;
  }
  else{

    int f = 0;
    if(h == 0) f = getmove(board, outbo, x, y, xx, yy, h, 7);
    else f = getmove(board, outbo, x, y, xx, yy, h, 17);

    int ff = nari(y, yy, h); //成ることが可能かをこれで見たい.
    f = f * ff; //両方 1の場合のみ成功.
      if(ff == 1){
          board[xx][yy] = 8 + 10 * (h == 1);
      }
    return f;
  }
}

int hu(int board[][5], int outbo[], int x, int y, int xx, int yy, int h, int n){

  if (!((y + (1 - 2 * h) == yy) && x == xx)){//不正な手ははじく
    return 0;
  }

  if(n == 0){
    int f = 0;
      if(yy == 4-4*h){ //歩を成らなかった
        return 0;
      }
    if(h == 0) f = getmove(board, outbo, x, y, xx, yy, h, 9);
    else f = getmove(board, outbo, x, y, xx, yy, h, 19);
    return f;
  }

  else{
    int f = 0;
    if(h == 0) f = getmove(board, outbo, x, y, xx, yy, h, 9);
    else f = getmove(board, outbo, x, y, xx, yy, h, 19);

    int ff = nari(y, yy, h);
    f = f * ff;
      if(ff == 1){
          board[xx][yy] = 10 + 10 * (h == 1);
      }
    return f;
  }
}


int kin(int board[][5], int outbo[], int x, int y, int xx, int yy, int h, int n){
  int dx = abs(x - xx), dy = abs(y - yy);
  if(!(dx + dy == 1 || (dx * dy == 1 && (y + 1 - 2 * h == yy)))){
    return 0;
  }
  if(n != 0){
    return 0;
  }

  int f = 0;
  if(h == 0) f = getmove(board, outbo, x, y, xx, yy, h, 2);
  else f = getmove(board, outbo, x, y, xx, yy, h, 12);
  return f;
}


int tokin(int board[][5], int outbo[], int x, int y, int xx, int yy, int h, int n){

  int dx = abs(x - xx), dy = abs(y - yy);

  if(!(dx + dy == 1 || (dx * dy == 1 && (y + 1 - 2 * h == yy)))){
    return 0;
  }

  if(n != 0){
    return 0;
  }
  
  int f = 0;
  if(h == 0) f = getmove(board, outbo, x, y, xx, yy, h, 10);
  else f = getmove(board, outbo, x, y, xx, yy, h, 20);
  return f;
}


int gin(int board[][5], int outbo[], int x, int y, int xx, int yy, int h, int n){

  int dx = abs(x - xx), dy = abs(y - yy);

  if (!(dx * dy == 1 || ((dx == 0) && yy == y + (1 - 2 * h)))){
    return 0;
  }

  if (n == 0){
    int f = 0;
    if(h == 0) f = getmove(board, outbo, x, y, xx, yy, h, 3);
    else f = getmove(board, outbo, x, y, xx, yy, h, 13);
    return f;
  }

  else{
    int f = 0;
    if(h == 0) f = getmove(board, outbo, x, y, xx, yy, h, 3);
    else f = getmove(board, outbo, x, y, xx, yy, h, 13);
    
    int ff = nari(y, yy, h);
    f = f * ff;
      if(ff == 1){
          board[xx][yy] = 4 + 10 * (h == 1);
      }
    return f;
  }
}

int ginnari(int board[][5], int outbo[], int x, int y, int xx, int yy, int h, int n){

  int dx = abs(x - xx), dy = abs(y - yy);

  if(!(dx + dy ==1 || (dx * dy == 1 && (y + 1 - 2 * h == yy)))){
    return 0;
  }

  if(n != 0){
    return 0;
  }

  int f = 0;
  if(h == 0) f = getmove(board, outbo, x, y, xx, yy, h, 4);
  else f = getmove(board, outbo, x, y, xx, yy, h, 14);
  return f;
}


int kaku(int board[][5], int outbo[], int x, int y, int xx, int yy, int h, int n){

  int dx = xx - x, dy = yy - y;
  int i;

  if (abs(dx) != abs(dy)){
    return 0;
  }
  if (dx == 0){
    return 0;
  }
  if (dx < 0){
    if (dy < 0){
      for (i = -1; i > dx; i--){
    if (board[x+i][y+i] != 0){
      return 0;
    }
      }
    }
    else{
      for (i = -1; i > dx; i--){
        if (board[x+i][y-i] != 0){
          return 0;
        }
      }
    }
  }
  else{
    if (dy < 0){
      for (i = 1; i < dx; i++){
        if (board[x+i][y-i] != 0){
          return 0;
        }
      }
    }
    else{
      for (i = 1; i < dx; i++){
    if (board[x+i][y+i] != 0){
          return 0;
        }
      }
    }
  }

  if (n == 0){
    int f = 0;
    if(h == 0) f = getmove(board, outbo, x, y, xx, yy, h, 5);
    else f = getmove(board, outbo, x, y, xx, yy, h, 15);
    return f;
  }

  else{
    int f = 0;
    if(h == 0) f = getmove(board, outbo, x, y, xx, yy, h, 5);
    else f = getmove(board, outbo, x, y, xx, yy, h, 15);

    int ff = nari(y, yy, h);
    f = f * ff;
      if(ff == 1){
          board[xx][yy] = 6 + 10 * (h == 1);
      }
    return f;
  }
}


int ryuu(int board[][5], int outbo[], int x, int y, int xx, int yy, int h, int n){

  if (n == 1){
    return 0;
  }
  if (x != xx && y != yy && abs(x - xx) * abs(y - yy) != 1){
    return 0;
  }
  if (x == xx && y == yy){
    return 0;
  }
  if (x == xx){
    int s = min(y, yy);
    int g = max(y, yy);

    for(int i=s+1; i<g; i++){

      if(board[x][i] != 0){ //駒があるケース.不正な手.
    return 0;
      }
    }
  }
  if (y == yy){
    int s = min(x, xx);
    int g = max(x, xx);

    for(int i=s+1; i<g; i++){

      if(board[i][y] != 0){ //駒があるケース.不正な手.
    return 0;
      }
    }
  }

  int f = 0;
  if (h == 0) f = getmove(board, outbo, x, y, xx, yy, h, 8);
  else f = getmove(board, outbo, x, y, xx, yy, h, 18);
  return f;
}


int uma(int board[][5], int outbo[], int x, int y, int xx, int yy, int h, int n){

  int dx = x - xx, dy = y - yy;
  int i;

  if (n == 1){
    return 0;
  }
  if (!(dx == 0 && abs(dy) == 1) && !(abs(dx) == 1 && dy == 0)){
    if (abs(dx) != abs(dy)){
      return 0;
    }
    if (dx == 0){
      return 0;
    }
    if (dx < 0){
        if (dy < 0){
            for (i = -1; i > dx; i--){
                if (board[x-i][y-i] != 0){
                    return 0;
                }
            }
        }
        else{
            for (i = -1; i > dx; i--){
                if (board[x-i][y+i] != 0){
                    return 0;
                }
            }
        }
    }
    else{
        if (dy < 0){
            for (i = 1; i < dx; i++){
                if (board[x-i][y+i] != 0){
                    return 0;
                }
            }
        }
        else{
            for (i = 1; i < dx; i++){
                if (board[x-i][y-i] != 0){
                    return 0;
                }
            }
        }
    }
  }

  int f = 0;
  if(h == 0) f = getmove(board, outbo, x, y, xx, yy, h, 6);
  else f = getmove(board, outbo, x, y, xx, yy, h, 16);
  return f;
}

int huriwake(int board[][5], int outbo[], int x, int y, int xx, int yy, int h, int k, int n){
  int f = 0;
  if(outbo[0]%10==1 || outbo[1]%10==1) return 0;
  if (!(x>=0 && x<=4 && y>=0 && y<=4 && xx>=0 && xx<=4 && yy>=0 && yy<=4)){//盤上の移動かどうか判定
    f = 0;
  }

  if(k == 0){
    f = 0;
  }
  if(k == 1 || k == 11){
    f = oushou(board, outbo, x, y, xx, yy, h, n);
  }

  if(k == 2 || k == 12){
    f = kin(board, outbo, x, y, xx, yy, h, n);
  }

  if(k == 3 || k == 13){
    f = gin(board, outbo, x, y, xx, yy, h, n);
  }

  if(k == 4 || k == 14){
    f = ginnari(board, outbo, x, y, xx, yy, h, n);
  }

  if(k == 5 || k == 15){
    f = kaku(board, outbo, x, y, xx, yy, h, n);
  }

  if(k == 6 || k == 16){
    f = uma(board, outbo, x, y, xx, yy, h, n);
  }

  if(k == 7 || k == 17){
    f = hisha(board, outbo, x, y, xx, yy, h, n);
  }

  if(k == 8 || k == 18){
    f = ryuu(board, outbo, x, y, xx, yy, h, n);
  }

  if(k == 9 || k == 19){
    f = hu(board, outbo, x, y, xx, yy, h, n);
  }

  if(k == 10 || k == 20){
    f = tokin(board, outbo, x, y, xx, yy, h, n);
  }

  STATE_BOARD now_board;
  init_board(board, outbo, &now_board);
  STATE_BIT now_bit = board_to_bit(now_board);
  if(sennitite(history,now_bit,co) && (h == 0 || co3>=5 && oute(board, outbo, 0))){
    f = 0;
  }
  return f;
  puts("error_huriwake");
  exit(1);
}

int oushou1(int board[][5], int outbo[], int x, int y, int xx, int yy, int h, int n){

  int dx = abs(xx - x);
  int dy = abs(yy - y);
  if(!(dx + dy == 1 || dx * dy == 1)){ //不正な移動を検出.
    return 0;
  }

  return 1; 
}

int hisha1(int board[][5], int outbo[], int x, int y, int xx, int yy, int h, int n){
  
  if(x != xx && y != yy){
    return 0;
  }

  if(x == xx && y == yy){
    return 0;
  }

  if(x != xx){ // x側の移動,移動経路に駒がないかを見ます. oso角gみたいなケースを弾く.
    int s = min(x, xx);
    int g = max(x, xx);

    for(int i = s + 1; i < g; i++){

      if(board[i][y] != 0){ //駒があるケース.不正な手.
        return 0;
      }
    }
  }


  if(y != yy){
    int s = min(y, yy);
    int g = max(y, yy);

    for(int i = s + 1; i < g; i++){

      if(board[x][i] != 0){ //駒があるケース.不正な手.
          return 0;
      }
    }
  }

    return 1;
}

int hu1(int board[][5], int outbo[], int x, int y, int xx, int yy, int h, int n){

  if (!((y + (1 - 2 * h) == yy) && x == xx)){//不正な手ははじく
    return 0;
  }
    return 1;
}


int kin1(int board[][5], int outbo[], int x, int y, int xx, int yy, int h, int n){

  int dx = abs(x - xx), dy = abs(y - yy);
  if(!(dx + dy == 1 || (dx * dy == 1 && (y + 1 - 2 * h == yy)))){
    return 0;
  }
  
  return 1;
}


int tokin1(int board[][5], int outbo[], int x, int y, int xx, int yy, int h, int n){

  int dx = abs(x - xx), dy = abs(y - yy);

  if(!(dx + dy == 1 || (dx * dy == 1 && (y + 1 - 2 * h == yy)))){
    return 0;
  }

  return 1;
}


int gin1(int board[][5], int outbo[], int x, int y, int xx, int yy, int h, int n){

  int dx = abs(x - xx), dy = abs(y - yy);

  if (!(dx * dy == 1 || ((dx == 0) && yy == y + (1 - 2 * h)))){
    return 0;
  }

    return 1;
}

int ginnari1(int board[][5], int outbo[], int x, int y, int xx, int yy, int h, int n){

  int dx = abs(x - xx), dy = abs(y - yy);

  if(!(dx + dy ==1 || (dx * dy == 1 && (y + 1 - 2 * h == yy)))){
    return 0;
  }

  return 1;
}


int kaku1(int board[][5], int outbo[], int x, int y, int xx, int yy, int h, int n){

  int dx = xx - x, dy = yy - y;
  int i;

  if (abs(dx) != abs(dy)){
    return 0;
  }
  if (dx == 0){
    return 0;
  }
  if (dx < 0){
    if (dy < 0){
      for (i = -1; i > dx; i--){
    if (board[x+i][y+i] != 0){
      return 0;
    }
      }
    }
    else{
      for (i = -1; i > dx; i--){
        if (board[x+i][y-i] != 0){
          return 0;
        }
      }
    }
  }
  else{
    if (dy < 0){
      for (i = 1; i < dx; i++){
        if (board[x+i][y-i] != 0){
          return 0;
        }
      }
    }
    else{
      for (i = 1; i < dx; i++){
    if (board[x+i][y+i] != 0){
          return 0;
        }
      }
    }
  }

  return 1;
}


int ryuu1(int board[][5], int outbo[], int x, int y, int xx, int yy, int h, int n){

  if (x != xx && y != yy && abs(x - xx) * abs(y - yy) != 1){
    return 0;
  }
  if (x == xx && y == yy){
    return 0;
  }
  if (x == xx){
    int s = min(y, yy);
    int g = max(y, yy);

    for(int i=s+1; i<g; i++){

      if(board[x][i] != 0){ //駒があるケース.不正な手.
    return 0;
      }
    }
  }
  if (y == yy){
    int s = min(x, xx);
    int g = max(x, xx);

    for(int i=s+1; i<g; i++){

      if(board[i][y] != 0){ //駒があるケース.不正な手.
    return 0;
      }
    }
  }

  return 1;
}


int uma1(int board[][5], int outbo[], int x, int y, int xx, int yy, int h, int n){

  int dx = x - xx, dy = y - yy;
  int i;

  if (!(dx == 0 && abs(dy) == 1) && !(abs(dx) == 1 && dy == 0)){
    if (abs(dx) != abs(dy)){
      return 0;
    }
    if (dx == 0){
      return 0;
    }
    if (dx < 0){
        if (dy < 0){
            for (i = -1; i > dx; i--){
                if (board[x-i][y-i] != 0){
                    return 0;
                }
            }
        }
        else{
            for (i = -1; i > dx; i--){
                if (board[x-i][y+i] != 0){
                    return 0;
                }
            }
        }
    }
    else{
        if (dy < 0){
            for (i = 1; i < dx; i++){
                if (board[x-i][y+i] != 0){
                    return 0;
                }
            }
        }
        else{
            for (i = 1; i < dx; i++){
                if (board[x-i][y-i] != 0){
                    return 0;
                }
            }
        }
    }
  }

  return 1;
}

int putboard(int board[][5], int outbo[], int x, int y, int h, int k){
    if(outbo[0]%10==1 || outbo[1]==10==1) return 0;
  int f = 0;
  int j = 0;
  for(int i=0; i<12; i++){
    if(outbo[i] == k){
      j = i;
      f = 1;
      break;
    }
  }

  if(board[x][y] == 0 && f == 1){

    board[x][y] = k;
    outbo[j] = 0;

    if(k == 9 || k == 19){ //歩を打った場合
      
      if(oute(board, outbo, 1-h)){ //打ち歩詰め
        f = 0;
      }

      int i, j;

      for(int ii = 0; ii < 5; ii++){ //王の位置
        for(int jj = 0; jj < 5; jj++){
          if (board[ii][jj] == 11-10*h) {
            i = ii;
            j = jj;
          }
        }
      }

      for(int l = -1; l < 2; l++){
        for(int m = -1; m < 2; m++){
          STATE_BOARD new_board;
          init_board(board, outbo, &new_board);
		  
          int f2 = huriwake(new_board.board, new_board.outbo, i, j, i+l, j+m, 1-h, new_board.board[i][j], 0);
          if(f2){
            if(!oute(new_board.board, new_board.outbo, 1-h)) f = 1;
          }              
        }
      }

        for (int i=0; i<5; i++){ 
          if (i != y && board[x][i] == k){ //二歩
            f = 0;
          }
        }

        if(y == 4-4*h){ //敵陣に持ち駒の歩を打った
          f = 0;
        }
    }
  }
  STATE_BOARD now_board;
  init_board(board, outbo, &now_board);
  STATE_BIT now_bit = board_to_bit(now_board);
  if(sennitite(history,now_bit,co) && (h == 0 || co3>=5 && oute(board, outbo, 0))){
    f = 0;
  }
  return f;
  puts("error1");
  exit(1);
}


int finjudge(int board[][5], int outbo[],int h){
  if(oute(board, outbo, 1-h)){

    int f2 = 1;

    for(int i = 0; i < 5; i++){
      for(int j = 0; j < 5; j++){
        if((h == 1 && board[i][j] > 0 && board[i][j] <= 10) || (h == 0 && board[i][j] > 10)){
          for(int l = 0; l < 5; l++){
            for(int m = 0; m < 5; m++){
              for(int n = 0; n < 1; n++){
                STATE_BOARD new_board;
                init_board(board, outbo, &new_board);
                int f = huriwake(new_board.board, new_board.outbo, i, j, l, m, 1-h, new_board.board[i][j], n);
                if(f){
                  if(!oute(new_board.board, new_board.outbo, 1-h)) f2 = 0;
                }
              }
            }
          }
        }
      }
    }

    for(int k = 0; k < 12; k++){
      if((h == 1 && outbo[k] > 0 && outbo[k] <= 10) || (h == 0 && outbo[k] > 10)){
        for(int l = 0;l < 5; l++){
          for(int m = 0; m< 5; m++){
            STATE_BOARD new_board;
            init_board(board, outbo, &new_board);

            int f = putboard(new_board.board, new_board.outbo, l, m, 1-h, new_board.outbo[k]);
            if(f){
              if(!oute(new_board.board, new_board.outbo, 1-h)) f2 = 0;
            }
          }
        }
      }
    }

    return f2;

  }
  return 0;
}

int komaeval[21] = {0, 15000, 485, 445, 485, 770, 850, 890, 1255, 80, 485, -15000, -485, -445, -485, -770, -850, -890, -1255, -80, -485};
int motigomaeval[12] = {0, 0, 540, 540, 495, 495, 855, 855, 945, 945, 90, 90};
int distvalue[9] = {1024, 496, 297, 272, 184, 166, 146, 116, 117};
int distvalue2[9] = {1024, 504, 357, 320, 220, 194, 160, 136, 130};

int dist(int x, int y, int xx, int yy){
  return max(abs(x - xx), abs(y - yy));
}

void komakiki(int board[][5], int outbo[], int bankiki[][5], int bankiki2[][5], int x, int y, int k){

  if(k == 1){
    for(int i=max(0,x-1); i<min(5,x+1); i++){
      for(int j=max(0,y-1); j<min(5,y+1); j++){
        if(oushou1(board,outbo,x,y,i,j,0,0)) bankiki[i][j]++;
      }
    }
  }

  if(k == 2){
    for(int i=max(0,x-1); i<min(5,x+1); i++){
      for(int j=max(0,y-1); j<min(5,y+1); j++){
        if(kin1(board,outbo,x,y,i,j,0,0)) bankiki[i][j]++;
      }
    }
  }

  if(k == 3){
    for(int i=max(0,x-1); i<min(5,x+1); i++){
      for(int j=max(0,y-1); j<min(5,y+1); j++){
        if(gin1(board,outbo,x,y,i,j,0,0)) bankiki[i][j]++;
      }
    }
  }

  if(k == 4){
    for(int i=max(0,x-1); i<min(5,x+1); i++){
      for(int j=max(0,y-1); j<min(5,y+1); j++){
        if(ginnari1(board,outbo,x,y,i,j,0,0)) bankiki[i][j]++;
      }
    }
  }

  if(k == 5){
    for(int i=0; i<5; i++){
      for(int j=0; j<5; j++){
        if(kaku1(board,outbo,x,y,i,j,0,0)) bankiki[i][j]++;
      }
    }
  }

  if(k == 6){
    for(int i=0; i<5; i++){
      for(int j=0; j<5; j++){
        if(uma1(board,outbo,x,y,i,j,0,0)) bankiki[i][j]++;
      }
    }
  }

  if(k == 7){
    for(int i=0; i<5; i++){
      for(int j=0; j<5; j++){
        if(hisha1(board,outbo,x,y,i,j,0,0)) bankiki[i][j]++;
      }
    }
  }

  if(k == 8){
    for(int i=0; i<5; i++){
      for(int j=0; j<5; j++){
        if(ryuu1(board,outbo,x,y,i,j,0,0)) bankiki[i][j]++;
      }
    }
  }

  if(k == 9){
    if(hu1(board,outbo,x,y,x,y+1,0,0)) bankiki[x][y+1]++;
  }

  if(k == 10){
    for(int i=max(0,x-1); i<min(5,x+1); i++){
      for(int j=max(0,y-1); j<min(5,y+1); j++){
        if(tokin1(board,outbo,x,y,i,j,0,0)) bankiki[i][j]++;
      }
    }
  }

  if(k == 11){
    for(int i=max(0,x-1); i<min(5,x+1); i++){
      for(int j=max(0,y-1); j<min(5,y+1); j++){
        if(oushou1(board,outbo,x,y,i,j,1,0)) bankiki2[i][j]++;
      }
    }
  }

  if(k == 12){
    for(int i=max(0,x-1); i<min(5,x+1); i++){
      for(int j=max(0,y-1); j<min(5,y+1); j++){
        if(kin1(board,outbo,x,y,i,j,1,0)) bankiki2[i][j]++;
      }
    }
  }

  if(k == 13){
    for(int i=max(0,x-1); i<min(5,x+1); i++){
      for(int j=max(0,y-1); j<min(5,y+1); j++){
        if(gin1(board,outbo,x,y,i,j,1,0)) bankiki2[i][j]++;
      }
    }
  }

  if(k == 14){
    for(int i=max(0,x-1); i<min(5,x+1); i++){
      for(int j=max(0,y-1); j<min(5,y+1); j++){
        if(ginnari1(board,outbo,x,y,i,j,1,0)) bankiki2[i][j]++;
      }
    }
  }

  if(k == 15){
    for(int i=0; i<5; i++){
      for(int j=0; j<5; j++){
        if(kaku1(board,outbo,x,y,i,j,1,0)) bankiki2[i][j]++;
      }
    }
  }

  if(k == 16){
    for(int i=0; i<5; i++){
      for(int j=0; j<5; j++){
        if(uma1(board,outbo,x,y,i,j,1,0)) bankiki2[i][j]++;
      }
    }
  }

  if(k == 17){
    for(int i=0; i<5; i++){
      for(int j=0; j<5; j++){
        if(hisha1(board,outbo,x,y,i,j,1,0)) bankiki2[i][j]++;
      }
    }
  }

  if(k == 18){
    for(int i=0; i<5; i++){
      for(int j=0; j<5; j++){
        if(ryuu1(board,outbo,x,y,i,j,1,0)) bankiki2[i][j]++;
      }
    }
  }

  if(k == 19){
    if(hu1(board,outbo,x,y,x,y-1,1,0)) bankiki2[x][y+1]++;
  }

  if(k == 20){
    for(int i=max(0,x-1); i<min(5,x+1); i++){
      for(int j=max(0,y-1); j<min(5,y+1); j++){
        if(tokin1(board,outbo,x,y,i,j,1,0)) bankiki2[i][j]++;
      }
    }
  }
}

int evaluate(int board[][5], int outbo[], int prev, int h){

  int score = 0;

  if(h == 0 && (outbo[0] == 1 || outbo[1] == 1)){
      return INT_MAX;
  }

  if(h == 1 && (outbo[0] == 11 || outbo[1] == 11)){
      return INT_MAX;
  }

  if(h == 0 && (outbo[0] == 11 || outbo[1] == 11)){
      return -INT_MAX;
  }

  if(h == 1 && (outbo[0] == 1 || outbo[1] == 1)){
      return -INT_MAX;
  }

  // komanokati

  for(int i=0; i<5; i++){
    for(int j=0; j<5; j++){
      if(1 <= board[i][j] && board[i][j] <= 20){
        score += komaeval[board[i][j]];
      }
    }
  }

  for(int i=2; i<12; i++){
    if(1 <= outbo[i] && outbo[i] <= 10) score += motigomaeval[i];
    else if(outbo[i] != 0) score -= motigomaeval[i];
  }

  if(h == 1) score *= -1;

  // ここまで駒得関数

  // 駒の利き

  int xpos = -10;
  int ypos = -10;
  int xpos1 = -10;
  int ypos1 = -10;

  for(int i=0; i<5; i++){
    for(int j=0; j<5; j++){
      if(board[i][j] == 1){
        xpos = i;
        ypos = j;
      }
    }
  }

  for(int i=0; i<5; i++){
    for(int j=0; j<5; j++){
      if(board[i][j] == 11){
        xpos1 = i;
        ypos1 = j;
      }
    }
  }

  if(xpos == -10 && ypos == -10){
    if(h == 0) return -1e7;
    else return 1e7;
  }

   if(xpos1 == -10 && ypos1 == -10){
    if(h == 0) return 1e7;
    else return -1e7;
  }

  int bankiki[5][5] = {};
  int bankiki2[5][5] = {};

  for(int i=0; i<5; i++){
    for(int j=0; j<5; j++){
      if(board[i][j] != 0){
        komakiki(board,outbo,bankiki,bankiki2,i,j,board[i][j]);
      }
    }
  }
  
  for(int i=0; i<5; i++){
      for(int j=0; j<5; j++){
          int dist = max(abs(xpos - i), abs(ypos - j));
          int s1 = bankiki[i][j] * distvalue[dist] / 1024;
          int s2 = bankiki2[i][j] * distvalue2[dist] / 1024;

          score += h == 0 ? (s1 * 68 - s2 * 96) : (s2 * 96 - s1 * 68);
      }
  }

  for(int i=0; i<5; i++){
      for(int j=0; j<5; j++){
          int dist = max(abs(xpos1 - i), abs(ypos1 - j));
          int s1 = bankiki[i][j] * distvalue[dist] / 1024;
          int s2 = bankiki2[i][j] * distvalue2[dist] / 1024;

          score += h == 0 ? (s1 * 96 - s2 * 68) : (s2 * 68 - s1 * 96);
      }
  }

  return score;
}

void printboard(STATE_BOARD state_board){
  printf("%d %d %d %d %d\n%d %d %d %d %d\n%d %d %d %d %d\n%d %d %d %d %d\n%d %d %d %d %d\n", state_board.board[0][0], state_board.board[0][1], state_board.board[0][2], state_board.board[0][3],state_board.board[0][4], state_board.board[1][0], state_board.board[1][1], state_board.board[1][2], state_board.board[1][3], state_board.board[1][4], state_board.board[2][0], state_board.board[2][1], state_board.board[2][2], state_board.board[2][3],state_board.board[2][4], state_board.board[3][0], state_board.board[3][1], state_board.board[3][2], state_board.board[3][3], state_board.board[3][4], state_board.board[4][0], state_board.board[4][1], state_board.board[4][2], state_board.board[4][3], state_board.board[4][4]);
}

NODE *get_child(NODE *current, int x, int h){//currentのx番目の子供を返す、0番目の子供から数える
    STATE_BOARD state_board = bit_to_board(current->state_bit);
    if(abs(evaluate(state_board.board, state_board.outbo, 0, h)) == INT_MAX) return current;
    if(x == 0 && current->child == NULL){//子供がいなかった場合
        current->child = (NODE *)malloc(sizeof(NODE));
        current->child->parent = current;
        current->child->child = NULL;
        current->child->right = NULL;
        current->child->left = NULL;
        current->child->state_bit.huhi = 0;
        current->child->state_bit.kakugin = 0;
        current->child->state_bit.kinou = 0;
        //初期化

        STATE_BIT current_bit = current->state_bit;
        STATE_BOARD next_board, current_board = bit_to_board(current_bit);
        init_board(current_board.board, current_board.outbo, &next_board);


        for(int j = 0; j < 5; j++){
            for(int k = 0; k < 5; k++){
                if((h == 0 && next_board.board[j][k] > 0 && next_board.board[j][k] <= 10) || (h == 1 && next_board.board[j][k] > 10)){
                    for(int l = 0; l < 5; l++){
                        for(int m = 0; m < 5; m++){
                            for(int n = 0; n < 1; n++){
                                int f = huriwake(next_board.board, next_board.outbo, j, k, l, m, h, next_board.board[j][k], n);

                                if(f == 1){

                                    STATE_BIT next_bit = board_to_bit(next_board);
                                    current->child->state_bit.huhi = next_bit.huhi;
                                    current->child->state_bit.kakugin = next_bit.kakugin;
                                    current->child->state_bit.kinou = next_bit.kinou;
                                    return current->child;
                                }
                                init_board(current_board.board, current_board.outbo, &next_board);
                            }
                        }
                    }
                }
            }
        }
    }

    //子供がいる場合
    NODE *node = current->child;
    int i = 1;
    for(i = 1; i <= x; i++){
        if(node->right != NULL){
            node = node->right;
        }else{
            break;
        }
    }
    //すでにいる子供を返す場合
    if(i == x + 1){
        return node;
    }else if(i == x){//x番目の子供がいない場合
        node->right = (NODE *)malloc(sizeof(NODE));
        node->right->right = NULL;
        node->right->child = NULL;
        node->right->left = node;
        node->right->parent = current;
        //node->rightがx番目の子供になる

        STATE_BOARD next_board;
        STATE_BOARD current_board = bit_to_board(current->state_bit);
        init_board(current_board.board, current_board.outbo, &next_board);

        int j, k, l, m, n;
        int z = 0;
        for(j = 0; j < 5; j++){
            for(k = 0; k < 5; k++){
                if((h == 0 && next_board.board[j][k] > 0 && next_board.board[j][k] <= 10) || (h == 1 && next_board.board[j][k] > 10)){
                    for(l = 0; l < 5; l++){
                        for(m = 0; m < 5; m++){
                            for(n = 0; n < 2; n++){

                                int f = huriwake(next_board.board, next_board.outbo, j, k, l, m, h, next_board.board[j][k], n);
        
                                if(f == 1){

                                    z++;
          
                                    if(z > x){
                                        goto end;
                                    }
                                }
                                init_board(current_board.board, current_board.outbo, &next_board);
                            }
                        }
                    }
                }
            }
        }
    
        for(j = 0; j < 12; j++){
            if((h == 0 && next_board.outbo[j] > 0 && next_board.outbo[j] <= 10) || (h == 1 && next_board.outbo[j] > 10)){
                for(k = 0; k < 5; k++){
                    for(l = 0; l < 5; l++){
                     
                        int f = putboard(next_board.board, next_board.outbo, k, l, h, next_board.outbo[j]);
                      
                        if(f == 1){

                            z++;
                            if(z > x){
                                goto end;
                            }
                        }
                        init_board(current_board.board, current_board.outbo, &next_board);
                    }
                }
            }
        }
        free(node->right);
        node->right = NULL;
        return NULL;
  

    end:
      
        node->right->state_bit = board_to_bit(next_board);
        return node->right;

    }else{//x - 1番目の子供がいないのにx番目の子供を欲しがった時
        puts("ERROR_getch");
        exit(1);
    }
}

void bring_top(NODE *parent, NODE *child){
  if(child->left == NULL) return;
  if(child->right == NULL){
    child->left->right = NULL;
  }else{
    child->left->right = child->right;
    child->right->left = child->left;
  }
  child->left = NULL;
  parent->child->left = child;
  child->right = parent->child;
  parent->child = child;
}

int ab_minmax(NODE *node, int depth, int alpha, int beta, int h){

    if(depth == 0){
        
        STATE_BOARD node_board = bit_to_board(node->state_bit);
        
        return evaluate(node_board.board, node_board.outbo, 0, h);
        
    }
    int i = 0;
    while(1){
        NODE *next = get_child(node, i, h);
        if(next == NULL) break;
        if(next == node){
            STATE_BOARD node_board = bit_to_board(node->state_bit);

            if((node_board.outbo[0] == 1 || node_board.outbo[1] == 1) && h == 1){
                return -INT_MAX;
            }else if((node_board.outbo[0] == 1 || node_board.outbo[1] == 1) && h == 0){
                return INT_MAX;
            }else if((node_board.outbo[0] == 11 || node_board.outbo[1] == 11) && h == 1){
                return INT_MAX;
            }else if((node_board.outbo[0] == 11 || node_board.outbo[1] == 11) && h == 0){
                return -INT_MAX;
            }
        }
    
        //ab_minの中身、返すのはuの値
        int alpha1 = alpha, beta1 = beta;
        int u = INT_MIN;
        if(depth - 1 == 0){
            STATE_BOARD next_board = bit_to_board(next->state_bit);
            u = evaluate(next_board.board, next_board.outbo, 0, h);
        }

    
        int j = 0;
        while(depth - 1 != 0){
            NODE *next_next = get_child(next, j, 1 - h);
            
            if(next_next == NULL) break;
            int v;
            if(next_next == next){
                STATE_BOARD node_board = bit_to_board(next->state_bit);
                
                if((node_board.outbo[0] == 1 || node_board.outbo[1] == 1) && h == 1){
                    u = -INT_MAX;
                    break;
                }else if((node_board.outbo[0] == 1 || node_board.outbo[1] == 1) && h == 0){
                    u = INT_MAX;
                    break;
                }else if((node_board.outbo[0] == 11 || node_board.outbo[1] == 11) && h == 1){
                    u = INT_MAX;
                    break;
                }else if((node_board.outbo[0] == 11 || node_board.outbo[1] == 11) && h == 0){
                    u = -INT_MAX;
                    break;
                }
            }else{
                v = ab_minmax(next_next, depth - 2, alpha1, beta1, h);
            }

            if(v < beta1){
                beta1 = v;
                bring_top(next, next_next);
                if(alpha1 >= beta1){
                    u = alpha1;
                    break;
                }
            }
            j++;
        }
     
        if(u == INT_MIN){
            u = beta1;
        }
        
    
        //ab_maxに戻った
        if(u > alpha){
            alpha = u;
            bring_top(node, next);
            if(alpha >= beta){
                return beta;
            }
        }
        i++;
    }
    return alpha;
}

void printmove(STATE_BOARD current, STATE_BOARD next){
    int x[4] = {0, 0, 0, 0};

    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            if(current.board[i][j] > 0 && next.board[i][j] == 0 ){
                x[0] = i + 6;
                x[1] = j + 6;
            }
            if(current.board[i][j] != next.board[i][j] && next.board[i][j] > 0){
                x[2] = i + 6;
                x[3] = j + 6;
            }
        }
    }

    if(x[0] == 0 && x[1] == 0){
        if(next.board[x[2] - 6][x[3] - 6] % 10 == 2){
            printf("%d%cKI\n", x[3] - 5, 'A' + x[2] - 6);
        }else if(next.board[x[2] - 6][x[3] - 6] % 10 == 3){
            printf("%d%cGI\n", x[3] - 5, 'A' + x[2] - 6);
        }else if(next.board[x[2] - 6][x[3] - 6] % 10 == 5){
            printf("%d%cKK\n", x[3] - 5, 'A' + x[2] - 6);
        }else if(next.board[x[2] - 6][x[3] - 6] % 10 == 7){
            printf("%d%cHI\n", x[3] - 5, 'A' + x[2] - 6);
        }else if(next.board[x[2] - 6][x[3] - 6] % 10 == 9){
            printf("%d%cFU\n", x[3] - 5, 'A' + x[2] - 6);
        }else{
            puts("ERROR_printmove1");
            exit(1);
        }
    }else{
    
        if(next.board[x[2] - 6][x[3] - 6] % 10 == 1){
            printf("%d%c%d%c\n", x[1] - 5, 'A'+ x[0] - 6, x[3] - 5, 'A'+ x[2] - 6);
        }else if(next.board[x[2] - 6][x[3] - 6] % 10 == 2){
            printf("%d%c%d%c\n", x[1] - 5, 'A' + x[0] - 6, x[3] - 5, 'A'+ x[2] - 6);
        }else if(next.board[x[2] - 6][x[3] - 6] % 10 == 3){
            printf("%d%c%d%c\n", x[1] - 5, 'A' + x[0] - 6, x[3] - 5, 'A'+ x[2] - 6);
        }else if(next.board[x[2] - 6][x[3] - 6] % 10 == 4){
            if(current.board[x[0] - 6][x[1] - 6] % 10 == 3){
                printf("%d%c%d%cN\n", x[1] - 5, 'A' + x[0] - 6, x[3] - 5, 'A'+ x[2] - 6);
            }else{
                printf("%d%c%d%c\n", x[1] - 5, 'A' + x[0] - 6, x[3] - 5, 'A'+ x[2] - 6);
            }
        }else if(next.board[x[2] - 6][x[3] - 6] % 10 == 5){
            printf("%d%c%d%c\n", x[1] - 5, 'A' + x[0] - 6, x[3] - 5, 'A'+ x[2] - 6);
        }else if(next.board[x[2] - 6][x[3] - 6] % 10 == 6){
            if(current.board[x[0] -6][x[1] - 6] % 10 == 5){
                printf("%d%c%d%cN\n", x[1] - 5, 'A' + x[0] - 6, x[3] - 5, 'A'+ x[2] - 6);
            }else{
                printf("%d%c%d%c\n", x[1] - 5, 'A' + x[0] - 6, x[3] - 5, 'A'+ x[2] - 6);
            }
        }else if(next.board[x[2] - 6][x[3] - 6] % 10 == 7){
            printf("%d%c%d%c\n", x[1] - 5, 'A' + x[0] - 6, x[3] - 5, 'A'+ x[2] - 6);
        }else if(next.board[x[2] - 6][x[3] - 6] % 10 == 8){
            if(current.board[x[0] -6][x[1] - 6] % 10 == 7){
                printf("%d%c%d%cN\n", x[1] - 5, 'A' + x[0] - 6, x[3] - 5, 'A'+ x[2] - 6);
            }else{
                printf("%d%c%d%c\n", x[1] - 5, 'A' + x[0] - 6, x[3] - 5, 'A'+ x[2] - 6);
            }
        }else if(next.board[x[2] - 6][x[3] - 6] % 10 == 9){
            printf("%d%c%d%c\n", x[1] - 5, 'A' + x[0] - 6, x[3] - 5, 'A'+ x[2] - 6);
        }else if(next.board[x[2] - 6][x[3] - 6] % 10 == 0){
            if(current.board[x[0] -6][x[1] - 6] % 10 == 9){
                printf("%d%c%d%cN\n", x[1] - 5, 'A' + x[0] - 6, x[3] - 5, 'A'+ x[2] - 6);
            }else{
                printf("%d%c%d%c\n", x[1] - 5, 'A' + x[0] - 6, x[3] - 5, 'A'+ x[2] - 6);
            }
        }else{
            printboard(next);
            puts("ERROR_printmove2");
            exit(1);
        }
    }
}


void free_node(NODE *node){
  if(node->child == NULL) return;
  if(node->child->child == NULL && node->child->right == NULL) free(node->child);
  if(node->child->right != NULL)
    free_node(node->child->right);
  if(node->child->child != NULL)
    free_node(node->child->child);
}


void compute_output(STATE_BOARD *current, int h){

    STATE_BIT state_bit = board_to_bit(*current);

    NODE node;
    node.right = NULL;
    node.left = NULL;
    node.parent = NULL;
    node.child = NULL;
    node.state_bit = state_bit;

    ab_minmax(&node, 4, -INT_MAX, INT_MAX, h);
    
    NODE *child_node = node.child;
    STATE_BOARD next = bit_to_board(child_node->state_bit);
    while(child_node->right != NULL){
        if(oute(next.board, next.outbo, h)){
            child_node = child_node->right;
            next = bit_to_board(child_node->state_bit);
        }else break;
    }
    //デバッグ用
    while(0){
    NODE *jikke = node.child->child;
    
    STATE_BOARD next = bit_to_board((node.child)->state_bit);
    STATE_BOARD next_next = bit_to_board(jikke->state_bit);
    
    printboard(next_next);
    printmove(next, next_next);
    while(jikke->right != NULL){
        jikke = jikke->right;
        next_next = bit_to_board(jikke->state_bit);
        printboard(next_next);
        printmove(next, next_next);
    }
    }
    
    printmove(*current, next);
    init_board(next.board, next.outbo, current);
    free_node(&node);
}

void printboard2(){
    
    printf("%s", " +--+--+--+--+--+\n");

    for (int i = 0; i < 5; i++){
        printf("%d", 5-i);
        printf("%s", "|");

        for (int j = 0; j < 5; j++){
            if(ban[j][4-i] >= 10){
                printf("%d", ban[j][4-i]);
                printf("%s", "|");
            }else if(ban[j][4-i] > 0){
                printf("%d ", ban[j][4-i]);
                printf("%s", "|");
            }else{
                printf("  ");
                printf("%s", "|");
            }
        }
        printf("%s", "\n +--+--+--+--+--+ \n");
    }
    printf("%s", "  A  B  C  D  E\n\n");
    puts("先手の持ち駒");
    for(int i = 0; i < 12; i++){
        if(motigoma[i] > 0 && motigoma[i] <= 10){
            printf("%d ", motigoma[i]);
        }
    }
    printf("\n");
    puts("後手の持ち駒");
    for(int i=0; i<12; i++){
        if(motigoma[i] > 10){
            printf("%d ", motigoma[i]);
        }
    }
    printf("\n");
}

int main(int argc, char **argv){

  //先手、後手判定
    int teban = *argv[1];

    STATE_BOARD now_board;
    STATE_BIT now_bit;
    for(int x = 0; x < 5; x++){
        for(int y = 0; y < 5; y++){
            now_board.board[x][y] = ban[x][y];
        }
    }
    for(int i = 0; i < 12; i++){
        now_board.outbo[i] = motigoma[i];
    }
    now_bit = board_to_bit(now_board);

    history[0] = now_bit;

    if(teban == '0'){
        while (1){
      //150超え引き分け判定
            if(co == 150){
                printf("HIKIWAKE\n"); //取り敢えず表示するようにしています.
                return 0;
            }
          
            int x, y, xx, yy;
            char s[100]={}; // 手の読み込み
            scanf("%s", s);
            y = s[0] - '1';
            x = s[1] - 'A';

            if('A' <= s[2] && s[2] <= 'Z'){ // 駒を打つ場合.

                int k = 0; // 打つ駒が何であるか.

                if(s[2] == 'H' && s[3] == 'I' && (motigoma[8] == 7 || motigoma[9] == 7) ){
                    k = 7;
                }
                else if(s[2] == 'F' && s[3] == 'U' && (motigoma[10] == 9 || motigoma[11] == 9) ){
                    k = 9;
                }
                else if(s[2] == 'G' && s[3] == 'I' && (motigoma[4] == 3 || motigoma[5] == 3) ){
                    k = 3;
                }
                else if(s[2] == 'K' && s[3] == 'I' && (motigoma[2] == 2 || motigoma[3] == 2) ){
                    k = 2;
                }
                else if(s[2] == 'K' && s[3] == 'K' && (motigoma[6] == 5 || motigoma[7] == 5)){
                    k = 5;
                }

                if(k == 0){ //不正手.
                    printf("You Lose\n");
                    return 0;
                }

                int f = putboard(ban, motigoma, x, y, 1, k);

                if(f != 1){ //不正手.
                    printf("You Lose\n");
                    return 0;
                }
            }

            else{
                yy = s[2] - '1';
                xx = s[3] - 'A';

                int k = ban[x][y]; // 動かす駒.
                int f = 0;
                if(s[4] == 'N'){
                    f = huriwake(ban, motigoma, x, y, xx, yy, 0, k, 1);
                }else{
                    f = huriwake(ban, motigoma, x, y, xx, yy, 0, k, 0);
                }

                if(f != 1){
                    printf("You Lose\n");
                    return 0;
                }
            }

            if(oute(ban, motigoma, 0)){
                printf("You Lose\n");
                return 0;
            }

            STATE_BOARD state_board;
            int cash[12] = {};
            for(int i = 0; i < 12; i++){
                cash[i] = motigoma[i];
            }
            init_board(ban, cash, &state_board);
            //printboard2();
            //printf("user end\n");

            co++;
            STATE_BOARD now_board;
            STATE_BIT now_bit;
            for(int x = 0; x < 5; x++){
                for(int y = 0; y < 5; y++){
                    now_board.board[x][y] = ban[x][y];
                }
            }
            for(int i = 0; i < 12; i++){
                now_board.outbo[i] = motigoma[i];
            }
            now_bit = board_to_bit(now_board);

            history[co] = now_bit;
            if(sennitite(history,now_bit,co)){
                if(co3>=6){ //連続王手の千日手
                    printf("You Win\n");
                    return 0;
                }else{
                    printf("You Lose\n");
                    return 0;
                }
            }

            int ff = finjudge(ban, motigoma, 0); // 勝利判定.

            if(ff == 1){
                printf("You Win\n");
                return 0;
            }

            // ここまでが自手番.
            
            STATE_BOARD current;
            int outbo[12];
            for(int i = 0; i < 12; i++){
                outbo[i] = motigoma[i];
            }

            init_board(ban, outbo, &current);
            //ここまでOK

            compute_output(&current, 1);
            for(int i = 0; i < 12; i++){
                motigoma[i] = current.outbo[i];
            }
            for(int i = 0; i < 5; i++){
                for(int j = 0; j < 5; j++){
                    ban[i][j] = current.board[i][j];
                }
            }
            if(oute(ban, motigoma, 1)){
                printf("You Win\n");
                return 0;
            }

            if(oute(ban, motigoma, 0)){
                co3++;
            }else{
                co3 = 0;
            }
            
            co++;
            STATE_BOARD now_board2;
            STATE_BIT now_bit2;
            for(int x = 0; x < 5; x++){
                for(int y = 0; y < 5; y++){
                    now_board2.board[x][y] = ban[x][y];
                }
            }
            for(int i = 0; i < 12; i++){
                now_board2.outbo[i] = motigoma[i];
            }

            now_bit2 = board_to_bit(now_board2);

            history[co] = now_bit2;
            if(sennitite(history,now_bit2,co)){
                if(co3>=7){ //連続王手の千日手
                    printf("You Win\n");
                    return 0;
                }else{
                    printf("You Lose\n");
                    return 0;
                }
            }

            //printboard2();
            //printf("AI end");
            //printf("\n");

            ff = finjudge(ban, motigoma, 1);
            if(ff == 1){
                printf("You Lose\n");
                return 0;
            }
      //ここまでがAI番
        }
    }


  else{ // 後手、他はほぼ一緒
      while (1){
      // 150超え引き分け判定
          if(co == 150){
              printf("HIKIWAKE\n");
              return 0;
          }
          STATE_BOARD current;
    
          int outbo[12];
          for(int i = 0; i < 12; i++){
              outbo[i] = motigoma[i];
          }
          init_board(ban, outbo, &current);

          compute_output(&current, 0);
          for(int i = 0; i < 12; i++){
              motigoma[i] = current.outbo[i];
          }
          for(int i = 0; i < 5;i++){
              for(int j =0; j < 5; j++){
                  ban[i][j] = current.board[i][j];
              }
          }

          if(oute(ban, motigoma, 0)){
              printf("You Win\n");
              return 0;
          }

          co++;

          STATE_BOARD now_board;
          STATE_BIT now_bit;
          for(int x = 0; x < 5; x++){
              for(int y = 0; y < 5; y++){
                  now_board.board[x][y] = ban[x][y];
              }
          }
          for(int i = 0; i < 12; i++){
              now_board.outbo[i] = motigoma[i];
          }
          now_bit = board_to_bit(now_board);

          history[co] = now_bit;
          if(sennitite(history,now_bit,co)){
              if(co2>=6){ //連続王手の千日手
                  printf("You Lose\n");
                  return 0;
              }else{
                  printf("You Win\n");
                  return 0;
              }
          }

          int ff = finjudge(ban, motigoma, 0);
          if(ff == 1){
              printf("You Lose\n");
              return 0;
          }

          //printboard2();
          //printf("%d\n", evaluate(ban,motigoma,0,0));
          //printf("AI end");
          //printf("\n");
          //ここまでがAI番

          int x, y, xx, yy;
          char s[100]={}; // 手の読み込み
          scanf("%s", s);

          y = s[0] - '1';
          x = s[1] - 'A';

          if('A' <= s[2] && s[2] <= 'Z'){ // 駒を打つ場合.

              int k = 0; // 打つ駒が何であるか.

              if(s[2] == 'H' && s[3] == 'I' && (motigoma[8] == 17 || motigoma[9] == 17) ){
                    k = 17;
              }
              else if(s[2] == 'F' && s[3] == 'U' && (motigoma[10] == 19 || motigoma[11] == 19) ){
                    k = 19;
              }
              else if(s[2] == 'G' && s[3] == 'I' && (motigoma[4] == 13 || motigoma[5] == 13) ){
                    k = 13;
              }
              else if(s[2] == 'K' && s[3] == 'I' && (motigoma[2] == 12 || motigoma[3] == 12) ){
                    k = 12;
              }
              else if(s[2] == 'K' && s[3] == 'K' && (motigoma[6] == 15 || motigoma[7] == 15)){
                    k = 15;
              }

              if(k == 0){ //不正手.
                    printf("You Lose\n");
                    return 0;
              }

              int f = putboard(ban, motigoma, x, y, 1, k);
		  
              if(f != 1){ //不正手.
                  printf("You Lose\n");
                  return 0;
              }
          }

          else{
              yy = s[2] - '1';
              xx = s[3] - 'A';


              int k = ban[x][y]; // 動かす駒.
              int f = 0;
              if(s[4] == 'N') f = huriwake(ban, motigoma, x, y, xx, yy, 1, k, 1);
              else            f = huriwake(ban, motigoma, x, y, xx, yy, 1, k, 0);

              if(f != 1){
                  printf("You Lose\n");
                  return 0;
              }
          }

          if(oute(ban, motigoma, 1)){
              printf("You Lose\n");
              return 0;
          }
          
          if(oute(ban, motigoma, 0)){
              co2++;
          }else{
              co2 = 0;
          }

          //printboard2();
          //printf("%d\n", evaluate(ban,motigoma,0,1));
          //puts("user end\n");

          co++;
          STATE_BOARD now_board2;
          STATE_BIT now_bit2;
          for(int x = 0; x < 5; x++){
              for(int y = 0; y < 5; y++){
                  now_board2.board[x][y] = ban[x][y];
              }
          }
          for(int i = 0; i < 12; i++){
              now_board2.outbo[i] = motigoma[i];
          }
          now_bit2 = board_to_bit(now_board2);

          history[co] = now_bit2;
          if(sennitite(history,now_bit2,co)){
              if(co2>=7){ //連続王手の千日手
                  printf("You Lose\n");
                  return 0;
              }else{
                  printf("You Win\n");
                  return 0;
              }
          }

          ff = finjudge(ban, motigoma, 1); // 勝利判定.

          if(ff == 1){
              printf("You Win\n");
              return 0;
          }

      }
  }
}

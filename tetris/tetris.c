#include "tetris.h"

static struct sigaction act, oact;
typedef struct _Node{
	char name[NAMELEN];
	int score;
	struct _Node *link;
}Node;

Node *head = NULL;
RecNode *root;
int ranknum;
int main(){
	int exit=0;
	
	createRankList();
	initscr();
	noecho();
	keypad(stdscr, TRUE);	
	root = (RecNode *)malloc(sizeof(RecNode));
	root->level = 0;
	root->accumulatedScore = 0;
	makeTree(root);
	srand((unsigned int)time(NULL));

	while(!exit){
		clear();
		switch(menu()){
		case MENU_PLAY: play(); break;
		case MENU_RANK: rank(); break;
		case MENU_REC: recommendedPlay(); break;
		case MENU_EXIT: exit=1; break;
		default: break;
		}
	}

	endwin();
	system("clear");
	return 0;
}

void InitTetris(){
	int i,j;

	for(j=0;j<HEIGHT;j++)
		for(i=0;i<WIDTH;i++)
			field[j][i]=0;

	nextBlock[0]=rand()%7;
	nextBlock[1]=rand()%7;
	nextBlock[2]=rand()%7;
	blockRotate=0;
	blockY=-1;
	blockX=WIDTH/2-2;
	score=0;	
	gameOver=0;
	timed_out=0;

	DrawOutline();
	DrawField();
	DrawBlock(blockY,blockX,nextBlock[0],blockRotate,' ');
	DrawNextBlock(nextBlock);
	modified_recommend(root);
	PrintScore(score);
}

void DrawOutline(){	
	int i,j;
	/* 블럭이 떨어지는 공간의 태두리를 그린다.*/

	DrawBox(0,0,HEIGHT,WIDTH);

	/* next block을 보여주는 공간의 태두리를 그린다.*/
	move(2,WIDTH+10);
	printw("NEXT BLOCK");
	DrawBox(3,WIDTH+10,4,8);

	move(8, WIDTH+10);
	DrawBox(9, WIDTH+10,4,8);
	/* score를 보여주는 공간의 태두리를 그린다.*/
	move(16,WIDTH+10);
	printw("SCORE");
	DrawBox(17,WIDTH+10,1,8);

	
}

int GetCommand(){
	int command;
	command = wgetch(stdscr);
	switch(command){
	case KEY_UP:
		break;
	case KEY_DOWN:
		break;
	case KEY_LEFT:
		break;
	case KEY_RIGHT:
		break;
	case ' ':	/* space key*/
		/*fall block*/
		break;
	case 'q':
	case 'Q':
		command = QUIT;
		break;
	default:
		command = NOTHING;
		break;
	}
	return command;
}

int ProcessCommand(int command){
	int ret=1;
	int drawFlag=0;
	switch(command){
	case QUIT:
		ret = QUIT;
		break;
	case KEY_UP:
		if((drawFlag = CheckToMove(field,nextBlock[0],(blockRotate+1)%4,blockY,blockX)))
			blockRotate=(blockRotate+1)%4;
		break;
	case KEY_DOWN:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY+1,blockX)))
			blockY++;
		break;
	case KEY_RIGHT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX+1)))
			blockX++;
		break;
	case KEY_LEFT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX-1)))
			blockX--;
		break;
	default:
		break;
	}
	if(drawFlag) DrawChange(field,command,nextBlock[0],blockRotate,blockY,blockX);
	return ret;	
}

void DrawField(){
	int i,j;
	for(j=0;j<HEIGHT;j++){
		move(j+1,1);
		for(i=0;i<WIDTH;i++){
			if(field[j][i]==1){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(".");
		}
	}
}


void PrintScore(int score){
	move(18,WIDTH+11);
	printw("%8d",score);
}

void DrawNextBlock(int *nextBlock){
	int i, j;
	for( i = 0; i < 4; i++ ){
		move(4+i,WIDTH+13);
		for( j = 0; j < 4; j++ ){
			if( block[nextBlock[1]][0][i][j] == 1 ){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}
	for(i=0;i<4;i++){
		move(10+i, WIDTH+13);
		for(j=0;j<4;j++){
			if(block[nextBlock[2]][0][i][j] == 1){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}
}

void DrawBlock(int y, int x, int blockID,int blockRotate,char tile){
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++){
			if(block[blockID][blockRotate][i][j]==1 && i+y>=0){
				move(i+y+1,j+x+1);
				attron(A_REVERSE);
				printw("%c",tile);
				attroff(A_REVERSE);
			}
		}

	move(HEIGHT,WIDTH+10);
}

void DrawBox(int y,int x, int height, int width){
	int i,j;
	move(y,x);
	addch(ACS_ULCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);
	for(j=0;j<height;j++){
		move(y+j+1,x);
		addch(ACS_VLINE);
		move(y+j+1,x+width+1);
		addch(ACS_VLINE);
	}
	move(y+j+1,x);
	addch(ACS_LLCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);
}

void play(){
	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM,&act,&oact);
	InitTetris();
	do{
		if(timed_out==0){
			alarm(1);
			timed_out=1;
		}

		command = GetCommand();
		if(ProcessCommand(command)==QUIT){
			alarm(0);
			DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
			move(HEIGHT/2,WIDTH/2-4);
			printw("Good-bye!!");
			refresh();
			getch();

			return;
		}
	}while(!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
	move(HEIGHT/2,WIDTH/2-4);
	printw("GameOver!!");
	refresh();
	getch();
	newRank(score);
}

char menu(){
	printw("1. play\n");
	printw("2. rank\n");
	printw("3. recommended play\n");
	printw("4. exit\n");
	return wgetch(stdscr);
}

/////////////////////////첫주차 실습에서 구현해야 할 함수/////////////////////////

int CheckToMove(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
	int i, j;
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			if(block[currentBlock][blockRotate][i][j]==1){
				if(i+blockY < 0 || j+blockX < 0 || i+blockY >= HEIGHT || j+blockX >= WIDTH){
					return 0;
				}
				if(field[i+blockY][j+blockX] == 1){
					return 0;
				}
			}
		}
	}
	return 1;
}

void DrawChange(char f[HEIGHT][WIDTH],int command,int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
	int i, j, x = blockX, y = blockY, r = blockRotate;
	//1. 이전 블록 정보를 찾는다. ProcessCommand의 switch문을 참조할 것
	switch(command){
	case KEY_DOWN:
		y--;
		break;
	case KEY_RIGHT:
		x--;
		break;
	case KEY_LEFT:
		x++;
		break;
	case KEY_UP:
		r = (r+3)%4;
		break;	
	}
	//2. 이전 블록 정보를 지운다. DrawBlock함수 참조할 것.
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			if(block[currentBlock][r][i][j]==1 && i+y>=0){
				move(i+y+1, j+x+1);
				printw(".");
			}
		}
	}
	//3. 새로운 블록 정보를 그린다. 
	//DrawBlock(blockY, blockX, currentBlock, blockRotate, ' ');
	while(CheckToMove(field, nextBlock[0], r, y+1, x)){
		y++;
	}
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			if(block[currentBlock][r][i][j]==1 && i+y>=0){
				move(i+y+1, j+x+1);
				printw(".");
			}
		}
	}
	DrawBlockWithFeatures(blockY, blockX, currentBlock, blockRotate);	
}

void BlockDown(int sig){
	// user code
	
	//강의자료 p26-27의 플로우차트를 참고한다.
	if(CheckToMove(field, nextBlock[0], blockRotate, blockY+1, blockX)){
		blockY++;
		DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, blockY, blockX);
	}
	else{
		if(blockY==-1){
			gameOver = 1;
		}
		score += AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX);
		score += DeleteLine(field);
		nextBlock[0] = nextBlock[1];
		nextBlock[1] = nextBlock[2];
		nextBlock[2] = rand()%7;
		blockRotate=0; blockY=-1; blockX=WIDTH/2-2;
		modified_recommend(root);
		DrawNextBlock(nextBlock);
		PrintScore(score);
		DrawField();
	}
	timed_out=0;
}

int AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code

	//Block이 추가된 영역의 필드값을 바꾼다.
	int i, j, touched=0;
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			if(block[currentBlock][blockRotate][i][j]==1){
				f[i+blockY][j+blockX] = 1;
				if(i+blockY == HEIGHT-1 || f[i+blockY+1][j+blockX]==1){
					touched++;
				}
			}
		}
	}
	return touched * 10;
}

int DeleteLine(char f[HEIGHT][WIDTH]){
	// user code
	int i, j, k, flag = 1, count = 0;
	//1. 필드를 탐색하여, 꽉 찬 구간이 있는지 탐색한다.
	for(i=0;i<HEIGHT;i++){
		for(j=0;j<WIDTH;j++){
			if(f[i][j]==0){
				flag = 0;
				break;
			}
			else flag = 1;
		}
	//2. 꽉 찬 구간이 있으면 해당 구간을 지운다. 즉, 해당 구간으로 필드값을 한칸씩 내린다.
		if(flag != 0){
			count++;
			for(k=i;k>0;k--){
				for(j=0;j<WIDTH;j++){
					f[k][j] = f[k-1][j];
				}
			}
		}
	}
	return count*count*100;
}

///////////////////////////////////////////////////////////////////////////

void DrawShadow(int y, int x, int blockID,int blockRotate){
	// user code
	while(CheckToMove(field, blockID, blockRotate, y+1, x)){
		y++;
	}
	
	DrawBlock(y, x, blockID, blockRotate, '/');	
}
void DrawBlockWithFeatures(int y, int x, int blockID, int blockRotate){
	DrawRecommend(recommendY, recommendX, nextBlock[0], recommendR);
	DrawShadow(y, x, blockID, blockRotate);
	DrawBlock(y, x, blockID, blockRotate, ' ');
}
void createRankList(){
	// user code
	FILE *fp = fopen("rank.txt", "r");
	int tmpscore;
	if(fp==NULL){
		ranknum=0;
		fp = fopen("rank.txt", "w");
		fprintf(fp, "%d", 0);
		//fclose(fp);
		//fp = fopen("rank.txt", "r");
	}
	else{
		fscanf(fp, "%d", &ranknum);
		int i = 0;
		for(i=0;i<ranknum;i++){
			Node *new = NULL;
			new = (Node *)malloc(sizeof(Node));
			fscanf(fp, "%s %d", new->name, &tmpscore);
			new->score = tmpscore;
			Node *tmp;
			if(head==NULL){
				head = new;
			}
			else {
				tmp = head;
				while(tmp->link != NULL){
					tmp = tmp->link;
				}
				tmp->link = new;
			}
		}
	}
	fclose(fp);
}

void rank(){
	// user code
	int x = 1, y = ranknum, key;
	Node *curr;
	clear();
	printw("1. list ranks from X to Y\n");
	printw("2. list ranks by a specific name\n");
	printw("3. delete a specific rank\n");
	key = wgetch(stdscr);
	if(key == '1'){
		curr = head;
		echo();
		printw("X: ");
		scanw("%d", &x);
		printw("Y: ");
		scanw("%d", &y);
		printw("      name      | score \n");
		printw("-------------------------\n");
		if(x<=0 || y<=0|| x>ranknum || y>ranknum || x>y)
			printw("search failure: no ranks in the list\n");
		else{
			int count = 0;
			while(1){
				count++;
				if(count >= x && count <= y){
					printw("%-16s  %d\n", curr->name, curr->score);
					if(count==y) break;
				}
				curr = curr->link;
			}
		}
		noecho();
	}
	if(key == '2'){
		char tmpname[NAMELEN];
		int flag = 0;
		echo();
		printw("input the name: ");
		scanw("%s", tmpname);
		noecho();
		if(ranknum==0){
			printw("search failure: no information in the list\n");
		}
		else{
			curr = head;
			printw("      name      | score \n");
			printw("-------------------------\n");
			while(curr!=NULL){
				if(strcmp(tmpname, curr->name)==0){
					printw("%-16s  %d\n", curr->name, curr->score);
					flag = 1;
				}
				curr = curr->link;
			}
			if(flag == 0){
				printw("search failure: no information in the list\n");
			}
		}
	}
	if(key == '3'){
		Node *pre;
		int rankdel, count = 0;
		echo();
		printw("input the rank: ");
		scanw("%d", &rankdel);
		noecho();
		if(rankdel>ranknum ||rankdel <= 0 || ranknum==0){
			printw("search failure: rank not in the list\n"); 
		}
		else if(rankdel==1){
			pre = head;
			head = head->link;
			free(pre);
			printw("result: the rank deleted\n");
			ranknum--;
		}
		else{
			curr = head;
			while(curr!=NULL){
				count++;
				if(count == rankdel){
					pre->link = curr->link;
					free(curr);
					printw("result: the rank deleted\n");
					break;
				}
				pre = curr;
				curr = curr->link;
			}
			ranknum--;
		}
		writeRankFile();
	}
	getch();
}

void writeRankFile(){
	// user code
	FILE *fp = fopen("rank.txt", "w");
	fprintf(fp, "%d\n", ranknum);
	int i =0;
	Node *curr = head;
	for(i=0;i<ranknum;i++){
		fprintf(fp, "%s %d\n", curr->name, curr->score);
		curr = curr->link;
	}
	fclose(fp);
}

void newRank(int score){
	// user code
	Node *new = NULL;
	new = (Node *)malloc(sizeof(Node));
	clear();
	echo();
	printw("Your name: ");
	scanw("%s", new->name);
	noecho();
	new->score = score;
	new->link = NULL;
	Node *pre = head, *tmp = head;
	int flag = 0;
	if(head == NULL){
		head = new;
	}
	else{
		if(new->score >= head->score){
			new->link = head;
			head = new;
		}
		else{
			while(tmp->link!=NULL){
				if(new->score >= tmp->score && tmp->link!=NULL){
					pre->link = new;
					new->link = tmp;
					flag = 1;
					break;
				}
				pre = tmp;
				tmp = tmp->link;
			}
			if(flag == 0 && new->score <= tmp->score){
				tmp->link = new;
			}
			else if(flag == 0 && new->score > tmp->score){
				pre->link = new;
				new->link = tmp;
			}
		}
	}
	ranknum++;
	writeRankFile();
}
void makeTree(RecNode *root){
	int i, h, w;
	//RecNode **c = root->c;
	if(root->level == 0){
		for(h=0;h<HEIGHT;h++){
			for(w=0;w<WIDTH;w++){
				root->recField[h][w] = field[h][w];
			}
		}
	}
	root->accumulatedScore = 0;
	for(h=0;h<HEIGHT;h++){
		for(w=0;w<WIDTH;w++)
			root->recField[h][w] = field[h][w];
	}
	/*for(i=0;i<CHILDREN_MAX;i++){
		c[i] = (RecNode *)malloc(sizeof(RecNode));
		c[i]->level = root->level+1;
		if(c[i]->level < VISIBLE_BLOCKS){
			makeTree(c[i]);
		}
	}*/
}
void DrawRecommend(int y, int x, int blockID,int blockRotate){
	// user code
	if(CheckToMove(field, nextBlock[0], recommendR, recommendY, recommendX))
	//	DrawBlock(recommendY, recommendX, nextBlock[0], recommendR, 'R');
		DrawBlock(recommendY, recommendX, nextBlock[0], recommendR, 'R');
}

int recommend(RecNode *root){
	// user code
	int max = 0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수
	int lv = root->level, currBlockNum, k = 0, i, j , h, w , x, y=1;
	int r[7]={2, 4, 4, 4, 1, 2, 2};
	currBlockNum = nextBlock[lv];
	int rot = r[currBlockNum], tmpscore, flag = 0, tempR, tempX, tempY;
	/*if(root->level ==0){
		root->accumulatedScore = score;
		for(h=0;h<HEIGHT;h++){
			for(w=0;w<WIDTH;w++)
				root->recField[h][w] = field[h][w];
		}
	}*/
	if(lv<VISIBLE_BLOCKS){
		root->child = (RecNode **)malloc(sizeof(RecNode *)*36);
	}
	for(i=-1;i<WIDTH;i++){
		for(j=0;j<rot;j++){
			if(CheckToMove(root->recField, currBlockNum, j, y, i)){
				RecNode *tmp = (RecNode *)malloc(sizeof(RecNode));
				tmp->level = lv + 1;
				tmp->parent = root;
				root->child[k] = tmp;
				for(h=0;h<HEIGHT;h++){
					for(w=0;w<WIDTH;w++){
						root->child[k]->recField[h][w] = root->recField[h][w];
					}
				}
				
				for(y=0;y<HEIGHT;y++){
					if(!CheckToMove(root->child[k]->recField, currBlockNum, j, y, i))
						break;
				}
				y--;
				
				root->child[k]->accumulatedScore = root->accumulatedScore; 
				root->child[k]->accumulatedScore += AddBlockToField(root->child[k]->recField, currBlockNum, j, y, i);
				root->child[k]->accumulatedScore += DeleteLine(root->child[k]->recField);
				if(lv<VISIBLE_BLOCKS-1)
					tmpscore = recommend(root->child[k]);
				else tmpscore = root->child[k]->accumulatedScore;
				if(max<tmpscore){
				 	max = tmpscore;
					flag = 1;
					tempX = i;
					tempY = y;
					tempR = j;
				}
				root->child[k]->curBlockID = currBlockNum;
				root->child[k]->recBlockX = recommendX;
				root->child[k]->recBlockY = recommendY;
				root->child[k]->recBlockRotate = recommendR;
				k++;
			}
			else continue;
		}
	}	
	if(lv == 0 && flag){
		recommendX = tempX;
		recommendY = tempY;
		recommendR = tempR;
	}
	return max;
}
void recDrawBlockWithFeatures(int y, int x, int blockID, int blockRotate){
	DrawRecommend(recommendY, recommendX, nextBlock[0], recommendR);
	DrawBlock(y, x, blockID, blockRotate, ' ');
}
int modified_recommend(RecNode *root){
	int max = 0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수
	int lv = root->level, currBlockNum, k = 0, i, j , h, w , x, y=1;
	int r[7]={2, 4, 4, 4, 1, 2, 2};
	currBlockNum = nextBlock[lv];
	int rot = r[currBlockNum], tmpscore, flag = 0, tempR, tempX, tempY, temp2R, temp2Y, temp2X;
	if(root->level ==0){
		root->accumulatedScore = score;
		for(h=0;h<HEIGHT;h++){
			for(w=0;w<WIDTH;w++)
				root->recField[h][w] = field[h][w];
		}
	}
	if(lv<VISIBLE_BLOCKS){
		root->child = (RecNode **)malloc(sizeof(RecNode *)*9);
	}
	for(i=-1;i<WIDTH;i++){
		for(j=0;j<rot;j++){
			if(CheckToMove(root->recField, currBlockNum, j, y, i)){
				RecNode *tmp = (RecNode *)malloc(sizeof(RecNode));
				tmp->level = lv + 1;
				tmp->parent = root;
				root->child[k] = tmp;
				for(h=0;h<HEIGHT;h++){
					for(w=0;w<WIDTH;w++){
						root->child[k]->recField[h][w] = root->recField[h][w];
					}
				}
				
				for(y=0;y<HEIGHT;y++){
					if(!CheckToMove(root->child[k]->recField, currBlockNum, j, y, i))
						break;
				}
				y--;
				
				root->child[k]->accumulatedScore = root->accumulatedScore; 
				root->child[k]->accumulatedScore += AddBlockToField(root->child[k]->recField, currBlockNum, j, y, i);
				if(DeleteLine(root->child[k]->recField)>=100){
					flag = 2;
					temp2X = i;
					temp2Y = y;
					temp2R = j;
				}
				root->child[k]->accumulatedScore += DeleteLine(root->child[k]->recField);
				if(lv<VISIBLE_BLOCKS-1)
					tmpscore = recommend(root->child[k]);
				else tmpscore = root->child[k]->accumulatedScore;
				if(max<=tmpscore){
				 	max = tmpscore;
					flag = 1;
					tempX = i;
					tempY = y;
					tempR = j;
				}
				k++;
			}
			else continue;
		}
	}	
	if(lv==0 && flag == 2){
		recommendX = temp2X;
		recommendY = temp2Y;
		recommendR = temp2R;
	}
	if(lv==0 && flag == 1){
		recommendX = tempX;
		recommendY = tempY;
		recommendR = tempR;
	}
	return max;
}
void BlockDown_rec(int sig){
	int i = 0;
	if(!CheckToMove(field, nextBlock[0], recommendR, recommendY, recommendX)) gameOver = 1;
	else{
		score += AddBlockToField(field, nextBlock[0], recommendR, recommendY, recommendX);
		score += DeleteLine(field);
		blockY=-1; blockX=(WIDTH/2)-2; blockRotate = 0;
		while(i<VISIBLE_BLOCKS-1){
			nextBlock[i] = nextBlock[i+1];
			i++;
		}
		nextBlock[VISIBLE_BLOCKS-1] = rand()%7;
		modified_recommend(root);
		DrawNextBlock(nextBlock);
		PrintScore(score);
		DrawField();
		recDrawBlockWithFeatures(blockY, blockX, nextBlock[0], blockRotate);
		timed_out = 0;
	}
}
void recommendedPlay(){
	// user code
	int command, count = 0;
	clear();
	act.sa_handler = BlockDown_rec;
	sigaction(SIGALRM,&act,&oact);
	InitTetris();
	do{
		if(timed_out==0){
			alarm(1);
			timed_out=1;
		}

		command = GetCommand();
		if(ProcessCommand(command)==QUIT){
			alarm(0);
			DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
			move(HEIGHT/2,WIDTH/2-4);
			printw("Good-bye!!");
			refresh();
			getch();

			return;
		}
	}while(!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
	move(HEIGHT/2,WIDTH/2-4);
	printw("GameOver!!");
	refresh();
	getch();
}

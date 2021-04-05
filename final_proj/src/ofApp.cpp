#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(30); //프레임율 30으로 설정
    
    // 배경을 검은색으로 설정합니다.
    ofBackground(0,0,0);
    
	// 공의 시작 위치와 움직일 양을 설정합니다.
	whiteBall.x = 250;
	whiteBall.y = 40;
	whiteBall.vx = 10;
	whiteBall.vy = 10;

	//왼쪽 막대의 위치를 설정합니다.
	left_bar.START_X = 0;
	left_bar.START_Y = 550;
	left_bar.END_X = 220;
	left_bar.END_Y = 600;

	//오른쪽 막대의 위치를 설정합니다.
	right_bar.START_X = ofGetWidth();
	right_bar.START_Y = 550;
	right_bar.END_X = ofGetWidth() - 220;
	right_bar.END_Y = 600;
	
	//draw_flag : d키를 눌렀는가? load_flag : 파일이 로드되었는가? die_flag : 게임오버가 되었는가?
    draw_flag = 0;
    load_flag = 0;
	die_flag = 0;
	//from_X : 현재 공의 위치 이전의 x 위치. 만약 250 -> 260으로 움직였다면 whiteBall.x = 260, from_X = 250입니다.
	from_X = whiteBall.x;
}

//--------------------------------------------------------------
void ofApp::update(){
	if (s_pressed) {	//s키를 눌렀을 때만 업데이트합니다.
		// 공의 x, y좌표를 업데이트합니다.
		whiteBall.x = whiteBall.x + whiteBall.vx;
		whiteBall.y = whiteBall.y + whiteBall.vy;
		// '/' 키를 눌렀을 때 막대를 위로 올리기 위해 y 끝점 좌표를 조정합니다.
		if (right_pressed) {
			right_bar.END_Y = 500;
		}
		// 'z' 키를 눌렀을 때 막대를 위로 올리기 위해 y 끝점 좌표를 조정합니다.
		if (left_pressed) {
			left_bar.END_Y = 500;
		}
		// 공이 막대 사이로 떨어지면 die_flag를 활성화하고 s_pressed를 0으로 설정합니다. (게임오버)
		if (whiteBall.x > left_bar.END_X && whiteBall.x < right_bar.END_X) {
			if (whiteBall.y >= 600) {
				die_flag = 1;
				s_pressed = 0;
			}
		}
		// 공이 윈도우 창의 테두리에 도달하면 반사될 수 있도록 vx, vy의 부호를 변경합니다.
		if (whiteBall.x<0 || whiteBall.x > ofGetWidth()) {
			whiteBall.vx = -whiteBall.vx;
		}
		if (whiteBall.y < 40 || whiteBall.y >= ofGetHeight()) {
			whiteBall.vy = -whiteBall.vy;
		}
		// 앞서 입력받은 오브젝트와 부딪혔을 때 반사합니다. 점수는 30점을 얻습니다.
		for (int i = 0; i < num_of_obj; i++) {
			float distance = (objlist[i].x - whiteBall.x)*(objlist[i].x - whiteBall.x) + (objlist[i].y - whiteBall.y)*(objlist[i].y - whiteBall.y);
			if (distance <= 900) {		// 공의 반지름은 10, 오브젝트의 반지름은 20이므로 distance가 900보다 작을 때 반사합니다.
					whiteBall.vx = -whiteBall.vx;
					whiteBall.x += (whiteBall.x - objlist[i].x);
					score += 30;
			}
		}
		getReflection();	// 막대와 공이 부딪힌다면 반사합니다.
		from_X = whiteBall.x;		//from_X를 업데이트합니다. getReflection()함수에서 from_X의 정보가 필요합니다.
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	// 윈도우의 위 아래에 네모를 그립니다. (자주색)
    ofSetColor(127,23,31);
    ofDrawRectangle(0, 0, 500, 40); // 왼쪽 상단 지점 (0, 0)을 기준으로 너비 500, 높이 40짜리 네모를 그립니다.
    ofDrawRectangle(0, 660, 500, 40); // 왼쪽 상단 지점 (0, 660)을 기준으로 너비 500, 높이 40짜리 네모를 그립니다.

	// 점수를 표시합니다.
	ofSetColor(255);
	ofDrawBitmapString("You Got : ", 10, 25);
	ofDrawBitmapString(score, 100, 25);			// 화면에 현재 점수를 나타냅니다.
    
	// d키를 눌렀을 때
    if( draw_flag ){
		// s키를 눌렀을 때만 공을 그립니다.
		if (s_pressed) {
			ofSetColor(255, 255, 255);
			ofDrawCircle(whiteBall.x, whiteBall.y, 10);		// 반지름 : 10, 흰색으로 공을 그립니다
		}

		// 왼쪽 막대와 오른쪽 막대를 그립니다.
		ofSetLineWidth(10);		// 굵기 : 10
		ofSetColor(76, 0, 153);		// 보라색으로 설정합니다.
		ofDrawLine(left_bar.START_X, left_bar.START_Y, left_bar.END_X, left_bar.END_Y);
		ofDrawLine(right_bar.START_X, right_bar.START_Y, right_bar.END_X, right_bar.END_Y);
		
		// 초록색 박스를 막대 사이에 그립니다. 이 박스에 공이 들어가면 게임이 오버됩니다.
		ofSetColor(0, 153, 0);
		ofDrawRectangle(225, 610, 50, 50);

		// 앞서 입력받은 오브젝트들을 화면에 그립니다.
		for (int i = 0; i < num_of_obj; i++) {
			ofColor color = ofColor(ofRandom(0, 255), ofRandom(0, 255), ofRandom(0, 255));	// 컬러를 랜덤으로 지정합니다.
			ofSetColor(color);
			ofDrawCircle(objlist[i].x, objlist[i].y, 20);
		}

		// 게임이 오버되었다면
		if (die_flag == 1 && s_pressed == 0) {
			ofClear(0);		// 화면을 검은색으로 덮습니다.
			ofSetColor(204, 0, 0);		// 어두운 빨간색으로 문구를 출력합니다.
			ofDrawBitmapString("You Died!!!", 200, 350);
			ofDrawBitmapString("You Got...", 175, 375);
			ofDrawBitmapString(score, 275, 375);
			ofDrawBitmapString("Press 'R' to make rank ...", 150, 400);
		}
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 'r') {
		MakeRank(score);		// 랭킹을 생성합니다.
    }
    if (key == 'q'){
        // 플래그를 초기화합니다.
        draw_flag = 0;
		load_flag = 0;
		die_flag = 0;
		s_pressed = 0;
		left_pressed = 0;
		right_pressed = 0;

        // 메모리를 할당 해제하고 종료합니다.
		freeMemory();
        cout << "Dynamically allocated memory has been freed." << endl;
        
        _Exit(0);
    }
    if (key == 'd'){
        if( !load_flag) return;		// 파일이 로드되지 않았다면 동작하지 않습니다.
		draw_flag = 1;		// draw_flag를 활성화하여 draw함수가 동작할 수 있도록 만듭니다.
    }
    if (key == 's'){
		die_flag = 0;		// die_flag를 0으로 설정합니다.
		s_pressed = 1;		// s_pressed를 활성화합니다.
    }
    if (key == '/'){
		right_pressed = 1;		// 오른쪽 막대를 올릴 수 있도록 right_pressed를 활성화합니다.
    }
	if (key == 'z') {
		left_pressed = 1;		// 왼쪽 막대를 올릴 수 있도록 left_pressed를 활성화합니다.
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if( key == 'l'){
        // Open the Open File Dialog
        ofFileDialogResult openFileResult= ofSystemLoadDialog("Select a only txt for pinball");
        
        // Check whether the user opened a file
        if( openFileResult.bSuccess){
            ofLogVerbose("User selected a file");
            
            // We have a file, so let's check it and process it
            processOpenFileSelection(openFileResult);
            load_flag = 1;
        }
    }
    
    if (key == '/'){
		right_pressed = 0;
		right_bar.END_Y = 600;		// '/' 키를 뗐을 때, 막대 위치를 되돌릴 수 있도록 keypressed에서 변경했던 것들을 되돌립니다.
    }
    if (key == 'z'){
		left_pressed = 0;
		left_bar.END_Y = 600;		// 'z' 키를 뗐을 때, 막대 위치를 되돌릴 수 있도록 keypressed에서 변경했던 것들을 되돌립니다.
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
 
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::processOpenFileSelection(ofFileDialogResult openFileResult) { 
    //Path to the comma delimited file
    
    string fileName = openFileResult.getName();
    ofFile file(fileName);
    
    if( !file.exists()) cout << "Target file does not exists." << endl;
    else cout << "We found the target file." << endl;
    
    ofBuffer buffer(file);
	int obj_idx = 0;
    // Read file line by line
    for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
        string line = *it;
        
        // Split line into strings
        vector<string> words = ofSplitString(line, " ");
        
        if( words.size() == 1){
            // 오브젝트의 개수를 받아옵니다.
                num_of_obj = atoi(words[0].c_str());
                cout << "The number of object is: " << num_of_obj << endl;
				objlist = (obj *)malloc(sizeof(obj)*num_of_obj);		// 오브젝트의 개수만큼 오브젝트 리스트를 만듭니다. (구조체배열)
        }
        else if (words.size() >= 2){
            int x1,y1;
            // 오브젝트의 x, y위치를 읽어와서 오브젝트 리스트에 기록합니다.
            x1 = atoi(words[0].c_str());
            y1 = atoi(words[1].c_str());
			objlist[obj_idx].x = x1;
			objlist[obj_idx++].y = y1;
        } // End of else if.
    } // End of for-loop (Read file line by line).
}
void ofApp::getReflection() {
	float y;
	// 공이 왼쪽 막대와 닿는지 확인합니다.
	if (whiteBall.x <= left_bar.END_X && whiteBall.x >= left_bar.START_X) {
		left_bar.slope = (float)(left_bar.END_Y - left_bar.START_Y) / (left_bar.END_X - left_bar.START_X);	// 왼쪽 막대의 기울기
		y = left_bar.slope * (whiteBall.x - left_bar.START_X) + left_bar.START_Y;	// 공의 x위치를 이용해서 막대에 닿았을 때의 y좌표를 계산합니다.
		if (y < whiteBall.y && !die_flag) {		// 게임이 오버되지 않았고 공이 튕겨져야 할 때
			whiteBall.y = y;
			if (!left_pressed) {		// 키를 눌러 올려치지 않았을 때는 속도가 느려지고 점수는 10점만 받습니다.
				whiteBall.vy = -whiteBall.vy * 0.8;
				score += 10;
			}
			else {		// 키를 눌러 올려 쳤을 때는 속도는 빨라지고 점수를 300점을 얻습니다.
				if(from_X <= whiteBall.x)		// 방향 조절을 위한 조건
					whiteBall.vx = -whiteBall.vx;
				whiteBall.vy = -whiteBall.vy * (1 + abs(left_bar.slope));
				score += 300;
			}
		}
	}
	// 공이 오른쪽 막대와 닿는지 확인합니다.
	if (whiteBall.x <= right_bar.START_X && whiteBall.x >= right_bar.END_X) {
		right_bar.slope = (float)(right_bar.END_Y - right_bar.START_Y) / (right_bar.END_X - right_bar.START_X);	 // 오른쪽 막대의 기울기
		y = right_bar.slope * (whiteBall.x - right_bar.START_X) + right_bar.START_Y;	// 공의 x위치를 이용해서 막대에 닿았을 때의 y좌표를 계산합니다.
		if (y < whiteBall.y && !die_flag) {		// 게임이 오버되지 않았고 공이 튕겨져야 할 때
			whiteBall.y = y;
			if (!right_pressed) {		// 키를 눌러 올려치지 않았을 때는 속도가 느려지고 점수는 10점만 받습니다.
				whiteBall.vy = -whiteBall.vy * 0.8;
				score += 10;
			}
			else {		// 키를 눌러 올려 쳤을 때는 속도는 빨라지고 점수를 300점을 얻습니다.
				if (from_X >= whiteBall.x)		// 방향 조절을 위한 조건
					whiteBall.vx = -whiteBall.vx;
				whiteBall.vy = -whiteBall.vy * (1 + abs(right_bar.slope));
				score += 300;
			}
		}
	}
}
void ofApp::ReadRank() {
	FILE *fp = fopen("rank.txt", "r");		// rank.txt 파일을 읽습니다.
	int tmpscore;
	if (fp == NULL) {		// 파일이 없다면 생성합니다.
		ranknum = 0;
		fp = fopen("rank.txt", "w");
		fprintf(fp, "%d", 0);
	}
	else {
		// 파일이 존재한다면 랭크 리스트를 읽어 연결리스트를 생성합니다.
		fscanf(fp, "%d", &ranknum);
		int i = 0;
		for (i = 0; i < ranknum; i++) {
			Node *newnode = NULL;
			newnode = (Node *)malloc(sizeof(Node));
			fscanf(fp, "%s %d", newnode->name, &tmpscore);
			newnode->score = tmpscore;
			newnode->link = NULL;
			Node *tmp;
			if (rankhead == NULL) {
				rankhead = newnode;
			}
			else {
				tmp = rankhead;
				while (tmp->link != NULL) {
					tmp = tmp->link;
				}
				tmp->link = newnode;
			}
		}
	}
	fclose(fp);
}
void ofApp::MakeRank(int score) {
	ReadRank();		// rank.txt를 읽어 랭크 리스트를 받아옵니다.
	Node *newnode = NULL;
	newnode = (Node *)malloc(sizeof(Node));
	printf("Your name: ");		// 플레이어의 이름을 입력받고 이름과 점수를 랭크 리스트의 올바른 위치에 삽입합니다.
	scanf("%s", newnode->name);
	newnode->score = score;
	newnode->link = NULL;
	Node *pre = rankhead, *tmp = rankhead;
	int flag = 0;
	if (rankhead == NULL) {		// 랭크 리스트가 없을 때
		rankhead = newnode;
	}
	else {		// 랭크 리스트가 존재하면 알맞은 위치를 찾아 삽입합니다.
		if (newnode->score >= rankhead->score) {
			newnode->link = rankhead;
			rankhead = newnode;
		}
		else {
			while (tmp->link != NULL) {
				if (newnode->score >= tmp->score && tmp->link != NULL) {
					pre->link = newnode;
					newnode->link = tmp;
					flag = 1;
					break;
				}
				pre = tmp;
				tmp = tmp->link;
			}
			if (flag == 0 && newnode->score <= tmp->score) {
				tmp->link = newnode;
			}
			else if (flag == 0 && newnode->score > tmp->score) {
				pre->link = newnode;
				newnode->link = tmp;
			}
		}
	}
	ranknum++;
	WriteRank();		// 저장한 랭크 리스트를 파일 출력합니다.
}
void ofApp::WriteRank() {
	// rank.txt파일로 랭킹을 출력합니다.
	FILE *fp = fopen("rank.txt", "w");		
	int i = 0;
	Node *curr = rankhead;
	fprintf(fp, "%d\n", ranknum);
	for (i = 0; i < ranknum; i++) {
		fprintf(fp, "%s %d\n", curr->name, curr->score);
		curr = curr->link;
	}
	fclose(fp);
	freeMemory();		// 종료하기 전에 메모리를 할당 해제합니다.
	cout << "Your score has been recorded." << endl;
	cout << "Dynamically allocated memory has been freed." << endl;
	_Exit(0);
}
void ofApp::freeMemory() {
	free(objlist);		// 오브젝트 리스트(구조체 배열)를 해제합니다.
	Node *temp = rankhead;		// 랭크 리스트를 해제합니다.
	Node *prev = temp;
	if (rankhead) {
		while (temp != NULL) {
			temp = temp->link;
			free(prev);
			prev = temp;
		}
	}
}
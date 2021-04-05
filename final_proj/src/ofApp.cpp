#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(30); //�������� 30���� ����
    
    // ����� ���������� �����մϴ�.
    ofBackground(0,0,0);
    
	// ���� ���� ��ġ�� ������ ���� �����մϴ�.
	whiteBall.x = 250;
	whiteBall.y = 40;
	whiteBall.vx = 10;
	whiteBall.vy = 10;

	//���� ������ ��ġ�� �����մϴ�.
	left_bar.START_X = 0;
	left_bar.START_Y = 550;
	left_bar.END_X = 220;
	left_bar.END_Y = 600;

	//������ ������ ��ġ�� �����մϴ�.
	right_bar.START_X = ofGetWidth();
	right_bar.START_Y = 550;
	right_bar.END_X = ofGetWidth() - 220;
	right_bar.END_Y = 600;
	
	//draw_flag : dŰ�� �����°�? load_flag : ������ �ε�Ǿ��°�? die_flag : ���ӿ����� �Ǿ��°�?
    draw_flag = 0;
    load_flag = 0;
	die_flag = 0;
	//from_X : ���� ���� ��ġ ������ x ��ġ. ���� 250 -> 260���� �������ٸ� whiteBall.x = 260, from_X = 250�Դϴ�.
	from_X = whiteBall.x;
}

//--------------------------------------------------------------
void ofApp::update(){
	if (s_pressed) {	//sŰ�� ������ ���� ������Ʈ�մϴ�.
		// ���� x, y��ǥ�� ������Ʈ�մϴ�.
		whiteBall.x = whiteBall.x + whiteBall.vx;
		whiteBall.y = whiteBall.y + whiteBall.vy;
		// '/' Ű�� ������ �� ���븦 ���� �ø��� ���� y ���� ��ǥ�� �����մϴ�.
		if (right_pressed) {
			right_bar.END_Y = 500;
		}
		// 'z' Ű�� ������ �� ���븦 ���� �ø��� ���� y ���� ��ǥ�� �����մϴ�.
		if (left_pressed) {
			left_bar.END_Y = 500;
		}
		// ���� ���� ���̷� �������� die_flag�� Ȱ��ȭ�ϰ� s_pressed�� 0���� �����մϴ�. (���ӿ���)
		if (whiteBall.x > left_bar.END_X && whiteBall.x < right_bar.END_X) {
			if (whiteBall.y >= 600) {
				die_flag = 1;
				s_pressed = 0;
			}
		}
		// ���� ������ â�� �׵θ��� �����ϸ� �ݻ�� �� �ֵ��� vx, vy�� ��ȣ�� �����մϴ�.
		if (whiteBall.x<0 || whiteBall.x > ofGetWidth()) {
			whiteBall.vx = -whiteBall.vx;
		}
		if (whiteBall.y < 40 || whiteBall.y >= ofGetHeight()) {
			whiteBall.vy = -whiteBall.vy;
		}
		// �ռ� �Է¹��� ������Ʈ�� �ε����� �� �ݻ��մϴ�. ������ 30���� ����ϴ�.
		for (int i = 0; i < num_of_obj; i++) {
			float distance = (objlist[i].x - whiteBall.x)*(objlist[i].x - whiteBall.x) + (objlist[i].y - whiteBall.y)*(objlist[i].y - whiteBall.y);
			if (distance <= 900) {		// ���� �������� 10, ������Ʈ�� �������� 20�̹Ƿ� distance�� 900���� ���� �� �ݻ��մϴ�.
					whiteBall.vx = -whiteBall.vx;
					whiteBall.x += (whiteBall.x - objlist[i].x);
					score += 30;
			}
		}
		getReflection();	// ����� ���� �ε����ٸ� �ݻ��մϴ�.
		from_X = whiteBall.x;		//from_X�� ������Ʈ�մϴ�. getReflection()�Լ����� from_X�� ������ �ʿ��մϴ�.
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	// �������� �� �Ʒ��� �׸� �׸��ϴ�. (���ֻ�)
    ofSetColor(127,23,31);
    ofDrawRectangle(0, 0, 500, 40); // ���� ��� ���� (0, 0)�� �������� �ʺ� 500, ���� 40¥�� �׸� �׸��ϴ�.
    ofDrawRectangle(0, 660, 500, 40); // ���� ��� ���� (0, 660)�� �������� �ʺ� 500, ���� 40¥�� �׸� �׸��ϴ�.

	// ������ ǥ���մϴ�.
	ofSetColor(255);
	ofDrawBitmapString("You Got : ", 10, 25);
	ofDrawBitmapString(score, 100, 25);			// ȭ�鿡 ���� ������ ��Ÿ���ϴ�.
    
	// dŰ�� ������ ��
    if( draw_flag ){
		// sŰ�� ������ ���� ���� �׸��ϴ�.
		if (s_pressed) {
			ofSetColor(255, 255, 255);
			ofDrawCircle(whiteBall.x, whiteBall.y, 10);		// ������ : 10, ������� ���� �׸��ϴ�
		}

		// ���� ����� ������ ���븦 �׸��ϴ�.
		ofSetLineWidth(10);		// ���� : 10
		ofSetColor(76, 0, 153);		// ��������� �����մϴ�.
		ofDrawLine(left_bar.START_X, left_bar.START_Y, left_bar.END_X, left_bar.END_Y);
		ofDrawLine(right_bar.START_X, right_bar.START_Y, right_bar.END_X, right_bar.END_Y);
		
		// �ʷϻ� �ڽ��� ���� ���̿� �׸��ϴ�. �� �ڽ��� ���� ���� ������ �����˴ϴ�.
		ofSetColor(0, 153, 0);
		ofDrawRectangle(225, 610, 50, 50);

		// �ռ� �Է¹��� ������Ʈ���� ȭ�鿡 �׸��ϴ�.
		for (int i = 0; i < num_of_obj; i++) {
			ofColor color = ofColor(ofRandom(0, 255), ofRandom(0, 255), ofRandom(0, 255));	// �÷��� �������� �����մϴ�.
			ofSetColor(color);
			ofDrawCircle(objlist[i].x, objlist[i].y, 20);
		}

		// ������ �����Ǿ��ٸ�
		if (die_flag == 1 && s_pressed == 0) {
			ofClear(0);		// ȭ���� ���������� �����ϴ�.
			ofSetColor(204, 0, 0);		// ��ο� ���������� ������ ����մϴ�.
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
		MakeRank(score);		// ��ŷ�� �����մϴ�.
    }
    if (key == 'q'){
        // �÷��׸� �ʱ�ȭ�մϴ�.
        draw_flag = 0;
		load_flag = 0;
		die_flag = 0;
		s_pressed = 0;
		left_pressed = 0;
		right_pressed = 0;

        // �޸𸮸� �Ҵ� �����ϰ� �����մϴ�.
		freeMemory();
        cout << "Dynamically allocated memory has been freed." << endl;
        
        _Exit(0);
    }
    if (key == 'd'){
        if( !load_flag) return;		// ������ �ε���� �ʾҴٸ� �������� �ʽ��ϴ�.
		draw_flag = 1;		// draw_flag�� Ȱ��ȭ�Ͽ� draw�Լ��� ������ �� �ֵ��� ����ϴ�.
    }
    if (key == 's'){
		die_flag = 0;		// die_flag�� 0���� �����մϴ�.
		s_pressed = 1;		// s_pressed�� Ȱ��ȭ�մϴ�.
    }
    if (key == '/'){
		right_pressed = 1;		// ������ ���븦 �ø� �� �ֵ��� right_pressed�� Ȱ��ȭ�մϴ�.
    }
	if (key == 'z') {
		left_pressed = 1;		// ���� ���븦 �ø� �� �ֵ��� left_pressed�� Ȱ��ȭ�մϴ�.
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
		right_bar.END_Y = 600;		// '/' Ű�� ���� ��, ���� ��ġ�� �ǵ��� �� �ֵ��� keypressed���� �����ߴ� �͵��� �ǵ����ϴ�.
    }
    if (key == 'z'){
		left_pressed = 0;
		left_bar.END_Y = 600;		// 'z' Ű�� ���� ��, ���� ��ġ�� �ǵ��� �� �ֵ��� keypressed���� �����ߴ� �͵��� �ǵ����ϴ�.
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
            // ������Ʈ�� ������ �޾ƿɴϴ�.
                num_of_obj = atoi(words[0].c_str());
                cout << "The number of object is: " << num_of_obj << endl;
				objlist = (obj *)malloc(sizeof(obj)*num_of_obj);		// ������Ʈ�� ������ŭ ������Ʈ ����Ʈ�� ����ϴ�. (����ü�迭)
        }
        else if (words.size() >= 2){
            int x1,y1;
            // ������Ʈ�� x, y��ġ�� �о�ͼ� ������Ʈ ����Ʈ�� ����մϴ�.
            x1 = atoi(words[0].c_str());
            y1 = atoi(words[1].c_str());
			objlist[obj_idx].x = x1;
			objlist[obj_idx++].y = y1;
        } // End of else if.
    } // End of for-loop (Read file line by line).
}
void ofApp::getReflection() {
	float y;
	// ���� ���� ����� ����� Ȯ���մϴ�.
	if (whiteBall.x <= left_bar.END_X && whiteBall.x >= left_bar.START_X) {
		left_bar.slope = (float)(left_bar.END_Y - left_bar.START_Y) / (left_bar.END_X - left_bar.START_X);	// ���� ������ ����
		y = left_bar.slope * (whiteBall.x - left_bar.START_X) + left_bar.START_Y;	// ���� x��ġ�� �̿��ؼ� ���뿡 ����� ���� y��ǥ�� ����մϴ�.
		if (y < whiteBall.y && !die_flag) {		// ������ �������� �ʾҰ� ���� ƨ������ �� ��
			whiteBall.y = y;
			if (!left_pressed) {		// Ű�� ���� �÷�ġ�� �ʾ��� ���� �ӵ��� �������� ������ 10���� �޽��ϴ�.
				whiteBall.vy = -whiteBall.vy * 0.8;
				score += 10;
			}
			else {		// Ű�� ���� �÷� ���� ���� �ӵ��� �������� ������ 300���� ����ϴ�.
				if(from_X <= whiteBall.x)		// ���� ������ ���� ����
					whiteBall.vx = -whiteBall.vx;
				whiteBall.vy = -whiteBall.vy * (1 + abs(left_bar.slope));
				score += 300;
			}
		}
	}
	// ���� ������ ����� ����� Ȯ���մϴ�.
	if (whiteBall.x <= right_bar.START_X && whiteBall.x >= right_bar.END_X) {
		right_bar.slope = (float)(right_bar.END_Y - right_bar.START_Y) / (right_bar.END_X - right_bar.START_X);	 // ������ ������ ����
		y = right_bar.slope * (whiteBall.x - right_bar.START_X) + right_bar.START_Y;	// ���� x��ġ�� �̿��ؼ� ���뿡 ����� ���� y��ǥ�� ����մϴ�.
		if (y < whiteBall.y && !die_flag) {		// ������ �������� �ʾҰ� ���� ƨ������ �� ��
			whiteBall.y = y;
			if (!right_pressed) {		// Ű�� ���� �÷�ġ�� �ʾ��� ���� �ӵ��� �������� ������ 10���� �޽��ϴ�.
				whiteBall.vy = -whiteBall.vy * 0.8;
				score += 10;
			}
			else {		// Ű�� ���� �÷� ���� ���� �ӵ��� �������� ������ 300���� ����ϴ�.
				if (from_X >= whiteBall.x)		// ���� ������ ���� ����
					whiteBall.vx = -whiteBall.vx;
				whiteBall.vy = -whiteBall.vy * (1 + abs(right_bar.slope));
				score += 300;
			}
		}
	}
}
void ofApp::ReadRank() {
	FILE *fp = fopen("rank.txt", "r");		// rank.txt ������ �н��ϴ�.
	int tmpscore;
	if (fp == NULL) {		// ������ ���ٸ� �����մϴ�.
		ranknum = 0;
		fp = fopen("rank.txt", "w");
		fprintf(fp, "%d", 0);
	}
	else {
		// ������ �����Ѵٸ� ��ũ ����Ʈ�� �о� ���Ḯ��Ʈ�� �����մϴ�.
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
	ReadRank();		// rank.txt�� �о� ��ũ ����Ʈ�� �޾ƿɴϴ�.
	Node *newnode = NULL;
	newnode = (Node *)malloc(sizeof(Node));
	printf("Your name: ");		// �÷��̾��� �̸��� �Է¹ް� �̸��� ������ ��ũ ����Ʈ�� �ùٸ� ��ġ�� �����մϴ�.
	scanf("%s", newnode->name);
	newnode->score = score;
	newnode->link = NULL;
	Node *pre = rankhead, *tmp = rankhead;
	int flag = 0;
	if (rankhead == NULL) {		// ��ũ ����Ʈ�� ���� ��
		rankhead = newnode;
	}
	else {		// ��ũ ����Ʈ�� �����ϸ� �˸��� ��ġ�� ã�� �����մϴ�.
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
	WriteRank();		// ������ ��ũ ����Ʈ�� ���� ����մϴ�.
}
void ofApp::WriteRank() {
	// rank.txt���Ϸ� ��ŷ�� ����մϴ�.
	FILE *fp = fopen("rank.txt", "w");		
	int i = 0;
	Node *curr = rankhead;
	fprintf(fp, "%d\n", ranknum);
	for (i = 0; i < ranknum; i++) {
		fprintf(fp, "%s %d\n", curr->name, curr->score);
		curr = curr->link;
	}
	fclose(fp);
	freeMemory();		// �����ϱ� ���� �޸𸮸� �Ҵ� �����մϴ�.
	cout << "Your score has been recorded." << endl;
	cout << "Dynamically allocated memory has been freed." << endl;
	_Exit(0);
}
void ofApp::freeMemory() {
	free(objlist);		// ������Ʈ ����Ʈ(����ü �迭)�� �����մϴ�.
	Node *temp = rankhead;		// ��ũ ����Ʈ�� �����մϴ�.
	Node *prev = temp;
	if (rankhead) {
		while (temp != NULL) {
			temp = temp->link;
			free(prev);
			prev = temp;
		}
	}
}
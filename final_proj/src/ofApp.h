#pragma once

#include "ofMain.h"
#include <stdio.h>
#include <stdlib.h>
#define NAMELEN 30		// �̸��� �ִ����
// ���� ���� ������ ���� ����ü
struct Ball {
	float x;
	float y;
	float vx;
	float vy;
};
// ���뿡 ���� ������ ���� ����ü
struct Bar {
	int START_X;
	int START_Y;
	int END_X;
	int END_Y;
	float slope = 0;
};
// ������Ʈ�� ���� ������ ���� ����ü
struct obj {
	int x, y;
};
// ��ŷ ����Ʈ�� ����µ� ����� ����ü
typedef struct _Node {
	char name[NAMELEN];
	int score;
	struct _Node *link;
}Node;

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    /* Project-related member variables Regions */

    // �÷��� ����
    int draw_flag;
    int load_flag;
	int s_pressed = 0;
	int left_pressed = 0;
	int right_pressed = 0;
	int die_flag = 0;
    // ������� ��ü�鿡 ���� ����
    int num_of_obj;
	Ball whiteBall;
	Bar right_bar;
	Bar left_bar;
	obj *objlist;
    // �ΰ������� �ʿ��� ����
	int score = 0;
	int ranknum = 0;
	int from_X;
	Node *rankhead = NULL;

	// user defined function
    void processOpenFileSelection(ofFileDialogResult openFileResult);
	void getReflection();
	void ReadRank();
	void MakeRank(int);
	void WriteRank();
	void freeMemory();
};

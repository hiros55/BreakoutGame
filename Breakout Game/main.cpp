#include<GL/glut.h>
#include <cmath>
#include "Collider.h"
#define WIDTH 640
#define HEIGHT 640
#define PI 3.1415926

//カメラ用変数(Z)
float cameraDistance = 100;

//ブロック用変数
int BlockLife[12][5];

//バー用変数
int BarWidth = 2;
int BarHeight = 4;

//球用変数
const float PI_OVER_180 = 0.0174532925;
float  radius = 0;//最初のアングルは前の挙動を考慮しないため値設定には注意
float BallMoveX = 0, BallMoveY = 1;
float posx = 0, velx = 0, angle = 81;//球移動用変数

//画面外当たり判定用変数
float FrameOver = 30;
float FrameRestriction = 5.5;


//GameControl
bool GameOverBool = true;
bool GameStartBool = true;
bool GameStop = true;
const char* ac[] = { "GameOver","Push [f] Start","Push [g] Stop and Restart" };

//当たり判定クラス
Collider collider;


void UpdateBall() {
	radius = angle * PI / 180;
	BallMoveX = cos(radius);
	BallMoveY = sin(radius);
}

void XCol() {
	angle = 180 - angle;
	UpdateBall();
}

void YCol() {
	angle = 360 - angle;
	UpdateBall();
}

void GameInitialize() {
	for (int i = 0; i < 12; i++)
		for (int l = 0; l < 5; l++)
			BlockLife[i][l] = 0;

	collider.X = 0;
	collider.Y = -20;
}


//画面外当たり判定
void FrameCollider(float FrameOver) {

	collider.X += BallMoveX;
	collider.Y += BallMoveY;

	if (collider.X <= -FrameOver || collider.X >= FrameOver) {
		XCol();
	}
	if (collider.Y > FrameOver) {
		YCol();
	}

	if (collider.Y < -FrameOver) {
		YCol();
		GameOverBool = true;
		GameStartBool = false;
		GameInitialize();
	}
}


void BlockCollider(int XMin, int XMax, int YMin, int YMax, float radius) {
	if (collider.StaticCollision(XMin, XMax, YMin, YMax, radius) == 1) {
		XCol();
		BlockLife[(XMin + 30) / 5][YMin / 3 - 3] = 1;
	}
	if (collider.StaticCollision(XMin, XMax, YMin, YMax, radius) == 2) {
		YCol();
		BlockLife[(XMin + 30) / 5][YMin / 3 - 3] = 1;
	}
}


void BarCollider(int XMin, int XMax, int YMin, int YMax, float radius) {
	if (collider.BarCollision(XMin, XMax, YMin, YMax, radius) == 1) {
		XCol();
	}

	if (collider.BarCollision(XMin, XMax, YMin, YMax, radius) == 2) {
		YCol();
	}
}


void BlockInstantiate(int n, int m) {
	for (int x = -n; x < n; x = x + 5) {
		for (int y = 12; y < m + 9; y = y + 3) {
			if (BlockLife[(x + 30) / 5][y / 3 - 3] <= 0) {
				glColor3f(0.0, 0.0, 1.0);
				glBegin(GL_POLYGON);
				glVertex2f(x, y);
				glVertex2f(x + BarHeight, y);
				glVertex2f(x + BarHeight, y + BarWidth);
				glVertex2f(x, y + BarWidth);
				BlockCollider(x, x + BarHeight, y, y + BarWidth, 1);
				glEnd();
			}
		}
	}
}

void BarInstantiate(int x, int y) {
	glBegin(GL_POLYGON);
	glColor3f(0.0, 1.0, 1.0);
	glVertex2d(x, y);
	glVertex2d(x + 4, y);
	glVertex2d(x + 4, y + 2);
	glVertex2d(x, y + 2);
	BarCollider(x + posx, x + 4 + posx, y, y + 4, 1);
	glEnd();
}

void BarMove() {
	//移動制限
	if (posx >= -FrameOver + FrameRestriction + 3 && posx <= FrameOver + FrameRestriction)
		posx += velx;
	else if (posx > FrameOver + FrameRestriction)
		posx = FrameOver + FrameRestriction - 0.1;
	else if (posx < -FrameOver + FrameRestriction + 3)
		posx = -FrameOver + FrameRestriction + 3 + 0.1;
}

void BallInstantiate(float radius, float step) {
	glColor3f(0, 1, 0);
	float StartAngle;
	glBegin(GL_POLYGON);
	for (float angle = 0; angle < 360.0; angle += step) {
		StartAngle = angle * PI_OVER_180;
		glVertex2f(radius * cos(StartAngle), radius * sin(StartAngle));
	}
	glEnd();
}


//テキスト表示()
void DrawTex(int i, int y) {
	const char* a = ac[i];
	glRasterPos2i(-16, y);
	for (int i = 0; a[i] != '\0'; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, a[i]);
}

//ボール移動（当たり判定含む）
void idle(void)
{
	if (GameStartBool == true) {
		if (GameStop == false) {
			FrameCollider(FrameOver);
			BarMove();
		}
	}
	Sleep(20);
}

void DisPlay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -cameraDistance);

	//ステージ枠
	glBegin(GL_LINE_LOOP);
	glColor3f(0, 0, 1);
	glVertex2f(FrameOver + 1, FrameOver + 1);
	glVertex2f(-FrameOver - 1, FrameOver + 1);
	glVertex2f(-FrameOver - 1, -FrameOver - 1);
	glVertex2f(FrameOver + 1, -FrameOver - 1);
	glEnd();

	if (GameOverBool == false) {
		DrawTex(2, 35);
		glColor3f(0, 1, 0);
		BlockInstantiate(30, 15);

		glTranslatef(posx, 0, 0);
		BarInstantiate(-10, -25);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(collider.X, collider.Y, 0);
		BallInstantiate(1, 0.1);
	}
	else {
		DrawTex(0, 0);
		DrawTex(1, 35);
	}
	glutPostRedisplay();
	glutSwapBuffers();
	glFlush();
}

void Reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (double)w / h, 1, 250);
}

void Keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'a':
	case 'A':
		velx = -0.5;
		break;
	case 'd':
	case 'D':
		velx = 0.5;
		break;
	case 'F':
	case 'f':
		GameStop = true;
		GameOverBool = false;
		GameStartBool = true;
		break;
	case 'g':
	case 'G':
		GameStop = !GameStop;
		break;
	default:
		velx = 0;
		break;
	}
}

void keyboardup(unsigned char key, int x, int y) {
	switch (key)
	{
	default:
		velx = 0;
		break;
	}
}

void Init() {
	glClearColor(1, 1, 1, 0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("ShootingGame");
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_STENCIL);

	glutIdleFunc(idle);
	glutDisplayFunc(DisPlay);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutKeyboardUpFunc(keyboardup);

	Init();
	glutMainLoop();

	return 0;
}
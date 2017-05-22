#include <string>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <windows.h>
#include <GL/glut.h>

using namespace std;

const int N = 3; // field count 
const double scaleRGB = 1.0 / 255; // scale for fucking RGB colors.

const int Scale = 140; // scale for size of ceil.

const int wh = N * Scale; // w and h.

bool gamer = 1; // 1 - x, 0 - O
int win = 0; // 0 no win
int coun = 0; // count that i did involute.

int winColumn = -1, winLine = -1; // wins column, line.

int c[N][N]; //create an array of value 

void init() {
	glClearColor(1, 1, 1, 1.0);

	for (int a = 0; a < N; a++) {
		for (int b = 0; b < N; b++) {
			c[a][b] = -1;
		}
	}

	gamer = 1;
	coun = 0;
}

void display(); // forward of display
void checkWin(); // forward of checkWin

void ifWin(int player, string text) { //if -1 then field had been fill.
	if (player != -1) cout << " " << text << " win. Player is " << player << endl;
	else cout << text << endl;

	if (player == -1) glClearColor(235 * scaleRGB, 168 * scaleRGB, 168 * scaleRGB, 0);
	if (player == 1) glClearColor(68 * scaleRGB, 168 * scaleRGB, 71 * scaleRGB, 0); // if x
	if (player == 0) glClearColor(68 * scaleRGB, 125 * scaleRGB, 168 * scaleRGB, 0); // if 0

	display();

	Sleep(1500);
	init();
	checkWin();
}

void drawWin() {
	if (win != -1) {
		glLineWidth(50);
		glColor3b(0, 0, 0); // color black
		glBegin(GL_LINES);
		if (win == 1) { //gorizontal
			glVertex2i(0, Scale*winLine + Scale / 2);
			glVertex2i(wh, Scale*winLine + Scale / 2);
		}
		if (win == 2) { //vertical 
			glVertex2i(Scale*winColumn + Scale / 2, 0);
			glVertex2i(Scale*winColumn + Scale / 2, wh);
		}
		if (win == 3) { //main diagonal
			glVertex2i(wh, 0);
			glVertex2i(0, wh);
		}
		if (win == 4) { //not main diagonal
			glVertex2i(0, 0);
			glVertex2i(wh, wh);
		}
		glEnd();
	}
}

void checkWin() { // check all win combo
	byte player;

	for (int line = 0; line < N; line++) { //check gorizontal win

		player = c[line][0];

		if (player != -1) {
			win = 1;
			for (int column = 0; column < N; column++) {
				if (c[line][column] != player) {
					win = 0;
					break;
				}
			}
			if (win) {
				winLine = line;
				ifWin(player, "GORIZONTAL");
				return;
			}
		}
	}

	for (int column = 0; column < N; column++) { //check vertical win

		player = c[0][column];

		if (player != -1) {
			win = 2;
			for (int line = 0; line < N; line++) {
				if (c[line][column] != player) {
					win = 0;
					break;
				}
			}
			if (win) {
				winColumn = column;
				ifWin(player, "VERTICAL");
				return;
			}
		}
	}
	//--------------block main diagonal
	player = c[N - 1][0];
	win = 3;

	if (player != -1) {
		for (int column = N - 1, line = 0; column >= 0 && line < N; column--, line++) {//check main diagonal
			if (c[column][line] != player) {
				win = 0;
				break;
			}
		}
		if (win) {
			ifWin(player, "Main diagonal");
			return;
		}
	}
	//-------------------block main diagonal

	//--------------block not main diagonal
	player = c[0][0];
	win = 4;

	if (player != -1) {
		for (int column = 0, line = 0; column<N && line<N; column++, line++) {//check not main diagonal
			if (c[column][line] != player) {
				win = 0;
				break;
			}
		}
		if (win) {
			ifWin(player, "Not main diagonal");
			return;
		}
	}
	//-------------------block not main diagonal

	if (coun == N*N) { //check full fiel.
		ifWin(-1, " Need remake fiel. ");
		win = 5;
		return;
	}

	win = 0;
}

void drawCircle(float x, float y, float r, int amountSegments) {
	glBegin(GL_LINE_LOOP);

	for (int i = 0; i < amountSegments; i++) {

		float angle = 2.0 * 3.1415926 * float(i) / float(amountSegments);

		float dx = r * cosf(angle);
		float dy = r * sinf(angle);

		glVertex2f(x + dx, y + dy);
	}
	glEnd();
}

void drawX(float x, float y) {
	glBegin(GL_LINES);
	glVertex2i(x * Scale, y * Scale);
	glVertex2i(x * Scale + Scale, y * Scale + Scale);
	glVertex2i(x * Scale, y * Scale + Scale);
	glVertex2i(x * Scale + Scale, y * Scale);
	glEnd();
}

void DrawField() {
	glLineWidth(1);
	glColor3b(0, 0, 0); // color black
	glBegin(GL_LINES);
	for (int xy = Scale; xy < wh; xy += Scale) {
		//draw to x
		glVertex2i(xy, 0);
		glVertex2i(xy, wh);
		//draw to y
		glVertex2i(0, xy);
		glVertex2i(wh, xy);
	}
	glEnd();
}

void DrawCards() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			glLineWidth(2);
			if (c[i][j] == 1) {
				drawX(j, i);
			}
			if (c[i][j] == 0) {
				drawCircle((j * Scale + (Scale / 2)), (i * Scale + (Scale / 2)), Scale / 2 - 2, 40);
			}
		}
	}
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	DrawField();
	drawWin();
	DrawCards();
	glutSwapBuffers();
}

void hodO() {

	int rand1 = rand() % N;
	int rand2 = rand() % N;

	while (c[rand1][rand2] != -1) {
		rand1 = rand() % N;
		rand2 = rand() % N;
	}

	c[rand1][rand2] = 0;
}

void MousePressed(int button, int state, int x, int y) {

	if (state == GLUT_DOWN) {

		int row = floor(y / Scale);
		int col = floor(x / Scale);

		if (c[row][col] == -1) { // if we have empty slot then need use it
			c[row][col] = gamer;
			cout << x << " " << y << ". row " << row << ", col " << col << " count = " << coun << endl;
			gamer = !gamer;
			coun++;
			checkWin();
			if (!gamer) {
				hodO();
				gamer = 1;
				coun++;
			}
			checkWin();
		}
	}
}

void timer(int) {
	display();
	glutTimerFunc(50, timer, 0);
}

int main(int argcp, char **argv) {

	glutInit(&argcp, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	glutInitWindowSize(wh, wh);
	glutInitWindowPosition(480, 200);
	glutCreateWindow("Tic-tac-toe");

	init();		//inicialization

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, wh, wh, 0);

	glutDisplayFunc(display);
	glutMouseFunc(MousePressed);
	glutTimerFunc(50, timer, 0);

	glutMainLoop();

	return 0;
}
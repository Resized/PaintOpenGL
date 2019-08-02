#include "glut.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <vector>

using namespace std;

const int N = 100;
const int W = 800;// window width
const int H = 600;// window height


typedef struct {
	unsigned char r, g, b;
} MY_COLOR;

typedef struct {
	int x, y;
} POINT_2D;

void drawLine(POINT_2D p1, POINT_2D p2, MY_COLOR color);
bool isColorEquals(MY_COLOR color1, MY_COLOR color2);
void replaceColor(POINT_2D node, MY_COLOR color);
MY_COLOR getColor(POINT_2D node);
void createColorPillars();

unsigned char pix[H][W][3]; // red,green and blue layers
double offset = 0;
POINT_2D firstClick;

bool isClicked = false;
MY_COLOR backgroundColor = { 255,255,255 };
MY_COLOR lineColor = { 0,0,0 };
MY_COLOR tColor = { 255,255,255 };
MY_COLOR rColor = { 255,0,0 };
MY_COLOR fillColor = { 255,0,0 };
int pillarWidth = 50;
int pillarHeight = 30;

void init()
{
	int i, j;
	glClearColor(0.5, 0.5, 0.5, 0);// color of window background

	srand(time(NULL));

	for (i = 0; i < H; i++)
		for (j = 0; j < W; j++)
		{
			pix[i][j][0] = backgroundColor.r; // R
			pix[i][j][1] = backgroundColor.g; // G
			pix[i][j][2] = backgroundColor.b; // B
		}

	createColorPillars();
}

bool isColorEquals(MY_COLOR color1, MY_COLOR color2)
{
	return color1.r == color2.r && color1.g == color2.g && color1.b == color2.b;
}

void createColorPillars()
{
	for (int i = 0; i < H; i++)
	{
		for (int j = 0; j < pillarWidth; j++)
		{
			pix[i][j][0] = i * 255 / H; // R
			pix[i][j][1] = 255 - i * 255 / H; // G
			pix[i][j][2] = j * 255 / pillarWidth; // B
		}
	}

	for (int i = 0; i < H; i++)
	{
		for (int j = pillarWidth; j < 2 * pillarWidth; j++)
		{
			pix[i][j][1] = i * 255 / H; // R
			pix[i][j][2] = 255 - i * 255 / H; // G
			pix[i][j][0] = j * 255 / pillarWidth; // B
		}
	}

	for (int i = 0; i < H; i++)
	{
		for (int j = 2 * pillarWidth; j < 3 * pillarWidth; j++)
		{
			pix[i][j][2] = i * 255 / H; // R
			pix[i][j][0] = 255 - i * 255 / H; // G
			pix[i][j][1] = j * 255 / pillarWidth; // B
		}
	}
}

MY_COLOR getColor(POINT_2D node)
{
	int r = pix[node.y][node.x][0];
	int g = pix[node.y][node.x][1];
	int b = pix[node.y][node.x][2];
	MY_COLOR color = { r,g,b };
	return color;
}

void replaceColor(POINT_2D node, MY_COLOR color)
{
	pix[node.y][node.x][0] = color.r;
	pix[node.y][node.x][1] = color.g;
	pix[node.y][node.x][2] = color.b;
}

void floodFill(POINT_2D node, MY_COLOR targetColor, MY_COLOR replacementColor)
{
	vector <POINT_2D> stack;
	POINT_2D point;
	if (isColorEquals(targetColor, replacementColor))
		return;
	else if (!isColorEquals(getColor(node), targetColor))
		return;
	replaceColor(node, replacementColor);
	stack.push_back(node);
	while (!stack.empty())
	{
		point = stack.back();
		stack.pop_back();
		POINT_2D westPoint = { point.x + 1,point.y };
		POINT_2D eastPoint = { point.x - 1,point.y };
		POINT_2D northPoint = { point.x,point.y + 1 };
		POINT_2D southPoint = { point.x,point.y - 1 };
		if (isColorEquals(getColor(westPoint), targetColor))
		{
			replaceColor(westPoint, replacementColor);
			stack.push_back(westPoint);
		}
		if (isColorEquals(getColor(eastPoint), targetColor))
		{
			replaceColor(eastPoint, replacementColor);
			stack.push_back(eastPoint);
		}
		if (isColorEquals(getColor(northPoint), targetColor))
		{
			replaceColor(northPoint, replacementColor);
			stack.push_back(northPoint);
		}
		if (isColorEquals(getColor(southPoint), targetColor))
		{
			replaceColor(southPoint, replacementColor);
			stack.push_back(southPoint);
		}
	}
}

void drawLine(POINT_2D p1, POINT_2D p2, MY_COLOR color) {
	int i, j, start, stop;
	double a, b;

	if (p1.x != p2.x)
	{
		a = ((double)p2.y - p1.y) / (p2.x - p1.x);
		if (fabs(a) < 1)
		{
			b = p1.y - a * p1.x;

			start = p1.x <= p2.x ? p1.x : p2.x;
			stop = p1.x > p2.x ? p1.x : p2.x;

			for (j = start; j <= stop; j++)
			{
				i = a * j + b;// y = ax+b
				pix[i][j][0] = color.r;
				pix[i][j][1] = color.g;
				pix[i][j][2] = color.b;
			}
		}
		else // fabs(slope) is > 1
		{
			a = 1 / a;
			b = p1.x - a * p1.y;

			start = p1.y <= p2.y ? p1.y : p2.y;
			stop = p1.y > p2.y ? p1.y : p2.y;

			for (i = start; i <= stop; i++)
			{
				j = a * i + b;
				pix[i][j][0] = color.r;
				pix[i][j][1] = color.g;
				pix[i][j][2] = color.b;
			}
		}
	}
	else
	{
		start = p1.y <= p2.y ? p1.y : p2.y;
		stop = p1.y > p2.y ? p1.y : p2.y;
		for (i = start; i < stop; i++)
		{
			pix[(int)p1.x][i][0] = color.r;
			pix[(int)p1.x][i][1] = color.g;
			pix[(int)p1.x][i][2] = color.b;
		}
	}
}


void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer

	glDrawPixels(W, H, GL_RGB, GL_UNSIGNED_BYTE, pix);

	glutSwapBuffers(); // show all
}

void idle()
{
	glutPostRedisplay(); // go to display
}

void mouse(int button, int state, int x, int y)
{
	POINT_2D pointClicked = { x, H - y };
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (x < pillarWidth * 3)
		{
			lineColor = getColor(pointClicked);
			return;
		}
		else if (!isClicked)
		{
			firstClick = pointClicked;
		}
		else
		{
			POINT_2D secondClick = pointClicked;
			drawLine(firstClick, secondClick, lineColor);
		}
		isClicked = !isClicked;
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		if (x < pillarWidth * 3)
		{
			fillColor = getColor(pointClicked);
		}
		else
		{
			floodFill(pointClicked, tColor, fillColor);
		}
	}

}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(W, H);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("Paint");

	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutMouseFunc(mouse);

	init();

	glutMainLoop();

	return 0;
}
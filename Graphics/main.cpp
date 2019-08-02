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
bool colorEquals(MY_COLOR color1, MY_COLOR color2);
void replaceColor(POINT_2D node, MY_COLOR color);
MY_COLOR getColor(POINT_2D node);

unsigned char pix[H][W][3]; // red,green and blue layers
double offset = 0;
POINT_2D firstClick;

bool isClicked = false;
MY_COLOR backgroundColor = { 255,255,255 };
MY_COLOR lineColor = { 0,0,0 };
MY_COLOR tColor = { 255,255,255 };
MY_COLOR rColor = { 255,0,0 };
MY_COLOR selectedColor = { 255,0,0 };
int pillarWidth = 10;
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

	for (int i = 0; i < H; i+= pillarHeight)
	{

	}
}

bool colorEquals(MY_COLOR color1, MY_COLOR color2)
{
	return color1.r == color2.r && color1.g == color2.g && color1.b == color2.b;
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
	if (colorEquals(targetColor, replacementColor))
		return;
	else if (!colorEquals(getColor(node), targetColor))
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
		if (colorEquals(getColor(westPoint), targetColor))
		{
			replaceColor(westPoint, replacementColor);
			stack.push_back(westPoint);
		}
		if (colorEquals(getColor(eastPoint), targetColor))
		{
			replaceColor(eastPoint, replacementColor);
			stack.push_back(eastPoint);
		}
		if (colorEquals(getColor(northPoint), targetColor))
		{
			replaceColor(northPoint, replacementColor);
			stack.push_back(northPoint);
		}
		if (colorEquals(getColor(southPoint), targetColor))
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

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (!isClicked)
		{
			firstClick = { x, H - y };
		}
		else
		{
			POINT_2D secondClick = { x, H - y };
			drawLine(firstClick, secondClick, lineColor);
		}
		isClicked = !isClicked;
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		POINT_2D point = { x, H - y };
		floodFill(point, tColor, rColor);
	}

}



//{
//	if (node.x >= W || node.y >= H || node.x < 0 || node.y < 0)
//		return;
//	node.color = { pix[node.y][node.x][0], pix[node.x][node.y][1], pix[node.y][node.x][2] };
//	if (targetColor.r == replacementColor.r && targetColor.g == replacementColor.g && targetColor.b == replacementColor.b)
//		return;
//	else if (node.color.r != targetColor.r || node.color.g != targetColor.g || node.color.b != targetColor.b)
//		return;
//	else
//	{
//		pix[node.y][node.x][0] = replacementColor.r;
//		pix[node.y][node.x][1] = replacementColor.g;
//		pix[node.y][node.x][2] = replacementColor.b;
//		node.color = replacementColor;
//		POINT_2D node_west = { node.x - 1, node.y };
//		POINT_2D node_north = { node.x, node.y + 1 };
//		POINT_2D node_east = { node.x + 1, node.y };
//		POINT_2D node_south = { node.x, node.y - 1 };
//		floodFill(node_west, targetColor, replacementColor);
//		floodFill(node_north, targetColor, replacementColor);
//		floodFill(node_east, targetColor, replacementColor);
//		floodFill(node_south, targetColor, replacementColor);
//	}
//	return;
//}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(W, H);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("Paint-Afeka");

	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutMouseFunc(mouse);

	init();

	glutMainLoop();

	return 0;
}
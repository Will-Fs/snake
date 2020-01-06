#include<stdio.h>
#include <stdbool.h>
#include<GL/glut.h> 
#include<math.h> 

int size = 10;
int head_pos[2] = {250, 100};
int head_vel[2] = {1, 0};
int amount_of_nodes;
int** body_nodes;
int length = 20;
int food[2] = {60, 60};
bool new_food = true;
bool go_override = false;
int tps = 20;

int round_to_10(int n)
{
    int tmp = n;
    int i = 0;
    while ((tmp /= size) >= size) {
        i++;
    }

    if (n % (int)(pow(size, i + 1) + 0.5)) {
        tmp++;
    }

    for (; i >= 0; i--) {
        tmp *= size;
    }

    return tmp;
}

void init (void) 
{ 
	body_nodes = (int **)malloc((length+1) * sizeof(int *));
    for (int i=0; i < length+1; i++)
    {
         body_nodes[i] = (int *)malloc(2 * sizeof(int));
    }
   	amount_of_nodes = length;
	body_nodes[0][0] = head_pos[0];
	body_nodes[0][1] = head_pos[1];
	for (int i = 1; i < amount_of_nodes + 1; i++)
	{
		body_nodes[i][0] = body_nodes[i - 1][0] - size;
		body_nodes[i][1] = body_nodes[i - 1][1];
	}

	glClearColor(0.0, 0.0, 0.0, 1.0); 
	
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity(); 
	
	gluOrtho2D(0, 500, 500, 0); 
} 

void spawn_food() 
{
	food[0] =  round_to_10(rand() % (500));
	food[1] =  round_to_10(rand() % (500));
	for (int i = 0; i < length; i++)
	{
		if (food[0] == body_nodes[i][0] && food[1] == body_nodes[i][1])
		{
			spawn_food();
			return;
		}	
	}
	if (food[0] == 500)
	{
		food[0] -= size;
	}
	if (food[1] == 500)
	{
		food[1] -= size;
	}
	if (food[0] == 0)
	{
		food[0] += size;
	}
	if (food[1] == 0)
	{
		food[1] += size;
	}
}

void update_snake()
{
	body_nodes[0][0] = head_pos[0];
	body_nodes[0][1] = head_pos[1];

	amount_of_nodes = length;

	head_pos[0] += head_vel[0] * size;
	head_pos[1] += head_vel[1] * size;

	if (head_pos[0] > 500 - size)
	{
		head_pos[0] = size;
	}
	if (head_pos[0] < size)
	{
		head_pos[0] = 500 - size;
	}
	if (head_pos[1] > 500 - size)
	{
		head_pos[1] = size;
	}
	if (head_pos[1] < size)
	{
		head_pos[1] = 500 - size;
	}

	bool go = true;

	for (int i = 0; i < length; i++)
	{
		if (head_pos[0] == body_nodes[i][0] && head_pos[1] == body_nodes[i][1])
		{
			head_pos[0] -= head_vel[0] * size;
			head_pos[1] -= head_vel[1] * size;
			go = false;
		}
	}
	if(go == false && go_override)
	{
		;
	}
	else {
		go_override = false;
	}

	if (head_pos[0] > food[0] - size/2 && head_pos[0] < food[0] + size/2 && head_pos[1] > food[1] - size/2 && head_pos[1] < food[1] + size/2)
	{
		length += 5;

		body_nodes = (int **)realloc(body_nodes, (length+1) * sizeof(int *));
		for (int i = length; i > length - 5; i--)
		{
		    body_nodes[i] = (int *)malloc(2 * sizeof(int));
		}
		new_food = true;
	}

	if (go && !go_override || go_override)
	{
		for (int i = amount_of_nodes; i > 0; i--)
		{
			body_nodes[i][0] = body_nodes[i - 1][0];
			body_nodes[i][1] = body_nodes[i - 1][1];
		}
	}

}

void draw_head()
{
	glColor3f(1.0, 0.0, 0.0); 

	glVertex2i(head_pos[0], head_pos[1]);
}

void draw_body()
{

	double col = 0.0;

	for (int i = 0; i < amount_of_nodes; i++)
	{
		glVertex2i(body_nodes[i][0], body_nodes[i][1]);
		if (col == 0)
		{
			glColor3f(1.0, 0.0, 0.2);
		}
		else if (col == 1)
		{
			glColor3f(1.0, 0.0, 0.4);
		}
		else if (col == 2)
		{
			glColor3f(1.0, 0.0, 0.6);
		}
		else if(col == 3)
		{
			glColor3f(1.0, 0.0, 0.8);
		}
		else if(col == 4)
		{
			glColor3f(1.0, 0.0, 1.0);
		}	
		else if(col == 5)
		{
			glColor3f(1.0, 0.0, 1.0);
		}
		else if (col == 6)
		{
			glColor3f(1.0, 0.0, 1.0);
		}
		else if (col == 7)
		{
			glColor3f(1.0, 0.0, 0.8);
		}
		else if(col == 8)
		{
			glColor3f(1.0, 0.0, 0.6);
		}
		else if(col == 9)
		{
			glColor3f(1.0, 0.0, 0.4);
		}	
		else if(col == 10)
		{
			glColor3f(1.0, 0.0, 0.2);

			col = -0.5;
		}
		col += 0.5;
	}
}

void draw_food()
{
	glColor3f(0.0, 1.0, 0.0);

	glVertex2i(food[0], food[1]);
	
}

void draw()
{
	glPointSize(size);
	glBegin(GL_POINTS);
	
	draw_body();
	draw_head();
	draw_food();

	glEnd(); 
	glFlush();
}

void display (void) 
{
	static int l;
	int n = glutGet(GLUT_ELAPSED_TIME);
	int t = (int) floor(n * tps / 1000);
	if (!l)
	{
		l = t - 1;
	}
	for ( ; l < t; l++)
	{
		update_snake();
	}
		draw();
	if (new_food)
	{
		spawn_food();
		new_food = false;
	}
	
	glClear(GL_COLOR_BUFFER_BIT); 
	glutPostRedisplay();
} 

void handle_keys(unsigned char key, int x, int y)
{
	if (key == 'a')
	{
		if (head_vel[0] == 1)
		{
			;
		}
		else 
		{
			head_vel[0] = -1;
			head_vel[1] = 0;
		}
	}
	else if (key == 'd')
	{
		if (head_vel[0] == -1)
		{
			;
		}
		else
		{
			head_vel[0] = 1;
			head_vel[1] = 0;
		}
	}
	else if (key == 'w')
	{
		if (head_vel[1] == 1)
		{
			;
		}
		else
		{
			head_vel[1] = -1;
			head_vel[0] = 0;
		}
	}
	else if (key == 's')
	{
		if (head_vel[1] == -1)
		{
			;
		}
		else
		{
			head_vel[1] = 1;
			head_vel[0] = 0;
		}
	}

	else if (key == 32)
	{
		go_override = true;
	}
}

void handle_mouse_pos (int x, int y) 
{
	
}


int main (int argc, char** argv) 
{
	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); 
	
	glutInitWindowSize(500, 500); 
	glutInitWindowPosition(0, 0); 
	
	glutCreateWindow("Snake"); 
	init();
	spawn_food();

	glutKeyboardFunc(handle_keys);
	glutPassiveMotionFunc(handle_mouse_pos);
	glutDisplayFunc(display);

	glutMainLoop();
} 

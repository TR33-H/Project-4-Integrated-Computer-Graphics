// Hayes, Demetrie


#include <stdlib.h>
#include <stdio.h>
#ifdef __APPLE_
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

//adding things for project 4
#define MAX_NUM_PARTICLES 1000
#define INITIAL_NUM_PARTICLES 25
#define INITIAL_POINT_SIZE 5.0
#define INITIAL_SPEED 1.0

typedef int bool;
#define TRUE 1
#define FALSE 0
//end


typedef float vertex[3];


static GLdouble viewer[] = { 0.0, 0.0, 5.0 };
static GLdouble extra[] = { 0.0, 1.0, 0.0 };
static GLdouble looking[] = { 0.0, 0.0, 0.0 };

//idk
static int eye = 1;
static int flat = 0;
int ww;

//static const unsigned int move_eye = 0;
//static const unsigned int twist_eye = 1;
//static const unsigned int zoom = 2;
//static const unsigned int move_none = 3;
enum { move_eye = 0, twist_eye = 1, zoom = 2, move_none = 3 };

static int action;

static double xStart = 0.0;
static double yStart = 0.0;
static float nearClip, farClip, distance, twistAngle, incAngle, azimAngle;
static int width, height;


//idk bittry
GLubyte wb[2] = { 0x00,0xff };
GLubyte check[512];


//particle try
void myinit();
void myIdle();
void myReshape(int, int);
void main_menu(int);
void collision(int);
float forces(int, int);


int num_particles; /* number of particles */

/* particle struct */

typedef struct particle
{
    int color;
    float position[3];
    float velocity[3];
    float mass;


} particle;

particle particles[MAX_NUM_PARTICLES]; /* particle system */

/* initial state of particle system */

int present_time;
int last_time;
int num_particles = INITIAL_NUM_PARTICLES;
float point_size = INITIAL_POINT_SIZE;
float speed = INITIAL_SPEED;
bool gravity = FALSE; /* gravity off */
bool elastic = FALSE; /* restitution off */
bool repulsion = FALSE; /* repulsion off */
float coef = 1.0; /* perfectly elastic collisions */
float d2[MAX_NUM_PARTICLES][MAX_NUM_PARTICLES]; /* array for interparticle distances */

GLsizei wh = 500, ww = 500; /* initial window size */

GLfloat colors[8][3] = { {0.0, 0.0, 0.0}, {1.0, 0.0, 0.0},{0.0, 1.0, 0.0},
    {0.0, 0.0, 1.0}, {0.0, 1.0, 1.0}, {1.0, 0.0, 1.0}, {1.0, 1.0, 0.0},
    {1.0, 1.0, 1.0} };

//end

/* initial tetrahedron */
vertex vertices[] = {
         {-0.8, -0.25, 0.25}, //0
         {-0.8, 0.25, 0.25},  //1
         {0.8, 0.25, 0.25},  //2
         {0.8, -0.25, 0.25}, //3

         {-0.8, -0.25, -0.25}, //4
         {-0.8, 0.25, -0.25},   //5
         {0.8, 0.25, -0.25}, //6
         {0.8, -0.25, -0.25},  //7

         //front 2
         {-0.8, -0.25, 0.25}, //8
         {-0.8, 0.5, 0.25}, //9
         {0.5, 0.5, 0.25}, //10
         {0.5, -0.25, 0.25}, //11

         //back 2
         {-0.8, -0.25, -0.25}, //12
         {-0.8, 0.5, -0.25}, //13
         {0.5, 0.5, -0.25}, //14
         {0.5, -0.25, -0.25}, //15

         //bus door
         {0.2, -0.24, 0.2501},//16
         {0.2, 0.45, 0.2501}, //17
         {0.4, 0.45, 0.2501}, //18
         {0.4, -0.24, 0.2501}, //19

         //window 1
         {-0.79, 0.25, 0.2501}, //20
         {-0.79, 0.4, 0.2501}, //21
         {0.19, 0.4, 0.2501}, //22
         {0.19, 0.25, 0.2501}, //23



         //cool lines
         {-0.8, 0.23, 0.252}, //24
         {-0.8, 0.18, 0.252}, //25
         {0.19, 0.18, 0.252}, //26
         {0.19, 0.23, 0.252}, //27

         {-0.8, 0.23, -0.252}, //28
         {-0.8, 0.18, -0.252}, //29
         {0.5, 0.18, -0.252}, //30
         {0.5, 0.23, -0.252}, //31

         //window 2
         {-0.79, 0.25, -0.2501}, //32
         {-0.79, 0.4, -0.2501}, //33
         {0.3, 0.4, -0.2501}, //34
         {0.3, 0.25, -0.2501}, //35

         //drivers window
         {0.33, 0.25, -0.252}, //36
         {0.33, 0.4, -0.252}, //37
         {0.49, 0.4, -0.252}, //38
         {0.49, 0.25, -0.252}, //39

         //cool lines 2.0
         {-0.8, 0.05, 0.252}, //40
         {-0.8, 0.0, 0.252}, //41
         {0.19, 0.0, 0.252}, //42
         {0.19, 0.05, 0.252}, //43

         {-0.8, 0.05, -0.252}, //44
         {-0.8, 0.0, -0.252}, //45
         {0.5, 0.0, -0.252}, //46
         {0.5, 0.05, -0.252}, //47

         //wheel
         {0.46, -0.34, 0.252},//48
         {0.46, -0.15, 0.252}, //49
         {0.65, -0.15, 0.252}, //50
         {0.65, -0.34, 0.252}, //51

         {0.5, -0.3, 0.2521},//52
         {0.5, -0.19, 0.2521}, //53
         {0.61, -0.19, 0.2521}, //54
         {0.61, -0.3, 0.2521}, //55

         {0.46, -0.34, -0.252},//56
         {0.46, -0.15, -0.252}, //57
         {0.65, -0.15, -0.252}, //58
         {0.65, -0.34, -0.252}, //59

         {0.5, -0.3, -0.2521},//60
         {0.5, -0.19, -0.2521}, //61
         {0.61, -0.19, -0.2521}, //62
         {0.61, -0.3, -0.2521}, //63

         //wheel 2 (rear)
         {-0.46, -0.34, 0.252},//64
         {-0.46, -0.15, 0.252}, //65
         {-0.65, -0.15, 0.252}, //66
         {-0.65, -0.34, 0.252}, //67

         {-0.5, -0.3, 0.2521},//68
         {-0.5, -0.19, 0.2521}, //69
         {-0.61, -0.19, 0.2521}, //70
         {-0.61, -0.3, 0.2521}, //71

         { -0.46, -0.34, -0.252 },//72
         { -0.46, -0.15, -0.252 }, //73
         { -0.65, -0.15, -0.252 }, //74
         { -0.65, -0.34, -0.252 }, //75

         { -0.5, -0.3, -0.2521 },//76
         { -0.5, -0.19, -0.2521 }, //77
         { -0.61, -0.19, -0.2521 }, //78
         { -0.61, -0.3, -0.2521 }, //79

        //top window
         {-0.6, 0.501, -0.23}, //80
         { -0.6, 0.501, 0.23 }, //81
         { -0.1, 0.501, 0.23 }, //82
        { -0.1, 0.501, -0.23 }, //83

        //grille
         { 0.81, 0.15, 0.1 }, //84
         { 0.81, -0.2, 0.1 }, //85
         { 0.81, -0.2, -0.1 }, //86
         { 0.81, 0.15, -0.1 }, //87

        //Front headlight 1
         { 0.811, 0.1, 0.25 }, //88
         { 0.811, -0.1, 0.25 }, //89
         { 0.811, -0.1, 0.1 }, //90
         { 0.811, 0.1, 0.1 }, //91

        //Front headlight 1 cool light maybe?
         { 0.812, 0.05, 0.2 }, //92
         { 0.812, -0.05, 0.2 }, //93
         { 0.812, -0.05, 0.15 }, //94
         { 0.812, 0.05, 0.15 }, //95

        //Front headlight 2
         { 0.811, 0.1, -0.25 }, //96
         { 0.811, -0.1, -0.25 }, //97
         { 0.811, -0.1, -0.1 }, //98
         { 0.811, 0.1, -0.1 }, //99

        //Front headlight 2 cool light maybe?
         { 0.812, 0.05, -0.2 }, //100
         { 0.812, -0.05, -0.2 }, //101
         { 0.812, -0.05, -0.15 }, //102
         { 0.812, 0.05, -0.15 }, //103

        //Door split
         { 0.29, -0.24, 0.2502 },//104
         { 0.29, 0.45, 0.2502 }, //105
         { 0.31, 0.45, 0.2502 }, //106
         { 0.31, -0.24, 0.2502 }, //107

        //Brake Light 1
         { -0.801, 0.1, 0.25 }, //88
         { -0.801, -0.1, 0.25 }, //89
         { -0.801, -0.1, 0.1 }, //90
         { -0.801, 0.1, 0.1 }, //91

        //Brake Light Red
         { -0.802, 0.09, 0.24 }, //88
         { -0.802, -0.09, 0.24 }, //89
         { -0.802, -0.09, 0.11 }, //90
         { -0.802, 0.09, 0.11 }, //91


        //Brake Light 2
         { -0.801, 0.1, -0.25 }, //88
         { -0.801, -0.1, -0.25 }, //89
         { -0.801, -0.1, -0.1 }, //90
         { -0.801, 0.1, -0.1 }, //91

        //Brake Light 2 Red
         { -0.802, 0.09, -0.24 }, //88
         { -0.802, -0.09, -0.24 }, //89
         { -0.802, -0.09, -0.11 }, //90
         { -0.802, 0.09, -0.11 }, //91

        //Back Window
         { -0.801, 0.4, -0.25 }, //88
         { -0.801, 0.2, -0.25 }, //89
         { -0.801, 0.2, 0.25 }, //90
         { -0.801, 0.4, 0.25 }, //91



};
static GLfloat theta[] = { 0.0,0.0,0.0 };


//Print
void printHelp()
{
    printf("Camera Controls:\n");
    printf("f - front\n");
    printf("b - back\n");
    printf("r - right side\n");
    printf("l - left side\n");
    printf("t - top\n");
    printf("u - under\n");
    printf("i - isometric\n");
    printf("I - Inside Door Towards Middle\n");
    printf("D - Middle Towards Door\n");
    printf("Light Controls:\n");
    printf("1 - Middle Right\n");
    printf("2 - Middle Left\n");
    printf("3 - Front Right\n");
    printf("4 - Front Left\n");
    printf("5 - Middle Middle\n");
    printf("6 - Front Middle\n");
    printf("General Controls:\n");
    printf("e - exit the program\n");
    printf("h - Help\n");
    printf("---------------------\n");
}



/* display one triangle using a line loop for wire frame, a single
normal for constant shading, or three normals for interpolative shading */
void face(int a, int b, int c, int d)
{
    glBegin(GL_POLYGON);
    glNormal3fv(vertices[a]);

    glVertex3fv(vertices[a]);
    glVertex3fv(vertices[b]);
    glVertex3fv(vertices[c]);
    glVertex3fv(vertices[d]);
    glEnd();
}

void MYcube()
{

    glColor3f(1.0, 1.0, 0.0);         //front
    face(0, 3, 2, 1);

    glColor3f(1.0, 1.0, 0.0);         //back
    face(4, 5, 6, 7);

    glColor3f(1.0, 1.0, 0.0);      //top
    face(1, 2, 6, 5);

    glColor3f(1.0, 1.0, 0.0);       //bottom 
    face(0, 3, 7, 4);

    glColor3f(1.0, 1.0, 0.0);     //right side
    face(2, 3, 7, 6);

    glColor3f(1.0, 1.0, 0.0);     //left side
    face(0, 1, 5, 4);

    //new
    glColor3f(1.0, 1.0, 0.0);         //front
    face(8, 11, 10, 9);

    glColor3f(1.0, 1.0, 0.0);         //back
    face(12, 13, 14, 15);

    glColor3f(1.0, 1.0, 0.0);      //top
    face(9, 10, 14, 13);

    glColor3f(1.0, 1.0, 0.0);       //bottom 
    face(8, 11, 15, 12);

    glColor3f(0.0, 0.0, 1.0);     // MAIN WINDOW
    face(10, 11, 15, 14);

    glColor3f(1.0, 1.0, 0.0);     // left side
    face(8, 9, 13, 12);

    //door
    glColor3f(0.3, 0.3, 0.3);
    face(16, 17, 18, 19);

    //window 1
    glColor3f(0.2, 0.2, 1.0);
    face(20, 21, 22, 23);

    //cool lines
    glColor3f(0.0, 0.0, 0.0);
    face(24, 25, 26, 27);

    glColor3f(0.0, 0.0, 0.0);
    face(28, 29, 30, 31);

    //window 2
    glColor3f(0.2, 0.2, 1.0);
    face(32, 33, 34, 35);

    //driver's window
    glColor3f(0.2, 0.2, 1.0);
    face(36, 37, 38, 39);

    //cool lines 2.0
    glColor3f(0.0, 0.0, 0.0);
    face(40, 41, 42, 43);

    glColor3f(0.0, 0.0, 0.0);
    face(44, 45, 46, 47);

    //wheel
    glColor3f(0.0, 0.0, 0.0);
    face(48, 49, 50, 51);

    glColor3f(0.4, 0.4, 0.4);
    face(52, 53, 54, 55);

    glColor3f(0.0, 0.0, 0.0);
    face(56, 57, 58, 59);
    glColor3f(0.4, 0.4, 0.4);
    face(60, 61, 62, 63);

    //wheel 2 (rear)
    glColor3f(0.0, 0.0, 0.0);
    face(64, 65, 66, 67);

    glColor3f(0.4, 0.4, 0.4);
    face(68, 69, 70, 71);

    glColor3f(0.0, 0.0, 0.0);
    face(72, 73, 74, 75);

    glColor3f(0.4, 0.4, 0.4);
    face(76, 77, 78, 79);

    //roof cool
    glColor3f(0.2, 0.2, 1.0);
    face(80, 81, 82, 83);

    //grille
    glColor3f(0.4, 0.4, 0.4);
    face(84, 85, 86, 87);

    //headlight 1
    glColor3f(0.0, 0.0, 0.0);
    face(88, 89, 90, 91);

    //headlight 1 light thing
    glColor3f(1.0, 1.0, 1.0);
    face(92, 93, 94, 95);

    //headlight 2
    glColor3f(0.0, 0.0, 0.0);
    face(96, 97, 98, 99);

    //headlight 2 light thing
    glColor3f(1.0, 1.0, 1.0);
    face(100, 101, 102, 103);

    //door split
    glColor3f(0.0, 0.0, 0.0);
    face(104, 105, 106, 107);

    //brake lights
    glColor3f(0.0, 0.0, 0.0);
    face(108, 109, 110, 111);

    glColor3f(1.0, 0.0, 0.0);
    face(112, 113, 114, 115);

    glColor3f(0.0, 0.0, 0.0);
    face(116, 117, 118, 119);

    glColor3f(1.0, 0.0, 0.0);
    face(120, 121, 122, 123);

    //Back window
    glColor3f(0.2, 0.2, 1.0);
    face(124, 125, 126, 127);


}

//Trying to steal stuff from HW

//Front Bottom Right
GLfloat light0_pos[4] = { -0.25, -0.25, 0.79, 0.0 };

//Front Middle
GLfloat light1_pos[4] = { 0.90, 0.90, -2.25, 0.0 };

//Front Bottom Left
GLfloat light2_pos[4] = { -0.25, -0.25, 0.79, 0.0 };

//trying another light
GLfloat light3_pos[4] = { 0.90, 0.90, -2.25, 0.0 };

//trying another light
GLfloat light4_pos[4] = { 0.90, 0.90, -2.25, 0.0 };

//trying another light
GLfloat light5_pos[4] = { 0.90, 0.90, -2.25, 0.0 };

typedef struct materialStruct {
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
    GLfloat shininess;
} materialStruct;

typedef struct lightingStruct {
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
} lightingStruct;


materialStruct colorCubeMaterials = {
    {0.2, 0.2, 0.2, 1.0},
    {0.8, 0.8, 0.8, 1.0},
    {0.0, 0.0, 0.0, 1.0},
    1.0
};

materialStruct brassMaterials = {
    {0.33, 0.22, 0.03, 1.0},
    {0.78, 0.57, 0.11, 1.0},
    {0.99, 0.91, 0.81, 1.0},
    27.8
};

materialStruct redPlasticMaterials = {
    {0.3, 0.0, 0.0, 1.0},
    {0.6, 0.0, 0.0, 1.0},
    {0.8, 0.6, 0.6, 1.0},
    32.0
};

materialStruct whiteShinyMaterials = {
    {1.0, 1.0, 1.0, 1.0},
    {1.0, 1.0, 1.0, 1.0},
    {1.0, 1.0, 1.0, 1.0},
    100.0
};

lightingStruct whiteLighting = {
    {0.0, 0.0, 0.0, 1.0},
    {1.0, 1.0, 1.0, 1.0},
    {1.0, 1.0, 1.0, 1.0}
};

lightingStruct coloredLighting = {
    {0.2, 0.0, 0.0, 1.0},
    {0.0, 1.0, 0.0, 1.0},
    {0.0, 0.0, 1.0, 1.0}
};


materialStruct* currentMaterials;
lightingStruct* currentLighting;



//IDK

void resetView()
{
    distance = (float)nearClip + (float)((farClip - nearClip) / 2.0);
    twistAngle = 0.0f;	/* rotation of viewing volume (camera) */
    incAngle = 0.0f;
    azimAngle = 0.0f;
}


void mouse(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN)
    {
        switch (button)
        {
        case GLUT_LEFT_BUTTON:
            if (eye == 0) action = twist_eye;
            if (eye == 1) action = move_eye;
            if (eye == 2) action = zoom;
            //idk
            //glBitmap(128, 128, 0.0, 0.0, 0.0, 0.0, check);
            //glRasterPos2i(x, ww - y);
            //glBitmap(128, 128, 0.0, 0.0, 0.0, 0.0, check);
            //glFlush();
            //idk
            break;
        case GLUT_RIGHT_BUTTON:
            action = zoom;
            break;
        default:
            break;
        }
        xStart = x;
        yStart = y;
    }
    else
    {
        action = move_none;
    }
}


void motion(int x, int y)
{
    switch (action)
    {
    case(move_eye):
        azimAngle += (float)(x - xStart);
        incAngle -= (float)(y - yStart);
        break;
    case(twist_eye):
        twistAngle = (float)(twistAngle + (int)(x - xStart) % 360);
        break;
    case(zoom):
        distance -= (float)(10.0 * (y - yStart) / width);
        break;
    default:
        break;
    }
    xStart = x;
    yStart = y;
    glutPostRedisplay();
}


void flythru1(float distance, float azimuth, float incidence, float twist)
{
    glTranslatef(0.0f, 0.0f, -distance);
    glRotatef(-twist, 0.0f, 0.0f, 1.0f);
    glRotatef(-incidence, 1.0f, 0.0f, 0.0f);
    glRotatef(-azimuth, 0.0f, 1.0f, 0.0f);
}



void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    flythru1(distance, azimAngle, incAngle, twistAngle);
    gluLookAt(viewer[0], viewer[1], viewer[2], looking[0], looking[1], looking[2], extra[0], extra[1], extra[2]);
    glColor3f(1.0, 0.0, 0.0);

    //glRasterPos2f(0.0, 0.0);
    //glBitmap(64, 64, 0.0, 0.0, 0.0, 0.0, check);

    //project 4
    int i;

    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS); /* render all particles */
    for (i = 0; i < num_particles; i++)
    {
        glColor3fv(colors[particles[i].color]);
        glVertex3fv(particles[i].position);
    }
    glEnd();
    glColor3f(0.0, 0.0, 0.0);
    glutWireCube(2.2);  /* outline of box */
    glutSwapBuffers();
    //end

    MYcube();
    glFlush();
}

/*
void myReshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho(-1.0, 1.0, -1.0 * (GLfloat)h / (GLfloat)w,
            1.0 * (GLfloat)h / (GLfloat)w, -5.0, 10.0);
    else
        glOrtho(-2.0 * (GLfloat)w / (GLfloat)h,
            2.0 * (GLfloat)w / (GLfloat)h, -2.0, 2.0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    //glutPostRedisplay();
}
*/


void myReshape(int width, int height)
{
    double aspect;

    glViewport(0, 0, width, height);

    /* compute aspect ratio */
    aspect = (double)width / (double)height;
    if (height > width) aspect = 1.0f;
    glMatrixMode(GL_PROJECTION);

    //idk
    ww = width;

    /* Reset world coordinates first ... */
    glLoadIdentity();

    /* Reset the viewing volume based on the new aspect ratio */
    gluPerspective(45.0f, aspect, nearClip, farClip);

    glutPostRedisplay();
    glMatrixMode(GL_MODELVIEW);
}


void initgfx()
{
    /* set clear color to black */
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    /* enable the depth buffer */
    glEnable(GL_DEPTH_TEST);

    nearClip = 0.1f;
    farClip = nearClip + 5.0f;


    resetView();
}


//Adding this in for Project 4
void myinit()
{
    int  i, j;

    /* set up particles with random locations and velocities */

    for (i = 0; i < num_particles; i++)
    {
        particles[i].mass = 1.0;
        particles[i].color = i % 8;
        for (j = 0; j < 3; j++)
        {
            particles[i].position[j] = 2.0 * ((float)rand() / RAND_MAX) - 1.0;
            particles[i].velocity[j] = speed * 2.0 * ((float)rand() / RAND_MAX) - 1.0;
        }
    }
    glPointSize(point_size);


    /* set clear color to grey */

    glClearColor(0.5, 0.5, 0.5, 1.0);
}
void myIdle()
{
    int i, j, k;
    float dt;
    present_time = glutGet(GLUT_ELAPSED_TIME);
    dt = 0.001 * (present_time - last_time);
    for (i = 0; i < num_particles; i++)
    {
        for (j = 0; j < 3; j++)
        {
            particles[i].position[j] += dt * particles[i].velocity[j];
            particles[i].velocity[j] += dt * forces(i, j) / particles[i].mass;
        }
        collision(i);
    }
    if (repulsion) for (i = 0; i < num_particles; i++) for (k = 0; k < i; k++)
    {
        d2[i][k] = 0.0;
        for (j = 0; j < 3; j++) d2[i][k] += (particles[i].position[j] -
            particles[k].position[j]) * (particles[i].position[j] -
                particles[k].position[j]);
        d2[k][i] = d2[i][k];
    }
    last_time = present_time;
    glutPostRedisplay();
}
float forces(int i, int j)
{
    int k;
    float force = 0.0;
    if (gravity && j == 1) force = -1.0; /* simple gravity */
    if (repulsion) for (k = 0; k < num_particles; k++)  /* repulsive force */
    {
        if (k != i) force += 0.001 * (particles[i].position[j] - particles[k].position[j]) / (0.001 + d2[i][k]);
    }
    return(force);
}

void collision(int n)

/* tests for collisions against cube and reflect particles if necessary */

{
    int i;
    for (i = 0; i < 3; i++)
    {
        if (particles[n].position[i] >= 1.0)
        {
            particles[n].velocity[i] = -coef * particles[n].velocity[i];
            particles[n].position[i] = 1.0 - coef * (particles[n].position[i] - 1.0);
        }
        if (particles[n].position[i] <= -1.0)
        {
            particles[n].velocity[i] = -coef * particles[n].velocity[i];
            particles[n].position[i] = -1.0 - coef * (particles[n].position[i] + 1.0);
        }
    }
}
//end



void keys(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'f':
        viewer[0] = 1;
        viewer[1] = 0;
        viewer[2] = 0;
        looking[0] = 0;
        looking[0] = 0;
        looking[0] = 0;
        extra[0] = 0;
        extra[1] = 1;
        extra[2] = 0;
        farClip = nearClip + 5.0f;
        resetView();
        break;
    case 'b':
        viewer[0] = -1;
        viewer[1] = 0;
        viewer[2] = 0;
        looking[0] = 0;
        looking[0] = 0;
        looking[0] = 0;
        extra[0] = 0;
        extra[1] = 1;
        extra[2] = 0;
        farClip = nearClip + 5.0f;
        resetView();
        break;
    case 'r':
        viewer[0] = 0;
        viewer[1] = 0;
        viewer[2] = 1;
        looking[0] = 0;
        looking[0] = 0;
        looking[0] = 0;
        extra[0] = 0;
        extra[1] = 1;
        extra[2] = 0;
        farClip = nearClip + 5.0f;
        resetView();
        break;
    case 'l':
        viewer[0] = 0;
        viewer[1] = 0;
        viewer[2] = -1;
        looking[0] = 0;
        looking[0] = 0;
        looking[0] = 0;
        extra[0] = 0;
        extra[1] = 1;
        extra[2] = 0;
        farClip = nearClip + 5.0f;
        resetView();
        break;
    case 't':
        viewer[0] = 0;
        viewer[1] = 1;
        viewer[2] = 0;
        looking[0] = 0;
        looking[0] = 0;
        looking[0] = 0;
        extra[0] = 0;
        extra[1] = 0;
        extra[2] = 1;
        farClip = nearClip + 5.0f;
        resetView();
        break;
    case 'u':
        viewer[0] = 0;
        viewer[1] = -1;
        viewer[2] = 0;
        looking[0] = 0;
        looking[0] = 0;
        looking[0] = 0;
        extra[0] = 0;
        extra[1] = 0;
        extra[2] = 1;
        farClip = nearClip + 5.0f;
        resetView();
        break;
    case 'i':
        viewer[0] = 1;
        viewer[1] = 1;
        viewer[2] = 1;
        looking[0] = 0;
        looking[0] = 0;
        looking[0] = 0;
        extra[0] = 0;
        extra[1] = 1;
        extra[2] = 0;
        farClip = nearClip + 5.0f;
        resetView();
        break;
    case 'I':
        viewer[0] = 0.3;
        viewer[1] = 0;
        viewer[2] = 0.15;
        looking[0] = 0;
        looking[0] = 0;
        looking[0] = 0;
        extra[0] = 0;
        extra[1] = 1;
        extra[2] = 0;
        farClip = nearClip + 0.0f;

        resetView();
        break;
    case 'D':
        viewer[0] = -0.7;
        viewer[1] = -0.1;
        viewer[2] = -0.1;
        looking[0] = 0.5;
        looking[0] = 0;
        looking[0] = 0.5;
        extra[0] = 0;
        extra[1] = 1;
        extra[2] = 0;
        farClip = nearClip + 0.0f;
        resetView();
        break;
    case 'e':
        exit(0);
        break;
    case 'h':
        printHelp();
        break;
    case 'R':
        resetView();
        break;
    case '0':
        glDisable(GL_LIGHT6);
        glDisable(GL_LIGHT5);
        glDisable(GL_LIGHT4);
        glDisable(GL_LIGHT3);
        glDisable(GL_LIGHT2);
        glDisable(GL_LIGHT1);
        glEnable(GL_LIGHT0);

        currentMaterials = &colorCubeMaterials;
        currentLighting = &whiteLighting;
        glLightfv(GL_LIGHT0, GL_AMBIENT, currentLighting->ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, currentLighting->diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, currentLighting->specular);
        break;
    case '1':

        //Middle RIGHT

        currentMaterials = &colorCubeMaterials;
        GLfloat light1_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
        GLfloat light1_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat light1_specular[] = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat light1_position[] = { 0.0, 0.0, 0.25, 1.0 };
        GLfloat spot_direction[] = { 1.0, 1.0, 0.0 };

        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHT2);
        glDisable(GL_LIGHT3);
        glDisable(GL_LIGHT4);
        glDisable(GL_LIGHT5);
        glDisable(GL_LIGHT6);
        //glEnable(GL_LIGHTING);
        //glEnable(GL_LIGHT1);

        glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
        glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
        glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
        glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.5);
        glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.5);
        glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.2);

        glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0);
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
        glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);

        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT1);
        break;
    case '2':

        //MIDDLE RIGHT


        currentMaterials = &colorCubeMaterials;
        GLfloat light2_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
        GLfloat light2_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat light2_specular[] = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat light2_position[] = { 0.0, 0.0, -0.25, 1.0 };
        GLfloat spot_direction2[] = { 1.0, 1.0, 0.0 };

        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHT1);
        glDisable(GL_LIGHT2);
        glDisable(GL_LIGHT3);
        glDisable(GL_LIGHT4);
        glDisable(GL_LIGHT5);
        glDisable(GL_LIGHT6);
        //glEnable(GL_LIGHTING);
        //glEnable(GL_LIGHT2);

        glLightfv(GL_LIGHT2, GL_AMBIENT, light2_ambient);
        glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
        glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);
        glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
        glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1.5);
        glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.5);
        glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.2);

        glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 45.0);
        glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction2);
        glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 2.0);

        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT2);
        break;
    case '3':

        //FRONT BOTTOM RIGHT


        currentMaterials = &colorCubeMaterials;
        GLfloat light3_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat light3_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat light3_specular[] = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat light3_position[] = { 0.5, -0.25, 0.25, 1.0 };
        GLfloat spot_direction3[] = { 0.0, 0.0, 0.0 };

        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHT1);
        glDisable(GL_LIGHT2);
        glDisable(GL_LIGHT3);
        glDisable(GL_LIGHT4);
        glDisable(GL_LIGHT5);
        glDisable(GL_LIGHT6);
        //glEnable(GL_LIGHTING);
        //glEnable(GL_LIGHT2);

        glLightfv(GL_LIGHT3, GL_AMBIENT, light3_ambient);
        glLightfv(GL_LIGHT3, GL_DIFFUSE, light3_diffuse);
        glLightfv(GL_LIGHT3, GL_SPECULAR, light3_specular);
        glLightfv(GL_LIGHT3, GL_POSITION, light3_position);
        glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, 1.5);
        glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, 0.5);
        glLightf(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, 0.2);

        glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 45.0);
        glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction3);
        glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 2.0);

        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT3);
        break;
    case '4':

        //FRONT BOTTOM LEFT


        currentMaterials = &colorCubeMaterials;
        GLfloat light4_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat light4_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat light4_specular[] = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat light4_position[] = { 0.5, -0.25, -0.25, 1.0 };
        GLfloat spot_direction4[] = { 0.0, 0.0, 0.0 };

        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHT1);
        glDisable(GL_LIGHT2);
        glDisable(GL_LIGHT3);
        glDisable(GL_LIGHT5);
        glDisable(GL_LIGHT6);
        //glEnable(GL_LIGHTING);
        //glEnable(GL_LIGHT2);

        glLightfv(GL_LIGHT4, GL_AMBIENT, light4_ambient);
        glLightfv(GL_LIGHT4, GL_DIFFUSE, light4_diffuse);
        glLightfv(GL_LIGHT4, GL_SPECULAR, light4_specular);
        glLightfv(GL_LIGHT4, GL_POSITION, light4_position);
        glLightf(GL_LIGHT4, GL_CONSTANT_ATTENUATION, 1.5);
        glLightf(GL_LIGHT4, GL_LINEAR_ATTENUATION, 0.5);
        glLightf(GL_LIGHT4, GL_QUADRATIC_ATTENUATION, 0.2);

        glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 45.0);
        glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, spot_direction4);
        glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, 2.0);

        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT4);
        break;

    case '5':

        //MIDDLE MIDDLE


        currentMaterials = &colorCubeMaterials;
        GLfloat light5_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
        GLfloat light5_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat light5_specular[] = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat light5_position[] = { 0.0, 0.0, 0.0, 1.0 };
        GLfloat spot_direction5[] = { 1.0, 1.0, 0.0 };

        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHT1);
        glDisable(GL_LIGHT2);
        glDisable(GL_LIGHT3);
        glDisable(GL_LIGHT4);
        glDisable(GL_LIGHT5);
        glDisable(GL_LIGHT6);
        //glEnable(GL_LIGHTING);
        //glEnable(GL_LIGHT2);

        glLightfv(GL_LIGHT5, GL_AMBIENT, light5_ambient);
        glLightfv(GL_LIGHT5, GL_DIFFUSE, light5_diffuse);
        glLightfv(GL_LIGHT5, GL_SPECULAR, light5_specular);
        glLightfv(GL_LIGHT5, GL_POSITION, light5_position);
        glLightf(GL_LIGHT5, GL_CONSTANT_ATTENUATION, 1.5);
        glLightf(GL_LIGHT5, GL_LINEAR_ATTENUATION, 0.5);
        glLightf(GL_LIGHT5, GL_QUADRATIC_ATTENUATION, 0.2);

        glLightf(GL_LIGHT5, GL_SPOT_CUTOFF, 45.0);
        glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, spot_direction5);
        glLightf(GL_LIGHT5, GL_SPOT_EXPONENT, 2.0);

        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT5);
        break;

    case '6':

        //MIDDLE MIDDLE


        currentMaterials = &colorCubeMaterials;
        GLfloat light6_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
        GLfloat light6_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat light6_specular[] = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat light6_position[] = { 0.5, 0.0, 0.0, 1.0 };
        GLfloat spot_direction6[] = { 1.0, 1.0, 0.0 };

        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHT1);
        glDisable(GL_LIGHT2);
        glDisable(GL_LIGHT3);
        glDisable(GL_LIGHT4);
        glDisable(GL_LIGHT5);
        //glEnable(GL_LIGHTING);
        //glEnable(GL_LIGHT2);

        glLightfv(GL_LIGHT6, GL_AMBIENT, light6_ambient);
        glLightfv(GL_LIGHT6, GL_DIFFUSE, light6_diffuse);
        glLightfv(GL_LIGHT6, GL_SPECULAR, light6_specular);
        glLightfv(GL_LIGHT6, GL_POSITION, light6_position);
        glLightf(GL_LIGHT6, GL_CONSTANT_ATTENUATION, 1.5);
        glLightf(GL_LIGHT6, GL_LINEAR_ATTENUATION, 0.5);
        glLightf(GL_LIGHT6, GL_QUADRATIC_ATTENUATION, 0.2);

        glLightf(GL_LIGHT6, GL_SPOT_CUTOFF, 45.0);
        glLightfv(GL_LIGHT6, GL_SPOT_DIRECTION, spot_direction6);
        glLightf(GL_LIGHT6, GL_SPOT_EXPONENT, 2.0);

        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT6);
        break;
    }
  

    //HW stuff try
    glMaterialfv(GL_FRONT, GL_AMBIENT, currentMaterials->ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, currentMaterials->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, currentMaterials->specular);
    glMaterialf(GL_FRONT, GL_SHININESS, currentMaterials->shininess);

    //glLightfv(GL_LIGHT0, GL_AMBIENT, currentLighting->ambient);
    //glLightfv(GL_LIGHT0, GL_DIFFUSE, currentLighting->diffuse);
    //glLightfv(GL_LIGHT0, GL_SPECULAR, currentLighting->specular);


    glutPostRedisplay();
    //end of HW try

    glutPostRedisplay();
}

//init from shadedcube
void init()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    currentMaterials = &redPlasticMaterials;

    glMaterialfv(GL_FRONT, GL_AMBIENT, currentMaterials->ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, currentMaterials->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, currentMaterials->specular);
    glMaterialf(GL_FRONT, GL_SHININESS, currentMaterials->shininess);

    currentLighting = &whiteLighting;

    glLightfv(GL_LIGHT0, GL_AMBIENT, currentLighting->ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, currentLighting->diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, currentLighting->specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);


}




void  main(int argc, char** argv)
{
    //idk
    int i, j;
    for (i = 0; i < 64; i++) for (j = 0; j < 8; j++) check[i * 8 + j] = wb[(i / 8 + j) % 2];
    //idk
    glutInit(&argc, argv);
    width = glutGet(GLUT_SCREEN_WIDTH);
    height = glutGet(GLUT_SCREEN_HEIGHT);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Hayes Demetrie BUS Version 3");
    //init
    myinit();
    init();
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    //alright
    glutIdleFunc(myIdle);
    glutReshapeFunc(myReshape);
    glutKeyboardFunc(keys);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutDisplayFunc(display);
    printHelp();
    glEnable(GL_DEPTH_TEST);
    //idk
    //glEnable(GL_COLOR_LOGIC_OP);
    glLogicOp(GL_XOR);
    //idk
    glClearColor(1.0, 1.0, 1.0, 1.0);
    initgfx();
    glutMainLoop();
}
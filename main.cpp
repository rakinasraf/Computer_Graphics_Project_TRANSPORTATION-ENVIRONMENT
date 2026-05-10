#include <windows.h>
#include <GL/glut.h>
#include <cmath>
using namespace std;

bool rainOn = false;
float rainX[300];
float rainY[300];
float rainSpeed = 0.03f;

bool snowOn = false;
float snowX[300];
float snowY[300];
float snowSpeed = 0.012f;

bool isPaused = false;
float globalSpeed = 1.0;

//circle
void drawCircle(float x, float y, float r)
{
    glBegin(GL_POLYGON);
    for(int i = 0; i < 100; i++)
    {
        float angle = 2 * 3.1416f * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void drawRain()
{
    if(!rainOn) return;

    glColor3f(0.6, 0.6, 1.0);
    glLineWidth(2);
    glBegin(GL_LINES);

    for(int i = 0; i < 300; i++)
    {
        glVertex2f(rainX[i], rainY[i]);
        glVertex2f(rainX[i] + 0.008f, rainY[i] - 0.05f);
    }

    glEnd();
}
void drawSnow()
{
    if(!snowOn) return;

    glColor3f(1, 1, 1);
    glPointSize(2);
    glBegin(GL_POINTS);

    for(int i = 0; i < 300; i++)
    {
        glVertex2f(snowX[i], snowY[i]);
    }

    glEnd();
}

//airport code........................................................................
float planeX = -1.0f;
float planeY = -0.4f;
float runwayX = 0.0f;
float cloudX11 = -1.2f;
float cloudX22= -0.4f;
float cloudX33= 0.6f;
float cloudX44= 0.8f;
float carX = 0.65f;
bool isNightAirport= false;
bool runwayLightOn = true;
bool planeNavLightOn = true;
bool tailLightOn = true;
bool starsVisibleAirport = true;
bool floodlightsOn = true;

//SUN/MOON/STARS
void drawSunMoonStars()
{
    if(isNightAirport)
    {
        // Moon
        glColor3f(1,1,1);
        drawCircle(0.75f, 0.6f, 0.1f);

        // Stars(twinkling)
        if(starsVisibleAirport)
        {
            glPointSize(2);
            glBegin(GL_POINTS);
                glVertex2f(-0.8,0.9);
                glVertex2f(-0.5,0.85);
                glVertex2f(-0.2,0.92);
                glVertex2f(0.2,0.88);
                glVertex2f(0.5,0.93);
                glVertex2f(0.8,0.9);
            glEnd();
        }
    }
    else
    {
        // Sun
        glColor3f(1,1,0);
        drawCircle(0.75f, 0.6f, 0.1f);
    }
}

void drawFloodlight(float x)
{
    // Pole
    glColor3f(0.4, 0.4, 0.4);
    glBegin(GL_QUADS);
        glVertex2f(x, -0.4);
        glVertex2f(x + 0.02, -0.4);
        glVertex2f(x + 0.02, 0.1);
        glVertex2f(x, 0.1);
    glEnd();
    if(isNightAirport && floodlightsOn)
    {
        // Light cone (transparent effect)
        glColor4f(1.0, 1.0, 0.8, 0.3);
        glBegin(GL_TRIANGLES);
            glVertex2f(x + 0.01, 0.1);
            glVertex2f(x - 0.2, -0.6);
            glVertex2f(x + 0.22, -0.6);
        glEnd();

        // Bright light point
        glColor3f(1, 1, 0.9);
        glPointSize(8);
        glBegin(GL_POINTS);
            glVertex2f(x + 0.01, 0.1);
        glEnd();
    }
}

void drawAllFloodlights()
{
    drawFloodlight(-0.95);
    drawFloodlight(0.2);
    drawFloodlight(0.49);
    drawFloodlight(0.76);
}

//CLOUD
void drawCloudAirport(float x, float y)
{
    glColor3f(0.9,0.9,0.9);
    drawCircle(x, y, 0.05);
    drawCircle(x+0.06, y, 0.06);
    drawCircle(x+0.12, y, 0.05);
}

//RUNWAY
void drawRunway()
{
    glColor3f(0,0,0);
    glBegin(GL_QUADS);
        glVertex2f(-1,-0.8);
        glVertex2f( 1,-0.8);
        glVertex2f( 1,-0.4);
        glVertex2f(-1,-0.4);
    glEnd();

    // white strips
    glColor3f(1,1,1);
    for(float x=-1;x<1;x+=0.4)
    {
        glBegin(GL_QUADS);
            glVertex2f(x+runwayX,-0.62);
            glVertex2f(x+0.2+runwayX,-0.62);
            glVertex2f(x+0.2+runwayX,-0.57);
            glVertex2f(x+runwayX,-0.57);
        glEnd();
    }

    // runway lights
    if(isNightAirport && runwayLightOn)
    {
        glColor3f(1,0,0);
        glPointSize(6);
        glBegin(GL_POINTS);
        for(float x=-0.95;x<1;x+=0.2)
            glVertex2f(x,-0.4);
        glEnd();

        glColor3f(1,0,0);
        glPointSize(6);
        glBegin(GL_POINTS);
        for(float x=-0.95;x<1;x+=0.2)
            glVertex2f(x,-0.8);
        glEnd();
    }
}

//AIRPORT
void drawWindow(float x, float y)
{
    if(isNightAirport) {glColor3f(1, 1, 0.3);}   // Night color
    else {glColor3f(0.2, 0.4, 0.8);}  // Day color

    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + 0.05, y);
        glVertex2f(x + 0.05, y + 0.07);
        glVertex2f(x, y + 0.07);
    glEnd();
}

void drawAirport()
{
    // Terminal 1
    glColor3f(0.6, 0.6, 0.6);
    glBegin(GL_QUADS);
        glVertex2f(-0.9, -0.4);
        glVertex2f(-0.05, -0.4);
        glVertex2f(-0.05, 0.0);
        glVertex2f(-0.9, 0.0);
    glEnd();

    glColor3f(0.3,0.3,0.3);
    glBegin(GL_QUADS);
        glVertex2f(-0.91f, 0.0f);
        glVertex2f(-0.04f, 0.0f);
        glVertex2f(-0.04f, 0.04f);
        glVertex2f(-0.91f, 0.04f);
    glEnd();


    drawWindow(-0.8, -0.25);
    drawWindow(-0.72, -0.25);
    drawWindow(-0.64, -0.25);

    drawWindow(-0.80, -0.15);
    drawWindow(-0.72, -0.15);
    drawWindow(-0.64, -0.15);

    drawWindow(-0.37, -0.25);
    drawWindow(-0.27, -0.25);
    drawWindow(-0.17, -0.25);

    drawWindow(-0.37, -0.15);
    drawWindow(-0.27, -0.15);
    drawWindow(-0.17, -0.15);

    // Control tower
    glColor3f(0.45, 0.45, 0.45);
    glBegin(GL_QUADS);
        glVertex2f(0.05, -0.4);
        glVertex2f(0.12, -0.4);
        glVertex2f(0.12, 0.22);
        glVertex2f(0.05, 0.22);
    glEnd();

    glBegin(GL_QUADS);
        glVertex2f(0.02, 0.22);
        glVertex2f(0.15, 0.22);
        glVertex2f(0.15, 0.28);
        glVertex2f(0.02, 0.28);
    glEnd();

    glBegin(GL_LINES);
    glColor3f(0.45, 0.45, 0.45);
        glVertex2f(0.09, 0.28);
        glVertex2f(0.09, 0.35);
    glEnd();

    glBegin(GL_POINTS);
    glColor3f(1,0,0);
        glVertex2f(0.09, 0.36);
    glEnd();
}

//APARTMENT
void drawApartment(float x, float y, int floors)
{
    float floorHeight = 0.12f;
    float width = 0.22f;

    // Building body
    glColor3f(0.55,0.55,0.55);
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y + floors * floorHeight);
        glVertex2f(x, y + floors * floorHeight);
    glEnd();

    // Roof
    glColor3f(0.3,0.3,0.3);
    glBegin(GL_QUADS);
        glVertex2f(x - 0.01f, y + floors * floorHeight);
        glVertex2f(x + width + 0.01f, y + floors * floorHeight);
        glVertex2f(x + width + 0.01f, y + floors * floorHeight + 0.04f);
        glVertex2f(x - 0.01f, y + floors * floorHeight + 0.04f);
    glEnd();

    // Windows
    for(int f = 0; f < floors; f++)
    {
        for(int w = 0; w < 3; w++)
        {
            if(isNightAirport){glColor3f(1.0,1.0,0.4);} // night windows
            else{glColor3f(0.2,0.4,0.8);} // day windows

            float wx = x + 0.03f + w * 0.07f;
            float wy = y + 0.03f + f * floorHeight;

            glBegin(GL_QUADS);
                glVertex2f(wx, wy);
                glVertex2f(wx + 0.04f, wy);
                glVertex2f(wx + 0.04f, wy + 0.05f);
                glVertex2f(wx, wy + 0.05f);
            glEnd();
        }
    }

    // Main door
    glColor3f(0.25,0.15,0.1);
    glBegin(GL_QUADS);
        glVertex2f(x + 0.09f, y);
        glVertex2f(x + 0.15f, y);
        glVertex2f(x + 0.15f, y + 0.1f);
        glVertex2f(x + 0.09f, y + 0.1f);
    glEnd();
}

void drawApartmentArea()
{
    drawApartment(0.25f, -0.1f, 4);
    drawApartment(0.52f, -0.1f, 5);
    drawApartment(0.80f, -0.1f, 3);
}

//ROAD
void drawRoad()
{
    glColor3f(0.2,0.2,0.2);
    glBegin(GL_QUADS);
        glVertex2f(-1.0f, -0.1f);
        glVertex2f( 1.0f, -0.1f);
        glVertex2f( 1.0f, -0.25f);
        glVertex2f(-1.0f, -0.25f);
    glEnd();

   // Road divider
    glColor3f(1,1,1);
    for(float x=-1; x<1; x+=0.3f)
    {
        glBegin(GL_QUADS);
            glVertex2f(x, -0.17f);
            glVertex2f(x+0.12f, -0.17f);
            glVertex2f(x+0.12f, -0.15f);
            glVertex2f(x, -0.15f);
        glEnd();
    }
}

//CAR
void drawCar(float x, float y)
{
    glPushMatrix();
    glTranslatef(x,y,0);

    // Car body
    glColor3f(1,0,0);
    glBegin(GL_QUADS);
        glVertex2f(0,0);
        glVertex2f(0.25,0);
        glVertex2f(0.25,0.07);
        glVertex2f(0,0.07);
    glEnd();

    // Car top
    glColor3f(0.8,0,0);
    glBegin(GL_QUADS);
        glVertex2f(0.05,0.07);
        glVertex2f(0.20,0.07);
        glVertex2f(0.16,0.12);
        glVertex2f(0.08,0.12);
    glEnd();

    // Windows
    glColor3f(0.6,0.9,1);
    glBegin(GL_QUADS);
        glVertex2f(0.09,0.075);
        glVertex2f(0.14,0.075);
        glVertex2f(0.13,0.11);
        glVertex2f(0.10,0.11);
    glEnd();

    // Wheels
    glColor3f(0,0,0);
    drawCircle(0.05,-0.005,0.025);
    drawCircle(0.20,-0.005,0.025);

    // Car headlights at night
    if(isNightAirport)
    {
        glColor4f(1, 1, 0.8, 0.4);
        glBegin(GL_TRIANGLES);
            glVertex2f(0.0, 0.02);
            glVertex2f(-0.35, -0.05);
            glVertex2f(-0.35, 0.09);
        glEnd();
    }
    glPopMatrix();
}

void drawPlaneAirport()
{
    glPushMatrix();
    glTranslatef(planeX, planeY, 0);

    //BODY
    glColor3f(0.75,0.75,0.75);
    glBegin(GL_POLYGON);
        glVertex2f(0.0f, 0.08f);
        glVertex2f(0.0f, 0.15f);
        glVertex2f(0.36f, 0.15f);
        glVertex2f(0.36f, 0.08f);
    glEnd();

    //COCKPIT
    glColor3f(0.35,0.35,0.35);
    glBegin(GL_POLYGON);
        glVertex2f(0.36f, 0.15f);
        glVertex2f(0.40f, 0.135f);
        glVertex2f(0.413f, 0.12f);
        glVertex2f(0.426f, 0.105f);
        glVertex2f(0.36f, 0.105f);
    glEnd();

    //COCKPIT OUTLINE
    glColor3f(0,0,0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(0.36f, 0.15f);
        glVertex2f(0.40f, 0.135f);
        glVertex2f(0.413f, 0.12f);
        glVertex2f(0.426f, 0.105f);
        glVertex2f(0.36f, 0.105f);
    glEnd();

    //LOWER TRIANGLE
    glColor3f(1,0,0);
    glBegin(GL_POLYGON);
        glVertex2f(0.36f, 0.105f);
        glVertex2f(0.426f, 0.105f);
        glVertex2f(0.426f, 0.098f);
        glVertex2f(0.389f, 0.08f);
        glVertex2f(0.36f, 0.08f);
    glEnd();

    //TAIL
    glColor3f(0.33,0.46,1.0);
    glBegin(GL_POLYGON);
        glVertex2f(0.0f, 0.15f);
        glVertex2f(0.0f, 0.22f);
        glVertex2f(0.03f, 0.22f);
        glVertex2f(0.107f, 0.15f);
    glEnd();

    //LEFT WING
    glColor3f(0,0,1);
    glBegin(GL_POLYGON);
        glVertex2f(0.173f, 0.15f);
        glVertex2f(0.133f, 0.185f);
        glVertex2f(0.2f, 0.185f);
        glVertex2f(0.24f, 0.15f);
    glEnd();

    //RIGHT WING
    glBegin(GL_POLYGON);
        glVertex2f(0.186f, 0.105f);
        glVertex2f(0.267f, 0.105f);
        glVertex2f(0.213f, 0.035f);
        glVertex2f(0.133f, 0.035f);
    glEnd();

    if(isNightAirport)
    {
        glColor4f(1, 1, 0.8, 0.4);
        glBegin(GL_TRIANGLES);
            glVertex2f(0.426f, 0.105f);
            glVertex2f(0.7f, 0.0f);
            glVertex2f(0.7f, 0.2f);
        glEnd();
    }
    if(planeNavLightOn)
    {
        glPointSize(6);
        glColor3f(1,0,0);
        glBegin(GL_POINTS);
            glVertex2f(0.133f, 0.185f);
        glEnd();

        glColor3f(0,1,0);
        glBegin(GL_POINTS);
            glVertex2f(0.267f, 0.105f);
        glEnd();
    }

    //TAIL BLINKING LIGHT
    if(tailLightOn)
    {
        glColor3f(1,0,0);
        glPointSize(6);
        glBegin(GL_POINTS);
            glVertex2f(0.015f, 0.22f);
        glEnd();
    }
    glPopMatrix();
}

//GRASS FIELD
void drawGrassField()
{
    if(isNightAirport){glColor3f(0.05f, 0.25f, 0.05f);}
    else{glColor3f(0.2f, 0.7f, 0.2f);}
    glBegin(GL_QUADS);
        glVertex2f(-1.0f, -1.0f);
        glVertex2f( 1.0f, -1.0f);
        glVertex2f( 1.0f, -0.8f);
        glVertex2f(-1.0f, -0.8f);
    glEnd();

    glBegin(GL_QUADS);
        glVertex2f(-1.0f, -0.4f);
        glVertex2f( 1.0f, -0.4f);
        glVertex2f( 1.0f, -0.25f);
        glVertex2f(-1.0f, -0.25f);
    glEnd();

    glBegin(GL_QUADS);
        glVertex2f(-1.0f, -0.1f);
        glVertex2f( 1.0f, -0.1f);
        glVertex2f( 1.0f, 0.01f);
        glVertex2f(-1.0f, 0.01f);
    glEnd();
}

//WATER
void drawWater()
{
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
        glVertex2f(-1.0f, 0.01f);
        glVertex2f( 1.0f, 0.01f);
        glVertex2f( 1.0f, 0.13f);
        glVertex2f(-1.0f, 0.13f);
    glEnd();
}

void displayAirport()
{
    glClear(GL_COLOR_BUFFER_BIT);

    if(isNightAirport){glClearColor(0.05,0.05,0.2,1);}
    else{glClearColor(0.5,0.9,1,1);}

    drawGrassField();
    drawWater();
    drawSunMoonStars();

    drawCloudAirport(cloudX11,0.75);
    drawCloudAirport(cloudX22,0.82);
    drawCloudAirport(cloudX33,0.78);
    drawCloudAirport(cloudX44,0.85);

    drawApartmentArea();
    drawRoad();
    drawCar(carX, -0.13f);

    drawRunway();
    drawAirport();
    drawAllFloodlights();
    drawPlaneAirport();
    drawRain();
    drawSnow();

    glutSwapBuffers();
}

void idle(){glutPostRedisplay();}

void update(int)
{
    if(isPaused)
    {
        glutPostRedisplay();
        glutTimerFunc(30, update, 0);
        return;
    }
    if(rainOn)
    {
        for(int i = 0; i < 300; i++)
        {
            rainY[i] -= rainSpeed;
            if(rainY[i] < -1.0f){rainY[i] = 1.0f + (i % 10) * 0.1f;}
        }
    }
    if(snowOn)
    {
        for(int i = 0; i < 300; i++)
        {
            snowY[i] -= snowSpeed;
            if(i % 2 == 0){snowX[i] += 0.0005f;}
            else{snowX[i] -= 0.0005f;}
            if(snowY[i] < -1.0f){snowY[i] = 1.0f + (i % 10) * 0.1f;}
        }
    }

    planeX += 0.01f * globalSpeed;
    planeY = -0.4 + 0.3*(planeX + 1.0f); // rises as it moves right

    cloudX11 += 0.002; if(cloudX11 > 1.3) cloudX11 = -1.3;
    cloudX22 += 0.002; if(cloudX22 > 1.3) cloudX22 = -1.3;
    cloudX33 += 0.002; if(cloudX33 > 1.3) cloudX33 = -1.3;
    cloudX44 += 0.002; if(cloudX33 > 1.3) cloudX33 = -1.3;

    static int t=0;
    t++;
    if(t>20)
    {
        planeNavLightOn = !planeNavLightOn;
        tailLightOn = !tailLightOn;
        starsVisibleAirport = !starsVisibleAirport;
        t=0;
    }
    if(planeX > 1.2)
    {
        planeX = -1;
        planeY = -0.4;
    }
    carX -= 0.005f;
    if(carX < -1.2f){carX = 0.65f;}
    glutPostRedisplay();
    glutTimerFunc(30, update, 0);
}

void initRain()
{
    for(int i = 0; i < 300; i++)
    {
        rainX[i] = -1.0f + (i % 30) * 0.07f;
        rainY[i] = -1.0f + (i * 0.03f);
    }
}

void initSnow()
{
    for(int i = 0; i < 300; i++)
    {
        snowX[i] = -1.0f + (i % 30) * 0.07f;
        snowY[i] = -1.0f + (i * 0.025f);
    }
}
void display(){displayAirport();}

void keyboardInput(unsigned char key,int x,int y)
{
    if(key=='n'||key=='N'){isNightAirport = true;}
    if(key=='d'||key=='D'){isNightAirport = false;}
    if(key == 'r' || key == 'R')
    {
        rainOn = true;
        snowOn = false;
    }
    if(key == 's' || key == 'S')
    {
        snowOn = false;
        rainOn = false;
    }
    if(key == 'p' || key == 'P'){isPaused = true;}
    if(key == 'c' || key == 'C'){isPaused = false;}
    if(key == 'w' || key == 'W')
    {
        snowOn = true;
        rainOn = false;
    }
}

void handleMouse(int key, int state, int x, int y)
{
    if(key == GLUT_LEFT_BUTTON){globalSpeed += 0.1f;}
    if (key == GLUT_RIGHT_BUTTON){globalSpeed -= 0.1f;}
    if(globalSpeed < 0.2){globalSpeed = 0.2;}
    glutPostRedisplay();
}


int main(int argc,char** argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(900,600);
    glutCreateWindow("Airplane Takeoff");
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardInput);
    glutMouseFunc(handleMouse);
    glutTimerFunc(30, update, 0);
    initRain();
    initSnow();
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}

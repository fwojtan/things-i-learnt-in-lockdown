#include <stdio.h>
#include <GL/glut.h>
#include <math.h>


double timescale = 1; // how many times faster than real life - we may have to scale distance instead of time
double t_step = 0.01667 * timescale; // define the discrete time interval

// define some constants
double bigG = 6.67430e-11;
double moon_mass = 7.347673e22;
double earth_mass = 5.9736e24;

GLfloat rotation = 90.0;

double rw2screen_units(double value){
	double result = value * 0.9 / 3.844e8;
	return result;
}

double screen2rw_units(double value){
	double result = value * 3.844e8 / 0.9;
	return result;
}

//define position and velocity for moving objects
double ShipPosX = 0.3, ShipPosY = 0, ShipPosZ = 0;
double ShipVelX = 0, ShipVelY = 0, ShipVelZ = 0;
double MoonPosX = 0.1, MoonPosY = 0.9, MoonPosZ = 0.1;
double MoonVelX = rw2screen_units(1.025e3), MoonVelY = 0, MoonVelZ = 0;

//

void reshape(int width, int heigth){
    /* window ro reshape when made it bigger or smaller*/

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //clip the windows so its shortest side is 2.0
    if (width < heigth) {
        glOrtho(-2.0, 2.0, -2.0 * (GLfloat)heigth / (GLfloat)width, 2.0 * (GLfloat)heigth / (GLfloat)width, 2.0, 2.0);
    }
    else{
        glOrtho(-2.0, 2.0, -2.0 * (GLfloat)width / (GLfloat)heigth, 2.0 * (GLfloat)width / (GLfloat)heigth,2.0 , 2.0);
    }
    // set viewport to use the entire new window
    glViewport(0, 0, width, heigth);
}






void DrawShip(){
    glBegin(GL_POLYGON);
    if (MoonPosY < 0){
    	glColor3f(1.0, 0.0, 0.0);
    } else {
    	glColor3f(1.0, 1.0, 1.0);
    }
    glVertex3f(0.0, -0.025, 0.0);
    glVertex3f(-0.03, -0.05, 0.0);
    glVertex3f(0.0, 0.05, 0.0);
    glVertex3f(0.03, -0.05, 0.0);
    glEnd();

}

void DrawCircle(double centerx, double centery, double radius, double number_points){
	glBegin(GL_LINE_LOOP);
	for(int ii = 0; ii < number_points; ii++){

		double angle = 2.0 * 3.1415926 * double(ii) / double(number_points);

		double x = radius * cosf(angle);
        double y = radius * sinf(angle);
        glVertex3f(x + centerx, y + centery, 0.0);
	}
	glEnd();
}

void DrawMoon(){
	DrawCircle(MoonPosX, MoonPosY, 0.05, 40);
}

void DrawEarth(){
	DrawCircle(0.0, 0.0, 0.15, 40);
}

void RK4_update(double *pos, double *vel, double t_step, double acc){
	// Calculates the position and velocity one timestep ahead

	double position = screen2rw_units(*pos);
	double velocity = screen2rw_units(*vel);

	//printf("%.15lf\n",velocity);
	//printf("acc: %.15lf\n", acc);

	double k1vel = acc * position;
	double k1pos = velocity;

	double k2vel = acc * (position + (k1pos * t_step / 2.0));
	double k2pos = velocity * k1vel * t_step / 2.0;

	double k3vel = acc * (position + (k2pos * t_step / 2.0));
	double k3pos = velocity * k2vel * t_step / 2.0;

	double k4vel = acc * (position + (k3pos * t_step));
	double k4pos = velocity * k3vel * t_step;

	//printf("kvals: %.15lf %.15lf %.15lf %.15lf", k1vel, k2vel, k3vel, k4vel);

	double adjust_pos = (k1vel + (2.0 * k2vel) + (2.0 * k3vel) + k4vel) * (t_step / 6.0);
	double adjust_vel = (k1pos + (2.0 * k2pos) + (2.0 * k3pos) + k4pos) * (t_step / 6.0);

	double out_vel = velocity + adjust_vel;
	double out_pos = position + adjust_pos;

	printf("RK4 Position: %.15lf\n", out_pos);
	printf("RK4 Velocity: %.15lf\n", out_vel);

	*vel = rw2screen_units(out_vel);
	*pos = rw2screen_units(out_pos);

	//printf("%.15lf ", *pos);

}


double find_moon_acc(double moon_pos){

	double rwX = screen2rw_units(MoonPosX);
	double rwY = screen2rw_units(MoonPosY);
	double rwZ = screen2rw_units(MoonPosZ);

	double EM_Distance = sqrt(pow(rwX, 2) + pow(rwY, 2) + pow(rwZ, 2));

	//printf("%.15lf\n", EM_Distance);

	double acceleration = -bigG * earth_mass * screen2rw_units(moon_pos) / pow(EM_Distance, 3.0);
	//double val = rw2screen_units(acceleration);
	
	//printf("%.15lf\n", acceleration);
	//printf("G: %.15lf\n Em: %.15lf\n MoonP: %.15lf\n Dist: %.15lf\n", bigG, earth_mass, moon_pos, EM_Distance);
	return acceleration;
}

void update_moon_position(){
	// feed position and velocity of moon into RK4 update function and calculate 
	// the required acceleration
	printf("\nNew coord update: \n");

	double acc_x = find_moon_acc(MoonPosX);
	double acc_y = find_moon_acc(MoonPosY);
	double acc_z = find_moon_acc(MoonPosZ);

	RK4_update(&MoonPosX, &MoonVelX, t_step, acc_x);
	RK4_update(&MoonPosY, &MoonVelY, t_step, acc_y);
	RK4_update(&MoonPosZ, &MoonVelZ, t_step, acc_z);
	
	printf("In screen coords:\n");
	printf("X coord: %.15lf\n", MoonPosX);
	printf("Y coord: %.15lf\n", MoonPosY);
	printf("X vel %.15lf\n", MoonVelX);
	printf("Y vel %.15lf\n", MoonVelY);
	printf("X acc %.15lf\n", acc_x);
	printf("Y acc %.15lf\n", acc_y);

}


void display(){
    //Clear Window
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();
    glTranslatef(ShipPosX,ShipPosY,ShipPosZ);
    DrawShip();
    glPopMatrix();

    update_moon_position();

    //glPushMatrix();
    //glTranslatef(MoonPosX,MoonPosY,MoonPosZ);
	DrawMoon();
	//glPopMatrix();

    DrawEarth();
    glFlush();
}


void init(){
    // set clear color to black
    glClearColor(0.0, 0.0, 0.0, 0.0);

    // set fill color to white
    glColor3f(1.0, 1.0, 1.0);

    //set up standard orthogonal view with clipping
    //box as cube of side 2 centered at origin
    //This is the default view and these statements could be removed
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);

}
float move_unit = 0.01f;
void keyboardown(int key, int x, int y)
{
	// cheers stack overflow for this one
    switch (key){
        case GLUT_KEY_RIGHT:
            ShipPosX+=move_unit;;
            break;

        case GLUT_KEY_LEFT:
            ShipPosX-=move_unit;;
        break;

        case GLUT_KEY_UP:
            ShipPosY+=move_unit;;
            break;

        case GLUT_KEY_DOWN:
            ShipPosY-=move_unit;;
        break;

        default:
         break;
    }
    glutPostRedisplay();
}


void timer(int extra){
    glutPostRedisplay();
    glutTimerFunc(1, timer, 0);
}


int main(int argc, char** argv){

    //initialize mode and open a windows in upper left corner of screen
    //Windows tittle is name of program

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600,600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Practice 1");
    glutDisplayFunc(display);
    init();
    glutSpecialFunc(keyboardown);
    glutTimerFunc(1000, timer, 0);
    glutMainLoop();

    return 0;

}
#include <stdio.h>
#include <GL/glut.h>
#include <math.h>

// define some constants
double bigG = 6.67430e-11;
double moon_mass = 7.347673e22;
double earth_mass = 5.9736e24;

double t_step = 100;
//define position and velocity for moving objects in screen units
double ShipPosX = 0, ShipPosY = 0.6, ShipPosZ = 0;
double ShipVelX = 0, ShipVelY = 0, ShipVelZ = 0;
double MoonPosX = 0, MoonPosY = 0.9, MoonPosZ = 0;
double MoonVelX = 0.0000023, MoonVelY = 0, MoonVelZ = 0;

// unit conversion

double rwUnits(double value){
	return value * 3.844e8 / 0.9;
}

double dispUnits(double value){
	return value * 0.9 / 3.844e8;
}

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
	DrawCircle(0, 0, 0.05, 40);
}

void DrawEarth(){
	DrawCircle(0.0, 0.0, 0.15, 40);
}

double find_moon_acceleration(double position, double distance){
	// Apply Newton's law of graviatition

	distance = rwUnits(distance);
	position = rwUnits(position);

	double acceleration = -bigG * earth_mass * position / pow(distance, 3);

	return dispUnits(acceleration);

}

double find_ship_acceleration(double position, double earth_dist, double moon_dist){
	earth_dist = rwUnits(earth_dist);
	moon_dist = rwUnits(moon_dist);
	position = rwUnits(position);

	double e_acc = -bigG * earth_mass * position / pow(earth_dist, 3);
	double m_acc = -bigG * moon_mass * position / pow(moon_dist, 3);

	return dispUnits(e_acc + m_acc);
}


void RK4_update(double *pos, double *vel, double t_step, double acc){
	// Calculates the position and velocity one timestep ahead

	// this implimentation of RK4 is very not good

	//double position = rwUnits(*pos);
	//double velocity = rwUnits(*vel);
	double position = *pos;
	double velocity = *vel;

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

	//printf("kvals: %.1lf %.1lf %.1lf %.1lf\n", k1pos, k2pos, k3pos, k4pos);

	double adjust_pos = (k1pos + (2.0 * k2pos) + (2.0 * k3pos) + k4pos) * (t_step / 6.0);
	double adjust_vel = (k1vel + (2.0 * k2vel) + (2.0 * k3vel) + k4vel) * (t_step / 6.0);

	double out_vel = velocity + adjust_vel;
	double out_pos = position + adjust_pos;

	//printf("RK4 Position: %.10lf + %10lf\n", position, adjust_pos);
	//printf("RK4 Velocity: %.10lf + %10lf\n", velocity, adjust_vel);

	//*vel = dispUnits(out_vel);
	//*pos = dispUnits(out_pos);
	*vel = out_vel;
	*pos = out_pos;

	//printf("New position:%.15l\n ", *pos);

}

void suvat(double *pos, double *vel, double t_step, double acc){

	*pos += (*vel + 0.5 * acc * t_step) * t_step;
	*vel += acc * t_step;

}


void update_moon_position(){

	double moon_dist = sqrt(MoonPosX * MoonPosX + MoonPosY * MoonPosY + MoonPosZ * MoonPosZ);

	double x_acc = find_moon_acceleration(MoonPosX, moon_dist);
	double y_acc = find_moon_acceleration(MoonPosY, moon_dist);
	double z_acc = find_moon_acceleration(MoonPosZ, moon_dist);

	//printf("\nNew step:\n");
	//printf("Distance: %0.15lf\n", moon_dist);
	//printf("Xacc: %0.15lf\n", x_acc);
	//printf("Yacc: %0.15lf\n", y_acc);
	//printf("Zacc: %0.15lf\n", z_acc);

	suvat(&MoonPosX, &MoonVelX, t_step, x_acc);
	suvat(&MoonPosY, &MoonVelY, t_step, y_acc);
	suvat(&MoonPosZ, &MoonVelZ, t_step, z_acc);

	//RK4_update(&MoonPosX, &MoonVelX, t_step, x_acc);
	//RK4_update(&MoonPosY, &MoonVelY, t_step, y_acc);
	//RK4_update(&MoonPosZ, &MoonVelZ, t_step, z_acc);

	//printf("In screen coords:\n");
	//printf("X coord: %.15lf\n", MoonPosX);
	//printf("Y coord: %.15lf\n", MoonPosY);
	//printf("X vel %.15lf\n", MoonVelX);
	//printf("Y vel %.15lf\n", MoonVelY);



}

void update_ship_position(){

	double moon_dist = sqrt(pow(MoonPosX - ShipPosX, 2)+pow(MoonPosY - ShipPosY, 2)+pow(MoonPosZ - ShipPosZ, 2));
	double earth_dist = sqrt(pow(ShipPosX, 2)+pow(ShipPosY, 2)+pow(ShipPosZ, 2));

	double x_acc = find_ship_acceleration(MoonPosX, earth_dist, moon_dist);
	double y_acc = find_ship_acceleration(MoonPosY, earth_dist, moon_dist);
	double z_acc = find_ship_acceleration(MoonPosZ, earth_dist, moon_dist);

	suvat(&ShipPosX, &ShipVelX, t_step, x_acc);
	suvat(&ShipPosY, &ShipVelY, t_step, y_acc);
	suvat(&ShipPosZ, &ShipVelZ, t_step, z_acc);

	//printf("Xacc: %0.15lf\n", x_acc);
	//printf("Yacc: %0.15lf\n", y_acc);
	//printf("Zacc: %0.15lf\n", z_acc);
	printf("New timestep: \n");
	printf("Mdist: %0.10lf\n", moon_dist);
	printf("Edist: %0.10lf\n", earth_dist);

	printf("X coord: %.10lf X vel: %.10lf X acc: %0.10lf\n", ShipPosX, ShipVelX, x_acc);
	printf("Y coord: %.10lf Y vel: %.10lf Y acc: %0.10lf\n", ShipPosY, ShipVelY, y_acc);
	//printf("X vel %.15lf\n", ShipVelX);
	//printf("Y vel %.15lf\n", ShipVelY);

}


void display(){
    //Clear Window
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    update_ship_position();

    glPushMatrix();
    glTranslatef(ShipPosX,ShipPosY,ShipPosZ);
    DrawShip();
    glPopMatrix();

    update_moon_position();

    glPushMatrix();
    glTranslatef(MoonPosX,MoonPosY,MoonPosZ);
	DrawMoon();
	glPopMatrix();

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
    glutTimerFunc(1, timer, 0);
    glutMainLoop();

    return 0;

}
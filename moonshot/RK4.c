#include <stdio.h>

double RK4_integrate(double, double, double (*func)(double, double), double);





int main(){
	

	return 0;
}

double RK4_integrate(double x, double y, double (*func)(double, double), double step){

	// A procedure to numerically integrate a first order ODE of the form dy/dx = f(x, y)
	
	double k1 = step * (*func)(x, y);
	double k2 = step * (*func)((x + (step / 2)), (y + (k1 / 2)));
	double k3 = step * (*func)((x + (step / 2)), (y + (k2 / 2)));
	double k4 = step * (*func)((x + step), (y + k3));

	double next_y = y + (k1 / 6) + (k2 / 3) + (k3 / 3) + (k4 / 6);
	return next_y;

}

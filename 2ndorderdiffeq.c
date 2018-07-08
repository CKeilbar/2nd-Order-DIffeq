/*Program to solve second order differential equations of the form ay''+by'+c=0, started 2018/07/08*/
#include <stdio.h>
#include <math.h>

int main(){

	float a=0; 
	float b=0;
	float c=0;
	float discriminant=0;
	float firstexp=0;
	float secondexp=0;
	const float EPSILON = 0.0000005;
	printf("Enter some numerical constants a, b, and c to find the solution to the differential equation ay\"+by'+c=0\n");
	printf("a = ");
	scanf("%f", &a);
	printf("b = ");
	scanf("%f",&b);
	printf("c = ");
	scanf("%f",&c);
	discriminant=b*b-4*a*c;
	printf("The general solution to the linear homogeneous second order equation with constant coefficients %.2fy\"%+.2fy'%+.2fy = 0 is:\n",a,b,c);
	if (fabs(discriminant)<= EPSILON){ /*case 1, repeated roots*/
		firstexp = (-b)/(2*a);
		printf("y = c1*exp(%f*t) + c2*t*exp(%f*t)\n",firstexp,firstexp);
	}
	else if (discriminant < 0){ /*characteristic equation has 2 complex conjugate roots*/
		firstexp = (-b)/(2*a);
		secondexp = sqrt(-1*discriminant)/(2*a);
		printf("y = c1*exp(%f*t)*sin(%f*t) + c2*exp(%f*t)*cos(%f*t)\n",firstexp,secondexp,firstexp,secondexp);
	}
	else { /*general case, 2 distinct real roots*/
		firstexp = (-b+sqrt(discriminant))/(2*a);
		secondexp = (-b-sqrt(discriminant))/(2*a);
		printf("y = c1*exp(%f*t) + c2*exp(%f*t)\n",firstexp,secondexp);
	}
	printf("For some arbitrary scalar constants c1 and c2.\n");
	return 0;
}
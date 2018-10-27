/*Program to solve second order RLC circuits, started 2018/10/26*/
/*Program cannot handle improper inputs at the moment, sanitize your inputs!*/
/*Right now it prints 8 decimal places. For additional precision, decrease epsilon and increase the decimal places*/
/*Note, however that this means it takes longer to detect when a steady state has been reached*/
#include <stdio.h>
#include <math.h>

double foundfunction(double arb1, double arb2, double finalval, double alpha, double omegad, double s1, double s2, double t){/*This bad boy calculates some data if needed*/
	if(s1 == -1 && s2 == -1 && omegad == -1){/*critically damped*/
		return arb1*t*exp(-1*alpha*t)+arb2*exp(-1*t)+finalval;
	}
	else if(omegad == -1){/*overdamped*/
		return arb1*exp(s1*t) + arb2*exp(s2*t) + finalval;
	}
	else{
		return exp(-1*alpha*t)*(arb1*cos(t*omegad)+arb2*sin(t*omegad)) + finalval;
	}
}

int main(){

	int voltage = 0; /*this looks for a solution of voltage or current as a function of time*/
	double resistance = 0;
	double capacitance = 0;
	double inductance = 0;
	double alpha = 0; 
	double omeganaught = 0;
	double omegad = -1;/*used in underdamped case*/
	double condition = 0;/*this is for determining which case of damping we have*/
	int parallel = 0;/*this determines if it is a series or a parallel circuit*/
	double initialat0 = 0;/*value at t = 0 of current or voltage*/
	double finalvalue = 0;/*final value of the circuit*/
	double initialderivative = 0; /*used for the derivative initial condition*/
	const double EPSILON = 0.0000000005;
	double arb1 = 0;/*arbitrary values of constants, determined with initial conditions*/
	double arb2 = 0;
	double s1 = -1;/*used to find the two unique roots*/
	double s2 = -1;

	int additionaldata = 0;
	double starttime = 0;
	double endtime = 0;
	double interval = 0;
	FILE* ourfile = fopen("data.txt","w");

	printf("Are we looking for voltage or current? Type 1 for voltage and 0 for current: ");
	scanf("%d",&voltage);
	printf("Is our circuit in parallel or in series? Type 1 for parallel and 0 for series: ");
	scanf("%d",&parallel);
	printf("Enter the resistance in Ohms: ");
	scanf("%lf", &resistance);
	printf("Enter the capacitance in Farads: ");
	scanf("%lf",&capacitance);
	printf("Enter the inductance in Henrys: ");
	scanf("%lf", &inductance);
	if(voltage){/*we thus need Vc(0+) and Ic(0+)*/
		printf("Enter the initial voltage across the capacitor, Vc(0+): ");
		scanf("%lf", &initialat0);
		printf("Enter the initial current through the capacitor, Ic(0+): ");
		scanf("%lf", &initialderivative);
		initialderivative = initialderivative/capacitance;
	}
	else{/*we want il(0+) and Vl(0+)*/
		printf("Enter the initial current through the inductor, Il(0+): ");
		scanf("%lf", &initialat0);
		printf("Enter the initial voltage across the capacitor, Vl(0+): ");
		scanf("%lf", &initialderivative);
		initialderivative = initialderivative/inductance;
	}
	printf("Enter the final value as t -> infinity: ");
	scanf("%lf", &finalvalue);
	omeganaught = 1/sqrt(inductance*capacitance);

	alpha = parallel ? 1/(2*resistance*capacitance) : resistance/(2*inductance);
	
	condition = alpha*alpha - omeganaught*omeganaught; /*we use this to determine the damping case*/
	if(fabs(condition) <= EPSILON){/*critical damping*/
		arb2 = initialat0 - finalvalue;
		arb1 = initialderivative + alpha*arb2;
		if(voltage){
			printf("Your solution is:\nVc(t) = %f*t*exp(-%f*t) + %f*exp(-%f*t) + %f\n", arb1,alpha,arb2,alpha,finalvalue);
		}
		else{
			printf("Your solution is:\nIl(t) = %f*t*exp(-%f*t) + %f*exp(-%f*t) + %f\n", arb1,alpha,arb2,alpha,finalvalue);
		}
	}
	else if (condition > 0){/*overdamping*/
		s1 = -1*alpha+sqrt(condition);/*reusing condition, as it is alpha^2-omega^2*/
		s2 = -1*alpha-sqrt(condition);
		arb1 = (initialderivative + s2*finalvalue - s2*initialat0)/(s1-s2);/*it can be shown this is true algebraically*/
		arb2 = initialat0 - finalvalue - arb1;
		if(voltage){
			printf("Your solution is:\nVc(t) = %f*exp(%f*t) + %f*exp(%f*t) + %f\n", arb1, s1, arb2, s2, finalvalue);
		}
		else{
			printf("Your solution is:\nIl(t) = %f*exp(%f*t) + %f*exp(%f*t) + %f\n", arb1, s1, arb2, s2, finalvalue);
		}
	}
	else{/*under damped*/
		omegad = sqrt(-1*condition);
		arb1 = initialat0 - finalvalue;
		arb2 = (initialderivative + alpha*arb1)/omegad;
		if(voltage){
			printf("Your solution is:\nVc(t) = exp(-%f*t)*(%f1*cos(%f*t) + %f*sin(%f*t)) + %f\n",alpha,arb1,omegad,arb2,omegad,finalvalue);
		}
		else{
			printf("Your solution is:\nIl(t) = exp(-%f*t)*(%f1*cos(%f*t) + %f*sin(%f*t)) + %f\n",alpha,arb1,omegad,arb2,omegad,finalvalue);
		}
	}
	/*begin part 2, where we generate data*/
	
	printf("Would you like to generate data for this circuit? Type 1 for yes and 0 for no: ");
	scanf("%d", &additionaldata);
	if(!additionaldata){
		printf("Goodbye!\n");
		fclose(ourfile);
		return 1;
	}
	printf("Enter the time to start collecting data in seconds: ");
	scanf("%lf",&starttime);
	printf("Enter the time to stop collecting data, or enter 0 to detect a steady state: ");
	scanf("%lf",&endtime);
	printf("Enter the data time interval: ");
	scanf("%lf", &interval);
	fprintf(ourfile,"Time (s)\t");
	voltage ? fprintf(ourfile,"Vc(t)\n") : fprintf(ourfile,"Ic(t)\n");
	if(endtime == 0){/*looking for a steady state. This is achieved after when the difference between successive data entries is negligible and we approach the final value*/
		fprintf(ourfile, "%0.8lf\t%0.8lf\n",starttime,foundfunction(arb1,arb2,finalvalue,alpha,omegad,s1,s2,starttime));
		starttime = starttime + interval;
		while(fabs(foundfunction(arb1,arb2,finalvalue,alpha,omegad,s1,s2,starttime)-finalvalue) > EPSILON || fabs(foundfunction(arb1,arb2,finalvalue,alpha,omegad,s1,s2,starttime-interval)-foundfunction(arb1,arb2,finalvalue,alpha,omegad,s1,s2,starttime)) > EPSILON){
			fprintf(ourfile, "%0.8lf\t%0.8lf\n",starttime,foundfunction(arb1,arb2,finalvalue,alpha,omegad,s1,s2,starttime));
			starttime = starttime + interval;
		}
	}
	else{/*note that the end time to stop collecting data isn't strictly enforced. We go up interval by interval until we pass it*/
		while(starttime <= endtime){
			fprintf(ourfile,"%0.8lf\t%0.8lf\n",starttime,foundfunction(arb1,arb2,finalvalue,alpha,omegad,s1,s2,starttime));
			starttime = starttime + interval;
		}
	}
	printf("The data has been printed to 'data.txt' in the current directory.\n");
	fclose(ourfile);
	return 0;
}
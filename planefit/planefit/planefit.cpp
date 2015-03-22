// planefit.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


using namespace cv; // all the new API is put into "cv" namespace. Export its content
using namespace std;
//using namespace cv::flann;

float A, B, C, D;
//http://mariotapilouw.blogspot.com/2011/05/plane-fitting-using-opencv.html
//Axi + Byi + C = zi
int cvtest()
{
CvMat *res  = cvCreateMat(3, 1, CV_32FC1);
CvMat *matX = cvCreateMat(50, 3, CV_32FC1);
CvMat *matZ = cvCreateMat(50, 1, CV_32FC1);
int		i;
float	A1, B1, C1;

	A= -2;
	B= 2;
	C= 3;

// create data 

	for (i=0;i<50; i++)
		{

		float x= rand() % 25;
		float y= rand() % 50;
		float z= A*x + B*y + C + rand()%50 - 25;

		cvmSet(matX, i, 0, x);  //x
		cvmSet(matX, i, 1, y); //y
		cvmSet(matX, i, 2, 1);  // 1
		cvmSet(matZ, i, 0, z);  //z

		}

	cvSolve(matX, matZ, res, CV_SVD);
	A1 = cvmGet(res, 0, 0);
	B1 = cvmGet(res, 1, 0);
	C1 = cvmGet(res, 2, 0);

	printf("result %f %f %f\r\n", A1, B1, C1);

#if 0
double sqrtc = sqrt(pow(A, 2) + pow(B, 2) + pow(C, 2));

float min      = 100;
float max      = -100;

// plane generation, Z = Ax + By + C
// distance calculation d = Ax + By - z + C / sqrt(A^2 + B^2 + C^2)
// pointlist generation
for(int row=0;row<filteredMat->rows;row++)
{
 for(int col=0;col<filteredMat->cols;col++)
 {
   double Zval     = cvmGet(filteredMat, row, col);
   double val      = col*pixScale*A + row*pixScale*B - Zval + C;
   double distance = val / sqrtc;

   if(min > distance)
     min = distance;

   if(max < distance)
     max = distance;

   cvmSet(planeMat, row, col, distance);
 }
}
#endif

return 0;
}


struct 
{

float x, y, z;

} pts[500];





float calcplaneZ(float a, float b, float c, float d, float x, float y)
{

float	z;

	z= (-d-a*x-b*y)/c;

	return z;
}

int calcplane(int pta, int ptb, int ptc)
{
	float vabx= pts[ptb].x - pts[pta].x;
	float vaby= pts[ptb].y - pts[pta].y;
	float vabz= pts[ptb].z - pts[pta].z;

	float vacx= pts[ptc].x - pts[pta].x;
	float vacy= pts[ptc].y - pts[pta].y;
	float vacz= pts[ptc].z - pts[pta].z;

	float cx= vaby*vacz - vabz*vacy;
	float cy= vabz*vacx - vabx*vacz;
	float cz= vabx*vacy - vaby*vacx;

	float sum= cx+cy+cz;
	cx= -cx/sum;
	cy= -cy/sum;
	cz= -cz/sum;
	float d= cx*pts[pta].x + cy*pts[pta].y + cz*pts[pta].z;

	A=cx;
	B=cy;
	C=cz;
	D= -d;

//	printf("%3.3f %3.3f %3.3f  %3.3f\r\n", cx, cy, cz, d);

	return 0;
}

float calcplaneerror()
{
float	dif, error;
int		i;
float	x, y, z;

	error= 0;
	for (i=0; i<50; i++)
		{
		x= pts[i].x;
		y= pts[i].y;
		z= calcplaneZ(A, B, C, D, x, y) ;
		dif= z-pts[i].z;
		error= error + dif*dif;
//		printf("%f %f %f\r\n", pts[i].x,  pts[i].y,  pts[i].z); 
		}

	return error;
}

int main2()
{
int		i;
int		pta, ptb, ptc;

	A= 2;
	B= -8;
	C= 5;
	D= -18;

	for (i=0; i<50; i++)
		{
		pts[i].x= rand()%20-10;
		pts[i].y= rand()%20-10;
		pts[i].z= calcplaneZ(A, B, C, D, pts[i].x, pts[i].y) + (rand()*3.0)/(float)RAND_MAX ;
		printf("%f %f %f\r\n", pts[i].x,  pts[i].y,  pts[i].z); 
		}

	printf("test %f\r\n", calcplaneZ(A, B, C, D, 1,-2));
	printf("test %f\r\n", calcplaneZ(A, B, C, D, 3,1));
	printf("test %f\r\n", calcplaneZ(A, B, C, D, 0,-1));

// calc the VAB VAC from 3 random points in the 50
	for (i=0; i<50; i++)
	{
pickrand:

	pta= rand() % 50;
	ptb= rand() % 50;
	ptc= rand() % 50;

	if (pta==ptb)
		goto pickrand;
	if (pta==ptc)
		goto pickrand;
	if (ptb==ptc)
		goto pickrand;

	calcplane(pta, ptb, ptc);
	float error= calcplaneerror();

	printf("%f %f %f %f %f\r\n", A, B, C, D, error);


	}

	return 0;
}

extern int main3d();
extern int cimg2(int argc,char **argv);

int main()
{

int		pta, ptb, ptc;

	cimg2(0, 0);
	main3d();
cvtest();

	main2();

	A= -1.0;
	B= 3.0;
	C= 2.0;
	D= -4;


		pts[0].x= 1;
		pts[0].y= 1;
		pts[0].z= 1;

		pts[1].x= -1;
		pts[1].y= 1;
		pts[1].z= 0;

		pts[2].x= 2;
		pts[2].y= 0;
		pts[2].z= 3;
////////////////////////
		pts[0].x= 1;
		pts[0].y= -2;
		pts[0].z= 0;

		pts[1].x= 3;
		pts[1].y= 1;
		pts[1].z= 4;

		pts[2].x= 0;
		pts[2].y= -1;
		pts[2].z= 2;

	pta= 0;
	ptb= 1;
	ptc= 2;


	printf("using pts %d %d %d\r\n", pta, ptb, ptc);

	float vabx= pts[ptb].x-pts[pta].x;
	float vaby= pts[ptb].y- pts[pta].y;
	float vabz= pts[ptb].z- pts[pta].z;


	float vacx= pts[ptc].x-pts[pta].x;
	float vacy= pts[ptc].y- pts[pta].y;
	float vacz= pts[ptc].z- pts[pta].z;

	float cx= vaby*vacz - vabz*vacy;
	float cy= vabz*vacx - vabx*vacz;
	float cz= vabx*vacy - vaby*vacx;

	printf("%f %f %f\r\n", cx, cy, cz);

	

	return 0;
}


// project is on this beacuse 
// can't install isula studio here


// Rectangle simulator

#include <iostream>
#include <graphics.h>
using namespace std;


//Constants

#define MAXX 1500
#define MAXY 800
#define DIFF 50
#define BOUND_COLOR GREEN
#define BACK_COLOR BLACK
#define DRAW_COLOR1 BLUE
#define DRAW_COLOR2	YELLOW
#define COL_COLOR RED
#define TIME_DIFF 0.02  // seconds
// global variables

int nFrames = 0;

// class definations

class Vector2D
{
	public:
		double x,y;
		Vector2D()	// constructor
		{
			x = y = 0.0 ;
		}
		Vector2D(double a,double b ) : x( a ), y( b ) // constructor 2
		{			
		}
		void setVal(double a=0.0,double b=0.0)
		{
			x = a;
			y = b;
		}
		void print()
		{
			cout << "(" << x << "," << y << ")" << endl;
		}
		void operator+(Vector2D &vec2) //////////////////////////////////////////sdgsg/sdgsgs/g/sg
		{
			Vector2D res;
			res.x = this->x + vec2.x ;
			res.y = this->y + vec2.y ;
		}
		
};


class Force : public Vector2D
{
	public:
		Vector2D pointOfAction;
	
};

class Rect
{
	public:
		Vector2D p[4] ;
		int point[8] ;
		Vector2D linV;
		Vector2D linA;
		double angV ;
		double angA  ;
		Vector2D com;
		double mass ;
		Vector2D disp ;	// displacement
		double theta;
		
		
		Rect(Vector2D *points) // constructor
		{
			for(int i=0; i<4; i++)
			{
				p[i].setVal( (points+i)->x, (points+i)->y) ;
			}
			
			mass = 1.0;
						
		}
		
		Rect(Vector2D *com, int halfL, int halfB)
		{
			p[0].setVal(com->x - halfL, com->y - halfB );
			p[1].setVal(com->x + halfL, com->y - halfB );
			p[2].setVal(com->x + halfL, com->y + halfB );
			p[3].setVal(com->x - halfL, com->y + halfB );
			
			mass = 1.0;
		}
		
		void draw(int color = DRAW_COLOR1)
		{
			
			for(int i=0; i<4; i++)
			{
				point[i*2] = (int)(p[i].x);
				point[i*2+1] = (int)(p[i].y);
			}
			setcolor(color) ;
			setfillstyle(SOLID_FILL, color) ;
			fillpoly(4,point) ;
		}
		void translate(double a, double b)
		{
			for(int i=0; i<4; i++)
			{
				p[i].x += a;
				p[i].y += b;
			}
		}
		void translate(Vector2D disp)
		{
			for(int i=0; i<4; i++)
			{
				p[i].x += disp.x;
				p[i].y += disp.y;
			}
		}
		void setKinametics(Vector2D *vel, Vector2D *acc, double angVel = 0, double angAcc = 0)
		{
			linV.setVal(vel->x, vel->y);
			linA.setVal(acc->x, acc->y);
			angV = angVel ;
			angA = angAcc ;
		}
		
		void updateLinear(double dt = TIME_DIFF)
		{			
			disp.x = linV.x*dt + 0.5*linA.x*dt*dt ;
			disp.y = linV.y*dt + 0.5*linA.y*dt*dt ;
			
			linV.x += linA.x*dt ;
			linV.y += linA.y*dt ;
			translate(disp) ;
		}
		
};

void drawBoundary()
{
	setfillstyle(SOLID_FILL,BOUND_COLOR);
	bar(0, 0, MAXX, DIFF) ; 
	bar(0, MAXY-DIFF, MAXX, MAXY);
	bar(0, DIFF, DIFF, MAXY-DIFF);
	bar(MAXX-DIFF, DIFF, MAXX, MAXY-DIFF) ;	
}

int main()
{
	
	short int halfL = 50;
	short int halfB = 80;
	Vector2D com(200,300);
	Vector2D velocity(10.0, 10.0) ;
	Vector2D acc(0, 9.8) ;
	
	Rect abcd(&com, halfL, halfB) ;
	abcd.setKinametics(&velocity, &acc, 0, 0) ;
	
	//for(int i=0; i<4; i++)
		//abcd.p[i].print();
	
	initwindow(MAXX, MAXY);
	
	abcd.draw();
	
	drawBoundary();
	
	while(1)
	{
		abcd.draw(BACK_COLOR) ;
		abcd.updateLinear();
		abcd.draw() ;
		delay(10);
	}
		
	
	return 0;
}

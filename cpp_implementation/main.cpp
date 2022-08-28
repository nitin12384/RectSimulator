// project is on this beacuse
// can't install visual studio here in the cp lab

// Rectangle simulator

//#define NO_GRAPHICS


#include <iostream>
#ifndef NO_GRAPHICS
	#include <graphics.h>
#endif
#include <math.h>

using namespace std;

//Constants
#ifdef NO_GRAPHICS

enum
{
	BLACK, BLUE, RED, YELLOW, GREEN, SOLID_FILL
} ;

#endif



#define MAXX 1500
#define MAXY 800
#define DIFF 50
#define BOUND_COLOR GREEN
#define BACK_COLOR BLACK
#define DRAW_COLOR1 BLUE
#define DRAW_COLOR2 YELLOW
#define COL_COLOR RED
#define TIME_DIFF 0.02 // seconds
#define DEF_E 0.900



// global variables

int nFrames = 0;
bool dM = true;
bool sM = 1;
bool error = 1;
bool isPhone = 1     ;

bool accurColRes = false; // two way prog

// class definations

class Vector2D
{
  public:
	double x, y;
	Vector2D() // constructor
	{
		x = y = 0.0;
	}
	Vector2D(double a, double b) : x(a), y(b) // constructor 2
	{
	}
	void setVal(double a = 0.0, double b = 0.0)
	{
		x = a;
		y = b;
	}
	void print()
	{
		cout << "(" << x << "," << y << ")" << endl;
	}
	Vector2D operator+(Vector2D &vec2) //////////////////////////////////////////sdgsg/sdgsgs/g/sg
	{
		Vector2D res;
		res.x = this->x + vec2.x;
		res.y = this->y + vec2.y;
		return res;
	}
	/*Vector2D operator=(Vector2D &vec2)
		{
		    Vector2D res;
		    res.x = vec2.x;
		    res.y = vec2.y ;
		    return res;
		}*/
/*	void operator+=(Vector2D &vec2) //////////////////////////////////////////sdgsg/sdgsgs/g/sg
	{
		this->x += vec2.x;
		this->y += vec2.y;
	}*/
	//??????
	void operator+=(Vector2D vec2) //////////////////////////////////////////sdgsg/sdgsgs/g/sg
	{
		this->x += vec2.x;
		this->y += vec2.y;
	}
	//???????
	Vector2D operator-(Vector2D &vec2)
	{
		Vector2D res;
		res.x = this->x - vec2.x;
		res.y = this->y - vec2.y;
		return res;
	}

	void operator-=(Vector2D &vec2)
	{
		this->x -= vec2.x;
		this->y -= vec2.y;
	}
	Vector2D operator/(double n)
	{
		Vector2D res;
		res.x = this->x / n;
		res.y = this->y / n;
		return res ;
	}
	void operator/=(double n)
	{
		this->x /= n;
		this->y /= n;
	}
	Vector2D operator*(double n)
	{
		Vector2D res;
		res.x = this->x * n;
		res.y = this->x * n;
		return res;
	}
	void operator*=(double n)
	{
		this->x *= n;
		this->y *= n;
	}
	bool operator==(Vector2D &vec2)
	{
		return (x == vec2.x && y == vec2.y);
	}
	double distFrom(Vector2D vec2) //expensive sqrt function
	{
		return sqrt((x - vec2.x) * (x - vec2.x) + (y - vec2.y) * (y - vec2.y));
	}
	double getMagSq()
	{
		return (x * x + y * y);
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
	Vector2D p[4];
	int point[8];
	Vector2D linV;
	Vector2D linA;
	double angV;
	double angA;
	Vector2D com;
	double mass;
	Vector2D disp; // displacement
	double theta;  // angular disp
	short int maxyIndex;
	//    maxx =( maxy + 1 )%4
	//    miny =( maxy + 2 )%4
	//    minx =( maxy + 3 )%4
	//    but.. yest it first

	bool colWall[4];

	Rect(Vector2D *points) // constructor
	{
		for (int i = 0; i < 4; i++)
		{
			p[i] = *(points + i);
		}

		mass = 1.0;
		updateCOM();
	}

	Rect(Vector2D *com, int halfL, int halfB)
	{
		p[0].setVal(com->x - halfL, com->y - halfB);
		p[1].setVal(com->x + halfL, com->y - halfB);
		p[2].setVal(com->x + halfL, com->y + halfB);
		p[3].setVal(com->x - halfL, com->y + halfB);

		mass = 1.0;
		updateCOM();
	}
	void getMaxY()
	{
		if (dM)
			cout << "getMaxY called \n";
		maxyIndex = 0;
		for (int i = 0; i < 4; i++)
		{
			if (p[i].y > p[maxyIndex].y)
			{
				maxyIndex = i;
			}
		}
	}
	bool chkColWall() // Sat theorom
	{
		if (dM)
			cout << "chkColWall called \n";
		for (int i = 0; i < 4; i++)
			colWall[i] = false;
		getMaxY();
		//
		if (p[maxyIndex].y >= MAXY - DIFF && linV.y > 0)
			colWall[0] = 1;
		if (p[(maxyIndex + 1) % 4].x >= MAXX - DIFF /*maxx*/ && linV.x > 0)
			colWall[1] = 1;
		if (p[(maxyIndex + 2) % 4].y <= DIFF /*miny*/ && linV.y < 0)
			colWall[2] = 1;
		if (p[(maxyIndex + 3) % 4].x <= DIFF /*minx*/ && linV.x < 0)
			colWall[3] = 1;

		return colWall[0] || colWall[1] || colWall[2] || colWall[3];
	}
	void colResWall()
	{
		if (dM)
			cout << "colResWall called \n";

		if (!chkColWall())
			return;
	}
	void colProcess(Vector2D norm)
	{
		if (dM)
			cout << "colProcess called \n";
	}

	Vector2D reflectVel(Vector2D norm = {1, 0}, double e = DEF_E) // returns change in velocityV(f) - V(i)
	{
		if (dM)
			cout << "reflectVel called \n";
		if (norm == * new Vector2D(0,0)) // ?????????????????????????????????
			return norm;

		Vector2D compParallel;
		Vector2D compPerpend;
		
		
		//left
		
		
		// continue here
		linV = compParallel * (-e) + compPerpend;
		return compParallel * (-e - 1);
	}
	void draw(int color = DRAW_COLOR1)
	{
		if(isPhone)
		{
			if(dM) cout << "Phone doesn't Support Graphics !" ;
			return ;
		}
		for (int i = 0; i < 4; i++)
		{
			point[i * 2] = (int)(p[i].x);
			point[i * 2 + 1] = (int)(p[i].y);
		}
		#ifndef NO_GRAPHICS
		setcolor(color);
		setfillstyle(SOLID_FILL, color);
		fillpoly(4, point);
		#endif
	}
	void translate(double a, double b)
	{
		for (int i = 0; i < 4; i++)
		{
			p[i].x += a;
			p[i].y += b;
		}
	}
	void translate(Vector2D disp)
	{
		for (int i = 0; i < 4; i++)
		{
			p[i] += disp;
		}
		updateCOM();
	}
	void setKinametics(Vector2D *vel, Vector2D *acc, double angVel = 0, double angAcc = 0)
	{
		linV.setVal(vel->x, vel->y);
		linA.setVal(acc->x, acc->y);
		angV = angVel;
		angA = angAcc;
	}

	void updateLinear(double dt = TIME_DIFF)
	{
		disp += linV * dt  ;
		disp +=  linA *(dt * dt * 0.5 ); 
		// would double * Vector2D work ??

		linV += linA * dt;
		translate(disp);
	}
	void rotate(double theta) // rotation in radian
	{
		for(int i=0; i<4; i++)
		{
			double tempX = p[i].x;
			double tempY = p[i].y ;
			double cosT, sinT;
			cosT = cos(theta);
			sinT = sin(theta);
			p[i].x = com.x + (tempX - com.x) * cosT - (tempY - com.y) * sinT;
			p[i].y = com.y + (tempX - com.x) * sinT + (tempY - com.y) * cosT;
		}
		
		updateCOM();
	}
	void updateCOM()
	{
		com = {0, 0};
		for (int i = 0; i < 4; i++)
		{
			com += p[i] / 4;
		}
	}
	void updateAngular(double dt = TIME_DIFF)
	{
		theta = angV * dt + 0.5 * angA * dt * dt;
		angV += angA * dt;

		rotate(theta);
	}
	void updatePhysics()
	{
		updateAngular();
		updateLinear();
	}
	void print()
	{
		cout << "Points are -- \n";
		for (int i = 0; i < 4; i++)
		{
			p[(maxyIndex + i) % 4].print();
		}

		cout << " COM - ";
		com.print();
		cout << " linV and linA - \n"; 
		linV.print();
		linA.print();

		cout << "angV and angA - " << angV << angA << endl;
		cout << "Mass is" << mass << endl;
	}
};

void drawBoundary()
{
	#ifndef NO_GRAPHICS
	setfillstyle(SOLID_FILL, BOUND_COLOR);
	bar(0, 0, MAXX, DIFF);
	bar(0, MAXY - DIFF, MAXX, MAXY);
	bar(0, DIFF, DIFF, MAXY - DIFF);
	bar(MAXX - DIFF, DIFF, MAXX, MAXY - DIFF);
	#endif
}

void delay1(int n)
{
	for(int i=0; i<n; i++)
	{
		for(int j=0; j<n; j++)
		{
			cout << " " ;
			cout << "\b";
		}
	}
}

int main()
{
	// Initalisation Values
	short int halfL = 50;
	short int halfB = 80;
	Vector2D com(200, 300);
	Vector2D velocity(10.0, 10.0);
	Vector2D acc(0, 9.8);

	Rect abcd(&com, halfL, halfB);
	abcd.setKinametics(&velocity, &acc, 0, 0);

	Rect pqrs(&com, halfL + 25, halfB + 25);
	pqrs.setKinametics(&velocity, &acc, 1.57, 1.57 / 10);
	//for(int i=0; i<4; i++)
	//abcd.p[i].print();
	
	#ifndef NO_GRAPHICS
	initwindow(MAXX, MAXY);
	drawBoundary();
	#endif
	
	abcd.draw(DRAW_COLOR1);
	pqrs.draw(DRAW_COLOR2);

	

	while (1)
	{
		// update abcd
		abcd.draw(BACK_COLOR);

		abcd.updatePhysics();
		abcd.draw(DRAW_COLOR1);

		// update pqrs
		pqrs.draw(BACK_COLOR);

		pqrs.updatePhysics();
		pqrs.draw(DRAW_COLOR1);
		#ifdef NO_GRAPHICS
			delay1(4000) ;
		#else
			delay(10);
		#endif
	}

	return 0;
}
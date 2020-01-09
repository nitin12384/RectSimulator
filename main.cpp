//#include "source.cpp"
#include <stdio.h>
#include <graphics.h>
#include <math.h>


// MACRO and STRUCTURES

#define ABS(x) ((x>0)?(x):(-x))

#define DEF_COLOR GREEN
#define MAXX 1500
#define MAXY 900
#define DIFF 50 				// boundary difference
#define TIME_F 0.02			// in second
#define ANGV_MULT 0.027560		
#define ANGV_MULT_I 36.284470
#define DEG2RAD  0.0174532925    
#define RAD2DEG 57.2957795 

typedef struct vectStruct
{
	float x;
	float y;
}vector;

typedef struct rectStruct
{
	vector p[4] ;
	vector linV;
	vector linA;
	float angV;             // rad per second
	float angA;
	float mass;
	vector com;
	
}rect;



//Funtion Declarations

void drawRect(vector[],int) ;
rect initRect(vector[]);
void updateLinear(rect*) ;
void updateAngular(rect*) ;
void printfInfo(rect*) ;
bool updateIfCollision(rect*) ;
bool isStraight(rect*) ;
void rotateRect(rect*,float); // inpute angle in radian
void stabalizeRect(rect*);
float distBtwPoints(vector, vector);
//Function Deinations

void drawRect(vector pt[], int color = DEF_COLOR )
{
	setcolor(color);
	setfillstyle(SOLID_FILL, color);
	//vectors array
	int vectors[8] ;
	int i;
	for(i=0; i<8; i+=2)
	{
		vectors[i] = (int)(pt[i/2].x) ;
		vectors[i+1] = (int)(pt[i/2].y) ;
	}
	fillpoly(4,vectors);
	
}


rect initRect(vector pt[])
{
	rect abcd;
	
	for(int i=0; i<4; i++)
	{
		abcd.p[i].x = pt[i].x ;
		abcd.p[i].y = pt[i].y ;
		abcd.com.x += pt[i].x*0.25 ;
		abcd.com.y += pt[i].y*0.25 ;
	}
	
	abcd.angA = abcd.angV = 0;
	abcd.linA.x = 0.0;
	abcd.linA.y = 0.0;
	abcd.linV.x = 0.0;
	abcd.linV.y = 0.0;

	abcd.mass = 1.0;
	
	return abcd;
	
}

void updateLinear(rect *abcd)
{
	float disp;
	for(int i=0; i<4; i++)
	{
		disp = (*abcd).linV.x*TIME_F + (*abcd).linA.x*(TIME_F*TIME_F*0.5) ;  // calc displacement in 20 msec s = ut + 1/2(at^2)
		(*abcd).linV.x += (*abcd).linA.x*TIME_F ;               // change linear velocity wrt current acceleration every 20 sec
		(*abcd).p[i].x += disp;
		(*abcd).com.x += disp*0.25 ;						  // update centre of mass also 
		
		disp = (*abcd).linV.y*TIME_F + (*abcd).linA.y*(TIME_F*TIME_F*0.5) ;
		(*abcd).linV.y += (*abcd).linA.y*TIME_F ;
		(*abcd).p[i].y += disp;
		(*abcd).com.y += disp*0.25 ;
		
	}
}

void updateAngular(rect *abcd)
{
	float theta = (*abcd).angV*TIME_F + (*abcd).angA*(TIME_F*TIME_F*0.5) ;  // calc angular disp  theta = (omega)dt + 0.5(alpha)dt^2  (in radians)
	(*abcd).angV += (*abcd).angA*TIME_F ;
	//printf("theta is %f degree \n", theta*RAD2DEG);	
	rotateRect(abcd,theta);
	
}


void rotateRect(rect* abcd,float theta)
{
	double cosT = cos((double)theta);
	double sinT = sin((double)theta);
	float tempX,tempY;
	for(int i=0; i<4; i++)
	{
		tempX = (*abcd).p[i].x ;
		tempY = (*abcd).p[i].y ;
		(*abcd).p[i].x = (*abcd).com.x*(1-cosT) + (*abcd).com.y*sinT + tempX*cosT - tempY*sinT ; //updating coordinates
		(*abcd).p[i].y = (*abcd).com.y*(1-cosT) - (*abcd).com.x*sinT + tempY*cosT + tempX*sinT ; 
		
	}
	//updating com
	(*abcd).com.x = 0.0;
	(*abcd).com.y = 0.0;
	
	for(int i=0; i<4; i++)
	{
		(*abcd).com.x += (*abcd).p[i].x*0.25 ;
		(*abcd).com.y += (*abcd).p[i].y*0.25 ;
	}
}

void printfInfo(rect *abcd)
{
	for(int i=0; i<4; i++)
	{
		printf("%c - (%.0f,%.0f) ",65+i,(*abcd).p[i].x,(*abcd).p[i].y) ;
	}
	printf("\n");
	printf("Parameter-----------X-axis--------------Y-axis-------------- \n");
	printf("%-20s%-20.1f%-20.1f \n","Linear Velocity",(*abcd).linV.x,(*abcd).linV.y);
	printf("%-20s%-20.1f%-20.1f \n","Linear Acceleration",(*abcd).linA.x,(*abcd).linA.y);
	printf("%-20s%-20.1f%-20.1f \n","Center of Mass",(*abcd).com.x,(*abcd).com.y);
	printf("Angular velocity is %.1f(deg/sec) Angular acceleration is %.1f \n",(*abcd).angV*ANGV_MULT_I,(*abcd).angA*ANGV_MULT_I);
	printf("\n");
}

bool updateIfCollision(rect* abcd)
{
	bool isColliding=true;
	// Fake Collisions
	
	if((*abcd).com.x > 1500 )
		(*abcd).linV.x = -390;
	if((*abcd).com.y > 900)
		(*abcd).linV.y = -290;
			
	if((*abcd).com.x < 50)
		(*abcd).linV.x = 390;
	if((*abcd).com.y < 50)
		(*abcd).linV.y = 290;
	
	
	// find minX - l ,maxX - j ,minY- k ,maxY - i for rectangle 
	int    i,   j,   k,   l;
	int maxY,maxX,minY,minX;
	
	i = j = k = l = 0;
	maxY = minY = (*abcd).p[0].y ;
	maxX = minX = (*abcd).p[0].x ;
	for(int a=0; a<4; a++)
	{
		// for maxX
		if(maxX < (int)(*abcd).p[a].x )
		{
			maxX = (int)(*abcd).p[a].x ;
			j = a;
		}
		// for maxY
		if(maxY < (int)(*abcd).p[a].y )
		{
			maxY = (int)(*abcd).p[a].y ;
			i = a;
		}
		// for minX
		if(minX > (int)(*abcd).p[a].x )
		{
			minX = (int)(*abcd).p[a].x ;
			l = a;
		}
		// for minY
		if(minY < (int)(*abcd).p[a].y )
		{
			minY = (int)(*abcd).p[a].y ;
			k = a;
		}
	}
	
	
	
	//printf("maxX is %d and its coords (%d,%d)\n",(int)(*abcd).p[j].x , (int)(*abcd).p[j].x ,(int)(*abcd).p[j].y );
	//setcolor(WHITE) ;
	//setfillstyle(SOLID_FILL, WHITE);
	//fillellipse( (int)(*abcd).p[j].x  , (int)(*abcd).p[j].y , 5 ,5 )	 ;
	
	// Collision with walls		
	bool colWall[4] = {0,0,0,0} ;
	
	
	
	if(minY < 2*DIFF)
	{
		if(minX < DIFF)
		{
			//case i-i
			colWall[2] = colWall[3] = 1;
		}
		else if(maxX > MAXX - DIFF)
		{
			//case i-iii
			colWall[2] = colWall[1] = 1;
		}
		else
		{
			//case i-ii
			colWall[2] = 1;
		}
	}
	else if(maxY > MAXY - DIFF)
	{
		if(minX < DIFF)
		{
			//case iii-i
			colWall[0] = colWall[3] = 1;
		}
		else if(maxX > MAXX - DIFF)
		{
			//case iii-iii
			colWall[0] = colWall[1] = 1;
		}
		else
		{
			//case iii-ii
			colWall[0] = 1;
			//if((*abcd).linV.y <= 0)
				//isCollidong = 
		}
	}
	else
	{
		if(minX < DIFF)
		{
			//case ii-i
			colWall[3] = 1;
		}
		else if(maxX > MAXX - DIFF)
		{
			//case ii-iii
			colWall[1] = 1;
		}
		else
		{
			//case ii-ii
			//no collision
		}
	}
	isColliding = colWall[0] || colWall[1] || colWall[2] || colWall[3] ;
	
	//straight collisions
		
	if( isStraight(abcd) && colWall[0])
		(*abcd).linV.y = -ABS((*abcd).linV.y) ;
	if( isStraight(abcd) && colWall[1])
		(*abcd).linV.x = -ABS((*abcd).linV.x) ;
	if( isStraight(abcd) && colWall[2])
		(*abcd).linV.y = ABS((*abcd).linV.y) ;
	if( isStraight(abcd) && colWall[3])
		(*abcd).linV.x = ABS((*abcd).linV.x) ;
		
	//oblique collisions
	
	if(!isStraight )
	{
		float l,b,tempW;
		l = distBtwPoints((*abcd).p[i] , (*abcd).p[j] ) ;  // lenght
		b = distBtwPoints((*abcd).p[j] , (*abcd).p[k] ) ;  // breadth
		
		if(colWall[0])
		{
			float tempY = (*abcd).linV.y ;
			float k0 = (*abcd).p[j].x - (*abcd).p[i].x + (b/l)*( (*abcd).p[j].y - (*abcd).p[i].y ) ;
			
			(*abcd).linV.y = ( (6*k0*k0)-(2*l*l) - 2 );
			
			
		}
	
		
		
	}
	
	
	return isColliding;
}


bool isStraight(rect* abcd)
{
	return ( ((int)(*abcd).p[0].x == (int)(*abcd).p[1].x ) || ((int)(*abcd).p[0].y == (int)(*abcd).p[1].y) )  ;
}

void stabalizeRect(rect* abcd)
{
	
}

float distBtwPoints(vector a, vector b)
{
	float res;
	res = sqrt( (a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y) );
	return res ;
}

int main()
{
	
	vector pt[4] = { {60.0,60.0} ,{200.0,60.0} , {200.0,500.0} , {60.0,500.0}	};

	
	rect abcd = initRect(pt); //initialised
	abcd.linV.x = 250.0;			// pixel per sencond
	abcd.linV.y = 440.0;
	abcd.linA.x = 0 ;
	abcd.linA.y = 0 ;		   // gravity 9.8 pixel per second square
	abcd.angV = 90*DEG2RAD;  		// 90 degree per second
	abcd.angA = -10*DEG2RAD;
	rotateRect(&abcd,90*DEG2RAD);
	
//	printf("%f ",abcd.p[2].x) ;
	
	initwindow(MAXX,MAXY);
	
	drawRect(abcd.p,BLUE);
	
	//main animation
	
	int cont = 1;
	int straightCount = 1;
	int colCount = 0;
		  						// why problem came on declaring char msg[100]***********?????????????????????????????????????**********************
	bool isColliding = 0; 		// 1 if collision is happening 0 if not
	int frames = 0;
	setfillstyle(SOLID_FILL,LIGHTGRAY);
	bar(DIFF,MAXY-DIFF,MAXX-DIFF,MAXY);
	bar(0,0,MAXX,DIFF);
	while(cont)
	{
		frames++;  						//Counting no of frames passed
		
		delay(TIME_F*1000);
	//	printf("Time passed %.2fsec\n",frames*0.02);
	//	sprintf(msg,"Time passed %.2fseconds",frames*0.02);
	//	settextstyle(DEFAULT_FONT, HORIZ_DIR,2);
	//	outtextxy(1100,20,"msg");
		drawRect(abcd.p,BLACK);        // erasing cureent rectangle
		// updating coordintaes
		
		updateLinear(&abcd);			
		updateAngular(&abcd);
		straightCount += isStraight(&abcd) ;
		
		isColliding = updateIfCollision(&abcd) ;
		if (isColliding)
		{
			colCount++;
			printf("Collision with wall *************************** \n");
			//setfillstyle(SOLID_FILL, RED);
			//fillellipse( 500,500, 100,100 );
		}
			
		
		if(frames%10==0)
			printfInfo(&abcd);			// printing info on console screen
		//printf("straight for %d time \n",straightCount);
		printf("collision for %d time \n",colCount);
		drawRect(abcd.p,BLUE);			// draw new rectangle
	}
	
	
	
	getch() ;
	
	return 0;
}

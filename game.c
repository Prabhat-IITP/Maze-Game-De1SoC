/* set a single pixel on the screen at x,y
 * x in [0,319], y in [0,239], and colour in [0,65535]
 */
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define JTAG_UART_BASE ((volatile int*) 0xFF201000)
#define JTAG_UART_CONTROL ((volatile int*) (0xFF201000+4))
short CHAR_COL = 0xF800;
#define M_PI   3.14159265358979323846264338327950288
#define n 20
short GREEN = 0xff00;
void write_pixel(int x, int y, short colour) {
  volatile short *vga_addr=(volatile short*)(0x08000000 + (y<<10) + (x<<1));
  *vga_addr=colour;
}
short read_pixel(int x, int y) {
  volatile short *vga_addr=(volatile short*)(0x08000000 + (y<<10) + (x<<1));
  return *vga_addr;
}
/* write a single character to the character buffer at x,y
 * x in [0,79], y in [0,59]
 */
void write_char(int x, int y, char c) {
  // VGA character buffer
  volatile char * character_buffer = (char *) (0x09000000 + (y<<7) + x);
  *character_buffer = c;
}
void write_string(int x,int y, char s[])
{
    int a=x; int b=y;
    for(int i=0;i<strlen(s);i++)
    {
        write_char(a,b,s[i]);
    a++;
}
}

/* use write_pixel to set entire screen to black (does not clear the character buffer) */
void clear_screen() {
  int x, y;
  for (x = 0; x < 320; x++) {
    for (y = 0; y < 240; y++) {
    write_pixel(x,y,0);
  }
  }
	
  for (x = 0; x < 80; x++) {
    for (y = 0; y < 60; y++) {
    write_char(x,y,(char)0);
  }
  }

}

void clear_pixel() {
  int x, y;
  for (x = 0; x < 320; x++) {
    for (y = 0; y < 240; y++) {
    write_pixel(x,y,0);
  }
  }
}
void clear_char() {
  int x, y;
  for (x = 0; x < 80; x++) {
    for (y = 0; y < 60; y++) {
    write_char(x,y,(char)0);
  }
  }
  
}
short rgbconv(int r, int g, int b) {
    short color = ((r & 0x1F) << 11) | ((g & 0x3F) << 5) | (b & 0x1F);
    return color;
}

double sqrt(double x)
{
  double temp,ans;
  ans=x/2;
  temp=0;
  while(temp!=ans)
  {
    temp=ans;
    ans=(x/temp + temp)/2;
  }
  return ans;

}

double sin(double x) //sin calculation
{
double sum;
double fa;
double pow;
sum = 0.0;
for(int i = 0; i <= n; i++)
{
fa = 1.0;
pow = 1.0;
for(int j = 1; j <= 2*i+1; j++)
{
fa *= j;
pow *= x;
}
sum += ((i%2?-1.0:1.0)/fa)*pow;
}
return sum;
}

double cos(double x) //cosine calculation
{
double sum;
double fa;
double pow;
sum = 0.0;
for(int i = 0; i <= n; i++)
{
fa = 1.0;
pow = 1.0;
for(int j = 1; j <= 2*i; j++)
{
fa *= j;
pow *= x;
}
sum += ((i%2?-1.0:1.0)/fa)*pow;
}
return sum;
}

/*This function takes in the coordinates of the top-left and bottom-right 
corners of the rectangle, and the desired colour to draw the rectangle with.*/
void draw_rectangle(int x1, int y1, int x2, int y2, short colour) {
  int x, y;
  // Draw the horizontal lines
  for (x = x1; x <= x2; x++) {
    write_pixel(x, y1, colour);
    write_pixel(x, y2, colour);
  }
  // Draw the vertical lines
  for (y = y1; y <= y2; y++) {
    write_pixel(x1, y, colour);
    write_pixel(x2, y, colour);
  }
}

void helpcirc(int xc, int yc, int x, int y, short c)
{
    write_pixel(xc+x, yc+y,c);
    write_pixel(xc-x, yc+y, c);
    write_pixel(xc+x, yc-y,c);
    write_pixel(xc-x, yc-y, c);
  
    write_pixel(xc+y, yc+x, c);
    write_pixel(xc-y, yc+x, c);
    write_pixel(xc+y, yc-x, c);
    write_pixel(xc-y, yc-x, c);
   

}
 
// Function for circle-generation
// using Bresenham's algorithm
void draw_circle(int xc, int yc, int r, short colour)
{
  short c = colour;
    int x = 0, y = r;
    int d = 3 - 2 * r;
    helpcirc(xc, yc, x, y, c);
    while (y >= x)
    {
        x++;
        if (d > 0)
        {
            y--;
            d = d + 4 * (x - y) + 10;
        }
        else
            d = d + 4 * x + 6;
        helpcirc(xc, yc, x, y, c);
        
    }
}

/* Draw a line on the screen from (x0, y0) to (x1, y1) 
The draw_line function takes the starting and ending coordinates of the line, 
as well as the desired color for the line. 
It uses the Bresenham's line algorithm to plot pixels along the line.*/
void draw_line(int x1, int y1, int x2, int y2,   short colour)
{
  int dx;
  int dy;
  int decide;
  int temp;
  if(x1>x2) dx=x1-x2; else dx=x2-x1;
  if(y1>y2) dy=y1-y2; else dy=y2-y1;
  if(dx>dy) decide=0; else decide=1;
  if(decide)  {
    temp=dx;
    dx=dy;
    dy=temp;
  }
  //decide is just for slope

    int pk = 2 * dy - dx;
    for (int i = 0; i <= dx; i++) {

        x1 < x2 ? x1++ : x1--;
        if (pk < 0) {
    
            if (decide == 0) {
               write_pixel(x1, y1, colour );
                pk = pk + 2 * dy;
            }
            else {
                //(y1,x1) is passed in xt
                write_pixel(y1, x1, colour );
                pk = pk + 2 * dy;
            }
        }
        else {
            y1 < y2 ? y1++ : y1--;
            if (decide == 0) {
 
              write_pixel(x1, y1, colour );
            }
            else {
               write_pixel(y1, x1, colour );
            }
            pk = pk + 2 * dy - 2 * dx;
        }
    }
}

/*This function takes in the coordinates of the three vertices of the triangle
, and the desired colour to draw the triangle with.*/
void draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3, short colour) {
  // Draw the three sides of the triangle
  draw_line(x1, y1, x2, y2, colour);
  draw_line(x2, y2, x3, y3, colour);
  draw_line(x3, y3, x1, y1, colour);
}
void draw_line_gupta(int x1, int y1, int x2, int y2, short colour) 
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int two_dx = 2 * dx;
    int two_dy = 2 * dy;
    int p = two_dy - dx;
    int x = x1, y = y1;
    int ix = x1 < x2 ? 1 : -1;
    int iy = y1 < y2 ? 1 : -1;
    if (dx >= dy) 
  {
        while (x != x2) 
    {
            write_pixel(x, y, colour);
            if (p >= 0)
      {
                y += iy;
                p -= two_dx;
            }
            p += two_dy;
            x += ix;
        }
    }
  else 
  {
        while (y != y2)
    {
            write_pixel(x, y, colour);
            if (p >= 0) 
      {
                x += ix;
                p -= two_dy;
          }
            p += two_dx;
            y += iy;
        }
    }
    write_pixel(x, y, colour);
}
char get_jtag(volatile int *JTAG_UART_ptr) {
  int data;
  data = *(JTAG_UART_ptr);
  if (data & 0x00008000) // check RVALID
    return ((char)data & 0xFF);
  else
    return ('\0');
}

//print user text to console
void put_jtag(volatile int *JTAG_UART_ptr, char c) {
  int control;
  control = *(JTAG_UART_ptr + 1); // read control reg
  if (control & 0xFFFF0000) {
    *(JTAG_UART_ptr) = c;
  }
}
void ssleep(int time)
{
  int x;
  for(x=0;x<time;x++) {}
}
void home_screen();
void draw_ellipse(int xc, int yc, int a, int b, short color) {
    int x = 0, y = b;
    int a_sqr = a * a;
    int b_sqr = b * b;
    int d = b_sqr + a_sqr * (0.25 - b);

    // loop for upper half
    while (b_sqr * (x + 1) < a_sqr * (y - 0.5)) {
        // fill upper half
        for (int i = xc - x; i <= xc + x; i++) {
            write_pixel(i, yc + y, color);
            write_pixel(i, yc - y, color);
        }

        // move to next point
        if (d < 0) {
            d += b_sqr * (2 * x + 3);
        } else {
            d += (b_sqr * (2 * x + 3) + a_sqr * (-2 * y + 2));
            y--;
        }
        x++;
    }

    // loop for lower half
    d = b_sqr * (x + 0.5) * (x + 0.5) + a_sqr * (y - 1) * (y - 1) - a_sqr * b_sqr;
    while (y >= 0) {
        // fill lower half
        for (int i = xc - x; i <= xc + x; i++) {
            write_pixel(i, yc + y, color);
            write_pixel(i, yc - y, color);
        }

        // move to next point
        if (d < 0) {
            d += b_sqr * (2 * x + 2) + a_sqr * (-2 * y + 3);
            x++;
        } else {
            d += a_sqr * (-2 * y + 3);
        }
        y--;
    }
}
void circle(int x0, int y0, int radius, short color)
{
	int x, y;
	for (y = -radius; y <= radius; y++)
	{
		for (x = -radius; x <= radius; x++)
		{
			if (x * x + y * y <= radius * radius)
			{
				write_pixel(x0 + x, y0 + y, color);
				
			}
		}
	}

}
// Define the colors
#define BORDER_COLOR 0xff00
#define BACKGROUND_COLOR 0x0000
#define CLEARED_COLOR 0x00ff
#define UNCLEARED_COLOR 0xffff
#define LEVEL_BOX_SIZE 50
#define LEVEL_BOX_PADDING 12

// Colors
#define LEVEL_BOX_COLOR 0xF000
#define CLEARED_LEVEL_BOX_COLOR 0xff70

// Define the level screen dimensions
#define LEVEL_SCREEN_WIDTH 320
#define LEVEL_SCREEN_HEIGHT 240
#define LEVEL_SCREEN_BORDER_SIZE 5
#define LEVEL_SCREEN_BOX_SIZE 50
#define LEVEL_SCREEN_BOX_GAP 10

// Define the number of levels
#define NUM_LEVELS 12
#define GREEN 0x0ff0
#define BLUE 0xff00
#define WHITE 0xffff
#define YELLOW 0xff70
  #define RED 0xF800
#define BLACK 0x0000
// Define the cleared levels array
int cleared[NUM_LEVELS] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int coins = 0;
int levelss = 0;
char* inttos(int num) {
    int temp = num;
    int length = 1;
    while (temp /= 10) {
        length++;
    }

    char* str = (char*)malloc(length + 1);
    str[length] = '\0';

    while (length--) {
        str[length] = num % 10 + '0';
        num /= 10;
    }

    return str;
}
draw_vert(int x1,int y1,int l, short colour)
{
	int i;
	for (i=0;i<l;i++)
		write_pixel(x1,y1+i,colour);
}
draw_hori(int x1,int y1,int l, short colour)
{
	int i;
	for (i=0;i<l;i++)
		write_pixel(x1+i,y1,colour);
}

void vert(int x,int y)
{
	draw_vert(x,y,10,GREEN);	
}

void hori(int x,int y)
{
	draw_hori(x,y,10,GREEN);
}

void draw_hort(int y,int x,int l, short col)
{
	draw_hori(x+20,y,l,col);
}


void portal2(int x1,int y1,short colour)
{
	draw_circle(x1,y1-6,5,colour);
	draw_circle(x1,y1-6,3,colour);
	draw_circle(x1,y1-6,1,colour);
}

void lvl3()
{
	draw_vert(40,20,200,GREEN);
	draw_vert(240,20,200,GREEN);
	draw_hori(40,20,200,GREEN);
	draw_hori(40,220,200,GREEN);
	vert(50,20);
	vert(50,30);
	vert(50,50);
	vert(50,60);
	vert(50,70);
	vert(50,100);
	vert(50,110);
	vert(50,120);
	vert(50,140);
	vert(50,200);
	
	vert(60,30);
	vert(60,40);
	vert(60,60);
	vert(60,90);
	vert(60,100);
	vert(60,120);
	vert(60,130);
	vert(60,160);
	vert(60,170);
	vert(60,190);
	vert(60,200);
	vert(60,210);
	
	vert(70,40);
	vert(70,50);
	vert(70,80);
	vert(70,90);
	vert(70,110);
	vert(70,140);
	vert(70,150);
	vert(70,160);
	vert(70,200);
	vert(70,210);
	
	draw_vert(80,30,70,GREEN);
	draw_vert(80,120,30,GREEN);
	vert(80,170);
	
	vert(90,70);
	vert(90,100);
	vert(90,110);
	vert(90,140);
	vert(90,160);
	vert(90,190);
	vert(90,200);
	
	draw_vert(100,20,30,GREEN);
	vert(100,80);
	vert(100,90);
	vert(100,120);
	vert(100,130);
	vert(100,180);
	vert(100,190);
	vert(100,210);
	
	draw_vert(110,50,30,GREEN);
	draw_vert(110,180,30,GREEN);
	vert(110,90);
	
	vert(120,20);
	vert(120,40);
	vert(120,50);
	vert(120,70);
	vert(120,80);
	draw_vert(120,100,30,GREEN);
	draw_vert(120,150,50,GREEN);
	
	vert(130,40);
	vert(130,60);
	vert(130,70);
	vert(130,90);
	vert(130,200);
	
	draw_vert(140,40,30,GREEN);
	draw_vert(140,100,20,GREEN);
	draw_vert(140,140,20,GREEN);
	draw_vert(140,180,10,GREEN);
	draw_vert(140,200,20,GREEN);
	
	draw_vert(150,30,30,GREEN);
	draw_vert(150,70,80,GREEN);
	draw_vert(150,160,20,GREEN);
	draw_vert(150,210,10,GREEN);
	
	draw_vert(160,20,20,GREEN);
	draw_vert(160,60,10,GREEN);
	draw_vert(160,80,10,GREEN);
	draw_vert(160,120,30,GREEN);
	draw_vert(160,170,20,GREEN);
	draw_vert(160,200,10,GREEN);
	
	
	draw_vert(170,30,20,GREEN);
	draw_vert(170,60,50,GREEN);
	draw_vert(170,150,10,GREEN);
	draw_vert(170,170,10,GREEN);
	
	draw_vert(180,20,10,GREEN);
	draw_vert(180,50,20,GREEN);
	draw_vert(180,90,10,GREEN);
	draw_vert(180,130,10,GREEN);
	draw_vert(180,160,10,GREEN);
	draw_vert(180,180,30,GREEN);
	
	draw_vert(190,30,10,GREEN);
	draw_vert(190,50,10,GREEN);
	draw_vert(190,100,30,GREEN);
	draw_vert(190,140,40,GREEN);
	draw_vert(190,190,30,GREEN);
	
	draw_vert(200,50,20,GREEN);
	draw_vert(200,80,20,GREEN);
	draw_vert(200,110,10,GREEN);
	draw_vert(200,130,10,GREEN);
	draw_vert(200,150,10,GREEN);
	draw_vert(200,180,10,GREEN);
	draw_vert(200,200,20,GREEN);
	
	draw_vert(210,30,20,GREEN);
	draw_vert(210,60,10,GREEN);
	draw_vert(210,80,10,GREEN);
	draw_vert(210,100,10,GREEN);
	draw_vert(210,120,20,GREEN);
	draw_vert(210,160,20,GREEN);
	draw_vert(210,190,10,GREEN);
	
	draw_vert(220,30,30,GREEN);
	draw_vert(220,70,30,GREEN);
	draw_vert(220,110,30,GREEN);
	draw_vert(220,180,10,GREEN);
	draw_vert(220,200,10,GREEN);
	
	draw_vert(230,60,10,GREEN);
	draw_vert(230,100,10,GREEN);
	draw_vert(230,130,50,GREEN);
	draw_vert(230,190,10,GREEN);
	
	
	
	draw_hort(30,50,20,GREEN);
	draw_hort(30,90,10,GREEN);
	draw_hort(30,110,20,GREEN);
	draw_hort(30,150,10,GREEN);
	draw_hort(30,170,20,GREEN);
	draw_hort(30,200,10,GREEN);
	
	draw_hort(40,40,10,GREEN);
	draw_hort(40,70,10,GREEN);
	draw_hort(40,90,10,GREEN);
	draw_hort(40,110,10,GREEN);
	draw_hort(40,160,20,GREEN);
	draw_hort(40,210,10,GREEN);
	
	draw_hort(50,20,20,GREEN);
	draw_hort(50,60,10,GREEN);
	draw_hort(50,130,30,GREEN);
	draw_hort(50,170,40,GREEN);
	
	draw_hort(60,60,30,GREEN);
	draw_hort(60,100,10,GREEN);
	draw_hort(60,120,10,GREEN);
	draw_hort(60,140,10,GREEN);
	draw_hort(60,200,10,GREEN);
	
	draw_hort(70,40,20,GREEN);
	draw_hort(70,70,10,GREEN);
	draw_hort(70,90,10,GREEN);
	draw_hort(70,130,10,GREEN);
	draw_hort(70,160,20,GREEN);
	draw_hort(70,190,10,GREEN);
	
	draw_hort(80,30,20,GREEN);
	draw_hort(80,70,10,GREEN);
	draw_hort(80,110,10,GREEN);
	draw_hort(80,150,30,GREEN);
	draw_hort(80,200,10,GREEN);
	
	draw_hort(90,20,20,GREEN);
	draw_hort(90,60,10,GREEN);
	draw_hort(90,100,40,GREEN);
	draw_hort(90,160,10,GREEN);
	draw_hort(90,190,10,GREEN);
	draw_hort(90,210,10,GREEN);
	
	
	draw_hort(100,70,20,GREEN);
	draw_hort(100,110,10,GREEN);
	draw_hort(100,140,20,GREEN);
	draw_hort(100,170,20,GREEN);
	draw_hort(100,200,10,GREEN);
	
	draw_hort(110,40,30,GREEN);
	draw_hort(110,80,30,GREEN);
	draw_hort(110,140,10,GREEN);
	draw_hort(110,160,10,GREEN);
	draw_hort(110,190,10,GREEN);
	draw_hort(110,210,10,GREEN);
	
	draw_hort(120,60,30,GREEN);
	draw_hort(120,110,10,GREEN);
	draw_hort(120,140,20,GREEN);
	draw_hort(120,180,10,GREEN);
	draw_hort(120,200,10,GREEN);
	
	draw_hort(130,20,10,GREEN);
	draw_hort(130,40,20,GREEN);
	draw_hort(130,70,10,GREEN);
	draw_hort(130,90,40,GREEN);
	draw_hort(130,150,30,GREEN);
	
	draw_hort(140,60,10,GREEN);
	draw_hort(140,80,40,GREEN);
	draw_hort(140,140,10,GREEN);
	draw_hort(140,170,10,GREEN);
	draw_hort(140,190,10,GREEN);
	
	draw_hort(150,30,20,GREEN);
	draw_hort(150,70,10,GREEN);
	draw_hort(150,90,20,GREEN);
	draw_hort(150,130,10,GREEN);
	draw_hort(150,150,20,GREEN);
	draw_hort(150,180,30,GREEN);
	
	draw_hort(160,30,10,GREEN);
	draw_hort(160,50,40,GREEN);
	draw_hort(160,110,20,GREEN);
	draw_hort(160,140,20,GREEN);	
	draw_hort(160,180,20,GREEN);
	
	
	draw_hort(170,20,10,GREEN);
	draw_hort(170,70,30,GREEN);
	draw_hort(170,110,20,GREEN);
	draw_hort(170,140,10,GREEN);
	draw_hort(170,170,10,GREEN);
	draw_hort(170,200,10,GREEN);
	
	draw_hort(180,20,50,GREEN);
	draw_hort(180,100,20,GREEN);
	draw_hort(180,160,10,GREEN);
	draw_hort(180,180,20,GREEN);
	
	draw_hort(190,30,40,GREEN);
	draw_hort(190,100,10,GREEN);
	draw_hort(190,120,40,GREEN);
	draw_hort(190,170,10,GREEN);
	draw_hort(190,190,10,GREEN);
	draw_hort(190,210,10,GREEN);
	
	draw_hort(200,20,10,GREEN);
	draw_hort(200,50,10,GREEN);
	draw_hort(200,70,10,GREEN);
	draw_hort(200,110,40,GREEN);
	draw_hort(200,200,10,GREEN);
	
	draw_hort(210,60,10,GREEN);
	draw_hort(210,100,10,GREEN);
	draw_hort(210,140,10,GREEN);
	draw_hort(210,190,10,GREEN);
	draw_hort(210,210,10,GREEN);
	portal2(145,220,WHITE);
	
	

}

void lvl2()
{
	draw_hori(40,10,220,GREEN);
	draw_hori(40,230,220,GREEN);
	draw_vert(40,10,220,GREEN);
	draw_vert(260,10,220,GREEN);
	
	draw_hort(50,60,20,GREEN);
	draw_hort(50,120,40,GREEN);
	draw_hort(50,200,40,GREEN);
	
	draw_hort(70,40,20,GREEN);
	draw_hort(70,140,80,GREEN);
	
	draw_hort(90,20,20,GREEN);
	draw_hort(90,80,20,GREEN);
	draw_hort(90,120,20,GREEN);
	draw_hort(90,160,40,GREEN);
	
	draw_hort(110,20,60,GREEN);
	draw_hort(110,100,20,GREEN);
	draw_hort(110,140,40,GREEN);
	
	draw_hort(130,40,20,GREEN);
	draw_hort(130,120,40,GREEN);
	draw_hort(130,180,40,GREEN);
	
	draw_hort(150,100,40,GREEN);
	draw_hort(150,160,40,GREEN);
	draw_hort(150,220,20,GREEN);
	
	draw_hort(170,60,40,GREEN);
	draw_hort(170,140,40,GREEN);

	draw_hort(190,140,20,GREEN);
	draw_hort(190,200,20,GREEN);
	
	draw_hort(210,40,20,GREEN);
	draw_hort(210,80,40,GREEN);
	draw_hort(210,140,40,GREEN);
	
	draw_vert(60,30,40,GREEN);
	draw_vert(60,130,80,GREEN);
	
	draw_vert(80,30,20,GREEN);
	draw_vert(80,70,40,GREEN);
	draw_vert(80,130,20,GREEN);
	draw_vert(80,190,40,GREEN);
	
	draw_vert(100,10,20,GREEN);
	draw_vert(100,50,40,GREEN);
	draw_vert(100,110,40,GREEN);
	draw_vert(100,190,20,GREEN);
	
	draw_vert(120,10,180,GREEN);
	
	draw_vert(140,30,60,GREEN);
	draw_vert(140,170,60,GREEN);
	
	draw_vert(160,10,20,GREEN);
	draw_vert(160,90,40,GREEN);
	draw_vert(160,150,20,GREEN);
	draw_vert(160,190,20,GREEN);
	
	draw_vert(180,10,40,GREEN);
	draw_vert(180,70,20,GREEN);
	draw_vert(180,130,20,GREEN);
	
	draw_vert(200,30,40,GREEN);
	draw_vert(200,170,40,GREEN);
	
	draw_vert(220,30,20,GREEN);
	draw_vert(220,90,20,GREEN);
	draw_vert(220,130,80,GREEN);
	
	draw_vert(240,10,20,GREEN);
	draw_vert(240,70,60,GREEN);
	draw_vert(240,170,20,GREEN);
	draw_vert(240,210,20,GREEN);
	portal2(250,220,WHITE);
}
int x[3]={0,0,0};
void lvl1()
{
	draw_hori(40,20,200,GREEN);
	draw_hori(40,220,200,GREEN);
	draw_vert(40,20,200,GREEN);
	draw_vert(240,20,200,GREEN);
	
	draw_hort(40,20,80,GREEN);
	draw_hort(40,120,80,GREEN);
	
	draw_hort(60,40,40,GREEN);
	draw_hort(60,140,40,GREEN);
	
	draw_hort(80,40,20,GREEN);
	draw_hort(80,180,40,GREEN);
	
	draw_hort(100,60,20,GREEN);
	draw_hort(100,100,40,GREEN);
	draw_hort(100,160,40,GREEN);
	
	draw_hort(120,40,20,GREEN);
	draw_hort(120,80,20,GREEN);
	
	draw_hort(140,20,20,GREEN);
	draw_hort(140,60,20,GREEN);
	draw_hort(140,120,40,GREEN);
	draw_hort(140,200,20,GREEN);
	
	draw_hort(160,40,20,GREEN);
	draw_hort(160,80,80,GREEN);
	
	draw_hort(180,20,60,GREEN);
	draw_hort(180,160,40,GREEN);
	
	draw_hort(200,40,60,GREEN);
	draw_hort(200,120,40,GREEN);
	draw_hort(200,180,20,GREEN);
	
	draw_vert(60,60,20,GREEN);
	draw_vert(60,100,20,GREEN);
	
	draw_vert(80,80,20,GREEN);
	draw_vert(80,120,40,GREEN);
	
	draw_vert(100,60,20,GREEN);
	draw_vert(100,100,20,GREEN);
	draw_vert(100,140,20,GREEN);
	
	draw_vert(120,40,40,GREEN);
	draw_vert(120,100,40,GREEN);
	draw_vert(120,160,60,GREEN);
	
	draw_vert(140,40,60,GREEN);
	draw_vert(140,120,20,GREEN);
	draw_vert(140,180,20,GREEN);
	
	draw_vert(160,60,20,GREEN);
	draw_vert(160,100,20,GREEN);
	draw_vert(160,160,20,GREEN);
	
	draw_vert(180,60,100,GREEN);
	draw_vert(180,180,20,GREEN);

	draw_vert(200,100,80,GREEN);
	draw_vert(200,200,20,GREEN);
	
	draw_vert(220,40,40,GREEN);
	draw_vert(220,120,20,GREEN);
	draw_vert(220,160,20,GREEN);
	portal2(230,210,WHITE);
}
void leveleasy();
void levelmedium();
void levelhard();
void draw_level_screen();
void levelhard()
{
short redd = CHAR_COL;
	clear_screen();
	volatile int * JTAG_UART_ptr = (int *) JTAG_UART_BASE;
	int i;
    char c;
	lvl3();
	circle(145,25,3,redd);
//circle(145,25,3,GREEN);
	int xc=145,yc=25;
	while(1)
	{
		c=get_jtag(JTAG_UART_ptr);
		if(c==0x64)
		{
			int flag=0;
			int j=yc;
			for(i=xc+5;i<xc+7;i++) if(read_pixel(i,j)==GREEN) flag=1;
			if(!flag)
			{
			circle(xc,yc,3,BLACK);
			xc+=10;
			circle(xc,yc,3,redd);
				
			}
		}
		if(c==0x61)
		{
			int flag=0;
			int j=yc;
			for(i=xc-5;i>xc-7;i--) if(read_pixel(i,j)==GREEN) flag=1;
			if(!flag)
			{
			circle(xc,yc,3,BLACK);
			xc-=10;
			circle(xc,yc,3,redd);
				
			}
		}
		if(c==0x77)
		{
			int flag=0;
			int j=xc;
			for(i=yc-5;i>yc-7;i--) if(read_pixel(j,i)==GREEN) flag=1;
			if(!flag)
			{
			circle(xc,yc,3,BLACK);
			yc-=10;
			circle(xc,yc,3,redd);	
			}
		}
		if(c==0x73)
		{
			int flag=0;
			int j=xc;
			for(i=yc+5;i<yc+7;i++) if(read_pixel(j,i)==GREEN) flag=1;
			if(!flag)
			{
			circle(xc,yc,3,BLACK);
			yc+=10;
			circle(xc,yc,3,redd);	
			}	
		}
		if(xc==145 && yc>210) x[2]++,draw_level_screen(); 
		if (c == 0x1b) { // proceed to level screen only when 'L' is received
    draw_level_screen();
  
    break; // or do something else here
  }
	} 
}
void leveleasy()
{
short redd = CHAR_COL;
	clear_screen();
	volatile int * JTAG_UART_ptr = (int *) JTAG_UART_BASE;
	int i;
    char c;
	lvl1();
	int m=20;int xc=50,yc=30;
	circle(xc,yc,6,redd);
//circle(145,25,3,GREEN);
	
	while(1)
	{
		c=get_jtag(JTAG_UART_ptr);
		if(c==0x64)
		{
			int flag=0;
			int j=yc;
			for(i=xc+8;i<xc+12;i++) if(read_pixel(i,j)==GREEN) flag=1;
			if(!flag)
			{
			circle(xc,yc,6,BLACK);
			xc+=m;
			circle(xc,yc,6,redd);
				
			}
		}
		if(c==0x61)
		{
			int flag=0;
			int j=yc;
			for(i=xc-8;i>xc-12;i--) if(read_pixel(i,j)==GREEN) flag=1;
			if(!flag)
			{
			circle(xc,yc,6,BLACK);
			xc-=m;
			circle(xc,yc,6,redd);
				
			}
		}
		if(c==0x77)
		{
			int flag=0;
			int j=xc;
			for(i=yc-8;i>yc-12;i--) if(read_pixel(j,i)==GREEN) flag=1;
			if(!flag)
			{
			circle(xc,yc,6,BLACK);
			yc-=m;
			circle(xc,yc,6,redd);	
			}
		}
		if(c==0x73)
		{
			int flag=0;
			int j=xc;
			for(i=yc+8;i<yc+12;i++) if(read_pixel(j,i)==GREEN) flag=1;
			if(!flag)
			{
			circle(xc,yc,6,BLACK);
			yc+=m;
			circle(xc,yc,6,redd);	
			}	
		}
		if(xc==230 && yc>200) x[0]++,draw_level_screen(); 
		if (c == 0x1b) { // proceed to level screen only when 'L' is received
    draw_level_screen();
  
    break; // or do something else here
  }
	} 
}
void levelmedium()
{
short redd = CHAR_COL;
	clear_screen();
	volatile int * JTAG_UART_ptr = (int *) JTAG_UART_BASE;
	int i;
    char c;
	lvl2();
	int xc=50,yc=20;
	circle(xc,yc,6,redd);
//circle(145,25,3,GREEN);
	
	while(1)
	{
		c=get_jtag(JTAG_UART_ptr);
		if(c==0x64)
		{
			int flag=0;
			int j=yc;
			for(i=xc+8;i<xc+12;i++) if(read_pixel(i,j)==GREEN) flag=1;
			if(!flag)
			{
			circle(xc,yc,6,BLACK);
			xc+=20;
			circle(xc,yc,6,redd);
				
			}
		}
		if(c==0x61)
		{
			int flag=0;
			int j=yc;
			for(i=xc-8;i>xc-12;i--) if(read_pixel(i,j)==GREEN) flag=1;
			if(!flag)
			{
			circle(xc,yc,6,BLACK);
			xc-=20;
			circle(xc,yc,6,redd);
				
			}
		}
		if(c==0x77)
		{
			int flag=0;
			int j=xc;
			for(i=yc-8;i>yc-12;i--) if(read_pixel(j,i)==GREEN) flag=1;
			if(!flag)
			{
			circle(xc,yc,6,BLACK);
			yc-=20;
			circle(xc,yc,6,redd);	
			}
		}
		if(c==0x73)
		{
			int flag=0;
			int j=xc;
			for(i=yc+8;i<yc+12;i++) if(read_pixel(j,i)==GREEN) flag=1;
			if(!flag)
			{
			circle(xc,yc,6,BLACK);
			yc+=20;
			circle(xc,yc,6,redd);	
			}	
		}
		if(xc==250 && yc>200) x[1]++,draw_level_screen(); 
		  if (c == 0x1b) { // proceed to level screen only when 'L' is received
    draw_level_screen();
  
    break; // or do something else here
  }
	} 
}
void draw_rec(int x, int y, int width, int height, short color) {
  int i, j;
  for (i = x; i < x + width; i++) {
    for (j = y; j < y + height; j++) {
      write_pixel(i, j, color);
    }
  }
}
void redg_rect(int x, int y, int width, int height, int r, int g, int b) {
  int i, j;
  for (i = x; i < x + width; i++) {
    for (j = y; j < y + height; j++) {
      int red_value = (int) ((float) (i - x) / (float) width * 31.0);
      short color = ((red_value << 11) | (g << 5) | b);
      write_pixel(i, j, color);
    }
  }
}
void greeng_rect(int x, int y, int width, int height, int r, int g, int b) {
  int i, j;
  for (i = x; i < x + width; i++) {
    for (j = y; j < y + height; j++) {
      int green_value = (int) ((float) (i - x) / (float) width * 63.0);
      short color = ((r << 11) | (green_value << 5) | b);
      write_pixel(i, j, color);
    }
  }
}
void blueg_rect(int x, int y, int width, int height, int r, int g, int b) {
  int i, j;
  for (i = x; i < x + width; i++) {
    for (j = y; j < y + height; j++) {
      int blue_value = (int) ((float) (i - x) / (float) width * 31.0);
      short color = ((r << 11) | (g << 5) | blue_value);
      write_pixel(i, j, color);
    }
  }
}



int curr_r=20;
int curr_b=30;
int curr_g=10;

void draw_level_screen() {
       
	clear_screen();
	volatile int * JTAG_UART_ptr = (int *) JTAG_UART_BASE;
	int k=0,levelss=0;
	for(k=0;k<3;k++) if(x[k]>0) levelss++; 
	int i;
    char c;
    for( i = 30; i <= 290; i++) {
        write_pixel(i, 210, 0xff00);
        write_pixel(i, 30, 0xff00);
    }
    for( i = 30; i <= 210; i++) {
        write_pixel(30, i,0xff00);
        write_pixel(290, i, 0xff00);
    }
 	write_string(22,18,"Press 1 for easy, 2 for Medium, 3 for hard ");
	
	write_string(30,21,"Levels Cleared : ");
	write_string(47,21,inttos(levelss));
	write_string(30,27,"Press Q to Quit");
	write_string(16,40,"Red->R+ T-");
	write_string(34,40,"Green->G+ H-");
	write_string(52,40,"Blue->B+ N-");
	write_string(28,48,"Character Color: ");
	draw_rec(105,190,100,10,rgbconv(curr_r,curr_g,curr_b));
	redg_rect(65,170,62,10,curr_r,curr_g,curr_b);
	
	greeng_rect(135,170,62,10,curr_r,curr_g,curr_b);
	
	blueg_rect(205,170,62,10,curr_r,curr_g,curr_b);
	
while (1) {
  c = get_jtag(JTAG_UART_ptr);
  if (c == 0x71) { // proceed to level screen only when 'L' is received
   
	  home_screen();
  
    break; // or do something else here
  }
if (c == 0x31) { // proceed to level screen only when 'L' is received
    leveleasy();
  
    break; // or do something else here
  }
if (c == 0x32) { // proceed to level screen only when 'L' is received
    levelmedium();
  
    break; // or do something else here
  }
if (c == 0x33) { // proceed to level screen only when 'L' is received
    levelhard();
  
    break; // or do something else here
  }
	if(c==0x72) 
	{
		if(curr_r<31) curr_r++;
		CHAR_COL = rgbconv(curr_r,curr_g,curr_b);
		//draw_rec(105,190,100,10,0);
		draw_rec(105,190,100,10,CHAR_COL);
			redg_rect(65,170,62,10,curr_r,curr_g,curr_b);

	
		
	}
		if(c==0x74) 
	{
		if(curr_r>0) curr_r--;
		CHAR_COL = rgbconv(curr_r,curr_g,curr_b);
		//draw_rec(105,190,100,10,0);
		draw_rec(105,190,100,10,CHAR_COL);
				redg_rect(65,170,62,10,curr_r,curr_g,curr_b);

		
	}

		if(c==0x67) 
	{
		if(curr_g<63) curr_g++;
		CHAR_COL = rgbconv(curr_r,curr_g,curr_b);
		//draw_rec(105,190,100,10,0);
		draw_rec(105,190,100,10,CHAR_COL);
			blueg_rect(205,170,62,10,curr_r,curr_g,curr_b);
		
	}
		if(c==0x68) 
	{
		if(curr_g>0) curr_g--;
		CHAR_COL = rgbconv(curr_r,curr_g,curr_b);
		//draw_rec(105,190,100,10,0);
		draw_rec(105,190,100,10,CHAR_COL);
			blueg_rect(205,170,62,10,curr_r,curr_g,curr_b);
		
	}
		if(c==0x62) 
	{
		if(curr_b<31) curr_b++;
		CHAR_COL = rgbconv(curr_r,curr_g,curr_b);
		//draw_rec(105,190,100,10,0);
		draw_rec(105,190,100,10,CHAR_COL);
				greeng_rect(135,170,62,10,curr_r,curr_g,curr_b);
		
	}
		if(c==0x6e) 
	{
		if(curr_b>0) curr_b--;
		CHAR_COL = rgbconv(curr_r,curr_g,curr_b);
		//draw_rec(105,190,100,10,0);
		draw_rec(105,190,100,10,CHAR_COL);
				greeng_rect(135,170,62,10,curr_r,curr_g,curr_b);
		
	}
	
}
	
}
void home_screen()
{
		 clear_screen();
  int i;
  int x;
  for(i=30;i<=290;i++) write_pixel(i,210,GREEN);
  for(i=30;i<=290;i++) write_pixel(i,30,GREEN);
  for(i=30;i<=210;i++) write_pixel(30,i,GREEN);
  for(i=30;i<=210;i++) write_pixel(290,i,GREEN);
   char* hw2 = "Welcome to Our Game";
      x = 30;
      while (*hw2) {
        write_char(x, 15, *hw2);
     x++;
     hw2++;
   }
   char* hw3 = "Press P to Play";
      x = 30;
      while (*hw3) {
        write_char(x, 19, *hw3);
     x++;
     hw3++;
   }

   volatile int * JTAG_UART_ptr = (int *) JTAG_UART_BASE;
   char c;



while (1) {
  c = get_jtag(JTAG_UART_ptr);
  if (c == 0x70) { // proceed to level screen only when 'L' is received
    draw_level_screen();

    break; // or do something else here
  }
}
}



int main () {

  home_screen();

   return 0;
}

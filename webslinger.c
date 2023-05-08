/* This files provides address values that exist in the system */
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
	
int error = 0;
int y_step;
int del_x;
int del_y;
bool is_steep;
bool falling = true;
bool release = false;

void plot_pixel(int x, int y, short int line_color);
void draw_pin(int x, int y);
void draw_box(int x, int y, short int line_color);
void wait();
void draw_line(int x0, int x1, int y0, int y1,short int line_color);	
void clear_screen();
void swap(int* a1, int* a2);
void draw_spider(int x, int y, short int line_color);
void draw_spider_big(int x, int y, short int line_color, int height, int width);
void srand(unsigned int seed);

#define SDRAM_BASE            0xC0000000
#define FPGA_ONCHIP_BASE      0xC8000000
#define FPGA_CHAR_BASE        0xC9000000

/* Cyclone V FPGA devices */
#define LEDR_BASE             0xFF200000
#define HEX3_HEX0_BASE        0xFF200020
#define HEX5_HEX4_BASE        0xFF200030
#define SW_BASE               0xFF200040
#define KEY_BASE              0xFF200050
#define TIMER_BASE            0xFF202000
#define PIXEL_BUF_CTRL_BASE   0xFF203020
#define CHAR_BUF_CTRL_BASE    0xFF203030

/* VGA colors */
#define WHITE 0xFFFF
#define YELLOW 0xFFE0
#define RED 0xF800
#define GREEN 0x07E0
#define BLUE 0x001F
#define CYAN 0x07FF
#define PURPLE 0xFFA8FF
#define GREY 0xC618
#define PINK 0xFC18
#define ORANGE 0xFC00
#define ABS(x) (((x) > 0) ? (x) : -(x))

int Colour[10] = {0xFFFF, 0xFFE0, 0xF800, 0x07E0, 0x001F, 0x07FF, 0xF81F,  0xC618, 0xFC18, 0xFC00};  

#define RESOLUTION_X 320
#define RESOLUTION_Y 240

#define BOX_LEN 2
#define NUM_BOXES 8
#define FALSE 0
#define TRUE 1

volatile int pixel_buffer_start;
volatile int pixel_back_buffer_start;

volatile int * key_pressed = (int *)0xFF200050;
volatile int * edge_cap = (int *)0xFF20005C;
volatile int * PS2_keyboard = (int *) 0xFF200100;
volatile int * led_ptr = (int *) 0xFF200000;

volatile int* buffer_ptr = (int*)0xC9000000;
int key_0 = 0b0001;

int center_x = 160;
int center_y = 120;
double string_len = 30;
bool connect_wait = true;
bool game_start;
bool game_over;
int score = 0;
int ps2_key;
int ps2_read;

double radian_convert = 0.0174532925;
double pi = 3.14159265358979;

int del_angle = -10;

typedef struct
		{
    		int x; //x position
    		int y;
			double fall_x;
			double fall_y;
			double curve_radius;
			int pivot_x;
			int pivot_y;
			int angle;
			int del_angle;
			int color;
			double vx;
			double vy;
			double ax;
			double ay;
			bool clockwise;
			int prev_pivot_x;
			int prev_pivot_y;
			int next_pivot_x;
			int next_pivot_y;
			int pivot_mov_x;
			int pivot_mov_y;
			bool new_connection;
	
	
		
		}Spider;

Spider current_spider;
	Spider pre_spider;
	

int main(void)
{	
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    pixel_buffer_start = *pixel_ctrl_ptr;
	pixel_back_buffer_start = *(pixel_ctrl_ptr+1);
	srand (time(NULL));
	
	
	
	current_spider.x = center_x + string_len;
	current_spider.y = center_y;
	current_spider.clockwise = false;
	
	
	current_spider.color = PURPLE;
	
	current_spider.x = 40;
	current_spider.y = 10;
	
	current_spider.pivot_x = 60;
	current_spider.pivot_y = 39;
	
	
	current_spider.next_pivot_x = rand() % ((280+1)-40) + 40;
	current_spider.next_pivot_y = rand() % ((200+1)-100) + 100;
	
	/*if(current_spider.x < current_spider.pivot_x){
				current_spider.del_angle = -10;
				current_spider.clockwise = false;
	}else{
				current_spider.del_angle = 10;
				current_spider.clockwise = true;
	}*/
	
	//draw_box(100,200,0XFFFF);
	
	
	//specify delta x and delta y
	
	

	
    //initialize location and direction of rectangles(not shown)

    /* set front pixel buffer to start of FPGA On-chip memory */
   *(pixel_ctrl_ptr + 1) = 0xC8000000; // first store the address in the 
                                        // back buffer
	
    /* now, swap the front/back buffers, to set the front buffer location */
	
	
	
    wait();

    /* initialize a pointer to the pixel buffer, used by drawing functions */
    pixel_buffer_start = *pixel_ctrl_ptr;
    clear_screen(); // pixel_buffer_start points to the pixel buffer
    /* set back pixel buffer to start of SDRAM memory */
   *(pixel_ctrl_ptr + 1) = 0xC0000000;
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // we draw on the back buffer
    clear_screen(); // pixel_buffer_start points to the pixel buffer
	
    int start_erase = 0;
	 
	// Erase any boxes and lines that were drawn in the last iteration 
	
	int counter = 0; 

	game_over=1;
	game_start=0;
	*led_ptr=0;
	//write_characters(0);
	while(1){
		
		if (game_over){
			clear_screen();
			draw_spider_big(center_x, center_y-50, RED, 30, 40);
			wait();
			game_over=0;
			*(short int*)(buffer_ptr + 32 + (7<<7)) = 'P';
			*(short int*)(buffer_ptr + 33 + (7<<7)) = 'R';
			*(short int*)(buffer_ptr + 34 + (7<<7)) = 'E';
			*(short int*)(buffer_ptr + 35 + (7<<7)) = 'S';
			*(short int*)(buffer_ptr + 36 + (7<<7)) = 'S';
			*(short int*)(buffer_ptr + 37 + (7<<7)) = ' ';
			*(short int*)(buffer_ptr + 38 + (7<<7)) = 'S';
			*(short int*)(buffer_ptr + 39 + (7<<7)) = 'P';
			*(short int*)(buffer_ptr + 40 + (7<<7)) = 'A';
			*(short int*)(buffer_ptr + 41 + (7<<7)) = 'C';
			*(short int*)(buffer_ptr + 42 + (7<<7)) = 'E';
			*(short int*)(buffer_ptr + 43 + (7<<7)) = ' ';
			*(short int*)(buffer_ptr + 44 + (7<<7)) = 'T';
			*(short int*)(buffer_ptr + 45 + (7<<7)) = 'O';
			*(short int*)(buffer_ptr + 46 + (7<<7)) = ' ';
			*(short int*)(buffer_ptr + 47 + (7<<7)) = 'S';
			*(short int*)(buffer_ptr + 48 + (7<<7)) = 'T';
			*(short int*)(buffer_ptr + 49 + (7<<7)) = 'A';
			*(short int*)(buffer_ptr + 50 + (7<<7)) = 'R';
			*(short int*)(buffer_ptr + 51 + (7<<7)) = 'T';
			draw_line(0, 120, 320, 120,RED);
			draw_line(0, 113, 320, 113 ,RED);
		}
		ps2_read = (*PS2_keyboard & 0x8000)>>15;
		ps2_key = *PS2_keyboard & 0xff;
		
		*led_ptr = ps2_key;
		if (ps2_read & (ps2_key == 0x29)){		
			game_start=1;
			clear_screen();
			wait();
			*(short int*)(buffer_ptr + 80) = ':';
			*(short int*)(buffer_ptr + 79) = 'E';
			*(short int*)(buffer_ptr + 78) = 'R';
			*(short int*)(buffer_ptr + 77) = 'O';
			*(short int*)(buffer_ptr + 76) = 'C';
			*(short int*)(buffer_ptr + 75) = 'S';
			int i;
			for (i=32; i<52; i++){*(short int*)(buffer_ptr + i + (7<<7)) = ' ';}
			write_characters(0);
			
		}
		
		
		
		//game_start=0;
		if (game_start){
			while(!game_over){
			
				if(falling == true){
					while (falling == true){
						draw_box(current_spider.pivot_x,current_spider.pivot_y,BLUE);
						//draw_box(current_spider.next_pivot_x,current_spider.next_pivot_y,BLUE);		
						if (start_erase > 0){
							draw_spider(pre_spider.x,pre_spider.y,0x006F9A);
						}
						
						// code for drawing the boxes and lines (not shown)
						// code for updating the locations of boxes (not shown)
						
							
						pre_spider.x = current_spider.x;
						pre_spider.y = current_spider.y;
							
							
						current_spider.fall_y = 3;
						current_spider.fall_x = 0;
							
						current_spider.fall_y = current_spider.fall_y+4;
							
							//current_spider.del_angle = -10;
						current_spider.y = current_spider.y + current_spider.fall_y;
								//make a copy of initial positions and put zeroes there
							
						double hyp = sqrt(pow(current_spider.x - current_spider.pivot_x,2) + pow(current_spider.y - current_spider.pivot_y,2));
					
						if(sqrt(pow(current_spider.x - current_spider.pivot_x,2) + pow(current_spider.y - current_spider.pivot_y,2)) <= string_len){
								falling = false;
							
							
							//current_spider.angle = 0;	
							if(current_spider.x < current_spider.pivot_x){
								current_spider.angle = 180-(asin((current_spider.y - current_spider.pivot_y)/hyp)/radian_convert); 	
							}
							else{
								current_spider.angle = asin((current_spider.y - current_spider.pivot_y)/hyp)/radian_convert;
							}
						}
							
						draw_spider(current_spider.x,current_spider.y,current_spider.color);
						int pin_count = 0;
						for(pin_count = 0; pin_count < 16; ++pin_count){
							draw_pin(0+pin_count*20,239);
						}
				
							
						++start_erase;
							
						wait(); // swap front and back buffers on VGA vertical sync
					
						pixel_buffer_start = *(pixel_ctrl_ptr + 1); // new back buffer
				
					}
				}

				if(falling ==false){		
					//   *edge_cap = 0xFFFF;
					
					
					if(current_spider.x < current_spider.pivot_x){
						current_spider.del_angle = -10;
						current_spider.clockwise = false;
					}
					else{
						current_spider.del_angle = 10;
						current_spider.clockwise = true;
					}
					
					int erase_x = current_spider.pivot_x;
					int erase_y = current_spider.pivot_y;
					if(current_spider.new_connection) {current_spider.next_pivot_x = rand() % ((280+1)-40) + 40;
						current_spider.next_pivot_y = rand() % ((200+1)-100) + 100;
						score++;
						write_characters(score);
													
					}	
					
					while (release == false){
						if(current_spider.new_connection == true){
							draw_box(current_spider.prev_pivot_x,current_spider.prev_pivot_y,0x006F9A);
							draw_box(current_spider.next_pivot_x,current_spider.next_pivot_y,0x006F9A);
							
						}
							
						draw_box(current_spider.pivot_x,current_spider.pivot_y,BLUE);	
						//if(current_spider.pivot_y ==41)draw_box(current_spider.next_pivot_x,current_spider.next_pivot_y,BLUE);	
						
						
							
						/*	if(*edge_cap){ //button released
								draw_line(0,0,220,220,0xFFFF);
								
							}*/
						if (start_erase > 0){
							
							draw_spider(pre_spider.x,pre_spider.y,0x006F9A);
							draw_line( pre_spider.pivot_x, pre_spider.pivot_y,pre_spider.x,pre_spider.y,0x006F9A);
							if(current_spider.new_connection == true){ 
								draw_box(pre_spider.pivot_x,pre_spider.pivot_y+3,0x006F9A);	
							}
						}
					
							
						// code for drawing the boxes and lines (not shown)
				
				
						// code for updating the locations of boxes (not shown)
						
						draw_box(current_spider.pivot_x,current_spider.pivot_y,BLUE);		
						
						//draw_box(current_spider.next_pivot_x,current_spider.next_pivot_y,BLUE);			 
							
							
						if(abs(current_spider.angle) > 360 && abs(current_spider.angle) < 380 ){
							current_spider.angle = 0;
						}
							
						ps2_read = (*PS2_keyboard & 0x8000)>>15;
						ps2_key = *PS2_keyboard & 0xff;
		
						*led_ptr = ps2_key;
						if(ps2_read & (ps2_key == 0x29)){
							release = true;
							
						}
						
							/*if(abs(current_spider.angle) > 210 && abs(current_spider.angle) < 220 ){ //abs because del angle can be negative
								//draw_line(0,0,220,220,0xFFFF);
								release = true;
							}*/
									
							
							
							
						pre_spider.x = current_spider.x;
						pre_spider.y = current_spider.y;
						pre_spider.pivot_x = current_spider.pivot_x;
						pre_spider.pivot_y = current_spider.pivot_y;
						pre_spider.angle = current_spider.angle;
							
							
							
							
							
							//current_spider.del_angle = -10;
						current_spider.x = current_spider.pivot_x + string_len*cos((current_spider.angle+current_spider.del_angle)*radian_convert);
						current_spider.y = current_spider.pivot_y + string_len*sin((current_spider.angle+current_spider.del_angle)*radian_convert);			
						current_spider.angle = current_spider.angle + current_spider.del_angle;
						
						if(current_spider.new_connection && current_spider.pivot_y > 40){
							current_spider.pivot_y = current_spider.pivot_y - 3;
						}
						
						if(current_spider.pivot_y <=40) 
							draw_box(current_spider.next_pivot_x,current_spider.next_pivot_y,BLUE);
							//make a copy of initial positions and put zeroes there
						
						
							draw_spider(current_spider.x,current_spider.y,current_spider.color);
							int pin_count = 0;
							for(pin_count = 0; pin_count < 16; ++pin_count){
								draw_pin(0+pin_count*20,239);
							}
				
							if(current_spider.new_connection == true) draw_box(erase_x,erase_y,0x006F9A);
							draw_line( current_spider.pivot_x, current_spider.pivot_y,current_spider.x,current_spider.y,current_spider.color);
								
						++start_erase;
							
						wait(); // swap front and back buffers on VGA vertical sync
					
					//	 
						pixel_buffer_start = *(pixel_ctrl_ptr + 1); // new back buffer
				
					}
				}
				
				
				current_spider.new_connection = false;
				
				if(release == true){		
				//   *edge_cap = 0xFFFF; 
					//set values based on angle
					falling = true;
					current_spider.ax = 0;
					current_spider.ay = 1;
					
					double opposite;
					double adjacent;
					double theta;
					
					adjacent = abs(current_spider.pivot_x - current_spider.x);
					opposite = abs(current_spider.pivot_y - current_spider.y);
					theta = atan(opposite/adjacent);
					
					int rotation;
					
					if(current_spider.clockwise == true){
						rotation = -1;
					}
					else rotation = 1;
					
						
					if(current_spider.x > current_spider.pivot_x){ //270 to 90	
							current_spider.vy = rotation*9*sin((pi/2)-theta);	
					}
					else
							if(current_spider.x < current_spider.pivot_x){ //90 to 270
								current_spider.vy = rotation*-9*sin((pi/2)-theta);
							}else{ // 90 and 270 specifically
								current_spider.vy = 0;
							}
						
						if(current_spider.y > current_spider.pivot_y){ //180 to 360 (screen flipped)
							current_spider.vx = rotation*-11*cos((pi/2)-theta);
						
						}else if(current_spider.y < current_spider.pivot_y){ //0 to 180
							current_spider.vx = rotation*11*cos((pi/2)-theta);	
							}else{//180 and 360 or 0 specifically
								current_spider.vx = 0;
							}
						
						current_spider.prev_pivot_x = current_spider.pivot_x;
						current_spider.prev_pivot_y = current_spider.pivot_y;
					
						current_spider.pivot_x = current_spider.next_pivot_x;
						current_spider.pivot_y = current_spider.next_pivot_y;
						
					while (falling == true){
						
							
						if (start_erase > 0){
							
							draw_spider(pre_spider.x,pre_spider.y,0x006F9A);
							draw_line(pre_spider.pivot_x, pre_spider.pivot_y,pre_spider.x,pre_spider.y,0x006F9A);
							draw_box(current_spider.prev_pivot_x,current_spider.prev_pivot_y,BLUE);
						}
						draw_box(current_spider.pivot_x,current_spider.pivot_y,BLUE);
							
						// code for drawing the boxes and lines (not shown)
				
				
						// code for updating the locations of boxes (not shown)
						
								
							
							
							if(abs(current_spider.angle) > 360 && abs(current_spider.angle) < 380 ){
							
								current_spider.angle = 0;
								
							}
							
							
							pre_spider.x = current_spider.x;
							pre_spider.y = current_spider.y;
							
							
							
							if ((current_spider.x+current_spider.vx)<15){current_spider.x = 15;}
							else if ((current_spider.x+current_spider.vx)>304){current_spider.x = 305;}
							else{current_spider.x = current_spider.x + current_spider.vx;}
							if ((current_spider.y + current_spider.vy)<8){current_spider.y = 8;}
							else 
							if ((current_spider.y + current_spider.vy)>222){current_spider.y = 222;
																		current_spider.vx = 0;
																		current_spider.color = RED;
																			game_over=1;
																			
							}
							else{current_spider.y = current_spider.y + current_spider.vy;}
				
				
							current_spider.vx = current_spider.vx + current_spider.ax;
							current_spider.vy = current_spider.vy + current_spider.ay;
							
						
							
							double hyp = sqrt(pow(current_spider.x - current_spider.pivot_x,2) + pow(current_spider.y - current_spider.pivot_y,2));
					
							if(sqrt(pow(current_spider.x - current_spider.pivot_x,2) + pow(current_spider.y - current_spider.pivot_y,2)) <= string_len){
								falling = false;
								release = false; //hooking onto same because next_pivot not updated
								current_spider.new_connection = true;
								
							
								if(current_spider.x < current_spider.pivot_x){
							current_spider.angle = 180-(asin((current_spider.y - current_spider.pivot_y)/hyp)/radian_convert); 	
							
							}
								else{
								current_spider.angle = asin((current_spider.y - current_spider.pivot_y)/hyp)/radian_convert;}
							
							}
							
							
								draw_spider(current_spider.x,current_spider.y,current_spider.color);
							int pin_count = 0;
							for(pin_count = 0; pin_count < 16; ++pin_count){
								draw_pin(0+pin_count*20,239);
							}
				
								//draw_line( current_spider.pivot_x, current_spider.pivot_y,current_spider.x,current_spider.y,current_spider.color);
							
							
						++start_erase;
							
						wait(); // swap front and back buffers on VGA vertical sync
					
						 
						pixel_buffer_start = *(pixel_ctrl_ptr + 1); // new back buffer
					
					}
					if (game_over){break;}
				}
			}
		}	
	}
}

void wait(){
		volatile int *pixel_ctrl_ptr = (int*)0xFF203020;
		register int status;
		
		*pixel_ctrl_ptr = 1; //start sync
		
		status = *(pixel_ctrl_ptr +3);
		while ((status & 0x01) != 0){
		status = *(pixel_ctrl_ptr + 3);
	 }
}
// code not shown for clear_screen() and draw_line() subroutines
void draw_line(int x0, int y0, int x1, int y1,short int line_color){
	error = 0;
	if (ABS(y1 - y0) > ABS(x1 - x0)){
		is_steep = true;
	}else{
		is_steep = false;}
		
	if (is_steep == true){	
	
		swap(&x0, &y0);
		swap(&x1, &y1);		
	}
	
	if (x0 > x1){
			
		swap(&x0, &x1);
		swap(&y0, &y1);	
	}
	
	int deltax = x1 - x0;
	int deltay = ABS(y1 - y0);
	int error = -(deltax / 2);
	int y = y0;
		
	if (y0 < y1){
	y_step = 1;
	}else{
	y_step = -1;}
	int x;	
	for (x = x0; x <= x1; ++x){
		if (is_steep == true){
			plot_pixel(y, x, line_color);	
		}else{
			plot_pixel(x, y, line_color);
		}
		error = error + deltay;
		if (error > 0){
  			y = y + y_step;
  			error = error - deltax;
		}
	}
		

}



void plot_pixel(int x, int y, short int line_color)
{
    *(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;
}

void plot_back_pixel(int x, int y, short int line_color)
{
    *(short int *)(pixel_back_buffer_start + (y << 10) + (x << 1)) = line_color;
}


void draw_spider(int x, int y, short int line_color){
	
	int i = 0;
	int j = 0;
	int e=0;
	int x1;
	int y1;
	
	int height = 7;
	int width = 10;
	
	for(int y1 = -height; y1<=height; y1++){
    	for(int x1 = -width; x1<=width; x1++){
        	if(x1*x1*height*height+y1*y1*width*width <= height*height*width*width)
           		 plot_pixel(x+x1, y+y1, line_color); 
   			}
	}
	
	height = 4;
	width = 4;
	
	for(int y1 = -height; y1<=height; y1++){
    	for(int x1 = -width; x1<=width; x1++){
        	if(x1*x1*height*height+y1*y1*width*width <= height*height*width*width)
           		 plot_pixel(x+x1-3, y+y1-2, 0x006F9A);
				
   			}
	}
	
	for(int y1 = -height; y1<=height; y1++){
    	for(int x1 = -width; x1<=width; x1++){
        	if(x1*x1*height*height+y1*y1*width*width <= height*height*width*width)
           		 plot_pixel(x+x1+3, y+y1-2, 0x006F9A);
				
   			}
	}
	
	height = 1;
	width = 1;
	
	for(int y1 = -height; y1<=height; y1++){
    	for(int x1 = -width; x1<=width; x1++){
        	if(x1*x1*height*height+y1*y1*width*width <= height*height*width*width)
           		 plot_pixel(x+x1-2, y+y1-2, line_color);
				
   			}
	}
	
	
	for(int y1 = - height; y1<=height; y1++){
    	for(int x1 = -width; x1<=width; x1++){
        	if(x1*x1*height*height+y1*y1*width*width <= height*height*width*width)
           		 plot_pixel(x+x1+2, y+y1-2, line_color);
				
   			}
	}
	
	plot_pixel(x-10,y+3,line_color);plot_pixel(x-11,y+2,line_color);plot_pixel(x-12,y+3,line_color);plot_pixel(x-13,y+4,line_color);//LEFT BOTTOM
	plot_pixel(x+10,y+3,line_color);plot_pixel(x+11,y+2,line_color);plot_pixel(x+12,y+3,line_color);plot_pixel(x+13,y+4,line_color);//RIGHT BOTTOM
	plot_pixel(x+10,y,line_color);plot_pixel(x+11,y-1,line_color);plot_pixel(x+12,y,line_color);plot_pixel(x+13,y+1,line_color);
	plot_pixel(x-10,y,line_color);plot_pixel(x-11,y-1,line_color);plot_pixel(x-12,y,line_color);plot_pixel(x-13,y+1,line_color);
	plot_pixel(x-10,y-3,line_color);plot_pixel(x-11,y-4,line_color);plot_pixel(x-12,y-5,line_color);plot_pixel(x-13,y-4,line_color);plot_pixel(x-14,y-3,line_color);plot_pixel(x-15,y-2,line_color);
	plot_pixel(x+10,y-3,line_color);plot_pixel(x+11,y-4,line_color);plot_pixel(x+12,y-5,line_color);plot_pixel(x+13,y-4,line_color);plot_pixel(x+14,y-3,line_color);plot_pixel(x+15,y-2,line_color);
	
}


void draw_spider_big(int x, int y, short int line_color, int height, int width){
	int i = 0;
	int j = 0;
	int e=0;
	int x1;
	int y1;
	
	
	//body
	for(int y1 = -height; y1<=height; y1++){
    	for(int x1 = -width; x1<=width; x1++){
        	if(x1*x1*height*height+y1*y1*width*width <= height*height*width*width)
           		 plot_pixel(x+x1, y+y1, PURPLE); 
   			}
	}
	
	height = height/1.7;
	width = height;
	

	//eyes
	for(int y1 = -height; y1<=height; y1++){
    	for(int x1 = -width; x1<=width; x1++){
        	if(x1*x1*height*height+y1*y1*width*width <= height*height*width*width)
           		 plot_pixel(x+x1-15, y+y1-2, 0xFFFF);
				
   			}
	}
	
	for(int y1 = -height; y1<=height; y1++){
    	for(int x1 = -width; x1<=width; x1++){
        	if(x1*x1*height*height+y1*y1*width*width <= height*height*width*width)
           		 plot_pixel(x+x1+15, y+y1-2, 0xFFFF);
				
   			}
	}
	
	
	
	height = height/2;
	width = height;
	
	for(int y1 = -height; y1<=height; y1++){
    	for(int x1 = -width; x1<=width; x1++){
        	if(x1*x1*height*height+y1*y1*width*width <= height*height*width*width)
           		 plot_pixel(x+x1-8, y+y1-2, PURPLE);
				
   			}
	}
	
	
	for(int y1 = - height; y1<=height; y1++){
    	for(int x1 = -width; x1<=width; x1++){
        	if(x1*x1*height*height+y1*y1*width*width <= height*height*width*width)
           		 plot_pixel(x+x1+8, y+y1-2, PURPLE);
				
   			}
	}
	int eyebrows;
	
	/*draw_line(130,100,x,y-5,0);
	draw_line(131,100,x,y-5,0);
	draw_line(132,100,x,y-5,0);
	draw_line(133,100,x,y-5,0);
	draw_line(134,100,x,y-5,0);
	draw_line(135,100,x,y-5,0);
	
	draw_line(190,100,x,y-5,0);
	draw_line(191,100,x,y-5,0);
	draw_line(192,100,x,y-5,0);
	draw_line(193,100,x,y-5,0);
	draw_line(194,100,x,y-5,0);
	draw_line(195,100,x,y-5,0);*/
	
		
	plot_pixel(x-10,y+3,line_color);plot_pixel(x-11,y+2,line_color);plot_pixel(x-12,y+3,line_color);plot_pixel(x-13,y+4,line_color);//LEFT BOTTOM
	plot_pixel(x+10,y+3,line_color);plot_pixel(x+11,y+2,line_color);plot_pixel(x+12,y+3,line_color);plot_pixel(x+13,y+4,line_color);//RIGHT BOTTOM
	plot_pixel(x+10,y,line_color);plot_pixel(x+11,y-1,line_color);plot_pixel(x+12,y,line_color);plot_pixel(x+13,y+1,line_color);
	plot_pixel(x-10,y,line_color);plot_pixel(x-11,y-1,line_color);plot_pixel(x-12,y,line_color);plot_pixel(x-13,y+1,line_color);
	plot_pixel(x-10,y-3,line_color);plot_pixel(x-11,y-4,line_color);plot_pixel(x-12,y-5,line_color);plot_pixel(x-13,y-4,line_color);plot_pixel(x-14,y-3,line_color);plot_pixel(x-15,y-2,line_color);
	plot_pixel(x+10,y-3,line_color);plot_pixel(x+11,y-4,line_color);plot_pixel(x+12,y-5,line_color);plot_pixel(x+13,y-4,line_color);plot_pixel(x+14,y-3,line_color);plot_pixel(x+15,y-2,line_color);

}

void draw_box(int x, int y, short int line_color){
	int i;
	int e;
	
	for(i=-2; i<3;++i){
		for(e=-2; e<3; ++e){
			plot_pixel(x+i, y+e, line_color);
		}
	}
	
	for(i=-1; i<2;++i){	
			plot_pixel(x+i, y-3, line_color);
			plot_pixel(x+i,y+3, line_color);
	}
	
	for(i=-1; i<2;++i){	
			plot_pixel(x+i, y-3, line_color);
			plot_pixel(x+i,y+3, line_color);
	}
	
	for(i=-1; i<2;++i){	
			plot_pixel(x-3, y+i, line_color);
			plot_pixel(x+3,y+i, line_color);
	}
		
}

void draw_pin(int x, int y){
	
	int x1 = 0;
	int y1 = 0;
   
   for (x1 = 1; x1 <= 10; ++x1, y1 = 0) {
      while (y1 != 2 * x1 - 1) {
       plot_pixel(x+x1, y-y1, RED);  
         ++y1;
      }
   }
	
	for (x1 = 1; x1 <= 10; ++x1, y1 = 0) {
      while (y1 != 2 * x1 - 1) {
       plot_pixel(x-x1+20, y-y1, RED);  
         ++y1;
      }
   }
}



void clear_screen(){

	int x;
	int y;
	
	for (x = 0; x < 320; ++x){
		for (y = 0; y < 240; ++y){
			plot_pixel(x,y,0x006F9A);
			plot_back_pixel(x,y,0x006F9A);
		}
	}
	
}
	
void swap(int* a1, int* a2){

	int temp = *a1;
	*a1 = *a2;
	*a2 = temp;
	
}

void write_characters(int num){
	int ascii_characters[10] ={0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39}; 
	volatile int* buffer_ptr = (int*)0xC9000000;
	int a, b, c;
	a = num/100;
	b = (num-(a*100))/10;
	c = num%10;
	
	*(short int*)(buffer_ptr + 81) = ascii_characters[a];
	*(short int*)(buffer_ptr + 82) = ascii_characters[b];
	*(short int*)(buffer_ptr + 83) = ascii_characters[c];
}
	
	
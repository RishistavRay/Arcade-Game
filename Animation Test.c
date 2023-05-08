/* This files provides address values that exist in the system */
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
	
int error = 0;
int y_step;
int del_x;
int del_y;
bool is_steep;


void plot_pixel(int x, int y, short int line_color);
void wait();
void draw_line(int x0, int x1, int y0, int y1,short int line_color);	
void clear_screen();
void swap(int* a1, int* a2);
void draw_box(int x, int y, short int line_color);
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
#define MAGENTA 0xF81F
#define GREY 0xC618
#define PINK 0xFC18
#define ORANGE 0xFC00
#define ABS(x) (((x) > 0) ? (x) : -(x))

/* Array of Colours */
int Colour[10] = {0xFFFF, 0xFFE0, 0xF800, 0x07E0, 0x001F, 0x07FF, 0xF81F,  0xC618, 0xFC18, 0xFC00};  
	
/* Screen size. */
#define RESOLUTION_X 320
#define RESOLUTION_Y 240

/* Constants for animation */
#define BOX_LEN 2
#define NUM_BOXES 8

#define FALSE 0
#define TRUE 1

// Begin part3.c code for Lab 7


volatile int pixel_buffer_start; // global variable

int main(void)
{
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    pixel_buffer_start = *pixel_ctrl_ptr;
	// declare other variables(not shown)
	//clear_screen();
	srand (time(NULL));
	
	typedef struct
		{
    		int x; //x position
    		int y;
			int del_x; //change in x direction
			int del_y; //change in y direction
			int color;
		}Box;
	
	Box box[8];
	Box pre_frame[8];
	
	int i;
	
	for (i = 0; i < 8; ++i){
	
		box[i].x = rand() % 300 + 5;
		box[i].y = rand() % 220 + 5;
		int color_code = rand() % 10;
		box[i].color = Colour[color_code];
		//draw_box(box[i].x,box[i].y,0xFFF);
		
	}
	
	int c2;
	
	for (c2 = 0; c2 < 8; ++c2){
	
		box[c2].del_x = ((rand() % 2)*2)-1; 
		box[c2].del_y = ((rand() % 2)*2)-1;
			
	} 
	
	
	
	/*
	box[0].del_x = 1;box[0].del_y = 1;
	box[1].del_x = 1;box[1].del_y = 1;
	box[2].del_x = 1;box[2].del_y = -1;
	box[3].del_x = 1;box[3].del_y = -1;
	box[4].del_x = -1;box[4].del_y = -1;
	box[5].del_x = -1;box[5].del_y = -1;
	box[6].del_x = -1;box[6].del_y = 1;
	box[7].del_x = -1;box[7].del_y = 1;*/
	
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
       
			while (1){
  		
			
			if (start_erase > 0){
				int c3;
				for (c3 = 0; c3 < 8; ++c3){
					draw_box(pre_frame[c3].x,pre_frame[c3].y,0);
			
				}
			
				draw_line(pre_frame[0].x,pre_frame[0].y,pre_frame[1].x,pre_frame[1].y,0x0);
				draw_line(pre_frame[1].x,pre_frame[1].y,pre_frame[2].x,pre_frame[2].y,0x0);
				draw_line(pre_frame[2].x,pre_frame[2].y,pre_frame[3].x,pre_frame[3].y,0x0);
				draw_line(pre_frame[3].x,pre_frame[3].y,pre_frame[4].x,pre_frame[4].y,0x0);
				draw_line(pre_frame[4].x,pre_frame[4].y,pre_frame[5].x,pre_frame[5].y,0x0);
				draw_line(pre_frame[5].x,pre_frame[5].y,pre_frame[6].x,pre_frame[6].y,0x0);
				draw_line(pre_frame[6].x,pre_frame[6].y,pre_frame[7].x,pre_frame[7].y,0x0);
				draw_line(pre_frame[7].x,pre_frame[7].y,pre_frame[0].x,pre_frame[0].y,0x0);
				
			}
   	      
				
        // code for drawing the boxes and lines (not shown)
	   
	   
        // code for updating the locations of boxes (not shown)
			 
			int i;
				for ( i = 0; i < 8; ++i){
					
				pre_frame[i].x = box[i].x;
				pre_frame[i].y = box[i].y;
					
				if ((box[i].x == 0 || box[i].x == 316 )){
				
					box[i].del_x = (box[i].del_x)*(-1);
				}
					
				if ((box[i].y == 1 || box[i].y == 237 )){
				
					box[i].del_y = (box[i].del_y)*(-1);
				}
				
				
					//make a copy of initial positions and put zeroes there
					
					
				box[i].x = box[i].x + box[i].del_x;
				box[i].y = box[i].y + box[i].del_y;
			}
			 
			 int c4;
			 for (c4 = 0; c4 < 8; ++c4){
				
			 	draw_box(box[c4].x,box[c4].y,box[c4].color);
	
			}
		
		draw_line(box[0].x,box[0].y,box[1].x,box[1].y,box[1].color);
		draw_line(box[1].x,box[1].y,box[2].x,box[2].y,box[2].color);
		draw_line(box[2].x,box[2].y,box[3].x,box[3].y,box[3].color);
		draw_line(box[3].x,box[3].y,box[4].x,box[4].y,box[4].color);
		draw_line(box[4].x,box[4].y,box[5].x,box[5].y,box[5].color);
		draw_line(box[5].x,box[5].y,box[6].x,box[6].y,box[6].color);
		draw_line(box[6].x,box[6].y,box[7].x,box[7].y,box[7].color);
		draw_line(box[7].x,box[7].y,box[0].x,box[0].y,box[0].color);
  
		//((rand() % 2)*2)-1;		
				
		++start_erase;
				
        wait(); // swap front and back buffers on VGA vertical sync
		
		//	 
        pixel_buffer_start = *(pixel_ctrl_ptr + 1); // new back buffer
		
			 
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

void draw_box(int x, int y, short int line_color){

	/*plot_pixel(x, y, line_color);
	plot_pixel(x+1,y, line_color);
	plot_pixel(x,y+1, line_color);
	plot_pixel(x+1,y+1, line_color);*/
	
	plot_pixel(x, y, line_color);
	plot_pixel(x+1, y, line_color);
	plot_pixel(x+2, y, line_color);
	plot_pixel(x, y+1, line_color);
	plot_pixel(x+1, y+1, line_color);
	plot_pixel(x+2, y+1, line_color);
	plot_pixel(x, y+2, line_color);
	plot_pixel(x+1, y+2, line_color);
	plot_pixel(x+2, y+2, line_color);
	
	
	
}


void clear_screen(){

	int x;
	int y;
	
	for (x = 0; x < 320; ++x){
		for (y = 0; y < 240; ++y){
			plot_pixel(x,y,0);
		}
	}
	
}
	
void swap(int* a1, int* a2){

	int temp = *a1;
	*a1 = *a2;
	*a2 = temp;
	
}	

	
	
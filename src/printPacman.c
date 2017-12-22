#include "printpacman.h"
#include "pac.h"
#include "startPage.h"

void print_range(unsigned int ** image, int rowBegin, int rowEnd,
  int columnBegin, int columnEnd){
  unsigned int *phy_addr = FB_ADDR;

  S3C_VIDW00ADD0B0 = FB_ADDR; // Buffer Address
  S3C_VIDW00ADD1B0 = S3C_VIDWxxADD1_VBASEL_F(FB_ADDR +
    (PAGE_WIDTH + S3CFB_OFFSET) * S3CFB_VRES);
  S3C_VIDW00ADD2  = S3C_VIDWxxADD2_OFFSIZE_F(S3CFB_OFFSET) |
    S3C_VIDWxxADD2_PAGEWIDTH_F(PAGE_WIDTH);

  //Write your code here!
  int i = 0,j = 0, n =0, m = 0;
  for(i = rowBegin; i < rowEnd; i++){
    for(j = columnBegin; j < columnEnd; j++){
      if(image[n][m] == 0x0)
        continue;
      else
        phy_addr[800*i + j] = image[n][m];
      m++;
    }
    n++;
  }

}
void printStartPage(){
  print_range(startPage, 0, 480, 0, 800);
  
}


void printIngame(){
  print_range(pac, 4, 445, 4, 444);//board
  /*print_range();//score
  print_range();//time
  print_range();//pac man
  print_range();//enemy1
  print_range();//enemy2
  print_range();//enemy3*/

}

// ModuleA
//
// Layout:
//  col(pin)|1(6)  |2(7)  |3(8)  |4(9)  |5(10)  |6(11)  |7(12)
//  row(pin)+------+------+------+------+-------+-------+-----
//   1(5)   |      |      |F1    |F2    |F3     |F4     |F5
//   2(4)   |      |      |1     |2     |3      |4      |5 
//   3(3)   |Tab   |      |Q     |W     |E      |R      |T
//   4(2)   |ESC   |      |A     |S     |D      |F      |G
//   5(1)   |Shft  |<>|   |Z     |X     |C      |V      |B
//   6(0)   |Ctrl  |Meta  |Alt   |Mnu   |PS     |       |Space
#include "Key.h"

#define numCols 7
#define numRows 6

int cols[] = {6,7,8,9,10,11,12};
int rows[] = {0,1,2,3,4,5};

uint8_t lastFrame[8] = {0};

uint8_t keys[numRows][numCols] = {
    {0,0, Key.F1, Key.F2, Key.F3, Key.F4, Key.F5}
    ,{}
    ,{}
    ,{}
    ,{}
    ,{}
};

void setup(){}
void loop(){}

/* KeySizeTest
 * Make a small 2x2 matrix to test keyholes.
 */
keyCols = 4;
keyRows = 4;
keyX = 15.5;
keyY = 13;
keySpace = 3.5;
edgeSpaceAddition = 4;
edgeSpace = (edgeSpaceAddition*2)-keySpace;
keyZ = -1;

moduleX = (edgeSpace+edgeSpaceAddition)+(keySpace*keyCols)+keyX*(keyCols-1)+keyX;
moduleY = (edgeSpace+edgeSpaceAddition-keySpace)+(keySpace*2*keyRows)+keyY*(keyRows-1)+keyY;
moduleZ = 1;

keySize = [keyX, keyY, moduleZ+2];

function position(x,y,z) = [edgeSpace+keySpace*x+keyX*(x-1)-(edgeSpaceAddition/2), edgeSpace-keySpace+(keySpace*2)*y+keyY*(y-1)-(edgeSpaceAddition/2), z];

module keyHoles (yStart, yEnd, xStart, xEnd){
  for(y = [yStart:yEnd]){
    for(x = [xStart:xEnd]){
      translate(position(x,y,keyZ)) cube(keySize);
    }
  }
}

module beam (bStart, bEnd){
  for (b = [bStart:bEnd]){  
    translate([edgeSpace,b*22-2,0]){  
      cube([moduleX-(edgeSpace*2),2,2]);
    }
  }
}


difference(){
  cube([moduleX,moduleY,moduleZ]);
  keyHoles(1,4,1,4);
  }
beam(1, keyRows-1);
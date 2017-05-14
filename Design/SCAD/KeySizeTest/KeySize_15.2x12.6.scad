/* KeySizeTest
 * Make a small 2x2 matrix to test keyholes.
 */
keyCols = 2;
keyRows = 2;
keyX = 15.2;
keyY = 12.6;
keySpace = 3.5;
vKeySpace=6;
edgeSpaceAddition = 4;
edgeSpace = (edgeSpaceAddition*2)-keySpace;
keyZ = -1;

moduleX = (edgeSpace+edgeSpaceAddition)+(keySpace*keyCols)+keyX*(keyCols-1)+keyX;
moduleY = (edgeSpaceAddition*2-keySpace)+(vKeySpace*keyRows)+keyY*(keyRows-1)+keyY;
moduleZ = 1;

keySize = [keyX, keyY, moduleZ+2];

function position(x,y,z) = [edgeSpace+keySpace*x+keyX*(x-1)-(edgeSpaceAddition/2), edgeSpace-keySpace+(vKeySpace)*y+keyY*(y-1)-(edgeSpaceAddition/2), z];

module keyHoles (yStart, yEnd, xStart, xEnd){
  for(y = [yStart:yEnd]){
    for(x = [xStart:xEnd]){
      translate(position(x,y,keyZ)) cube(keySize);
    }
  }
}

module hBeam (bStart, bEnd){
  for (b = [bStart:bEnd]){  
    color([0.5,0,0]){
      translate(position(0.9,b-0.2,1)){  
        cube([moduleX-(edgeSpace*2),2,3]);
      }
    }
  }
}

module vBeam (bStart, bEnd){
  for (b = [bStart:bEnd]){  
    color([0.5,0,0]){
      translate(position(b-0.125,0.9,1)){  
        cube([1.5,moduleY-(edgeSpace*1.5),3]);
      }
    }
  }
}


difference(){
  cube([moduleX,moduleY,moduleZ]);
  keyHoles(1,2,1,2);
}

hBeam(2, keyRows);
vBeam(2, keyCols);

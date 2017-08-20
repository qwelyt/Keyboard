/* Module A v2
 * Fewer keys
 * Keycaps actually fit
 * Cross beams for support
 * Keys actally  fit, snuggly
 */
 
keyCols = 7;
keyRows = 5;
keyX = 15.2;
keyY = 12.6;
keySpace = 3.5;
vKeySpace=6;
edgeSpaceAddition = 4;
edgeSpace = (edgeSpaceAddition*2)-keySpace;
keyZ = -1;

supportH = 3.5;
supportW = 1.5;
supportZ = -3;

moduleX = (edgeSpace+edgeSpaceAddition)+(keySpace*keyCols)+keyX*(keyCols-1)+keyX;
moduleY = (edgeSpaceAddition*2-keySpace)+(vKeySpace*keyRows)+keyY*(keyRows-1)+keyY;
moduleZ = 1;

keySize = [keyX, keyY, moduleZ+2];

function position(x,y,z) = [edgeSpace+keySpace*x+keyX*(x-1)-(edgeSpaceAddition/2), edgeSpace-keySpace+(vKeySpace)*y+keyY*(y-1)-(edgeSpaceAddition/2), z];

module screwHole (x, y){
	translate([x,y,keyZ]) cylinder(h=moduleZ+2, r=2, $fn=10);
}

module keyHoles (yStart, yEnd, xStart, xEnd){
  for(y = [yStart:yEnd]){
    for(x = [xStart:xEnd]){
      translate(position(x,y,keyZ)) cube(keySize);
    }
  }
}

module keyHoles2 (yStart, yEnd, xStart, xEnd, keySize){
  for(y = [yStart:yEnd]){
    for(x = [xStart:xEnd]){
      translate(position(x,y,keyZ)) cube(keySize);
    }
  }
}

module hBeam (bStart, bEnd){
  for (b = [bStart:bEnd]){  
    color([0.5,0,0]){
      translate(position(0.9,b-0.2,supportZ)){  
        cube([moduleX-(edgeSpace*2),supportW,supportH]);
      }
    }
  }
}

module hBeam2 (xStart, xEnd, yStart, yEnd){
  for(y = [yStart:yEnd]){
    color([0.5,0,0]){
      translate(position((0.9*xStart),y-0.2,supportZ)){
        cube([(keyX*xEnd)-1.7,1.5,supportH]);
      }
    }
  }
}

module vBeam (bStart, bEnd){
  for (b = [bStart:bEnd]){  
    color([0.5,0,0]){
      translate(position(b-0.125,0.9,supportZ)){  
        cube([1.5,moduleY-(edgeSpace*1.5),supportH]);
      }
    }
  }
}

module vBeam2 (xStart, xEnd, yStart, yEnd){
  for (x = [xStart:xEnd]){  
      color([0.5,0,0]){
        translate(position(x,yStart,supportZ)){  
          cube([1.5,(keyY*yEnd)-1.7,supportH]);
        }
      }
    }
}


difference(){
  cube([moduleX,moduleY,moduleZ]);
  keyHoles(1,1,2,7);
  keyHoles(2,2,1,5);
  keyHoles(2,2,6.5,6.5);
  keyHoles(3,5,1,6);
  //keyHoles(3.6,3.6,7,7);
  keyHoles2(3.53,3.53,7.06,7.06, [keyY, keyX, moduleZ+2]);
  keyHoles(5,5,7,7);

  
  screwHole(3,3);
  screwHole(3,moduleY-3);
	screwHole(moduleX-3,moduleY-3);
	screwHole(moduleX-3,3);
}

hBeam(2, 3);
hBeam2(1, 7.5,4,4);
hBeam(5,5);

vBeam(2, 6);
vBeam2(6.3, 6.3, 1.8,1.7);
vBeam2(6.85, 6.85, 0.91,1.5);
vBeam2(7.5, 7.5, 1.8,1.7);
vBeam2(6.85, 6.85, 2.8,4.54);
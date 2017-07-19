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

module screwHoleD (x, y, depth){
	translate([x,y,keyZ]) cylinder(h=depth, r=2, $fn=10);
}

module screwHole (x, y){
  screwHoleD(x,y,moduleZ+2);
}



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
      translate(position(0.9,b-0.2,supportZ)){  
        cube([moduleX-(edgeSpace*2),supportW,supportH]);
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
        translate(position(x-0.125,(0.9*yStart),supportZ)){  
          cube([1.5,(keyY*yEnd)-1.7,supportH]);
        }
      }
    }
}

difference(){
  cube([moduleX,moduleY,moduleZ]);
  keyHoles(1,1,1,5);
  keyHoles(1,1,6.5,6.5);
  
  keyHoles(2,2,1,7);
  
  keyHoles(3,3,1.5,1.5);
  keyHoles(3,3,3,7);
  
  keyHoles(4,4,1.5,1.5);
  keyHoles(4,4,3,7);
  
  keyHoles(5,5,3,7);
  
  screwHole(3,3);
  screwHole(3,moduleY-3);
	screwHole(moduleX-3,moduleY-3);
	screwHole(moduleX-3,3);
  
}


hBeam(2, keyRows);
//vBeam(2, keyCols);
vBeam2(6.4,6.4, 1, 1.5);
vBeam2(7.6,7.6, 1, 1.5);
vBeam2(2,2, 1, 3);
vBeam2(1.3,1.3, 3.2, 4.4);
vBeam2(2.6,2.6, 3.2, 4.4);
vBeam(3,6);
vBeam2(7,7, 2, 6);


//translate([0,0,-10]){
//  difference(){
//    cube([moduleX,moduleY,moduleZ]);
//    translate([10,5,0]){
//      cube([10,moduleY-10,moduleZ]);
//    }
//    translate([moduleX-20,5,0]){
//      cube([10,moduleY-10,moduleZ]);
//    }
//    
//    screwHole(3,3);
//    screwHole(3,moduleY-3);
//    screwHole(moduleX-3,moduleY-3);
//    screwHole(moduleX-3,3);
//  }
//  
//  translate([(moduleX/2)-9, edgeSpace, -5]){
//    cube([supportW,moduleY-(edgeSpace*2),5]);
//  }
//  translate([(moduleX/2)+9, edgeSpace, -5]){
//    cube([supportW,moduleY-(edgeSpace*2),5]);
//  }
//}
/* Module A v2
 * Fewer keys
 * Cross beams for support
 * Keys actally  fit, snuggly
 */
 
top=false;
beams=false;
spacer=false;
uSupports=false;
uHolder=true;
bottom=true;
 
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

module screwHoleD (x, y, depth){
	translate([x,y,keyZ]) cylinder(h=depth, r=2, $fn=10);
}

module screwHoleDR (x, y, depth, radius){
	translate([x,y,keyZ]) cylinder(h=depth, r=radius, $fn=10);
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

module spacer(){
  difference(){
    thickness = (supportH+1)*2;
    screws = thickness+1;
    reducer = (edgeSpace*2+3);
    cube([moduleX,moduleY,thickness]);
    screwHoleD(3,3,screws);
    screwHoleD(3,moduleY-3,screws);
    screwHoleD(moduleX-3,moduleY-3,screws);
    screwHoleD(moduleX-3,3,screws);
    
    translate([edgeSpace/2,edgeSpace+1.5,0]){
      cube([moduleX-edgeSpace,moduleY-reducer,screws]);
    }
    translate([edgeSpace+2,(edgeSpace/2),0]){
      cube([moduleX-reducer,moduleY-edgeSpace,screws]);
    }
  }
}

module microHolder(){
  width= 34;
length = 51-0.1;
height = 4.5;

left = [40,0,0];
front = [-34,1,0];
under = [0,0,-10];
over = [0,0,10];
   translate([0,0,0]){
      cube([13,1.3,7.5]);
    }
    translate([width-13,0,0]){
      cube([13,1.3,7.5]);
    }
      
    difference(){
      cube([width,length,height]);
      
      // Inner
      translate([8,1.3,2.65]){
        cube([18,length-2,3]); // Where arduino sits
      }
      
      smallHoles = 0.6;
      translate([11,1.3,0]){
        cube([12,length-2,3]); // remove middle short sides
        translate([-1.5,1.5,-1]) cylinder(h=20, r=smallHoles, $fn=10);
        translate([12+1.5,1.5,-1]) cylinder(h=20, r=smallHoles, $fn=10);
        
      }
      translate([8,4.3,0]){
        cube([18,length-8,3]); // remove middle longsides
        
        translate([1.5,length-8+1.5,-1]) cylinder(h=20, r=smallHoles, $fn=10);
        translate([18-1.5,length-8+1.5,-1]) cylinder(h=20, r=smallHoles, $fn=10);
      }
      
      // Outer
      translate([0,2,2]){
        cube([7,length,3]);
      }
      translate([27,2,2]){
        cube([7,length,3]);
      }
      
      //holes
      
      in  = 21.5;
      in2 = length-11.4;
      asd=5.4;
      translate([asd,in,0]){
        cylinder(h=20, r=1.5, $fn=10);
      }
      translate([width-asd,in,0]){
        cylinder(h=20, r=1.5, $fn=10);
      }
      
      translate([asd,in2,0]){
        cylinder(h=20, r=1.5, $fn=10);
      }
      translate([width-asd,in2,0]){
        cylinder(h=20, r=1.5, $fn=10);
      }

    }
}

rSup = (supportH)*4+1;
rSupY = -(supportH)*4+1;
module uSupport(){
  difference(){
    screwHoleDR(3,3,rSup, 2.7);
    screwHoleDR(3,3,rSup-5, 1.5);
  }
}

module bottom(){
  thickness = 8.5;
  difference(){
    cube([moduleX,moduleY,thickness]);
    screws = thickness+2;
    screwHoleD(3,3,screws);
    screwHoleD(3,moduleY-3,screws);
    screwHoleD(moduleX-3,moduleY-3,screws);
    screwHoleD(moduleX-3,3,screws);
    
    reducer = (edgeSpace*2+3);
    translate([edgeSpace/2,edgeSpace+1.5,2]){
      cube([moduleX-edgeSpace,moduleY-reducer,thickness-1]);
    }
    translate([edgeSpace+2,(edgeSpace/2),2]){
      cube([moduleX-reducer,moduleY-edgeSpace,thickness-1]);
    }   
    leng = 51;
    wid = 34;
    thi = thickness;
    translate([(140-wid)/2-0.5, 97.5-leng, 1]){
      cube([wid+0.5,leng+2,thi]);
    }
    
        
    // Big holes
    translate([55.1,55,rSupY+1]){
      screwHoleDR(3,3,rSup, 1.5);
    }
    translate([78.3,55,rSupY+1]){
      screwHoleDR(3,3,rSup, 1.5);
    }

    translate([55.1,73,rSupY+1]){
      screwHoleDR(3,3,rSup, 1.5);
    }
    translate([78.3,73,rSupY+1]){
      screwHoleDR(3,3,rSup, 1.5);
    }

    // -- Small holes
    // Inner
    translate([moduleX/2-10.5,moduleY-51.7,rSupY+1]){
      screwHoleDR(3,3,rSup, 0.6);
    }
    translate([moduleX/2+4.5,moduleY-51.7,rSupY+1]){
      screwHoleDR(3,3,rSup, 0.6);
    }
    
    // Outer
    translate([moduleX/2-10.5,moduleY-5.8,rSupY+1]){
      screwHoleDR(3,3,rSup, 0.6);
    }
    translate([moduleX/2+4.5,moduleY-5.8,rSupY+1]){
      screwHoleDR(3,3,rSup, 0.6);
    }
    //--
  }
}

if(top){
  difference(){
    cube([moduleX,moduleY,moduleZ]);
    keyHoles(1,1,2,7);
    keyHoles(2,2,1,5);
    keyHoles(2,2,6.5,6.5);
    keyHoles(3,5,1,6);
    //keyHoles(3.6,3.6,7,7);
    i=3.43;
    keyHoles2(i,i,7.06,7.06, [keyY, keyX, moduleZ+2]);  
    keyHoles(5,5,7,7);

    
    screwHole(3,3);
    screwHole(3,moduleY-3);
    screwHole(moduleX-3,moduleY-3);
    screwHole(moduleX-3,3);
  }
  
}

if(beams){
  hBeam(2, 3);
  hBeam2(1, 7.5,4,4);
  hBeam(5,5);

  vBeam(2, 6);
  vBeam2(6.3, 6.3, 1.8,1.7);
  vBeam2(6.85, 6.85, 0.91,1.5);
  vBeam2(7.5, 7.5, 1.8,1.7);
  vBeam2(6.85, 6.85, 2.8,4.54);
}

if(spacer){
  translate([0,0,-(supportH+1)*2]){
    spacer();
  }
}

if(uSupports){
  translate([55.1,55,rSupY]){
    uSupport();
  }
  translate([78.3,55,rSupY]){
    uSupport();
  }
  translate([55.1,73,rSupY]){
    uSupport();
  }
  translate([78.3,73,rSupY]){
    uSupport();
  }
}

if(uHolder){
  translate([0,60,0]){
  translate([(moduleX/2)+34/2, moduleY, -(supportH)*4-2.7]){
    rotate(180){
      microHolder();
    }
  }
}
}

if(bottom){
translate([0,0,rSupY-4.8]){
  bottom();
}
}
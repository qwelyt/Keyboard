/* Module A v2
 * Spacer
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



translate([0,0,0]){
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

/* ModuleA
 * backplate
 */
keyCols = 7;
keyRows = 6;
keyX = 15;
keyY = 12.5;
keySpace = 3.5;
edgeSpaceAddition = 5;
edgeSpace = (edgeSpaceAddition*2)-keySpace;
keyZ = -1;

moduleX = (edgeSpace+edgeSpaceAddition)+(keySpace*keyCols)+keyX*(keyCols-1)+keyX;
moduleY = (edgeSpace+edgeSpaceAddition)+(keySpace*keyRows)+keyY*(keyRows-1)+keyY;
moduleZ = 2;

keySize = [keyX, keyY, moduleZ+2];

function position(x,y,z) = [edgeSpace+keySpace*x+keyX*(x-1)-(edgeSpaceAddition/2), edgeSpace+keySpace*y+keyY*(y-1)-(edgeSpaceAddition/2), z];

module keyHoles (yStart, yEnd, xStart, xEnd){
    for(y = [yStart:yEnd]){
        for(x = [xStart:xEnd]){
			translate(position(x,y,keyZ)) cube(keySize);
        }
    }
}

module screwHole (x, y){
	translate([x,y,keyZ]) cylinder(h=moduleZ+2, r=2, $fn=10);
}

difference(){
    cube([moduleX,moduleY,moduleZ]);

	screwHole(3,3);
	screwHole(3,moduleY-3);
	screwHole(moduleX-3,moduleY-3);
	screwHole(moduleX-3,3);
}

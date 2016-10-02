/* ModuleA
 * All keyholes made. Should be valid for creation.
 */

keyCols = 7;
keyRows = 6;
keyX = 15;
keyY = 12.5;
keySpace = 3.5;
edgeSpace = 5-keySpace;
keyZ = -1;

moduleX = (edgeSpace+5)+(keySpace*keyCols)+keyX*(keyCols-1)+keyX;
moduleY = (edgeSpace+5)+(keySpace*keyRows)+keyY*(keyRows-1)+keyY;
moduleZ = 1;

keySize = [keyX, keyY, moduleZ+2];

function position(x,y,z) = [edgeSpace+keySpace*x+keyX*(x-1), edgeSpace+keySpace*y+keyY*(y-1), z];

module keyHoles (yStart, yEnd, xStart, xEnd){
    for(y = [yStart:yEnd]){
        for(x = [xStart:xEnd]){
			translate(position(x,y,keyZ)) cube(keySize);
        }
    }
}

difference(){
    cube([moduleX,moduleY,moduleZ]);

	keyHoles(1,1,1,5);
	keyHoles(1,1,6.5,6.5);

	keyHoles(2,2,1,7);

	keyHoles(3,4,1.5,1.5);
	keyHoles(3,4,3,7);

	keyHoles(5,6,3,7);
}

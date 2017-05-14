/* KeySizeTest
 * Make a small 2x2 matrix to test keyholes.
 */
keyCols = 2;
keyRows = 2;
keyX = 15;
keyY = 13;
keySpace = 3.5;
edgeSpaceAddition = 4;
edgeSpace = (edgeSpaceAddition*2)-keySpace;
keyZ = -1;

moduleX = (edgeSpace+edgeSpaceAddition)+(keySpace*keyCols)+keyX*(keyCols-1)+keyX;
moduleY = (edgeSpace+edgeSpaceAddition)+(keySpace*keyRows)+keyY*(keyRows-1)+keyY;
moduleZ = 1;

keySize = [keyX, keyY, moduleZ+2];

function position(x,y,z) = [edgeSpace+keySpace*x+keyX*(x-1)-(edgeSpaceAddition/2), edgeSpace+keySpace*y+keyY*(y-1)-(edgeSpaceAddition/2), z];

module keyHoles (yStart, yEnd, xStart, xEnd){
    for(y = [yStart:yEnd]){
        for(x = [xStart:xEnd]){
			translate(position(x,y,keyZ)) cube(keySize);
        }
    }
}

difference(){
    cube([moduleX,moduleY,moduleZ]);

	keyHoles(1,2,1,2);
}
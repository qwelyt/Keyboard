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

module screwHoleD (x, y, depth){
	translate([x,y,keyZ]) cylinder(h=depth, r=2, $fn=10);
}


thickness = 8.5;


translate([(140-34)/2, 97.5-50, 1]){
color([1,0,0])translate([34,50,0])  rotate([0,0,180]) import("moduleA_arudinoMicro_holder.stl");

}

translate([0,0,0]){
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
    translate([(140-wid)/2, 97.5-leng, 1]){
      cube([wid,leng,thi]);
    }
    translate([(140-34)/2, 97.5-2, 1]){
      //cube([34,4,thickness-2]);
    }
    
    smallHoles = 1.5;
    translate([(140-34)/2, 97.5-50, 1]){
      translate([3.5,14.1,-2]) cylinder(h=20, r=smallHoles, $fn=10);
      translate([3.5,leng-16,-2]) cylinder(h=20, r=smallHoles, $fn=10);
    }
    
    translate([(140-34)/2+27, 97.5-50, 1]){
      translate([3.5,14.1,-2]) cylinder(h=20, r=smallHoles, $fn=10);
      translate([3.5,leng-16,-2]) cylinder(h=20, r=smallHoles, $fn=10);
    }
  }
 }
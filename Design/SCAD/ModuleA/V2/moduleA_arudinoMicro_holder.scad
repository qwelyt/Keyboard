
width= 34;
length = 51-0.1;
height = 4.5;

left = [40,0,0];
front = [-34,1,0];
under = [0,0,-10];
over = [0,0,10];

none = [0,0,0];

color([0,1,0]){
  translate(none){
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
      
      in  = 15;
      in2  = length-in;
      translate([3.5,in,0]){
        cylinder(h=20, r=1.5, $fn=10);
      }
      translate([width-3.5,in,0]){
        cylinder(h=20, r=1.5, $fn=10);
      }
      
      translate([3.5,in2,0]){
        cylinder(h=20, r=1.5, $fn=10);
      }
      translate([width-3.5,in2,0]){
        cylinder(h=20, r=1.5, $fn=10);
      }
      
      
    }
  }
}
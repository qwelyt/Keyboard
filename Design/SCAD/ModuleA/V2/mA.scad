color([1,0,0]){
  translate([0,0,8.5+9]) import("moduleA_top_v2.stl");
}


color([0,1,0]){
  translate([0,0,8.5]){ import("ModuleA_Spacer.stl");}
}

translate([(140-34)/2, 97.5-50, 1]){
color([0,0,1])translate([34,50,0])  rotate([0,0,180]) import("moduleA_arudinoMicro_holder.stl");

}

color ([1,1,0])translate([0,0,0]) import("moduleA_Bottom.stl");
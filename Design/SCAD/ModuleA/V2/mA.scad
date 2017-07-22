color([1,0,0]){
  translate([0,0,9]) import("moduleA_top_v2.stl");
}
color([0,1,0]){
  translate([0,0,0]){ import("ModuleA_Spacer.stl");}
}

color([0,0,1]){
  translate([(140-34)/2, 97.5-50, 0]){
    translate([0,50,0]){
      rotate([180,0,0]) import("moduleA_arudinoMicro_holder.stl");
    }
  }
}

translate([0,0,9]) import("moduleA_Bottom.stl");
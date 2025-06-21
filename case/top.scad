include <common.scad>

module top(){
  tb_surface(display_y);

  translate([total_width-wall_thickness,0,-top_wall_side]) {
    cuboid([wall_thickness,total_depth, top_wall_side], anchor=[-1,-1,-1], rounding=rnd, edges=[FRONT+RIGHT, BACK+RIGHT]);
  }
  translate([0,0,-top_wall_side]) {
    cuboid([wall_thickness,total_depth, top_wall_side], anchor=[-1,-1,-1], rounding=rnd, edges=[FRONT+LEFT, BACK+LEFT]);
  }


  translate([wall_thickness,0,-top_wall_front]) {
    difference(){
      cube([total_width-2*wall_thickness, wall_thickness, top_wall_front]);
    }
  }

  color("pink")
  translate([wall_thickness,total_depth-wall_thickness,-top_wall_back_right_z]) {
    difference(){
      cube([wall_back_right_x, wall_thickness, top_wall_back_right_z]);
    }
  }

  color("orange")
  translate([wall_thickness+wall_back_right_x,total_depth-wall_thickness,-top_wall_back_left_z]) {
    difference(){
      cube([wall_back_left_x, wall_thickness, top_wall_back_left_z]);

     translate([wall_back_left_x-cables_fan_x-cables_fan_offset_x1-perfboard_offset_x,0,cables_fan_offset_z]) cube([cables_fan_x,wall_thickness,cables_fan_z]);
     translate([wall_back_left_x-cables_fan_x-cables_fan_offset_x2-perfboard_offset_x,0,cables_fan_offset_z]) cube([cables_fan_x,wall_thickness,cables_fan_z]);
     
     
    translate([4,0,top_wall_back_right_z+1.5])
      rotate([-90,0,0])
        surface_perfboard_screw("M2,10");
    }
  }

}

top();

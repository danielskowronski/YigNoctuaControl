include <common.scad>

display_board_x = 24.8;
display_board_z = 27.0;
display_screw_offset=2.0;
display_window_offset_z_top=5.1;
display_window_offset_z_bottom=5.5;
display_window_offset_x=0.0;

display_master_offset_z=5+perfboard_thickness+1;


cables_thm_offset_x1=7;
cables_thm_offset_x2=43;
cables_thm_z=2.5;
cables_thm_x=10;

cables_usb_offset_x=5;
cables_usb_x=12;
cables_usb_z=7.5;

module display_screw(){
  translate([0,wall_thickness,0])
    rotate([90,0,0])
      surface_perfboard_screw("M3,10");
}

module display_plate(){

difference(){
    cube([total_width-2*wall_thickness, wall_thickness, bottom_wall_front]);
  
    translate([((total_width-2*wall_thickness)- display_board_x)/2, 0, bottom_wall_front-display_master_offset_z]){
      translate([display_screw_offset,0,-display_screw_offset]) display_screw();
      translate([display_board_x-display_screw_offset,0,-display_screw_offset]) display_screw();
      translate([display_screw_offset,0,-display_board_z+display_screw_offset]) display_screw();
      translate([display_board_x-display_screw_offset,0,-display_board_z+display_screw_offset]) display_screw();
      
      translate([display_window_offset_x, 0, -display_board_z+display_window_offset_z_top])
        cube([(display_board_x-2*display_window_offset_x), wall_thickness, (display_board_z-display_window_offset_z_top-display_window_offset_z_bottom)]);
    }
    }
}

module bottom(){
  tb_surface(display_y);
  
  translate([total_width-wall_thickness,0,-bottom_wall_side]) {
    cuboid([wall_thickness,total_depth, bottom_wall_side], anchor=[-1,-1,-1], rounding=rnd, edges=[FRONT+RIGHT, BACK+RIGHT]);
  }
  translate([0,0,-bottom_wall_side]) {
    cuboid([wall_thickness,total_depth, bottom_wall_side], anchor=[-1,-1,-1], rounding=rnd, edges=[FRONT+LEFT, BACK+LEFT]);
  }
  
  
  translate([wall_thickness,0,-bottom_wall_front]) {
    display_plate();
  }
  
  color("orange")
  translate([wall_thickness,total_depth-wall_thickness,-bottom_wall_back_left_z]) {
    difference(){
      cube([wall_back_left_x, wall_thickness, bottom_wall_back_left_z]);
      translate([perfboard_offset_x+cables_thm_offset_x1,0,0]) cube([cables_thm_x,wall_thickness,cables_thm_z]);
      translate([perfboard_offset_x+cables_fan_offset_x1,0,-cables_fan_z-cables_fan_offset_z]) cube([cables_fan_x,wall_thickness,cables_fan_z]);
      translate([perfboard_offset_x+cables_thm_offset_x2,0,0]) cube([cables_thm_x,wall_thickness,cables_thm_z]);
      translate([perfboard_offset_x+cables_fan_offset_x2,0,-cables_fan_z-cables_fan_offset_z]) cube([cables_fan_x,wall_thickness,cables_fan_z]);
    }
  }

  color("pink")
  translate([wall_thickness+wall_back_left_x,total_depth-wall_thickness,-bottom_wall_back_right_z]) {
    difference(){
      cube([wall_back_right_x, wall_thickness, bottom_wall_back_right_z]);
      translate([cables_usb_offset_x,0,0]) cube([cables_usb_x,wall_thickness,cables_usb_z]);
    }
  }
}

//display_plate();
bottom();

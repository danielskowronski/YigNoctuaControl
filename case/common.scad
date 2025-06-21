include <BOSL2/std.scad>
include <BOSL2/screws.scad>

$fa = 1;
$fs = 0.5;
$fn=36;

perfboard_screw_dia = 2;
perfboard_screw_offset = 1.75; // 2?
perfboard_thickness = 1.5;

perfboard_x=90;
perfboard_y=70;
display_y=5;

wall_thickness=2;
total_width=perfboard_x+2*1+2*wall_thickness; // 96
total_depth=perfboard_y+2*1+2*wall_thickness+display_y;
total_height=5+perfboard_thickness+30+2*wall_thickness;

perfboard_offset_x=(total_width-perfboard_x)/2; // 3

bottom_wall_side=total_height-2*wall_thickness;
bottom_wall_front=total_height-2*wall_thickness;// side with display
bottom_wall_back_left_z=12; // fans and thermal sensors
wall_back_left_x=60; // fans and thermal sensors
wall_back_right_x=total_width-2*wall_thickness-wall_back_left_x; // usb
bottom_wall_back_right_z=25; // usb

top_wall_side=total_height-bottom_wall_side-2*wall_thickness;
top_wall_front=total_height-bottom_wall_front-2*wall_thickness;
top_wall_back_left_z=total_height-2*wall_thickness-bottom_wall_back_left_z;
top_wall_back_right_z=total_height-2*wall_thickness-bottom_wall_back_right_z;


cables_fan_offset_x1=21;
cables_fan_offset_x2=31;
cables_fan_offset_z=-3.25;
cables_fan_z=11;
cables_fan_x=7;

rnd = wall_thickness/2;

module surface_perfboard_screw(screw="M2,10"){
    color("red")
    translate([0,0,wall_thickness+rnd/3])
    screw_hole(screw,head="flat",counterbore=0,anchor=TOP);
}

module tb_surface(y_offset=0){
    difference(){
        cuboid([total_width, total_depth, wall_thickness], anchor=[-1,-1,-1], rounding=rnd, edges=[TOP, "Z"]); // main front/back

        translate([(total_width-perfboard_x)/2+perfboard_screw_offset,(total_depth-perfboard_y+display_y)/2+perfboard_screw_offset,0]) surface_perfboard_screw();
        translate([(total_width+perfboard_x)/2-perfboard_screw_offset,(total_depth-perfboard_y+display_y)/2+perfboard_screw_offset,0]) surface_perfboard_screw();

        translate([(total_width-perfboard_x)/2+perfboard_screw_offset,(total_depth+perfboard_y+display_y)/2-perfboard_screw_offset,0]) surface_perfboard_screw();
        translate([(total_width+perfboard_x)/2-perfboard_screw_offset,(total_depth+perfboard_y+display_y)/2-perfboard_screw_offset,0]) surface_perfboard_screw();

    }

}

/* ModuleA
 * Putting it together
 */

color([0,0,1])
import("moduleA-backplate.stl");

translate([0,0,2])
color([0,1,0])
import("moduleA-middle.stl");

translate([0,0,15])
color([1,0,0])
import("moduleA-top.stl");


// #include <iostream>
#include "rtweekend.h"
#include "camera.h"
#include "hittable.h"
#include "material.h"
#include "hittable_list.h"
#include "sphere.h"
#include "BVH.h"
#include "texture.h"
#include "quad.h"
#include "constant_medium.h"
#include "tri.h"
#include "mesh.h"
#include <sstream>
#include <string>


//Color definitions
color white = hexConvert(0xFFFFFF);
color blue = hexConvert(0x7FB2FF);
color red = hexConvert(0xFF0000);
color green = hexConvert(0x00FF00);
color black = hexConvert(0x000000);
color sky_blue = hexConvert(0xB3CCFF);
color dark_brown = hexConvert(0x522E0E);
color orange_brown = hexConvert(0xC75B12);
color golden_brown = hexConvert(0xC27D0E);
color blue_gray = hexConvert(0x375a66);
color dark_purple = hexConvert(0x16062E);
color sky_orange = hexConvert(0x993800);
color violet = hexConvert(0x6205E6);
color yellow = hexConvert(0xFFB700);
color burnt_yellow = hexConvert(0xAD7D02);
color dark_red = hexConvert(0x660000);
color salmon = hexConvert(0xFF6054);

void bouncing_spheres() {
    hittable_list world;

    auto checker = make_shared<checker_texture>(0.32, color(.2, .3, .1), color(.9, .9, .9));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(checker)));

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    auto center2 = center + vec3(0, random_double(0,.5), 0);
                    world.add(make_shared<sphere>(center, center2, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    world = hittable_list(make_shared<bvh_node>(world));

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 800;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;
    cam.background = sky_blue;

    cam.output_filename = "image.ppm";
    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0.6;
    cam.focus_dist    = 10.0;

    cam.render(world);
}

void checkered_spheres() {
    hittable_list world;

    auto checker = make_shared<checker_texture>(0.32, color(.2, .3, .1), color(.9, .9, .9));

    world.add(make_shared<sphere>(point3(0,-10, 0), 10, make_shared<lambertian>(checker)));
    world.add(make_shared<sphere>(point3(0, 10, 0), 10, make_shared<lambertian>(checker)));

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;
    cam.background = sky_blue;

    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void earth() {
    auto earth_texture = make_shared<image_texture>("earthmap.jpg");
    auto earth_surface = make_shared<lambertian>(earth_texture);
    auto globe = make_shared<sphere>(point3(0,0,0), 2, earth_surface);

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;
    cam.background = sky_blue;

    cam.vfov     = 20;
    cam.lookfrom = point3(0,0,12);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(hittable_list(globe));
}

void perlin_spheres() {
    hittable_list world;

    auto pertext = make_shared<noise_texture>(4);
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(pertext)));
    world.add(make_shared<sphere>(point3(0,2,0), 2, make_shared<lambertian>(pertext)));

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;
    cam.background = sky_blue;

    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void quads() {
    hittable_list world;

    // Materials
    auto left_red     = make_shared<lambertian>(color(1.0, 0.2, 0.2));
    auto back_green   = make_shared<lambertian>(color(0.2, 1.0, 0.2));
    auto right_blue   = make_shared<lambertian>(color(0.2, 0.2, 1.0));
    auto upper_orange = make_shared<lambertian>(color(1.0, 0.5, 0.0));
    auto lower_teal   = make_shared<lambertian>(color(0.2, 0.8, 0.8));

    // Quads
    world.add(make_shared<quad>(point3(-3,-2, 5), vec3(0, 0,-4), vec3(0, 4, 0), left_red));
    world.add(make_shared<quad>(point3(-2,-2, 0), vec3(4, 0, 0), vec3(0, 4, 0), back_green));
    world.add(make_shared<quad>(point3( 3,-2, 1), vec3(0, 0, 4), vec3(0, 4, 0), right_blue));
    world.add(make_shared<quad>(point3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), upper_orange));
    world.add(make_shared<quad>(point3(-2,-3, 5), vec3(4, 0, 0), vec3(0, 0,-4), lower_teal));

    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;
    cam.background = sky_blue;

    cam.vfov     = 80;
    cam.lookfrom = point3(0,0,9);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void simple_light() {
    hittable_list world;

    auto pertext = make_shared<noise_texture>(4);
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(pertext)));
    world.add(make_shared<sphere>(point3(0,2,0), 2, make_shared<lambertian>(pertext)));

    auto difflight = make_shared<diffuse_light>(color(4,4,4));
    world.add(make_shared<sphere>(point3(0,7,0), 2, difflight));
    world.add(make_shared<quad>(point3(3,1,-2), vec3(2,0,0), vec3(0,2,0), difflight));

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;
    cam.background        = black;

    cam.vfov     = 20;
    cam.lookfrom = point3(26,3,6);
    cam.lookat   = point3(0,2,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void cornell_box() {
    hittable_list world;

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));

    world.add(make_shared<quad>(point3(555,0,0), vec3(0,555,0), vec3(0,0,555), green));
    world.add(make_shared<quad>(point3(0,0,0), vec3(0,555,0), vec3(0,0,555), red));
    world.add(make_shared<quad>(point3(343, 554, 332), vec3(-130,0,0), vec3(0,0,-105), light));
    world.add(make_shared<quad>(point3(0,0,0), vec3(555,0,0), vec3(0,0,555), white));
    world.add(make_shared<quad>(point3(555,555,555), vec3(-555,0,0), vec3(0,0,-555), white));
    world.add(make_shared<quad>(point3(0,0,555), vec3(555,0,0), vec3(0,555,0), white));

    shared_ptr<hittable> box1 = box(point3(0,0,0), point3(165,330,165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265,0,295));
    world.add(box1);

    shared_ptr<hittable> box2 = box(point3(0,0,0), point3(165,165,165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(130,0,65));
    world.add(box2);

    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 600;
    cam.samples_per_pixel = 200;
    cam.max_depth         = 50;
    cam.background        = color(0,0,0);

    cam.vfov     = 40;
    cam.lookfrom = point3(278, 278, -800);
    cam.lookat   = point3(278, 278, 0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void cornell_smoke() {
    hittable_list world;

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(7, 7, 7));

    world.add(make_shared<quad>(point3(555,0,0), vec3(0,555,0), vec3(0,0,555), green));
    world.add(make_shared<quad>(point3(0,0,0), vec3(0,555,0), vec3(0,0,555), red));
    world.add(make_shared<quad>(point3(113,554,127), vec3(330,0,0), vec3(0,0,305), light));
    world.add(make_shared<quad>(point3(0,555,0), vec3(555,0,0), vec3(0,0,555), white));
    world.add(make_shared<quad>(point3(0,0,0), vec3(555,0,0), vec3(0,0,555), white));
    world.add(make_shared<quad>(point3(0,0,555), vec3(555,0,0), vec3(0,555,0), white));

    shared_ptr<hittable> box1 = box(point3(0,0,0), point3(165,330,165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265,0,295));

    shared_ptr<hittable> box2 = box(point3(0,0,0), point3(165,165,165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(130,0,65));

    world.add(make_shared<constant_medium>(box1, 0.01, color(0,0,0)));
    world.add(make_shared<constant_medium>(box2, 0.01, color(1,1,1)));

    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 600;
    cam.samples_per_pixel = 200;
    cam.max_depth         = 50;
    cam.background        = color(0,0,0);

    cam.vfov     = 40;
    cam.lookfrom = point3(278, 278, -800);
    cam.lookat   = point3(278, 278, 0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void final_scene(int image_width, int samples_per_pixel, int max_depth) {
    hittable_list boxes1;
    auto ground = make_shared<lambertian>(color(0.48, 0.83, 0.53));

    int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 100.0;
            auto x0 = -1000.0 + i*w;
            auto z0 = -1000.0 + j*w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = random_double(1,101);
            auto z1 = z0 + w;

            boxes1.add(box(point3(x0,y0,z0), point3(x1,y1,z1), ground));
        }
    }

    hittable_list world;

    world.add(make_shared<bvh_node>(boxes1));

    auto light = make_shared<diffuse_light>(color(7, 7, 7));
    world.add(make_shared<quad>(point3(123,554,147), vec3(300,0,0), vec3(0,0,265), light));

    auto center1 = point3(400, 400, 200);
    auto center2 = center1 + vec3(30,0,0);
    auto sphere_material = make_shared<lambertian>(color(0.7, 0.3, 0.1));
    world.add(make_shared<sphere>(center1, center2, 50, sphere_material));

    world.add(make_shared<sphere>(point3(260, 150, 45), 50, make_shared<dielectric>(1.5)));
    world.add(make_shared<sphere>(
        point3(0, 150, 145), 50, make_shared<metal>(color(0.8, 0.8, 0.9), 1.0)
    ));

    auto boundary = make_shared<sphere>(point3(360,150,145), 70, make_shared<dielectric>(1.5));
    world.add(boundary);
    world.add(make_shared<constant_medium>(boundary, 0.2, color(0.2, 0.4, 0.9)));
    boundary = make_shared<sphere>(point3(0,0,0), 5000, make_shared<dielectric>(1.5));
    world.add(make_shared<constant_medium>(boundary, .0001, color(1,1,1)));

    auto emat = make_shared<lambertian>(make_shared<image_texture>("earthmap.jpg"));
    world.add(make_shared<sphere>(point3(400,200,400), 100, emat));
    auto pertext = make_shared<noise_texture>(0.2);
    world.add(make_shared<sphere>(point3(220,280,300), 80, make_shared<lambertian>(pertext)));

    hittable_list boxes2;
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.add(make_shared<sphere>(point3::random(0,165), 10, white));
    }

    world.add(make_shared<translate>(
        make_shared<rotate_y>(
            make_shared<bvh_node>(boxes2), 15),
            vec3(-100,270,395)
        )
    );

    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = image_width;
    cam.samples_per_pixel = samples_per_pixel;
    cam.max_depth         = max_depth;
    cam.background        = color(0,0,0);

    cam.vfov     = 40;
    cam.lookfrom = point3(478, 278, -600);
    cam.lookat   = point3(278, 278, 0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void tri_test() {
    hittable_list world;
    std::string stl_file = "/Users/ryanmeyer1/Desktop/Blender Stuff/TEST_BOT.stl";

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto black = make_shared<lambertian>(color(0,0,0));
    auto light = make_shared<diffuse_light>(color(7, 7, 7));
    auto glass = make_shared<dielectric>(2.5);



    auto panel_texture = make_shared<image_texture>("panel.jpg");
    auto panel_surface = make_shared<lambertian>(panel_texture);
    
    mesh bot = mesh(stl_file, green);
    shared_ptr<hittable> bot_ptr = make_shared<bvh_node>(*bot.get_geometry());

    // Instance transforms
    bot_ptr = make_shared<rotate_y>(bot_ptr, 25.0);
    bot_ptr = make_shared<translate>(bot_ptr, vec3(0,0,0));

    world.add(bot_ptr);

    
    world.add(make_shared<quad>(point3(-10, 0, -10), vec3(20,0,0), vec3(0,0,20), red));
    world.add(make_shared<quad>(point3(-10, 0, 10), vec3(0,20,0), vec3(20,0,0), white));

    world.add(make_shared<quad>(point3(-10, 0, -10), vec3(0,20,0), vec3(0,0,20), green));
    world.add(make_shared<quad>(point3(10, 0, -10), vec3(0,20,0), vec3(0,0,20), green));

    world.add(make_shared<quad>(point3(10, 0, -20), vec3(10,0,0), vec3(0,10,0), light));

    world.add(make_shared<quad>(point3(-5, 20, -5), vec3(10,0,0), vec3(0,0,10), light));

    
    world = hittable_list(make_shared<bvh_node>(world));


    // Setup camera
    camera cam;

    cam.aspect_ratio      = 16.0/9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 30;
    cam.max_depth         = 50;
    cam.background = color(0,0,0);

    cam.vfov     = 50;
    cam.lookfrom = point3(0, 20, -25);
    cam.lookat   = point3(0, 0, 10);
    cam.vup      = vec3(0, 1, 0);

    cam.defocus_angle = 0;

    cam.render(world);
}


//attempt at Fractal Brownian motion terrain generation using Perlin noise
void buildPerlinMap(double noise_map[200][200], int width, int height, double scale, double max_height) {
    perlin noise_gen;
    double second_pass = 0.05;
    double third_pass = 0.005;

    //octave one
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            point3 p(x * scale, y * scale, 0);
            noise_map[y][x] = noise_gen.turb(p, 7) * max_height;
        }
    }
    //octave two
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            point3 p(x * second_pass, y * second_pass, 0);
            noise_map[y][x] += noise_gen.turb(p * 1.5, 7) * max_height * 0.15;
        }
    }
    //octave three
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            point3 p(x * third_pass, y * third_pass, 0);
            noise_map[y][x] += noise_gen.turb(p * 6, 7) * max_height * 0.05;
        }
    }
}

void final_submission() {
    hittable_list world; 
    const int width = 200;
    const int height = 200;

    // Terrain parameters
    double terrain_noise_map[height][width];
    double tile_scale = 1.0;
    double noise_scale = 0.005;
    double max_height = 35.0; 
    double sky_noise_map[height][width];
    
    
    //build height map from perlin noise
    buildPerlinMap(terrain_noise_map, width, height, noise_scale, max_height);

    

    //build terrain from height map
    auto list = make_shared<hittable_list>();
    for (int i = 0; i < height; i = i + 1) {
        for (int j = 0; j < width; j = j + 1) {
            //build the two tris for each tile
            point3 v0 = point3(i * tile_scale, terrain_noise_map[i][j], j * tile_scale);
            point3 v1 = point3((i + 1) * tile_scale, terrain_noise_map[i + 1][j], j * tile_scale);
            point3 v2 = point3(i * tile_scale, terrain_noise_map[i][j + 1], (j + 1) * tile_scale);
            point3 v3 = point3((i + 1) * tile_scale, terrain_noise_map[i + 1][j + 1], (j + 1) * tile_scale);
            
            // Calculate average height for this quad
            double avg_height = (v0.y() + v1.y() + v2.y() + v3.y()) / 4.0;
            double height_factor = std::min(avg_height / max_height, 1.0);
            
            //interpolation function: f = f1 * (1 - t) + f2 * t
            color terrain_color;
            if (height_factor < 0.3) {
                double t = height_factor / 0.3;
                terrain_color = dark_brown * (1 - t) + orange_brown * t;
            } else if (height_factor < 0.5) {
                double t = (height_factor - 0.3) / 0.2;
                terrain_color = orange_brown * (1 - t) + golden_brown * t;
            } else {
                double t = (height_factor - 0.5) / 0.5;
                terrain_color = golden_brown * (1 - t) + yellow * t;
            }
            
            //make and add the two tries, using the 4 vertices and color 
            list->add(make_shared<tri>(v0, v1, v2, make_shared<lambertian>(terrain_color)));
            list->add(make_shared<tri>(v2, v1, v3, make_shared<lambertian>(terrain_color)));

        }
    }
    shared_ptr<hittable> terrain = make_shared<bvh_node>(*list);
    world.add(terrain);


    //build sky quad
    auto sky_texture = make_shared<gradient_texture_3>(yellow, red, dark_purple);
    auto sky = make_shared<quad>(point3(-50, 0, 150), vec3(300, 0, 0), vec3(0, 100, 0), make_shared<lambertian>(sky_texture));
    world.add(sky);


    //build suns 
    auto sun_surface = make_shared<diffuse_light>(burnt_yellow * 50.0);
    auto sun = make_shared<sphere>(point3(50, 25, 155), 10, sun_surface);
    world.add(sun);

    auto mini_sun_surface = make_shared<diffuse_light>(salmon * 50.0);
    auto mini_sun = make_shared<sphere>(point3(20, 35, 150), 5, mini_sun_surface);
    world.add(mini_sun);
    
    auto sun_light_surface = make_shared<diffuse_light>(white * 7.0);
    auto sun_light = make_shared<sphere>(point3(100, 150, 100), 40, sun_light_surface);
    world.add(sun_light);

    //build background fog
    //auto fog_boundary = make_shared<sphere>(point3(100, -10, 170), 95, make_shared<lambertian>(blue_gray));
    //world.add(make_shared<constant_medium>(fog_boundary, 0.000001, dark_purple));

    
    // Setup camera
    camera cam;
    cam.aspect_ratio      = 16.0/9.0;
    cam.image_width       = 1920;
    cam.samples_per_pixel = 500;
    cam.max_depth         = 50;
    cam.background = dark_purple * 0.5;
    cam.vfov     = 40;
    cam.lookfrom = point3(width / 2, 15, 0);
    cam.lookat   = point3(width / 2, 0, 100);
    cam.vup      = vec3(0, 1, 0);
    cam.render(world);
}


int main() {
    switch(11) {
        case 1: bouncing_spheres(); break;
        case 2: checkered_spheres(); break;
        case 3: earth(); break;
        case 4: perlin_spheres(); break;
        case 5: quads(); break;
        case 6: simple_light(); break;
        case 7: cornell_box(); break;
        case 8: cornell_smoke(); break;
        case 9: final_scene(400,   250,  4); break;
        case 10: tri_test(); break;
        case 11: final_submission(); break;
    }
    return 0;
}


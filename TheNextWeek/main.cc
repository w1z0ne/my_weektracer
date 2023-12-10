#include "common/rtweekend.h"
#include "common/camera.h"
#include "common/rtw_stb_image.h"
#include "common/texture.h"

#include "aarect.h"
#include "box.h"
#include "bvh.h"
#include "constant_medium.h"
#include "hittable_list.h"
#include "material.h"
#include "moving_sphere.h"
#include "sphere.h"


#include <iostream>

vec3 color(const ray& r, hittable *world, int depth) {
  hit_record rec;
  if (world->hit(r, 0.0001, MAXFLOAT, rec)) {
    ray scattered;
    vec3 attenuation;
    vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
    if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
      return  attenuation*color(scattered, world, depth+1);
    }
    else {
      return vec3(0,0,0);
    }
  }
  else {
    vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
  }
}

hittable *final() {
  int nb = 23;
  hittable **list = new hittable*[30];
  hittable **boxlist = new hittable*[10000];
  hittable **boxlist2 = new hittable*[10000];
  material *white = new lambertian( new constant_texture(vec3(0.73, 0.73, 0.73)));
  material *red = new lambertian( new constant_texture(vec3(0.65, 0.05, 0.05)) );
  
  material *green = new lambertian( new constant_texture(vec3(0.12, 0.45, 0.15)) );
  material *light = new diffuse_light( new constant_texture(vec3(7, 7, 7)) );
  material *blue=new lambertian(new constant_texture(vec3(0.05,0.05,0.65)));
  material *pink=new lambertian(new constant_texture(vec3(0.8,0.3,0.3)));
  material *ground = new lambertian( new constant_texture(vec3(0.48, 0.83, 0.53)) ); // light green
  material *ball_moving = new lambertian( new constant_texture(vec3(0.83, 0.15, 0.3))); // purplish red
  int b = 0;
  /*for (int i = 0; i < nb; i++) {
    for (int j = 0; j < nb; j++) {
      float w = 100;
      float x0 = -1000 + i*w;
      float z0 = -1000 + j*w;
      float y0 = 0;
      float x1 = x0 + w;
      float y1 = 100*(random_double()+0.01);
      float z1 = z0 + w;
      boxlist[b++] = new box(vec3(x0,y0,z0), vec3(x1,y1,z1), ground);
    }
  }*/
  boxlist[b++] = new box(vec3(-100000,-20,-100000),vec3(100000,0,100000),ground);
  boxlist[b++]=new box(vec3(0,0.01,500),vec3(100,100,600),pink);
  int l = 0;
  list[l++] = new bvh_node(boxlist, b, 0, 1);

  /*material *light = new diffuse_light( new constant_texture(vec3(7, 7, 7)));
  list[l++] = new xz_rect(23, 523, 47, 512, 454, light);*/

  int nx, ny, nn;
  unsigned char *tex_data = stbi_load("assets/xiaohui.jpg", &nx, &ny, &nn, 0);
  material *emat =  new lambertian(new image_texture(tex_data, nx, ny));
  list[l++] = new sphere(vec3(400, 100, 400), 100, emat);

  list[l++]=new sphere(vec3(700,60,600),60,new dielectric(1.5));
  
  list[l++]=new sphere(vec3(770,50,770),50,green);
  list[l++]=new sphere(vec3(-900,100,-900),100,emat);
  list[l++]=new sphere(vec3(-900,100,800),100,red);
  list[l++]=new sphere(vec3(900,40,300),40,blue);
  list[l++]=new sphere(vec3(700,80,350),80,new metal(vec3(0.7,0.6,0.5),0));
  
  list[l++]=new sphere(vec3(850,50,550),50,red);
  //list[l++]=new sphere(vec3(0,50,600),50,pink);
  
  //list[l++]=new sphere(vec3(350,100,70),100,new metal(vec3(0.7,0.6,0.5),0));
  list[l++]=new sphere(vec3(0,90,0),90,new dielectric(1.5));
  

  list[l++] =  new sphere(vec3(-900, 80, 200), 80, new dielectric( 1.5 ));
  //list[l++] =  new sphere(vec3(220, 280, 300), -(80-0.005), new dielectric( 1.5 ));

  /*int ns = 800;
  for (int j = 0; j < ns; j++) {
    boxlist2[j] = new
      sphere( vec3(165*random_double(), 165*random_double(), 165*random_double()), 10, white);
  }
  list[l++] = new
    translate(new rotate_y(new bvh_node(boxlist2, ns, 0.0, 1.0), 15), vec3(-100,270,395));*/
  return new hittable_list(list,l);
}


int main (int argc, char** argv) {

  // default values
  bool SUPER_QUALITY_RENDER = !true;
  bool HIGH_QUALITY_RENDER = !true;
  bool MEDIUM_QUALITY_RENDER = !true;

  // handle command line arguments
  if (argc >= 2) {
    // first command line argument is "SH"?
    if (std::string(argv[1]) == "SH") {
      SUPER_QUALITY_RENDER = true;
    }
    // first command line argument is "HQ"?
    if (std::string(argv[1]) == "HQ") {
      HIGH_QUALITY_RENDER = true;
    }
    // first command line argument is "MQ"?
    if (std::string(argv[1]) == "MQ") {
      MEDIUM_QUALITY_RENDER = true;
    }
  }

  int nx, ny, ns;

  if (SUPER_QUALITY_RENDER) {
    nx = 500;
    ny = 500;
    ns = 10000;
    // nx *= 2; ny *= 2;
    // ns /= 4;
  } else if (HIGH_QUALITY_RENDER) {
    nx = 500;
    ny = 500;
    ns = 100;
  } else if (MEDIUM_QUALITY_RENDER) {
    nx = 400;
    ny = 400;
    ns = 24;
  } else {
    nx = 200;
    ny = 200;
    ns = 10;
  }

  std::cerr << "Total Scanlines: " << ny << std::endl;

  std::cout << "P3\n" << nx << " " << ny << "\n255\n";

  //hittable *world = random_scene();
  //hittable *world = two_spheres();
  //hittable *world = two_perlin_spheres();
  //hittable *world = globe();
  //hittable *world = simple_light();
  //hittable *world = cornell_box();
  //hittable *world = cornell_box_blocks();
  //hittable *world = cornell_balls();
  //hittable *world = cornell_smoke();
  //hittable *world = cornell_final();
  hittable *world = final();

  // vec3 lookfrom(13,2,3);
  // vec3 lookat(0,0,0);
  // float dist_to_focus = 10.0;
  // float aperture = 0.0;
  // float vfov = 20.0;

  // vec3 lookfrom(278, 278, -800);
  // vec3 lookat(278,278,0);
  // float dist_to_focus = 10.0;
  // float aperture = 0.0;
  // float vfov = 40.0;

  /*vec3 lookfrom(478, 278, -600);
  vec3 lookat(290,278,0); */
  vec3 lookfrom(1600,278,800);
  vec3 lookat(400,278,400);
  float dist_to_focus = 10.0;
  float aperture = 0.0;
  float vfov = 40.0;

  camera cam(lookfrom, lookat, vec3(0,1,0), vfov,
             float(nx)/float(ny), aperture, dist_to_focus,
             0.0, 1.0);

  for (int j = ny-1; j >= 0; j--) {
    std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
    for (int i = 0; i < nx; i++) {
      vec3 col(0, 0, 0);
      for (int s = 0; s < ns; s++) {
        float u = float(i + random_double() - (1.0/2.0)) / float(nx);
        float v = float(j + random_double() - (1.0/2.0)) / float(ny);
        ray r = cam.get_ray(u, v);
        col += color(r, world, 0);
      }
      col /= float(ns);


      // gamma 2 correction -> pow(1/gamma) aka square root
      col = vec3( sqrt(col[0]), sqrt(col[1]), sqrt(col[2]) );

      int ir = int(255.99*col[0]);
      int ig = int(255.99*col[1]);
      int ib = int(255.99*col[2]);

      std::cout << ir << " " << ig << " " << ib << "\n";
    }
  }
  std::cerr << "\nDone.\n";

}

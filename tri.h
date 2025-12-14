#ifndef TRI_H
#define TRI_H

#include "hittable.h"
#include "hittable_list.h"

class tri : public hittable {
  public:
    tri(const vec3& v1, const vec3& v2, const vec3& v3, shared_ptr<material> mat)
      : v1(v1), v2(v2), v3(v3), mat(mat)
    {
        // Compute the triangle normal and plane constant D.
        auto n = cross(v2 - v1, v3 - v1);
        normal = unit_vector(n);

        D = dot(normal, v1);
        w = n / dot(n, n);

        set_bounding_box();
    }

    virtual void set_bounding_box() {
        // Compute the bounding box of all three vertices.
        auto min_point = point3(
            std::fmin(std::fmin(v1.x(), v2.x()), v3.x()),
            std::fmin(std::fmin(v1.y(), v2.y()), v3.y()),
            std::fmin(std::fmin(v1.z(), v2.z()), v3.z())
        );
    
        auto max_point = point3(
            std::fmax(std::fmax(v1.x(), v2.x()), v3.x()),
            std::fmax(std::fmax(v1.y(), v2.y()), v3.y()),
            std::fmax(std::fmax(v1.z(), v2.z()), v3.z())
        );
    
        bbox = aabb(min_point, max_point);
    }

    aabb bounding_box() const override { return bbox; }

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        auto denom = dot(normal, r.direction());

        // No hit if the ray is parallel to the triangle plane.
        if (std::fabs(denom) < 1e-8)
            return false;

        // Return false if the hit point parameter t is outside the ray interval.
        auto t = (D - dot(normal, r.origin())) / denom;
        if (!ray_t.contains(t))
            return false;

        // Compute the intersection point.
        auto intersection = r.at(t);

        // Compute barycentric coordinates using signed areas
        vec3 area_abc = cross(v2 - v1, v3 - v1);  
        
        vec3 na = cross(v3 - v2, intersection - v2);  
        vec3 nb = cross(v1 - v3, intersection - v3);  
        vec3 nc = cross(v2 - v1, intersection - v1);  
        
        // Use dot product to get SIGNED area ratios
        double u = dot(na, area_abc) / dot(area_abc, area_abc);
        double v = dot(nb, area_abc) / dot(area_abc, area_abc);
        double w = dot(nc, area_abc) / dot(area_abc, area_abc);

        if (!is_interior(u, v, w, rec))
            return false;

        rec.t = t;
        rec.p = intersection;
        rec.mat = mat;
        rec.set_face_normal(r, normal);

        return true;
    }

    virtual bool is_interior(double u, double v, double w, hit_record& rec) const {
        // Check if barycentric coordinates are all non-negative (point is inside triangle)
        if (u < 0 || v < 0 || w < 0)
            return false;

        // Set UV coordinates for texture mapping (using first two barycentric coords)
        rec.u = v;
        rec.v = w;
        return true;
    }

  private:
    vec3 v1,v2,v3;
    vec3 w;
    shared_ptr<material> mat;
    aabb bbox;
    vec3 normal;
    double D;
};

#endif
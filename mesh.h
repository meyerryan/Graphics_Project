#ifndef MESH_H
#define MESH_H

#include "hittable_list.h"
#include "tri.h"
#include <fstream>
#include <sstream>
#include <string>
#include <array>

class mesh {
public:
    mesh(const std::string& path, const shared_ptr<material>& mat) {
        geometry = load_ascii_stl(path, mat);
    }

    shared_ptr<hittable_list> get_geometry() const {
        return geometry;
    }

private:
    shared_ptr<hittable_list> geometry;

    shared_ptr<hittable_list> load_ascii_stl(const std::string& path, const shared_ptr<material>& mat) {
        auto list = make_shared<hittable_list>();

        std::ifstream in(path);
        if (!in) {
            std::cerr << "Failed to open STL: " << path << "\n";
            return list;
        }

        std::string line;
        std::array<point3, 3> verts;
        int v = 0;

        while (std::getline(in, line)) {
            std::istringstream iss(line);
            std::string token;
            iss >> token;
            if (token == "vertex") {
                double x, y, z;
                if (!(iss >> x >> y >> z)) continue;
                verts[v++] = point3(x, y, z);
                if (v == 3) {
                    list->add(make_shared<tri>(verts[0], verts[1], verts[2], mat));
                    std::cout << "tri added: " << verts[0] << ", " << verts[1] << ", " << verts[2] << "\n";
                    v = 0;
                }
            }
        }
        return list;
    }
};

#endif
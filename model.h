#pragma once

#include <vector>
#include <string>
#include "geometry.h"

class Model {
    vec3 pos{};
    std::string filename;
    std::vector<vec3> verts{};
    std::vector<vec2> tex_coord{};
    std::vector<vec3> norms{};
    std::vector<int> facet_vrt{};
    std::vector<int> facet_tex{};
    std::vector<int> facet_nrm{};
public:
    Model(const std::string filename);
    std::string GetFilename() const;
    vec3 GetPosition() const;
    int GetNumberOfVertices() const;
    int GetNumberOfFaces() const;
    vec3 GetVert(const int i) const;
    vec3 GetVert(const int iface, const int nthvert) const;
    vec3 GetNormal(const int i) const;
    vec3 GetNormal(const int iface, const int nthvert) const;
    vec2 GetTexcoord(const int i) const;
    vec2 GetTexcoord(const int iface, const int nthvert) const;
};


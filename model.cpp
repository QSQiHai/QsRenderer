#include <iostream>
#include <fstream>
#include <sstream>
#include "model.h"

Model::Model(const std::string filename) : filename(filename) {
	std::ifstream in;
	in.open(filename, std::ifstream::in);
	if (in.fail()) return;
	std::string line;
	while (!in.eof()) {
		std::getline(in, line);
		std::istringstream iss(line.c_str());
		char trash;
		if (!line.compare(0, 2, "v ")) {
			iss >> trash;
			vec3 v;
			for (int i = 0; i < 3; i++) iss >> v[i];
			verts.push_back(v);
		}
		else if (!line.compare(0, 3, "vn ")) {
			iss >> trash >> trash;
			vec3 n;
			for (int i = 0; i < 3; i++) iss >> n[i];
			norms.push_back(n.normalize());
		}
		else if (!line.compare(0, 3, "vt ")) {
			iss >> trash >> trash;
			vec2 uv;
			for (int i = 0; i < 2; i++) iss >> uv[i];
			tex_coord.push_back({ uv.x, 1 - uv.y });
		}
		else if (!line.compare(0, 2, "f ")) {
			int f, t, n;
			iss >> trash;
			int cnt = 0;
			while (iss >> f >> trash >> t >> trash >> n) {
				facet_vrt.push_back(--f);
				facet_tex.push_back(--t);
				facet_nrm.push_back(--n);
				cnt++;
			}
			if (3 != cnt) {
				std::cerr << "Error: the obj file is supposed to be triangulated" << std::endl;
				in.close();
				return;
			}
		}
	}
	in.close();
}

vec3 Model::GetPosition() const {
	return pos;
}

std::string Model::GetFilename() const {
	return filename;
}

int Model::GetNumberOfVertices() const {
	return (int)verts.size();
}

int Model::GetNumberOfFaces() const {
	return (int)facet_vrt.size() / 3;
}



vec3 Model::GetVert(const int i) const {
	return verts[i];
}

vec3 Model::GetVert(const int iface, const int nthvert) const {
	return verts[facet_vrt[iface * 3 + nthvert]];
}



vec2 Model::GetTexcoord(const int i) const {
	return tex_coord[i];
}

vec2 Model::GetTexcoord(const int iface, const int nthvert) const {
	return tex_coord[facet_tex[iface * 3 + nthvert]];
}



vec3 Model::GetNormal(const int i) const {
	return norms[i];
}

vec3 Model::GetNormal(const int iface, const int nthvert) const {
	return norms[facet_nrm[iface * 3 + nthvert]];
}



#pragma once
#include "Hitable.h"
#include <fstream>
#include "Triangle.h"
#include <vector>
#include <iostream>
#include <string>
#include "BVH.h"

void LoadOBJ1(std::string filename, std::vector<vertex>& Indices, std::vector<vec3>& Points, std::vector<vec3>& Normals, std::vector<vec3>& UVWs);

class mesh : public hitable
{
public:
	mesh(std::vector<vertex>& indices, std::vector<vec3>& points, std::vector<vec3>& normals, std::vector<vec3>& uvws, material* m);
	
	bool hit(const ray& ray, float & t_min, float & t_max, hit_record& rec) const override;
	bool bounding_box(float t0, float t1, aabb& box) const override;
	vec3 GetUVW(const int uvw_index);
	vec3 GetPosition(const int position_index);
	vec3 GetNormal(const int uvw_index);
	int GetTriangleCount() const;
	hitable** AsHitableList();
	hitable* AsBVH();
	static mesh* LoadMesh(char* filepath, material* mat);
	material* GetMaterial();
protected:
	material*				mat_ptr;
	std::vector<vertex>		Indices;
	std::vector<vec3>		Points;
	std::vector<vec3>		UVWs;
	std::vector<vec3>		Normals;
	std::vector<triangle*>	WorldTriangles;
	aabb					BoundingBox;
};

inline mesh::mesh(std::vector<vertex>& indices, std::vector<vec3>& points, std::vector<vec3>& normals, std::vector<vec3>& uvws, material* m)
	:Indices(indices), Points(points), Normals(normals), UVWs(uvws), mat_ptr(m)
{

	for (int i = 0; i < indices.size(); i += 3)
	{
		vertex v1 = indices[i];
		vertex v2 = indices[i + 1];
		vertex v3 = indices[i + 2];
		triangle* t = new triangle(v1, v2, v3, this);// reinterpret_cast<mesh*>(const_cast<mesh*>(this)));
		aabb temp = aabb(vec3(1e6, 1e6, 1e6), vec3(-1e6, -1e6, -1e6));
		t->bounding_box(0, 1, temp);
		BoundingBox = aabb::surrounding_box(BoundingBox, temp);
		WorldTriangles.push_back(t);
	}
}

inline bool mesh::hit(const ray& ray, float & t_min, float & t_max, hit_record& rec) const
{
	bool hit = false;
	for (int i = 0; i < WorldTriangles.size(); ++i)
	{
		if (WorldTriangles[i]->hit(ray, t_min, t_max, rec))
		{
			hit = true;
			rec.mat_ptr = mat_ptr;
		}
	}
	return hit;
}

inline bool mesh::bounding_box(float t0, float t1, aabb& box) const
{
	box = BoundingBox;
	return true;
}

inline vec3 mesh::GetUVW(const int uvw_index)
{
	return UVWs[uvw_index];
}

inline vec3 mesh::GetPosition(const int position_index)
{
	return Points[position_index];
}

inline vec3 mesh::GetNormal(const int normal_index)
{
	return Normals[normal_index];
}

inline int mesh::GetTriangleCount() const
{
	return WorldTriangles.size();
}

inline hitable** mesh::AsHitableList()
{
	return reinterpret_cast<hitable**>(&WorldTriangles[0]);
}

inline hitable* mesh::AsBVH()
{
	return new bvh_node(reinterpret_cast<hitable**>(&WorldTriangles[0]), GetTriangleCount(), 0, 1);
}

inline mesh* mesh::LoadMesh(char* filepath, material* mat)
{
	std::vector<vertex> indices;
	std::vector<vec3> points;
	std::vector<vec3> normals;
	std::vector<vec3> uvws;
	LoadOBJ1(filepath, indices, points, normals, uvws);
	return new mesh(indices, points, normals, uvws, mat);

}

inline material* mesh::GetMaterial()
{
	return mat_ptr;
}

inline void LoadOBJ1(std::string filename, std::vector<vertex>& Indices, std::vector<vec3>& Points, std::vector<vec3>& Normals, std::vector<vec3>& UVWs)
{

	std::ifstream InputFileStream(filename);
	if (!InputFileStream)
	{
		std::cerr << "Failed to load file: " << filename << std::endl;
	}

	std::string Line;

	int P0, N0, P1, N1, P2, N2;
	int U0, U1, U2;

	int NumberOfVertices = 0;
	int NumberOfNormals = 0;
	int NumberOfFaces = 0;
	int NumberOfUVWs = 0;
	float x, y, z;
	while (!InputFileStream.eof() && !InputFileStream.fail())
	{
		InputFileStream >> Line;
		if (Line == "v")
		{
			NumberOfVertices++;
		}
		else if (Line == "vn")
		{
			NumberOfNormals++;
		}
		else if (Line == "f")
		{
			NumberOfFaces++;
		}
	}
	InputFileStream.clear();
	InputFileStream.seekg(0);
	std::cout << "Normals: " << NumberOfNormals << std::endl;
	std::cout << "Vertices: " << NumberOfVertices << std::endl;
	std::cout << "Faces: " << NumberOfFaces << std::endl;
	std::string temp;

	while (InputFileStream >> temp)
	{
		if (temp == "v")
		{

			InputFileStream >> temp;
			x = atof(temp.c_str());
			InputFileStream >> temp;
			y = atof(temp.c_str());
			InputFileStream >> temp;
			z = atof(temp.c_str());
			vec3 v(x, y, z);
			Points.push_back(v);
		}

		else if (temp == "vt")
		{
			InputFileStream >> temp;
			x = atof(temp.c_str());
			InputFileStream >> temp;
			y = atof(temp.c_str());
			InputFileStream >> temp;
			z = atof(temp.c_str());
			vec3 UVW(x, y, z);
			UVWs.push_back(UVW);
		}
		else if (temp == "vn")
		{
			InputFileStream >> temp;
			x = atof(temp.c_str());
			InputFileStream >> temp;
			y = atof(temp.c_str());
			InputFileStream >> temp;
			z = atof(temp.c_str());
			vec3 Normal(x, y, z);
			Normals.push_back(Normal);
		}
		else if (temp == "f")
		{
			// Vertex//Normal f 1//1 2//2 3//3
			// Vertex/Texture/Normal f 1/1/1 2/3/5 1/5/7
			std::getline(InputFileStream, temp);
			std::string number;
			std::vector<int> tokens;
			int count = 0;
			for (char& ch : temp)
			{
				if (isdigit(ch))
					number += ch;
				else if (ch == '/')
				{
					count++;
					tokens.push_back(std::atoi(number.c_str()));
					number = "";
				}
				else if (isspace(ch))
				{
					if (count > 1)
					{
						tokens.push_back(std::atoi(number.c_str()));
						number = "";
						count = 0;
					}
				}
			}
			P0 = tokens[0];
			U0 = tokens[1];
			N0 = tokens[2];

			P1 = tokens[3];
			U1 = tokens[4];
			N1 = tokens[5];

			P2 = tokens[6];
			U2 = tokens[7];
			N2 = tokens[8];
			Indices.push_back(vertex(P0 - 1, N0 - 1, U0 - 1));
			Indices.push_back(vertex(P1 - 1, N1 - 1, U1 - 1));
			Indices.push_back(vertex(P2 - 1, N2 - 1, U2 - 1));

		}

	}

	InputFileStream.close();
}

#pragma once

#include "Hitable.h"
#include "Ray.h"
#include "Material.h"
#include "AABB.h"

class mesh;

struct vertex
{
	vertex(int p, int n, int u);
	int positionIndex;
	int normalIndex;
	int uvwIndex;
};

inline vertex::vertex(int p, int n, int u)
	: positionIndex(p), normalIndex(n), uvwIndex(u)
{
	
}

class triangle : public hitable
{
public:
	triangle(vertex v0, vertex v1, vertex v2, mesh* parent);
	bool hit(const ray& ray, float & t_min, float & t_max, hit_record& rec) const override;
	bool bounding_box(float t0, float t1, aabb& box) const override;
protected:
	vertex v0;
	vertex v1;
	vertex v2;
	mesh* parent;
	aabb bounds;
};


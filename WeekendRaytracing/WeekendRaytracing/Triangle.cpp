#include "Triangle.h"
#include "Mesh.h"


triangle::triangle(vertex v0, vertex v1, vertex v2, mesh* parent)
	:v0(v0), v1(v1), v2(v2), parent(parent)
{
	bounds = aabb(vec3(1e6, 1e6, 1e6), vec3(-1e6, -1e6, -1e6));
	bounds = bounds + parent->GetPosition(v0.positionIndex);
	bounds = bounds + parent->GetPosition(v1.positionIndex);
	bounds = bounds + parent->GetPosition(v2.positionIndex);
}

bool triangle::hit(const ray& ray, float & t_min, float & t_max, hit_record& rec) const
{
	double T;
	vec3 edge1 = parent->GetPosition(v1.positionIndex) - parent->GetPosition(v0.positionIndex);
	vec3 edge2 = parent->GetPosition(v2.positionIndex) - parent->GetPosition(v0.positionIndex);

	vec3 pVec = cross(ray.direction(), edge2);

	double determinant = dot(edge1, pVec);
	if (determinant > -t_min && determinant < t_min)
	{
		return false;
	}
	double invDeterminant = 1.0 / determinant;

	vec3 tVec = ray.origin() - parent->GetPosition(v0.positionIndex);
	float U = dot(tVec, pVec) * invDeterminant;
	if (U < 0.0 || U > 1.0)
	{
		return false;
	}

	vec3 qVec = cross(tVec, edge1);

	double V = dot(ray.direction(), qVec) * invDeterminant;
	if (V < 0.0 || U + V > 1.0)
	{
		return false;
	}

	T = dot(edge2, qVec) * invDeterminant;
	vec3 uvw = parent->GetUVW(v0.uvwIndex)* (1 - U - V) + parent->GetUVW(v1.uvwIndex) * U + parent->GetUVW(v2.uvwIndex) * V;

	if (T > t_min && T < t_max)
	{
		rec.t = T;
		t_max = T;
		rec.normal = ((1 - U - V) * parent->GetNormal(v0.normalIndex)) + (U * parent->GetNormal(v1.normalIndex)) + (V * parent->GetNormal(v2.normalIndex));
		rec.mat_ptr = parent->GetMaterial();
		rec.p = ray(T);
		rec.u = uvw[0];
		rec.v = uvw[1];
		return true;
	}
	return false;

}

bool triangle::bounding_box(float t0, float t1, aabb& box) const
{
	box = bounds;
	return true;
}

#pragma once

#include "Hitable.h"
#include "AABB.h"

class bvh_node : public hitable
{
public:
	bvh_node();
	bvh_node(hitable** l, int n, float time0, float time1);
	
	virtual bool hit(const ray& r, float & tmin, float & tmax, hit_record& rec) const override;
	bool bounding_box(float t0, float t1, aabb& box) const override;

protected:
	hitable* left;
	hitable* right;
	aabb box;
};

inline bvh_node::bvh_node()
{
}

class compare_hitable
{
public:
	int axis;
	
	compare_hitable(int axis)
		:axis(axis)
	{
		
	}

	bool operator()(hitable* a, hitable* b) const
	{
		aabb box_left, box_right;
		a->bounding_box(0, 0, box_left);
		b->bounding_box(0, 0, box_right);
		if (box_left.min()[axis] - box_right.min()[axis] < 0.0f)
		{
			return false;
		}
		return true;
	}
};

inline bvh_node::bvh_node(hitable** l, int n, float time0, float time1)
	: left(nullptr), right(nullptr)
{
	aabb* boxes = new aabb[n];
	float* left_area = new float[n];
	float* right_area = new float[n];

	// calculate bounds of all hitables
	aabb main_box;
	l[0]->bounding_box(time0, time1, main_box);
	for (int i = 1; i < n; ++i)
	{
		aabb new_box;
		l[i]->bounding_box(time0, time1, new_box);
		main_box = aabb::surrounding_box(new_box, main_box);
	}

	// split by axis
	int axis = main_box.longest_axis();
	compare_hitable compare_axis(axis);
	std::sort(&l[0], &l[n - 1], compare_axis);
	for (int i = 0; i < n; ++i)
	{
		l[i]->bounding_box(time0, time1, boxes[i]);
	}

	left_area[0] = boxes[0].area();
	aabb left_box = boxes[0];
	for (int i = 1; i < n - 1; ++i)
	{
		left_box = aabb::surrounding_box(left_box, boxes[i]);
		left_area[i] = left_box.area();
	}

	right_area[n - 1] = boxes[n - 1].area();
	aabb right_box = boxes[n - 1];
	for (int i = n - 2; i > 0; --i)
	{
		right_box = aabb::surrounding_box(right_box, boxes[i]);
		right_area[i] = right_box.area();
	}

	float min_SAH = FLT_MAX;
	int min_SAH_idx = 0;
	for (int i = 0; i < n - 1; ++i)
	{
		float SAH = i * left_area[i] + (n - i - 1) * right_area[i + 1];
		if (SAH < min_SAH)
		{
			min_SAH_idx = i;
			min_SAH = SAH;
		}
	}

	if (min_SAH_idx == 0)
	{
		left = l[0];
	}
	else
	{
		left = new bvh_node(l, min_SAH_idx + 1, time0, time1);
	}

	if (min_SAH_idx == n - 2)
	{
		right = l[min_SAH_idx + 1];
	}
	else
	{
		right = new bvh_node(l + min_SAH_idx + 1, n - min_SAH_idx - 1, time0, time1);
	}
	box = main_box;
}

inline bool bvh_node::hit(const ray& r, float & tmin, float & tmax, hit_record& rec) const
{
	if (box.hit(r, tmin, tmax))
	{
		hit_record left_record, right_record;
		bool left_hit = left->hit(r, tmin, tmax, left_record);
		bool right_hit = right->hit(r, tmin, tmax, right_record);

		if (left_hit && right_hit)
		{
			if (left_record.t < right_record.t)
			{
				rec = left_record;
			}
			else
			{
				rec = right_record;
			}
			return true;
		}
		else if (left_hit)
		{
			rec = left_record;
			return true;
		}
		else if (right_hit)
		{
			rec = right_record;
			return true;
		}
		else
		{
			return false;
		}
	}
	
	return false;
}

inline bool bvh_node::bounding_box(float t0, float t1, aabb& b) const
{
	b = box;
	return true;
}

#pragma once

#include <Eigen/core>
#include <Eigen/geometry>

namespace gtrace
{
namespace material
{
	class GMaterial;
}

namespace geometry 
{
	float Deg2Rad(float degree);
	float Rad2Deg(float radian);
	float clamp(float value, float min=0.0f, float max=1.0f);
	float random(float min, float max);
	
	class GRay
	{
		public:
			GRay();
			GRay(Eigen::Vector3f direction);
			GRay(Eigen::Vector3f origin, Eigen::Vector3f direction);

			Eigen::Vector3f m_direction;
			Eigen::Vector3f m_origin;

	};

	class GBody;

	class GHit
	{
	public:
		typedef enum Fields { DISTANCE = 1, POSITION = 2, NORMAL = 4, TEXCOORDS = 8, ALL = 15 };
		int fields;
		float distance;
		Eigen::Vector3f position;
		Eigen::Vector3f normal;
		float u,v;
		const GBody* obj;
	};

	class GBody
	{
		public:
			GBody(Eigen::Vector3f position=Eigen::Vector3f::Zero());

			virtual ~GBody() {}
			virtual bool Intersect(const GRay& ray, GHit& hit) const = 0;
			// future ideas:

			// -- Materials:
			// should also be able to calculate texture coordinates for an intersection
			// could return many texture coordinates, according to how many texture coordinate channels it has
			// texture coordinate channels could have a texture coordinate unwrapper assigned to them, which calculates the coordinates
			// based on the tex coordinates, the material of te object can be calculated
			// material has different properties, which should have resolvers, which could return a constant value, or lookup in a texture based on a texture coordinate channel

			// -- Lighting:
			// Should also be able to return a random point on the surface, which will be good for monte carlo simulations (shadow rays, area lights etc)
			const gtrace::material::GMaterial* GetMaterial() const { return m_material; }
			void SetMaterial(gtrace::material::GMaterial* material) { m_material = material; }
			const Eigen::Vector3f GetPosition()const { return m_position; }

			virtual Eigen::Vector3f GetSamplePoint(int n, int count)const =0;

		protected:
			gtrace::material::GMaterial* m_material;
			Eigen::Vector3f m_position;

	};

}
}
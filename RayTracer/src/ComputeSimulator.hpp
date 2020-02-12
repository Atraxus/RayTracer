//#define FAR_CLIP 1000.0f
//
//#include "glm/glm.hpp"
//#include <algorithm>
//
//
//struct Triangle {
//	glm::vec3 pointA, pointB, pointC;
//	glm::vec4 color;
//};
//
//class ComputeSimulator 
//{
//// Structs
//private:
//	struct Camera {
//		glm::vec3 position;
//		glm::vec3 direction;
//		glm::vec3 yAxis;
//		glm::vec3 xAxis;
//		float tanFovY;
//		float tanFovX;
//	};
//
//	struct Ray {
//		glm::vec3 origin;
//		glm::vec3 direction;
//	};
//
//	struct Light {
//		glm::vec3 position;
//		float intensity;
//	};
//
//// Begin of class
//public:
//
//	ComputeSimulator(Triangle* triangle, float FovX, float FovY, int triCount, int height, int width) :
//		m_TriangleCount(triCount),
//		m_Width(height),
//		m_Height(width)
//	{
//		m_Camera;
//		m_Triangles = triangle;
//
//
//		m_Camera.position = glm::vec3(0.0f, 0.0f, 4.0f);
//		m_Camera.direction = glm::vec3(0.0f, 0.0f, -1.0f);
//		m_Camera.xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
//		m_Camera.yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
//
//		m_Camera.tanFovX = FovX;
//		m_Camera.tanFovY = FovY;
//
//	}
//
//
//	// Initialize the primary ray for pixel x, y
//	glm::vec3 getNormal(Triangle tri) {
//		// calculate vectors from points A to B and A to C
//		glm::vec3 AB = tri.pointB - tri.pointA;
//		glm::vec3 AC = tri.pointC - tri.pointA;
//
//		//calculate normal
//		glm::vec3 normal = glm::cross(AC, AB); // need to normalize?
//		normal = glm::normalize(normal);
//		return normal;
//	}
//
//	Ray initRay(unsigned int x, unsigned int y)
//	{
//		float halfWidth = float(m_Width) / 2.0f;
//		float halfHeight = float(m_Height) / 2.0f;
//
//		float a = m_Camera.tanFovX * ((float(x) - halfWidth + 0.5f) / halfWidth);
//		float b = m_Camera.tanFovY * ((halfHeight - float(y) - 0.5f) / halfHeight);
//		glm::vec3 direction = glm::normalize((a * m_Camera.xAxis + b * m_Camera.yAxis + m_Camera.direction));
//		return { m_Camera.position, direction };
//	}
//
//	// return FAR_CLIP on miss and 
//	float hitTriangle(Ray ray, Triangle tri)
//	{
//
//		glm::vec3 AB = tri.pointB - tri.pointA;
//		glm::vec3 AC = tri.pointC - tri.pointA;
//
//		//calculate normal
//		glm::vec3 normal = getNormal(tri);
//		float denom = dot(normal, ray.direction);
//		
//
//		if (denom > 0.000001)
//		{
//			//calculate scalar for ray
//			//ray.direction = normalize(ray.direction);
//			//if (denom <= 0.000001) { return FAR_CLIP; } // ray and normal orthogonal?
//
//			glm::vec3 P0 = tri.pointA - ray.origin;
//			float temp = dot(P0, normal);
//
//			float t = temp / denom;
//			if (t < 0.0f) { return FAR_CLIP; } // t goes to opposite direction
//
//			glm::vec3 P = ray.origin + (t * ray.direction); // Point where ray hits plane
//
//			// Compute vectors
//			glm::vec3 AP = P - tri.pointA;
//
//			// Compute dot products
//			float dot00 = dot(AC, AC);
//			float dot01 = dot(AC, AB);
//			float dot02 = dot(AC, AP);
//			float dot11 = dot(AB, AB);
//			float dot12 = dot(AB, AP);
//
//			// Compute barycentric coordinates
//			float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
//			float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
//			float v = (dot00 * dot12 - dot01 * dot02) * invDenom;
//
//			// Check if point is in triangle
//			if ((u >= 0) && (v >= 0) && (u + v < 1)) {
//				return t;
//			}
//		}
//		return FAR_CLIP;
//	}
//
//	Ray calculateReflectionRay(Ray ray, int nearestObjectID, glm::vec3 hitPoint) {
//		Triangle tri = m_Triangles[nearestObjectID];
//		//reflection calculation: https://math.stackexchange.com/questions/13261/how-to-get-a-reflection-vector
//		glm::vec3 normal = getNormal(tri);
//		glm::vec3 rayDirection = ray.direction - (glm::dot((2.0f * ray.direction), normal) / (dot(normal, normal) * dot(normal, normal))) * normal;
//		Ray reflectionRay = { hitPoint, rayDirection };
//		return reflectionRay;
//	}
//
//	glm::vec4 calculateColor(glm::vec3 hitPoint, int colorID, Light light) {
//		float distance = glm::distance(hitPoint, light.position);
//		if (distance >= light.intensity) {
//			return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
//		}
//		float brightness = ((light.intensity - distance) / light.intensity);
//		glm::vec4 originalColor = m_Triangles[colorID].color;
//		return glm::vec4(std::min(1.0f, originalColor.x * brightness), std::min(1.0f, originalColor.y * brightness), std::min(1.0f, originalColor.z * brightness), originalColor.w);
//	}
//
//	glm::vec4 traceRay(Ray ray, glm::vec4 color, unsigned int reflectionDepth) {
//
//		//brute force triangle hits
//		float nearestTriangle = FAR_CLIP;
//		int nearestObjectID;
//		float rayScalar;
//		for (int i = 0; i < 1; i++) {
//
//			//check if ray hits triangle
//			rayScalar = hitTriangle(ray, m_Triangles[i]);
//
//			//save scalar to nearest triangle
//			if (rayScalar < nearestTriangle) {
//				nearestTriangle = rayScalar;
//				nearestObjectID = i;
//			}
//		}
//
//		//if triangle was hit..
//		if (nearestTriangle < FAR_CLIP) {
//			color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
//		}
//		return color;
//	}
//
//
//	void run(void)
//	{
//		// TODO: iterating x and y needed
//		unsigned int x = 0;
//		unsigned int y = 450;
//		glm::vec4 color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
//		Ray ray = initRay(x, y);
//
//
//		//substitute with ReflectionDepth
//		color = traceRay(ray, color, 0);
//		printf("for break");
//
//	}
//
//private:
//	Camera m_Camera;
//	Triangle* m_Triangles;
//
//	int m_Width;
//	int m_Height;
//	int m_TriangleCount;
//
//};
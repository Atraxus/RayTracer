#version 430
#define FAR_CLIP 1000.0f


struct Camera {
	vec4 position;
	vec4 direction;
	float FovX;
	float FovY;
};

// Object types still need to be defined 
struct Object {
	Primitive p;
	int type;
};

struct Triangle {
    vec3 pointA, pointB, pointC;
};

struct Ray{
    vec3 origin;
    vec3 direction;
};

struct Light {
	vec3 position;
	float intensity;
};

layout(std430) buffer PrimitiveBuffer {
	Object objects[];
};

// --- uniforms --- 
uniform Camera camera;
uniform uint width;
uniform uint height;
writeonly uniform image2D outputTexture;

// --- functions ---

// Initialize the primary ray for pixel x, y
Ray initRay(uint x, uint y)
{
	float halfWidth = float(width) / 2.0f;
	float halfHeight = float(height) / 2.0f;

	float a = camera.tanFovX * ((float(x) - halfWidth + 0.5f) / halfWidth);
	float b = camera.tanFovY * ((halfHeight - float(y) - 0.5f) / halfHeight);

	vec3 direction = normalize((a * camera.xAxis + b * camera.yAxis + camera.direction).xyz);

	return Ray(camera.pos.xyz, direction);
}

// return FAR_CLIP on miss and 
float hitTriangle(Ray ray, Triangle tri)
{
	// calculate vectors from points A to B and A to C
    vec3 AB = tri.pointB.xyz - tri.pointA.xyz;
    vec3 AC = tri.pointC.xyz - tri.pointA.xyz;
	//calculate normal
	vec3 normal = cross(AB, AC); // need to normalize?
	

    if(dot(ray.direction, normal) <= 0.000001){ // no definite solution
        return FAR_CLIP;
    } 
	else {
		//calculate scalar for ray
		float denom = dot(normal, ray.direction);
		if (denom <= 0.000001) return FAR_CLIP; // ray and normal orthogonal?

		vec3 P0 = tri.pointA - ray.origin;

		float t = dot(P0, normal) / denom;
		if (t < 0) return FAR_CLIP; // t goes to opposite direction

		vec3 P = ray.origin + (t * ray.direction); // Point where ray hits plane

		// Compute vectors
		vec3 AP = P - tri.pointA.xyz;

		// Compute dot products
		float dot00 = dot(AC, AC);
		float dot01 = dot(AC, AB);
		float dot02 = dot(AC, AP);
		float dot11 = dot(AB, AB);
		float dot12 = dot(AB, AP);

		// Compute barycentric coordinates
		float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
		float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
		float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

		// Check if point is in triangle
		if ((u >= 0) && (v >= 0) && (u + v < 1))
			return t;
		else
			return FAR_CLIP;
    }
}

vec4 calculateColor(vec3 hitPoint, int objectID, Light light) {
	float distance = distance(hitPoint, light.position);
	if (distance >= light.intensity) {
		vec4 color(0.0f, 0.0f, 0.0f, 1.0f);
		return color;
	}
	float brightness = ((light.intensity - distance) / light.intensity);
	//TODO needs to be changed accoring to color buffer
	vec4 originalColor = object[objectID].color;
	vec4 color(min(1, originalColor.x * brightness), min(1, originalColor.y * brightness), min(1, originalColor.z * brightness, originalColor.w));
	return color;
}

layout(local_size_variable) in; // work group size = 16
void main()
{
    uint x = gl_GlobalInvocationID.x;
    uint y = gl_GlobalInvocationID.y;
	vec4 color(0.0f, 0.0f, 0.0f, 1.0f);
	Ray ray = initRay(x, y);

	//brute force triangle hits
	float nearestTriangle = FAR_CLIP;
	int nearestObjectID;
	for (int i = 0; i < objects.length(), i++) {

		//check if ray hits triangle
		float rayScalar = hitTriangle(ray, objects[i]);

		//save scalar to nearest triangle
		if (rayScalar < nearestTriangle) {
			nearestTriangle = rayScalar;
			nearestObjectID = i;
		}
	}

	//if triangle was hit..
	if (nearestTriangle < FAR_CLIP) {

		//calculate hit point
		hitPoint = ray.origin + (rayScalar * ray.direction);

		//create ray to light
		Ray toLight(hitPoint, (light.position - hitPoint));

		//brute force triangles to find shadows
		bool shadow = false;
		for (int j = 0; j < objects.length(), j++) {
			float lightScalar = hitTriangle(toLight, light.position);

			//if shadow was found then set bool and stop searching for more shadows
			if (lightScalar < FAR_CLIP) {
				shadow = true;
				break;
			}
		}
		//if light hits point then calculate color
		if (!shadow) {
			color = calculateColor(hitPoint, nearestObjectID, light);
		}

	}
	imageStore(outputTexture, ivec2(x, y), color);

}
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

layout(std430) buffer PrimitiveBuffer {
	Object objects[];
};

// --- uniforms --- 
uniform Camera camera;
uniform uint width;
uniform uint height;

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
	normal = cross(AB, AC); // need to normalize?
	

    if(dot(ray.direction, normal) <= 0.000001){ // no definite solution
        return FAR_CLIP;
    } 
	else {
		//calculate scalar for ray
		float denom = dot(normal, ray.direction);
		if (denom <= 0.000001) return FAR_CLIP; // ray and normal orthogonal?

		float P0 = tri.pointA - ray.origin;

		float t = dot(P0, normal) / denom;
		if (t < 0) return FAR_CLIP; // t goes to opposite direction

		vec3 P = ray.origin + (tri * ray.direction); // Point where ray hits plane

		// Compute vectors
		AP = P - tri.pointA.xyz;

		// Compute dot products
		dot00 = dot(AC, AC)
		dot01 = dot(AC, AB)
		dot02 = dot(AC, AP)
		dot11 = dot(AB, AB)
		dot12 = dot(AB, AP)

		// Compute barycentric coordinates
		invDenom = 1 / (dot00 * dot11 - dot01 * dot01)
		u = (dot11 * dot02 - dot01 * dot12) * invDenom
		v = (dot00 * dot12 - dot01 * dot02) * invDenom

		// Check if point is in triangle
		if ((u >= 0) && (v >= 0) && (u + v < 1))
			return tri;
		else
			return FAR_CLIP;
    }
}

vec3 calculateColor(vec3 hitPoint, Light light;) {

}

layout(local_size_variable) in; // work group size = 16
void main()
{
    uint x = gl_GlobalInvocationID.x;
    uint y = gl_GlobalInvocationID.y;

	//brute force triangle hits
	Ray ray = initRay(x, y);
	float nearestTriangle = FAR_CLIP;
	for (int i = 0; i < objects.length(), i++) {

		//check if ray hits triangle
		float rayScalar = hitTriangle(ray, objects[i]);

		//save scalar to nearest triangle
		if (rayScalar < nearestTriangle) {
			nearestTriangle = rayScalar;
		}
	}

	//if triangle was hit..
	if (nearestTriangle < FAR_CLIP) {

		//calculate hit point
		hitPoint = ray.origin + (rayScalar * ray.dir);

		//create ray to light
		Ray toLight(hitPoint, (light.position - hitPoint));

		//brute force triangles to find shadows
		bool shadow = false;
		for (int j = 0; j < objects.length(), j++) {
			float lightScalar = hitTriangle(toLight, light.position);

			//if shadow was found then set bool and stop searching for more shadows
			if (lightScalar < Far_CLIP) {
				shadow = true;
				break;
			}
		}
		//if light hits point then calculate color
		if (!shadow) {
			vec3 color = calculateColor(hitPoint, light)
		}

		//TODO: write color onto pixel
	}


}
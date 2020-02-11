#version 430
#define FAR_CLIP 1000.0f


struct Camera {
	vec3 position;
	vec3 direction;
	vec3 yAxis;
	vec3 xAxis;
	float tanFovY;
	float tanFovX;
};

struct Triangle {
    vec3 pointA, pointB, pointC;
	vec4 color;
};

struct Ray{
    vec3 origin;
    vec3 direction;
};

struct Light {
	vec3 position;
	float intensity;
};

layout(std430) buffer TriangleBuffer {
	Triangle triangles[];
};

// --- uniforms --- 
uniform Camera camera;
uniform int width;
uniform int height;
uniform int triangleCount;
uniform Light light;
uniform uint reflectionDepth;

writeonly uniform image2D outputTexture;
// --- functions ---

// Initialize the primary ray for pixel x, y
vec3 getNormal(Triangle tri) {
	// calculate vectors from points A to B and A to C
	vec3 AB = tri.pointB.xyz - tri.pointA.xyz;
	vec3 AC = tri.pointC.xyz - tri.pointA.xyz;
	//calculate normal
	vec3 normal = cross(AB, AC); // need to normalize?
	return normal;
}

Ray initRay(uint x, uint y)
{
	float halfWidth = float(width) / 2.0f;
	float halfHeight = float(height) / 2.0f;

	float a = camera.tanFovX * ((float(x) - halfWidth + 0.5f) / halfWidth);
	float b = camera.tanFovY * ((halfHeight - float(y) - 0.5f) / halfHeight);

	vec3 direction = normalize((a * camera.xAxis + b * camera.yAxis + camera.direction).xyz);

	return Ray(camera.position.xyz, direction);
}

// return FAR_CLIP on miss and 
float hitTriangle(Ray ray, Triangle tri)
{
	// calculate vectors from points A to B and A to C
    vec3 AB = tri.pointB.xyz - tri.pointA.xyz;
    vec3 AC = tri.pointC.xyz - tri.pointA.xyz;

	//calculate normal
	vec3 normal = getNormal(tri);
	

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

Ray calculateReflectionRay(Ray ray, int nearestObjectID, vec3 hitPoint) {
	Triangle tri = triangles[nearestObjectID];
	//reflection calculation: https://math.stackexchange.com/questions/13261/how-to-get-a-reflection-vector
	vec3 normal = getNormal(tri);
	vec3 rayDirection = ray.direction - (dot((2 * ray.direction), normal) / (dot(normal, normal) * dot(normal, normal)))* normal;
	Ray reflectionRay = Ray(hitPoint, rayDirection);
	return reflectionRay;
}

vec4 calculateColor(vec3 hitPoint, int colorID, Light light) {
	float distance = distance(hitPoint, light.position);
	if (distance >= light.intensity) {
		return vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}
	float brightness = ((light.intensity - distance) / light.intensity);
	vec4 originalColor = triangles[colorID].color;
	return vec4(min(1, originalColor.x * brightness), min(1, originalColor.y * brightness), min(1, originalColor.z * brightness), originalColor.w);
}

Color traceRay(Ray ray, vec4 color, uint reflectionDepth) {

	//brute force triangle hits
	float nearestTriangle = FAR_CLIP;
	int nearestObjectID;
	float rayScalar;
	for (int i = 0; i < triangleCount; i++) {

		//check if ray hits triangle
		rayScalar = hitTriangle(ray, triangles[i]);

		//save scalar to nearest triangle
		if (rayScalar < nearestTriangle) {
			nearestTriangle = rayScalar;
			nearestObjectID = i;
		}
	}

	//if triangle was hit..
	if (nearestTriangle < FAR_CLIP) {

		//calculate hit point
		vec3 hitPoint = ray.origin + (rayScalar * ray.direction);

		//create ray to light
		Ray toLight = Ray(hitPoint, (light.position - hitPoint));


		//brute force triangles to find shadows
		bool shadow = false;
		for (int j = 0; j < triangleCount; j++) {
			float lightScalar = hitTriangle(toLight, triangles[j]);

			//if shadow was found then set bool and stop searching for more shadows
			if (lightScalar < FAR_CLIP) {
				shadow = true;
				break;
			}
		}
		//if light hits point then calculate color
		if (!shadow) {
			color += 0.5f * calculateColor(hitPoint, nearestObjectID, light);
		}
		vec4 tempColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
		//calculate reflection ray
		if (reflectionDepth > 0) {
			Ray reflectionRay = calculateReflectionRay(ray, nearestObjectID, hitPoint);
			tempColor = traceRay(reflectionRay, color, reflectionDepth-1);
		}
		return color += 0.5f * tempColor;
	}
}


layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in; // work group size = 8
void main()
{
    uint x = gl_GlobalInvocationID.x;
    uint y = gl_GlobalInvocationID.y;
	vec4 color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	Ray ray = initRay(x, y);
	color = traceRay(ray, color);
	imageStore(outputTexture, ivec2(x, y), color);

}
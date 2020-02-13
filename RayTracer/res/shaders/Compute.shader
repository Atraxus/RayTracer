#version 430
#define FAR_CLIP 1000.0f

vec3 hp;

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
};

struct Ray{
    vec3 origin;
    vec3 direction;
};

struct Light {
	vec3 position;
	float intensity;
};

layout(std430, binding = 1) buffer  APointBuffer {
	vec3 aPoints[];
};

layout(std430, binding = 2) buffer  BPointBuffer {
	vec3 bPoints[];
};

layout(std430, binding = 3) buffer  CPointBuffer {
	vec3 cPoints[];
};

layout(std430, binding = 4) buffer  ColorBuffer {
	vec4 colors[];
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
	if (normal.x <= 0.01 && normal.x >= -0.01) {
		if (normal.y <= 0.01 && normal.y >= -0.01) {
			if (normal.z <= 0.01 && normal.z >= -0.01) {
				imageStore(outputTexture, ivec2(500, 500), vec4(1.0f, 0.0f, 0.0f, -1.0f));
			}
		}
	}
	normal = normalize(normal);
	return normal;
}


Ray initRay(uint x, uint y)
{
	float halfWidth = float(width) / 2.0f;
	float halfHeight = float(height) / 2.0f;

	float a = 2.0f*((float(x) - halfWidth + 0.5f) / halfWidth);
	float b = 2.0f*((halfHeight - float(y) - 0.5f) / halfHeight);
	vec3 direction = normalize((a * camera.xAxis + b * camera.yAxis + vec3(0.0f, 0.0f, 1.0f)).xyz);
	return Ray(camera.position, direction);
}

float hitTriangle(Ray ray, Triangle tri)
{
	
    vec3 AB = tri.pointB.xyz - tri.pointA.xyz;
    vec3 AC = tri.pointC.xyz - tri.pointA.xyz;
	
	//calculate normal
	vec3 normal = getNormal(tri);

	float denom = dot(normal, ray.direction);
	if (denom > 0.000001) 
	{
		//calculate scalar for ray
		//ray.direction = normalize(ray.direction);
		if (isnan(ray.direction.x))
			imageStore(outputTexture, ivec2(400, 1000), vec4(0.0f, 1.0f, 0.0f, 1.0f));
		//if (denom <= 0.000001) { return FAR_CLIP; } // ray and normal orthogonal?

		vec3 P0L0 = tri.pointA - ray.origin;
		float temp = dot(P0L0, normal);

		float t =temp / denom;
		if (isnan(temp))
			imageStore(outputTexture, ivec2(300, 1000), vec4(0.0f, 0.0f, 1.0f, 1.0f));
		if (t < 0.0f) { return FAR_CLIP; } // t goes to opposite direction
		if(isnan(t))
			imageStore(outputTexture, ivec2(100, 200), vec4(0.0f, 1.0f, 0.0f, 1.0f));


		hp = ray.origin + (t * ray.direction); // Point where ray hits plane

		// Compute vectors
		vec3 AP = hp - tri.pointA.xyz;

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
		if ((u >= 0) && (v >= 0) && (u + v < 1)) {
			return t;
		}
    }
	hp = vec3(FAR_CLIP, FAR_CLIP, FAR_CLIP);
	return FAR_CLIP;
}

Ray calculateReflectionRay(Ray ray, int nearestObjectID, vec3 hitPoint) {
	//reflection calculation: https://math.stackexchange.com/questions/13261/how-to-get-a-reflection-vector
	vec3 normal = getNormal(Triangle(aPoints[nearestObjectID], bPoints[nearestObjectID], cPoints[nearestObjectID]));
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
	vec4 originalColor = colors[colorID];
	return vec4(originalColor.x * brightness, originalColor.y * brightness, originalColor.z * brightness, 1);
}

vec4 traceRay(Ray ray, vec4 color, uint reflectionDepth) {

	//brute force triangle hits
	float nearestTriangle = FAR_CLIP;
	int nearestObjectID;
	vec3 nearestHP = vec3(FAR_CLIP, FAR_CLIP, FAR_CLIP);
	float rayScalar;
	for (int i = 0; i < 14; i++) {

		//check if ray hits triangle
		/*if (triangles[11].pointB.x <= 0.01 && triangles[11].pointB.x >= -0.01) {
			if (triangles[11].pointB.y <= 0.01 && triangles[11].pointB.y >= -0.01) {
				if (triangles[11].pointB.z <= 0.01 && triangles[11].pointB.z >= -0.01) {
					imageStore(outputTexture, ivec2(1000, 1000), vec4(0.0f, 1.0f, 0.0f, 1.0f));
				}
			}
		}*/
		rayScalar = hitTriangle(ray, Triangle(aPoints[i], bPoints[i], cPoints[i]));

		//save scalar to nearest triangle
		if (distance(ray.origin, hp)<nearestTriangle) {
			nearestHP = hp;
			nearestTriangle = distance(ray.origin, hp);
			nearestObjectID = i;
		}
	}
	//color = vec4((nearestTriangle/FAR_CLIP)-0.5f, 0.0f, 0.0f, 1.0f);

	//if triangle was hit..
	if (nearestTriangle < FAR_CLIP) {
		color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
		//color = colors[0];
		//calculate hit point
		//vec3 hitPoint = ray.origin + (rayScalar * ray.direction);

		//create ray to light
		Ray toLight = Ray(nearestHP, (light.position - nearestHP));


		//brute force triangles to find shadows
		bool shadow = false;
		for (int j = 0; j <14; j++) {
			float lightScalar = hitTriangle(toLight, Triangle(aPoints[j], bPoints[j], cPoints[j]));

			//if shadow was found then set bool and stop searching for more shadows
			if (distance(nearestHP, hp) < FAR_CLIP) {

				shadow = true;
				break;
			}
		}
		//if light hits point then calculate color
		if (!shadow) {
			color = calculateColor(nearestHP, nearestObjectID, light);
			//color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
		}
		//vec4 tempColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
		//calculate reflection ray
		/*
		if (reflectionDepth > 0) {
			Ray reflectionRay = calculateReflectionRay(ray, nearestObjectID, hitPoint);
			tempColor = traceRay(reflectionRay, color, reflectionDepth-1);
		}
		*/
		 //+= 0.5f * tempColor;
	}
	return color;
}


layout(local_size_x = 16, local_size_y = 16, local_size_z = 1) in; // work group size = 8
void main()
{
	
    uint x = gl_GlobalInvocationID.x;
    uint y = gl_GlobalInvocationID.y;
	for (int i = 0; i < 13; i++) {
		imageStore(outputTexture, ivec2(200+(5*i), 200), colors[i]);
	}
	vec4 color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	Ray ray = initRay(x, y);

	aPoints[0].x = -2.5f;
	aPoints[0].y = -2.5f;
	aPoints[0].z = -7.5f;
	bPoints[0].x =  2.5f;
	bPoints[0].y = -2.5f;
	bPoints[0].z = -7.5f;
	cPoints[0].x = -2.5f;
	cPoints[0].y =  2.5f;
	cPoints[0].z = -7.5f;
	//triangles[0].color = vec4(0.8f, 0.1f, 0.1f, 1.0f);
	aPoints[1].x =  2.5f;
	aPoints[1].y = -2.5f;
	aPoints[1].z = -7.5f;
	bPoints[1].x =  2.5f;
	bPoints[1].y =  2.5f;
	bPoints[1].z = -7.5f;
	cPoints[1].x = -2.5f;
	cPoints[1].y =  2.5f;
	cPoints[1].z = -7.5f;
	//triangles[1].color = vec4(0.8f, 0.1f, 0.1f, 1.0f);
	aPoints[2].x =  2.5f;
	aPoints[2].y =  2.5f;
	aPoints[2].z = -7.5f;
	bPoints[2].x = -2.5f;
	bPoints[2].y = -2.5f;
	bPoints[2].z = -12.5f;
	cPoints[2].x =  2.5f;
	cPoints[2].y = -2.5f;
	cPoints[2].z = -12.5f;


	/*if (triangles[8].pointA.x > 0.0956f) {
		imageStore(outputTexture, ivec2(50, 50), vec4(1.0f, 0.0f, 0.0f, 1.0f));
	}
	*/
	//substitute with ReflectionDepth
	color = traceRay(ray, color, 0);
	imageStore(outputTexture, ivec2(x, y), color);


}
#version 430
#define FAR_CLIP 1000.0f

layout(local_size_variable) in; // work group size = 16

layout(std430) buffer PrimitiveBuffer {
    Object objects[];
};

struct Triangle {
    vec3 A, B, C;
};

struct Ray{
    vec3 origin;
    vec3 dir;
};

// return FAR_CLIP on miss and 
float hitTriangle(Ray r, Triangle t)
{
    vec3 AB = t.B.xyz - t.A.xyz;
    vec3 AC = t.C.xyz - t.A.xyz;
    mat3 mat = mat3(AB, AC, -1.0f * r.dir);

    float det = determinant(mat);

    if(det == 0.0f){ // no definite solution
        return FAR_CLIP;
    } else {
        vec3 oA = r.origin - t.A.xyz;

        mat3 Di = inverse(mat);
        vec3 solution = Di * oA;

        // barycentric coordinates
        if(solution.x >= -0.0001 && solution.x <= 1.0001){
            if(solution.y >= -0.0001 && solution.y <= 1.0001){
                if(solution.x + solution.y <= 1.0001){
                    return solution.z;
                }
            }
        }
        return FAR_CLIP;
    }
}

void main()
{
    uint x = gl_GlobalInvocationID.x;
    uint y = gl_GlobalInvocationID.y;


}
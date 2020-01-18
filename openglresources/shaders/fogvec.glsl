/**********************************************************
 *   fogvec.glsl:  A shader to feed the fogfrag.glsl shader
 *   the indices for the images and the distance from
 *   the viewer to calculate the fog, as well as manage
 *   the position and viewing geometry for the object.
 *   Created by: Edward Charles Eberle <eberdeed@eberdeed.net>
 *   12/2019 San Diego, California USA
 * ********************************************************/
#version 300 es


precision highp float;

const int NUM_IMAGES = 16;
const int NUM_INSTANCES = 30;

struct TexIO {
    vec3 Normal;
    vec3 Position;
    vec2 TexCoord;
    vec4 index1;
    vec2 index2;
    float dist1;
};

in vec3 position;
in vec3 normal;
in vec2 texCoord;

out TexIO texData;

uniform int repetition;
uniform mat4 projection;
uniform mat4 view;

layout (packed) uniform itemData 
{
    vec4 instIndex1[NUM_IMAGES * NUM_INSTANCES];
    vec2 instIndex2[NUM_IMAGES * NUM_INSTANCES];
    vec2 instDist[NUM_IMAGES * NUM_INSTANCES];
    mat4 instModel[NUM_IMAGES * NUM_INSTANCES];
};
mat4 model;
void main( void )
{
    model = instModel[gl_InstanceID + (repetition * NUM_INSTANCES)];
    gl_Position = projection * view * model * vec4(position, 1.0f);
    texData.Normal = normal;
    texData.index1 = instIndex1[gl_InstanceID + (repetition * NUM_INSTANCES)];
    texData.index2 = instIndex2[gl_InstanceID + (repetition * NUM_INSTANCES)];
    texData.dist1 = instDist[gl_InstanceID + (repetition * NUM_INSTANCES)].x;
    texData.TexCoord = texCoord;
}

/**********************************************************
 *   fogvec.glsl:  A shader to provide multiple images in the
 *   sample2DArray uniform for the cube objects, toggle the 
 *   fog using the boolean foggy uniform, and provide 
 *   multiple lighting sources.
 *   Created by: Edward Charles Eberle <eberdeed@eberdeed.net>
 *   12/2019 San Diego, California USA
 * ********************************************************/

#version 300 es

precision mediump float;

const int numlights = 8;

struct Lights {
    vec3 lightPos;
    vec4 lightColor;
};

struct TexIO {
    vec3 Normal;
    vec3 Position;
    vec2 TexCoord;
    vec4 index1;
    vec2 index2;
    float dist1;
};

in TexIO texData;

out vec4 outColor;

uniform Lights lighting[numlights];
uniform vec3 viewPos;
uniform vec4 fogColor;
uniform float fogMaxDist;
uniform float fogMinDist;

uniform sampler2D cratetex;
uniform highp sampler2DArray tex;
uniform int side;
uniform bool foggy;

vec4 CalcDirLight(vec3 light, vec3 normal, vec3 lightDir, vec3 viewDir);
float computeLinearFogFactor();
vec4 texVal;
vec4 tmpVal;
vec3 texVec;
float shininess = 50.0;
vec3 normal;
vec4 rescolor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
float sideIndex[6];

void main()
{
    for (int x = 0; x < 4; x++)
    {
        sideIndex[x] = texData.index1[x];
    }
    sideIndex[4] = texData.index2.x;
    sideIndex[5] = texData.index2.y;  
    float fogFactor = computeLinearFogFactor();
    texVec = vec3(texData.TexCoord.x, texData.TexCoord.y, sideIndex[side]);
    normal = normalize(texData.Normal);
    texVal = mix(texture(cratetex, texData.TexCoord), texture(tex, texVec), 0.3);
    //texVal = texture(cratetex, texData.TexCoord);
    rescolor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    for (int x = 0; x < numlights; x++)
    {
        vec3 I = -normalize(lighting[x].lightPos - texData.Position);
        vec3 viewDir = normalize(viewPos - texData.Position);
        vec3 R = reflect(I, normal);
        //vec3 R = vec3(-5.0, 10.0, 10.0);
        //skyLight = vec3(texture(skybox, R));
        vec4 result = CalcDirLight(lighting[x].lightColor.xyz, normal, R, viewDir);
        rescolor += result;
    }
    rescolor *= 0.6;
    if (!foggy)
    {
        outColor =rescolor;
    }
    else
    {
        outColor = (rescolor * fogFactor) + (fogColor * (1.0 - fogFactor)); 
    } 
}  

vec4 CalcDirLight(vec3 light, vec3 normal, vec3 lightDir, vec3 viewDir)
{
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // Combine results
    vec3 ambient  = 0.2 * texVal.xyz;
    vec3 diffuse  = light  * diff * texVal.xyz;
    vec3 specular = light * spec * texVal.xyz;
    tmpVal = vec4((ambient + diffuse + specular), 1.0);
    return normalize(tmpVal);
    //return vec4((ambient + diffuse), 1.0) * 0.8;
    //return vec4(diff, 0.0, 0.0, 1.0);
} 
float computeLinearFogFactor()
{
   float factor;
   
   // Compute linear fog equation
   factor = (fogMaxDist - texData.dist1) /
            (fogMaxDist - fogMinDist );
   factor *= 2.0;
   // Clamp in the [0,1] range
   factor = clamp(factor, 0.0, 1.0);
            
   return factor;            
}


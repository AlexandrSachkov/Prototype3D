struct PS_INPUT {
    float3 posW : POSITION;
    float3 normalW : NORMAL;
    float3 tangentW : TANGENT;
    float3 bitangentW : BITANGENT;
    float2 uv : TEXCOORD;
};

cbuffer SceneConstants : register(b0) {
    float3 ambientLight;
};

cbuffer Material : register(b1) {
    float3 diffuseColor;
    float shininess;
    float3 specularColor;
    float shininessStrength;
    float3 transparencyColor;
    float reflectivity;
    float3 reflectionColor;
    float refracti;
    float3 emissionColor;
    float opacity;

    int hasDiffuteTex;
    int hasEmissionTex;
    int hasLightmapTex;
    int hasNormalTex;
    int hasOpacityTex;
    int hasReflectionTex;
    int hasShininessTex;
    int hasSpecularTex;
};

struct PointLight {
    float3 position;
    float range;
    float3 ambientColor;
    float constAttenuation;
    float3 diffuseColor;
    float linearAttenuation;
    float3 specularColor;
    float quadraticAttenuation;
};

cbuffer Lights : register(b2) {
    PointLight pointLights[64];
    float3 eyePosition;
    int numPointLights;
}

Texture2D<float4> diffuseTex : register(t0);
Texture2D<float4> emissionTex : register(t1);
Texture2D<float4> lightmapTex : register(t2);
Texture2D<float4> normalTex : register(t3);
Texture2D<float4> opacityTex : register(t4);
Texture2D<float4> reflectionTex : register(t5);
Texture2D<float4> shininessTex : register(t6);
Texture2D<float4> specularTex : register(t7);

SamplerState samplerState : register(s0);

void calculatePointLightContribution(
    float4 matDiffuse, float4 matSpecular,
    float3 pixelPos, float3 normal, float3 toEye,
    out float4 diffuse, out float4 specular
);

float4 main(PS_INPUT input) : SV_TARGET {
    input.normalW = normalize(input.normalW);

    float2 uvFlipped = float2(input.uv.x, -input.uv.y);//flip from OpenGL standard
    float3 toEyeW = normalize(eyePosition - input.posW);

    float4 diffuseColorTex = float4(diffuseColor, opacity);
    if (hasDiffuteTex) {
        diffuseColorTex = diffuseTex.Sample(samplerState, uvFlipped);
    }
    
    float opacityFinal = opacity;
    if (hasOpacityTex) {
        opacityFinal = opacityTex.Sample(samplerState, uvFlipped).r;
    }
    
    clip(opacityFinal < 0.05f ? true : false);

    float4 specularColorTex = float4(specularColor, 1.0f);
    if (hasSpecularTex) {
        specularColorTex = specularTex.Sample(samplerState, uvFlipped);
    }

    float3 normal = input.normalW;
    if (hasNormalTex) {
        input.tangentW = normalize(input.tangentW - dot(input.tangentW, input.normalW)*input.normalW);
        input.bitangentW = normalize(input.bitangentW - dot(input.bitangentW, input.normalW)*input.normalW);

        normal = normalTex.Sample(samplerState, uvFlipped).xyz;
        normal = 2.0f * normal - 1.0f;
        float3x3 TBN = float3x3(input.tangentW, input.bitangentW, input.normalW);
        normal = mul(normal, TBN);
    }

    float4 ambientTotal = float4(ambientLight, 1.0f) * diffuseColorTex;
    float4 diffuseTotal = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 specularTotal = float4(0.0f, 0.0f, 0.0f, 0.0f);

    float4 diffusePoint = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 specularPoint = float4(0.0f, 0.0f, 0.0f, 0.0f);

    calculatePointLightContribution(
        diffuseColorTex, specularColorTex,
        input.posW, normal, toEyeW,
        diffusePoint, specularPoint
    );

    diffuseTotal += diffusePoint;
    specularTotal += specularPoint;

    float4 litColor = ambientTotal + diffuseTotal + specularTotal;
    litColor.a = opacityFinal;
    return litColor;
}

void calculatePointLightContribution(
    float4 matDiffuse, float4 matSpecular, 
    float3 pixelPos, float3 normal, float3 toEye,
    out float4 diffuse, out float4 specular
) {
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

    for (int i = 0; i < numPointLights; i++) {
        PointLight light = pointLights[i];
        float3 lightVec = light.position - pixelPos;
        float distance = length(lightVec);
        if (distance > light.range) {
            return;
        }  

        lightVec /= distance; //normalize

        float diffuseFactor = dot(lightVec, normal);
        if (diffuseFactor > 0.0f) {
            float3 v = reflect(-lightVec, normal);
            float specFactor = pow(max(dot(v, toEye), 0.0f), matSpecular.w);

            diffuse = diffuseFactor * matDiffuse * float4(light.diffuseColor, 1.0f);
            specular = specFactor * matSpecular * float4(light.specularColor, 1.0f);
        }

        float attenuation = 1.0f / dot(
            float3(light.constAttenuation, light.linearAttenuation, light.quadraticAttenuation), 
            float3(1.0f, distance, distance * distance)
        );

        diffuse *= attenuation;
        specular *= attenuation;
    }
}
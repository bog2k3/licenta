attribute vec3 vPos;
attribute vec3 vNormal;
attribute vec4 vColor;
attribute vec2 vUV1;
 
varying vec4 fColor;
varying vec2 fUV1;

uniform mat4 mViewProj;
 
void main() {
    gl_Position = mViewProj * vec4(vPos, 1);
    gl_Normal = mViewProj * vec4(vNormal, 0);
    fColor = vColor;
    fUV1 = vUV1;
}

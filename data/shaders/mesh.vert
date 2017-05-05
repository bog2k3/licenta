attribute vec3 vPos;
attribute vec3 vNormal;
attribute vec4 vColor;
attribute vec2 vUV1;
 
varying vec4 fColor;
varying vec2 fUV1;

uniform mat4 mPVW;
 
void main() {
    gl_Position = mPVW * vec4(vPos, 1);
    gl_Normal = mPVW * vec4(vNormal, 0);
    fColor = vColor;
    fUV1 = vUV1;
}

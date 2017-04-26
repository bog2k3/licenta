varying vec4 fColor;
varying vec2 fUV1;

sampler tex1;

void main() {
	gl_FragColor = fColor * gl_Tex2D(tex1, fUV1);
}

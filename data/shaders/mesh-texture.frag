varying vec4 fColor;
varying vec2 fUV1;

uniform sampler2D tex1;

void main() {
	gl_FragColor = fColor * texture(tex1, fUV1);
}

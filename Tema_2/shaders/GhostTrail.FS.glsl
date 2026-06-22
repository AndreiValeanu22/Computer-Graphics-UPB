#version 330

in vec4 vColor; // Culoarea primita de la Vertex Shader, interpolata pe suprafata fetelor
out vec4 FragColor; // Culoarea finala care va fi scrisa in buffer-ul de imagine al ecranului
uniform float uAlpha; // Valoare intre 0 si 1; ajuta la controlul transparentei cursorului (ex: 0.5 pentru semitransparent)

void main() {
    FragColor = vec4(vColor.rgb, vColor.a * uAlpha); // Seteaza culoarea pixelului; inmultirea cu uAlpha modifica doar canalul de transparenta (Alpha)
}
#version 330 core
in vec4 vertexColor;
in vec4 vertexPosition;

out vec4 FragColor;

void main() {
  FragColor = vertexPosition;
}

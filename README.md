Companion implementation and notes for https://learnopengl.com

---

## Notas


### Introdução

**O que é?**

- OpenGL é uma especificação mantida pelo Khronos Group.
- Funciona como uma grande máquina de estados
- Tem biblioteca implementada em C99

**Immediate vs Core-profile**

Para OpenGL antes da versão 3.2, a operação era mais simples e direta, mas bem mais ineficiente. É o que se chama hoje de immediate mode. A partir da versão 3.2 a maneira antiga é considerada obsoleta e não deve ser usada. Foi substituída pelo Core-profile, que é de nível mais baixo, operação mais complexa, mas muito mais performática e flexível.

**Extensões**

Para que hardware diferentes possam ser suportados com suas novidades, o OpenGL possui extensões além do Core-profile que permitem o uso de recursos mais modernos e avançados. Isso pode ser verificado em runtime e tirado proveito.

```cpp
if(GL_ARB_extension_name) {
    // Do cool new and modern stuff supported by hardware
} else {
    // Extension not supported: do it the old way
}
```

**Binding para outras linguagens**

Para traduzir melhor para bindings com linguagens de mais alto nível, usa structs para implementar o conceito de objetos.


### Criando uma janela

A especificação do OpenGL não entra em detalhes de sistema operacional/plataform, portanto coisas específicas de SO como criação de janelas e input ficam a cargo do implementador. Felizmente há uma série de bibliotecas que abstraem essas diferenças. Este tutorial usa GLFW.

**GLFW**

Biblioteca implementada em C.


### Shaders

Como tudo representado no OpenGL em 3 dimensões, uma parte do trabalho do OpenGL é transformar as coordenadas 3D para um plano 2D (análogo à tela) e transformar estas coordenadas 2D em pixels coloridos.

Para isso existe uma pipeline com passos altamente especializados e dependentes um dos outros, que são simples o suficiente para serem executados em paralelo com grande eficiência pelas GPUs: os shaders.

Shaders podem ser configurados ou implementados pelos desenvolvedores e podem ser adicionados à pipeline. São escritos em GLSL (OpenGL Shading Language).

### Pipeline de renderização

Vertex Data - São os dados de entrada da pipeline. Lista de trios de coordenadas 3d (cada uma forma um triângulo). Para saber o que fazer com uma coleção de vértices, o OpenGL precisa receber um **primitiva**, que indica se queremos tratar os vértices como pontos (`GL_POINTS`), triângulos (`GL_TRIAGLES`) ou uma linha (`GL_LINE_STRIP`). Os vértices podem conter quaisquer atributos adicionais que quisermos.

Vertex Shader - Primeiro passo, recebe apenas um vértice como entrada. Tem como trabalho transformar as coordenadas 3D em outras coordenadas 3D (TODO: adicionar mais detalhes) e permite um processamento básico dos atributos dos vértices.

Geometry Shader - Passo opcional, recebe a saída do Vertex Shader. Sua entrada é uma lista de vértices que formam uma primitiva e seu trabalho é gerar outras formas emitindo (criando) novos vértices para formar outras primitivas.

Primitive/Shape Assembly - Recebe todos os vértices (ou o vértice, no caso de GL_POINTS) dos passos anteriores que formam uma ou mais primitivas e junta todos os pontos no formato da primitiva desejada.

Rasterization - Mapeia o resultado das primitivas para pixels correspondentes na tela. A saída são framentos. Antes da próxima etapa, um processo de "clipping" é realizado, tirando da saída tudo que não vai aparecer na tela.

Fragmentos em OpenGL são todos os dados necessários para renderizar um único pixel.

Fragment Shader - Responsável por calcular a cor final de cada pixel a ser renderizado. Normalmente é onde os efeitos avançados de OpenGL são aplicados, pois costuma conter dados da cena 3D usado para calcular a cor final (iluminação, sombras, etc).

Alpha Tests/Blending - Onde a ordem de aparição dos fragmentos é determinada, fragmentos oclusos são descartados, a transparência do restante é calculada e os objetos são "misturados", permitindo que a cor final dos pixels calculada no Fragment Shader sofra alterações na renderização.


### Usando uma pipeline de renderização

Atualmente é esperado que sejam providos ao menos um Vertex Shader e um Fragment Shader, visto que não existem shaders default para estes passos na GPU.


#### Vertex Data

Para que o OpenGL considere os valores dos vértices, precisam estar no intervalo [-1.0,1.0] nos 3 eixos. Isso é conhecido como "normalized device coordinates" (NDC). Todas as regiões fora deste intervalo não serão visíveis na tela.

```cpp
float myVertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};
```

A transformação destas coordenadas para coordenadas de tela ("screen-space coordinates") é feita pela transformação de viewport, usando os dados passados para a função `glViewport`.

Para enviar estes dados à GPU, é preciso escrever em sua memória e dizer como interpretar aquela região de memória. Isto opde ser feito através de Vertex Buffer Objects (VBO). O processo de enviar dados para a GPU é relativamente lento, então é desejável que seja feito o quanto antes e na maior quantidade possível. Uma vez que os dados estão lá, o acesso pela GPU é muito rápido.

VBOs são objetos OpenGL e podem ser gerados como os demais:

```cpp
unsigned int VBO;
glGenBuffers(1, &VBO);
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(myVertices), myVertices, GL_STATIC_DRAW)
```

Os valores possíveis para como a GPU deve gerir os dados que estamos enviando são:
- `GL_STREAM_DRAW`: os dados são copiados uma vez e utilizado no máximo algumas vezes.
- `GL_STATIC_DRAW`: os dados são copiados uma vez e utilizados muitas vezes.
- `GL_DYNAMIC_DRAW`: os dados são modificados e utilizados muitas vezes.


#### Vertex Shader

Primeiramente é preciso escrever um shader utilizando GLSL:

```glsl
#version 300 core
layout (location = 0) in vec3 aPos;

void main() {
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
```

O pragma na primeira linha define a versão de OpenGL para aquele shader (3.0.0) e explicitamente define que usa a especificação core (não-extensão). `aPos` é o único parâmetro de entrada e um vetor de 3 dimensões. A variável embutida `gl_Position` (um vetor de 4 dimensões) define a posição 3D do vértice sua "perspective division". Ao final da execução de `main()`, será o valor desta variável a ser usado pela GPU.

Este é provavelmente o shader de vértice mais simples que conseguimos implementar, visto que não faz nenhum tipo de transformação, apenas passa a posição do vértice adiante e fixa o valor de w (perspective division) em 1.0.

Para compilar o shader em tempo de execução, basta registrá-lo com a GPU e invocar `glCompileShader()`:

```cpp
unsigned int vertexShader;
vertexShader = glCreateShader(GL_VERTEX_SHADER);
glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
glCompileShader(vertexShader);
```

Para checar se a compilação deu certo:

```cpp
int  success;
char infoLog[512];
glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

if (!success) {
  glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
  std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
}
```

#### Fragment Shader

Como o fragment shader define a cor de saída dos pixels, o shader mais simples possível é um que define a mesma cor para qualquer pixel:

```glsl
#version 330 core
out vec4 FragColor;

void main() {
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
```

Aqui é definida uma variável de saída (`out vec4 FragColor`) e não usamos nenhuma entrada. Para compilar o shader, o processo é muito parecido com o vertex shader:

```cpp
unsigned int fragmentShader;
fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
glCompileShader(fragmentShader);
```

#### Shader Program

Depois de escrever e compilar os shaders necessários ou desejados, precisamos ligá-los em um programa de shaders, para que a GPU possa usá-los:

```cpp
// cria programa e adiciona shaders
unsigned int shaderProgram;
shaderProgram = glCreateProgram();
glAttachShader(shaderProgram, vertexShader);
glAttachShader(shaderProgram, fragmentShader);
glLinkProgram(shaderProgram);

// verifica se programa foi linkado
glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADERPROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
}
// usa programa
glUseProgram(shaderProgram);

// deleta shaders da memória RAM (já estão "na GPU")
glDeleteShader(vertexShader);
glDeleteShader(fragmentShader);
```

#### Configuração do Vertex Buffer

Como o vertex shader pode ser bastante flexível com os dados que recebe, é preciso especificar como o input está organizado e que dados correspondem a que coordenadas de cada vértice. Para isso usamos o `glVertexAttribPointer`. As instruções são aplicadas ao VOB associado no momento.

```cpp
// params: posição do atributo ("location" no shader), tamanho do vértice, tipo,
// se deve ser normalizado, stride pra chegar no mesmo atributo do próximo vértice,
// offset para chegar no atributo no começo do input
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);
```

#### Vertex Array Objects

VAOs são estruturas de dados que agrupam dados (VBOs) e configurações de atributos para vertex data. A ideia é usar um VAO para armazenar um VBO e sua configuração de atributos, e só ficar fazendo bind e draw na hora de renderizar. Primeiro é preciso criar o VAO e fazer bind do VAO e VBO, nesta ordem:

```cpp
unsigned int VBO, VAO;
glGenVertexArrays(1, &VAO);
glGenBuffers(1, &VBO);

glBindVertexArray(VAO);
glBindBuffer(GL_ARRAY_BUFFER, VBO);

glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);
```

Depois disso é seguro fazer unbind tanto do VBO quanto do VAO, pois tudo estará "linkado" no VAO:

```cpp
glBindBuffer(GL_ARRAY_BUFFER, 0);
glBindVertexArray(0);
```

No loop de renderização, basta fazer bind e desenhar:

```cpp
while (...) {
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // se houver mais coisa pra renderizar, fazer unbind, etc...
}
```

#### Limpando recursos

Ao fim do programa ou quando não precisar mais, podemos destruir os Shader Programs, VAOs e BAOs, para liberar memória:

```cpp
glDeleteVertexArrays(1, &VAO);
glDeleteBuffers(1, &VBO);
glDeleteProgram(shaderProgram);
```

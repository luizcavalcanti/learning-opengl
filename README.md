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

#### Usando uma pipeline de renderização

Atualmente é esperado que sejam providos ao menos um Vertex Shader e um Fragment Shader, visto que não existem shaders default para estes passos na GPU.


#### Vertex Shader

Para que o OpenGL considere os valores dos vértices, precisam estar no intervalo [-1.0,1.0] nos 3 eixos. Isso é conhecido como "normalized device coordinates". Todas as regiões fora deste intervalo não serão visíveis na tela.

```cpp
float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};
```

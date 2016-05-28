#include "../Renderer.h"

int main()
{
    Renderer& renderer = Renderer::getInstance();
    renderer.render();

    return 0;
}


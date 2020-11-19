#include "model.h"
#include "renderer.h"

int main()
{
	Renderer renderer(1024, 1024);
	Model m("african_head.obj");
	m.load_texture("african_head_diffuse.bmp");
	renderer.draw_model(m);
	renderer.dump("test.bmp");
	return 0;
}
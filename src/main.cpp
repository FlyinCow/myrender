#include "renderer.h"
#include "model.h"

int main()
{
	Renderer renderer(1024, 1024);
	
	Model model("african_head.obj");
	model.load_texture("african_head_diffuse.bmp");

	renderer.draw_model(model);
	renderer.dump("test.bmp");
	return 0;
}
#include "RenderData/GraphicObject.h"
#include "Shader.h"

class Render2D {
public:
	Render2D();

	void draw(int index, int size);
	void draw(GraphicObject object, Dimension::Shader shader);

	~Render2D();

private:

};
#pragma once
#include <string>
#include "Window.h"
#include "Input/Events/Event.h"
#include "Input/Events/Events.h"
#include "Input/Events/AplicationEvent.h"
#include "Input/Events/MouseEvent.h"
#include "Input/Events/KeyEvent.h"
#include "Layers/LayerStack.h"

class GraphicObject;
class Texture;
namespace objl {
	class Loader;
};
namespace Dimension {
	class Aplication {
		public:
			Aplication(const char* title, int width, int height);
			void Run();

			static Aplication* Get() { return app; }
			std::shared_ptr<Window> GetWindow() { return window; }
			void LoadDefaultResources();
			GraphicObject CreateObject(Texture texture, objl::Loader mesh);

			/*Demo functions*/
			void PrepereRender();
			void DrawObjects();
			void LoadRequestedResources();
			void RenderUI();
			void renderInformation();
			void EntytiesControll();
			void ResoursesControl();
			void GraphicObjectsControll();
			void CreateEditGraphicObject();
			void PngSelection();
			void ObjSelection();
			void CameraControll();
			void SystemMonitor();
			void MainMenuBar();
			
			~Aplication();
		private:
			static Aplication* app;
			std::shared_ptr<Window> window;
			Events events;
			bool Running;

			void Close();

			LayerStack m_Layers;

			std::vector<GraphicObject> objectsToRender;
	};
}
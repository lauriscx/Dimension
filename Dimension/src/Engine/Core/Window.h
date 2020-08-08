#pragma once
#include <memory>

namespace Dimension {
	class Events;

	class Window {
		public:
			static std::shared_ptr<Window> Create(const char * title, uint32_t width, uint32_t height);

			virtual				~Window			()				= default;
			virtual uint32_t	GetWidth		()				= 0;
			virtual uint32_t	GetHeight		()				= 0;
			virtual float		GetAspectRation	()				= 0;
			virtual void		Update			()				= 0;
			virtual void		EventsHandler	(Events* event)	= 0;
			virtual void *		Context			()				= 0;
	};
}
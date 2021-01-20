#pragma once
#include <memory>

namespace Dimension {
	class Events;

	class Window {
		public:
			static std::shared_ptr<Window> Create(const char * title, uint32_t width, uint32_t height);

			virtual				~Window			()					= default;
			virtual uint32_t	GetWidth		()					= 0;
			virtual uint32_t	GetHeight		()					= 0;
			virtual float		GetAspectRation	()					= 0;
			virtual void		Update			()					= 0;
			virtual void		SetVsync		(bool vsync)		= 0;
			virtual void		SetFullScreen	(bool FullScreen)	= 0;
			virtual void		EventsHandler	(Events* event)		= 0;
			virtual const char* GetVendor		()					= 0;
			virtual const char* GetRenderer		()					= 0;
			virtual const char* GetVersion		()					= 0;
			virtual int			GetExtensionNum	()					= 0;
			virtual const char* GetExtension	(int i)				= 0;
			virtual void *		Context			()					= 0;
	};
}
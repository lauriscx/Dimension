#pragma once
#include <string>

namespace Dimension {
	/*Define class*/
	class Events;

	class Layer {
	public:
		Layer(const std::string& name = "Layer");

		virtual void OnAttach	();
		virtual void OnDetach	();
		virtual void OnUpdate	();
		virtual void OnEvent	(Events& events);
		virtual void Enable		();
		virtual void Disable	();

		virtual ~Layer();

		void SetEnable(bool Enable);

		inline const bool			IsEnable() { return m_Enabled;	}
		inline const std::string&	GetName	() { return m_Name;		}

	private:
		std::string m_Name;
		bool		m_Enabled;
	};
}
#pragma once

#ifdef FLATGRAPHICS
#define FLATGRAPHICS __declspec(dllexport)
#else
#define FLATGRAPHICS __declspec(dllexport)
#endif

#ifdef __cplusplus
extern "C" 
{
#endif 

	class IRenderer abstract
	{
	public:
		virtual void FLATGRAPHICS Initialize(int hWnd, unsigned width, unsigned height, bool useImGUI) abstract;
		virtual void FLATGRAPHICS Destroy() abstract;
		virtual void FLATGRAPHICS OnResize(unsigned width, unsigned height) abstract;

		virtual void FLATGRAPHICS BeginRender() abstract;
		virtual void FLATGRAPHICS Excute() abstract;
		virtual void FLATGRAPHICS EndRender() abstract;

		virtual void FLATGRAPHICS Submit() abstract;

	};

	class Exporter
	{
	public:
		static FLATGRAPHICS IRenderer* GetRenderer();
		static FLATGRAPHICS void DeleteRenderer();

	private:
		static IRenderer* m_renderer;
	};


#ifdef __cplusplus
}
#endif 
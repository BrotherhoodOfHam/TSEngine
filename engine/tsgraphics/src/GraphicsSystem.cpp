/*
	Main Graphics Subsystem source
*/

#include <tscore/debug/log.h>

#include <tsgraphics/GraphicsSystem.h>
#include <tsgraphics/GraphicsContext.h>
#include <tsgraphics/api/RenderApi.h>

#include "platform/borderless.h"

using namespace std;
using namespace ts;

/////////////////////////////////////////////////////////////////////////////////////////////////
//	Internal implementation
/////////////////////////////////////////////////////////////////////////////////////////////////
struct GraphicsSystem::System
{
	GraphicsSystem* system;
	SGraphicsSystemConfig systemConfig;

	CTextureManager textureManager;
	CShaderManager shaderManager;

	IRenderContext* context;

	System(GraphicsSystem* system, const SGraphicsSystemConfig& cfg) :
		system(system),
		systemConfig(cfg),
		context(nullptr)
	{

	}

	~System()
	{

	}
};

/////////////////////////////////////////////////////////////////////////////////////////////////

GraphicsSystem::GraphicsSystem(const SGraphicsSystemConfig& cfg)
{
	//Configure low level render api
	SRenderApiConfig apicfg;
	apicfg.adapterIndex = 0; //hard code the adapter for now
	apicfg.display.resolutionH = cfg.height;
	apicfg.display.resolutionW = cfg.width;
	apicfg.display.fullscreen = (cfg.displaymode == EDisplayMode::eDisplayFullscreen);
	apicfg.display.multisampleCount = cfg.multisampling.count;
	apicfg.windowHandle = cfg.windowHandle;

#ifdef _DEBUG
	apicfg.flags |= ERenderApiFlags::eFlagDebug;
#endif

	//Initialize low level render API
	this->init(cfg.apiid, apicfg);

	//Allocate internal implementation
	pSystem.reset(new System(this, cfg));

	//Initialize texture/shader/mesh managers
	
	Path sourcepath(cfg.rootpath);
	sourcepath.addDirectories("shaders/bin");

	pSystem->textureManager = CTextureManager(this, cfg.rootpath);
	pSystem->shaderManager = CShaderManager(this, sourcepath, eShaderManagerFlag_Debug);

	//Create main render context
	getApi()->createContext(&pSystem->context);
}

GraphicsSystem::~GraphicsSystem()
{
	if (pSystem)
	{
		getApi()->destroyContext(pSystem->context);
		pSystem->context = nullptr;

		//Destroy all cached shaders
		pSystem->shaderManager.clear();
		pSystem->textureManager.clear();

		//Release all resources before deinitialization (causes memory leak otherwise)
		pSystem.reset();

		//Deinitialize low level render api
		this->deinit();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void GraphicsSystem::setDisplayConfiguration(EDisplayMode displaymode, uint32 w, uint32 h, SMultisampling sampling)
{
	if (!pSystem)
	{
		return;
	}

	auto& config = pSystem->systemConfig;

	SDisplayConfig display;
	getApi()->getDisplayConfiguration(display);

	//Update multisample count
	if (sampling.count)
	{
		if (config.multisampling.count != sampling.count)
		{
			config.multisampling = sampling;
			display.multisampleCount = sampling.count;
		}
	}

	//Update fullscreen state
	if (displaymode)
	{
		if (config.displaymode != displaymode)
		{
			intptr winhandle = config.windowHandle;

			if (displaymode == eDisplayFullscreen)
			{
				//Exit borderless if previous mode was borderless
				if (config.displaymode == eDisplayBorderless)
					exitBorderless(winhandle);

				//Enter fullscreen
				display.fullscreen = true;
			}
			else if (displaymode == eDisplayBorderless)
			{
				//Exit fullscreen if the previous mode was fullscreen
				if (config.displaymode == eDisplayFullscreen)
					display.fullscreen = false;
				
				//Enter borderless fullscreen
				enterBorderless(winhandle);
			}
			else if (displaymode == eDisplayWindowed) //Windowed mode
			{
				//Exit fullscreen if the previous mode was fullscreen
				if (config.displaymode == eDisplayFullscreen)
					display.fullscreen = false;
				//Exit borderless if previous mode was borderless
				else if (config.displaymode == eDisplayBorderless)
					exitBorderless(winhandle);

				//Do nothing
			}

			config.displaymode = displaymode;
		}
	}

	//Update resolution
	if (w || h)
	{
		w = (w) ? w : config.width;
		h = (h) ? h : config.height;

		if (config.width != w || config.height != h)
		{
			display.resolutionH = h;
			display.resolutionW = w;

			config.width = w;
			config.height = h;
		}
	}

	getApi()->setDisplayConfiguration(display);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//	Getters
/////////////////////////////////////////////////////////////////////////////////////////////////

void GraphicsSystem::getConfiguration(SGraphicsSystemConfig& cfg)
{
	if (pSystem)
	{
		cfg = pSystem->systemConfig;
	}
}

CTextureManager* GraphicsSystem::getTextureManager()
{
	return &pSystem->textureManager;
}

CShaderManager* GraphicsSystem::getShaderManager()
{
	return &pSystem->shaderManager;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void GraphicsSystem::execute(GraphicsContext* context)
{
	tsassert(pSystem);

	IRenderContext* rc = pSystem->context;

	getApi()->drawBegin();

	HTarget display = HTARGET_NULL;
	getApi()->getDisplayTarget(display);

	rc->clearRenderTarget(display, (const Vector&)colours::Azure);
	rc->clearDepthTarget(display, 1.0f);

	if (CommandQueue* queue = context->render(display))
	{
		queue->flush(rc);
	}

	rc->finish();

	getApi()->drawEnd(&rc, 1);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
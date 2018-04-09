/*
	Graphics Context source
*/

#include <tsgraphics/GraphicsContext.h>
#include <tsgraphics/api/RenderApi.h>

using namespace ts;
using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ctor/dtor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

GraphicsContext::GraphicsContext(GraphicsSystem* system) :
	m_system(system)
{
	//Create render queue
	m_drawQueue = CommandQueue(1024);

	//Get root asset directory
	Path rootpath = m_system->getRootPath();

	//Shader files are located in a folder called shaderbin in the root asset directory
	Path sourcepath(rootpath);
	sourcepath.addDirectories("shaderbin");

	//Create resource managers
	m_bufferPool = CBufferPool(m_system);
	m_meshManager = CMeshManager(m_system);
	m_textureManager = CTextureManager(m_system, rootpath);
	m_shaderManager = CShaderManager(m_system, sourcepath, eShaderManagerFlag_Debug);
}

GraphicsContext::~GraphicsContext()
{
	//Destroy all cached resources
	m_shaderManager.clear();
	m_textureManager.clear();
	m_meshManager.clear();
	m_bufferPool.clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Draw command manager methods
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

GraphicsContext::ItemID GraphicsContext::createItem()
{
	ItemID item;
	m_itemAllocator.alloc(item);
	return item;
}

bool GraphicsContext::isItem(ItemID renderItem) const
{
	return m_itemAllocator.exists(renderItem);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Commit command queue
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GraphicsContext::commit()
{
	//Sort queue before submission
	m_drawQueue.sort();
	m_system->execute(&m_drawQueue);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
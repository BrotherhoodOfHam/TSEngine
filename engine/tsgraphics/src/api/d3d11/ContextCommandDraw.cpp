/*
	Render API

	Draw command methods
*/

#include "context.h"
#include "helpers.h"
#include "HandleTarget.h"
#include "HandleCommandDraw.h"

using namespace ts;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void D3D11RenderContext::draw(
	HTarget t,
	const SViewport& view,
	const SViewport& scissor,
	HDrawCmd hDraw
)
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	//Set rendertargets/depthstencil
	D3D11Target* target = D3D11Target::upcast(t);
	target->bind(m_context.Get());

	//Set viewports/scissors
	D3D11_VIEWPORT viewport;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
	viewport.TopLeftX = (FLOAT)view.x;
	viewport.TopLeftY = (FLOAT)view.y;
	viewport.Width = (FLOAT)view.w;
	viewport.Height = (FLOAT)view.h;
	m_context->RSSetViewports(1, &viewport);

	D3D11_RECT scissorrect;
	scissorrect.top = scissor.y;
	scissorrect.left = scissor.x;
	scissorrect.bottom = scissor.h;
	scissorrect.right = scissor.w;
	m_context->RSSetScissorRects(1, &scissorrect);
	
	D3D11DrawCommand* command = D3D11DrawCommand::upcast(hDraw);

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	//Set shaders
	m_context->VSSetShader(command->shaderVertex.Get(), nullptr, 0);
	m_context->PSSetShader(command->shaderPixel.Get(), nullptr, 0);
	m_context->GSSetShader(command->shaderGeometry.Get(), nullptr, 0);
	m_context->DSSetShader(command->shaderDomain.Get(), nullptr, 0);
	m_context->HSSetShader(command->shaderHull.Get(), nullptr, 0);

	//Set states
	m_context->RSSetState(command->rasterState.Get());
	const float blendfactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_context->OMSetBlendState(command->blendState.Get(), blendfactor, UINT_MAX);
	m_context->OMSetDepthStencilState(command->depthState.Get(), 0);

	//Bind textures
	for (int i = 0; i < SDrawCommand::eMaxTextureSlots; i++)
	{
		auto srv = command->shaderResourceViews[i].Get();
		m_context->PSSetShaderResources(i, 1, &srv);
		m_context->DSSetShaderResources(i, 1, &srv);
	}

	//Bind texture samples
	for (int i = 0; i < SDrawCommand::eMaxTextureSamplerSlots; i++)
	{
		auto sampler = command->shaderSamplerStates[i].Get();
		m_context->PSSetSamplers(i, 1, &sampler);
		m_context->DSSetSamplers(i, 1, &sampler);
	}


	//Bind index buffer
	m_context->IASetIndexBuffer(command->indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	//Bind vertex buffers
	for (int i = 0; i < SDrawCommand::eMaxVertexBuffers; i++)
	{
		auto v = command->vertexBuffers[i].Get();
		uint32 offset = command->vertexOffsets[i];
		uint32 stride = command->vertexStrides[i];

		m_context->IASetVertexBuffers(i, 1, &v, &stride, &offset);
	}
	
	//Bind constant buffers
	for (int i = 0; i < SDrawCommand::eMaxConstantBuffers; i++)
	{
		ID3D11Buffer* u = command->constantBuffers[0].Get();
		m_context->VSSetConstantBuffers(i, 1, &u);
		m_context->PSSetConstantBuffers(i, 1, &u);
		m_context->GSSetConstantBuffers(i, 1, &u);
		m_context->HSSetConstantBuffers(i, 1, &u);
		m_context->DSSetConstantBuffers(i, 1, &u);
	}

	//Bind an input layout and a primitive topology
	m_context->IASetInputLayout(command->inputLayout.Get());
	m_context->IASetPrimitiveTopology(command->topology);

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	switch (command->mode)
	{
		case EDrawMode::eDraw:
		{
			m_context->Draw(command->vertexCount, command->vertexStart);
			break;
		}
		case EDrawMode::eDrawIndexed:
		{
			m_context->DrawIndexed(command->indexCount, command->indexStart, command->vertexBase);
			break;
		}
		case EDrawMode::eDrawIndexedInstanced:
		{
			m_context->DrawIndexedInstanced(command->indexCount, command->instanceCount, command->indexStart, command->vertexBase, 0);
			break;
		}
		case EDrawMode::eDrawInstanced:
		{
			m_context->DrawInstanced(command->vertexCount, command->instanceCount, command->vertexStart, 0);
			break;
		}
	}

	//For debugging
	m_api->incrementDrawCallCounter();

	/////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "Core/Common.h"
#include "Renderer/DX11/Internal/Common.h"
#include "Renderer/DX11/Internal/Resources/Buffer.h"
#include "Renderer/DX11/Internal/Resources/ConstantBuffer.h"
using namespace std;
class OutLinePass
{
private:
  MeshConstantBuffer* _CB = nullptr;

public:
  vector<RenderMesh::StaticMesh> _staticMesh;
  vector<RenderMesh::SkelMesh> _skelMesh;

public:
  OutLinePass(MeshConstantBuffer* cb) : _CB{cb} {}
  ~OutLinePass() {}

public:
  void DrawStaticOutline(ID3D11DeviceContext* dc)
  {
    std::ranges::for_each(_staticMesh, [dc, this](RenderMesh::StaticMesh mesh) {
      dc->IASetVertexBuffers(0, 1, mesh.buffer->vertexBuffer.GetAddressOf(),
                             &(mesh.buffer->stride), &(mesh.buffer->offset));
      dc->IASetIndexBuffer(mesh.buffer->indexBuffer.Get(), DXGI_FORMAT_R32_UINT,
                           0);
      Constant::World world = {mesh.world.Transpose()};
      _CB->UpdateContantBuffer(world, MeshCBType::World);
      Constant::PixelData pixel;
      pixel.albedoFactor = mesh.outlineColor;
      _CB->UpdateContantBuffer(pixel, MeshCBType::PixelData);
      dc->DrawIndexed(mesh.buffer->nIndices, 0, 0);
    });
  }
  void DrawSkelOutline(ID3D11DeviceContext* dc)
  {
    std::ranges::for_each(_skelMesh, [dc, this](RenderMesh::SkelMesh mesh) {
      Constant::BoneMatrix boneMat = {};
      std::copy(mesh.boneMatrix.begin(), mesh.boneMatrix.end(), boneMat.matrix);

      _CB->UpdateContantBuffer(boneMat, MeshCBType::BoneMatrix);
      dc->VSSetShaderResources(15, 1, mesh.buffer->boneIDSrv.GetAddressOf());
      dc->VSSetShaderResources(16, 1,
                               mesh.buffer->boneWeightsSrv.GetAddressOf());
      dc->IASetVertexBuffers(0, 1, mesh.buffer->vertexBuffer.GetAddressOf(),
                             &(mesh.buffer->stride), &(mesh.buffer->offset));
      dc->IASetIndexBuffer(mesh.buffer->indexBuffer.Get(), DXGI_FORMAT_R32_UINT,
                           0);
      Constant::World world = {mesh.world.Transpose()};
      _CB->UpdateContantBuffer(world, MeshCBType::World);
      Constant::PixelData pixel;
      pixel.albedoFactor = mesh.outlineColor;
      _CB->UpdateContantBuffer(pixel, MeshCBType::PixelData);
      dc->DrawIndexed(mesh.buffer->nIndices, 0, 0);
    });
  }

  void WriteStaticStencil(ID3D11DeviceContext* dc)
  {
    std::ranges::for_each(_staticMesh, [dc, this](RenderMesh::StaticMesh mesh) {
      dc->IASetVertexBuffers(0, 1, mesh.buffer->vertexBuffer.GetAddressOf(),
                             &(mesh.buffer->stride), &(mesh.buffer->offset));
      dc->IASetIndexBuffer(mesh.buffer->indexBuffer.Get(), DXGI_FORMAT_R32_UINT,
                           0);
      Constant::World world = {mesh.world.Transpose()};
      _CB->UpdateContantBuffer(world, MeshCBType::World);
      dc->DrawIndexed(mesh.buffer->nIndices, 0, 0);
    });
  }
  void WriteSkelStencil(ID3D11DeviceContext* dc)
  {
    std::ranges::for_each(_skelMesh, [dc, this](RenderMesh::SkelMesh mesh) {
      Constant::BoneMatrix boneMat = {};
      std::copy(mesh.boneMatrix.begin(), mesh.boneMatrix.end(), boneMat.matrix);

      _CB->UpdateContantBuffer(boneMat, MeshCBType::BoneMatrix);
      dc->VSSetShaderResources(15, 1, mesh.buffer->boneIDSrv.GetAddressOf());
      dc->VSSetShaderResources(16, 1,
                               mesh.buffer->boneWeightsSrv.GetAddressOf());
      dc->IASetVertexBuffers(0, 1, mesh.buffer->vertexBuffer.GetAddressOf(),
                             &(mesh.buffer->stride), &(mesh.buffer->offset));
      dc->IASetIndexBuffer(mesh.buffer->indexBuffer.Get(), DXGI_FORMAT_R32_UINT,
                           0);
      Constant::World world = {mesh.world.Transpose()};
      _CB->UpdateContantBuffer(world, MeshCBType::World);
      dc->DrawIndexed(mesh.buffer->nIndices, 0, 0);
    });
  }
  void ClearMeshes()
  {
    _staticMesh.clear();
    _skelMesh.clear();
  }
};
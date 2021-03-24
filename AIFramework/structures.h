#pragma once
#define PI 3.1415926535f

using namespace std;
using namespace DirectX;

//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------

#include <vector>
class DrawableGameObject;
class Waypoint;
class PickupItem;

using namespace std;

struct ConstantBuffer
{
	DirectX::XMMATRIX mWorld;
	DirectX::XMMATRIX mView;
	DirectX::XMMATRIX mProjection;
};


struct SimpleVertex
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT2 TexCoord;
};
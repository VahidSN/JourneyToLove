#pragma once

class Scene;

namespace CollisionUtils
{
	enum class CellType
	{
		None,
		Block,
		Hazard,
		Checkpoint,
		EndPoint
	};

	CellType IntToCellType(int val);

	CellType DetectRightCellOfObject(const Scene* scene, const Vector2& objectPosition, const float objectSize);
	CellType DetectLeftCellOfObject(const Scene* scene, const Vector2& objectPosition, float objectSize);
	CellType DetectUpCellOfObject(const Scene* scene, const Vector2& objectPosition, float objectSize);
	CellType DetectDownCellOfObject(const Scene* scene, const Vector2& objectPosition, float objectSize);
}
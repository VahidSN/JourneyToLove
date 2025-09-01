#include "raylib.h"
#include "CollisionUtils.h"
#include "Scene.h"
#include <cmath>

namespace CollisionUtils
{
	constexpr float PENETERATION_OFFSET = 0.01f; // for collision detection
	constexpr float HAZARD_DETECTION_OFFSET = 5.0f; // for hazard detection

	static CellType DetectCellInOffests(
		const Scene* scene,
		const Vector2& objectPosition,
		const float objectSize,
		const std::vector<Vector2>& offsets)
	{
		//Priority 1: Check for finish
		for (const auto& offset : offsets)
		{
			Vector2 checkPos = { objectPosition.x + offset.x, objectPosition.y + offset.y };
			CellType detected = IntToCellType(scene->DetectCellTypeOnPosition(checkPos));
			if (detected == CellType::EndPoint)
			{
				return detected;
			}
		}

		//Priority 2: Check for block
		for (const auto& offset : offsets)
		{
			Vector2 checkPos = { objectPosition.x + offset.x, objectPosition.y + offset.y };
			CellType detected = IntToCellType(scene->DetectCellTypeOnPosition(checkPos));
			if (detected == CellType::Block)
			{
				return detected;
			}
		}

		//Priority 3: Check for hazard
		for (const auto& offset : offsets)
		{
			Vector2 checkPos = { objectPosition.x + offset.x + std::signbit(offset.x) * HAZARD_DETECTION_OFFSET,
								 objectPosition.y + offset.y + std::signbit(offset.y) * HAZARD_DETECTION_OFFSET };
			CellType detected = IntToCellType(scene->DetectCellTypeOnPosition(checkPos));
			if (detected == CellType::Hazard)
			{
				return detected;
			}
		}

		//Priority 4: Check for checkpoint
		for (const auto& offset : offsets)
		{
			Vector2 checkPos = { objectPosition.x + offset.x, objectPosition.y + offset.y };
			CellType detected = IntToCellType(scene->DetectCellTypeOnPosition(checkPos));
			if (detected == CellType::Checkpoint)
			{
				return detected;
			}
		}

		return CellType::None;
	}

	static CellType GetCellTypeAt(const Scene* scene, const Vector2& position)
	{
		return IntToCellType(scene->DetectCellTypeOnPosition(position));
	}

	CellType IntToCellType(int value)
	{
		switch (value)
		{
		case 0: return CellType::None;
		case 1: return CellType::Block;
		case 2: return CellType::Hazard;
		case 3: return CellType::Checkpoint;
		case 4: return CellType::EndPoint;
		default: return CellType::None;
		}
	}

	CellType DetectRightCellOfObject(const Scene* scene, const Vector2& objectPosition, const float objectSize)
	{
		return DetectCellInOffests(scene, objectPosition, objectSize,
			{
				{ objectSize, 0.0f },
				{ objectSize, objectSize - PENETERATION_OFFSET }
			}
		);
	}
	CellType DetectLeftCellOfObject(const Scene* scene, const Vector2& objectPosition, const float objectSize)
	{
		return DetectCellInOffests(scene, objectPosition, objectSize,
			{
				{ 0.0f, 0.0f },
				{ 0.0f, objectSize - PENETERATION_OFFSET }
			}
		);
	}
	CellType DetectUpCellOfObject(const Scene* scene, const Vector2& objectPosition, const float objectSize)
	{
		return DetectCellInOffests(scene, objectPosition, objectSize,
			{
				{ 0.0f, 0.0f },
				{ objectSize - PENETERATION_OFFSET,0.0f }
			}
		);
	}
	CellType DetectDownCellOfObject(const Scene* scene, const Vector2& objectPosition, const float objectSize)
	{
		return DetectCellInOffests(scene, objectPosition, objectSize,
			{
				{ 0.0f, objectSize },
				{ objectSize - PENETERATION_OFFSET,objectSize }
			}
		);
	}
}

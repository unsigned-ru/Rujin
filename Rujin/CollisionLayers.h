#ifndef COLLISION_LAYERS_H
#define COLLISION_LAYERS_H

#include <cstdint>
#include <unordered_map>

namespace rujin
{
	enum class CollisionResponse : uint8_t
	{
		Block,
		Overlap,
		Ignore
	};

	enum class CollisionLayer : uint8_t
	{
		Default,
		OverlapAll,
		Bullet
	};

	//for each collision layer we want a array of collision responses
	const std::unordered_map<CollisionLayer, std::unordered_map<CollisionLayer, CollisionResponse>> g_CollisionResponses
	{
		{
			CollisionLayer::Default,
			{
				{ CollisionLayer::Default, CollisionResponse::Block },
				{ CollisionLayer::OverlapAll, CollisionResponse::Overlap },
				{ CollisionLayer::Bullet, CollisionResponse::Ignore },
			}
		},
		{
			CollisionLayer::OverlapAll,
			{
				{ CollisionLayer::Default, CollisionResponse::Overlap },
				{ CollisionLayer::OverlapAll, CollisionResponse::Overlap },
				{ CollisionLayer::Bullet, CollisionResponse::Overlap },
			}
		},
		{
			CollisionLayer::Bullet,
			{
				{ CollisionLayer::Default, CollisionResponse::Overlap },
				{ CollisionLayer::OverlapAll, CollisionResponse::Overlap },
				{ CollisionLayer::Bullet, CollisionResponse::Ignore },
			}
		}
	};


	inline CollisionResponse GetResponseForCollisionLayer(CollisionLayer collisionLayer, CollisionLayer responseTo)
	{
		return g_CollisionResponses.at(collisionLayer).at(responseTo);
	}
}


#endif // Include Guard: COLLISION_LAYERS_H

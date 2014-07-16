/**
* A node that can contain son nodes or entities
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "basic_node.h"

namespace Ness
{
	// some needed predeclares
	class Sprite;
	class Canvas;
	class Text;
	class Node;
	class ZNode;
	class TileMap;
	class RectangleShape;
	class NodeAPI;
	class LightNode;

	// node and sprite pointers
	NESSENGINE_API typedef NessSharedPtr<Node>				NodePtr;
	NESSENGINE_API typedef NessSharedPtr<ZNode>				ZNodePtr;
	NESSENGINE_API typedef NessSharedPtr<LightNode>			LightNodePtr;
	NESSENGINE_API typedef NessSharedPtr<Sprite>			SpritePtr;
	NESSENGINE_API typedef NessSharedPtr<Canvas>			CanvasPtr;
	NESSENGINE_API typedef NessSharedPtr<Text>				TextPtr;
	NESSENGINE_API typedef NessSharedPtr<RectangleShape>	RectangleShapePtr;
	NESSENGINE_API typedef NessSharedPtr<TileMap>			TileMapPtr;
	NESSENGINE_API typedef NessSharedPtr<NodeAPI>			NodeAPIPtr;

	// a scene node that can create all type of built-in entities and nodes
	class Node : public BaseNode
	{
	public:
		NESSENGINE_API Node(Renderer* renderer, NodeAPI* parent = nullptr) : 
			BaseNode(renderer, parent) {}

		// create and return a son entities
		NESSENGINE_API virtual NodePtr create_node();
		NESSENGINE_API virtual ZNodePtr create_znode();
		NESSENGINE_API virtual LightNodePtr create_light_node();
		NESSENGINE_API virtual RectangleShapePtr create_rectangle();
		NESSENGINE_API virtual SpritePtr create_sprite(const std::string& textureName);
		NESSENGINE_API virtual CanvasPtr create_canvas(const std::string& textureName, const Sizei& size = Sizei::ZERO);
		NESSENGINE_API virtual TileMapPtr create_tilemap(const std::string& spriteFile, Sizei mapSize, Size singleTileSize = Size(36, 36));
		NESSENGINE_API virtual TextPtr create_text(const std::string& fontFile, const std::string& text, int fontSize = 12);

	};
};
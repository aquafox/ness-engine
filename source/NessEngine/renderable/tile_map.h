/**
* A renderable tilemap, consist of a matrix of sprites.
* have special optimizations for culling and efficient rendering
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "sprite.h"
#include <vector>

namespace Ness
{
	// callback function to run on all tiles
	NESSENGINE_API typedef void (*executeOnTiles)(const Pointi& index, const SpritePtr& tile);

	class TileMap : public Renderable, public RenderableParent
	{
	private:
		Sizei										m_size;						// size of the tilemap
		Size										m_tile_size;				// size of a single tile
		std::vector< std::vector<SpritePtr> >		m_sprites;					// the sprites matrix
		SRenderTransformations						m_absolute_transformations;	// absolute transformations of this tilemap

	public:

		// create the tilemap
		NESSENGINE_API TileMap(Renderer* renderer, RenderableParent* parent, const std::string& spriteFile, Sizei mapSize, Size singleTileSize = Size(36, 36));

		// return the absolute transformations of this tilemap
		NESSENGINE_API virtual const SRenderTransformations& get_absolute_transformations();

		// get a specific sprite by index
		NESSENGINE_API inline SpritePtr& get_sprite(const Pointi& index) {return m_sprites[index.x][index.y];}

		// get a specific sprite by position
		// return empty if out of range
		NESSENGINE_API SpritePtr& get_sprite_by_position(const Point& position);

		// get all visible son entities
		NESSENGINE_API virtual void __get_visible_entities(std::vector<Renderable*>& out_list, const CameraPtr& camera = NullCamera);

		// update that the tilemap needs update
		NESSENGINE_API virtual void transformations_update();

		// check if this tilemap is really visible
		NESSENGINE_API virtual bool is_really_visible(const CameraPtr& camera = NullCamera);

		// get range of tiles that are within the map
		// returns a rectangle. x is first tile i index, w is last tile i index. y is the same for j indexes
		// for example:
		// Rectangle range = map->get_tiles_in_screen();
		// for (int i = range.x; i < range.w; i++) ....
		NESSENGINE_API Rectangle get_tiles_in_screen(const CameraPtr& camera);

		// set source rect from sprite sheet for all tiles
		// for example, if your sprite sheet contains 4x8 animation steps, and you want to set sprite to step (3,2), use it like so:
		//		set_source_from_sprite_sheet(Pointi(3,2), Sizei(4,8));
		// step: the index of the animation step inside the sprite sheet (x, y)
		// stepsCount: how many total animation steps the sheet has (x, y)
		NESSENGINE_API void set_all_tiles_type(const Pointi& step, const Sizei stepsCount);

		// apply the given function to all tiles
		// every call will contain a single tile and its index.
		NESSENGINE_API void apply_to_all(executeOnTiles func);

		// render this tilemap
		NESSENGINE_API virtual void render(const CameraPtr& camera = NullCamera);
	};

	NESSENGINE_API typedef std::shared_ptr<TileMap> TileMapPtr;
};
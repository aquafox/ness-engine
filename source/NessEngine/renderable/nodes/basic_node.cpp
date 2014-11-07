/* 
  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Ronen Ness
  ronenness@gmail.com

*/

#include "basic_node.h"
#include <algorithm>
#include "all_nodes.h"
#include "../entities/all_entities.h"
#include "../../renderer/renderer.h"

namespace Ness
{

	void BaseNode::__get_visible_entities(Containers::Vector<RenderableAPI*>& out_list, const CameraPtr& camera)
	{
		for (unsigned int i = 0; i < m_entities.size(); i++)
		{
			// check if current entity is a node
			NodeAPI* currentNode = dynamic_cast<NodeAPI*>(m_entities[i].get());
			if (currentNode)
			{
				currentNode->__get_visible_entities(out_list, camera);
			}
			// if not a node, check if in screen and if so add it
			else
			{
				RenderableAPI* current = m_entities[i].get();
				if (!current->is_really_visible(camera))
					continue;

				// add to rendering list
				out_list.push_back(current);
			}
		}
	}

	void BaseNode::__get_all_entities(Containers::Vector<RenderableAPI*>& out_list, bool breakGroups)
	{
		for (unsigned int i = 0; i < m_entities.size(); i++)
		{
			// if need to break son nodes:
			if (breakGroups)
			{
				NodeAPI* currentNode = dynamic_cast<NodeAPI*>(m_entities[i].get());
				if (currentNode)
				{
					currentNode->__get_all_entities(out_list, breakGroups);
				}
				else
				{
					out_list.push_back(m_entities[i].get());
				}
			}
			// if not a node, check if in screen and if so add it
			else
			{
				out_list.push_back(m_entities[i].get());
			}
		}
	}

	void BaseNode::add(const RenderablePtr& object)
	{
		object->transformations_update();
		m_entities.push_back(object);
		object->__change_parent(this);
	}

	void BaseNode::add_first(const RenderablePtr& object)
	{
		object->transformations_update();
		m_entities.insert(m_entities.begin(), object);
		object->__change_parent(this);
	}

	void BaseNode::remove(const RenderablePtr& object)
	{
		object->transformations_update();
		object->__change_parent(nullptr);
		m_entities.erase(std::remove(m_entities.begin(), m_entities.end(), object), m_entities.end());
	}

	bool BaseNode::is_really_visible(const CameraPtr& camera)
	{
		// is this node even visible?
		if (!m_visible || m_absolute_trans.color.a <= 0)
			return false;

		// check all sprites
		for (unsigned int i = 0; i < m_entities.size(); i++)
		{
			if (m_entities[i]->is_really_visible(camera))
				return true;
		}

		// if got here it means the node is not visible
		return false;
	}

	const SRenderTransformations& BaseNode::get_absolute_transformations()
	{

		// if need to update transformations from parent, do it
		if (m_need_trans_update)
		{
			// begin with this node basic transformations and add parent's transformations
			m_absolute_trans = m_transformations;
			m_absolute_trans.add_transformations(m_parent->get_absolute_transformations());

			// set no longer need update
			m_need_trans_update = false;
		}

		// return the cached absolute transformations
		return m_absolute_trans;
	}

	void BaseNode::set_render_target(const ManagedResources::ManagedTexturePtr& NewTarget) 
	{
		m_render_target = NewTarget;
	}

	void BaseNode::set_render_target(const CanvasPtr& NewTarget) 
	{
		if (NewTarget->parent() == this)
		{
			throw IllegalAction("Cannot set node render target that is a direct son of the node!");
		}
		m_render_target = NewTarget->get_texture();
	}

	void BaseNode::remove_render_target() 
	{
		m_render_target.reset();
	}

	void BaseNode::transformations_update()
	{
		m_need_trans_update = true;
		for (unsigned int i = 0; i < m_entities.size(); i++)
		{
			Transformable* current = dynamic_cast<Transformable*>(m_entities[i].get());
			if (current)
			{
				current->transformations_update();
			}
		}
	}

	RenderablePtr BaseNode::get_son(const String& name)
	{
		for (unsigned int i = 0; i < m_entities.size(); i++)
		{
			if (m_entities[i]->get_name() == name)
				return m_entities[i];
		}
		return RenderablePtr();
	}

	void BaseNode::render(const CameraPtr& camera)
	{
		// if invisible skip
		if (!m_visible)
			return;

		// if there is alternative target texture
		if (m_render_target)
		{
			m_renderer->push_render_target(m_render_target);
		}

		// render all sprites
		for (unsigned int i = 0; i < m_entities.size(); i++)
		{
			m_entities[i]->render(camera);
		}

		// remove target texture
		if (m_render_target)
		{
			m_renderer->pop_render_target();
		}
	}
};
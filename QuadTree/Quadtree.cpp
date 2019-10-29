#include "Quadtree.h"
#include <iostream>
#include <queue>
#include "SFML\Graphics.hpp"

inline bool node_vs_rect_test(float mx, float my, float sx, float sy, float left, float top, float width, float height)
{
	if (left > mx + sx || left + width < mx - sx || top > my + sy || top + height < my - sy)
		return false;
	return true;
}

inline bool rect_vs_rect_test(float left1, float top1, float width1, float height1, float left2, float top2, float width2, float height2)
{
	if (left1 > left2 + width2 || left1 + width1 < left2 || top1 > top2 + height2 || top1 + height1 < top2)
	{
		return false;
	}

	return true;
}


Quadtree::Quadtree(QuadNodeData root, int max_depth, int max_child) :
	root(root),
	MAX_DEPTH(max_depth),
	MAX_CHILD(max_child),
	list_enodes(enode_num),
	list_nodes(node_num)
{
	list_nodes.set_field(list_nodes.insert(), node_count, 0);
	list_nodes.set_field(0, node_idx_fc, -1);
}

Quadtree::~Quadtree()
{
}

void Quadtree::insert_entities(std::vector<Entity *>* entity, int ID, int32_t idx_node, float mx, float my, float sx, float sy, int depth)
{
	Entity* e((*entity)[ID]);

	if (!node_vs_rect_test(mx, my, sx, sy, e->left, e->top, e->width, e->height))
	{
		return;
	}

	//neu la node la thi insert vao node do
	//int count(list_nodes.get_field(idx_node, node_count));
	int count(list_nodes.data[idx_node*node_num + node_count]);

	if (count >= 0)
	{

		int new_elt_node(list_enodes.insert());

		//list_enodes.set_field(new_elt_node, enode_ID_elt, ID);
		//list_enodes.set_field(new_elt_node, enode_idx_next, list_nodes.get_field(idx_node, node_idx_fc));

		list_enodes.data[new_elt_node*enode_num + enode_ID_elt] = ID;
		list_enodes.data[new_elt_node*enode_num + enode_idx_next] = list_nodes.data[idx_node*node_num + node_idx_fc];

		//list_nodes.set_field(idx_node, node_idx_fc, new_elt_node);
		//list_nodes.set_field(idx_node, node_count, count + 1);

		list_nodes.data[idx_node*node_num + node_idx_fc] = new_elt_node;
		list_nodes.data[idx_node*node_num + node_count] = count + 1;

		if (count + 1 > MAX_CHILD && depth < MAX_DEPTH)
		{
			split(entity, idx_node, mx, my, sx, sy, depth);
		}

		return;
	}

	//neu la node nhanh thi insert vao cac node con cua no
	//int fc(list_nodes.get_field(idx_node, node_idx_fc));

	int fc(list_nodes.data[node_num*idx_node + node_idx_fc]);

	sx /= 2.f;
	sy /= 2.f;

	insert_entities(entity, ID, fc + 0, mx - sx, my - sy, sx, sy, depth + 1);
	insert_entities(entity, ID, fc + 1, mx + sx, my - sy, sx, sy, depth + 1);
	insert_entities(entity, ID, fc + 2, mx + sx, my + sy, sx, sy, depth + 1);
	insert_entities(entity, ID, fc + 3, mx - sx, my + sy, sx, sy, depth + 1);
}



void Quadtree::traverse(sf::RenderWindow* window, std::vector<Entity*>* list_enities, int32_t idx_node, float mx, float my, float sx, float sy, int depth)
{
	//neu la node nhanh thi duyet tat ca cac node con


	if (list_nodes.data[idx_node*node_num + node_count] < 0)
	{
		sx /= 2.f;
		sy /= 2.f;

		//int fc = list_nodes.get_field(idx_node, node_idx_fc);
		int fc(list_nodes.data[idx_node*node_num + node_idx_fc]);

		traverse(window, list_enities, fc + 0, mx - sx, my - sy, sx, sy, depth + 1);
		traverse(window, list_enities, fc + 1, mx + sx, my - sy, sx, sy, depth + 1);
		traverse(window, list_enities, fc + 2, mx + sx, my + sy, sx, sy, depth + 1);
		traverse(window, list_enities, fc + 3, mx - sx, my + sy, sx, sy, depth + 1);

		return;
	}



	//neu khong thi xet collision cua tat ca cac elt trong node
	//roi xoa het cac elt
	//int elt_node = list_nodes.get_field(idx_node, node_idx_fc);
	int elt_node(list_nodes.data[idx_node*node_num + node_idx_fc]);

	IntList list_ID(1);


	while (elt_node != -1)
	{
		//int id = list_enodes.get_field(elt_node, enode_ID_elt);
		//list_ID.set_field(list_ID.insert(), 0, id);

		list_ID.data[list_ID.insert()] = list_enodes.data[elt_node*enode_num + enode_ID_elt];

		//int pre(elt_node);
		//elt_node = list_enodes.get_field(elt_node, enode_idx_next);
		elt_node = list_enodes.data[elt_node*enode_num + enode_idx_next];
	}


	//int size(list_ID.get_num_elt());
	int size(list_ID.num_elt);

	Entity* e1 = nullptr;
	Entity* e2 = nullptr;

	for (int i(0); i < size - 1; ++i)
	{
		e1 = (*list_enities)[list_ID.data[i]];
		//e1->color = sf::Color::Blue;

		for (int j(i + 1); j < size; ++j)
		{
			e2 = (*list_enities)[list_ID.data[j]];

			if (rect_vs_rect_test(
				e1->left, e1->top, e1->width, e1->height,
				e2->left, e2->top, e2->width, e2->height))
			{
				e1->color = sf::Color::Red;
				e2->color = e1->color;
			}
		}
	}
}

QuadNodeData Quadtree::get_root() const
{
	return root;
}

void Quadtree::draw(sf::RenderWindow * window, int32_t idx_node, int mx, int my, int sx, int sy)
{
	/*sf::RectangleShape shape(sf::Vector2f(sx << 1, sy << 1));
	shape.setPosition(mx - sx, my - sy);
	shape.setOutlineColor(sf::Color::Green);
	shape.setOutlineThickness(1.f);
	window->draw(shape);*/




	if (list_nodes.get_field(idx_node, node_count) >= 0)
	{
		//mVertexArray.append(sf::VertexArray(sf:));


		return;
	}

	int fc(list_nodes.get_field(idx_node, node_idx_fc));

	sx >>= 1;
	sy >>= 1;

	draw(window, fc++, mx - sx, my - sy, sx, sy);
	draw(window, fc++, mx + sx, my - sy, sx, sy);
	draw(window, fc++, mx + sx, my + sy, sx, sy);
	draw(window, fc, mx - sx, my + sy, sx, sy);
}


/*
*	tim den cac node la va xoa het cac elt trong do.
*	neu node nhanh hien tai co 4 node la node leaf trong thi free 4 node do,
*	sau do xet node do thanh node trong
*/
void  Quadtree::defrence_clear()
{
	//push node goc vao
	std::queue<int> to_process;
	to_process.push(0);

	while (to_process.size() > 0)
	{
		//xét 4 node con
		//node nào là node lá thì xóa hết elt trong đó
		//rồi cho node lá đó là node empty
		int emtpty_child(0);
		int idx_node(to_process.front());
		to_process.pop();
		int fc(list_nodes.get_field(idx_node, node_idx_fc));


		for (int i(0); i < 4; ++i)
		{
			//nếu node con là node nhánh thì push vào to_process
			if (list_nodes.get_field(fc + i, node_count) < 0)
			{
				to_process.push(fc + i);
			}
			else if (list_nodes.get_field(fc + i, node_count) == 0)
			{
				++emtpty_child;
			}
			else
			{
				list_nodes.set_field(fc + i, node_count, 0);
				list_nodes.set_field(fc + i, node_idx_fc, -1);
				//++emtpty_child;
			}
		}

		if (emtpty_child == 4)
		{
			list_nodes.remove(fc+3);
			list_nodes.remove(fc+2);
			list_nodes.remove(fc+1);
			list_nodes.remove(fc+0);

			//set node do thanh node la trong
			list_nodes.set_field(idx_node, node_count, 0);
			list_nodes.set_field(idx_node, node_idx_fc, -1);
		}
	}

	list_enodes.clear();
}



void Quadtree::clear()
{
	list_enodes.clear();
	list_nodes.clear();

	//list_nodes.set_field(list_nodes.insert(), node_count, 0);
	//list_nodes.set_field(0, node_idx_fc, -1);

	list_nodes.data[list_nodes.insert()*node_num + node_count] = 0;
	list_nodes.data[node_idx_fc] = -1;

	
}

void Quadtree::split(std::vector<Entity*>* e, int idx_node, float mx, float my, float sx, float sy, int depth)
{
	IntList list_ID(1);
	int idx_enode(list_nodes.get_field(idx_node, node_idx_fc));

	while (idx_enode != -1)
	{
		list_ID.set_field(list_ID.insert(), 0, list_enodes.get_field(idx_enode, enode_ID_elt));
		int pre(idx_enode);
		idx_enode = list_enodes.get_field(idx_enode, enode_idx_next);

		list_enodes.remove(pre);
	}

	//tao ra 4 node con
	int fc(list_nodes.insert());

	//list_nodes.data[(fc + 0) * node_num + node_count] = 0;
	list_nodes.set_field(fc, node_count, 0);

	//list_nodes.data[(fc + 0) * node_num + node_idx_fc] = -1;
	list_nodes.set_field(fc, node_idx_fc, -1);

	list_nodes.insert();

	//list_nodes.data[(fc + 1) * node_num + node_count] = 0;
	list_nodes.set_field(fc + 1, node_count, 0);

	//list_nodes.data[(fc + 1) * node_num + node_idx_fc] = -1;
	list_nodes.set_field(fc + 1, node_idx_fc, -1);

	list_nodes.insert();

	//list_nodes.data[(fc + 2) * node_num + node_count] = 0;
	list_nodes.set_field(fc + 2, node_count, 0);

	//list_nodes.data[(fc + 2) * node_num + node_idx_fc] = -1;
	list_nodes.set_field(fc + 2, node_idx_fc, -1);

	list_nodes.insert();

	//list_nodes.data[(fc + 3) * node_num + node_count] = 0;
	list_nodes.set_field(fc + 3, node_count, 0);

	//list_nodes.data[(fc + 3) * node_num + node_idx_fc] = -1;
	list_nodes.set_field(fc + 3, node_idx_fc, -1);


	//list_nodes.data[idx_node * node_num + node_idx_fc] = fc;
	list_nodes.set_field(idx_node, node_idx_fc, fc);

	//list_nodes.data[idx_node * node_num + node_count] = -1;
	list_nodes.set_field(idx_node, node_count, -1);


	int id(0);
	sx /= 2.f;
	sy /= 2.f;

	//int size(list_ID.num_elt);
	int size(list_ID.get_num_elt());

	for (int i(0); i < size; ++i)
	{
		//id = list_ID.data[i];
		id = list_ID.get_field(i, 0);

		insert_entities(e, id, fc + 0, mx - sx, my - sy, sx, sy, depth + 1);
		insert_entities(e, id, fc + 1, mx + sx, my - sy, sx, sy, depth + 1);
		insert_entities(e, id, fc + 2, mx + sx, my + sy, sx, sy, depth + 1);
		insert_entities(e, id, fc + 3, mx - sx, my + sy, sx, sy, depth + 1);

	}

}

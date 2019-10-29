#pragma once
#include "Entity.h"
#include "IntList.h"


enum 
{
	//elt_node
	enode_num = 2,
	enode_ID_elt = 0,
	enode_idx_next = 1,

	//quadnode
	node_num = 2,
	node_count = 1,
	node_idx_fc = 0,

	//Quadnodedata
	ndt_num = 6,
	ndt_mx = 0,
	ndt_my = 1,
	ndt_sx = 2,
	ndt_sy = 3,
	ndt_dix_node = 4,
	ndt_depth = 5,
};

struct Elt_node
{
	int32_t idx_elt;
	int32_t idx_next;
};

struct QuadNode
{
	int32_t idx_fc;
	int32_t count;
};

struct QuadNodeData
{
	QuadNodeData(float _mx, float _my, float _sx, float _sy, int32_t _idx_node, int32_t _depth):
		mx(_mx),
		my(_my),
		sx(_sx),
		sy(_sy),
		idx_node(_idx_node),
		depth(_depth)
	{}

	float mx, my, sx, sy;

	int32_t idx_node;
	int32_t depth;
};



class Quadtree
{
public:
	Quadtree(QuadNodeData root, int max_depth, int max_child);
	~Quadtree();

	void insert_entities(std::vector<Entity*>* entity,int ID, int32_t idx_node, float mx, float my, float sx, float sy, int depth);
	void traverse(sf::RenderWindow* window, std::vector<Entity*>* list_enities, int32_t idx_node, float mx, float my, float sx, float sy, int depth);
	QuadNodeData get_root() const;

	void draw(sf::RenderWindow* window, int32_t idx_node, int top, int left, int width, int height);
	void clear();
	void defrence_clear();
private:
	void split(std::vector<Entity*>* e, int idx_node, float mx, float my, float sx, float sy, int depth);

	IntList list_enodes;
	IntList list_nodes;

	QuadNodeData root;
	sf::VertexArray mVertexArray;

	const int MAX_DEPTH;
	const int MAX_CHILD;
};


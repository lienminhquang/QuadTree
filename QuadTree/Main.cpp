#include <SFML/Graphics.hpp>
#include <iostream>
#include "Quadtree.h"
#include <random>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define BOX_MAX_SIZE 5
#define COUNT 100

void create_entity(std::vector<Entity*>* list, Quadtree* qt, float x, float y)
{
	list->push_back(
		new Entity(sf::Color::Blue
			, x
			, y
			, BOX_MAX_SIZE, BOX_MAX_SIZE
			, 50 - rand() % 100 + 10
			, 50 - rand() % 100 + 10)
	);

	qt->insert_entities(list, list->size() - 1, 0, SCREEN_WIDTH / 2,SCREEN_HEIGHT/2, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 1);
}

void build_vertex_array(sf::VertexArray* array, std::vector<Entity*>* lst)
{
	array->clear();
	Entity* e;

	int size(lst->size());
	for (int i(0); i < size; ++i)
	{
		e = (*lst)[i];
		
		array->append(sf::Vertex(sf::Vector2f(e->left					, e->top						), e->color, sf::Vector2f(0					, 0					)));
		array->append(sf::Vertex(sf::Vector2f(e->left + e->width		, e->top						), e->color, sf::Vector2f(e->width			, 0					)));
		array->append(sf::Vertex(sf::Vector2f(e->left + e->width		, e->top + e->height			), e->color, sf::Vector2f(e->width			, e->height			)));
		array->append(sf::Vertex(sf::Vector2f(e->left					, e->top + e->height			), e->color, sf::Vector2f(0					, e->left			)));

		e->color = sf::Color::Blue;
	}
}

void update_entity_and_quadtree(std::vector<Entity*>* lst, Quadtree* qt, float dt)
{
	QuadNodeData root(qt->get_root());
	qt->clear();
	//qt->defrence_clear();

	std::vector<Entity*> vec(*lst);

	int size(lst->size());
	for (int i(0); i < size; ++i)
	{
		vec[i]->update(dt);
		qt->insert_entities(lst, i, 0, root.mx, root.my, root.sx, root.sy, 1);
	}
}

int main()
{
	int count = 1000;

	sf::VertexArray mVertexArray(sf::Quads);	
	srand(0);

	bool is_needto_update_vertexarray(false);

	//-----------tao list entities----------------------
	float sx(SCREEN_WIDTH / 2.f), sy(SCREEN_HEIGHT / 2.f);

	std::vector<Entity*> list_entities;
	Quadtree qt(QuadNodeData(sx, sy, sx, sy, 0, 1), 8, 8);

	for (int i(0); i < count; ++i)
	{
		list_entities.push_back(
			new Entity(sf::Color::Blue
				, rand() % SCREEN_WIDTH
				, rand() % SCREEN_HEIGHT
				, BOX_MAX_SIZE, BOX_MAX_SIZE
				, 50 - rand() % 100 + 10
				, 50 - rand() % 100 + 10)
		);

		qt.insert_entities(&list_entities, i, 0, sx, sy, sx, sy, 1);
	}
	//----------------------------------------------------------


	sf::RenderWindow mwindow(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Quadtree 3");
	sf::Clock mClock;
	sf::Event e;
	
	const float Time_per_frame = 1 / 60.f;
	float time_since_last_update = 0;
	float time_FPS = 0;
	int countFrame = 0;
	QuadNodeData root = qt.get_root();


	while (mwindow.isOpen())
	{

		//---------------------update and process event----------
		time_since_last_update += mClock.restart().asSeconds();


		while (time_since_last_update >= Time_per_frame)
		{
			time_since_last_update -= Time_per_frame;
			
			while (mwindow.pollEvent(e))
			{
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && e.type == sf::Event::MouseMoved)
				{
					create_entity(&list_entities, &qt, e.mouseMove.x, e.mouseMove.y);
					++count;
				}


				if (e.type == sf::Event::Closed)
					mwindow.close();
			}


			update_entity_and_quadtree(&list_entities, &qt, Time_per_frame);


			qt.traverse(&mwindow, &list_entities, 0, root.mx, root.my, root.sx, root.sy, 1);

			is_needto_update_vertexarray = true;
		}
		//------------------------------------------------


		//------------------create vertext array----------
		if (is_needto_update_vertexarray) {
			build_vertex_array(&mVertexArray, &list_entities);
			is_needto_update_vertexarray = false;
		}
		//------------------------------------------------



		//-----------------------Display------------------
		mwindow.clear();		
		qt.draw(&mwindow, 0, sx, sy, sx, sy);

		mwindow.draw(mVertexArray);
		mwindow.display();
		//------------------------------------------------

		//
		//qt.clear();

		//---------------------counting frame-------------
		++countFrame;
		time_FPS += mClock.getElapsedTime().asSeconds();
		if (time_FPS >= 1.f)
		{
			time_FPS -= 1.f;

			//system("cls");
			std::cout << "Particles: " << count << std::endl << "FPS: " << countFrame << std::endl;
			countFrame = 0;
		}
		//------------------------------------------------
	}


	//system("pause");

	return 0;
}
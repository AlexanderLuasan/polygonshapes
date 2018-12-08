#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "poly.h"
#include <random>
#include "line.h"
#include "collision.h"
using namespace std;

void draw(poly * shape);

int main() {
	cout << "hello" << endl;

	al_init();
	al_init_primitives_addon();
	ALLEGRO_DISPLAY * screen;
	ALLEGRO_TIMER * timmer;
	ALLEGRO_EVENT_QUEUE * queue;
	
	screen = al_create_display(800,600);
	timmer = al_create_timer(1 / 60.0);
	queue = al_create_event_queue();
	al_install_keyboard();
	al_start_timer(timmer);

	al_register_event_source(queue,al_get_display_event_source(screen));
	al_register_event_source(queue, al_get_timer_event_source(timmer));
	al_register_event_source(queue, al_get_keyboard_event_source());


	//poly testing

	collisionObject triangle;
	triangle.setX(400);
	triangle.setY(275);
	triangle.addline(line(0,0,0,100));
	triangle.addline(line(0, 100, 200, 0));
	triangle.addline(line(200, 0, 0, 0));
	triangle.centerpoint();
	collisionObject rectangle;
	rectangle.setX(400);
	rectangle.setY(300);
	rectangle.addline(line(-100,50,100,50));
	rectangle.addline(line(100,50, 100, -50));
	rectangle.addline(line(100, -50, -100, -50));
	rectangle.addline(line(-100, -50, -100, 50));
	rectangle.setrotation(0);

	cout << triangle.colides(&rectangle) << endl;

	system("pause");
	int attemp = 0;
	while (true) {
		attemp += 1;
		if (!triangle.colidesforserch(&rectangle, 0, attemp)) {
			break;
		}

	}
	cout << attemp << endl;

	system("pause");

	slope t1;
	t1.deltay = 543;
	t1.deltax = 234;
	t1 = reduce(t1);
	cout << "slope = " << t1.deltay << "/" << t1.deltax << endl;
	cout << "line testing" << endl;
	line test1 = line(3,5,8,5);
	line test2 = line(1,5,10,5);
	line test3 = line(1, 4, 10, 4);

	cout << test1.collision(test1,0,0) << endl;  //1
	cout << test1.collision(test2, 0, 0) << endl;//1
	cout << test1.collision(test3, 0, 0) << endl;//0
	cout << test1.collision(test3, 0, 1) << endl;//1
	

	system("pause");
	bool done = false;
	while (!done) {
		ALLEGRO_EVENT event;
		al_wait_for_event(queue, &event);

		if (event.type == ALLEGRO_EVENT_TIMER) {
			
			ALLEGRO_KEYBOARD_STATE keys;
			al_get_keyboard_state(&keys);

			if (al_key_down(&keys, ALLEGRO_KEY_UP) && al_key_down(&keys, ALLEGRO_KEY_DOWN)) {
				triangle.setY(triangle.gety() + 0);
			}
			else if (al_key_down(&keys, ALLEGRO_KEY_DOWN)) {
				triangle.setY(triangle.gety() + 1);
			}
			else if (al_key_down(&keys, ALLEGRO_KEY_UP)) {
				triangle.setY(triangle.gety() - 1);
			}
			else {
				triangle.setY(triangle.gety() + 0);
			}
			if (al_key_down(&keys, ALLEGRO_KEY_LEFT) && al_key_down(&keys, ALLEGRO_KEY_RIGHT)) {
				triangle.setX(triangle.getx() + 0);
			}
			else if (al_key_down(&keys, ALLEGRO_KEY_LEFT)) {
				triangle.setX(triangle.getx() - 1);
			}
			else if (al_key_down(&keys, ALLEGRO_KEY_RIGHT)) {
				triangle.setX(triangle.getx() + 1);
			}
			else {
				triangle.setX(triangle.getx() + 0);
			}

			if (al_key_down(&keys, ALLEGRO_KEY_Q) && al_key_down(&keys, ALLEGRO_KEY_E)) {
				triangle.setrotation(triangle.getrotation() + 0);
			}
			else if (al_key_down(&keys, ALLEGRO_KEY_Q)) {
				triangle.setrotation(triangle.getrotation() - 1);
			}
			else if (al_key_down(&keys, ALLEGRO_KEY_E)) {
				triangle.setrotation(triangle.getrotation() + 1);
			}

			/*
			collisionObject shape = collisionObject();
			for (int i = 0; i < 6; i++) {
				int x1, y1, x2, y2;
				x1 = (rand() % 50 - 25);
				y1 = (rand() % 50 - 25);
				x2 = (rand() % 50 - 25);
				y2 = (rand() % 50 - 25);
				line c = line(x1,y1,x2,y2);
				shape.addline(c);
			}
			shape.setX(100);
			shape.setY(100);
			*/
			if (triangle.circularcolide(&rectangle)) {
				if (triangle.colides(&rectangle)) {

					int directions[4];
					directions[0] = triangle.top(&rectangle);
					directions[1] = triangle.right(&rectangle);
					directions[2] = triangle.bottom(&rectangle);
					directions[3] = triangle.left(&rectangle);
					int min = 0;
					for (int i = 0; i < 4; i++) {
						if (directions[min] != 0 && directions[i] != 0) {
							if (abs(directions[min]) > abs(directions[i])) {
								min = i;
							}
						}
						else if (directions[i] != 0) {
							min = i;
						}
					}
					cout << "collision: " << min << " ";
					for (int i = 0; i < 4; i++) {
						cout << " " << directions[i];
					}
					cout << endl;
					if (min == 0) {
						triangle.setY(triangle.gety() + directions[min]);
					}
					else if (min == 2) {
						triangle.setY(triangle.gety() + directions[min]);
					}
					else if (min == 1) {
						triangle.setX(triangle.getx() + directions[min]);
					}
					else if (min == 3) {
						triangle.setX(triangle.getx() + directions[min]);
					}
				}
				else {
					cout << "no collision" << endl;
				}
			}
			else {
				cout << "no test" << endl;
			}

			al_clear_to_color(al_map_rgb(255,255,255));
			draw(triangle.getdraw());
			draw(rectangle.getdraw());
			//draw(shape.getdraw());
			al_flip_display();
		}
		else if(event.type ==ALLEGRO_EVENT_DISPLAY_CLOSE){
			done = true;
		}
	}



	system("pause");

	return 0;
}

void draw(poly * shape)
{
	int xc, yc;
	xc = shape->getposx();
	yc = shape->getposy();
	al_draw_filled_circle(xc, yc, 5, al_map_rgb(0,0,0));
	for (int i = 0; i < shape->getsize(); i++) {
		
		lineunit t = shape->getline(i);
		
		al_draw_line(xc+t.x1,yc+t.y1,xc+t.x2,yc+t.y2,al_map_rgb(0,0,0),2);
	}
	delete shape;

}

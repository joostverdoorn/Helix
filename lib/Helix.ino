#include <StandardCplusplus.h>
#include <vector>
#include "Projectile.h"

using namespace std;

int deltaT = 200;
int lastUpdate = 0;

vector<Projectile*> projectiles;
vector<Hop*> hops;


typedef enum Pin {
	StatusPin = 13,

	LedPin1 = 3,
	LedPin2 = 5,
	LedPin3 = 6,

	SwitchPin = 12

} Pin;


void setup() {
	//Serial.println("Blaat");

	Serial.begin(9600);
	Hop *hop1 = new Hop(3);
	Hop *hop2 = new Hop(5);
	Hop *hop3 = new Hop(6);

	//hop3->emit();

	hop1->addRightNeighbour(*hop2);
	hop1->addLeftNeighbour(*hop3);

	hop2->addRightNeighbour(*hop3);
	hop2->addLeftNeighbour(*hop1);

	hop3->addRightNeighbour(*hop1);
	hop3->addLeftNeighbour(*hop2);

	hops.push_back(hop1);
	hops.push_back(hop2);
	hops.push_back(hop3);

	projectiles.push_back(new Projectile(*hop1, Projectile::LEFT));

}

void loop() {
	if(lastUpdate + deltaT < millis()) {
		for(int i = 0; i < projectiles.size(); i++) {
    		projectiles[i]->move();
		}

		lastUpdate = millis();
	}

	for(int i = 0; i < hops.size(); i++) {
		hops[i]->fade();
	}

	//hops[2]->emit();

	//Serial.println("Blaat");


}



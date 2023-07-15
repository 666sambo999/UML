#include <iostream>
#include <conio.h>

using namespace std;

#define MIN_TANK_VOLUME 28
#define MAX_TANK_VOLUME 60

#define Escape 27
#define Enter 13

class Tank
{
	const int VOLUME; // объем бака
	double fuel_level; // уровень бака топливом
public:
	int get_VOLUME()const
	{
		return VOLUME;
	}
	double get_fuel_level()const
	{
		return fuel_level;
	}
	void fill(double fuel)
	{
		if (fuel < 0)return;
		fuel_level += fuel;
		if (fuel_level > VOLUME)fuel_level = VOLUME;
	}
	// создаем бак - конструктор 
	Tank(int volume=39)
		:VOLUME
		(
			volume < MIN_TANK_VOLUME ? MIN_TANK_VOLUME :
			volume > MAX_TANK_VOLUME ? MAX_TANK_VOLUME :
			volume
		)
	{
		fuel_level = 0;
		cout << "Tank is ready" << endl; 
	}
	~Tank()
	{
		cout << "Tank is over" << endl; 
	}
	// методы 
	void info()const
	{
		cout << "Volume: " << VOLUME << " liters.\n";
		cout << "Fuel lvl: " << fuel_level << " liters.\n";
	}

};


#define MIN_ENGINE_CONSUMPTION 4
#define MAX_ENGINE_CONSUMPTION 25
class Engine
{
	double consumption; // потребление 
	double fuel_consumption; // потребления в секунду
	bool is_started; // запуск двигателя 
public:
	double get_consumption()const
	{
		return consumption;
	}
	double get_fuel_consumption()const
	{
		return fuel_consumption;
	}
	bool started()const
	{
		return is_started;
	}
	void start()
	{
		is_started = true;
	}
	void stop()
	{
		is_started = false;
	}
	void set_consumption(double consumption)
	{
		if (consumption < MIN_ENGINE_CONSUMPTION)consumption = MIN_ENGINE_CONSUMPTION;
		if (consumption > MAX_ENGINE_CONSUMPTION)consumption = MAX_ENGINE_CONSUMPTION;
		this->consumption = consumption;
		set_fuel_consumption();
	}
	void set_fuel_consumption()
	{
		fuel_consumption = consumption * 3e-5;
	}
	// Конструктор 
	Engine(double consumption)
	{
		set_consumption(consumption);
		is_started = false;
		cout << "Engine is ready" << endl;
	}
	~Engine()
	{
		cout << "Engine is over" << endl;
	}
	void info()const
	{
		cout << "Consumption per 100 km: " << consumption << " liters.\n";
		cout << "Consumption per second: " << fuel_consumption << " liters.\n";
		cout << "Engine is " << (is_started ? "started" : "stopted") << endl; 
	}
};

#define MAX_SPEED_LOW_LIMIT 40
#define MAX_SPEED_HIGH_LIMIT 400

class Car
{
	Engine engine;
	Tank tank;
	int speed;
	const int MAX_SPEED;
	bool driver_inside;
public:
	Car(const Engine& engine, const Tank& tank, int max_speed) :
		engine(engine),
		tank(tank),
		speed(0),
		MAX_SPEED
		(
			max_speed < MAX_SPEED_LOW_LIMIT ? MAX_SPEED_LOW_LIMIT :
			max_speed > MAX_SPEED_HIGH_LIMIT ? MAX_SPEED_HIGH_LIMIT :
			max_speed
		)
	{
		driver_inside = false;
		cout << "Your car is ready to go." << endl; 
	}

	Car(int consumption, int volume, int max_speed) :
		engine(consumption),
		tank(volume),
		speed(0),
		MAX_SPEED
		(
			max_speed < MAX_SPEED_LOW_LIMIT ? MAX_SPEED_LOW_LIMIT :
			max_speed > MAX_SPEED_HIGH_LIMIT ? MAX_SPEED_HIGH_LIMIT :
			max_speed
		)
	{
		driver_inside = false; 
		cout << "Your car is ready to go." << endl;
	}
	
	void get_in()
	{
		driver_inside = true;
		panel();
	}
	void get_out()
	{
		driver_inside = false; 
	}
	
	void control()
	{
		cout << "press Enter to get in" << endl; 
		char key;
		do
		{
			key = _getch();
			switch(key)
			{
			case 'F':case 'f': 
				double fuel;
				cout << "Введите объем топлива: "; cin >> fuel;
				tank.fill(fuel);
				break;
			case 'E': case 'e': case Enter:
				if (driver_inside)get_out();
				else get_in();
				break; 
			}
		} while (key != Escape);
	}
	
	void panel()const
	{
		while (driver_inside)
		{
			system("CLS");
			cout << "Fuel level:\t" << tank.get_fuel_level() << "liters.\n";
			cout << "Engine is " << (engine.started() ? "started" : "stoted") << endl;
		}
	}
	
	void fill(double fuel)
	{
		tank.fill(fuel);
	}

	void info()const 
	{
		cout << "\n================================================\n" << endl; 
		engine.info();
		tank.info();
		cout << "Max speed: " << MAX_SPEED << " km\h.\n";
		cout << "\n================================================\n" << endl; 
	}

	~Car()
	{
		cout << "End: game over '_' " << endl;
	}
};


//#define TANK_CHECK
//#define ENGINE_CHECK
void main()
{
	setlocale(LC_ALL, "");
	
#ifdef TANK_CHECK

	Tank tank;
	double fuel;
	/*do
	{
		cout << "Введите объем топлива: "; cin >> fuel;
		tank.fill(fuel);
		tank.info();
	} while (true);*/
#endif // TANK_CHECK

#ifdef ENGINE_CHECK
	Engine engine(12);
	engine.info();
#endif // ENGINE_CHECK

	//Car vaz2106(engine, tank, 180); vaz2106.info();1 вариант 

	//2 вариант 
	//Car vaz2106(Engine(12), Tank(39), 180); vaz2106.info();

	Car vaz2106(12, 39, 180); // vazeraty
	/*vaz2106.fill(25);
	vaz2106.info(); */
	vaz2106.control();

}
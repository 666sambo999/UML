#include <iostream>
#include <conio.h>
#include <thread>
#include <Windows.h>

using namespace std;

#define MIN_TANK_VOLUME 28
#define MAX_TANK_VOLUME 60

#define Escape 27
#define Enter 13

// Вазератти 2106

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
	double give_fuel(double amount)
	{
		fuel_level -= amount;
		if (fuel_level < 0)fuel_level = 0;
		return fuel_level;
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
	double default_fuel_consumption; // потребления в секунду
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
	double set_fuel_consumption()
	{
		return fuel_consumption = default_fuel_consumption = consumption * 3e-5;
	}
	double set_fuel_consumption(int speed)
	{
		if (speed == 0)fuel_consumption = consumption * 3e-5;
		else if (speed <= 60) fuel_consumption = default_fuel_consumption * 20 / 3;
		else if (speed <=70 ) fuel_consumption = default_fuel_consumption * 40 + default_fuel_consumption  * 20 / 3;
		else if (speed <= 120) fuel_consumption = default_fuel_consumption * 20 / 3;
		else if (speed <= 140) fuel_consumption = default_fuel_consumption * 50 / 6;
		else fuel_consumption = default_fuel_consumption * 10;
		return fuel_consumption;
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
	int accelleration; 
	bool driver_inside;
	struct Threads
	{
		std::thread panel_thread;
		std::thread engine_idle_thread;
		std::thread free_wheeling_thread;
	}threads;
public:
	Car(const Engine& engine=10, const Tank& tank=60, int max_speed=250,int acceleration=10) :
		engine(engine),
		tank(tank),
		speed(0),
		accelleration(acceleration),
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

	Car(int consumption=10, int volume=60, int max_speed=250,int accelleration =10) :
		engine(consumption),
		tank(volume),
		speed(0),
		accelleration(accelleration),
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
		//panel();
		threads.panel_thread = std::thread(&Car::panel, this);
	}
	void get_out()
	{
		driver_inside = false; 
		if (threads.panel_thread.joinable())threads.panel_thread.join();
		system("CLS");
		cout << "You are of your car" << endl; 
	}
	
	void control()
	{
		cout << "press Enter to get in" << endl; 
		char key=0;
		do
		{
			if (_kbhit())
			{
				key = _getch();
				switch (key)
				{
				case 'F':case 'f':
					if (driver_inside)
					{
						cout << "Слышь, из тачки выйди ... " << endl;
					}
					else
					{
						double fuel;
						cout << "Введите объем топлива: "; cin >> fuel;
						tank.fill(fuel);
						//cin.ignore();
						key = 0;
						cout << "Ваш автомобиль заправлен. Have a nice day!" << endl;
					}
					break;
				case 'E': case 'e': case Enter:
					if (speed)
					{
						cout << "нельзя так делать!!!!" << endl;
						std::this_thread::sleep_for(1s);
						break;
					}
					if (driver_inside)get_out();
					else get_in();
					break;
				case'I':case'i': // зажигание 
					if (driver_inside) engine.started() ? stop() : start();
					break;
				case'W':case'w':
					if (driver_inside && engine.started())
					{
						accellerate();
						//std::this_thread::sleep_for(1s);
					}
					break;
				case'S': case's':
					if (driver_inside)
					{
						slow_down();
						//std::this_thread::sleep_for(1s);
					}
					break;
				case Escape:
					if (speed) speed = 0;
					stop();
					get_out();
				}
			}
			engine.set_fuel_consumption(speed);
			if (tank.get_fuel_level() == 0)stop();
			if (speed && !threads.free_wheeling_thread.joinable())
				threads.free_wheeling_thread = std::thread(&Car::free_wheeling, this);
			if (speed == 0 && threads.free_wheeling_thread.joinable())
				threads.free_wheeling_thread.join();
		} while (key != Escape);
	}
	
	void engine_idle()
	{
		while (engine.started() && tank.give_fuel(engine.get_fuel_consumption()))
			std::this_thread::sleep_for(1s);
	}
	
	void start()
	{
		if (driver_inside && tank.get_fuel_level())
		{
			engine.start();
			threads.engine_idle_thread = std::thread(&Car::engine_idle, this);
		}
	}

	void stop()
	{
		engine.stop();
		if (threads.engine_idle_thread.joinable())threads.engine_idle_thread.join();
	}

	void panel()const
	{
		while (driver_inside)
		{
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			system("CLS");
			double coefficient = 2.3;
			for (int i = 0; i < MAX_SPEED / coefficient; i++)
			{
				SetConsoleTextAttribute(hConsole, 0x02);
				if (i > 70/ coefficient)SetConsoleTextAttribute(hConsole, 0x0E);
				if (i > 120/ coefficient)SetConsoleTextAttribute(hConsole, 0x0C);
				cout << (i < speed / coefficient ? "|" : ".");
				SetConsoleTextAttribute(hConsole, 0x07);
			}
			cout << endl; 
			cout << "Fuel level:\t" << tank.get_fuel_level() << "liters.\t";
			if (tank.get_fuel_level() <= 5)
			{
				SetConsoleTextAttribute(hConsole, 0xCF);
				cout << " LOW FUEL ";
				SetConsoleTextAttribute(hConsole, 0x07);
			}
			cout << endl; 
			cout << "Engine is " << (engine.started() ? "started" : "stoted") << endl;
			cout << "Speed:\t" << speed << " km/h." << endl; 
			cout << "Consumption fuel: " << engine.get_fuel_consumption() << " liters" << endl;
			std::this_thread::sleep_for(1s);
		}
	}

	void free_wheeling()
	{
		while (speed)
		{
			if (speed-- < 0)speed = 0;
			//engine.set_fuel_consumption(speed);
			std::this_thread::sleep_for(1s);
		}
	}

	void accellerate()
	{
		//if(driver_inside && engine.started()) 
		speed += accelleration;
		if (speed > MAX_SPEED)speed = MAX_SPEED;
		std::this_thread::sleep_for(1s);
	}
	void slow_down()
	{
		//if(driver_inside) 
		if (speed) speed -= accelleration*2;
		if (speed < 0)speed = 0;
		std::this_thread::sleep_for(1s);
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
	
	
	// vazeraty
	Car vaz2106(10, 39, 180, 15); 
	//vaz2106.fill(25);
	vaz2106.info();
	vaz2106.control();

}
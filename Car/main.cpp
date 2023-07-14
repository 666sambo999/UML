#include <iostream>
using namespace std;

#define MIN_TANK_VOLUME 28
#define MAX_TANK_VOLUME 60

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


//#define TANK_CHECK

void main()
{
	setlocale(LC_ALL, "");
	
#ifdef TANK_CHECK

	Tank tank;
	double fuel;
	do
	{
		cout << "Введите объем топлива: "; cin >> fuel;
		tank.fill(fuel);
		tank.info();
	} while (true);
#endif // TANK_CHECK

	Engine engine(10);
	engine.info();
}
#include <iostream>
#include <thread>
using namespace std;
using namespace std::chrono_literals;

bool finish = false;

void Plus()
{
	while (!finish)
	{
		cout << "+ ";
		std::this_thread::sleep_for(1s);
	}
}

void Minus()
{
	while (!finish)
	{
		cout << "- ";
		std::this_thread::sleep_for(1s);
	}
}

void main()
{
	setlocale(LC_ALL, "");
	// 1 вариант ------------------
	//Plus();
	//Minus();
	
	// 2 вариант ------------------------------
	/*std::thread plus_thread = std:: thread(Plus);
	std::thread minus_thread = std::thread(Minus);*/

	// 3 вариант -----------------------
	std::thread plus_thread(Plus);
	std::thread minus_thread(Minus);
	
	cin.get(); // функция ожидает нажатия Enter
	finish = true; 

	// нужно синхронизировать потоки 
	if (minus_thread.joinable())minus_thread.join();
	if (plus_thread.joinable())plus_thread.join();
}
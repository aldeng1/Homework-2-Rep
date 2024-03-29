#include<iostream>
#include<chrono>
#include <thread>
#include<mutex>
#include<array>
#include <semaphore>
#include <condition_variable>


//std::lock_guard<std::mutex> lock(cout_mutex);

//std::counting_semaphore<5> enabled{ 1 };

std::binary_semaphore enabled1(1);

std::mutex cout_mutex;
std::mutex m;
std::mutex mtx;
std::condition_variable cv;

bool ATCsleep = true;		// air traffic control is sleeping
int aircraftinpattern = 0;


void pilotprocedure(int aircraftid, int landtime)
{

	if (ATCsleep==true && aircraftinpattern<3)
	{
		ATCsleep = false;		//air traffic control awakes

		aircraftinpattern++;

		{
			std::lock_guard<std::mutex> lock(cout_mutex);				//Allows for multiple cout to not occur at same time and get mixed together
			std::cout << "Aircraft #" << aircraftid << " requesting landing." << std::endl;
			
		}

		{
			std::lock_guard<std::mutex> lock(cout_mutex);			//Allows for multiple cout to not occur at same time and get mixed together
			std::cout << "Aircraft #" << aircraftid << " is cleared to land." << std::endl;
		}

		{
			std::lock_guard<std::mutex> lock(cout_mutex);		//Allows for multiple cout to not occur at same time and get mixed together
			std::cout << "Runway is now free." << std::endl;
		}

		std::this_thread::sleep_for(std::chrono::seconds(landtime));

		aircraftinpattern--;		// decreases the aircraft in pattern

		{
			std::lock_guard<std::mutex> lock(m);
			ATCsleep = true;			// air traffic control sleeps
		}

		cv.notify_one();

	}
	else if (ATCsleep==false)
	{

		if (aircraftinpattern <= 3)
		{		
			aircraftinpattern++;    // adds to the aircraft pattern variable to only allow 3 aircaft to be in rotation at a time

			{
				std::lock_guard<std::mutex> lock(cout_mutex);	//Allows for multiple cout to not occur at same time and get mixed together
				std::cout << "Aircraft #" << aircraftid << " requesting landing." << std::endl;
			}

			std::unique_lock<std::mutex> lock(m);
			cv.wait(lock, [] { return ATCsleep; });			//waits till condtional variables is released

			aircraftinpattern--;		//decreases aircraft in pattern to allow another one in if there is one

			enabled1.acquire();			//allows 1 thread in at a time

			ATCsleep = false;			//sets air traffic control tbe awake

			{
				std::lock_guard<std::mutex> lock(cout_mutex);	//Allows for multiple cout to not occur at same time and get mixed together
				std::cout << "Aircraft #" << aircraftid << " is cleared to land." << std::endl;
			}

			std::this_thread::sleep_for(std::chrono::seconds(landtime));     //sleeps for the 1 second for aircraft to land

			{
				std::lock_guard<std::mutex> lock(cout_mutex);	//Allows for multiple cout to not occur at same time and get mixed together
				std::cout << "Runway is now free." << std::endl;
			}

			//aircraftinpattern--;		//decreases aircraft in pattern to allow another one in if there is one

			enabled1.release();		// releases 1 thread to allow another one n

			{
				std::lock_guard<std::mutex> lock(mtx);
				ATCsleep = true;
			}

			cv.notify_one();				//notifices the wake up conidtional variable 

		}
		else
		{

			{
				std::lock_guard<std::mutex> lock(cout_mutex);	//Allows for multiple cout to not occur at same time and get mixed together
				std::cout << "Aircraft #" << aircraftid << " requesting landing." << std::endl;
			}

			{
				std::lock_guard<std::mutex> lock(cout_mutex);	//Allows for multiple cout to not occur at same time and get mixed together
				std::cout << "Approach pattern is full. Aircraft #" << aircraftid << " redirected to another airport." << std::endl;
			}

			{
				std::lock_guard<std::mutex> lock(cout_mutex);	//Allows for multiple cout to not occur at same time and get mixed together
				std::cout << "Aircraft #" << aircraftid << " flying to another airport." << std::endl;
			}

		}

	}

}




int main()
{
	int maxaircraft = 3, landingtime = 1;

	int aircraft[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	auto startTime = std::chrono::high_resolution_clock::now();		//Starts clock

	std::thread t1(pilotprocedure, aircraft[0], landingtime); // Creates thread 1 with specific aircraft and conditions
	std::thread t2(pilotprocedure, aircraft[1], landingtime); // Creates thread 2 with specific aircraft and conditions
	std::thread t3(pilotprocedure, aircraft[2], landingtime); // Creates thread 3 with specific aircraft and conditions
	std::thread t4(pilotprocedure, aircraft[3], landingtime); // Creates thread 4 with specific aircraft and conditions
	std::thread t5(pilotprocedure, aircraft[4], landingtime); // Creates thread 5 with specific aircraft and conditions
	std::thread t6(pilotprocedure, aircraft[5], landingtime); // Creates thread 6 with specific aircraft and conditions
	std::thread t7(pilotprocedure, aircraft[6], landingtime); // Creates thread 7 with specific aircraft and conditions
	std::thread t8(pilotprocedure, aircraft[7], landingtime); // Creates thread 8 with specific aircraft and conditions
	std::thread t9(pilotprocedure, aircraft[8], landingtime); // Creates thread 9 with specific aircraft and conditions
	std::thread t10(pilotprocedure, aircraft[9],landingtime); // Creates thread 10 with specific aircraft and conditions
	
	t1.join(); // Joins thread 1
	t2.join(); // Joins thread 2
	t3.join(); // Joins thread 3
	t4.join(); // Joins thread 4
	t5.join(); // Joins thread 5
	t6.join(); // Joins thread 6
	t7.join(); // Joins thread 7
	t8.join(); // Joins thread 8
	t9.join(); // Joins thread 9
	t10.join(); // Joins thread 10

	auto stopTime = std::chrono::high_resolution_clock::now();		//ends clock

	auto duration = std::chrono::duration_cast<std::chrono::seconds>(stopTime - startTime);		//calculated duraton of time

	std::cout << "Duration: " << duration.count() << " seconds" << std::endl;

	return 0;

}
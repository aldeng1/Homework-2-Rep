#include<iostream>
#include<chrono>
#include <thread>
#include<mutex>
#include<array>
#include <semaphore>


std::counting_semaphore<5> enabled{2};	 //Creates the semiphore for 5 thread with only allowing two to work at same time

std::mutex cout_mutex;   //creates a mutex

void robottask(int robotid, int grabtools1, int preformtask1, std::array<std::mutex, 5>& mutexes)
{
	//std::lock_guard<std::mutex> lock(cout_mutex);

	std::lock(mutexes[robotid], mutexes[(robotid + 1) % 5]);	// Lock mutexes for current and next robots
	enabled.acquire();	// Acquire semaphore to ensure only two robots work simultaneously

	{
		std::lock_guard<std::mutex> lock(cout_mutex);    //Allows for multiple cout to not occur at same time and get mixed together
		std::cout << "Robot " << robotid << " is collecting data." << std::endl;
	}

	std::this_thread::sleep_for(std::chrono::seconds(grabtools1));     //Allows robots to grab tools

	{
		std::lock_guard<std::mutex> lock(cout_mutex);	//Allows for multiple cout to not occur at same time and get mixed together
		std::cout << "Robot " << robotid << " aquired tools and starts performing a task." << std::endl;
	}

	std::this_thread::sleep_for(std::chrono::seconds(preformtask1));     // Allows robots to perfrom task and return tools

	{
		std::lock_guard<std::mutex> lock(cout_mutex);			//Allows for multiple cout to not occur at same time and get mixed together
		std::cout << "Robot " << robotid << " finished the task and returning the tools." << std::endl;
	}

	enabled.release();		// Release semaphore after task completion
	mutexes[robotid].unlock();		 // Unlock the mutex for the current robot
	mutexes[(robotid + 1) % 5].unlock();		 // Unlock the mutex for the next robot
	
}



int main() 
{
	int robot[5] = { 0,1,2,3,4 };
	int grabtools = 1, performtask = 5;

	auto startTime =std::chrono::high_resolution_clock::now();     //Starts time for duration calculation

	std::array<std::mutex, 5> mutexes;

	std::thread t1(robottask, robot[0], grabtools, performtask, std::ref(mutexes));				//Creates thread 1 with specific robot and conditions
	std::thread t2(robottask, robot[1], grabtools, performtask, std::ref(mutexes));			//Creates thread 2 with specific robot and conditions
	std::thread t3(robottask, robot[2], grabtools, performtask, std::ref(mutexes));			//Creates thread 3 with specific robot and conditions
	std::thread t4(robottask, robot[3], grabtools, performtask, std::ref(mutexes));			//Creates thread 4 with specific robot and conditions
	std::thread t5(robottask, robot[4], grabtools, performtask, std::ref(mutexes));			//Creates thread 5 with specific robot and conditions
	t1.join();   //Joins thread 1
	t2.join();	//Joins thread 2
	t3.join();	//Joins thread 3
	t4.join();	//Joins thread 4
	t5.join();	//Joins thread 5

	auto stopTime =std::chrono::high_resolution_clock::now();		//Ends time for duration calculation

	auto duration = std::chrono::duration_cast<std::chrono::seconds>(stopTime - startTime);		// perfroms duration calculation
		
	std::cout << "duration : " << duration.count() <<" seconds" << std::endl;		//outputs duration

	return 0;
}
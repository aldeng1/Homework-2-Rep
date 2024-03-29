#include <assert.h>
#include<iostream>
#include<chrono>
#include <string>
#include <vector>



class Sensor //Abstract Class
{
    public:

      //  Sensor();
        
        virtual void gatherData()=0;     //Abstract Class virtual call
        virtual void processData()=0;      //Abstract Class virtual call
            
};

class pressure : public Sensor {            //Pressure Sensor class

    public:

        //pressure();

        void gatherData() 
        {
            std::cout << "Gathering data from pressure sensor" << std::endl;        //outputs gather data phrase
        };

        void processData()
        {
            std::cout << "Processing data from pressure sensor" << std::endl;    //outputs process data phrase
        };
        

};

class velocity : public Sensor {        //velocty sensor class

    public:

       // velocity();

        void gatherData() 
        {
            std::cout << "Gathering data from velocity sensor" << std::endl;       //outputs gather data phrase
        };

        void processData()
        {
            std::cout << "Processing data from velocity sensor" << std::endl;       //outputs process data phrase
        };

};

class position : public Sensor {        //position sensor class

    public:

       // position();

         void gatherData() 
        {
            std::cout << "Gathering data from position sensor" << std::endl;     //outputs gather data phrase
        };

        void processData()
        {
            std::cout << "Processing data from position sensor" << std::endl;           //outputs process data phrase
        };

};

class SensorFactory 
{
    public:

    static Sensor* createSensor(const std::string sensorname)           //static function for pointer
    {
        if (sensorname == "pressure")                   // If and Else if function that assigns a pointer to that specfic sensor or outputs nothing.
        {
            return new pressure();
        }
        else if (sensorname == "velocity")
        {
            return new velocity();
        }
        else if (sensorname == "position")
        {
            return new position();
        }
        else 
        {
            std::cout << "Unkown sensor name " << sensorname << std::endl;

            return nullptr;             // null pointer call
        }
    }

};

class AerospaceControlSystem
{
private:

    std::vector <Sensor*> sensorsname;      // vector that stores the pointers to help with memory loss

public:

    void addSensor(Sensor* name)  // adds pointer or sensor name to vector 
    {
        sensorsname.push_back(name);        
    }

    void monitorAndAdjust() 
    {
        for (Sensor* name:sensorsname)          //calls all the sensors and outputs adjustng contorls
        {
            name->gatherData();
            name->processData();
            std::cout << "Adjusting controls based on sensor data." << std::endl;

        }

    }

};


int main()
{
   
    AerospaceControlSystem ctrlsys;         // creates an object for AerospaceControlSystsem

    ctrlsys.addSensor(SensorFactory::createSensor("pressure"));         //used the object to call the addsensor function with the correct pointer determined from sensorfactory
    ctrlsys.addSensor(SensorFactory::createSensor("velocity"));          //used the object to call the addsensor function with the correct pointer determined from sensorfactory
    ctrlsys.addSensor(SensorFactory::createSensor("position"));         //used the object to call the addsensor function with the correct pointer determined from sensorfactory   

    ctrlsys.monitorAndAdjust(); //used the object to call the monitor and ajusudt function

    return 0;

}
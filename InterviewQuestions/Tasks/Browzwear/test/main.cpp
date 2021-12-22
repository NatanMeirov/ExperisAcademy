#include <iostream> // std::cout, std::endl
#include <string>
#include <memory> // std::shared_ptr
#include <vector>
#include <algorithm> // std::equal
#include "reflector.hpp"
#include "sf_base.hpp"
#include "sf_point.hpp"
#include "sf_circle.hpp"
#include "sf_point_factory.hpp"
#include "sf_circle_factory.hpp"
#include "in_archive.hpp"
#include "out_archive.hpp"
#include "json_file_handler.hpp"
#include "json_formatter.hpp"
#include "xml_file_handler.hpp"
#include "xml_formatter.hpp"
#include "example_file_handler.hpp"
#include "example_formatter.hpp"


using namespace ser_fw;
using namespace ser_fw_test;


bool ExampleFormatTest();
bool JsonFileTest();
bool XMLFileTest();


int main()
{
    std::cout << "Example Format test: " << (ExampleFormatTest() ? "Passed!" : "Failed...") << std::endl;
    // std::cout << "Json test: " << (JsonFileTest() ? "Passed!" : "Failed...") << std::endl;
    // std::cout << "XML test: " << (XMLFileTest() ? "Passed!" : "Failed...") << std::endl;

    return 0;
}

bool ExampleFormatTest()
{
    std::vector<std::shared_ptr<Base>> objects;
    objects.push_back(std::make_shared<Point>("p1", 1, 2));
    objects.push_back(std::make_shared<Point>("p2", 50, 12));
    objects.push_back(std::make_shared<Circle>("c1", 0, 0, 10));

    {
        OutArchive write_archive("data.txt", std::make_shared<ExampleFileHandler>(), std::make_shared<ExampleFormatter>());
        write_archive.Write(objects);
    }

    std::vector<std::shared_ptr<Base>> loaded_objects;
    Reflector reflector;
    reflector.AddReflection("Point", std::make_shared<PointFactory>());
    reflector.AddReflection("Circle", std::make_shared<CircleFactory>());

    InArchive read_archive("data.txt", std::make_shared<XMLFileHandler>(), std::make_shared<XMLFormatter>());
    read_archive.Read(loaded_objects, reflector);

    // loaded_objects should contain the same objects as in objects
    return std::equal(objects.begin(), objects.end(), loaded_objects.begin());
}


bool JsonFileTest()
{
    // TODO: finish implementing JsonFormatter and JsonFileHandler
    std::vector<std::shared_ptr<Base>> objects;
    objects.push_back(std::make_shared<Point>("p1", 1, 2));
    objects.push_back(std::make_shared<Point>("p2", 50, 12));
    objects.push_back(std::make_shared<Circle>("c1", 0, 0, 10));

    {
        OutArchive write_archive("data.json", std::make_shared<JsonFileHandler>(), std::make_shared<JsonFormatter>());
        write_archive.Write(objects);
    }

    std::vector<std::shared_ptr<Base>> loaded_objects;
    Reflector reflector;
    reflector.AddReflection("Point", std::make_shared<PointFactory>());
    reflector.AddReflection("Circle", std::make_shared<CircleFactory>());

    InArchive read_archive("data.json", std::make_shared<JsonFileHandler>(), std::make_shared<JsonFormatter>());
    read_archive.Read(loaded_objects, reflector);

    // loaded_objects should contain the same objects as in objects
    return std::equal(objects.begin(), objects.end(), loaded_objects.begin());
}


bool XMLFileTest()
{
    // TODO: finish implementing XMLFormatter and XMLFileHandler
    std::vector<std::shared_ptr<Base>> objects;
    objects.push_back(std::make_shared<Point>("p1", 1, 2));
    objects.push_back(std::make_shared<Point>("p2", 50, 12));
    objects.push_back(std::make_shared<Circle>("c1", 0, 0, 10));

    {
        OutArchive write_archive("data.xml", std::make_shared<XMLFileHandler>(), std::make_shared<XMLFormatter>());
        write_archive.Write(objects);
    }

    std::vector<std::shared_ptr<Base>> loaded_objects;
    Reflector reflector;
    reflector.AddReflection("Point", std::make_shared<PointFactory>());
    reflector.AddReflection("Circle", std::make_shared<CircleFactory>());

    InArchive read_archive("data.xml", std::make_shared<XMLFileHandler>(), std::make_shared<XMLFormatter>());
    read_archive.Read(loaded_objects, reflector);

    // loaded_objects should contain the same objects as in objects
    return std::equal(objects.begin(), objects.end(), loaded_objects.begin());
}

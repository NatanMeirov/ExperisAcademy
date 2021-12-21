#include <iostream> // std::cout, std::endl
#include <string>
#include <memory> // std::shared_ptr
#include <algorithm> // std::equal
#include "sf_base.hpp"
#include "sf_point.hpp"
#include "sf_circle.hpp"
#include "in_archive.hpp"
#include "out_archive.hpp"
#include "json_file_handler.hpp"
#include "json_formatter.hpp"
#include "xml_file_handler.hpp"
#include "xml_formatter.hpp"


using namespace ser_fw;


bool JsonFileTest();
bool XMLFileTest();


int main()
{
    std::cout << "Json test: " << (JsonFileTest() ? "Passed!" : "Failed...") << std::endl;
    std::cout << "XML test: " << (XMLFileTest() ? "Passed!" : "Failed...") << std::endl;

    return 0;
}


bool JsonFileTest()
{
    OutArchive::SerFwObjsContainer objects;
    objects.push_back(std::make_shared<Point>("p1", 1, 2));
    objects.push_back(std::make_shared<Point>("p2", 50, 12));
    objects.push_back(std::make_shared<Circle>("c1", 0, 0, 10));

    {
        OutArchive write_archive("data.json", std::make_shared<JsonFileHandler>(), std::make_shared<JsonFormatter>());
        write_archive.Write(objects);
    }

    InArchive::SerFwObjsContainer loaded_objects;
    InArchive read_archive("data.json", std::make_shared<JsonFileHandler>(), std::make_shared<JsonFormatter>());
    read_archive.Read(loaded_objects);

    // loaded_objects should contain the same objects as in objects
    return std::equal(objects.begin(), objects.end(), loaded_objects.begin());
}


bool XMLFileTest()
{
    OutArchive::SerFwObjsContainer objects;
    objects.push_back(std::make_shared<Point>("p1", 1, 2));
    objects.push_back(std::make_shared<Point>("p2", 50, 12));
    objects.push_back(std::make_shared<Circle>("c1", 0, 0, 10));

    {
        OutArchive write_archive("data.xml", std::make_shared<XMLFileHandler>(), std::make_shared<XMLFormatter>());
        write_archive.Write(objects);
    }

    InArchive::SerFwObjsContainer loaded_objects;
    InArchive read_archive("data.xml", std::make_shared<XMLFileHandler>(), std::make_shared<XMLFormatter>());
    read_archive.Read(loaded_objects);

    // loaded_objects should contain the same objects as in objects
    return std::equal(objects.begin(), objects.end(), loaded_objects.begin());
}

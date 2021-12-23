#include <iostream> // std::cout, std::endl
#include <string>
#include <memory> // std::shared_ptr
#include <vector>
#include <algorithm> // std::for_each
#include "iformatted_file_writer.hpp"
#include "reflector.hpp"
#include "iser_fw_obj.hpp"
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

bool ExampleFormatTest_SharedPtr();
bool ExampleFormatTest_RawPtr();
void PrintISerFwObjsCollection_SharedPtr(std::vector<std::shared_ptr<infra::ISerFwObj>> a_container);
void PrintISerFwObjsCollection_RawPtr(std::vector<infra::ISerFwObj*> a_container);


// FOR BOTH TEST CASES - NO VALIDATION ABOUT THE PTR TYPE! (PTR SHOULD BE ISerFwObj (raw or shared_ptr))
template <typename ISerFwObjPtrT>
inline bool AreEqualISerFwObjsCollection(std::vector<ISerFwObjPtrT> a_first, std::vector<ISerFwObjPtrT> a_second)
{
    auto itrFirst = a_first.begin();
    auto itrSecond = a_second.begin();
    auto firstEnd = a_first.end();
    auto secondEnd = a_second.end();

    while(itrFirst != firstEnd && itrSecond != secondEnd)
    {
        if((*itrFirst)->TransformTo() != (*itrSecond)->TransformTo())
        {
            return false;
        }

        ++itrFirst;
        ++itrSecond;
    }

    return itrFirst == firstEnd && itrSecond == secondEnd;
}


int main()
{
    std::cout << "Example Format test (shared_ptr):\n" << (ExampleFormatTest_SharedPtr() ? "Shared_Ptr - Passed!" : "Failed...") << std::endl;
    std::cout << std::endl;
    std::cout << "Example Format test (raw pointer):\n" << (ExampleFormatTest_RawPtr() ? "Raw_Ptr - Passed!" : "Failed...") << std::endl;

    return 0;
}


bool ExampleFormatTest_SharedPtr()
{
    std::vector<std::shared_ptr<infra::ISerFwObj>> objects;
    objects.push_back(std::make_shared<Point>("p1", 1, 2));
    objects.push_back(std::make_shared<Point>("p2", 50, 12));
    objects.push_back(std::make_shared<Circle>("c1", 0, 0, 10));
    objects.push_back(std::make_shared<Circle>("c2", 5, 5, 5));
    objects.push_back(std::make_shared<Circle>("c3", 1000, 100, 20));

    {
        OutArchive<> write_archive("data.txt", std::make_shared<ExampleFileHandler>(), std::make_shared<ExampleFormatter>());
        write_archive.Write(objects);
    }

    std::vector<std::shared_ptr<infra::ISerFwObj>> loaded_objects;
    Reflector<> reflector;
    reflector.AddReflection("Point", std::make_shared<PointFactory>());
    reflector.AddReflection("Circle", std::make_shared<CircleFactory>());

    InArchive<> read_archive("data.txt", std::make_shared<ExampleFileHandler>(), std::make_shared<ExampleFormatter>());
    read_archive.Read(loaded_objects, reflector);

    // loaded_objects should contain the same objects as in objects
    std::cout << "objects content:" << std::endl;
    PrintISerFwObjsCollection_SharedPtr(objects);
    std::cout << "loaded_objects content:" << std::endl;
    PrintISerFwObjsCollection_SharedPtr(loaded_objects);

    return AreEqualISerFwObjsCollection(objects, loaded_objects);
}


bool ExampleFormatTest_RawPtr()
{
    // Important Note: I am NOT deleting the raw pointers in this test case.
    std::vector<infra::ISerFwObj*> objects;
    objects.push_back(new Point("p1", 1, 2));
    objects.push_back(new Point("p2", 50, 12));
    objects.push_back(new Circle("c1", 0, 0, 10));
    objects.push_back(new Circle("c2", 5, 5, 5));
    objects.push_back(new Circle("c3", 1000, 100, 20));

    {
        OutArchive<infra::IFormattedFileWriter*, infra::ISerializationFormatter*> write_archive("data.txt", new ExampleFileHandler(), new ExampleFormatter());
        write_archive.Write(objects);
    }

    std::vector<infra::ISerFwObj*> loaded_objects;
    Reflector<infra::ISerFwObjFactory<infra::ISerFwObj*>*, infra::ISerFwObj*> reflector;
    reflector.AddReflection("Point", new RawPointFactory());
    reflector.AddReflection("Circle", new RawCircleFactory());

    InArchive<infra::IFormattedFileReader*, infra::IDeserializationFormatter*> read_archive("data.txt", new ExampleFileHandler(), new ExampleFormatter());
    read_archive.Read(loaded_objects, reflector);

    // loaded_objects should contain the same objects as in objects
    std::cout << "objects content:" << std::endl;
    PrintISerFwObjsCollection_RawPtr(objects);
    std::cout << "loaded_objects content:" << std::endl;
    PrintISerFwObjsCollection_RawPtr(loaded_objects);

    return AreEqualISerFwObjsCollection(objects, loaded_objects);
}


void PrintISerFwObjsCollection_SharedPtr(std::vector<std::shared_ptr<infra::ISerFwObj>> a_container)
{
    // Hard-coded printing test:
    std::for_each(a_container.begin(), a_container.end(),
    [](std::shared_ptr<infra::ISerFwObj> a_serFwObjPtr)
    {
        if(a_serFwObjPtr->TypeName() == "Circle")
        {
            std::static_pointer_cast<Circle>(a_serFwObjPtr)->Print();
        }

        if(a_serFwObjPtr->TypeName() == "Point")
        {
            std::static_pointer_cast<Point>(a_serFwObjPtr)->Print();
        }
    });
}


void PrintISerFwObjsCollection_RawPtr(std::vector<infra::ISerFwObj*> a_container)
{
    // Hard-coded printing test:
    std::for_each(a_container.begin(), a_container.end(),
    [](infra::ISerFwObj* a_serFwObjPtr)
    {
        if(a_serFwObjPtr->TypeName() == "Circle")
        {
            static_cast<Circle*>(a_serFwObjPtr)->Print();
        }

        if(a_serFwObjPtr->TypeName() == "Point")
        {
            static_cast<Point*>(a_serFwObjPtr)->Print();
        }
    });
}

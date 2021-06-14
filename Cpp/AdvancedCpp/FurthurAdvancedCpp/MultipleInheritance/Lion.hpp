#ifndef __LION_HPP__
#define __LION_HPP__


#include "Animal.hpp"
#include "Mammal.hpp"
#include "AnimalData.hpp"
#include "MammalData.hpp"


class Lion : public Mammal
{
public:
	Lion(const std::string& name, const std::string& species) : Mammal(name, species) {}

protected:
	virtual const MammalData& GetMammalData() const { return this->_lionData; }
	virtual void Print(std::ostream& a_os) const;

private:
	static MammalData _lionData;
};


#endif // __LION_HPP__
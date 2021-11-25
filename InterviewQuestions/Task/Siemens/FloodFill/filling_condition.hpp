#ifndef FILLING_CONDITION_HPP
#define FILLING_CONDITION_HPP


#include <memory> // std::shared_ptr
#include "drawing.h"


namespace nm
{
	class FillingConditionStrategy
	{
	public:
		virtual ~FillingConditionStrategy() = default;
		virtual bool Check(int x) const = 0;
		virtual int Offset() const = 0;
	};


	class FillingRightStrategy : public FillingConditionStrategy
	{
	public:
		virtual bool Check(int x) const override;
		virtual int Offset() const override;
	};


	class FillingLeftStrategy : public FillingConditionStrategy
	{
		virtual bool Check(int x) const override;
		virtual int Offset() const override;
	};


	class FillingCondition
	{
	public:
		FillingCondition() : m_fillingCondition(nullptr) {}
		FillingCondition(std::shared_ptr<FillingConditionStrategy> a_fillingCondition) : m_fillingCondition(a_fillingCondition) {}
		bool CheckCondition(int x, int y, Color colorToReplace);
		virtual int Offset() const;

	private:
		std::shared_ptr<FillingConditionStrategy> m_fillingCondition;
	};

} // nm


#endif // FILLING_CONDITION_HPP

#ifndef PENDING_COORDINATES_UPDATER_HPP
#define PENDING_COORDINATES_UPDATER_HPP


#include <stack>
#include <memory> // std::shared_ptr
#include "filling_indicator.hpp"


namespace nm
{

	class SideUpdatingStrategy
	{
	public:
		virtual ~SideUpdatingStrategy() = default;
		virtual bool IsValidSideToUpdate(int y) const = 0;
		virtual int TransformY(int y) const = 0;
	};


	class UpperSideUpdatingStrategy : public SideUpdatingStrategy
	{
		virtual bool IsValidSideToUpdate(int y) const override;
		virtual int TransformY(int y) const override;
	};


	class DownSideUpdatingStrategy : public SideUpdatingStrategy
	{
		virtual bool IsValidSideToUpdate(int y) const override;
		virtual int TransformY(int y) const override;
	};


	class SideUpdatingValidator
	{
	public:
		SideUpdatingValidator() : m_sidesUpdater(nullptr) {}
		SideUpdatingValidator(std::shared_ptr<SideUpdatingStrategy> a_sidesUpdater) : m_sidesUpdater(a_sidesUpdater) {}

		bool IsValidSideUpdating(int y) const;
		int UpdateY(int y) const;

	private:
		std::shared_ptr<SideUpdatingStrategy> m_sidesUpdater;
	};


	class DriftingCheckStrategy
	{
	public:
		virtual ~DriftingCheckStrategy() = default;
		virtual bool IsValidDrifting(int driftingX, int endingX) const = 0;
		virtual int Offset() const = 0;
	};


	class DriftingRightCheckStrategy : public DriftingCheckStrategy
	{
	public:
		virtual bool IsValidDrifting(int driftingX, int endingX) const override;
		virtual int Offset() const override;
	};


	class DriftingLeftCheckStrategy : public DriftingCheckStrategy
	{
		virtual bool IsValidDrifting(int driftingX, int endingX) const override;
		virtual int Offset() const override;
	};


	class PendingCoordinatesUpdater
	{
	public:
		PendingCoordinatesUpdater() : m_driftingChecker(nullptr) {}
		PendingCoordinatesUpdater(std::shared_ptr<DriftingCheckStrategy> a_driftingChecker)
		: m_driftingChecker(a_driftingChecker)
		{
		}

		bool HasAnotherCoordinatesToCheck(int driftingX, int endingX) const;
		int Offset() const;

	private:
		std::shared_ptr<DriftingCheckStrategy> m_driftingChecker;
	};

} // nm


#endif // PENDING_COORDINATES_UPDATER_HPP

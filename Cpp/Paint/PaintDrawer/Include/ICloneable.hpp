#ifndef __PAINTDRAWER_ICLONEABLE_HPP__
#define __PAINTDRAWER_ICLONEABLE_HPP__


namespace PaintDrawer {

// An interface to be used as a factory to create a derived class object of a given object (on the heap)
// (Gives the responsability of the copy creation of each class' instance - to itself)
class ICloneable {
public:
    virtual ~ICloneable() = default;
    virtual ICloneable* Clone() const = 0; // After using this method - the returned value should be casted to the Derived Class' type (using static_cast)
};

} // PaintDrawer


#endif // __PAINTDRAWER_ICLONEABLE_HPP__
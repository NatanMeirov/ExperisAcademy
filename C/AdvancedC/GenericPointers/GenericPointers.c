
/* Generic Pointers Swap */
void PointersSwap(void** _ptr1, void** _ptr2)
{
	void* temp = *_ptr1;
	*_ptr1 = *_ptr2;
	*_ptr2 = temp;
}


int GenericBubbleSort(void* _arrayToSort, size_t _sizeOfArray, size_t _eachElementSizeInBytes, int(*SortCriteria)(const void* _firstElm, const void* _secondElm))
{
	/* Use memcpy, and copy the bytes using a temp var in _eachElementSizeInBytes */

}

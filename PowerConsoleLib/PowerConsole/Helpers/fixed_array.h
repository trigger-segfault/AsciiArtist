/*=================================================================|
|* File:				fixed_array.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef FIXED_ARRAY_H
#define FIXED_ARRAY_H

#include <memory>
#include <vector>
#include <PowerConsole/Declarations.h>

namespace PowerConsole::Helpers {
//=================================================================|
// CLASSES														   |
//=================================================================/

/* A fixed-length array where the size can be specified at runtime. */
template<typename T> class fixed_array {

	//=========== MEMBERS ============
	#pragma region Members

	/* The data of the array. */
	std::unique_ptr<T[]> _data;
	/* The size of the array. */
	size_t _size;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs an empty fixed_array of size zero. */
	fixed_array() {
		_data = std::make_unique<T[]>(0);
		_size = 1;
	}
	/* Constructs a fixed_array of the specified size. */
	fixed_array(size_t size) {
		_data = std::make_unique<T[]>(size);
		_size = size;
	}
	/* Constructs a fixed_array of the specified size and with the specified default value. */
	fixed_array(size_t size, const T& defaultValue) {
		_data = std::make_unique<T[]>(size);
		_size = size;
		for (size_t i = 0; i < size; i++)
			_data[i] = defaultValue;
	}
	/* Constructs a copy of the specified fixed array. */
	fixed_array(const fixed_array<T>& fixedArray) {
		_size = fixedArray._size;
		_data = std::make_unique<T[]>(_size);
		for (size_t i = 0; i < _size; i++) {
			_data[i] = fixedArray._data[i];
		}
	}
	/* Constructs a fixed array moved from another fixed array. */
	fixed_array(fixed_array<T>&& fixedArray) {
		_size = fixedArray._size;
		_data = std::move(fixedArray._data);
		fixedArray._data = nullptr;
	}
	/* Constructs a copy of the specified vector as a fixed array. */
	fixed_array(const std::vector<T>& vectorArray) {
		_size = vectorArray.size();
		_data = std::make_unique<T[]>(_size);
		for (size_t i = 0; i < _size; i++) {
			_data[i] = vectorArray[i];
		}
	}
	/* Constructs an initializer list of a fixed array. */
	fixed_array(std::initializer_list<T> initList) {
		_data = std::make_unique<T[]>(initList.size());
		_size = initList.size();
		size_t index = 0;
		for (const T& t : initList) {
			_data[index] = t;
			index++;
		}
	}

	#pragma endregion
	//========== OPERATORS ===========
	#pragma region Operators

	// Assignment
	/* Assigns a copy of the fixed array to this fixed array. */
	fixed_array<T>& operator=(const fixed_array<T>& fixedArray) {
		_size = fixedArray._size;
		_data = std::make_unique<T[]>(_size);
		for (size_t i = 0; i < _size; i++) {
			_data[i] = fixedArray._data[i];
		}
		return *this;
	}
	/* Moves the fixed array to this fixed array. */
	fixed_array<T>& operator=(fixed_array<T>&& fixedArray) {
		_size = fixedArray._size;
		_data = std::move(fixedArray._data);
		fixedArray._data = nullptr;
		return *this;
	}

	// Index
	/* Gets the element at the specified index in the fixed array. */
	T& operator[](size_t index) {
		if (index >= _size)
			throw std::out_of_range("Index out of range in fixed_array::operator[]");
		return _data[index];
	}
	/* Gets the element at the specified index in the fixed array. */
	const T& operator[](size_t index) const {
		if (index >= _size)
			throw std::out_of_range("Index out of range in fixed_array::operator[]");
		return _data[index];
	}

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	/* Gets the size of the fixed array. */
	size_t size() const {
		return _size;
	}
	/* Returns true if the size of the fixed array is zero. */
	bool empty() const {
		return _size == 0;
	}
	/* Gets a pointer to the data of the fixed array. */
	T* data() {
		return _data.get();
	}
	/* Gets a pointer to the data of the fixed array. */
	const T* data() const {
		return _data.get();
	}

	#pragma endregion
	//=========== ELEMENTS ===========
	#pragma region Elements

	/* Gets the element at the specified index in the fixed array. */
	T& at(size_t index) {
		if (index >= _size)
			throw std::out_of_range("Index out of range in fixed_array::at(...)");
		return _data[index];
	}
	/* Gets the element at the specified index in the fixed array. */
	const T& at(size_t index) const {
		if (index >= _size)
			throw std::out_of_range("Index out of range in fixed_array::at(...)");
		return _data[index];
	}
	/* Gets the first element in the fixed array. */
	T& front() {
		return _data[0];
	}
	/* Gets the first element in the fixed array. */
	const T& front() const {
		return _data[0];
	}
	/* Gets the last element in the fixed array. */
	T& back() {
		return _data[_size - 1];
	}
	/* Gets the last element in the fixed array. */
	const T& back() const {
		return _data[_size - 1];
	}

	#pragma endregion
	//========== ITERATORS ===========
	#pragma region Iterators

	//typedef typename vector<T>::iterator iterator;
	//typedef typename vector<T>::const_iterator const_iterator;

	/* Returns the beginning iterator of the container. */
	//iterator begin();
	/* Returns the constant beginning iterator of the container. */
	//const_iterator begin() const;
	/* Returns the ending iterator of the container. */
	//iterator end();
	/* Returns the constant ending iterator of the container. */
	//const_iterator end() const;

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|

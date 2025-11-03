#pragma once
#include <iostream>

template <typename T>
class MiArray {
	T* arr;
	int size;
	int capacity;
public:
	MiArray():capacity(4), size(0) {
		arr = new T[capacity];
	}
	MiArray(const MiArray<T>& otro) {
		this->size = otro.size;
		this->capacity = otro.capacity;
		this->arr = new T[otro.capacity];
		for (int i = 0;i < otro.size;i++) {
			this->arr[i] = otro.arr[i];
		}
	}
	MiArray& operator=(const MiArray& otro) {
		if (this == &otro) {
			return *this;
		}
		T* tmp = new T[otro.capacity];
		for (int i = 0; i < otro.size; i++){
			tmp[i] = otro.arr[i];
		}
		delete[] this->arr;
		this->arr = tmp;
		this->size = otro.size;
		this->capacity = otro.capacity;
		return *this;
	}
	~MiArray() {
		delete[] arr;
	}
	void push_back(T new_entry) {
		if (size == capacity) {
			T* tmp = new T[capacity * 2];
			for (int i = 0;i < size;i++) {
				tmp[i] = arr[i];
			}
			delete[] arr;
			arr = tmp;
			capacity *= 2;
		}
		arr[size] = new_entry;
		size++;
	}
	T* last_elem() {
		if (size == 0) {
			return nullptr;
		}
		return &arr[size - 1];
	}
	T& operator[](int index) {
		if (index >= size) {
			throw std::out_of_range("Mi array fuera de rango");
		}
		return arr[index];
	}
	int get_size() {
		return this->size;
	}
	int get_capacity() {
		return this->capacity;
	}
};


#pragma once

#include <stdexcept>
#include <string>

template<typename T>
class matrix2d
{
public:
	matrix2d(size_t rows, size_t columns);
	matrix2d(const matrix2d &src);
	matrix2d(matrix2d &&src);
	~matrix2d();

	matrix2d &operator+=(const matrix2d &rhs);
	matrix2d &operator-=(const matrix2d &rhs);

	matrix2d &operator=(const matrix2d &rhs);
	matrix2d &operator=(matrix2d &&rhs);

	matrix2d operator+(const matrix2d &rhs) const;
	matrix2d operator-(const matrix2d &rhs) const;
	matrix2d operator*(const matrix2d &rhs) const;

	bool is_invalid() const;
	bool is_square() const;

	size_t get_rows() const;
	size_t get_columns() const;

	void zero();
	void fill(T value);

	/*
		Row references are useful for C++ indexing operator (i.e. matrix[row][col]) and matrix multiplication
	*/

	class row_reference {
	public:
		row_reference(T *elements, size_t columns)
			:m_elements(elements),
			m_columns(columns)
		{ }

		T &operator[](size_t index) {
			if(index >= m_columns) throw std::out_of_range("param 'index' out-of-bounds");
			return m_elements[index];
		}
		const T &operator[](size_t index) const {
			if(index >= m_columns) throw std::out_of_range("param 'index' out-of-bounds");
			return m_elements[index];
		}
	private:
		T *m_elements;
		size_t m_columns;
	};

	row_reference row_vector(size_t row)
	{
		if(row >= m_rows) throw std::out_of_range("param 'row' out-of-bounds");
		return row_reference(&m_elements[row * m_columns], m_columns);
	}

	row_reference operator[](size_t row)
	{
		return row_vector(row);
	}

	class const_row_reference {
	public:
		const_row_reference(const T *elements, size_t columns)
			:m_elements(elements),
			m_columns(columns)
		{ }

		const T &operator[](size_t index) const {
			if(index >= m_columns) throw std::out_of_range("param 'index' out-of-bounds");
			return m_elements[index];
		}
	private:
		const T *m_elements;
		size_t m_columns;
	};

	const const_row_reference row_vector(size_t row) const
	{
		if(row >= m_rows) throw std::out_of_range("param 'row' out-of-bounds");
		return const_row_reference(&m_elements[row * m_columns], m_columns);
	}

	const const_row_reference operator[](size_t row) const
	{
		return row_vector(row);
	}

	/*
		Column references are useful for matrix multiplication
	*/

	class column_reference {
	public:
		column_reference(T *elements, size_t rows, size_t columns)
			:m_elements(elements),
			m_rows(rows),
			m_columns(columns)
		{ }

		T &operator[](size_t index) {
			if(index >= m_rows) throw std::out_of_range("param 'index' out-of-bounds");
			return m_elements[index * m_columns];
		}
		const T &operator[](size_t index) const {
			if(index >= m_rows) throw std::out_of_range("param 'index' out-of-bounds");
			return m_elements[index * m_columns];
		}
	private:
		T *m_elements;
		size_t m_rows;
		size_t m_columns;
	};

	column_reference column_vector(size_t column)
	{
		if(column >= m_columns) throw std::out_of_range("param 'column' out-of-bounds");
		return column_reference(&m_elements[column], m_rows, m_columns);
	}

	class const_column_reference {
	public:
		const_column_reference(const T *elements, size_t rows, size_t columns)
			:m_elements(elements),
			m_rows(rows),
			m_columns(columns)
		{ }

		const T &operator[](size_t index) const {
			if(index >= m_rows) throw std::out_of_range("param 'index' out-of-bounds");
			return m_elements[index * m_columns];
		}
	private:
		const T *m_elements;
		size_t m_rows;
		size_t m_columns;
	};

	const const_column_reference column_vector(size_t column) const
	{
		if(column >= m_columns) throw std::out_of_range("param 'column' out-of-bounds");
		return const_column_reference(&m_elements[column], m_rows, m_columns);
	}

private:
	size_t m_rows;
	size_t m_columns;
	T *m_elements;
};

#include "matrix.inl"
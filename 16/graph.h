#pragma once

#include "matrix.h"

#include <iostream>

class weighted_graph
{
public:
	weighted_graph(int size);

	void add_edge(int n0, int n1, int weight);
	void calculate_shortest_path_lengths();

	int get_edge(int n0, int n1) const;
	int get_shortest_path_length(int n0, int n1);
	int get_shortest_path_length(int n0, int n1) const;
	int get_size() const;

private:
	matrix2d<int> m_edge_weights;
	matrix2d<int> m_shortest_path_lengths;
	bool m_path_lengths_are_dirty;
};

std::ostream &operator<<(std::ostream &os, const weighted_graph &g);
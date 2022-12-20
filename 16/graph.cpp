#include "graph.h"

#include <climits>
#include <iomanip>

weighted_graph::weighted_graph(int size)
	:m_edge_weights(size, size),
	m_shortest_path_lengths(size, size),
	m_path_lengths_are_dirty(true)
{ 
	m_edge_weights.fill(INT_MAX);
	m_shortest_path_lengths.fill(INT_MAX);
	for(int i=0; i<size; ++i) {
		m_edge_weights[i][i] = 0;
		m_shortest_path_lengths[i][i] = 0;
	}
}

void weighted_graph::add_edge(int n0, int n1, int weight)
{
	if(n0 >= m_edge_weights.get_rows()) throw std::out_of_range("param 'n0' out-of-bounds");
	if(n1 >= m_edge_weights.get_rows()) throw std::out_of_range("param 'n1' out-of-bounds");

	m_edge_weights[n0][n1] = weight;
	m_edge_weights[n1][n0] = weight;

	m_path_lengths_are_dirty = true;
}

void weighted_graph::calculate_shortest_path_lengths()
{
	const size_t size = m_edge_weights.get_rows();

	// Taken from "Introduction to Algorithms, 2nd ed.", ISBN 0-07-013151-1
	auto extend_shortest_paths = [size](matrix2d<int> &l, matrix2d<int> &w) -> matrix2d<int> {		
		matrix2d<int> ll(size, size);
		ll.fill(INT_MAX);
		for(int i=0; i<size; ++i) {
			ll[i][i] = 0;
		}

		for(size_t i=0; i<size; ++i) {
			for(size_t j=0; j<size; ++j) {
				for(size_t k=0; k<size; ++k) {
					if(l[i][k] < INT_MAX && w[k][j] < INT_MAX) {
						ll[i][j] = std::min(ll[i][j], l[i][k] + w[k][j]);
					}
				}
			}
		}

		return ll;
	};

	m_shortest_path_lengths = m_edge_weights;
	for(int i=1; i<size; i = i<<1) {
		m_shortest_path_lengths = extend_shortest_paths(m_shortest_path_lengths, m_shortest_path_lengths);
	}

	m_path_lengths_are_dirty = false;
}

int weighted_graph::get_edge(int n0, int n1) const
{
	if(n0 >= m_edge_weights.get_rows()) throw std::out_of_range("param 'n0' out-of-bounds");
	if(n1 >= m_edge_weights.get_rows()) throw std::out_of_range("param 'n1' out-of-bounds");

	return m_edge_weights[n0][n1];
}

int weighted_graph::get_shortest_path_length(int n0, int n1)
{
	if(n0 >= m_edge_weights.get_rows()) throw std::out_of_range("param 'n0' out-of-bounds");
	if(n1 >= m_edge_weights.get_rows()) throw std::out_of_range("param 'n1' out-of-bounds");

	if(m_path_lengths_are_dirty) {
		calculate_shortest_path_lengths();
	}

	return m_shortest_path_lengths[n0][n1];
}

int weighted_graph::get_shortest_path_length(int n0, int n1) const
{
	if(n0 >= m_edge_weights.get_rows()) throw std::out_of_range("param 'n0' out-of-bounds");
	if(n1 >= m_edge_weights.get_rows()) throw std::out_of_range("param 'n1' out-of-bounds");

	return m_shortest_path_lengths[n0][n1];
}

int weighted_graph::get_size() const
{
	return m_edge_weights.get_rows();
}


std::ostream &operator<<(std::ostream &os, const weighted_graph &g)
{
	const int matrix_width = 3 + 4*g.get_size();

	os << std::setw(matrix_width / 2 + 1) << "W" << std::setw(matrix_width) << " " << "S" << " " << std::endl;
	for(int i=0; i<g.get_size(); ++i) {
		os << std::setw(0) << "[ ";
		for(int j=0; j<g.get_size(); ++j) {
			int edge = g.get_edge(i, j);
			if(edge == INT_MAX) {
				os << std::setw(0) << "inf ";
			} else {
				os << std::setw(3) << edge << std::setw(0) << " ";
			}
		}
		os << std::setw(0) << "] [ ";
		for(int j=0; j<g.get_size(); ++j) {
			int path = g.get_shortest_path_length(i, j);
			if(path == INT_MAX) {
				os << std::setw(0) << "inf ";
			} else {
				os << std::setw(3) << path << std::setw(0) << " ";
			}
		}
		os << std::setw(0) << "]" << std::endl;
	}
	//            W                       S
	// [ XXX XXX XXX XXX XXX ] [ XXX XXX XXX XXX XXX ]
	// [ XXX XXX XXX XXX XXX ] [ XXX XXX XXX XXX XXX ]
	// [ XXX XXX XXX XXX XXX ] [ XXX XXX XXX XXX XXX ]
	// [ XXX XXX XXX XXX XXX ] [ XXX XXX XXX XXX XXX ]
	// [ XXX XXX XXX XXX XXX ] [ XXX XXX XXX XXX XXX ]
	return os;
}
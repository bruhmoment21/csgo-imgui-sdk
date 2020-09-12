#pragma once
//#include "constants.hpp"								Uncomment when needed
#include "vec3_t.hpp"

struct matrix_3x4 {
public:
	float m[ 3 ][ 4 ];

	/*
	 *	in-depth:
	 *		first array meaning:
	 *			m[0] = x value
	 *			m[1] = y value
	 *			m[2] = z value
	 *		second array meaning:
	 *			m[x][0] = x axis vector
	 *			m[x][1] = y axis vector
	 *			m[x][2] = z axis vector
	 *			m[x][3] = origin vector
	 */
	
	explicit matrix_3x4( const vec3_t& x_axis, const vec3_t& y_axis, const vec3_t& z_axis, const vec3_t& origin_vector ) {

		m[ 0 ][ 0 ] = x_axis.x; m[ 0 ][ 1 ] = y_axis.x; m[ 0 ][ 2 ] = z_axis.x; m[ 0 ][ 3 ] = origin_vector.x;
		m[ 1 ][ 0 ] = x_axis.y; m[ 1 ][ 1 ] = y_axis.y; m[ 1 ][ 2 ] = z_axis.y; m[ 1 ][ 3 ] = origin_vector.y;
		m[ 2 ][ 0 ] = x_axis.z; m[ 2 ][ 1 ] = y_axis.z; m[ 2 ][ 2 ] = z_axis.z; m[ 2 ][ 3 ] = origin_vector.z;
	}
};

struct matrix_4x4 { // aka view_matrix
public:
	float m[ 4 ][ 4 ];

	/*
	 *	in-depth:
	 *		first array meaning:
	 *			m[0] = x value
	 *			m[1] = y value
	 *			m[2] = z value
	 *			m[3] = origin value
	 *		second array meaning:
	 *			m[x][0] = x axis vector
	 *			m[x][1] = y axis vector
	 *			m[x][2] = z axis vector
	 *			m[x][3] = origin vector
	 */
	
	explicit matrix_4x4( const matrix_3x4& matrix ) {
		
		m[ 0 ][ 0 ] = matrix.m[ 0 ][ 0 ]; m[ 0 ][ 1 ] = matrix.m[ 0 ][ 1 ]; m[ 0 ][ 2 ] = matrix.m[ 0 ][ 2 ]; m[ 0 ][ 3 ] = matrix.m[ 0 ][ 3 ];
		m[ 1 ][ 0 ] = matrix.m[ 1 ][ 0 ]; m[ 1 ][ 1 ] = matrix.m[ 1 ][ 1 ]; m[ 1 ][ 2 ] = matrix.m[ 1 ][ 2 ]; m[ 1 ][ 3 ] = matrix.m[ 1 ][ 3 ];
		m[ 2 ][ 0 ] = matrix.m[ 2 ][ 0 ]; m[ 2 ][ 1 ] = matrix.m[ 2 ][ 1 ]; m[ 2 ][ 2 ] = matrix.m[ 2 ][ 2 ]; m[ 2 ][ 3 ] = matrix.m[ 2 ][ 3 ];

		m[ 3 ][ 0 ] = 0.f; m[ 3 ][ 1 ] = 0.f; m[ 3 ][ 2 ] = 0.f; m[ 3 ][ 3 ] = 1.f;
	}
};
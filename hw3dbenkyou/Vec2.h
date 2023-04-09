/******************************************************************************************
*	Chili DirectX Framework Version 16.10.01											  *
*	Vec2.h																				  *
*	Copyright 2016 PlanetChili <http://www.planetchili.net>								  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#pragma once
#include <cmath>

template <typename T>
class vec2
{
public:
	vec2()
	{}
	vec2( T x,T y )
		:
		x( x ),
		y( y )
	{}
	vec2( const vec2& vect )
		:
		vec2( vect.x,vect.y )
	{}
	template <typename T2>
	explicit operator vec2<T2>() const
	{
		return{ (T2)x,(T2)y };
	}
	T		LenSq() const
	{
		return x * x + y * y;
	}
	T		Len() const
	{
		return sqrt(LenSq());
	}
	vec2&	Normalize()
	{
		const T length = Len();
		x /= length;
		y /= length;
		return *this;
	}
	vec2	GetNormalized() const
	{
		vec2 norm = *this;
		norm.Normalize();
		return norm;
	}
	vec2	operator-() const
	{
		return vec2( -x,-y );
	}
	vec2&	operator=( const vec2 &rhs )
	{
		x = rhs.x;
		y = rhs.y;
		return *this;
	}
	vec2&	operator+=( const vec2 &rhs )
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	vec2&	operator-=( const vec2 &rhs )
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
	vec2	operator+( const vec2 &rhs ) const
	{
		return vec2( *this ) += rhs;
	}
	vec2	operator-( const vec2 &rhs ) const
	{
		return vec2( *this ) -= rhs;
	}
	vec2&	operator*=( const T &rhs )
	{
		x *= rhs;
		y *= rhs;
		return *this;
	}
	vec2	operator*(const T& rhs) const
	{
		return vec2(*this) *= rhs;
	}
	vec2&	operator/=( const T &rhs )
	{
		x /= rhs;
		y /= rhs;
		return *this;
	}
	vec2	operator/( const T &rhs ) const
	{
		return vec2( *this ) /= rhs;
	}
	vec2 multiply_components(const vec2& rhs) const
	{
		return vec2(x * rhs.x, y * rhs.y);
	}
	bool	operator==( const vec2 &rhs ) const
	{
		return x == rhs.x && y == rhs.y;
	}
	bool	operator!=( const vec2 &rhs ) const
	{
		return !(*this == rhs);
	}
	T operator*(const vec2& other ) const
	{
		return x * other.x + y * other.y;
	}
public:
	T x;
	T y;
};
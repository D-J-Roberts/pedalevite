/*****************************************************************************

        FixedPoint.hpp
        Author: Laurent de Soras, 2016

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



#if ! defined (fstb_FixedPoint_CODEHEADER_INCLUDED)
#define fstb_FixedPoint_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "fstb/def.h"
#include "fstb/fnc.h"

#include <cassert>



namespace fstb
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



FixedPoint::FixedPoint ()
{
	_val._all = 0;
}



FixedPoint::FixedPoint (int32_t int_val)
{
	_val._part._msw = int_val;
	_val._part._lsw = 0;
}



FixedPoint::FixedPoint (int32_t int_val, uint32_t frac_val)
{
	_val._part._msw = int_val;
	_val._part._lsw = frac_val;
}



FixedPoint::FixedPoint (double val)
{
	set_val (val);
}



FixedPoint::FixedPoint (NoInit dummy)
:	_val ()
{
	// Nothing
	fstb::unused (dummy);
}



constexpr void	FixedPoint::set_val_int64 (int64_t val)
{
	_val._all = val;
}



constexpr void	FixedPoint::clear ()
{
	_val._all = 0;
}



void	FixedPoint::set_val (double val)
{
	const double	uint_scale = fstb::TWOP32;

	_val._part._msw = fstb::floor_int (val);
	_val._part._lsw = uint32_t ((val - _val._part._msw) * uint_scale);
}



constexpr void	FixedPoint::set_val (int32_t int_val, uint32_t frac_val)
{
	_val._part._msw = int_val;
	_val._part._lsw = frac_val;
}



constexpr int64_t	FixedPoint::get_val_int64 () const
{
	return _val._all;
}



constexpr double	FixedPoint::get_val_dbl () const
{
	return _val._part._msw + get_frac_val_dbl ();
}



constexpr float	FixedPoint::get_val_flt () const
{
	return _val._part._msw + get_frac_val_flt ();
}



constexpr void	FixedPoint::set_int_val (int32_t int_val)
{
	_val._part._msw = int_val;
}



constexpr int32_t	FixedPoint::get_int_val () const
{
	return _val._part._msw;
}



constexpr int32_t	FixedPoint::get_round () const
{
	Fixed3232      temp (_val);
	temp._all += 0x80000000U;

	return temp._part._msw;
}



constexpr int32_t	FixedPoint::get_ceil () const
{
	Fixed3232      temp (_val);
	temp._all += 0xFFFFFFFFU;

	return temp._part._msw;
}



constexpr void	FixedPoint::set_frac_val (uint32_t frac_val)
{
	_val._part._lsw = frac_val;
}



constexpr double	FixedPoint::get_frac_val_dbl () const
{
	return _val._part._lsw * double (fstb::TWOPM32);
}



constexpr float	FixedPoint::get_frac_val_flt () const
{
	return float (_val._part._lsw * fstb::TWOPM32);
}



constexpr uint32_t	FixedPoint::get_frac_val () const
{
	return _val._part._lsw;
}



constexpr void	FixedPoint::neg ()
{
	_val._all = -_val._all;
}



constexpr void	FixedPoint::abs ()
{
	if (_val._part._msw < 0)
	{
		neg ();
	}
}



// Retruns positive or negative values, depending on the initial value sign
constexpr void	FixedPoint::bound (int32_t len)
{
	assert (len > 0);

	_val._part._msw %= len;

	assert (_val._part._msw < len);
	assert (_val._part._msw > -len);
}



// Returns only positive values
constexpr void	FixedPoint::bound_positive (int32_t len)
{
	assert (len > 0);

	bound (len);
	if (_val._part._msw < 0)
	{
		_val._part._msw += len;
	}

	assert (_val._part._msw >= 0);
	assert (_val._part._msw < len);
}



constexpr void	FixedPoint::bound_and (int32_t and_val)
{
	_val._part._msw &= and_val;
}



constexpr void	FixedPoint::add (const FixedPoint &val)
{
	_val._all += val._val._all;
}



constexpr void	FixedPoint::add (const FixedPoint &val, int32_t and_val)
{
	add (val);
	bound_and (and_val);
}



constexpr void	FixedPoint::add_int (int32_t int_val)
{
	_val._part._msw += int_val;
}



constexpr void	FixedPoint::add_int (int32_t int_val, int32_t and_val)
{
	add_int (int_val);
	bound_and (and_val);
}



constexpr void	FixedPoint::add_frac (uint32_t frac_val)
{
	_val._all += frac_val;
}



constexpr void	FixedPoint::add_frac (uint32_t frac_val, int32_t and_val)
{
	add_frac (frac_val);
	bound_and (and_val);
}



constexpr FixedPoint &	FixedPoint::operator += (const FixedPoint &other)
{
	add (other);

	return *this;
}



constexpr FixedPoint &	FixedPoint::operator += (int32_t int_val)
{
	add_int (int_val);

	return *this;
}



constexpr void	FixedPoint::sub (const FixedPoint &val)
{
	_val._all -= val._val._all;
}



constexpr void	FixedPoint::sub (const FixedPoint &val, int32_t and_val)
{
	sub (val);
	bound_and (and_val);
}



constexpr void	FixedPoint::sub_int (int32_t int_val)
{
	_val._part._msw -= int_val;
}



constexpr void	FixedPoint::sub_int (int32_t int_val, int32_t and_val)
{
	sub_int (int_val);
	bound_and (and_val);
}



constexpr void	FixedPoint::sub_frac (uint32_t frac_val)
{
	_val._all -= frac_val;
}



constexpr void	FixedPoint::sub_frac (uint32_t frac_val, int32_t and_val)
{
	sub_frac (frac_val);
	bound_and (and_val);
}



constexpr FixedPoint &	FixedPoint::operator -= (const FixedPoint &other)
{
	sub (other);

	return *this;
}



constexpr FixedPoint &	FixedPoint::operator -= (int32_t int_val)
{
	sub_int (int_val);

	return *this;
}



constexpr void	FixedPoint::shift (int nbr_bits)
{
	if (nbr_bits > 0)
	{
		shift_left (nbr_bits);
	}
	else if (nbr_bits < 0)
	{
		shift_right (-nbr_bits);
	}
}



constexpr void	FixedPoint::shift_left (int nbr_bits)
{
	assert (nbr_bits >= 0);

	_val._all <<= nbr_bits;
}



constexpr void	FixedPoint::shift_right (int nbr_bits)
{
	assert (nbr_bits >= 0);

	_val._all >>= nbr_bits;

	assert (   (nbr_bits >= 64)
	        ||    ((_val._all << nbr_bits) & INT64_MIN)
	           == ( _val._all              & INT64_MIN));
}



constexpr FixedPoint &	FixedPoint::operator <<= (int nbr_bits)
{
	shift_left (nbr_bits);

	return *this;
}



constexpr FixedPoint &	FixedPoint::operator >>= (int nbr_bits)
{
	shift_right (nbr_bits);

	return *this;
}



constexpr void	FixedPoint::mul_int (int32_t val)
{
	_val._all *= val;
}



void	FixedPoint::mul_flt (double val)
{
	const double		val_flt = get_val_dbl ();
	set_val (val_flt * val);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



bool	operator == (const FixedPoint &left, const FixedPoint &right)
{
	return left._val._all == right._val._all;
}



bool	operator != (const FixedPoint &left, const FixedPoint &right)
{
	return ! (left == right);
}



bool	operator < (const FixedPoint &left, const FixedPoint &right)
{
	return left._val._all < right._val._all;
}



bool	operator <= (const FixedPoint &left, const FixedPoint &right)
{
	return ! (left > right);
}



bool	operator > (const FixedPoint &left, const FixedPoint &right)
{
	return right < left;
}



bool	operator >= (const FixedPoint &left, const FixedPoint &right)
{
	return ! (left < right);
}



FixedPoint	operator + (FixedPoint left, const FixedPoint &right)
{
	left += right;

	return left;
}



FixedPoint	operator - (FixedPoint left, const FixedPoint &right)
{
	left -= right;

	return left;
}



}  // namespace fstb



#endif   // fstb_FixedPoint_CODEHEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

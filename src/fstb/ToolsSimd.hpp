/*****************************************************************************

        ToolsSimd.hpp
        Author: Laurent de Soras, 2016

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



#if ! defined (fstb_ToolsSimd_CODEHEADER_INCLUDED)
#define fstb_ToolsSimd_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "fstb/fnc.h"



namespace fstb
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



template <typename MEM>
ToolsSimd::VectF32	ToolsSimd::load_f32 (const MEM *ptr)
{
	assert (is_ptr_align_nz (ptr, 16));

#if fstb_IS (ARCHI, X86)
	return _mm_load_ps (reinterpret_cast <const float *> (ptr));
#elif fstb_IS (ARCHI, ARM)
	return vld1q_f32 (reinterpret_cast <const float32_t *> (ptr));
#endif // ff_arch_CPU
}



template <typename MEM>
ToolsSimd::VectS32	ToolsSimd::load_s32 (const MEM *ptr)
{
	assert (is_ptr_align_nz (ptr, 16));

#if fstb_IS (ARCHI, X86)
	return _mm_load_si128 (reinterpret_cast <const __m128i *> (ptr));
#elif fstb_IS (ARCHI, ARM)
	return vld1q_s32 (reinterpret_cast <const int32_t *> (ptr));
#endif // ff_arch_CPU
}



template <typename MEM>
void	ToolsSimd::store_f32 (MEM *ptr, VectF32 v)
{
	assert (is_ptr_align_nz (ptr, 16));

#if fstb_IS (ARCHI, X86)
	_mm_store_ps (reinterpret_cast <float *> (ptr), v);
#elif fstb_IS (ARCHI, ARM)
	vst1q_f32 (reinterpret_cast <float32_t *> (ptr), v);
#endif // ff_arch_CPU
}



template <typename MEM>
void	ToolsSimd::store_s32 (MEM *ptr, VectS32 v)
{
	assert (is_ptr_align_nz (ptr, 16));

#if fstb_IS (ARCHI, X86)
	_mm_store_si128 (reinterpret_cast <__m128i *> (ptr), v);
#elif fstb_IS (ARCHI, ARM)
	vst1q_s32 (reinterpret_cast <int32_t *> (ptr), v);
#endif // ff_arch_CPU
}



// n = number of scalars to store (from the LSB)
template <typename MEM>
void	ToolsSimd::store_f32_part (MEM *ptr, VectF32 v, int n)
{
	assert (n > 0);

	if (n >= 4)
	{
		store_f32 (ptr, v);
	}
	else
	{
		store_f32_part_n13 (ptr, v, n);
	}
}



template <typename MEM>
ToolsSimd::VectF32	ToolsSimd::loadu_f32 (const MEM *ptr)
{
	assert (ptr != 0);

#if fstb_IS (ARCHI, X86)
	return _mm_loadu_ps (reinterpret_cast <const float *> (ptr));
#elif fstb_IS (ARCHI, ARM)
	return vreinterpretq_f32_u8 (
		vld1q_u8 (reinterpret_cast <const uint8_t *> (ptr))
	);
#endif // ff_arch_CPU
}



template <typename MEM>
ToolsSimd::VectF32	ToolsSimd::loadu_f32_part (const MEM *ptr, int n)
{
	assert (n > 0);

	if (n >= 4)
	{
		return loadu_f32 (ptr);
	}

	return load_f32_part_n13 (ptr, n);
}



template <typename MEM>
void	ToolsSimd::storeu_f32 (MEM *ptr, VectF32 v)
{
	assert (ptr != 0);

#if fstb_IS (ARCHI, X86)
	_mm_storeu_ps (reinterpret_cast <float *> (ptr), v);
#elif fstb_IS (ARCHI, ARM)
	vst1q_u8 (reinterpret_cast <uint8_t *> (ptr), vreinterpretq_u8_f32 (v));
#endif // ff_arch_CPU
}



template <typename MEM>
void	ToolsSimd::storeu_s32 (MEM *ptr, VectS32 v)
{
	assert (ptr != 0);

#if fstb_IS (ARCHI, X86)
	_mm_storeu_si128 (reinterpret_cast <__m128i *> (ptr), v);
#elif fstb_IS (ARCHI, ARM)
	vst1q_u8 (reinterpret_cast <uint8_t *> (ptr), vreinterpretq_u8_s32 (v));
#endif // ff_arch_CPU
}



// n = number of scalars to store (from the LSB)
template <typename MEM>
void	ToolsSimd::storeu_f32_part (MEM *ptr, VectF32 v, int n)
{
	assert (n > 0);

	if (n >= 4)
	{
		storeu_f32 (ptr, v);
	}
	else
	{
		store_f32_part_n13 (ptr, v, n);
	}
}



// n = number of scalars to store (from the LSB)
template <typename MEM>
void	ToolsSimd::storeu_s32_part (MEM *ptr, VectS32 v, int n)
{
	assert (n > 0);

	if (n >= 4)
	{
		storeu_s32 (ptr, v);
	}
	else
	{
		store_s32_part_n13 (ptr, v, n);
	}
}



// Returns: ptr [0] | ptr [1] | ? | ?
template <typename MEM>
ToolsSimd::VectF32	ToolsSimd::loadu_2f32 (const MEM *ptr)
{
	assert (ptr != 0);

#if fstb_IS (ARCHI, X86)
	const auto     x_0 = _mm_load_ss (reinterpret_cast <const float *> (ptr)    );
	const auto     x_1 = _mm_load_ss (reinterpret_cast <const float *> (ptr) + 1);
	const auto     x   = _mm_unpacklo_ps (x_0, x_1);
	return x;
#elif fstb_IS (ARCHI, ARM)
	const float32x2_t x = vreinterpret_f32_u8 (
		vld1_u8 (reinterpret_cast <const uint8_t *> (ptr))
	);
	return vcombine_f32 (x, x);
#endif // ff_arch_CPU
}



// ptr [0] = v0
// ptr [1] = v1
template <typename MEM>
void	ToolsSimd::storeu_2f32 (MEM *ptr, VectF32 v)
{
	assert (ptr != 0);

#if fstb_IS (ARCHI, X86)
	_mm_store_ss (reinterpret_cast <float *> (ptr)    , v );
	const auto     v1 = _mm_shuffle_ps (v, v, 1 << 0);
	_mm_store_ss (reinterpret_cast <float *> (ptr) + 1, v1);
#elif fstb_IS (ARCHI, ARM)
	vst1_u8 (
		reinterpret_cast <uint8_t *> (ptr),
		vreinterpret_u8_f32 (vget_low_f32 (v))
	);
#endif // ff_arch_CPU
}



// *ptr = v0
template <typename MEM>
void	ToolsSimd::storeu_1f32 (MEM *ptr, VectF32 v)
{
	assert (ptr != 0);

#if fstb_IS (ARCHI, X86)
	_mm_store_ss (reinterpret_cast <float *> (ptr), v);
#elif fstb_IS (ARCHI, ARM)
	vst1q_lane_f32 (reinterpret_cast <float32_t *> (ptr), v, 0);
#endif // ff_arch_CPU
}



ToolsSimd::VectF32	ToolsSimd::set_f32_zero ()
{
#if fstb_IS (ARCHI, X86)
	return _mm_setzero_ps ();
#elif fstb_IS (ARCHI, ARM)
	return vdupq_n_f32 (0);
#endif // ff_arch_CPU
}



ToolsSimd::VectS32	ToolsSimd::set_s32_zero ()
{
#if fstb_IS (ARCHI, X86)
	return _mm_setzero_si128 ();
#elif fstb_IS (ARCHI, ARM)
	return vdupq_n_s32 (0);
#endif // ff_arch_CPU
}



// Returns a0 | a0 | a0 | a0
ToolsSimd::VectF32	ToolsSimd::set1_f32 (float a)
{
#if fstb_IS (ARCHI, X86)
	return _mm_set1_ps (a);
#elif fstb_IS (ARCHI, ARM)
	return vdupq_n_f32 (a);
#endif // ff_arch_CPU
}



// Returns a0 | a0 | a0 | a0
ToolsSimd::VectS32	ToolsSimd::set1_s32 (int32_t a)
{
#if fstb_IS (ARCHI, X86)
	return _mm_set1_epi32 (a);
#elif fstb_IS (ARCHI, ARM)
	return vdupq_n_s32 (a);
#endif // ff_arch_CPU
}



// Returns a0 | a1 | a2 | a3
ToolsSimd::VectF32	ToolsSimd::set_f32 (float a0, float a1, float a2, float a3)
{
#if fstb_IS (ARCHI, X86)
	return _mm_set_ps (a3, a2, a1, a0);
#elif fstb_IS (ARCHI, ARM)
 #if 1
	float32x2_t    v01 = vdup_n_f32 (a0);
	float32x2_t    v23 = vdup_n_f32 (a2);
	v01 = vset_lane_f32 (a1, v01, 1);
	v23 = vset_lane_f32 (a3, v23, 1);
 #else // Not tested
	const float32x2_t    v01 = vcreate_f32 (
		  (uint64_t (*reinterpret_cast <const uint32_t *> (&a0))      )
		| (uint64_t (*reinterpret_cast <const uint32_t *> (&a1)) << 32)
	);
	const float32x2_t    v23 = vcreate_f32 (
		  (uint64_t (*reinterpret_cast <const uint32_t *> (&a2))      )
		| (uint64_t (*reinterpret_cast <const uint32_t *> (&a3)) << 32)
	);
 #endif
	return vcombine_f32 (v01, v23);
#endif // ff_arch_CPU
}



// Returns a0 | a1 | ? | ?
ToolsSimd::VectF32	ToolsSimd::set_2f32 (float a0, float a1)
{
#if fstb_IS (ARCHI, X86)
	return _mm_unpacklo_ps (_mm_set_ss (a0), _mm_set_ss (a1));
#elif fstb_IS (ARCHI, ARM)
	return vsetq_lane_f32 (a1, vdupq_n_f32 (a0), 1);
#endif // ff_arch_CPU
}



// Returns a0 | a1 | a0 | a1
ToolsSimd::VectF32	ToolsSimd::set_2f32_fill (float a02, float a13)
{
#if fstb_IS (ARCHI, X86)
	return _mm_unpacklo_ps (_mm_set1_ps (a02), _mm_set1_ps (a13));
#elif fstb_IS (ARCHI, ARM)
	const float32x2_t v01 = vset_lane_f32 (a13, vdup_n_f32 (a02), 1);
	return vcombine_f32 (v01, v01);
#endif // ff_arch_CPU
}



// Returns a01 | a01 | a23 | a23
ToolsSimd::VectF32	ToolsSimd::set_2f32_dbl (float a01, float a23)
{
#if fstb_IS (ARCHI, X86)
	return _mm_shuffle_ps (_mm_set_ss (a01), _mm_set_ss (a23), 0x00);
#elif fstb_IS (ARCHI, ARM)
	return vcombine_f32 (vdup_n_f32 (a01), vdup_n_f32 (a23));
#endif // ff_arch_CPU
}



ToolsSimd::VectF32	ToolsSimd::swap_2f32 (VectF32 v)
{
#if fstb_IS (ARCHI, X86)
	return _mm_shuffle_ps (v, v, (2<<0) + (3<<2) + (0<<4) + (1<<6));
#elif fstb_IS (ARCHI, ARM)
	const float32x2_t v01 = vget_low_f32 (v);
	const float32x2_t v23 = vget_high_f32 (v);
	return vcombine_f32 (v23, v01);
#endif // ff_arch_CPU
}



void	ToolsSimd::extract_2f32 (float &a0, float &a1, VectF32 v)
{
#if fstb_IS (ARCHI, X86)
	a0 = _mm_cvtss_f32 (v);
	a1 = _mm_cvtss_f32 (_mm_shuffle_ps (v, v, 1));
#elif fstb_IS (ARCHI, ARM)
	a0 = vgetq_lane_f32 (v, 0);
	a1 = vgetq_lane_f32 (v, 1);
#endif // ff_arch_CPU
}



// ra = v0 | v1 | v0 | v1
// rb = v2 | v3 | v2 | v3
void	ToolsSimd::spread_2f32 (VectF32 &ra, VectF32 &rb, VectF32 v)
{
#if fstb_IS (ARCHI, X86)
	ra = _mm_shuffle_ps (v, v, (0<<0) + (1<<2) + (0<<4) + (1<<6));
	rb = _mm_shuffle_ps (v, v, (2<<0) + (3<<2) + (2<<4) + (3<<6));
#elif fstb_IS (ARCHI, ARM)
	const float32x2_t v01 = vget_low_f32 (v);
	const float32x2_t v23 = vget_high_f32 (v);
	ra = vcombine_f32 (v01, v01);
	rb = vcombine_f32 (v23, v23);
#endif // ff_arch_CPU
}



void	ToolsSimd::mac (VectF32 &s, VectF32 a, VectF32 b)
{
#if fstb_IS (ARCHI, X86)
	s = _mm_add_ps (s, _mm_mul_ps (a, b));
#elif fstb_IS (ARCHI, ARM)
	s = vmlaq_f32 (s, a, b);
#endif // ff_arch_CPU
}



void	ToolsSimd::msu (VectF32 &s, VectF32 a, VectF32 b)
{
#if fstb_IS (ARCHI, X86)
	s = _mm_sub_ps (s, _mm_mul_ps (a, b));
#elif fstb_IS (ARCHI, ARM)
	s = vmlsq_f32 (s, a, b);
#endif // ff_arch_CPU
}



ToolsSimd::VectF32	ToolsSimd::fmadd (VectF32 x, VectF32 a, VectF32 b)
{
#if fstb_IS (ARCHI, X86)
	return _mm_add_ps (_mm_mul_ps (x, a), b);
#elif fstb_IS (ARCHI, ARM)
	return vmlaq_f32 (b, x, a);
#endif // ff_arch_CPU
}



ToolsSimd::VectF32	ToolsSimd::fmsub (VectF32 x, VectF32 a, VectF32 b)
{
#if fstb_IS (ARCHI, X86)
	return _mm_sub_ps (_mm_mul_ps (x, a), b);
#elif fstb_IS (ARCHI, ARM)
	return vmlsq_f32 (b, x, a);
#endif // ff_arch_CPU
}



ToolsSimd::VectF32	ToolsSimd::min_f32 (VectF32 lhs, VectF32 rhs)
{
#if fstb_IS (ARCHI, X86)
	return _mm_min_ps (lhs, rhs);
#elif fstb_IS (ARCHI, ARM)
	return vminq_f32 (lhs, rhs);
#endif // ff_arch_CPU
}



ToolsSimd::VectF32	ToolsSimd::max_f32 (VectF32 lhs, VectF32 rhs)
{
#if fstb_IS (ARCHI, X86)
	return _mm_max_ps (lhs, rhs);
#elif fstb_IS (ARCHI, ARM)
	return vmaxq_f32 (lhs, rhs);
#endif // ff_arch_CPU
}



float	ToolsSimd::sum_h_flt (VectF32 v)
{
#if fstb_IS (ARCHI, X86)
	v = _mm_add_ps (v, _mm_shuffle_ps (v, v, (3 << 2) | 2));
	return _mm_cvtss_f32 (_mm_add_ss (v, _mm_shuffle_ps (v, v, 1)));
#elif fstb_IS (ARCHI, ARM)
	float32x2_t    v2 = vadd_f32 (vget_high_f32 (v), vget_low_f32 (v));
	return vget_lane_f32 (vpadd_f32 (v2, v2), 0);
#endif // ff_arch_CPU
}



float	ToolsSimd::min_h_flt (VectF32 v)
{
#if fstb_IS (ARCHI, X86)
	v = _mm_min_ps (v, _mm_shuffle_ps (v, v, (3 << 2) | 2));
	return _mm_cvtss_f32 (_mm_min_ss (v, _mm_shuffle_ps (v, v, 1)));
#elif fstb_IS (ARCHI, ARM)
	float32x2_t    v2 = vmin_f32 (vget_high_f32 (v), vget_low_f32 (v));
	return vget_lane_f32 (vpmin_f32 (v2, v2), 0);
#endif // ff_arch_CPU
}



float	ToolsSimd::max_h_flt (VectF32 v)
{
#if fstb_IS (ARCHI, X86)
	v = _mm_max_ps (v, _mm_shuffle_ps (v, v, (3 << 2) | 2));
	return _mm_cvtss_f32 (_mm_max_ss (v, _mm_shuffle_ps (v, v, 1)));
#elif fstb_IS (ARCHI, ARM)
	float32x2_t    v2 = vmax_f32 (vget_high_f32 (v), vget_low_f32 (v));
	return vget_lane_f32 (vpmax_f32 (v2, v2), 0);
#endif // ff_arch_CPU
}



// Assumes "to nearest" rounding mode on x86
ToolsSimd::VectF32	ToolsSimd::round (VectF32 v)
{
#if fstb_IS (ARCHI, X86)
	return _mm_cvtepi32_ps (_mm_cvtps_epi32 (v));
#elif fstb_IS (ARCHI, ARM)
	const auto     zero = vdupq_n_f32 ( 0.0f);
	const auto     m    = vdupq_n_f32 (-0.5f);
	const auto     p    = vdupq_n_f32 (+0.5f);
	const auto     gt0  = vcgtq_f32 (v, zero);
	const auto     u    = vbslq_f32 (gt0, p, m);
	v = vaddq_f32 (v, u);
	return v;
#endif // ff_arch_CPU
}



ToolsSimd::VectF32	ToolsSimd::abs (VectF32 v)
{
#if fstb_IS (ARCHI, X86)
	return _mm_andnot_ps (_mm_set1_ps (-0.f), v);
#elif fstb_IS (ARCHI, ARM)
	return vabsq_f32 (v);
#endif // ff_arch_CPU
}



ToolsSimd::VectF32	ToolsSimd::signbit (VectF32 v)
{
#if fstb_IS (ARCHI, X86)
	return _mm_and_ps (signbit_mask_f32 (), v);
#elif fstb_IS (ARCHI, ARM)
	return vreinterpretq_f32_u32 (vandq_u32 (
		vreinterpretq_u32_f32 (v),
		vdupq_n_u32 (0x80000000U)
	));
#endif // ff_arch_CPU
}



ToolsSimd::VectF32	ToolsSimd::signbit_mask_f32 ()
{
#if fstb_IS (ARCHI, X86)
	return _mm_set1_ps (-0.f);
#elif fstb_IS (ARCHI, ARM)
	return vreinterpretq_f32_u32 (vdupq_n_u32 (0x80000000U)); 
#endif // ff_arch_CPU
}



ToolsSimd::VectF32	ToolsSimd::rcp_approx (VectF32 v)
{
#if fstb_IS (ARCHI, X86)
	return _mm_rcp_ps (v);
#elif fstb_IS (ARCHI, ARM)
	float32x4_t    r = vrecpeq_f32 (v);
	r = vmulq_f32 (vrecpsq_f32 (v, r), r);
//	r = vmulq_f32 (vrecpsq_f32 (v, r), r); // Only one step needed?
	return r;
#endif // ff_arch_CPU
}



// With more accuracy
ToolsSimd::VectF32	ToolsSimd::rcp_approx2 (VectF32 v)
{
#if fstb_IS (ARCHI, X86)
	__m128         r = _mm_rcp_ps (v);
	r *= ToolsSimd::set1_f32 (2) - v * r;
	return r;
#elif fstb_IS (ARCHI, ARM)
	float32x4_t    r = vrecpeq_f32 (v);
	r = vmulq_f32 (vrecpsq_f32 (v, r), r);
	r = vmulq_f32 (vrecpsq_f32 (v, r), r);
	return r;
#endif // ff_arch_CPU
}



ToolsSimd::VectF32	ToolsSimd::div_approx (VectF32 n, VectF32 d)
{
#if fstb_IS (ARCHI, X86)
	return _mm_div_ps (n, d);
#elif fstb_IS (ARCHI, ARM)
	return n * rcp_approx (d);
#endif // ff_arch_CPU
}



// With more accuracy
ToolsSimd::VectF32	ToolsSimd::div_approx2 (VectF32 n, VectF32 d)
{
#if fstb_IS (ARCHI, X86)
	return _mm_div_ps (n, d);
#elif fstb_IS (ARCHI, ARM)
	return n * rcp_approx2 (d);
#endif // ff_arch_CPU
}



ToolsSimd::VectF32	ToolsSimd::sqrt (VectF32 v)
{
#if fstb_IS (ARCHI, X86)
	return _mm_sqrt_ps (v);
#elif fstb_IS (ARCHI, ARM)
	const uint32x4_t  nz_flag = vtstq_u32 (
		vreinterpretq_u32_f32 (v),
		vreinterpretq_u32_f32 (v)
	);
	float32x4_t    rs      = vrsqrteq_f32 (v);
	rs *= vrsqrtsq_f32 (v, rs * rs);
	rs *= vrsqrtsq_f32 (v, rs * rs);
	rs *= vrsqrtsq_f32 (v, rs * rs);
	const float32x4_t sqrt_a  = rs * v;
	return vreinterpretq_f32_u32 (vandq_u32 (
		vreinterpretq_u32_f32 (sqrt_a),
		nz_flag
	));
#endif // ff_arch_CPU
}



ToolsSimd::VectF32	ToolsSimd::sqrt_approx (VectF32 v)
{
#if fstb_IS (ARCHI, X86)
	const __m128   nz_flag = _mm_cmpgt_ps (v, _mm_setzero_ps ());
	const __m128   sqrt_a  = _mm_mul_ps (v, _mm_rsqrt_ps (v));
	return _mm_and_ps (sqrt_a, nz_flag);
#elif fstb_IS (ARCHI, ARM)
	const uint32x4_t  nz_flag = vtstq_u32 (
		vreinterpretq_u32_f32 (v),
		vreinterpretq_u32_f32 (v)
	);
	float32x4_t    rs      = vrsqrteq_f32 (v);
	rs *= vrsqrtsq_f32 (rs * v, rs);
	const float32x4_t sqrt_a  = rs * v;
	return vreinterpretq_f32_u32 (vandq_u32 (
		vreinterpretq_u32_f32 (sqrt_a),
		nz_flag
	));
#endif // ff_arch_CPU
}



ToolsSimd::VectF32	ToolsSimd::rsqrt_approx (VectF32 v)
{
#if fstb_IS (ARCHI, X86)
	return _mm_rsqrt_ps (v);
#elif fstb_IS (ARCHI, ARM)
	float32x4_t    rs = vrsqrteq_f32 (v);
	rs *= vrsqrtsq_f32 (rs * v, rs);
	return rs;
#endif // ff_arch_CPU
}



ToolsSimd::VectF32	ToolsSimd::rsqrt_approx2 (VectF32 v)
{
#if fstb_IS (ARCHI, X86)
	__m128         rs = _mm_rsqrt_ps (v);
	rs = _mm_set1_ps (0.5f) * rs * (_mm_set1_ps (3) - v * rs * rs);
	return rs;
#elif fstb_IS (ARCHI, ARM)
	float32x4_t    rs = vrsqrteq_f32 (v);
	rs *= vrsqrtsq_f32 (rs * v, rs);
	rs *= vrsqrtsq_f32 (rs * v, rs);
	return rs;
#endif // ff_arch_CPU
}



ToolsSimd::VectF32	ToolsSimd::log2_approx (VectF32 v)
{
	const int32_t  log2_sub = 128;
	const float		coef_a   = -1.f / 3;
	const float		coef_b   =  2.f;
	const float		coef_c   = -2.f / 3;

#if fstb_IS (ARCHI, X86)

	// Extracts the exponent (actually log2_int = exponent - 1)
	__m128i        xi = _mm_castps_si128 (v);
	xi = _mm_srli_epi32 (xi, 23);
	const __m128i  l2_sub = _mm_set1_epi32 (log2_sub);
	xi = _mm_sub_epi32 (xi, l2_sub);
	const auto     log2_int = _mm_cvtepi32_ps (xi);

#elif fstb_IS (ARCHI, ARM)

	int32x4_t      xi = vreinterpretq_s32_f32 (v);
	xi = vshrq_n_s32 (xi, 23);
	const int32x4_t   l2_sub = vdupq_n_s32 (log2_sub);
	xi -= l2_sub;
	const auto     log2_int = vcvtq_f32_s32 (xi);

#endif // ff_arch_CPU

	// Extracts the multiplicative part in [1 ; 2[
	const auto     mask_mantissa = set1_f32 (1.17549421e-38f); // Binary: (1 << 23) - 1
	auto           part          = and_f32 (v, mask_mantissa);
	const auto     bias          = set1_f32 (1.0f);            // Binary: 127 << 23
	part = or_f32 (part, bias);

	// Computes the polynomial [1 ; 2[ -> [1 ; 2[
	// y = -1/3*x^2 + 2*x - 2/3
	// Ensures the C1 continuity over the whole range.
	// Its exact inverse is:
	// x = 3 - sqrt (7 - 3*y)
	const auto     a = set1_f32 (coef_a);
	auto           poly = a * part;
	const auto     b = set1_f32 (coef_b);
	poly += b;
	poly *= part;
	const auto     c = set1_f32 (coef_c);
	poly += c;

	// Sums the components
	const auto     total = log2_int + poly;

	return total;
}



// Formula by 2DaT
// 12-13 ulp
// https://www.kvraudio.com/forum/viewtopic.php?f=33&t=532048
ToolsSimd::VectF32	ToolsSimd::log2_approx2 (VectF32 v)
{
	const auto     c0    = set1_f32 (1.011593342e+01f);
	const auto     c1    = set1_f32 (1.929443550e+01f);
	const auto     d0    = set1_f32 (2.095932245e+00f);
	const auto     d1    = set1_f32 (1.266638851e+01f);
	const auto     d2    = set1_f32 (6.316540241e+00f);
	const auto     one   = set1_f32 (1.0f);
	const auto     multi = set1_f32 (1.41421356237f);
	const auto     mantissa_mask = set1_s32 ((1 << 23) - 1);

#if fstb_IS (ARCHI, X86)

	__m128i        x_i           = _mm_castps_si128 (v);
	__m128i        spl_exp       = _mm_castps_si128 (v * multi);
	spl_exp = spl_exp - _mm_castps_si128 (one);
	spl_exp = _mm_andnot_si128 (mantissa_mask, spl_exp);
	const __m128   spl_mantissa  = _mm_castsi128_ps (x_i - spl_exp);
	spl_exp = _mm_srai_epi32 (spl_exp, 23);
	const __m128   log2_exponent = _mm_cvtepi32_ps (spl_exp);

#elif fstb_IS (ARCHI, ARM)

	const int32x4_t   x_i        = vreinterpretq_s32_f32 (v);
	int32x4_t      spl_exp       = vreinterpretq_s32_f32 (v * multi);
	spl_exp = spl_exp - vreinterpretq_s32_f32 (one);
	spl_exp = vandq_s32 (vmvnq_s32 (mantissa_mask), spl_exp);
	const float32x4_t   spl_mantissa  = vreinterpretq_f32_s32 (x_i - spl_exp);
	spl_exp = vshrq_n_s32 (spl_exp, 23);
	const float32x4_t   log2_exponent = vcvtq_f32_s32 (spl_exp);

#endif // ff_arch_CPU

	auto           num = spl_mantissa + c1;
	num = fmadd (num, spl_mantissa, c0);
	num *= spl_mantissa - one;

	auto           den = d2;
	den = fmadd (den, spl_mantissa, d1);
	den = fmadd (den, spl_mantissa, d0);

	auto           res = div_approx2 (num, den);
	res += log2_exponent;

	return res;
}



ToolsSimd::VectF32	ToolsSimd::exp2_approx (VectF32 v)
{
	const int32_t  exp2_add = 127;
	const float    coef_a   =  1.f / 3;
	const float    coef_b   =  2.f / 3;
	const float    coef_c   =  1.f;

	// Separates integer and fractional parts
#if fstb_IS (ARCHI, X86)
	const auto     round_toward_m_i = set1_f32 (-0.5f);
	auto           xi        = _mm_cvtps_epi32 (v + round_toward_m_i);
	const auto     val_floor = _mm_cvtepi32_ps (xi);
#elif fstb_IS (ARCHI, ARM)
	const int      round_ofs = 256;
	int32x4_t      xi = vcvtq_s32_f32 (v + set1_f32 (round_ofs));
	xi -= vdupq_n_s32 (round_ofs);
	const auto     val_floor = vcvtq_f32_s32 (xi);
#endif // ff_arch_CPU

	const auto     frac = v - val_floor;

	// Computes the polynomial [0 ; 1] -> [1 ; 2]
	// y = 1/3*x^2 + 2/3*x + 1
	// Ensures the C1 continuity over the whole range.
	// Its exact inverse is:
	// x = sqrt (3*y - 2) - 1
	const auto     a    = set1_f32 (coef_a);
	auto           poly = a * frac;
	const auto     b    = set1_f32 (coef_b);
	poly += b;
	poly *= frac;
	const auto     c    = set1_f32 (coef_c);
	poly += c;

	// Integer part
#if fstb_IS (ARCHI, X86)
	const __m128i	e2_add = _mm_set1_epi32 (exp2_add);
	xi += e2_add;
	xi  = _mm_slli_epi32 (xi, 23);
	const auto     int_part = _mm_castsi128_ps (xi);
#elif fstb_IS (ARCHI, ARM)
	xi += vdupq_n_s32 (exp2_add);
	xi  = vshlq_n_s32 (xi, 23);
	const auto     int_part = vreinterpretq_f32_s32 (xi);
#endif // ff_arch_CPU

	const auto     total = int_part * poly;

	return total;

}



// Formula by 2DaT
// 3-4 ulp
// https://www.kvraudio.com/forum/viewtopic.php?p=7161124#p7161124
ToolsSimd::VectF32	ToolsSimd::exp2_approx2 (VectF32 v)
{
	// [-0.5, 0.5] 2^x approx polynomial ~ 2.4 ulp
	const auto     c0 = set1_f32 (1.000000119e+00f);
	const auto     c1 = set1_f32 (6.931469440e-01f);
	const auto     c2 = set1_f32 (2.402212024e-01f);
	const auto     c3 = set1_f32 (5.550713092e-02f);
	const auto     c4 = set1_f32 (9.675540961e-03f);
	const auto     c5 = set1_f32 (1.327647245e-03f);

	// i = round (v)
	// v = v - i   
#if fstb_IS (ARCHI, X86)
	auto           i = _mm_cvtps_epi32 (v);          
	v -= _mm_cvtepi32_ps (i);
#elif fstb_IS (ARCHI, ARM)
	const int      round_ofs = 256;
	const auto     r = set1_f32 (round_ofs + 0.5f);
	auto           i = vcvtq_s32_f32 (v + r);
	i -= set1_s32 (round_ofs);
	v -= vcvtq_f32_s32 (i);
#endif // ff_arch_CPU

	// Estrin-Horner evaluation scheme
	const auto     v2  = v * v;
	const auto     p23 = fmadd (c3, v, c2);
	const auto     p01 = fmadd (c1, v, c0);
	auto           p   = fmadd (c5, v, c4);
	p = fmadd (p, v2, p23);
	p = fmadd (p, v2, p01);

	// i << 23
	// r = (2^i) * (2^v)
	// directly in floating point exponent
#if fstb_IS (ARCHI, X86)
	i = _mm_slli_epi32 (i, 23);
	return _mm_castsi128_ps (i + _mm_castps_si128 (p));
#elif fstb_IS (ARCHI, ARM)
	i = vshlq_n_s32 (i, 23);
	return vreinterpretq_f32_s32 (i + vreinterpretq_s32_f32 (p));
#endif // ff_arch_CPU
}



ToolsSimd::VectF32	ToolsSimd::select (VectF32 cond, VectF32 v_t, VectF32 v_f)
{
#if fstb_IS (ARCHI, X86)
	const auto     cond_1 = _mm_and_ps (cond, v_t);
	const auto     cond_0 = _mm_andnot_ps (cond, v_f);
	return _mm_or_ps (cond_0, cond_1);
#elif fstb_IS (ARCHI, ARM)
	return vbslq_f32 (vreinterpretq_u32_f32 (cond), v_t, v_f);
#endif // ff_arch_CPU
}



ToolsSimd::VectF32	ToolsSimd::cmp_gt_f32 (VectF32 lhs, VectF32 rhs)
{
#if fstb_IS (ARCHI, X86)
	return _mm_cmpgt_ps (lhs, rhs);
#elif fstb_IS (ARCHI, ARM)
	return vreinterpretq_f32_u32 (vcgtq_f32 (lhs, rhs));
#endif // ff_arch_CPU
}



ToolsSimd::VectS32	ToolsSimd::cmp_gt_s32 (VectS32 lhs, VectS32 rhs)
{
#if fstb_IS (ARCHI, X86)
	return _mm_cmpgt_epi32 (lhs, rhs);
#elif fstb_IS (ARCHI, ARM)
	return vreinterpretq_s32_u32 (vcgtq_s32 (lhs, rhs));
#endif // ff_arch_CPU
}



ToolsSimd::VectF32	ToolsSimd::cmp_lt_f32 (VectF32 lhs, VectF32 rhs)
{
#if fstb_IS (ARCHI, X86)
	return _mm_cmplt_ps (lhs, rhs);
#elif fstb_IS (ARCHI, ARM)
	return vreinterpretq_f32_u32 (vcltq_f32 (lhs, rhs));
#endif // ff_arch_CPU
}



ToolsSimd::VectS32	ToolsSimd::cmp_lt_s32 (VectS32 lhs, VectS32 rhs)
{
#if fstb_IS (ARCHI, X86)
	return _mm_cmplt_epi32 (lhs, rhs);
#elif fstb_IS (ARCHI, ARM)
	return vreinterpretq_s32_u32 (vcltq_s32 (lhs, rhs));
#endif // ff_arch_CPU
}



ToolsSimd::VectF32	ToolsSimd::cmp_eq_f32 (VectF32 lhs, VectF32 rhs)
{
#if fstb_IS (ARCHI, X86)
	return _mm_cmpeq_ps (lhs, rhs);
#elif fstb_IS (ARCHI, ARM)
	return vreinterpretq_f32_u32 (vceqq_f32 (lhs, rhs));
#endif // ff_arch_CPU
}



ToolsSimd::VectS32	ToolsSimd::cmp_eq_s32 (VectS32 lhs, VectS32 rhs)
{
#if fstb_IS (ARCHI, X86)
	return _mm_cmpeq_epi32 (lhs, rhs);
#elif fstb_IS (ARCHI, ARM)
	return vreinterpretq_s32_u32 (vceqq_s32 (lhs, rhs));
#endif // ff_arch_CPU
}



ToolsSimd::VectF32	ToolsSimd::cmp_ne_f32 (VectF32 lhs, VectF32 rhs)
{
#if fstb_IS (ARCHI, X86)
	return _mm_cmpneq_ps (lhs, rhs);
#elif fstb_IS (ARCHI, ARM)
	return vreinterpretq_f32_u32 (vmvnq_u32 (vceqq_f32 (lhs, rhs)));
#endif // ff_arch_CPU
}



ToolsSimd::VectS32	ToolsSimd::cmp_ne_s32 (VectS32 lhs, VectS32 rhs)
{
#if fstb_IS (ARCHI, X86)
	const auto     eq = _mm_cmpeq_epi32 (lhs, rhs);
	return _mm_xor_si128 (eq, _mm_set1_epi32 (-1));
#elif fstb_IS (ARCHI, ARM)
	return vreinterpretq_s32_u32 (vmvnq_u32 (vceqq_s32 (lhs, rhs)));
#endif // ff_arch_CPU
}



ToolsSimd::VectF32	ToolsSimd::and_f32 (VectF32 lhs, VectF32 rhs)
{
#if fstb_IS (ARCHI, X86)
	return _mm_and_ps (lhs, rhs);
#elif fstb_IS (ARCHI, ARM)
	return vreinterpretq_f32_u32 (vandq_u32 (
		vreinterpretq_u32_f32 (lhs),
		vreinterpretq_u32_f32 (rhs)
	));
#endif // ff_arch_CPU
}



ToolsSimd::VectS32	ToolsSimd::and_s32 (VectS32 lhs, VectS32 rhs)
{
#if fstb_IS (ARCHI, X86)
	return _mm_and_si128 (lhs, rhs);
#elif fstb_IS (ARCHI, ARM)
	return vandq_s32 (lhs, rhs);
#endif // ff_arch_CPU
}



ToolsSimd::VectF32	ToolsSimd::or_f32 (VectF32 lhs, VectF32 rhs)
{
#if fstb_IS (ARCHI, X86)
	return _mm_or_ps (lhs, rhs);
#elif fstb_IS (ARCHI, ARM)
	return vreinterpretq_f32_u32 (vorrq_u32 (
		vreinterpretq_u32_f32 (lhs),
		vreinterpretq_u32_f32 (rhs)
	));
#endif // ff_arch_CPU
}



ToolsSimd::VectS32	ToolsSimd::or_s32 (VectS32 lhs, VectS32 rhs)
{
#if fstb_IS (ARCHI, X86)
	return _mm_or_si128 (lhs, rhs);
#elif fstb_IS (ARCHI, ARM)
	return vorrq_s32 (lhs, rhs);
#endif // ff_arch_CPU
}



ToolsSimd::VectF32	ToolsSimd::xor_f32 (VectF32 lhs, VectF32 rhs)
{
#if fstb_IS (ARCHI, X86)
	return _mm_xor_ps (lhs, rhs);
#elif fstb_IS (ARCHI, ARM)
	return vreinterpretq_f32_u32 (veorq_u32 (
		vreinterpretq_u32_f32 (lhs),
		vreinterpretq_u32_f32 (rhs)
	));
#endif // ff_arch_CPU
}



ToolsSimd::VectS32	ToolsSimd::xor_s32 (VectS32 lhs, VectS32 rhs)
{
#if fstb_IS (ARCHI, X86)
	return _mm_xor_si128 (lhs, rhs);
#elif fstb_IS (ARCHI, ARM)
	return veorq_s32 (lhs, rhs);
#endif // ff_arch_CPU
}



// Works only with well-formed condition results (tested bits depends on the implementation).
// For each scalar, true = all bits set, false = all bits cleared
bool	ToolsSimd::and_h (VectF32 cond)
{
#if fstb_IS (ARCHI, X86)
	return (_mm_movemask_ps (cond) == 15);
#elif fstb_IS (ARCHI, ARM)
	const uint32x2_t  tmp = vreinterpret_u32_u16 (
		vqmovn_u32 (vreinterpretq_u32_f32 (cond))
	);
	return (   vget_lane_u32 (tmp, 0) == 0xFFFFFFFFU
	        && vget_lane_u32 (tmp, 1) == 0xFFFFFFFFU);
#endif // ff_arch_CPU
}



// Works only with well-formed condition results (tested bits depends on the implementation).
// For each scalar, true = all bits set, false = all bits cleared
bool	ToolsSimd::or_h (VectF32 cond)
{
#if fstb_IS (ARCHI, X86)
	return (_mm_movemask_ps (cond) != 0);
#elif fstb_IS (ARCHI, ARM)
	const uint32x2_t  tmp = vreinterpret_u32_u16 (
		vqmovn_u32 (vreinterpretq_u32_f32 (cond))
	);
	return (   vget_lane_u32 (tmp, 0) != 0
	        || vget_lane_u32 (tmp, 1) != 0);
#endif // ff_arch_CPU
}



// Works only with well-formed condition results (tested bits depends on the implementation).
// For each scalar, true = all bits set, false = all bits cleared
bool	ToolsSimd::and_h (VectS32 cond)
{
#if fstb_IS (ARCHI, X86)
	return (_mm_movemask_epi8 (cond) == 0xFFFF);
#elif fstb_IS (ARCHI, ARM)
	const uint32x2_t  tmp = vreinterpret_u32_u16 (
		vqmovn_u32 (vreinterpretq_u32_s32 (cond))
	);
	return (   vget_lane_u32 (tmp, 0) == 0xFFFFFFFFU
	        && vget_lane_u32 (tmp, 1) == 0xFFFFFFFFU);
#endif // ff_arch_CPU
}



// Works only with well-formed condition results (tested bits depends on the implementation).
// For each scalar, true = all bits set, false = all bits cleared
bool	ToolsSimd::or_h (VectS32 cond)
{
#if fstb_IS (ARCHI, X86)
	return (_mm_movemask_epi8 (cond) != 0);
#elif fstb_IS (ARCHI, ARM)
	const uint32x2_t  tmp = vreinterpret_u32_u16 (
		vqmovn_u32 (vreinterpretq_u32_s32 (cond))
	);
	return (   vget_lane_u32 (tmp, 0) != 0
	        || vget_lane_u32 (tmp, 1) != 0);
#endif // ff_arch_CPU
}



// "true" must be 1 and nothing else.
ToolsSimd::VectF32	ToolsSimd::set_mask_f32 (bool m0, bool m1, bool m2, bool m3)
{
#if 1 // Fast version
#if fstb_IS (ARCHI, X86)
	return _mm_castsi128_ps (_mm_sub_epi32 (
		_mm_setzero_si128 (),
		_mm_set_epi32 (m3, m2, m1, m0)
	));
#elif fstb_IS (ARCHI, ARM)
	float32x2_t    v01 = vdup_n_f32 (m0);
	float32x2_t    v23 = vdup_n_f32 (m2);
	v01 = vset_lane_f32 (m1, v01, 1);
	v23 = vset_lane_f32 (m3, v23, 1);
	return vreinterpretq_f32_s32 (vnegq_s32 (vreinterpretq_s32_f32 (
		vcombine_f32 (v01, v23)
	)));
#endif
#else // Safer but slower version
#if fstb_IS (ARCHI, X86)
	return _mm_castsi128_ps (_mm_sub_epi32 (
		_mm_set_epi32 (!m3, !m2, !m1, !m0),
		_mm_set1_epi32 (1)
	));
#elif fstb_IS (ARCHI, ARM)
	float32x2_t    v01 = vdup_n_f32 (!m0);
	float32x2_t    v23 = vdup_n_f32 (!m2);
	v01 = vset_lane_f32 (!m1, v01, 1);
	v23 = vset_lane_f32 (!m3, v23, 1);
	const auto     one  = vdupq_n_s32 (1);
	return vreinterpretq_f32_s32 (vsubq_s32 (
		vreinterpretq_s32_f32 (vcombine_f32 (v01, v23)),
		one
	));
#endif // ff_arch_CPU
#endif // Versions
}



int	ToolsSimd::count_bits (VectS32 x)
{
#if fstb_IS (ARCHI, X86)
	// https://stackoverflow.com/questions/17354971/fast-counting-the-number-of-set-bits-in-m128i-register
	static const __m128i  popcount_mask1 = _mm_set1_epi8 (0x77);
	static const __m128i  popcount_mask2 = _mm_set1_epi8 (0x0F);
	// Count bits in each 4-bit field.
	__m128i        n = _mm_srli_epi64(x, 1);
	n = _mm_and_si128 (popcount_mask1, n);
	x = _mm_sub_epi8 (x, n);
	n = _mm_srli_epi64 (n, 1);
	n = _mm_and_si128 (popcount_mask1, n);
	x = _mm_sub_epi8 (x, n);
	n = _mm_srli_epi64 (n, 1);
	n = _mm_and_si128 (popcount_mask1, n);
	n = _mm_sub_epi8 (x, n);
	n = _mm_add_epi8 (n, _mm_srli_epi16 (n, 4));
	n = _mm_and_si128 (popcount_mask2, n);
	// Counts the number of bits in the low and high 64-bit parts
	n = _mm_sad_epu8 (n, _mm_setzero_si128 ());
	// Counts the number of bits in the whole 128-bit register
	n = _mm_add_epi32 (n, _mm_unpackhi_epi64 (n, n));
	return _mm_cvtsi128_si32 (n);
#elif fstb_IS (ARCHI, ARM)
	const uint8x16_t  cnt_8  = vcntq_u8 (vreinterpretq_u8_s32 (x));
	const uint16x8_t  cnt_16 = vpaddlq_u8 (cnt_8);
	const uint32x4_t  cnt_32 = vpaddlq_u16 (cnt_16);
	const uint64x2_t  cnt_64 = vpaddlq_u32 (cnt_32);
	const int32x4_t   cnt_s  = vreinterpretq_s32_u64 (cnt_64);
	return vgetq_lane_s32 (cnt_s, 0) + vgetq_lane_s32 (cnt_s, 2);
#endif // ff_arch_CPU
}



// Assumes x is a result of a comparison, with all bits the same
// in each 32-bit element.
unsigned int	ToolsSimd::movemask_f32 (VectF32 x)
{
#if fstb_IS (ARCHI, X86)
	return (unsigned int) (_mm_movemask_ps (x));
#elif fstb_IS (ARCHI, ARM)
	uint64x2_t     tmp1 =
		vreinterpretq_u64_f32 (x);    // ddd...ddd ccc...ccc bbb...bbb aaa...aaa
	tmp1 = vshrq_n_u64 (tmp1, 31);   // 000...00d ddd...ddc 000...00b bbb...bba
	uint64x1_t     tmp2 = vsli_n_u64 (
		vget_high_u64 (tmp1),
		vget_low_u64 (tmp1),
		2
	);
	return (vget_lane_u32 (vreinterpret_u32_u64 (tmp2), 0) & 0xF);
#endif // ff_arch_CPU
}



// p1[1 0] p0[1 0]
ToolsSimd::VectF32	ToolsSimd::interleave_2f32_lo (VectF32 p0, VectF32 p1)
{
#if fstb_IS (ARCHI, X86)
	return _mm_shuffle_ps (p0, p1, (1<<6) + (0<<4) + (1<<2) + (0<<0));
#elif fstb_IS (ARCHI, ARM)
	const float32x2_t  p0x = vget_low_f32 (p0);
	const float32x2_t  p1x = vget_low_f32 (p1);
	return vcombine_f32 (p0x, p1x);
#endif // ff_arch_CPU
}



// p1[3 2] p0[3 2]
ToolsSimd::VectF32	ToolsSimd::interleave_2f32_hi (VectF32 p0, VectF32 p1)
{
#if fstb_IS (ARCHI, X86)
	return _mm_shuffle_ps (p0, p1, (3<<6) + (2<<4) + (3<<2) + (2<<0));
#elif fstb_IS (ARCHI, ARM)
	const float32x2_t  p0x = vget_high_f32 (p0);
	const float32x2_t  p1x = vget_high_f32 (p1);
	return vcombine_f32 (p0x, p1x);
#endif // ff_arch_CPU
}



void	ToolsSimd::interleave_f32 (VectF32 &i0, VectF32 &i1, VectF32 p0, VectF32 p1)
{
#if fstb_IS (ARCHI, X86)
	i0 = _mm_unpacklo_ps (p0, p1);
	i1 = _mm_unpackhi_ps (p0, p1);
#elif fstb_IS (ARCHI, ARM)
	const float32x4x2_t  tmp = vzipq_f32 (p0, p1);
	i0 = tmp.val [0];
	i1 = tmp.val [1];
#endif // ff_arch_CPU
}



void	ToolsSimd::deinterleave_f32 (VectF32 &p0, VectF32 &p1, VectF32 i0, VectF32 i1)
{
#if fstb_IS (ARCHI, X86)
	p0 = _mm_shuffle_ps (i0, i1, 0x88);
	p1 = _mm_shuffle_ps (i0, i1, 0xDD);
#elif fstb_IS (ARCHI, ARM)
	const float32x4x2_t  tmp = vuzpq_f32 (i0, i1);
	p0 = tmp.val [0];
	p1 = tmp.val [1];
#endif // ff_arch_CPU
}



ToolsSimd::VectF32	ToolsSimd::deinterleave_f32_lo (VectF32 i0, VectF32 i1)
{
#if fstb_IS (ARCHI, X86)
	return _mm_shuffle_ps (i0, i1, 0x88);
#elif fstb_IS (ARCHI, ARM)
	return vuzpq_f32 (i0, i1).val [0];
#endif // ff_arch_CPU
}



ToolsSimd::VectF32	ToolsSimd::deinterleave_f32_hi (VectF32 i0, VectF32 i1)
{
#if fstb_IS (ARCHI, X86)
	return _mm_shuffle_ps (i0, i1, 0xDD);
#elif fstb_IS (ARCHI, ARM)
	return vuzpq_f32 (i0, i1).val [1];
#endif // ff_arch_CPU
}



void	ToolsSimd::transpose_f32 (VectF32 &a0, VectF32 &a1, VectF32 &a2, VectF32 &a3)
{
	VectF32        k0, k1, k2, k3;
	interleave_f32 (k0, k1, a0, a2);
	interleave_f32 (k2, k3, a1, a3);
	interleave_f32 (a0, a1, k0, k2);
	interleave_f32 (a2, a3, k1, k3);
}



ToolsSimd::VectF32	ToolsSimd::monofy_2f32_lo (VectF32 v)
{
#if fstb_IS (ARCHI, X86)
	return _mm_shuffle_ps (v, v, 0xA0);
#elif fstb_IS (ARCHI, ARM)
	return vuzpq_f32 (v, v).val [0];
#endif // ff_arch_CPU
}



ToolsSimd::VectF32	ToolsSimd::monofy_2f32_hi (VectF32 v)
{
#if fstb_IS (ARCHI, X86)
	return _mm_shuffle_ps (v, v, 0xF5);
#elif fstb_IS (ARCHI, ARM)
	return vuzpq_f32 (v, v).val [1];
#endif // ff_arch_CPU
}



// Rounding method is unspecified (towards 0 on ARM, depends on MXCSR on x86)
ToolsSimd::VectS32	ToolsSimd::conv_f32_to_s32 (VectF32 x)
{
#if fstb_IS (ARCHI, X86)
	return _mm_cvtps_epi32 (x);
#elif fstb_IS (ARCHI, ARM)
	return vcvtq_s32_f32 (x);
#endif // ff_arch_CPU
}



// Not exact on the rounding boundaries
// Assumes rounding mode is to-nearest on x86
ToolsSimd::VectS32	ToolsSimd::round_f32_to_s32 (VectF32 x)
{
#if fstb_IS (ARCHI, X86)
	return _mm_cvtps_epi32 (x);
#elif fstb_IS (ARCHI, ARM)
	const auto     zero = vdupq_n_f32 ( 0.0f);
	const auto     m    = vdupq_n_f32 (-0.5f);
	const auto     p    = vdupq_n_f32 (+0.5f);
	const auto     gt0  = vcgtq_f32 (x, zero);
	const auto     u    = vbslq_f32 (gt0, p, m);
	x = vaddq_f32 (x, u);
	return vcvtq_s32_f32 (x);
#endif // ff_arch_CPU
}



// Not exact on the rounding boundaries
// Assumes rounding mode is to-nearest on x86
ToolsSimd::VectS32	ToolsSimd::floor_f32_to_s32 (VectF32 x)
{
#if fstb_IS (ARCHI, X86)
	x = _mm_add_ps (x, _mm_set1_ps (-0.5f));
	return _mm_cvtps_epi32 (x);
#elif fstb_IS (ARCHI, ARM)
	const auto     zero = vdupq_n_f32 (0.0f);
	const auto     one  = vdupq_n_f32 (1.0f);
	const auto     gt0  = vcgtq_f32 (x, zero);
	x = vbslq_f32 (gt0, x, vsubq_f32 (one, x));
	auto           i    = vcvtq_s32_f32 (x);
	i = vbslq_s32 (gt0, i, vnegq_s32 (i));
	return i;
#endif // ff_arch_CPU
}



ToolsSimd::VectF32	ToolsSimd::conv_s32_to_f32 (VectS32 x)
{
#if fstb_IS (ARCHI, X86)
	return _mm_cvtepi32_ps (x);
#elif fstb_IS (ARCHI, ARM)
	return vcvtq_f32_s32 (x);
#endif // ff_arch_CPU
}



void	ToolsSimd::start_lerp (VectF32 &val_cur, VectF32 &step, float val_beg, float val_end, int size)
{
	assert (size > 0);

	const float    dif = val_end - val_beg;
	float          four_over_size;
	if (size < _inv_table_4_len)
	{
		four_over_size = _inv_table_4 [size];
	}
	else
	{
		four_over_size = 4.0f / float (size);
	}
	step    = set1_f32 (dif * four_over_size);
	val_cur = set1_f32 (val_beg);
	const auto     c0123 = set_f32 (0, 0.25f, 0.5f, 0.75f);
	fstb::ToolsSimd::mac (val_cur, step, c0123);
}



ToolsSimd::VectS32	ToolsSimd::select (VectS32 cond, VectS32 v_t, VectS32 v_f)
{
#if fstb_IS (ARCHI, X86)
	const auto     cond_1 = _mm_and_si128 (cond, v_t);
	const auto     cond_0 = _mm_andnot_si128 (cond, v_f);
	return _mm_or_si128 (cond_0, cond_1);
#elif fstb_IS (ARCHI, ARM)
	return vbslq_s32 (vreinterpretq_u32_s32 (cond), v_t, v_f);
#endif // ff_arch_CPU
}



ToolsSimd::VectS32	ToolsSimd::min_s32 (VectS32 lhs, VectS32 rhs)
{
#if fstb_IS (ARCHI, X86)
	const auto     gt = _mm_cmpgt_epi32 (lhs, rhs);
	return select (gt, lhs, rhs);
#elif fstb_IS (ARCHI, ARM)
	return vminq_s32 (lhs, rhs);
#endif // ff_arch_CPU
}



ToolsSimd::VectS32	ToolsSimd::max_s32 (VectS32 lhs, VectS32 rhs)
{
#if fstb_IS (ARCHI, X86)
	const auto     lt = _mm_cmplt_epi32 (lhs, rhs);
	return select (lt, lhs, rhs);
#elif fstb_IS (ARCHI, ARM)
	return vmaxq_s32 (lhs, rhs);
#endif // ff_arch_CPU
}



// Positive = left
template <int SHIFT>
ToolsSimd::VectF32	ToolsSimd::Shift <SHIFT>::rotate (VectF32 a)
{
#if fstb_IS (ARCHI, X86)
	switch (SHIFT & 3)
	{
	case 1:  return _mm_shuffle_ps (a, a, (2<<6) | (1<<4) | (0<<2) | (3<<0));
	case 2:  return _mm_shuffle_ps (a, a, (1<<6) | (0<<4) | (3<<2) | (2<<0));
	case 3:  return _mm_shuffle_ps (a, a, (0<<6) | (3<<4) | (2<<2) | (1<<0));
	default: return a;
	}
#elif fstb_IS (ARCHI, ARM)
	int32x4_t     aa = vreinterpretq_s32_f32 (a);
	switch (SHIFT & 3)
	{
	case 1:  aa = vextq_s32 (aa, aa, 3); break;
	case 2:  aa = vextq_s32 (aa, aa, 2); break;
	case 3:  aa = vextq_s32 (aa, aa, 1); break;
	default: aa = aa;
	}
	return vreinterpretq_f32_s32 (aa);
#endif // ff_arch_CPU
}



template <int SHIFT>
float	ToolsSimd::Shift <SHIFT>::extract (VectF32 a)
{
#if fstb_IS (ARCHI, X86)
	switch (SHIFT & 3)
	{
	case 1:  a = _mm_shuffle_ps (a, a, 1);	break;
	case 2:  a = _mm_shuffle_ps (a, a, 2);	break;
	case 3:  a = _mm_shuffle_ps (a, a, 3);	break;
	default: a = a;
	}
	return _mm_cvtss_f32 (a);
#elif fstb_IS (ARCHI, ARM)
	return vgetq_lane_f32 (a, SHIFT & 3);
#endif // ff_arch_CPU
}



template <int SHIFT>
ToolsSimd::VectF32	ToolsSimd::Shift <SHIFT>::insert (VectF32 a, float val)
{
#if fstb_IS (ARCHI, X86)
	a = Shift <(-SHIFT) & 3>::rotate (a);
	a = _mm_move_ss (a, _mm_set_ss (val));
	a = Shift <  SHIFT     >::rotate (a);
	return a;
#elif fstb_IS (ARCHI, ARM)
	return vsetq_lane_f32 (val, a, SHIFT & 3);
#endif // ff_arch_CPU
}



template <int SHIFT>
ToolsSimd::VectF32	ToolsSimd::Shift <SHIFT>::spread (VectF32 a)
{
#if fstb_IS (ARCHI, X86)
	return _mm_shuffle_ps (a, a, 0x55 * SHIFT);
#elif fstb_IS (ARCHI, ARM)
	return vdupq_n_f32 (vgetq_lane_f32 (a, SHIFT));
#endif // ff_arch_CPU
}



// Positive = left
template <int SHIFT>
ToolsSimd::VectS32	ToolsSimd::Shift <SHIFT>::rotate (VectS32 a)
{
#if fstb_IS (ARCHI, X86)
	switch (SHIFT & 3)
	{
	case 1:  return _mm_shuffle_epi32 (a, (2<<6) | (1<<4) | (0<<2) | (3<<0));
	case 2:  return _mm_shuffle_epi32 (a, (1<<6) | (0<<4) | (3<<2) | (2<<0));
	case 3:  return _mm_shuffle_epi32 (a, (0<<6) | (3<<4) | (2<<2) | (1<<0));
	default: return a;
	}
#elif fstb_IS (ARCHI, ARM)
	switch (SHIFT & 3)
	{
	case 1:  a = vextq_s32 (a, a, 3); break;
	case 2:  a = vextq_s32 (a, a, 2); break;
	case 3:  a = vextq_s32 (a, a, 1); break;
	default: a = a;
	}
	return a;
#endif // ff_arch_CPU
}



template <int SHIFT>
int32_t	ToolsSimd::Shift <SHIFT>::extract (VectS32 a)
{
#if fstb_IS (ARCHI, X86)
	switch (SHIFT & 3)
	{
	case 1:  a = _mm_shuffle_epi32 (a, 1);	break;
	case 2:  a = _mm_shuffle_epi32 (a, 2);	break;
	case 3:  a = _mm_shuffle_epi32 (a, 3);	break;
	default: a = a;
	}
	return _mm_cvtsi128_si32 (a);
#elif fstb_IS (ARCHI, ARM)
	return vgetq_lane_s32 (a, SHIFT & 3);
#endif // ff_arch_CPU
}



template <int SHIFT>
ToolsSimd::VectS32	ToolsSimd::Shift <SHIFT>::insert (VectS32 a, int32_t val)
{
#if fstb_IS (ARCHI, X86)
	a = Shift <(-SHIFT) & 3>::rotate (a);
	a = _mm_castps_si128 (_mm_move_ss (
		_mm_castsi128_ps (a),
		_mm_castsi128_ps (_mm_set_ss (val))
	));
	a = Shift <  SHIFT     >::rotate (a);
	return a;
#elif fstb_IS (ARCHI, ARM)
	return vsetq_lane_s32 (val, a, SHIFT & 3);
#endif // ff_arch_CPU
}



template <int SHIFT>
ToolsSimd::VectS32	ToolsSimd::Shift <SHIFT>::spread (VectS32 a)
{
#if fstb_IS (ARCHI, X86)
	return _mm_shuffle_epi32 (a, 0x55 * SHIFT);
#elif fstb_IS (ARCHI, ARM)
	return vdupq_n_s32 (vgetq_lane_s32 (a, SHIFT));
#endif // ff_arch_CPU
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



template <typename MEM>
void	ToolsSimd::store_f32_part_n13 (MEM *ptr, VectF32 v, int n)
{
	assert (n > 0);
	assert (n < 4);

	float *        f_ptr = reinterpret_cast <float *> (ptr);

#if fstb_IS (ARCHI, ARM)

	vst1q_lane_f32 (f_ptr + 0, v, 0);
	if (n >= 2)
	{
		vst1q_lane_f32 (f_ptr + 1, v, 1);
		if (n >= 3)
		{
			vst1q_lane_f32 (f_ptr + 2, v, 2);
		}
	}

#else

	for (int i = 0; i < n; ++i)
	{
		f_ptr [i] = Shift < 0>::extract (v);
		v         = Shift <-1>::rotate (v);
	}

#endif
}



template <typename MEM>
void	ToolsSimd::store_s32_part_n13 (MEM *ptr, VectS32 v, int n)
{
	assert (n > 0);
	assert (n < 4);

	int32_t *      f_ptr = reinterpret_cast <int32_t *> (ptr);

#if fstb_IS (ARCHI, ARM)

	vst1q_lane_s32 (f_ptr + 0, v, 0);
	if (n >= 2)
	{
		vst1q_lane_s32 (f_ptr + 1, v, 1);
		if (n >= 3)
		{
			vst1q_lane_s32 (f_ptr + 2, v, 2);
		}
	}

#else

	for (int i = 0; i < n; ++i)
	{
		f_ptr [i] = Shift < 0>::extract (v);
		v         = Shift <-1>::rotate (v);
	}

#endif
}



template <typename MEM>
ToolsSimd::VectF32	ToolsSimd::load_f32_part_n13 (const MEM *ptr, int n)
{
	assert (n > 0);
	assert (n < 4);

	const float *  f_ptr = reinterpret_cast <const float *> (ptr);
	VectF32        v;
#if fstb_IS (ARCHI, X86)
	switch (n)
	{
	case 1:
		v = _mm_load_ss (f_ptr);
		break;
	case 2:
		v = _mm_load_ss (f_ptr + 1);
		v = Shift <1>::rotate (v);
		v = Shift <0>::insert (v, f_ptr [0]);
		break;
	case 3:
		v = _mm_load_ss (f_ptr + 2);
		v = Shift <1>::rotate (v);
		v = Shift <0>::insert (v, f_ptr [1]);
		v = Shift <1>::rotate (v);
		v = Shift <0>::insert (v, f_ptr [0]);
		break;
	}
#elif fstb_IS (ARCHI, ARM)
	v = vmovq_n_f32 (f_ptr [0]);
	if (n >= 2)
	{
		v = vld1q_lane_f32 (f_ptr + 1, v, 1);
		if (n >= 3)
		{
			v = vld1q_lane_f32 (f_ptr + 2, v, 2);
		}
	}
#endif // ff_arch_CPU

	return v;
}



}  // namespace fstb



#if fstb_IS (COMPILER, MSVC)



fstb::ToolsSimd::VectF32 &	operator += (fstb::ToolsSimd::VectF32 &lhs, fstb::ToolsSimd::VectF32 rhs)
{
#if fstb_IS (ARCHI, X86)
	lhs = _mm_add_ps (lhs, rhs);
#elif fstb_IS (ARCHI, ARM)
	lhs = vaddq_f32 (lhs, rhs);
#endif // ff_arch_CPU
	return lhs;
}



fstb::ToolsSimd::VectF32 &	operator -= (fstb::ToolsSimd::VectF32 &lhs, fstb::ToolsSimd::VectF32 rhs)
{
#if fstb_IS (ARCHI, X86)
	lhs = _mm_sub_ps (lhs, rhs);
#elif fstb_IS (ARCHI, ARM)
	lhs = vsubq_f32 (lhs, rhs);
#endif // ff_arch_CPU
	return lhs;
}



fstb::ToolsSimd::VectF32 &	operator *= (fstb::ToolsSimd::VectF32 &lhs, fstb::ToolsSimd::VectF32 rhs)
{
#if fstb_IS (ARCHI, X86)
	lhs = _mm_mul_ps (lhs, rhs);
#elif fstb_IS (ARCHI, ARM)
	lhs = vmulq_f32 (lhs, rhs);
#endif // ff_arch_CPU
	return lhs;
}



fstb::ToolsSimd::VectF32 operator + (fstb::ToolsSimd::VectF32 lhs, fstb::ToolsSimd::VectF32 rhs)
{
#if fstb_IS (ARCHI, X86)
	return _mm_add_ps (lhs, rhs);
#elif fstb_IS (ARCHI, ARM)
	return vaddq_f32 (lhs, rhs);
#endif // ff_arch_CPU
}



fstb::ToolsSimd::VectF32 operator - (fstb::ToolsSimd::VectF32 lhs, fstb::ToolsSimd::VectF32 rhs)
{
#if fstb_IS (ARCHI, X86)
	return _mm_sub_ps (lhs, rhs);
#elif fstb_IS (ARCHI, ARM)
	return vsubq_f32 (lhs, rhs);
#endif // ff_arch_CPU
}



fstb::ToolsSimd::VectF32 operator * (fstb::ToolsSimd::VectF32 lhs, fstb::ToolsSimd::VectF32 rhs)
{
#if fstb_IS (ARCHI, X86)
	return _mm_mul_ps (lhs, rhs);
#elif fstb_IS (ARCHI, ARM)
	return vmulq_f32 (lhs, rhs);
#endif // ff_arch_CPU
}



fstb::ToolsSimd::VectS32 &	operator += (fstb::ToolsSimd::VectS32 &lhs, fstb::ToolsSimd::VectS32 rhs)
{
#if fstb_IS (ARCHI, X86)
	lhs = _mm_add_epi32 (lhs, rhs);
#elif fstb_IS (ARCHI, ARM)
	lhs = vaddq_s32 (lhs, rhs);
#endif // ff_arch_CPU
	return lhs;
}



fstb::ToolsSimd::VectS32 &	operator -= (fstb::ToolsSimd::VectS32 &lhs, fstb::ToolsSimd::VectS32 rhs)
{
#if fstb_IS (ARCHI, X86)
	lhs = _mm_sub_epi32 (lhs, rhs);
#elif fstb_IS (ARCHI, ARM)
	lhs = vsubq_s32 (lhs, rhs);
#endif // ff_arch_CPU
	return lhs;
}



fstb::ToolsSimd::VectS32 &	operator *= (fstb::ToolsSimd::VectS32 &lhs, fstb::ToolsSimd::VectS32 rhs)
{
#if fstb_IS (ARCHI, X86)
	// mullo_epi32
	// Code of this function shamelessly borrowed from tp7
	// https://github.com/tp7/masktools/blob/16bit/masktools/common/simd.h
	const __m128i  lhs13  = _mm_shuffle_epi32 (lhs, 0xF5);       // (-,a3,-,a1)
	const __m128i  rhs13  = _mm_shuffle_epi32 (rhs, 0xF5);       // (-,b3,-,b1)
	const __m128i  prod02 = _mm_mul_epu32 (lhs, rhs);            // (-,a2*b2,-,a0*b0)
	const __m128i  prod13 = _mm_mul_epu32 (lhs13, rhs13);        // (-,a3*b3,-,a1*b1)
	const __m128i  prod01 = _mm_unpacklo_epi32 (prod02, prod13); // (-,-,a1*b1,a0*b0)
	const __m128i  prod23 = _mm_unpackhi_epi32 (prod02, prod13); // (-,-,a3*b3,a2*b2)
	lhs                   = _mm_unpacklo_epi64 (prod01 ,prod23); // (ab3,ab2,ab1,ab0)
#elif fstb_IS (ARCHI, ARM)
	lhs = vmulq_s32 (lhs, rhs);
#endif // ff_arch_CPU
	return lhs;
}



fstb::ToolsSimd::VectS32 & operator >>= (fstb::ToolsSimd::VectS32 &x, int scalar)
{
	assert (scalar >= 0);
#if fstb_IS (ARCHI, X86)
	x = _mm_srai_epi32 (x, scalar);
#elif fstb_IS (ARCHI, ARM)
	x = vshrq_n_s32 (x, scalar);
#endif // ff_arch_CPU
	return x;
}



fstb::ToolsSimd::VectS32 & operator <<= (fstb::ToolsSimd::VectS32 &x, int scalar)
{
	assert (scalar >= 0);
#if fstb_IS (ARCHI, X86)
	x = _mm_slli_epi32 (x, scalar);
#elif fstb_IS (ARCHI, ARM)
	x = vshlq_n_s32 (x, scalar);
#endif // ff_arch_CPU
	return x;
}



fstb::ToolsSimd::VectS32 operator + (fstb::ToolsSimd::VectS32 lhs, fstb::ToolsSimd::VectS32 rhs)
{
#if fstb_IS (ARCHI, X86)
	return _mm_add_epi32 (lhs, rhs);
#elif fstb_IS (ARCHI, ARM)
	return vaddq_s32 (lhs, rhs);
#endif // ff_arch_CPU
}



fstb::ToolsSimd::VectS32 operator - (fstb::ToolsSimd::VectS32 lhs, fstb::ToolsSimd::VectS32 rhs)
{
#if fstb_IS (ARCHI, X86)
	return _mm_sub_epi32 (lhs, rhs);
#elif fstb_IS (ARCHI, ARM)
	return vsubq_s32 (lhs, rhs);
#endif // ff_arch_CPU
}



fstb::ToolsSimd::VectS32 operator * (fstb::ToolsSimd::VectS32 lhs, fstb::ToolsSimd::VectS32 rhs)
{
#if fstb_IS (ARCHI, X86)
	return lhs *= rhs;
#elif fstb_IS (ARCHI, ARM)
	return vmulq_s32 (lhs, rhs);
#endif // ff_arch_CPU
}



fstb::ToolsSimd::VectS32 operator >> (fstb::ToolsSimd::VectS32 x, int scalar)
{
	assert (scalar >= 0);
#if fstb_IS (ARCHI, X86)
	return _mm_srai_epi32 (x, scalar);
#elif fstb_IS (ARCHI, ARM)
	return vshrq_n_s32 (x, scalar);
#endif // ff_arch_CPU
}



fstb::ToolsSimd::VectS32 operator << (fstb::ToolsSimd::VectS32 x, int scalar)
{
	assert (scalar >= 0);
#if fstb_IS (ARCHI, X86)
	return _mm_slli_epi32 (x, scalar);
#elif fstb_IS (ARCHI, ARM)
	return vshlq_n_s32 (x, scalar);
#endif // ff_arch_CPU
}



#endif // MSVC



#endif   // fstb_ToolsSimd_CODEHEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

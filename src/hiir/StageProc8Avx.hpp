/*****************************************************************************

        StageProc8Avx.hpp
        Port of StageProc4See.hpp from SSE to AVX by Dario Mambro
        StageProc4See.hpp by Laurent de Soras, 2015

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



#if defined (hiir_StageProc8Avx_CURRENT_CODEHEADER)
	#error Recursive inclusion of StageProc8Avx code header.
#endif
#define hiir_StageProc8Avx_CURRENT_CODEHEADER

#if ! defined (hiir_StageProc8Avx_CODEHEADER_INCLUDED)
#define hiir_StageProc8Avx_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "hiir/StageDataAvx.h"



#if defined (_MSC_VER)
	#pragma inline_depth (255)
#endif



namespace hiir
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



template <>
hiir_FORCEINLINE void	StageProc8Avx <1>::process_sample_pos (const int nbr_coefs, __m256 &spl_0, __m256 &spl_1, StageDataAvx *stage_arr) noexcept
{
	const int      cnt   = nbr_coefs + 2 - 1;

	const __m256   tmp_0 = _mm256_add_ps (
		_mm256_mul_ps (
			_mm256_sub_ps (spl_0, _mm256_load_ps (stage_arr [cnt    ]._mem)),
			_mm256_load_ps (stage_arr [cnt    ]._coef)
		),
		_mm256_load_ps (stage_arr [cnt - 2]._mem)
	);

	_mm256_store_ps (stage_arr [cnt - 2]._mem, spl_0);
	_mm256_store_ps (stage_arr [cnt - 1]._mem, spl_1);
	_mm256_store_ps (stage_arr [cnt    ]._mem, tmp_0);

	spl_0 = tmp_0;
}



template <>
hiir_FORCEINLINE void	StageProc8Avx <0>::process_sample_pos (const int nbr_coefs, __m256 &spl_0, __m256 &spl_1, StageDataAvx *stage_arr) noexcept
{
	const int      cnt = nbr_coefs + 2;

	_mm256_store_ps (stage_arr [cnt - 2]._mem, spl_0);
	_mm256_store_ps (stage_arr [cnt - 1]._mem, spl_1);
}



template <int REMAINING>
void	StageProc8Avx <REMAINING>::process_sample_pos (const int nbr_coefs, __m256 &spl_0, __m256 &spl_1, StageDataAvx *stage_arr) noexcept
{
	const int      cnt   = nbr_coefs + 2 - REMAINING;

	const __m256   tmp_0 = _mm256_add_ps (
		_mm256_mul_ps (
			_mm256_sub_ps (spl_0, _mm256_load_ps (stage_arr [cnt    ]._mem)),
			_mm256_load_ps (stage_arr [cnt    ]._coef)
		),
		_mm256_load_ps (stage_arr [cnt - 2]._mem)
	);
	const __m256   tmp_1 = _mm256_add_ps (
		_mm256_mul_ps (
			_mm256_sub_ps (spl_1, _mm256_load_ps (stage_arr [cnt + 1]._mem)),
			_mm256_load_ps (stage_arr [cnt + 1]._coef)
		),
		_mm256_load_ps (stage_arr [cnt - 1]._mem)
	);

	_mm256_store_ps (stage_arr [cnt - 2]._mem, spl_0);
	_mm256_store_ps (stage_arr [cnt - 1]._mem, spl_1);

	spl_0 = tmp_0;
	spl_1 = tmp_1;

	StageProc8Avx <REMAINING - 2>::process_sample_pos (
		nbr_coefs,
		spl_0,
		spl_1,
		stage_arr
	);
}



template <>
hiir_FORCEINLINE void	StageProc8Avx <1>::process_sample_neg (const int nbr_coefs, __m256 &spl_0, __m256 &spl_1, StageDataAvx *stage_arr) noexcept
{
	const int      cnt   = nbr_coefs + 2 - 1;

	const __m256   tmp_0 = _mm256_sub_ps (
		_mm256_mul_ps (
			_mm256_add_ps (spl_0, _mm256_load_ps (stage_arr [cnt    ]._mem)),
			_mm256_load_ps (stage_arr [cnt    ]._coef)
		),
		_mm256_load_ps (stage_arr [cnt - 2]._mem)
	);

	_mm256_store_ps (stage_arr [cnt - 2]._mem, spl_0);
	_mm256_store_ps (stage_arr [cnt - 1]._mem, spl_1);
	_mm256_store_ps (stage_arr [cnt    ]._mem, tmp_0);

	spl_0 = tmp_0;
}



template <>
hiir_FORCEINLINE void	StageProc8Avx <0>::process_sample_neg (const int nbr_coefs, __m256 &spl_0, __m256 &spl_1, StageDataAvx *stage_arr) noexcept
{
	const int      cnt = nbr_coefs + 2;

	_mm256_store_ps (stage_arr [cnt - 2]._mem, spl_0);
	_mm256_store_ps (stage_arr [cnt - 1]._mem, spl_1);
}



template <int REMAINING>
void	StageProc8Avx <REMAINING>::process_sample_neg (const int nbr_coefs, __m256 &spl_0, __m256 &spl_1, StageDataAvx *stage_arr) noexcept
{
	const int      cnt   = nbr_coefs + 2 - REMAINING;

	const __m256   tmp_0 = _mm256_sub_ps (
		_mm256_mul_ps (
			_mm256_add_ps (spl_0, _mm256_load_ps (stage_arr [cnt    ]._mem)),
			_mm256_load_ps (stage_arr [cnt    ]._coef)
		),
		_mm256_load_ps (stage_arr [cnt - 2]._mem)
	);
	const __m256   tmp_1 = _mm256_sub_ps (
		_mm256_mul_ps (
			_mm256_add_ps (spl_1, _mm256_load_ps (stage_arr [cnt + 1]._mem)),
			_mm256_load_ps (stage_arr [cnt + 1]._coef)
		),
		_mm256_load_ps (stage_arr [cnt - 1]._mem)
	);

	_mm256_store_ps (stage_arr [cnt - 2]._mem, spl_0);
	_mm256_store_ps (stage_arr [cnt - 1]._mem, spl_1);

	spl_0 = tmp_0;
	spl_1 = tmp_1;

	StageProc8Avx <REMAINING - 2>::process_sample_neg (
		nbr_coefs,
		spl_0,
		spl_1,
		stage_arr
	);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace hiir



#endif   // hiir_StageProc8Avx_CODEHEADER_INCLUDED

#undef hiir_StageProc8Avx_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

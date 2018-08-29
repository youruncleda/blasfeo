/**************************************************************************************************
*                                                                                                 *
* This file is part of BLASFEO.                                                                   *
*                                                                                                 *
* BLASFEO -- BLAS For Embedded Optimization.                                                      *
* Copyright (C) 2016-2018 by Gianluca Frison.                                                     *
* Developed at IMTEK (University of Freiburg) under the supervision of Moritz Diehl.              *
* All rights reserved.                                                                            *
*                                                                                                 *
* This program is free software: you can redistribute it and/or modify                            *
* it under the terms of the GNU General Public License as published by                            *
* the Free Software Foundation, either version 3 of the License, or                               *
* (at your option) any later version                                                              *.
*                                                                                                 *
* This program is distributed in the hope that it will be useful,                                 *
* but WITHOUT ANY WARRANTY; without even the implied warranty of                                  *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                                   *
* GNU General Public License for more details.                                                    *
*                                                                                                 *
* You should have received a copy of the GNU General Public License                               *
* along with this program.  If not, see <https://www.gnu.org/licenses/>.                          *
*                                                                                                 *
* The authors designate this particular file as subject to the "Classpath" exception              *
* as provided by the authors in the LICENSE file that accompained this code.                      *
*                                                                                                 *
* Author: Gianluca Frison, gianluca.frison (at) imtek.uni-freiburg.de                             *
*                                                                                                 *
**************************************************************************************************/

#include <stdlib.h>
#include <stdio.h>

#include "../include/blasfeo_target.h"
#include "../include/blasfeo_common.h"
#include "../include/blasfeo_timing.h"
#include "../include/blasfeo_d_aux.h"
#include "../include/blasfeo_d_aux_ext_dep.h"
#include "../include/blasfeo_d_kernel.h"
#include "../include/blasfeo_d_blas.h"

#if defined(REF_BLAS_BLIS)
#include "../include/d_blas_64.h"
#elif defined(REF_BLAS_MKL)
#include "mkl.h"
#else
#include "../include/d_blas.h"
#endif


int main()
	{

#if !defined(BLAS_API)
	printf("\nRecompile with BLAS_API=1 to run this benchmark!\n\n");
	exit(1);
#endif

	const double GHz_max = 3.3;
	const double flops_max = 16;

	FILE *f;
	f = fopen("./build/tmp_run.m", "w"); // a

	printf("A = [%f %f];\n", GHz_max, flops_max);
	fprintf(f, "A = [%f %f];\n", GHz_max, flops_max);
	printf("\n");
	fprintf(f, "\n");
	printf("B = [\n");
	fprintf(f, "B = [\n");

	int ii, jj, ll;

	int rep, rep_in;
	int nrep_in = 10; // number of benchmark batches

	int nn[] = {4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64, 68, 72, 76, 80, 84, 88, 92, 96, 100, 104, 108, 112, 116, 120, 124, 128, 132, 136, 140, 144, 148, 152, 156, 160, 164, 168, 172, 176, 180, 184, 188, 192, 196, 200, 204, 208, 212, 216, 220, 224, 228, 232, 236, 240, 244, 248, 252, 256, 260, 264, 268, 272, 276, 280, 284, 288, 292, 296, 300, 304, 308, 312, 316, 320, 324, 328, 332, 336, 340, 344, 348, 352, 356, 360, 364, 368, 372, 376, 380, 384, 388, 392, 396, 400, 404, 408, 412, 416, 420, 424, 428, 432, 436, 440, 444, 448, 452, 456, 460, 500, 550, 600, 650, 700};
	int nnrep[] = {10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 400, 400, 400, 400, 400, 200, 200, 200, 200, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 20, 20, 20, 20, 20, 20, 20, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 4, 4, 4, 4, 4};

//	for(ll=0; ll<1; ll++)
//	for(ll=0; ll<2; ll++) // up to 8
//	for(ll=0; ll<3; ll++) // up to 12
//	for(ll=0; ll<4; ll++) // up to 16
//	for(ll=0; ll<24; ll++)
//	for(ll=0; ll<63; ll++) // up to 256
	for(ll=0; ll<75; ll++) // up to 300
//	for(ll=0; ll<115; ll++)
//	for(ll=0; ll<120; ll++)

		{

		int n = nn[ll];
//		int n = 12;
		int nrep = nnrep[ll]/nrep_in;
		nrep = nrep>1 ? nrep : 1;
//		int n = ll+1;
//		int nrep = nnrep[0];
//		n = n<12 ? 12 : n;
//		n = n<8 ? 8 : n;
//		nrep = 1;


		double *A = malloc(n*n*sizeof(double));
		for(ii=0; ii<n*n; ii++)
			A[ii] = ii;
		int lda = n;
//		d_print_mat(n, n, A, n);

		double *B = malloc(n*n*sizeof(double));
		for(ii=0; ii<n*n; ii++)
			B[ii] = 0;
		for(ii=0; ii<n; ii++)
			B[ii*(n+1)] = 1.0;
		int ldb = n;
//		d_print_mat(n, n, B, ldb);

		double *C = malloc(n*n*sizeof(double));
		for(ii=0; ii<n*n; ii++)
			C[ii] = -1;
		int ldc = n;
//		d_print_mat(n, n, C, ldc);

		double *D = malloc(n*n*sizeof(double));
		for(ii=0; ii<n*n; ii++)
			D[ii] = -1;
		int ldd = n;
//		d_print_mat(n, n, C, ldc);


		int bs = 4;

		struct blasfeo_dmat sA; blasfeo_allocate_dmat(n, n, &sA);
		blasfeo_pack_dmat(n, n, A, n, &sA, 0, 0);
		int sda = sA.cn;
		struct blasfeo_dmat sB; blasfeo_allocate_dmat(n, n, &sB);
		blasfeo_pack_dmat(n, n, B, n, &sB, 0, 0);
		int sdb = sB.cn;
		struct blasfeo_dmat sC; blasfeo_allocate_dmat(n, n, &sC);
		blasfeo_pack_dmat(n, n, C, n, &sC, 0, 0);
		int sdc = sC.cn;
		struct blasfeo_dmat sD; blasfeo_allocate_dmat(n, n, &sD);
		blasfeo_pack_dmat(n, n, D, n, &sD, 0, 0);
		int sdd = sD.cn;


		/* timing */
		blasfeo_timer timer;

		double time_blasfeo   = 1e15;
		double time_blas      = 1e15;
		double time_blas_pack = 1e15;
		double tmp_time_blasfeo;
		double tmp_time_blas;
		double tmp_time_blas_pack;

		/* benchmarks */

		char ta = 'n';
		char tb = 't';
		char uplo = 'l';
		int info = 0;

		double alpha = 1.0;
		double beta = 0.0;





		/* call blas */
		for(rep_in=0; rep_in<nrep_in; rep_in++)
			{

			for(ii=0; ii<n*n; ii++) C[ii] = B[ii];
			dgemm_(&ta, &tb, &n, &n, &n, &alpha, A, &n, A, &n, &beta, C, &n);

			// BENCHMARK_BLAS
			blasfeo_tic(&timer);

			// averaged repetions
			for(rep=0; rep<nrep; rep++)
				{

//				for(ii=0; ii<n*n; ii++) C[ii] = B[ii];
				dgemm_(&ta, &tb, &n, &n, &n, &alpha, A, &n, B, &n, &beta, C, &n);
//				for(ii=0; ii<n*n; ii++) D[ii] = C[ii];
//				dpotrf_(&uplo, &n, D, &n, &info);
//				dpotrf_(&uplo, &n, B, &n, &info);

				}

			tmp_time_blas = blasfeo_toc(&timer) / nrep;
			time_blas = tmp_time_blas<time_blas ? tmp_time_blas : time_blas;
			// BENCHMARK_BLAS

			}

//		d_print_mat(n, n, C, ldc);
//		d_print_mat(n, n, D, ldd);





		/* call blas with packing */
		for(rep_in=0; rep_in<nrep_in; rep_in++)
			{

			for(ii=0; ii<n*n; ii++) C[ii] = B[ii];
			blasfeo_dgemm(&ta, &tb, &n, &n, &n, &alpha, A, &n, A, &n, &beta, C, &n);

			// BENCHMARK_BLASFEO
			blasfeo_tic(&timer);

			// averaged repetions
			for(rep=0; rep<nrep; rep++)
				{

//				for(ii=0; ii<n*n; ii++) C[ii] = B[ii];
				blasfeo_dgemm(&ta, &tb, &n, &n, &n, &alpha, A, &n, B, &n, &beta, C, &n);
//				for(ii=0; ii<n*n; ii++) D[ii] = C[ii];
//				blasfeo_dpotrf(&uplo, &n, D, &n);
//				blasfeo_dpotrf(&uplo, &n, B, &n);

				}

			tmp_time_blas_pack = blasfeo_toc(&timer) / nrep;
			time_blas_pack = tmp_time_blas_pack<time_blas_pack ? tmp_time_blas_pack : time_blas_pack;
			// BENCHMARK_BLASFEO_END

			}

//		d_print_mat(n, n, C, ldc);
//		d_print_mat(n, n, D, ldd);





		/* call blasfeo */
		for(rep_in=0; rep_in<nrep_in; rep_in++)
			{

			// BENCHMARK_BLASFEO
			blasfeo_tic(&timer);

			// averaged repetions
			for(rep=0; rep<nrep; rep++)
				{
				
//				blasfeo_dgemm_nn(n, n, n, alpha, &sA, 0, 0, &sB, 0, 0, beta, &sC, 0, 0, &sC, 0, 0);
				blasfeo_dgemm_nt(n, n, n, alpha, &sA, 0, 0, &sB, 0, 0, beta, &sC, 0, 0, &sC, 0, 0);
//				blasfeo_dpotrf_l(n, &sB, 0, 0, &sB, 0, 0);

				}

			tmp_time_blasfeo = blasfeo_toc(&timer) / nrep;
			time_blasfeo = tmp_time_blasfeo<time_blasfeo ? tmp_time_blasfeo : time_blasfeo;
			// BENCHMARK_BLASFEO_END

			}

//		d_print_mat(n, n, C, ldc);
//		blasfeo_print_dmat(n, n, &sC, 0, 0);



		double Gflops_max = flops_max * GHz_max;

		double flop_operation = 2.0*n*n*n; // gemm
//		double flop_operation = 1.0/3.0*n*n*n; // potrf

		double Gflops_blas      = 1e-9*flop_operation/time_blas;
		double Gflops_blas_pack = 1e-9*flop_operation/time_blas_pack;
		double Gflops_blasfeo   = 1e-9*flop_operation/time_blasfeo;

		printf("%d\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\n",
			n,
			Gflops_blas, 100.0*Gflops_blas/Gflops_max,
			Gflops_blas_pack, 100.0*Gflops_blas_pack/Gflops_max,
			Gflops_blasfeo, 100.0*Gflops_blasfeo/Gflops_max);
		fprintf(f, "%d\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\t%7.2f\n",
			n,
			Gflops_blas, 100.0*Gflops_blas/Gflops_max,
			Gflops_blas_pack, 100.0*Gflops_blas_pack/Gflops_max,
			Gflops_blasfeo, 100.0*Gflops_blasfeo/Gflops_max);

		free(A);
		free(B);
		free(C);
		free(D);
		blasfeo_free_dmat(&sA);
		blasfeo_free_dmat(&sB);
		blasfeo_free_dmat(&sC);
		blasfeo_free_dmat(&sD);

		}

	printf("];\n");
	fprintf(f, "];\n");

	fclose(f);

	return 0;

	}
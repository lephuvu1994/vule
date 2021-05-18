#include "filter.h"
#include "matrix.h"

/*float LPF(float x, float cutoff, float sample_rate)
{
	float RC,dt, alpha,y;
	static float ylast =0;
	RC = 1.0/(cutoff * 2 * 3.14);
	dt = 1.0/ sample_rate;
	alpha = dt /(RC+dt);
	y = ylast + alpha *(x-ylast);
	ylast = y;
	return y;
}//
//

//
float HPF(float x, float cutoff, float sample_rate)
{
	float RC = 1.0/(cutoff * 2 * 3.14);
	float dt = 1.0/sample_rate;
	float alpha = RC/(RC+dt);
	float y;
	static float xlast = 0, ylast = 0;
	ylast = y;
	xlast = x;
	return y;
}*/
//

//
float kalman_single(float z, float measure_noise, float process_noise)
{
	const float R = measure_noise * measure_noise;
	const float Q = process_noise * process_noise;
	static float x_hat, P;
	float P_, K;
	
	
	
	/*********Noi suy kalman *********************/
	P_ = P + Q;							//P_ = A*P*P' +Q
	K = P_ / (P + R);				// K = P_*H'*inv(H*P_*H'  +R
	x_hat = x_hat + K*(z-x_hat);
	P = ( 1 - K) *P_ ;
	
	/**********************************************/
	return x_hat;
}//
//
void kalman( float *in, float *out, float measure_noise, float process_noise)
{
	float R[3][3] = {0}, Q[3][3] = {0};
	
	const float B[3][3] = {
													{1,0,0},
													{0,1,0},
													{0,0,1}	
	};
	const float I[3][3] = {
													{1,0,0},
													{0,1,0},
													{0,0,1}	
	};
	
	static float x_hat[3][1];
	static float P[3][3];
	float z[3][1];
	float x_hat_[3][1];
	float P_[3][3];
	float K[3][3];
	float P_R1[3][3], P_R2[3][3];
	float z_x_hat_[3][1], K_z_x_hat_[3][1];
	float I_K[3][3];
	
	R[0][0] = measure_noise * measure_noise;
	R[1][1] = measure_noise * measure_noise;
	R[2][2] = measure_noise * measure_noise;
	Q[0][0] = measure_noise * measure_noise;
	Q[1][1] = measure_noise * measure_noise;
	Q[2][2] = measure_noise * measure_noise;
	
	matrix_transpose((float*)in, 1,3, (float*)z);
	
	matrix_multiply((float*)B, (float*)x_hat, 3,3,1, (float*)x_hat_);
	
	matrix_addition((float*)P, (float*)Q, 3,3, (float*)P_);
	
	matrix_addition((float*)P_, (float*)R, 3,3, (float*)P_R1);
	
	matrix_inversion((float*)P_R1, 3, (float*)P_R2);
	
	matrix_multiply((float*)P_, (float*)P_R2, 3,3,3, (float*)K);
//-------------------------------------------------------------------
	
	matrix_subtraction((float*)z, (float*)x_hat_, 3,1, (float*)z_x_hat_);
	
	matrix_multiply((float*)K, (float*)z_x_hat_, 3,3,1, (float*)K_z_x_hat_);
	
	matrix_addition((float*)x_hat, (float*)K_z_x_hat_, 3,1, (float*)x_hat);
	//--------------------------------------------------------------------
	
	matrix_subtraction((float*)I, (float*)K, 3,3, (float*)I_K);
	matrix_multiply((float*)I_K, (float*)P_, 3,3,3, (float*)P);
	
//---------------------------------------------------------------------

	matrix_transpose((float*)x_hat, 3,1, (float*)out);
}
//



//

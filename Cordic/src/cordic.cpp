#include "cordic.h"

void cordic(float theta_in, float &s_in, float &c_in)
{

#pragma HLS INTERFACE s_axilite port=s_in
#pragma HLS INTERFACE s_axilite port=c_in
//#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE s_axilite port=theta_in
	THETA_TYPE theta = theta_in;

  // Set the initial vector that we will rotate
  // current_cos = I; current_sin = Q
  COS_SIN_TYPE current_cos = 0.60735;
  COS_SIN_TYPE current_sin = 0.0;

  // This loop iteratively rotates the initial vector to find the
  // sine and cosine values corresponding to the input theta angle
  for (int j = 0; j < NUM_ITERATIONS; j++) {
#pragma HLS PIPELINE
      // Multiply previous iteration by 2^(-j).  This is equivalent to
      // a right shift by j on a fixed-point number.
      COS_SIN_TYPE cos_shift = current_cos >> j;
      COS_SIN_TYPE sin_shift = current_sin >> j;

    // Determine if we are rotating by a positive or negative angle
    if(theta >= 0) {
        // Perform the rotation
        current_cos = current_cos - sin_shift;
        current_sin = current_sin + cos_shift;

        // Determine the new theta
        theta = theta - cordic_phase[j];
    }
	else {
        // Perform the rotation
        current_cos = current_cos + sin_shift;
        current_sin = current_sin - cos_shift;

        // Determine the new theta
        theta = theta + cordic_phase[j];
    }
  }

  // Set the final sine and cosine values
  s_in = current_sin;  c_in = current_cos;
}



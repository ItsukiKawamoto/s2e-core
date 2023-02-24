/**
 * @file relative_orbit_models.hpp
 * @brief Functions for relative orbit
 */

#ifndef S2E_LIBRARY_ORBIT_RELATIVE_ORBIT_MODEL_HPP_
#define S2E_LIBRARY_ORBIT_RELATIVE_ORBIT_MODEL_HPP_

#include "../math/matrix.hpp"
#include "../math/vector.hpp"

/**
 * @enum RelativeOrbitModel
 * @brief Relative orbit model
 */
enum class RelativeOrbitModel { Hill = 0 };

/**
 * @enum StmModel
 * @brief State Transition Matrix for the relative orbit
 */
enum class StmModel { HCW = 0 };

// Dynamics Models
/**
 * @fn CalculateHillSystemMatrix
 * @brief Calculate Hill System Matrix
 * @param [in] orbit_radius: Orbit radius [m]
 * @param [in] mu: Gravity constant of the center body [m3/s2]
 * @return System matrix
 */
libra::Matrix<6, 6> CalculateHillSystemMatrix(double orbit_radius, double mu);

// STMs
/**
 * @fn CalculateHCWSTM
 * @brief Calculate HCW State Transition Matrix
 * @param [in] orbit_radius: Orbit radius [m]
 * @param [in] mu: Gravity constant of the center body [m3/s2]
 * @param [in] mu: Elapsed time [s]
 * @return State Transition Matrix
 */
libra::Matrix<6, 6> CalculateHCWSTM(double orbit_radius, double mu, double elapsed_sec);

#endif  // S2E_LIBRARY_ORBIT_RELATIVE_ORBIT_MODEL_HPP_

#ifndef A32_MOTION_H
#define A32_MOTION_H

#include <stddef.h>
#include <stdint.h>

/**
 * A motion object.
 */
typedef struct {
  /**
   * The configured maximum velocity in m/t.
   *
   * @note: Must be configured before calling update.
   */
  float max_velocity;

  /**
   * The configured maximum acceleration in m/t^2.
   *
   * @note: Must be configured before calling update.
   */
  float max_acceleration;

  /**
   * The current position. This position should be approached next after an update. The position can be modified with
   * the real world position to compensate slow/fast movements.
   */
  float position;

  /**
   * The last position. Can be used to calculate the change by subtracting it from position.
   */
  float last_position;

  /**
   * The current velocity.
   */
  float velocity;

  /**
   * The last velocity. Can be used to calculate the change by subtracting it from velocity.
   */
  float last_velocity;

  /**
   * The current acceleration.
   */
  float acceleration;
} a32_motion_t;

/**
 * Constructs and returns a motion object with the specified maximum velocity and acceleration.
 *
 * @param max_velocity The maximum velocity.
 * @param max_acceleration The maximum acceleration.
 * @return A motion object.
 */
a32_motion_t a32_motion_make(float max_velocity, float max_acceleration);

/**
 * Update the motion profile.
 *
 * The algorithm will calculate the next position to approach in order to reach the provided target.
 *
 * @param m The motion object.
 * @param target The target to approach.
 * @param delta The time delta from now to the position to calculate.
 */
void a32_motion_update(a32_motion_t *m, float target, float delta);

#endif  // A32_MOTION_H

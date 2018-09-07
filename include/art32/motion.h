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
  double max_velocity;

  /**
   * The configured maximum acceleration in m/t^2.
   *
   * @note: Must be configured before calling update.
   */
  double max_acceleration;

  /**
   * The current position. This position should be approached next after an update. The position can be modified with
   * the real world position to compensate slow/fast movements.
   */
  double position;

  /**
   * The last position. Can be used to calculate the change by subtracting it from position.
   */
  double last_position;

  /**
   * The current velocity.
   */
  double velocity;

  /**
   * The last velocity. Can be used to calculate the change by subtracting it from velocity.
   */
  double last_velocity;

  /**
   * The current acceleration.
   */
  double acceleration;
} a32_motion_t;

/**
 * Update the motion profile.
 *
 * The algorithm will calculate the next position to approach in order to reach the provided target.
 *
 * @param m The motion object.
 * @param target The target to approach.
 * @param delta The time delta from now to the position to calculate.
 */
void a32_motion_update(a32_motion_t *m, double target, uint32_t delta);

#endif  // A32_MOTION_H

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
   */
  double max_velocity;

  /**
   * The configured maximum acceleration in m/t^2.
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

  /**
   * The time of the last update.
   */
  uint32_t last_update;
} a32_motion_t;

/**
 * Initialize a motion object.
 *
 * @param m The motion object.
 * @param max_velocity Max velocity in m/t.
 * @param max_acceleration Max acceleration in m/t^2.
 */
void a32_motion_init(a32_motion_t *m, double max_velocity, double max_acceleration);

/**
 * Update the motion profile.
 *
 * The algorithm will calculate the next position to approach in order to reach the provided target.
 *
 * @param m The motion object.
 * @param target The target to approach.
 * @param time The current time.
 */
void a32_moption_update(a32_motion_t *m, double target, uint32_t time);

#endif  // A32_MOTION_H

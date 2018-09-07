#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <art32/motion.h>

// Adapted from https://github.com/WRidder/MotionProfileGenerator.

static void motion_calculate(a32_motion_t *m, double target, uint32_t delta) {
  // check if we should de-accelerate
  if ((pow(m->velocity, 2) / m->max_acceleration) / 2 >= fabs(target - m->position)) {
    // perform de-acceleration
    if (m->velocity < 0) {
      m->position = (m->velocity + m->max_acceleration * delta) * delta + m->position;
    } else if (m->velocity > 0) {
      m->position = (m->velocity - m->max_acceleration * delta) * delta + m->position;
    }

    return;
  }

  // check if we should accelerate
  if ((fabs(m->velocity) < m->max_velocity || (target < m->position && m->velocity > 0) ||
       (target > m->position && m->velocity < 0))) {
    // accelerate up to max acceleration
    if (target > m->position) {
      double speed = fmin(m->velocity + m->max_acceleration * delta, m->max_velocity);
      m->position = speed * delta + m->position;
    } else {
      double speed = fmax(m->velocity - m->max_acceleration * delta, -m->max_velocity);
      m->position = speed * delta + m->position;
    }

    return;
  }

  // keep velocity constant
  if (target > m->position) {
    m->position = m->max_velocity * delta + m->position;
  } else {
    m->position = -m->max_velocity * delta + m->position;
  }
}

void a32_motion_update(a32_motion_t *m, double target, uint32_t delta) {
  // copy state
  m->last_position = m->position;
  m->last_velocity = m->velocity;

  // calculate motion profile
  motion_calculate(m, target, delta);

  // update velocity
  m->velocity = (m->position - m->last_position) / delta;
  if (m->velocity != m->velocity) {
    m->velocity = 0;
  }

  // update acceleration
  m->acceleration = (m->velocity - m->last_velocity) / delta;
  if (m->acceleration != m->acceleration) {
    m->acceleration = 0;
  }
}

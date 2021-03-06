"""Tests for openai interface."""
import numpy as np
import gym
import scrimmage


def test_openai():
    """Open single entity scenario and make sure it banks."""
    try:
        env = gym.make('scrimmage-v0')
    except gym.error.Error:
        mission_file = scrimmage.find_mission('external_control.xml')

        gym.envs.register(
            id='scrimmage-v0',
            entry_point='scrimmage.external_control:ScrimmageEnv',
            max_episode_steps=1e9,
            reward_threshold=1e9,
            kwargs={"enable_gui": False, "mission_file": mission_file}
        )
        env = gym.make('scrimmage-v0')

    ROLL_IDX = 6
    YAW_IDX = 8

    # the observation is the state of the aircraft
    obs = env.reset()
    done = False
    tot_reward = 0
    while not done:
        action = env.action_space.sample()

        # the action is the desired state of the aircraft
        # we tell it to turn hard left
        action = obs
        action[YAW_IDX] += np.pi / 2

        obs, reward, done = env.step(action)[:3]
        tot_reward += reward

    env.env.close()
    ROLL_LIMIT = np.radians(30)
    assert tot_reward == 0
    assert abs(obs[ROLL_IDX]) > 0.5 * ROLL_LIMIT

if __name__ == '__main__':
    test_openai()

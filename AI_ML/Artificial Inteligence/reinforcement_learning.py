import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import random

# Gridworld environment
class GridWorld:
    def __init__(self, size=5, goal=(4, 4), obstacles=[(1, 1), (2, 2)], start=(0, 0)):
        self.size = size
        self.goal = goal
        self.start = start
        self.obstacles = set(obstacles)
        self.reset()

    def reset(self):
        self.agent_pos = self.start
        return self.agent_pos

    def step(self, action):
        x, y = self.agent_pos
        if action == 0: x -= 1  # up
        elif action == 1: x += 1  # down
        elif action == 2: y -= 1  # left
        elif action == 3: y += 1  # right

        x = max(0, min(self.size - 1, x))
        y = max(0, min(self.size - 1, y))

        if (x, y) in self.obstacles:
            x, y = self.agent_pos

        self.agent_pos = (x, y)
        reward = 1 if self.agent_pos == self.goal else -0.1
        done = self.agent_pos == self.goal
        return self.agent_pos, reward, done

# Q-Learning Algorithm
class QLearningAgent:
    def __init__(self, env, alpha=0.1, gamma=0.9, epsilon=0.2):
        self.env = env
        self.alpha = alpha
        self.gamma = gamma
        self.epsilon = epsilon
        self.q_table = np.zeros((env.size, env.size, 4))

    def choose_action(self, state):
        if random.random() < self.epsilon:
            return random.randint(0, 3)
        else:
            x, y = state
            return np.argmax(self.q_table[x, y])

    def step_learn(self):
        state = self.env.reset()
        done = False
        while not done:
            action = self.choose_action(state)
            next_state, reward, done = self.env.step(action)
            x, y = state
            nx, ny = next_state

            td_target = reward + self.gamma * np.max(self.q_table[nx, ny])
            td_error = td_target - self.q_table[x, y, action]
            self.q_table[x, y, action] += self.alpha * td_error
            state = next_state

    def get_policy(self):
        policy = np.full((self.env.size, self.env.size), -1)
        for x in range(self.env.size):
            for y in range(self.env.size):
                if (x, y) not in self.env.obstacles:
                    policy[x, y] = np.argmax(self.q_table[x, y])
        return policy

# Render arrows on grid
arrow_map = {0: '↑', 1: '↓', 2: '←', 3: '→', -1: ' '}
def render_policy(policy, ax, env):
    ax.clear()
    ax.set_title("Policy Visualization")
    ax.set_xticks(np.arange(-.5, env.size, 1), minor=True)
    ax.set_yticks(np.arange(-.5, env.size, 1), minor=True)
    ax.grid(which='minor', color='gray', linestyle='-', linewidth=0.5)
    ax.set_xticks([])
    ax.set_yticks([])

    for x in range(env.size):
        for y in range(env.size):
            if (x, y) in env.obstacles:
                ax.add_patch(plt.Rectangle((y, x), 1, 1, color='black'))
            elif (x, y) == env.goal:
                ax.add_patch(plt.Rectangle((y, x), 1, 1, color='red'))
            else:
                action = policy[x, y]
                if action != -1:
                    ax.text(y + 0.5, x + 0.5, arrow_map[action], ha='center', va='center', fontsize=16)

    ax.set_xlim(0, env.size)
    ax.set_ylim(0, env.size)
    ax.invert_yaxis()

# Main animation logic
if __name__ == "__main__":
    env = GridWorld()
    agent = QLearningAgent(env)

    fig, ax = plt.subplots()

    def update(frame):
        for _ in range(5):  # train for a few episodes per frame
            agent.step_learn()
        policy = agent.get_policy()
        render_policy(policy, ax, env)
        ax.set_title(f"Episode {frame * 5}")

    # stop after 50 frames
    ani = animation.FuncAnimation(fig, update, frames=50, repeat=False, interval=300)
    plt.show()
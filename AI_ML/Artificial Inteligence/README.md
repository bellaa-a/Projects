# Reinforcement Learning GridWorld Q-Learning Visualizer

This project visualizes the Q-Learning process in a GridWorld environment using Python and Matplotlib. The goal is to show how the agent learns to navigate the grid and find the optimal policy to reach the goal while avoiding obstacles.

## Overview of the Animation

The animation demonstrates the learning process of an agent performing Q-Learning to navigate a GridWorld. The key components of the animation are:

### **1. GridWorld Environment:**
- **Grid**: The grid is 5x5 (default size) with obstacles placed at specific coordinates.
- **Agent**: The agent starts at the top-left corner (0, 0) and aims to reach the goal at the bottom-right (4, 4).
- **Obstacles**: The agent cannot move through obstacles, which are placed at specific grid positions.
- **Goal**: The agent aims to reach the goal at (4, 4), which provides a reward of +1. Moving to any other grid cell gives a small penalty of -0.1.

### **2. Q-Learning Algorithm:**
- **Exploration vs. Exploitation**: The agent chooses an action based on an exploration-exploitation tradeoff. In the early stages, the agent explores the grid randomly, while later it exploits learned knowledge to follow the optimal path.
- **Q-Table**: The agent maintains a Q-table that stores the expected future rewards for each state-action pair. The agent updates the Q-table using the Q-learning update rule after each action.
- **Learning Process**: The agent improves its policy (the set of actions it takes at each state) over time as it learns from its interactions with the environment.

### **3. Animation Details:**
- **Arrows**: The animation shows arrows (↑, ↓, ←, →) on the grid to indicate the optimal action that the agent will take from each grid cell. These arrows represent the agent's learned policy.
  - **↑ (Up)**: Move up.
  - **↓ (Down)**: Move down.
  - **← (Left)**: Move left.
  - **→ (Right)**: Move right.
- **Learning Progress**: As the agent learns over time, the arrows on the grid improve to reflect the optimal path toward the goal.
- **Episodes**: The animation runs for 50 frames (representing 250 episodes). In each frame, the agent undergoes 5 learning episodes. This allows us to see the evolution of the agent's policy as it improves over time.

### **4. Stopping Condition:**
- The animation stops after 50 frames (or 250 learning episodes). During each frame, the agent takes actions and updates its Q-table based on rewards. Over time, the agent converges to an optimal set of actions to reach the goal.

### **5. The Grid Layout:**
- **Obstacles**: Black cells represent obstacles where the agent cannot move.
- **Goal**: The goal is marked in red.
- **Agent**: The agent’s current position is marked by a blue square.

## How the Animation Works

- Each frame in the animation corresponds to several episodes of learning.
- During each episode, the agent interacts with the environment, chooses actions (either randomly or based on its learned policy), and updates its Q-values.
- The animation shows how the agent’s policy evolves as it learns to navigate toward the goal efficiently.

## Requirements

- Python 3.x
- NumPy
- Matplotlib

You can install the required libraries using pip:
pip install numpy matplotlib

## Running the Visualizer

1. Clone this repository:
git clone https://github.com/bellaa-a/AI-reinforcement-learning.git

2. Navigate to the project directory:
cd AI-reinforcement-learning

3. Run the visualization script:
python reinforcement_learning.py

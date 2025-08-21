from core import * # replaced with python 3 alternative
import pandas as pd
from tabulate import tabulate

arm1 = BernoulliArm(0.7)
arm1.draw()
arm1.draw()

arm2 = NormalArm(10.0, 1.0)
arm2.draw()
arm2.draw()

arm3 = BernoulliArm(0.2)
arm3.draw()
arm3.draw()

arms = [arm1, arm2, arm3]

n_arms = len(arms)

algo1 = EpsilonGreedy(0.1, [], [])
algo2 = Softmax(1.0, [], [])
algo3 = UCB1([], [])
algo4 = Exp3(0.2, [])

algos = [algo1, algo2, algo3, algo4]

for algo in algos:
  algo.initialize(n_arms)

for t in range(1000):
  for algo in algos:
    chosen_arm = algo.select_arm()
    reward = arms[chosen_arm].draw()
    algo.update(chosen_arm, reward)

algo1.counts
algo1.values

algo2.counts
algo2.values

algo3.counts
algo3.values

algo4.weights

num_sims = 1000
horizon = 10
results = test_algorithm(algo1, arms, num_sims, horizon) # switch algo here

# plot results
import matplotlib.pyplot as plt
import numpy as np

# Unpack results
sim_nums, times, chosen_arms, rewards, cumulative_rewards = results

# Reshape cumulative_rewards into a 2D array [num_sims x horizon]
cum_rewards_2d = np.array(cumulative_rewards).reshape(num_sims, horizon)

# Plot mean cumulative reward across simulations
mean_cum_reward = np.mean(cum_rewards_2d, axis=0)
plt.plot(range(1, horizon + 1), mean_cum_reward, 'b-', label="EpsilonGreedy (ε=0.1)")
plt.xlabel("Time Step")
plt.ylabel("Cumulative Reward")
plt.title("Multi-Armed Bandit Algorithm Performance")
plt.legend()
plt.show()

# table results
# Convert results to a pandas DataFrame
sim_nums, times, chosen_arms, rewards, cumulative_rewards = results
df = pd.DataFrame({
    'Simulation': sim_nums,
    'Time Step': times,
    'Chosen Arm': chosen_arms,
    'Reward': rewards,
    'Cumulative Reward': cumulative_rewards
})

# Generate summary statistics
summary = df.groupby('Time Step').agg({
    'Chosen Arm': lambda x: pd.Series.mode(x)[0],  # Most frequently chosen arm
    'Reward': 'mean',
    'Cumulative Reward': 'mean'
}).reset_index()

# Print full results (first 20 rows)
print("=== Raw Results (First 20 Rows) ===")
print(tabulate(df.head(20), headers='keys', tablefmt='grid'))

# Print summary table
print("\n=== Performance Summary ===")
print(tabulate(summary, headers='keys', tablefmt='grid', showindex=False))

# Additional statistics
print(f"\nKey Insights:")
print(f"- Average reward per step: {df['Reward'].mean():.3f}")
print(f"- Most explored arm: Arm {df['Chosen Arm'].mode()[0]}")
print(f"- Total cumulative reward: {df['Cumulative Reward'].max():.1f}")
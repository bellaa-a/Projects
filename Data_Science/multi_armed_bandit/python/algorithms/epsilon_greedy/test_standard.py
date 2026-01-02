import sys
from pathlib import Path
sys.path.append(str(Path(__file__).parent.parent.parent))  # Go up 3 levels
import numpy as np
from core import *
import random

def run_standard_epsilon(seed=1, num_sim=10, horizon=500, num_arms=5, epsilon=0.1, arm_type="bernoulli"):
    random.seed(seed)
    if arm_type == "bernoulli":
        arms, probs = setup_bernoulli_arms(num_arms, horizon)
        actual_best_arm = ind_max(probs)
        best_arm_prob = probs[actual_best_arm]
    elif arm_type == "normal":
        arms, means = setup_normal_arms(num_arms, horizon)
        actual_best_arm = ind_max(means)
        best_arm_prob = means[actual_best_arm]

    # Initialize algorithm
    algo = EpsilonGreedy(epsilon, [], [])
    algo.initialize(num_arms)
    results = test_algorithm(algo, arms, num_sim, horizon)

    # Track total regret across simulations
    total_regret_pct = 0.0

    # Track arm selections (to find the most picked arm)
    arm_counts = [0] * num_arms  # Initialize counts for each arm

    # Open files for writing 
    with open("algorithms/epsilon_greedy/standard_results.csv", "w") as f_detail, \
         open("algorithms/epsilon_greedy/standard_regret.csv", "w") as f_summary:

        # Write headers
        f_detail.write("e\tsim\tt\tarm\treward\tcum_reward\n")
        f_summary.write("e\tsim\treg_pct\n")

        for sim in range(1, num_sim + 1):
            # Filter results for current simulation
            sim_mask = [s == sim for s in results[0]]
            optimal_reward = best_arm_prob * sum(sim_mask)
            actual_reward = sum(r for r, m in zip(results[3], sim_mask) if m)
            regret_pct = max(0, (optimal_reward - actual_reward) / optimal_reward * 100)
            total_regret_pct += regret_pct

            # Write to summary file
            f_summary.write(f"{epsilon}\t{sim}\t{regret_pct:.2f}\n")

            # Track arm selections for this simulation
            for i in range(len(results[0])):
                if results[0][i] == sim:
                    chosen_arm = results[2][i]  # Arm chosen at step i
                    arm_counts[chosen_arm] += 1  # Increment count for this arm
                    f_detail.write(f"{epsilon}\t")
                    f_detail.write("\t".join([str(results[j][i]) for j in range(len(results))]))
                    f_detail.write("\n")

    # Calculate average regret percentage
    avg_regret_pct = total_regret_pct / num_sim

    # Find the most and second-most frequently picked arms
    sorted_counts = sorted([(count, arm) for arm, count in enumerate(arm_counts)], reverse=True)
    best_arm = sorted_counts[0][1]
    best_count = sorted_counts[0][0]
    second_best_count = sorted_counts[1][0] if num_arms > 1 else 0

    # Calculate confidence percentage (how much better best arm is than 2nd best)
    if second_best_count == 0:
        confidence_pct = 100.0  # Avoid division by zero if only one arm
    else:
        confidence_pct = ((best_count - second_best_count) / best_count) * 100

    return avg_regret_pct, best_arm, confidence_pct, actual_best_arm
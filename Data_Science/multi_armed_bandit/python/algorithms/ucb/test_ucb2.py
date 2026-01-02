# explores in phases; i.e. AAA BBBB CC 
import sys
from pathlib import Path
sys.path.append(str(Path(__file__).parent.parent.parent))
import numpy as np
from core import *
import random
import os

def run_ucb2(seed=1, num_sim=10, horizon=500, num_arms=5, alpha=0.1, arm_type="bernoulli"):
    random.seed(seed)
    if arm_type == "bernoulli":
        arms, probs = setup_bernoulli_arms(num_arms, horizon)
        actual_best_arm = ind_max(probs)
        best_arm_prob = probs[actual_best_arm]
    elif arm_type == "normal":
        arms, means = setup_normal_arms(num_arms, horizon)
        actual_best_arm = ind_max(means)
        best_arm_prob = means[actual_best_arm]

    total_regret_pct = 0.0
    arm_counts = [0] * num_arms  # Track arm selections during phases
    
    # Ensure output directory exists
    os.makedirs("algorithms/ucb", exist_ok=True)
    
    with open(f"algorithms/ucb/ucb2_alpha{alpha}_results.csv", "w") as f_detail, \
         open(f"algorithms/ucb/ucb2_alpha{alpha}_regret.csv", "w") as f_summary:

        # Write headers
        f_detail.write("sim\tt\tarm\treward\tcum_reward\n")
        f_summary.write("sim\treg_pct\n")

        # Run algorithm
        algo = UCB2(alpha, [], [])
        algo.initialize(num_arms)
        results = test_algorithm(algo, arms, num_sim, horizon)
        
        # Process results in single pass
        for i in range(len(results[0])):
            # Track arm selection (counting phase plays)
            chosen_arm = results[2][i]
            arm_counts[chosen_arm] += 1
            
            # Write detailed results
            f_detail.write(f"{results[0][i]}\t{results[1][i]}\t{results[2][i]}\t{results[3][i]}\t{results[4][i]}\n")

        # Calculate regret per simulation
        for sim in range(1, num_sim + 1):
            sim_mask = [s == sim for s in results[0]]
            optimal_reward = best_arm_prob * sum(sim_mask)
            actual_reward = sum(r for r, m in zip(results[3], sim_mask) if m)
            regret_pct = max(0, (optimal_reward - actual_reward) / optimal_reward * 100)
            total_regret_pct += regret_pct
            f_summary.write(f"{sim}\t{regret_pct:.2f}\n")

    # Calculate metrics
    avg_regret_pct = total_regret_pct / num_sim
    
    # Find best arm and confidence (based on phase counts)
    sorted_counts = sorted([(count, arm) for arm, count in enumerate(arm_counts)], reverse=True)
    best_arm = sorted_counts[0][1]
    best_count = sorted_counts[0][0]
    second_best_count = sorted_counts[1][0] if num_arms > 1 else 0
    
    confidence_pct = 100.0 if second_best_count == 0 else \
                   ((best_count - second_best_count) / best_count) * 100

    return avg_regret_pct, best_arm, confidence_pct, actual_best_arm
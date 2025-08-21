from algorithms.epsilon_greedy.test_standard import *
from algorithms.epsilon_greedy.test_annealing import *
from algorithms.ucb.test_ucb1 import *
from algorithms.ucb.test_ucb2 import *
from algorithms.thompson.test_thompson import *
import pandas as pd
import time

# Define all test configurations
configs = [
    {'name': 'Baseline (2:200)', 'num_arms': 2, 'horizon': 200},
    # {'name': 'Small Problem (5:500)', 'num_arms': 5, 'horizon': 500},
    # {'name': 'Few Arms/Long Horizon (5:10,000)', 'num_arms': 5, 'horizon': 10000},
    # {'name': 'Medium Problem (10:2,000)', 'num_arms': 10, 'horizon': 2000},
    # {'name': 'Many Arms/Short Horizon (50:1,000)', 'num_arms': 50, 'horizon': 1000}
]

# Algorithm parameters
epsilons = [0.1, 0.2, 0.3, 0.4, 0.5]
alphas_ucb2 = [0.1, 0.3, 0.5, 0.7, 0.9]
seed = 1
num_sim = 100
arm_type = "bernoulli"  # "normal" or "bernoulli"
if arm_type == "normal":
    ts_prior = "NormalGamma(μ₀=0, λ=1, α=1, β=1)"
elif arm_type == "bernoulli":
    ts_prior = "beta(1,1)"

# Open output file with UTF-8 encoding
output_filename = f"{arm_type}_output.txt"
with open(output_filename, 'w', encoding='utf-8') as f_out:
    for config in configs:
        results = []
        num_arms = config['num_arms']
        horizon = config['horizon']
        
        # Write header
        header = f"\n=== Running {config['name']} ==="
        f_out.write(header + "\n")
        print(header)
        
        # ε-Greedy (Standard)
        for eps in epsilons:
            start_time = time.time()  # Start timer
            regret, best_arm, confidence, actual_best_arm = run_standard_epsilon(
                seed=seed, num_sim=num_sim, 
                horizon=horizon, num_arms=num_arms, 
                epsilon=eps,
                arm_type=arm_type
            )
            elapsed_time = time.time() - start_time  # End timer
            results.append({
                'Algorithm': 'ε-Greedy (Standard)',
                'Param': f'ε={eps}',
                'Regret%': regret,
                'Found': best_arm,
                'Best': actual_best_arm,
                'Conf%': confidence,
                'Time (s)': f"{elapsed_time:.4f}"  # Add time column
            })
        
        # ε-Greedy (Annealing)
        start_time = time.time()
        regret, best_arm, confidence, actual_best_arm = run_annealing_epsilon(
            seed=seed, num_sim=num_sim, 
            horizon=horizon, num_arms=num_arms,
            arm_type=arm_type
        )
        elapsed_time = time.time() - start_time
        results.append({
            'Algorithm': 'ε-Greedy (Annealing)',
            'Param': 'N/A',
            'Regret%': regret,
            'Found': best_arm,
            'Best': actual_best_arm,
            'Conf%': confidence,
            'Time (s)': f"{elapsed_time:.4f}"
        })
        
        # UCB1
        start_time = time.time()
        regret, best_arm, confidence, actual_best_arm = run_ucb1(
            seed=seed, num_sim=num_sim, 
            horizon=horizon, num_arms=num_arms,
            arm_type=arm_type
        )
        elapsed_time = time.time() - start_time
        results.append({
            'Algorithm': 'UCB1',
            'Param': 'N/A',
            'Regret%': regret,
            'Found': best_arm,
            'Best': actual_best_arm,
            'Conf%': confidence,
            'Time (s)': f"{elapsed_time:.4f}"
        })
        
        # UCB2
        for alpha in alphas_ucb2:
            start_time = time.time()
            regret, best_arm, confidence, actual_best_arm = run_ucb2(
                seed=seed, num_sim=num_sim, 
                horizon=horizon, num_arms=num_arms, 
                alpha=alpha,
                arm_type=arm_type
            )
            elapsed_time = time.time() - start_time
            results.append({
                'Algorithm': 'UCB2',
                'Param': f'α={alpha}',
                'Regret%': regret,
                'Found': best_arm,
                'Best': actual_best_arm,
                'Conf%': confidence,
                'Time (s)': f"{elapsed_time:.4f}"
            })
        
        # Thompson Sampling
        start_time = time.time()
        regret, best_arm, confidence, actual_best_arm = run_thompson(
            seed=seed, num_sim=num_sim, 
            horizon=horizon, num_arms=num_arms,
            arm_type=arm_type
        )
        elapsed_time = time.time() - start_time
        results.append({
            'Algorithm': 'Thompson',
            'Param': ts_prior,
            'Regret%': regret,
            'Found': best_arm,
            'Best': actual_best_arm,
            'Conf%': confidence,
            'Time (s)': f"{elapsed_time:.4f}"
        })
        
        # Create DataFrame
        df = pd.DataFrame(results)
        
        # Write to file
        f_out.write(f"\nResults for {config['name']} (arms={num_arms}, horizon={horizon}):\n")
        f_out.write(df.to_markdown(index=False, floatfmt=".2f"))
        f_out.write("\n\n" + "="*100 + "\n")
        
        # Print to console
        print(f"\nResults for {config['name']} (arms={num_arms}, horizon={horizon}):")
        print(df.to_markdown(index=False, floatfmt=".2f"))
        print("\n" + "="*100 + "\n")

print(f"\nAll results saved to {output_filename}")
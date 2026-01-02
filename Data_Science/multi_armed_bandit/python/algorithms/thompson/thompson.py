import numpy as np
from scipy.stats import beta, norm
from core import ind_max

class ThompsonSampling:
    def __init__(self, counts, values, arm_type="bernoulli"):
        self.counts = counts  # Number of pulls per arm
        self.values = values  # Observed rewards
        self.arm_type = arm_type.lower()
        
        # Initialize priors based on arm type
        if self.arm_type == "bernoulli":
            self.alpha = [1] * len(counts)  # Beta prior (successes + 1)
            self.beta = [1] * len(counts)   # Beta prior (failures + 1)
        elif self.arm_type == "normal":
            # Normal-Gamma prior (μ ~ N(μ0, 1/(λτ)), τ ~ Gamma(α, β)
            self.mu_0 = [0] * len(counts)    # Prior mean
            self.lambda_ = [1] * len(counts)  # Precision scaling
            self.alpha_norm = [1] * len(counts)  # Shape parameter
            self.beta_norm = [1] * len(counts)   # Rate parameter
        else:
            raise ValueError("arm_type must be either 'bernoulli' or 'normal'")
    
    def initialize(self, n_arms):
        self.counts = [0] * n_arms
        self.values = [0.0] * n_arms
        
        if self.arm_type == "bernoulli":
            self.alpha = [1] * n_arms  # Uniform Beta(1,1) prior
            self.beta = [1] * n_arms
        else:  # normal
            self.mu_0 = [0] * n_arms      # Prior mean
            self.lambda_ = [1] * n_arms   # Precision scaling
            self.alpha_norm = [1] * n_arms  # Gamma shape
            self.beta_norm = [1] * n_arms   # Gamma rate
    
    def select_arm(self):
        if self.arm_type == "bernoulli":
            # Sample from Beta distributions for Bernoulli arms
            theta_samples = [beta.rvs(a, b) for a, b in zip(self.alpha, self.beta)]
        else:
            # Sample from Normal distributions for Normal arms
            theta_samples = []
            for i in range(len(self.counts)):
                if self.counts[i] == 0:
                    # Use prior if no data
                    theta_samples.append(norm.rvs(loc=self.mu_0[i], 
                                                scale=1/np.sqrt(self.lambda_[i])))
                else:
                    # Posterior parameters for Normal arms
                    post_var = 1/(self.lambda_[i] * self.alpha_norm[i]/self.beta_norm[i])
                    theta_samples.append(norm.rvs(loc=self.mu_0[i], 
                                                scale=np.sqrt(post_var)))
        return ind_max(theta_samples)
    
    def update(self, chosen_arm, reward):
        self.counts[chosen_arm] += 1
        n = self.counts[chosen_arm]
        
        # Update empirical mean
        self.values[chosen_arm] = ((n - 1) / n) * self.values[chosen_arm] + (1 / n) * reward
        
        if self.arm_type == "bernoulli":
            # Update Beta parameters for Bernoulli arms
            if reward == 1:
                self.alpha[chosen_arm] += 1
            else:
                self.beta[chosen_arm] += 1
        else:
            # Update Normal-Gamma parameters for Normal arms
            old_mu = self.mu_0[chosen_arm]
            old_lambda = self.lambda_[chosen_arm]
            old_alpha = self.alpha_norm[chosen_arm]
            old_beta = self.beta_norm[chosen_arm]
            
            # Update parameters (conjugate prior update)
            new_lambda = old_lambda + 1
            new_mu = (old_lambda * old_mu + reward) / new_lambda
            new_alpha = old_alpha + 0.5
            new_beta = old_beta + 0.5 * old_lambda * (reward - old_mu)**2 / new_lambda
            
            self.mu_0[chosen_arm] = new_mu
            self.lambda_[chosen_arm] = new_lambda
            self.alpha_norm[chosen_arm] = new_alpha
            self.beta_norm[chosen_arm] = new_beta
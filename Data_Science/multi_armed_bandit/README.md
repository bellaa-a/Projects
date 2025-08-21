# multi_armed_bandit

This repo contains code in several languages that implements several standard algorithms for solving the Multi-Armed Bandits Problem, including:

* epsilon-Greedy
* Softmax (Boltzmann)
* UCB1
* UCB2
* Exp3
* Thompson Sampling

It also contains code that provides a testing framework for bandit algorithms based around simple Monte Carlo simulations.

# Languages

This codebase is split up by language. In most languages, there are parallel implementations of the core algorithms and infrastructure for testing the algorithms:

* Python
* Ruby

In R, there is a body of code for visualizing the results of simulations and analyzing those results. The R code would benefit from some refactoring to make it DRYer.

# Getting Started

To try out this code, you can go into the Python directories and then run the demo script.

In Python, that looks like:

    python demo.py
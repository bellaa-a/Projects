# Xuanyu (Bella) Lu – Projects Portfolio

Welcome to my projects repository! This collection showcases my work in **AI/ML, Deep Learning, Algorithms, C/C++, and Computer Systems**. Each project demonstrates my technical skills, problem-solving abilities, and experience collaborating on complex, real-world tasks.

---

## Table of Contents
- [Game Development](#game-development)
- [AI & Machine Learning](#ai--machine-learning)
- [Deep Learning](#deep-learning)
- [Algorithms](#algorithms)
- [C / C++ Projects](#c--c-projects)
- [Computer Systems](#computer-systems)
- [Data Science](#data-science)
- [Hackathon Project](#hackathon-project)
- [Data Structures (Java)](#data-structures-java)
- [MATLAB Projects](#matlab-projects)
- [UI/UX & Fullstack](#uiux--fullstack)
- [Deepmreye (Brain Sciences Lab Project)](#deepmreye-brain-sciences-lab-project)

---
## Game Development

**Marble Run – Physics-Based Puzzle Game (Godot)**

An original puzzle game developed in the Godot Engine where players design courses by strategically placing and rotating blocks to guide a marble to its goal. The project combines physics simulation, puzzle design, multiplayer networking, and custom game systems, and is being prepared for release on Steam.

### Key Features
- Physics-based gameplay using rigid body simulation and custom interactions.
- Level editor with draggable, rotatable, and snap-to-grid blocks.
- Local and online multiplayer with synchronized gameplay using Steam networking.
- Strategic drafting and building phases inspired by competitive board games.
- Custom shaders, animations, audio systems, and polished UI/UX.
- Save systems, settings menus, scene transitions, and progression across multiple worlds with unique mechanics.

### Technical Highlights
- Designed a modular architecture in Godot using GDScript and reusable scene components.
- Implemented multiplayer synchronization using RPCs, authority management, and state replication.
- Developed custom gameplay mechanics including gravity switching, connected blocks, timed objectives, lives, and replay systems.
- Integrated Steamworks for lobby creation, matchmaking, and multiplayer functionality.
- Optimized gameplay systems through extensive debugging, testing, and performance improvements.

**Skills Used:** Godot, GDScript, Game Development, Multiplayer Networking, Steamworks API, Physics Simulation, UI/UX Design, Software Architecture, State Synchronization, Debugging

---

## AI & Machine Learning
Projects focus on classical AI concepts and game-theoretic approaches:

**1. Search Algorithms**  
- Implemented different search algorithms using robotic simulations.

**2. Adversarial Search**  
- Minimax implementation for “good robot vs bad robot” games.

**3. Multiagent Systems & Game Theory**  
- Explored coordination and competition between multiple agents.

**4. Map Coloring / Constraint Satisfaction**  
- Implemented the Four Color Theorem as a constraint satisfaction problem.  
- Represented maps as graphs with keys: `nodes`, `edges`, `coordinates` for visualization using NetworkX.

**Skills Used:** Python, AI, Game Theory, Multiagent Systems, Graph Algorithms

---

## Deep Learning
Projects demonstrate foundational and applied neural network techniques:

- **AutoGrad Engine:** Built a `Value` class to implement backpropagation for computational graphs.  
- **Neural Network Layers:** Implemented linear layers, threshold activations, CNNs, and Transformers.  
- **Segmentation & Data Augmentation:** Applied convolutional networks for image processing tasks.

**Skills Used:** Python, Backpropagation, CNNs, Transformers, Computer Vision

---

## Algorithms
- Implemented CLRS-based algorithms: dynamic programming, graph algorithms, network flows, NP/P complexity, monoids, and learning theory.  
- Projects emphasize efficiency, correctness, and theoretical understanding.

**Skills Used:** Python, C++, Data Structures, Algorithm Design, Complexity Analysis

---

## C / C++ Projects
- **Function Plotter / Image Generator:** Parses mathematical expressions to generate pixel-based images in PNG format.  
- **Key Contributions:**  
  - Expression parsing, tree construction, pixel calculation, and color filling.  
  - Exception handling and memory management for robust rendering.  
- **Collaboration:** Designed algorithms and implemented structured handling.  

- **Puzzle Solver:** Recursive algorithms to solve sliding block puzzles.  
  - Developed helper functions to manage directions, move blocks, and verify solved states.  
  - Coordinated work between design and implementation based on team strengths.  

**Skills Used:** C++, File I/O, Parsing, Exception Handling, Memory Management, Collaborative Development

---

## Computer Systems
- **Memory & Cache Projects:**  
  - Implemented low-level memory operations: `uint256` functions, cache read/write, load/store, eviction policies.  
  - Conducted thorough unit testing and performance evaluation.

- **Chat Server:**  
  - Implemented multithreaded server with mutex and semaphore-based synchronization.  
  - Ensured deadlock-free, high-throughput concurrent messaging.  
  - Critical sections scoped to single mutexes to avoid cyclic locking and guarantee thread safety.

**Skills Used:** C++, Assembly, Multithreading, Synchronization, Unit Testing, Performance Optimization

---

## Data Science
Projects focused on statistical modeling, reinforcement learning, and video analysis.

**1. Multi-Armed Bandits**  
- Implemented standard algorithms to solve the Multi-Armed Bandits problem:  
  - epsilon-Greedy, Softmax (Boltzmann), UCB1, UCB2, Exp3, Thompson Sampling  
- Developed a Monte Carlo-based testing framework for algorithm evaluation.  
- Implemented in **Python**, **Ruby**, and **R** (R code for visualization and analysis).

**2. Spectral Action Segmentation**  
- Clustered video frames into distinct human actions.  
- Workflow involved preprocessing, feature extraction, frame cropping, and clustering using Python.  
- Produced actionable visualizations and analysis of action segments.

**Skills Used:** Python, R, Ruby, Monte Carlo Simulations, Video Processing, Clustering, Data Visualization

---

## Hackathon Project
**HoldSteady – Surgical Hand Stability Monitoring**  
- Motivation: Reduce errors caused by surgeon fatigue during long surgeries.  
- Implementation:  
  - Wristbands and an app track hand shakiness using **computer vision** and **inertial sensors**.  
  - Buddy system alerts the nurse only when hands exceed shakiness thresholds.  
- Post-surgery analysis tracks variance over time and identifies periods of high shakiness, aiding schedule optimization.

**Skills Used:** Python, Computer Vision, Sensor Integration, Real-Time Data Analysis, UX/UI Design

---

## Data Structures (Java)
- Implemented core data structures from scratch:  
  - Index Lists, Heaps, Linked Arrays, Linked Sets  
  - Trees: Binary Trees, Binary Search Trees, AVL Trees, and more  
- Focused on efficiency, correctness, and practical applications in algorithms.

**Skills Used:** Java, Data Structures, Algorithms, Object-Oriented Programming

---

## MATLAB Projects
**1. Game of Life**  
- Simulated Conway’s Game of Life and generated movies from evolving patterns using `imagesc()` and `colormap(gray)`.

**2. Gurobi Optimization (Linear Programming)**  
- Solved complex scheduling problems with constraints for sports teams.  
- Constraints included: home/away games, travel distances, consecutive game limits, holiday schedules, and hard constraints for fairness and logistics.

**Skills Used:** MATLAB, Linear Programming, Optimization, Simulation

---

## UI/UX & Fullstack
**Posts – Simplified Social Media Platform**  
- Minimalistic Twitter-like clone for educational purposes.  
- Backend (**API**) in Node.js / pnpm; Frontend (**Web App**) using modern JavaScript frameworks.  
- Features: Database setup, seeding, API calls, and local deployment.

**Local Setup:**  
1. Clone repository  
2. Navigate to `api/` → install dependencies → create database → seed → run server  
3. Navigate to `web/` → add `.env` → install dependencies → run dev server  

**Skills Used:** Fullstack Development, JavaScript, Node.js, pnpm, Database Management, Web Deployment, UI/UX

---

## Deepmreye (Brain Sciences Lab Project)
- Goal: Predict participant gaze from fMRI scans using Python.  
- Focused on neuroscience data preprocessing, modeling, and predictive analysis.

**Skills Used:** Python, fMRI Analysis, Data Modeling, Predictive Analytics, Neuroscience Research


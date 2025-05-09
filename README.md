# Implementation-of-ANC-algorithms
This project presents a comprehensive design and implementation of real-time Active Noise Cancellation systems using the Filtered-x Least Mean Square (FxLMS) algorithm across multiple platforms—MATLAB, Vitis HLS for FPGA, and a Raspberry Pi-based embedded system.
To develop, simulate, and implement ANC algorithms—primarily the FxLMS algorithm—for real-time noise reduction in audio signals using various hardware and software platforms. This project explores the trade-offs between flexibility, performance, and cost in implementing ANC systems on:
  1. MATLAB and HDL Coder for algorithm design and simulation
  2. Vitis HLS for FPGA-based hardware acceleration
  3. Raspberry Pi for low-cost, real-time embedded implementation
Background: 
Active Noise Cancellation (ANC) is a signal processing technique that reduces unwanted ambient noise by generating an anti-noise signal that is 180° out of phase with the original. The core of ANC systems is typically an adaptive filter, which continuously adjusts its parameters to minimize the error signal—the residual noise.

The Filtered-x LMS (FxLMS) algorithm is a popular choice for ANC systems, as it effectively compensates for the secondary path (speaker, amplifier, etc.) that affects the anti-noise signal.

Hardware Used: 
  1. Raspberry Pi 3B
  2. MEMS microphones
  3. Headphones
  4. USB Audio Interface
Software FPGA Boards used in Vitis for Simulation: Basys3, Zybo Z7, Zynq Ultrascale+

Software Tools:
  1. MATLAB
  2. Vitis HLS 2022.2
  3. Vivado Design Suite
  4. Python (for Raspberry Pi implementation)
  5. SSH for Raspberry Pi access

Conclusion: 
This project demonstrates a full-stack exploration of ANC systems from simulation to real-time hardware deployment. It shows the effectiveness of the FxLMS algorithm in different environments and highlights the benefits and challenges of deploying ANC systems across various platforms.
Key Takeaways:
  Raspberry Pi: Ideal for prototyping and low-cost experiments
  FPGA: Offers superior latency and real-time performance
  MATLAB: Excellent for algorithm development and testing











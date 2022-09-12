<p align="center">
<img src="https://www.digitalartsandentertainment.be/dae/images/DAE_headerLogo.png" length=20% width=20%>
</p>
<h1 align="center">CPU Raytracer</h1>
<p align="center">A CPU raytracer made during the <a href="https://digitalartsandentertainment.be/page/43/Graphics+programming+1">Graphics Programming 1</a> cource at DAE.<br>
<sub>By <a href="https://elisebriers.github.io/">Elise Briers</a></sub></p>
<p align="center">
    <a href="https://github.com/EliseBriers/Software-Raytracer/actions/workflows/msbuild.yml">
        <img src="https://github.com/EliseBriers/Software-Raytracer/actions/workflows/msbuild.yml/badge.svg">
    </a>
    <a href="https://opensource.org/licenses/MIT">
        <img src="https://img.shields.io/badge/License-MIT-yellow.svg">
    </a>
</p>

***

## Concept
We where tasked with writing a software raytracer using C++. The project featured very mathematical coding where performance was very important. I decided to challange myself and make the code relatively performant.

## Basic features
As it remains a school project, there where some feature requirements set by the teachers:
 * Moveable camera
 * Rendering of the following objects:
   * Plane
   * Sphere
   * Triabngle
   * Mesh
 * Directional light and point light
 * [PBR rendering](https://en.wikipedia.org/wiki/Physically_based_rendering)

## Extra features
I wanted to go a bit beyond the minimum asked by the teachers, partially out of personal interest and partially to teach myself valuable topics. These are the extras I wanted to implement in this project:
 * Usage of the [Pareto principle](https://en.wikipedia.org/wiki/Pareto_principle#In_computing) to optimize the code
 * Fast and bug-free multithreading using a thread-pool

## Running and using the project
The project can be build using a recent version of visual studio, I recommend running the project in release mode for better performance.

The camera can be controlled in the following way:
 * Holing the right mouse button: looking around like in a first person game
 * Holding the left mouse button: forwards/backwards movement + look left/right
 * WASD: movement
 * q/e: up and down movement

Extra controls are explained in the console.

## Conclusion
I really enjoyed working on this project. Using the 80/20 rule to optimize the code is a very setifying process that will equip me to write faster code into the future while focussing on the code that will yield most benefits. While other cources covered the topic of multithreading, they did not go nearly as deep into the topic as I did here. I learned how to write fast multithreaded code while avoiding common bugs like race conditions. This project has tought me a lot of valuable skills related to performance. I am certain these skills will be useful later on regardless of what type of project I'm working on.

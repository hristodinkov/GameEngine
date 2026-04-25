# Game engine made with OpenGL

This project is a custom ***C++/OpenG***L game engine built from scratch, featuring a Unity‑like ***Transform system*** with ***hierarchical GameObjects, parent–child relationships***, and automatic world/local matrix updates. Every object can contain models, colliders, and modular logic through a flexible ***Behavior system*** like in Unity.
The engine includes a full ***Separating Axis Theorem (SAT) collision pipeline*** combined with a high‑performance ***Spatial Hash Grid*** broadphase, enabling efficient collision detection across hundreds of convex objects. A built‑in ***benchmarking framework*** tracks FPS, frame time, SAT test counts, and SAT timings, exporting all results directly to Excel for analysis.

Rendering is handled through a programmable ***OpenGL pipeline*** with custom shaders, an FPS‑style camera, and dynamic ***ADS lighting***. The engine also supports multiple scenes, a developing ***post‑processing*** pipeline (invert, grayscale, edge detection, pixelization), and a customizable ImGui interface for real‑time debugging and visualization.

Together, these systems form a compact but fully functional engine that demonstrates core real‑time concepts — rendering, transforms, behaviors, collision detection, scene management, and performance analysis — all implemented in modern C++.

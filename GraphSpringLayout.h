#ifndef GRAPH_SPRING_LAYOUT_H
#define GRAPH_SPRING_LAYOUT_H

#include <vector>
#include <array>
#include <cmath>
#include <algorithm>
#include <cstddef>

// -----------------------------------------------------------------------------
// Minimal header-only spring (force-directed) layout for undirected graphs.
// - No external libs, works with MinGW/MSVC/Clang
// - Deterministic if you pass the same initial positions
// - Positions are updated in-place (float)
// -----------------------------------------------------------------------------
//
// Usage:
//   #include "layout/graph_spring_layout.h"
//   std::vector<std::array<float,2>> pos(n);
//   std::vector<std::pair<size_t,size_t>> edges; // (u,v), u<v
//   // fill pos with initial coordinates (e.g., circle) and edges
//   spring::Settings cfg; cfg.iterations = 200;
//   spring::run(pos, edges, cfg);
//
// After run(), write pos[i] back to your Node::setPos(pos[i][0], pos[i][1]).
//
// -----------------------------------------------------------------------------

namespace spring {

struct Settings {
    // Core parameters
    int   iterations    = 200;   // total iterations
    float step          = 0.05f; // global speed (0..1)
    float gravity       = 0.02f; // pull to origin
    float repel_scale   = 2.0f;  // K in repulsion ~ K / r^2
    float attract_scale = 1.0f;  // K in attraction ~ (r^2) / K
    // Stabilizers
    float max_step      = 5.0f;  // clamp per-iter displacement (pixels)
    float collide_dist  = 36.0f; // minimal preferred distance between nodes
    float collide_boost = 1.8f;  // extra push when r < collide_dist
    float jitter        = 0.10f; // tiny noise to avoid perfect lines (0..0.5)
    // Annealing
    bool  cool          = true;  // gradually reduce max_step to max_step*0.1
};

inline float invLen(float x, float y) {
    float d2 = x*x + y*y;
    if (d2 < 1e-6f) return 0.0f;
    return 1.0f / std::sqrt(d2);
}

// Force-directed layout (Fruchterman–Reingold style + collision + gravity)
inline void run(std::vector<std::array<float,2>>& pos,
                const std::vector<std::pair<size_t,size_t>>& edges,
                const Settings& S = {})
{
    const size_t n = pos.size();
    if (n < 2) return;

    std::vector<std::array<float,2>> disp(n, {0.f, 0.f});

    // cooling schedule
    const float maxStepStart = std::max(0.1f, S.max_step);
    const float maxStepEnd   = S.cool ? (maxStepStart * 0.1f) : maxStepStart;

    for (int it = 0; it < std::max(1, S.iterations); ++it) {
        // reset displacements
        std::fill(disp.begin(), disp.end(), std::array<float,2>{0.f, 0.f});

        // --- REPULSION + collision boost (O(n^2), fine for ~hundreds of nodes)
        for (size_t i = 0; i < n; ++i) {
            const float xi = pos[i][0], yi = pos[i][1];
            for (size_t j = i + 1; j < n; ++j) {
                float dx = xi - pos[j][0];
                float dy = yi - pos[j][1];
                float inv = invLen(dx, dy);
                if (inv == 0.0f) {
                    // tiny random nudge to break symmetry
                    dx = 0.001f; dy = 0.0f; inv = 1.0f/0.001f;
                }

                float dist   = 1.0f / inv;
                float repel  = S.repel_scale * inv * inv;   // ~ K / r^2
                if (dist < S.collide_dist) {
                    // extra boost if nodes are too close
                    repel *= (1.0f + S.collide_boost * (S.collide_dist / std::max(dist, 1.0f)));
                }

                float fx = repel * dx * inv;
                float fy = repel * dy * inv;

                disp[i][0] += fx;  disp[i][1] += fy;
                disp[j][0] -= fx;  disp[j][1] -= fy;
            }
        }

        // --- ATTRACTION along edges (Hooke-like ~ r^2 / K)
        for (const auto& e : edges) {
            const size_t u = e.first, v = e.second;
            if (u >= n || v >= n) continue;

            float dx = pos[u][0] - pos[v][0];
            float dy = pos[u][1] - pos[v][1];
            float inv = invLen(dx, dy);
            if (inv == 0.0f) { dx = 0.001f; dy = 0.0f; inv = 1.0f/0.001f; }

            float dist = 1.0f / inv;
            float attr = (dist * dist) / std::max(1e-4f, S.attract_scale);

            float fx = attr * dx * inv;
            float fy = attr * dy * inv;

            disp[u][0] -= fx; disp[u][1] -= fy;
            disp[v][0] += fx; disp[v][1] += fy;
        }

        // --- GRAVITY to origin + jitter
        for (size_t i = 0; i < n; ++i) {
            disp[i][0] -= pos[i][0] * S.gravity;
            disp[i][1] -= pos[i][1] * S.gravity;

            if (S.jitter > 0.f) {
                // very small pseudo-random nudge based on index & iter
                const float jx = ( ( (int(i*73856093u + it*19349663u) & 1023) / 512.0f ) - 1.0f ) * S.jitter;
                const float jy = ( ( (int(i*83492791u + it*2971215073u) & 1023) / 512.0f ) - 1.0f ) * S.jitter;
                disp[i][0] += jx;
                disp[i][1] += jy;
            }
        }

        // --- apply movement with clamping and global step
        const float t = (S.cool && S.iterations > 1)
                            ? (maxStepStart + (maxStepEnd - maxStepStart) * (float(it) / float(S.iterations - 1)))
                            : maxStepStart;

        for (size_t i = 0; i < n; ++i) {
            float dx = disp[i][0] * S.step;
            float dy = disp[i][1] * S.step;
            // clamp per-iteration displacement
            float len = std::sqrt(dx*dx + dy*dy);
            if (len > t) {
                const float scale = t / len;
                dx *= scale; dy *= scale;
            }
            pos[i][0] += dx;
            pos[i][1] += dy;
        }
    }
}

} // namespace spring

#endif // GRAPH_SPRING_LAYOUT_H
